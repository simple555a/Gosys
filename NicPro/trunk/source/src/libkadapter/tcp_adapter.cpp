/*
	Tcp Adapter
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"
#pragma hdrstop

#ifdef _WIN32
#include <winsock2.h>
#endif

#include <time.h>
#include <zlib.h>

#define LOCK(a)		((CLock*)a->m_lock)->Lock()
#define UNLOCK(a)	((CLock*)a->m_lock)->UnLock()

// #define IS_INVALID_SOCKET(s) (INVALID_SOCKET == s)

static __bool recvPacket(
	IKAdapter * k, 
	F8_SOCKET s, 
	char * buf, 
	int left, 
	int timeout,
	int startIndicator,
	int endIndicator
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	clock_t	t1, t2;
	__bool bReportStatus = __false;
	int totalBytes, percent;

	t1 = clock();
	if(left > 10240){
		bReportStatus = __true;
		totalBytes = left;
	}
	while(left && timeout > 0){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		select(s + 1, &sset, 0, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			DEBUG_PRINTF(("recvPacket timeout.\n"));
			return __false;
		}
		bytes = recv(s, (char*)buf, left, 0);
		if(bytes < 0){
#ifdef _WIN32
			DEBUG_PRINTF((
				"recv failed in tcp adapter, code %d\n", 
				WSAGetLastError()
				));
#endif
			return __false;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		timeout -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
		t1 = t2;
		if(bReportStatus){
			percent = (totalBytes - left) * (endIndicator - startIndicator) 
				/ totalBytes + startIndicator ;
			IKAdapter_queue_event(k, AE_IN_PROGRESS, percent, 0);
		}
	}
	if(left){
		return __false;
	}
	return __true;
}

static __bool sendPacket(
	IKAdapter * k,
	F8_SOCKET s, 
	const char * buf, 
	int left, 
	int timeout,
	int startIndicator,
	int endIndicator
	)
{
	int bytes;
	fd_set sset;
	struct timeval tout;
	clock_t	t1, t2;
	__bool bReportStatus = __false;
	int totalBytes, percent;
	
	t1 = clock();
	if(left > 10240){
		bReportStatus = __true;
		totalBytes = left;
	}
	while(left && timeout > 0){
		FD_ZERO(&sset);
		FD_SET(s, &sset);
		tout.tv_sec = 0;
		tout.tv_usec = timeout * 1000;
		select(s + 1, 0, &sset, 0, &tout);
		if(!FD_ISSET(s, &sset)){
			DEBUG_PRINTF(("sendPacket timeout.\n"));
			return __false;
		}
		bytes = send(s, buf, left, 0);
		if(bytes < 0){
#ifdef _WIN32
			DEBUG_PRINTF((
				"send failed in tcp adapter, code %d\n", 
				WSAGetLastError()
				));
#endif
			return __false;
		}
		left -= bytes;
		buf += bytes;
		t2 = clock();
		timeout -= (t2 - t1) * 1000 / CLOCKS_PER_SEC;
		t1 = t2;
		if(bReportStatus){
			percent = (totalBytes - left) * (endIndicator - startIndicator) 
				/ totalBytes + startIndicator ;
			IKAdapter_queue_event(k, AE_IN_PROGRESS, percent, 0);
		}
	}
	return __true;
}

static __bool skipPacket(F8_SOCKET s, __uint bytes)
{	
	__uint sbytes;
	static char foo[10240];

	printf("Skipping %d bytes\n", bytes);
	
	sbytes = sizeof(foo);
	if(sbytes > bytes){
		sbytes = bytes;
	}

	while(sbytes > 0){
		if(recv(s, foo, sbytes, 0) <= 0){
			return __false;
		}
		bytes -= sbytes;
		sbytes = sizeof(foo);
		if(sbytes > bytes){
			sbytes = bytes;
		}
	}

	return __true;
}

/*
	go offline without lock
*/
static f8_status ITcpAdapter_offline_nolock(ITcpAdapter * __this)
{
	IKAdapter * _this = __ucast__(ITcpAdapter, IKAdapter, __this);
	
	if(!IS_INVALID_SOCKET(__this->m_socket)){
		closesocket(__this->m_socket);
		__this->m_socket = INVALID_SOCKET;
	}
	_this->flags &= ~ADAPTER_F_ONLINE;

	IKAdapter_queue_event(_this, AE_STATUS_CHANGED, 0, 0);
	
	return F8_SUCCESS;
}

static f8_status ITcpAdapter_online(IKAdapter * _this, const char * url, __uint to)
{
	ITcpAdapter * __this;
	f8_status	retcode;
	struct sockaddr_in peer;
	fd_set sset;
	struct timeval timeout;
	int ret;
	char * hostAddr = 0;

	
	__this = __dcast__(IKAdapter, ITcpAdapter, _this);
	
	if(!__this->m_lock){
		return F8_ADAPTER_NO_LOCK;
	}
	
	LOCK(__this);

	memset(&peer, 0, sizeof(peer));
	peer.sin_family = AF_INET;
	const char * colon;
	colon = strchr(url, ':');
	if(colon){
		hostAddr = strdup(url);
		hostAddr[strlen(url) - strlen(colon)] = 0;
		peer.sin_port = htons(atoi(hostAddr + (strlen(url) - strlen(colon)) + 1));
	}else{
		hostAddr = (char *)url;
		peer.sin_port = htons(F8_CFG_TCP_PORT);
	}
	peer.sin_addr.s_addr = inet_addr(hostAddr);
	if(peer.sin_addr.S_un.S_addr == INADDR_NONE){
		struct hostent * he;
		he = gethostbyname(hostAddr);
		if(!he){
			retcode = F8_INVALID_ADDRESS;
			goto __failed;
		}
		peer.sin_addr = *((struct in_addr*)he->h_addr);		
	}

	ITcpAdapter_offline_nolock(__this);

	__this->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(IS_INVALID_SOCKET(__this->m_socket)){
		retcode = F8_NO_SOCKET;
		goto __failed;
	}

	/* put the socket into non-blocking mode*/
#ifdef _WIN32
	{
		unsigned long trueVal = 1;
		ioctlsocket(__this->m_socket, FIONBIO, &trueVal);
	}
#else
	fcntl(
		__this->m_socket, 
		F_SETFL, 
		fcntl(__this->m_socket, F_GETFL, 0) | O_NONBLOCK
		);
#endif

	utils_debug("F8 - connecting to %s\n", url);
	FD_ZERO(&sset);
	FD_SET(__this->m_socket, &sset);
	timeout.tv_sec = 0;
	timeout.tv_usec = to * 1000;
	connect(__this->m_socket, (struct sockaddr*)&peer, sizeof(peer));

	ret = select(__this->m_socket + 1, NULL, &sset, NULL, &timeout);
	if(FD_ISSET(__this->m_socket, &sset)){
		utils_debug(
			"F8 device connected on %s:%d\n",
			url,
			F8_CFG_TCP_PORT
			);
	}else{
		closesocket(__this->m_socket);
		__this->m_socket = INVALID_SOCKET;
		retcode = F8_NO_CONNECTION;
		goto __failed;
	}

	_this->flags |= ADAPTER_F_ONLINE;
	retcode = F8_SUCCESS;
		
	IKAdapter_queue_event(_this, AE_STATUS_CHANGED, 0, 0);
	
__failed:
	UNLOCK(__this);
	if(hostAddr && hostAddr != url){
		free(hostAddr);
	}
	return retcode;;
}

/*
	go offline
*/
static f8_status ITcpAdapter_offline(IKAdapter * _this)
{
	ITcpAdapter * __this = __dcast__(IKAdapter, ITcpAdapter, _this);

	if(!__this->m_lock){
		return F8_ADAPTER_NO_LOCK;
	}

	LOCK(__this);

	ITcpAdapter_offline_nolock(__this);

	UNLOCK(__this);
	
	return F8_SUCCESS;
}

#define _handle_error()\
do{\
	ITcpAdapter_offline_nolock(__this);\
	cmd->retcode = F8_CONNECTION_LOST;\
	goto __failed;\
}while(0)

/*	
	ITcpAdapter::exec_remote

	Execute a configuration command remotely.

	2005-8-19

	Note, the timeout value is automatically picked, assuming a
	64KBps transfer rate.
*/
static void ITcpAdapter_exec_remote(IKAdapter *_this, struct kern_conf_cmd * cmd, __uint timeout)
{
	ITcpAdapter * __this;
	struct kern_conf_cmd ack;
	__uint to;
	unsigned char *buf2=0;
	unsigned long len;
	void *savedIbuf=cmd->ibuffer;
	
#define EXPECTED_BPS (64*1024)

	__this = __dcast__(IKAdapter, ITcpAdapter, _this);

	if(!_online(_this)){
		cmd->retcode = F8_ADAPTER_OFFLINE;
		return;
	}

	if(!__this->m_lock){
		cmd->retcode = F8_ADAPTER_NO_LOCK;
		return;
	}

/* auto compression */
	if(cmd->isize > 4096){
		// the estimated compressed size is double the original size
		// just in case the things goes worse
		buf2=(unsigned char*)malloc(cmd->isize*2);
		if(buf2){
			len=cmd->isize*2-sizeof(f8_u32);
			if(compress(buf2+sizeof(f8_u32),&len,(unsigned char*)cmd->ibuffer,cmd->isize) == Z_OK){
				*((f8_u32*)buf2)=cmd->isize;
				cmd->isize=len+sizeof(f8_u32);
				cmd->ibuffer=buf2;
				cmd->cmd |= F8CMD_Compressed_Buffer;
			}else{
				// compression failed
			}
		}
	}
	
	LOCK(__this);

	/* send this command packet to peer */
	if(!sendPacket(
		_this,
		__this->m_socket, 
		(const char *)cmd, 
		sizeof(*cmd), 
		(int)timeout,
		0,
		10
		)){
		_handle_error();
	}
	if(cmd->isize){
		to = cmd->isize * 1000 / EXPECTED_BPS;
		if(to < timeout){
			to = timeout;
		}
		if(!sendPacket(
			_this,
			__this->m_socket, 
			(const char *)cmd->ibuffer, 
			cmd->isize,
			(int)timeout,
			10,
			100
			)){
			_handle_error();
		}
	}

	if(buf2){
		free(buf2);
		buf2=0;
		cmd->cmd &= ~F8CMD_Compressed_Buffer;
		cmd->ibuffer=savedIbuf;
	}
	
	if(!recvPacket(
		_this,
		__this->m_socket, 
		(char *)&ack, 
		sizeof(ack),
		(int)timeout,
		0,
		10
		)){
		_handle_error();
	}

	if(cmd->cmd != (ack.cmd & 0xff)){
		_handle_error();
	}
	
	if(cmd->osize < ack.osize){
		cmd->retcode = F8_BUFFER_TOO_SMALL;
		skipPacket(__this->m_socket, ack.osize);
		goto __failed;
	}

	if(ack.osize > 0){
		to = ack.osize * 1000 / EXPECTED_BPS;
		if(to < timeout){
			to = timeout;
		}

		if(ack.cmd & F8CMD_Compressed_Buffer){
			buf2=(unsigned char *)malloc(ack.osize);
			if(!buf2){
				cmd->retcode=F8_LOW_MEMORY;
				goto __failed;
			}		
		}else{
			buf2=(unsigned char *)cmd->obuffer;
		}
		if(!recvPacket(
			_this,
			__this->m_socket, 
			(char*)buf2, 
			ack.osize,
			(int)timeout,
			10,
			100
			)){
			_handle_error();
		}

		if(ack.cmd & F8CMD_Compressed_Buffer){
			// auto de-compression
			len=*((f8_u32*)buf2);
			if(uncompress((unsigned char*)cmd->obuffer, &len, buf2+sizeof(f8_u32), ack.osize) == Z_OK){
				ack.osize=len;
			}else{
				cmd->retcode=F8_INVALID_COMPRESSED_DATA;
				goto __failed;
			}
		}
	}

	cmd->osize = ack.osize;
	cmd->retcode = ack.retcode;
	OLD_ID(cmd) = OLD_ID(&ack);
	NEW_ID(cmd) = NEW_ID(&ack);

__failed:
	UNLOCK(__this);

	if(buf2 && buf2 != cmd->obuffer){
		free(buf2);
	}
}

__define_vtbl__(ITcpAdapter)
__define_vtbl_end__(ITcpAdapter)

__global_constructor__(ITcpAdapter)
{
	__v_copy__(ITcpAdapter, IKAdapter);
	__override__(ITcpAdapter, IKAdapter, online);
	__override__(ITcpAdapter, IKAdapter, offline);
	__override__(ITcpAdapter, IKAdapter, exec_remote);
}

__constructor__(ITcpAdapter)
{
	IKAdapter *b;
	if(__this){
		__touch__();
		__create_base__(ITcpAdapter, IKAdapter, __this);
		b = __ucast__(ITcpAdapter, IKAdapter, __this);
		b->type = AT_TCP_ADAPTER;
#ifdef _WIN32
		__this->m_event = 0;
#endif
		__this->m_lock = (void *)new CLock();
	}
	return __this;
}

__destructor__(ITcpAdapter)
{
	IKAdapter * ka;
	if(__this){
		ka = __ucast__(ITcpAdapter, IKAdapter, __this);
		ITcpAdapter_offline(ka);
		__destroy_base__(ITcpAdapter, IKAdapter, __this);
		if(__this->m_lock){
			delete (CLock*)__this->m_lock;
			__this->m_lock = 0;
		}
	}
	return __this;
}
