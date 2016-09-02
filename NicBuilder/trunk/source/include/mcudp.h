/*
	PC based Monitoring System source file
	version 2.03
	by J.Hunter,jhunter@263.net,May 2001 
*/
#ifndef __mcudp_h__
#define __mcudp_h__

#ifdef _WIN32	
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SOCKET int
#define SOCKADDR_IN struct sockaddr_in
#endif

#include <rtknet.h>

#define WM_SOCKET_READY (WM_USER+15426)

#define IFF_DISABLED	0X80000000

#ifdef __LINUX__

typedef struct __tag_INTERFACE_INFO{
  u_long iiFlags;
  struct sockaddr iiAddress;
  struct sockaddr iiBroadcastAddress;
  struct sockaddr iiNetmask;
}INTERFACE_INFO;

#define SOCKET_ERROR   1
#define INVALID_SOCKET 1
#define ADDR_ANY  0

#endif

class RTKNET_API CMulticastSocket
{
public:
	CMulticastSocket();
	virtual ~CMulticastSocket();
	int JoinGroup(char * IP, int Port, int nTTL=255, bool bLoopback=true);
	int LeaveGroup();
 
	int Socket(int=SOCK_DGRAM);
	int Create(int port=0, int=SOCK_DGRAM);
	int Close();
	inline SOCKET Handle(){return m_hSocket;};
	int ReceiveFrom(void * buf, int len, sockaddr_in * addr);
	int ReceiveFrom(void * buf,int len,char * addr,int & port);
	int SendTo(void *buf,int len,int port,char * addr);
	int SendTo(void *buf, int len, sockaddr_in * addr);
	/* a feature only supported on Win32 */
	int SetAsynchronous();
	int Bind(int port,char * addr=0);
	int GetLocalAddress(char * ip, int ip_length, int * port);
	int LocalPort();
	void RejoinGroup();

	virtual int OnReceive(int);
	virtual int OnEvent(int,int,int);
	virtual int OnData(void * buf,int len,int port,char * src_addr);

	int SetSockOpt(int optname,const void * optval,int optlen,int level=SOL_SOCKET);
	int GetSockOpt(int optname,const void * optval,int *optlen,int level=SOL_SOCKET);
	union{
		void * m_hWnd;
		void * hEvent;
	};

protected:
	// SOCKADDR structure to hold IP/Port of the Host group to send data to it
	SOCKADDR_IN m_saHostGroup;
	SOCKADDR_IN	m_saLocal;
	SOCKET m_hSocket;

	bool m_bMulticast;
	
	// cached string address
	char group_ip[256];
	int  group_port;
	int  group_loopback;
	int  group_ttl;
	
	bool m_bReuseAddr;

	// Assume there will be no more than 10 IP interfaces
	INTERFACE_INFO m_localAddr[10];   
	int 		   m_localAddrCount; 

	bool isLocalAddress(sockaddr_in & addr);
};

CDECL_BEGIN

RTKNET_API void PMC_API get_interface_list(
	INTERFACE_INFO * addr, 
	int & count, 
	bool bSilent
	);

CDECL_END

#endif
