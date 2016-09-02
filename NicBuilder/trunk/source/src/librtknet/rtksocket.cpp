/*
  PC based Monitoring System source file
  version 1.0
  by J.Hunter,jhunter@263.net,May 2001 
*/
#include "precomp.h"
#pragma hdrstop

CRtkSocket::CRtkSocket()
{	
	head = 0;
	in_packets = out_packets = duplicated_packets = 0;
	in_bytes = out_bytes = 0;
	memset(cached_packet_ids, -1, sizeof(cached_packet_ids));
	total_delay=0;
}

CRtkSocket::~CRtkSocket()
{
}

int CRtkSocket::OnReceive(int nErrorCode)
{
	if(nErrorCode) {
		utils_error(
			"Error %d occured in net-module chnl-%d.\n", 
			nErrorCode, 
			RemotePort()
			);
		return 0;
	}

	int i;
	int  idx;

	memset(buffer, 0, sizeof(buffer));
	int res = ReceiveFrom(buffer, sizeof(buffer), &peerAddress);
	
	if(res < sizeof(RTK_PACKET) - 1) {
		return 0;
	}

	RTK_PACKET * p=(RTK_PACKET*)buffer;

	if(!WillingToAccept(p)){
		return 0;
	}

	in_packets++;
	in_bytes += res;

	if(res < sizeof(RTK_PACKET)-1){
		return 0;
	}
	
	/* check if this was a redundancy packet */
	PACKET_ID pid;
	pid.guid = p->guid;
	pid.src = p->src;
	pid.grp_id = p->grp_id;
	pid.frgmt_id = p->frgmt_id;
	for(i=0; i<cached_packets; i++){
		idx = head-1+i;
		if(idx<0) {
			idx += cached_packets;
		}
		if(idx >= cached_packets) {
			idx -= cached_packets;
		}
		if(!memcmp(&cached_packet_ids[idx], &pid, sizeof(pid))){
			break;
		}
	}

	/* if it is, drop it */
	if(i < cached_packets){
		RTK_TIME t1;
		double   delay;
		rtk_time_mark(&t1);
		delay = rtk_time_diff(&t1, &cached_packet_times[idx])*1000;
#ifndef NDEBUG
		char guid_name[128];
		p->guid.to_string(guid_name);	       
		DEBUG_PRINTF((
			"Packet duplicated, ID : %s:%d/%d/%d, Src=%s:%d delayed by %.3f\n",
			guid_name, 
			p->grp_id,
			p->frgmt_id, 
			p->total_frag, 
			(char*)CHostName(p->src.host), 
			p->src.port, 
			delay
			));
#endif
		duplicated_packets++;
		total_delay+=delay;
		return 0;
	}
	/* if not, add its id to cache, then process it*/
	cached_packet_ids[head] = pid;
	rtk_time_mark(&cached_packet_times[head]);
	head++;
	if(head == cached_packets) head=0;

	OnPacket((RTK_PACKET*)buffer, res);

	return res;
}

/*
  Returns :
  bytes sent: if OK
  0	    : if failed
*/
int CRtkSocket::Multicast(void *buf, int len, int port)
{
	int i;
	int count, countmax; 

	if(!m_localAddrCount){
		return 0;
	}	
    
	count = countmax = 0;    

	for(i=0; i<m_localAddrCount; i++){
	// for(i=0; i<1; i++){
		if(! (m_localAddr[i].iiFlags & IFF_UP) ){
			continue;
		}
		sockaddr_in if_addr = *((sockaddr_in*)&m_localAddr[i].iiAddress);
		count = SetSockOpt(
			IP_MULTICAST_IF,
			(char*)&if_addr.sin_addr, sizeof(sockaddr_in), IPPROTO_IP
			);
		if(SOCKET_ERROR == count){
			wperror("MULTICAST_IF");
			continue;
		}
#define max_retry	3
		for(int j=0; j<max_retry; j++){			
			count = sendto(
				m_hSocket,
				(const _SOCKBUF)buf,
				len,
				0,
				(sockaddr*)&m_saHostGroup,
				sizeof(m_saHostGroup)
				);
			if( count != SOCKET_ERROR){
				countmax = countmax<count? count : countmax;
				break;
			}else{			
#ifndef UTILS_NODEBUG
				if(j < max_retry-1){
					utils_debug(
						">> NET ERROR %d, resend NO.%d.\n", 
						GetLastError(), 
						j+1
						);
				}
#endif
		
				rtk_sleep(10);
			}			
		}
		if(SOCKET_ERROR == count){
#ifndef UTILS_NODEBUG
			wperror("SendTo");
#endif
		}
		// 2003/7/14, Jackie, another delay should be introduced
		// when switching interfaces
		rtk_sleep(10);
	}
    
	out_packets++;
	out_bytes += countmax;
	// DEBUG_PRINTF(("%d bytes multicasted.\n", countmax));
	return countmax;
}

__uint CRtkSocket::RemotePort()
{
	return ntohs(peerAddress.sin_port);
}

