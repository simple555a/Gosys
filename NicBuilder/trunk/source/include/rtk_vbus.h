/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\rtk\vbus.h
	file path:	E:\vss\pmc2\src\include\rtk
	author:	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Virtual bus	support
	history:
	created:	10:11:2002   15:48
*********************************************************************/
#ifndef __rtk_vbus2_h__
#define __rtk_vbus2_h__

#ifdef _WIN32
#ifdef LIBVBUS_EXPORTS
#define VBUS_API __declspec(dllexport)
#else
#define VBUS_API __declspec(dllimport)
#endif
#else
#define VBUS_API
#endif

#ifdef __cplusplus
#include <map>
#include <list>
#endif

#include <rtk_object.h>

#include <pshpack1.h>

#define RTK_BROADCAST_ADDR	"*"

typedef struct	__tag_RTK_HOST{
	char	Data[rtk_vbus_node_key_length];
}PACKED RTK_HOST;
typedef RTK_HOST * PRTK_HOST;
typedef const RTK_HOST * PCRTK_HOST;

typedef __u32 VBUS_ID;
typedef __u16 VBUS_PORT;

#define PORT_NONE				((VBUS_PORT)0)
#define PORT_ALL					((VBUS_PORT)-1)
#define PORT_ALL_SERVER			((VBUS_PORT)1)
#define PORT_ALL_CLIENT			((VBUS_PORT)2)
#define PORT_ALL_SLAVE			((VBUS_PORT)3)
#define PORT_ALL_CONTROLLER	((VBUS_PORT)4)

typedef struct __tag_NODE_TABLE_ENTRY{
	RTK_HOST	key;
}PACKED NODE_TABLE_ENTRY, *PNODE_TABLE_ENTRY;

#include <poppack.h>

#include <rtk_socket.h>

CDECL_BEGIN

/*
	note the two flags:

	RTK_INIT_AS_SERVER
	&
	VBUS_CONNECT_AS_SERVER

	the previous one is used in init_network on the default
	buses, the behaviour of this function is that the default buses
	will be connected either as server or as client on condition
	of the specified ``networlInitFlags'', if it has RTK_INIT_AS_SERVER
	bit set, then the buses are server side, else client side.
	
	Further, once set, the buses type cannot be changed
	unless an explicit disconnect/re-connect. The later
	connect_vbus call will NOT change the established
	server/client setting!!!
*/
#define VBUS_INIT_AS_SERVER		0x00000001

VBUS_API __bool RTK_API init_vbus(
	__int networlInitFlags, 
	const char * local_addr DEFAULT_PARAM(0)
	);
VBUS_API void RTK_API uninit_vbus(void);

typedef void * HVBUS;

#define VBUS_EVT_GOT_FRAGMENT		1
#define VBUS_EVT_COLLISION			2

// callback
typedef void (RTK_API * VBUS_RECEIVE_PROC)(PCRTK_PACKET data, HVBUS bus);
typedef int (RTK_API * VBUS_EVENT_PROC)(HVBUS bus, int event, int data);

#define VBUS_CONNECT_AS_SERVER	0x00000040
#define VBUS_ACTIVE_SERVER		0x80000000

VBUS_API HVBUS RTK_API connect_vbus(
	__uint busid, 
	__uint flags,
	VBUS_RECEIVE_PROC r_proc,
	VBUS_EVENT_PROC f_proc
	);

VBUS_API void RTK_API disconnect_vbus(HVBUS channel);

VBUS_API __uint RTK_API send_rtk_packet(
	HVBUS channel,
	RTK_PACKET * packet
	);

VBUS_API __uint RTK_API broadcast_rtk_data(
	HVBUS channel,
	__u16 port,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

VBUS_API __uint RTK_API broadcast_rtk_data_with_guid(
	HVBUS channel,
	__u16 port,
	const RTK_GUID * guid,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

VBUS_API __uint RTK_API send_rtk_data(
	HVBUS channel,
	const RTK_ADDR * addr,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

VBUS_API __uint RTK_API send_rtk_data_with_guid(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)	
	);

VBUS_API __uint RTK_API send_rtk_data_with_guid_ex(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__uint packet_type,
	__u16 grp_id,
	const void * data,
	__uint data_len
);

VBUS_API int RTK_API default_server_filter(
	HVBUS b,
	int event,
	int data
	);

VBUS_API int RTK_API default_client_filter(
	HVBUS b,
	int event,
	int data
	);

VBUS_API int RTK_API set_personality(
	const char * local_addr
	);

extern VBUS_API PNODE_TABLE_ENTRY g_ThisNode;

CDECL_END

#ifdef __cplusplus

typedef CAlphaName<RTK_HOST> CHostName;

inline bool operator < (const RTK_HOST &t1, const RTK_HOST &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}

inline bool operator == (const RTK_HOST &t1, const RTK_HOST &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}

#endif

__inline void mk_bcast_addr(PRTK_HOST host)
{
	memset(host, 0, sizeof(RTK_HOST));
	strncpy(host->Data, RTK_BROADCAST_ADDR, sizeof(RTK_HOST));
}

#ifndef is_same_key
#define is_same_key(k1, k2)			\
	(!strnicmp((const char *)(k1), (const char *)(k2), sizeof(*(k1))))
#endif


/*
	classes used internally to librtknet.dll
*/

#pragma warning(disable: 4786 4275)

/*
	a receive item is an object representing the incoming
	data packets as a bunch of fragments. The fragments
	firstly belong to a group, and the groups in turn belong
	to a RECEIVE_ITEM.
*/
struct RECEIVE_ITEM{
	__uint		unreceived_fragments;
	PRTK_PACKET	header;	
	RTK_TIME	birthday;
	double		life;
	__uint		max_frgmt_id_received;
	
	// packet group, a circular linked list
	RTK_LIST_ENTRY	li;
	
	__u8		bitmap[VBUS_MAX_PACKET_SIZE>>2];

};

struct SEND_ITEM{
	PRTK_PACKET	header;
	RTK_LIST_ENTRY li;
	RTK_TIME	birthday;
	double		life;
};

#ifdef __cplusplus

// BUGBUG: Why should the CLock be here?
// BUGBUG: actually CPacketMan never use the lock itself
class CPacketMan : public CLock
{
	friend class CRtkVBus;
public:
	static __uint m_mtu;				/* maximum transmission unit */
	static __uint m_dwEffectiveLoad;	// 
	static __uint m_dwHeaderSize;

public:
	CPacketMan();
	virtual ~CPacketMan();
	
	/* packet reconstruction */
	__bool IsFragment(RTK_PACKET * packet);
	RECEIVE_ITEM * MergePacket(RTK_PACKET * input_packet);
	void DeleteReceiveItem(RECEIVE_ITEM * item);

	/* packet fragmentation */
	__bool IsFragmentationNeeded(const RTK_PACKET * input);
	SEND_ITEM * SplitPacket(const RTK_PACKET *input);
	SEND_ITEM * GetSendItem(const RTK_GUID & Guid, __uint grpId);
	virtual __uint SendPacket(PRTK_PACKET p)=0;
	void FlushQueue();
	
	void reap_packets();
	void dbg_dump();

protected:
	void * Alloc(__uint size);
	void Free(void * mem);
	
private:
	void request_packets(RECEIVE_ITEM * h);
	void	reap_received(PCRTK_TIME);
	void	reap_sent(PCRTK_TIME);	
	
	typedef std::map<RTK_GUID, RTK_LIST_ENTRY> RITEM_LIST;
	typedef std::map<RTK_GUID, RTK_LIST_ENTRY> SITEM_LIST;

	RITEM_LIST	r_lst;
	SITEM_LIST	s_lst;
};

// use 4 bytes alignment
// to keep CTransaction::m_iRefCount at dword boundary
#include <pshpack4.h>	
class VBUS_API CTransaction{
public:
	CTransaction();	
	virtual ~CTransaction();
	virtual void OnEvent(PCRTK_PACKET packet)=0;
	virtual void OnTimeout(){};	
	virtual void OnProgress(__uint total, __uint completed, PCRTK_PACKET p){};
public:
	// int		TimeOut; // in miliseconds
	RTK_GUID Guid;           //链表s_lst  <RTK_GUID, RTK_LIST_ENTRY>的键，在类CTransaction的构造函数中创建
	RTK_TIME birth;
	double	 life;
	int		 m_iRefcount;

	friend class CTransactionContainer;
	CTransactionContainer *m_Parent;
};
#include <poppack.h>

class VBUS_API CWaitableTransaction : public CTransaction{
protected:
	CEvent	Event;	 //
	__bool	m_bWaitResult;
public:
	CWaitableTransaction();
	__bool Wait(__uint TimeOut, __bool ProcessMessage = __false);
	inline __bool WaitResult()
	{
		return m_bWaitResult;
	}
	void Cancel();
	void SetEvent();
	virtual void OnTimeout()
	{
		this->SetEvent();
	}
	virtual void OnCancel(){};
	virtual ~CWaitableTransaction();
};

VBUS_API __bool RTK_API add_transaction(HVBUS, CTransaction *);

class CTransactionContainer : public CRWLock
{
private:
	typedef std::list<CTransaction *> CTList;
	CTList	tlist;
public:
	CTransactionContainer();
	virtual ~CTransactionContainer();
	void	reap();
	int 	add(CTransaction *);
	int 	remove(CTransaction *);
	int 	remove(const RTK_GUID & Guid);
	CTransaction * _borrow(const RTK_GUID & guid);
	void	_return(CTransaction *t);
};

class VBUS_API CRtkVBus : 
	public CRtkSocket,
	public CRtkThread,
	public CPacketMan
{
	friend class CRtkDebugger;
	friend VBUS_API __bool RTK_API add_transaction(HVBUS, CTransaction *);
	
public:
	int 			flag;
	__uint		busid;	
protected:

	VBUS_RECEIVE_PROC r_proc;
	VBUS_EVENT_PROC	f_proc;
	RTK_GUID	serverGuid;

	virtual __bool WillingToAccept(PCRTK_PACKET p);
public:
	CRtkVBus( 
		__u16 port, 
		__uint flags, 
		VBUS_RECEIVE_PROC r_proc, 
		VBUS_EVENT_PROC f_proc
		);
	virtual ~CRtkVBus();
	void	connect(int flag, VBUS_RECEIVE_PROC r, VBUS_EVENT_PROC f);

	virtual __uint	run();
	virtual void	OnPacket(RTK_PACKET * packet, int len);
	virtual __uint	SendPacket(PRTK_PACKET packet);
	
protected:
    __bool	init();
    __bool	do_socket_io();
    
	__uint SendFragment(PRTK_PACKET fragment);
	void handlePacket(PRTK_PACKET p);
	void	ResendFragments(PCRTK_PACKET src);
	void claimServer();
	
	// class 'CPacketMan' needs to have dll-interface to be used by clients of....
	#pragma warning(disable : 4251)
	class CTransactionContainer broker;
	friend struct CPacketIoProcessor;
	RTK_HANDLE hIoProcessor;
	__u16	m_iRtkPort;
};

#endif /* c plus plus */

#include <pshpack1.h>
typedef struct __tag_VBUS_DATA{
	__uint		size;
	VBUS_ID 	id;
	__uint		in_packets;
	__uint		out_packets;
	__uint		duplicated_packets;
	__i64		in_bytes;
	__i64		out_bytes;
	double		total_delay;
	struct sockaddr_in localAddr;
	struct sockaddr_in peerAddr;
	RTK_GUID	serverGuid;
}PACKED VBUS_DATA, *PVBUS_DATA;
typedef const VBUS_DATA * PCVBUS_DATA;
#include <poppack.h>

VBUS_API __bool RTK_API get_vbus_data(
	HVBUS handle, 
	PVBUS_DATA data
	);

VBUS_API __u16 RTK_API alloc_vbus_port();

VBUS_API __u16 RTK_API get_vbus_port(
	HVBUS handle
	);

VBUS_API __u16 RTK_API set_vbus_port(
	HVBUS handle,
	__u16 newPort
	);

VBUS_API __uint RTK_API get_vbus_flag(
	HVBUS handle
	);

VBUS_API __uint RTK_API set_vbus_flag(
	HVBUS handle,
	__uint flag
	);

VBUS_API __uint RTK_API reset_vbus_flag(
	HVBUS handle,
	__uint flag
	);

#endif
