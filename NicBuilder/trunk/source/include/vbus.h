/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\rtk\vbus.h
	file path:	E:\vss\pmc2\src\include\rtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Virtual bus	support
	history:
	created:	10:11:2002   15:48
*********************************************************************/
#ifndef __rtk_vbus_h__
#define __rtk_vbus_h__

#include <map>
#include <powermgr.h>

#include <object.h>
#include <rtksocket.h>
#include <packman.h>
#include <transaction.h>
#include <rtknet.h>

#ifdef LIBRTKNET_EXPORTS
#define CRTKVBUS_DEFINE
#else
#ifdef LIBTIMENET_EXPORTS
#define CRTKVBUS_DEFINE
#endif
#endif

#ifdef CRTKVBUS_DEFINE
/*
	classes used internally to librtknet.dll
*/

class CTransactionContainer : public CRWLock
{
public:
	typedef std::list<CTransaction *> CTList;
	CTList	tlist;
	CTransactionContainer();
	virtual ~CTransactionContainer();
	void	reap();
	int 	add(CTransaction *);
	int 	remove(CTransaction *);
	int 	remove(const RTK_GUID & Guid);
	CTransaction * _borrow(const RTK_GUID & guid);
	void	_return(CTransaction *t);
};

// class 'CPacketMan' needs to have dll-interface to be used by clients of....
#pragma warning(disable : 4251)
class RTKNET_API CRtkVBus : 
	public CRtkSocket,
	public CRtkThread,
	public CPacketMan,
	public CPowerAware
{
	friend class CRtkDebugger;
	friend RTKNET_API __bool PMC_API add_transaction(HVBUS, CTransaction *);
	friend struct CPacketIoProcessor;
	friend extern void net_debug();
	
public:
	int 			flag;    // server of client
	__uint		busid;	      // classic: alarm/system/config/......
protected:

	VBUS_RECEIVE_PROC r_proc;   // function pointer for receive packet
	VBUS_EVENT_PROC	f_proc;     // function pointer for event(or send packet)???
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
	virtual __bool OnPowerStateChanged(int ns);

protected:
    __bool	init();
    __bool	do_socket_io();
    
	__uint SendFragment(PRTK_PACKET fragment);
	void	ResendFragments(PCRTK_PACKET src);
	void	OnSecond();
	void claimServer();
	void handlePacket(PRTK_PACKET p);
	
	class CTransactionContainer broker;
	__u16	m_iRtkPort;
	HANDLE hIoProcessor;
};

#endif

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
	sockaddr_in localAddr;
	sockaddr_in peerAddr;
	RTK_GUID	serverGuid;
}PACKED VBUS_DATA, *PVBUS_DATA;
typedef const VBUS_DATA * PCVBUS_DATA;
#include <poppack.h>

RTKNET_API __bool PMC_API get_vbus_data(
	HVBUS handle, 
	PVBUS_DATA data
	);

RTKNET_API __u16 PMC_API alloc_vbus_port();

RTKNET_API __u16 PMC_API get_vbus_port(
	HVBUS handle
	);

RTKNET_API __u16 PMC_API set_vbus_port(
	HVBUS handle,
	__u16 newPort
	);

RTKNET_API __uint PMC_API get_vbus_flag(
	HVBUS handle
	);

RTKNET_API __uint PMC_API set_vbus_flag(
	HVBUS handle,
	__uint flag
	);

RTKNET_API __uint PMC_API reset_vbus_flag(
	HVBUS handle,
	__uint flag
	);

#endif
