/*
	PC based Monitoring System source file
	version 2.03
	by J.Hunter, jhunter@tsinghua.edu.cn, May 2001

	Network layer for Real-Time kernel

	Network layer provides a connection-less, unreliable communication
	interface. The other part of this software is assured to use this
	this interface to contact with any other interface in the network,
	at any time, on any topic and using any contents.

	The network layer is characterized by the VBUS transport protocol,
	VBUS is an protocol with 
	*) hand-shaking
	*) error detection
	*) flow control
	*) broadcast and multicast support
	it's designed specifically for use in industrial environment.

	May 21,2001:
		added RTK_PACKET::Index field to support unique identification
	of network packet, which is crucial to redundant packet disgarding.
	
	Sept, 2002
		major modification to version 2.0, VBUS 1.0 introduced.

	Sept, 2004
		VBUS event report interface added, its first use is to detect address
	collision.
*/
#ifndef __rtknet_h_
#define __rtknet_h_

#include <pmc_config.h>
#include <librtk.h>
#include <rtk_compat.h>

#ifdef _WIN32
	#ifdef LIBRTKNET_EXPORTS
		#define RTKNET_API		__declspec(dllexport)
	#else
		#define RTKNET_API		__declspec(dllimport)
	#endif	
#else
	#define RTKNET_API
#endif

#include <pshpack1.h>

#define RTK_BROADCAST_ADDR	"*"

typedef struct	__tag_RTK_HOST{
	char	Data[rtkm_node_key_length];
}PACKED RTK_HOST;
typedef RTK_HOST * PRTK_HOST;
typedef const RTK_HOST * PCRTK_HOST;

typedef __u32 VBUS_ID;
typedef __u16 VBUS_PORT;

#define BUSID_NONE			0
#define BUSID_ALARM			5		/* alarm raising */
#define BUSID_RTDB			6		/* read/write data */	 
#define BUSID_SYSTEM		4		/* system command */	
#define BUSID_OPERATION		2		/* write device */
#define BUSID_CONFIG			1		/* online configuration */
// above buses are auto-created buses
#define BUSID_HISLOG			7
#define BUSID_EVENTLOG		8
#define BUSID_F8CFG			9

#define BUSID_USER			64	   /* user defined bus id */

#define PORT_NONE				((VBUS_PORT)0)
#define PORT_ALL					((VBUS_PORT)-1)
#define PORT_ALL_SERVER			((VBUS_PORT)1)
#define PORT_ALL_CLIENT			((VBUS_PORT)2)
#define PORT_ALL_SLAVE			((VBUS_PORT)3)
#define PORT_ALL_CONTROLLER	((VBUS_PORT)4)

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

typedef struct __tag_NODE_TABLE_ENTRY{
	RTK_HOST	key;
}PACKED NODE_TABLE_ENTRY, *PNODE_TABLE_ENTRY;

#include <poppack.h>

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

	So if you'd like to override this default behaviour, set
	the RTK_SKIP_STANDARD_BUSES bit, then call connect_vbus
	with the desired flag manually.
*/
#define RTK_INIT_AS_SERVER			0x00000001
#define RTK_SKIP_STANDARD_BUSES	0x00000002

RTKNET_API __bool PMC_API init_network(
	__int networlInitFlags, 
	const char * local_addr DEFAULT_PARAM(0)
	);
RTKNET_API void PMC_API uninit_network(void);

typedef void * HVBUS;

#define VBUS_EVT_GOT_FRAGMENT		1
#define VBUS_EVT_COLLISION			2

// callback
typedef void (PMC_API * VBUS_RECEIVE_PROC)(PCRTK_PACKET data, HVBUS bus);
typedef int (PMC_API * VBUS_EVENT_PROC)(HVBUS bus, int event, int data);

#define VBUS_CONNECT_AS_SERVER	0x00000040
#define VBUS_ACTIVE_SERVER			0x80000000

RTKNET_API HVBUS PMC_API connect_vbus(
	__uint busid, 
	__uint flags,
	VBUS_RECEIVE_PROC r_proc,
	VBUS_EVENT_PROC f_proc
	);

RTKNET_API void PMC_API disconnect_vbus(HVBUS channel);

RTKNET_API __uint PMC_API send_rtk_packet(
	HVBUS channel,
	RTK_PACKET * packet
	);

RTKNET_API __uint PMC_API broadcast_rtk_data(
	HVBUS channel,
	__u16 port,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

RTKNET_API __uint PMC_API broadcast_rtk_data_with_guid(
	HVBUS channel,
	__u16 port,
	const RTK_GUID * guid,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

RTKNET_API __uint PMC_API send_rtk_data(
	HVBUS channel,
	const RTK_ADDR * addr,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)
	);

RTKNET_API __uint PMC_API send_rtk_data_with_guid(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__u32 packet_type DEFAULT_PARAM(0),
	const void * data DEFAULT_PARAM(0),
	__uint data_len DEFAULT_PARAM(0)	
	);

RTKNET_API __uint PMC_API send_rtk_data_with_guid_ex(
	HVBUS channel,
	const RTK_ADDR * addr,
	const RTK_GUID * guid,
	__uint packet_type,
	__u16 grp_id,
	const void * data,
	__uint data_len
);

RTKNET_API int PMC_API default_server_filter(
	HVBUS b,
	int event,
	int data
	);

RTKNET_API int PMC_API default_client_filter(
	HVBUS b,
	int event,
	int data
	);

RTKNET_API int PMC_API set_personality(
	const char * local_addr
	);

extern RTKNET_API PNODE_TABLE_ENTRY g_ThisNode;

/* predefined virtual buses */
extern RTKNET_API HVBUS vbus_alarm;			/* BUSID = BUSID_ALARM */
extern RTKNET_API HVBUS vbus_rtdb;			/* BUSID = BUSID_RTDB */
extern RTKNET_API HVBUS vbus_system;		/* BUSID = BUSID_SYSTEM */
extern RTKNET_API HVBUS vbus_operation;		/* BUSID = BUSID_OPERATION */
extern RTKNET_API HVBUS vbus_config;		/* BUSID = BUSID_CONFIG */

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

#endif
