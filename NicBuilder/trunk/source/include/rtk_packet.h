/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\include\rtk\rtk_packet.h
	file path:	E:\vss\pmc2\src\include\rtk
	author	:  Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	message structures on the network	
	history:
	created:	10:10:2002	 17:33

	For the system-defined buses(rtdb, alarm, system, config, operation),
	the pakcet flags/type constants/packet structures are defined here.

	For private buses(hislog, evtd, f8 controler, etc.), it's up to the specific
	implementor to choose any values and packet structures. It wont' interfere
	with values and types defined here.

*********************************************************************/
#ifndef __rtk_packet_h__
#define __rtk_packet_h__

#include <pshpack1.h>

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

//
//
//
// ------------------------- 1) alarm channel ------------------------------
//
//
//
#define PT_Alarm		0x0000

#ifndef RTK_NO_PACKET_STRUCTS
/*
	layout of the alarm packet
	tag_name
	class (alarm,ack_alarm,cancel_alarm)
	type  (hihi,hi,lo,lolo,...)
	parameter_1
	parameter_2
*/
struct RTK_ALARM_PACKET{
	TAG_NAME	Tag;
	ALARM_CLASS Class;
	/* 0~1000 */
	int			Rank; 
	TAG_VALUE	Value;
	/* Since Jul 11, 2001, alarm is formatted into a message string instead of a 
	 binary structure.
	 消息字符串，而不是二进制结构
	*/
	char		Msg[1];
}PACKED;
#endif
//
//
//
// ------------------------- 2) rtdb channel ------------------------------
//
//
//
#define PT_ChangedData	0x1000		// 变位以及循环传送数据包
#define PT_QueryData		0x1001		// 直接读数据包

#ifndef RTK_NO_PACKET_STRUCTS

typedef struct __tag_CHANGED_DATA_ENTRY{
	TAG_KEY		name;
	struct pmc_value_t value;
}PACKED CHANGED_DATA_ENTRY;

typedef struct __tag_CHANGED_DATA{
	GROUP_KEY	group;
	__uint		count;
	// TAG_DATA_PACKET_ENTRY	tags[1];
}PACKED CHANGED_DATA;

typedef struct __tag_QUERY_DATA_ENTRY{
	SHORT_TAG_NAME	name;
	struct pmc_value_t value;
}PACKED QUERY_DATA_ENTRY;

#endif

//
//
//
// ------------------------- 3) system channel ------------------------------
//
//
//
#define PT_System		0x2000
#define PT_GetFile			0x2001
#define PT_PutFile			0x2002
#define PT_GetFileSize		0x2003
#define PT_DeleteFile		0x2004
#define PT_DiscoverFiles	0x2005
//
//
//
// ------------------------- 4) operation channel ------------------------------
//
//
//
#define PT_Operation		0x3000
#define PT_Operation2		0x3001
#define PT_Operation3		0x3002
#define PT_Operation4		0x3003
#define PT_Operation5		0x3004

//
//
//
// ------------------------- 5) online config channel --------------------------
//
//
//
#define PT_DiscoverNodes			0x4000
#define PT_DiscoverGroups			0x4001
#define PT_DiscoverTags			    0x4002
#define PT_ReloadTags				0x4003
#define PT_DiscoverDevices			0x4004
#define PT_DiscoverDrivers			0x4005
#define PT_DiscoverInstalledDrivers	0x4006
#define PT_DiscoverPictures			0x4007
#define PT_DiscoverConfigFiles		0x4008
#define PT_DiscoverHisLoggers		0x4009

#define PT_AddTag		0x4101
#define PT_DropTag		0x4102
#define PT_EditTag		0x4103
#define PT_AddGroup		0x4104
#define PT_DropGroup		0x4105
#define PT_EditGroup		0x4106
#define PT_AddDriver		0x4107
#define PT_DelDriver		0x4108
#define PT_EditDriver		0x4109
#define PT_AddDevice		0x4110
#define PT_DelDevice		0x4111
#define PT_EditDevice		0x4112
#define PT_AddNode		0x4113
#define PT_DropNode		0x4114

#define PT_GetString		0x4200
#define PT_SetString		0x4201
#define PT_GetStringEx	0x4202
#define PT_SetStringEx		0x4203

typedef __u32			OC_COMMAND;

#if defined(__cplusplus) && !defined(RTK_NO_PACKET_STRUCTS)

/*++
	the structures defined here are totally shit!
	the ack_guid is absolutely unnecessary because RtkPacket already
	has a GUID field, why should there be another?
--*/

typedef struct __tag_ONLINE_CONFIG{
	struct discover_nodes{
		RTK_GUID ack_guid;
	}PACKED;
	struct discover_groups{
		RTK_GUID ack_guid;
	}PACKED;
	struct discover_groups_ack{
		__u32		count;
		GROUP_KEY	items[1];
	}PACKED;
	struct discover_tags{
		RTK_GUID	ack_guid;
		GROUP_KEY	group;
	}PACKED;
	struct reload_tags_ack{
		RTK_TAG		tag;
	}PACKED;
	typedef struct reload_tags_ack add_tags_ack;
	union{
		discover_groups		d_groups;
		discover_nodes		d_nodes;
		discover_tags		d_tags;
	}PACKED;
}PACKED ONLINE_CONFIG, *PONLINE_CONFIG;

#endif

#include <poppack.h>

#endif // __rtk_packet_h__
