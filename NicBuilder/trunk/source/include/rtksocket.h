/*
	PC based Monitoring System source file
	version 2.00.38
	by J.Hunter,jhunter@263.net,May 2001 

	May 20, 2001:
		added redundant packet disgarding algorithm
	May 20, 2003
		documentation revised, vbus-related stuff moved out of CRtkSocket
	layer.
*/
#ifndef __rtksocket_h__
#define __rtksocket_h__

#include <mcudp.h>
#include <rtknet.h>
#include <utils.h>

/*
	This class overides the OnReceive() method of its base class.

	Each CRtkSocket object created in the RTK library(librtknet.dll)
	provides a communication CHANNEL to the upper layer software.

	Of course u can also create ur own CRtkSocket object, but it's
	not recommended, reason? read on and see.

	CRtkSocket object overrides the SendTo method of CMulticastSocket,
	when sending, it put outgoing packet in every available interfaces
	on the local machine, that is, if the node has two or more network 
	cards, the outgoing messages will be duplicated.

	CRtkSocket object recognizes the predefined RTK_PACKET structure,
	and it automatically disgards duplicated packets sent by any
	multi-hosted nodes on the network. It does this through caching
	guids of upto cached_packages_guids(currently 16) entries of the
	most recently received packets on this channel.

	CRtkSocket supports transaction-based asynchronized mode. It
	dispatches incoming packets to any ``CTransaction'' object who has
	claimed ownership of the incoming RTK_GUID. If no such object
	was found, the virtual function ``OnPacket()'' was called to process 
	this packet.

	So u can c, it's not encouraged to derive new classes from CRtkSocket,
	because it's already highly specialized. In most cases u might
	want to write new classes from CMulticastSocket directly.
*/

#include <pshpack1.h>
struct PACKET_ID{
	RTK_GUID	guid;
	RTK_ADDR	src;
	__u16		grp_id;
	__u16		frgmt_id;
};

#include <poppack.h>

struct RTKNET_API CRtkSocket : public CMulticastSocket
{
public:
	CRtkSocket();
	CRtkSocket(__uint port, const __u8 * mcast_addr);
	virtual ~CRtkSocket();
	virtual int OnReceive(int nErrorCode);
	__uint RemotePort();
	int Multicast(void * buf,int len,int port);

	inline virtual void OnPacket(RTK_PACKET * packet, int /* len */){};
protected:
	virtual __bool WillingToAccept(PCRTK_PACKET p){
		return __true;
	}
	enum {
		cached_packets=16,
	};
	/* a ring buffer */
	PACKET_ID	cached_packet_ids[cached_packets];
	int			head;
	sockaddr_in	peerAddress;

	char		buffer[MAX_PACKET_SIZE];
public:	
	RTK_TIME	cached_packet_times [cached_packets];
	__uint		in_packets, out_packets, duplicated_packets;
	__i64		in_bytes, out_bytes;
	double		total_delay;
};

#endif

