/*
    PC based Monitoring System source file
    version 2.03
    by J.Hunter,jhunter@263.net,May 2001 
*/
#ifndef __rtk_tcp_h__
#define __rtk_tcp_h__

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
#define SOCKADDR_IN sockaddr_in
#endif

class RTKNET_API CTcpSocket
{
public:
    CTcpSocket();
    virtual ~CTcpSocket();
 
    int Socket();
    inline SOCKET Handle(){return m_hSocket;};
	int Receive(void * buf, int len);
    int Send(void *buf,int len);

    /* a feature only supported on Win32 */
    int SetAsynchronous();
    int Bind(int port, char * addr=0);
    int LocalPort();

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
	char group_ip[256];
	int  group_port;
	int  group_loopback;
	int  group_ttl;
	bool m_bReuseAddr;

	// Assume there will be no more than 10 IP interfaces
	INTERFACE_INFO m_localAddr[10];   
	int            m_localAddrCount; 
};

CDECL_BEGIN

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

CDECL_END

#endif
