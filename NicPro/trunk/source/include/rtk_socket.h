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
#ifndef __rtk_socket_h__
#define __rtk_socket_h__

#include <utils.h>

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

#ifdef __cplusplus

class VBUS_API CMulticastSocket
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

#endif	/* c plus plus */

CDECL_BEGIN

VBUS_API void RTK_API get_interface_list(
	INTERFACE_INFO * addr, 
	int * count, 
	__bool bSilent
	);

CDECL_END

/*
	This class overides the OnReceive() method of its base class.

	Each CRtkSocket object created in the RTK library(librtksocket.dll)
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

typedef struct	__tag_RTK_ADDR{
	RTK_HOST	host;
	__u16		port;
}PACKED RTK_ADDR, *PRTK_ADDR;
typedef const RTK_ADDR *PCRTK_ADDR;

typedef struct __tag_RTK_PACKET{
	// header begins
	__u32		version;		// 0x0, must be equal to RTK_VERSION
							// for reliable communication between different
							// versions of RTK interfaces on the network
	RTK_ADDR	src, dest;	// 0x4, sender & receiver in rtk-addr form
	RTK_GUID	guid;		// 0x28, uniquely identifies a transaction	  
							// added May 21, 2001
	__u16		frgmt_id;	// 0x38, together with Guid and grpid, uniquely identifies a packet
	__u16		total_frag;	// 0x3a, number of total segments
	__u32		offset;		// 0x3c, data offset of this fragment into the big packet
	__u32		data_size;	// 0x40, data size of this fragment

	__u32		packet_type;	// 0x44, flags and type
	__u16		grp_id;		// 0x48, group id
	// header ends
	char	 data[1];				// 0x4a, effective load
}PACKED RTK_PACKET, *PRTK_PACKET;
typedef const RTK_PACKET * PCRTK_PACKET;

/*
	the most significant 8 bits of the packet_type field
	is used as a 8 flags.
*/
#define PF_ACK				0x80000000	// acknowledge packet
#define PF_Failed				0x40000000	// 

/*
	The flags below are used internally, never set this flag when transmitting.
*/
#define PF_ReSend			0x20000000
#define PF_PacketGroup		0x10000000

#define PF_Compressed		0x08000000

#define PACKET_TYPE_MASK	0x00ffffff
#define PACKET_FLAG_MASK	0xff000000

#define RTK_FAILED(p)	((p)->packet_type & PF_Failed)
#define PACKET_TYPE(p)	((p)->packet_type & PACKET_TYPE_MASK)
#define PACKET_FLAGS(p)	((p)->packet_type & PACKET_FLAG_MASK)

//
//
//
// -------------- 0) packet types used by VBUS protocols ------------------
//1 Never build packets of these types, it will confuse the VBUS transport layer
//
//
//
#define PT_RequestFragment	0x00800000
#define PT_ResendFragment	0x00800001
#define PT_Ping				0x00800002
#define PT_ServerClaim			0x00800003
#define PT_Debug	0x00800004

struct PACKET_ID{
	RTK_GUID	guid;
	RTK_ADDR	src;
	__u16		grp_id;
	__u16		frgmt_id;
};

#include <poppack.h>

#ifdef __cplusplus

struct VBUS_API CRtkSocket : public CMulticastSocket
{
public:
	CRtkSocket();
	CRtkSocket(__uint port, const __u8 * mcast_addr);
	virtual ~CRtkSocket();
	virtual int OnReceive(int nErrorCode);
	__uint RemotePort();
	int Multicast(void * buf, int len, int port);

	inline virtual void OnPacket(PRTK_PACKET packet, int /* len */){};
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

	char		buffer[VBUS_MAX_PACKET_SIZE];
public:	
	RTK_TIME	cached_packet_times [cached_packets];
	__uint		in_packets, out_packets, duplicated_packets;
	__i64		in_bytes, out_bytes;
	double		total_delay;
};

#endif	/* cplusplus */


#endif

