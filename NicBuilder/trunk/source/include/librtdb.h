/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\include\librtdb.h
	file path:	E:\vss\pmc2\src\include
	author:	Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Realtime database types	
	history:
	Created : Feb,2001, version 1.0

	Upgraded: to version 2.03, 10:8:2002   14:53
	
	Upgraded: to version 3.5, 2004/8/14, 10 more tag types introduced
*********************************************************************/
#ifndef __sysdb_h__
#define __sysdb_h__

#include <string.h>
#include <librtk.h>
#include <utils.h>
#include <pmc_config.h>
#include <rtk_list_entry.h>

#ifdef _WIN32
	#ifdef LIBRTDB_EXPORTS
		#define RTDB_API	  __declspec(dllexport)
	#else
		#define RTDB_API	  __declspec(dllimport)
	#endif
#else
	#define RTDB_API
#endif

#define RTDB_MODULE_NUM 'DB'

#define RTDB_ERROR(code) PMC_ERROR(RTDB_MODULE_NUM, (code))

#define RTDB_OBJECT_NOT_FOUND \
	RTDB_ERROR(-1)
	
#define RTDB_ACCESS \
	RTDB_ERROR(-2)

/*
	database has reached its maximum capacity
*/
#define RTDB_HARD_CONSTRAINTS \
	RTDB_ERROR(-3)

#include <pmc_var.h>

CDECL_BEGIN

/*
	Data types & constants
*/

#include <pshpack1.h>

typedef struct __tag_NODE_KEY{
	char	Data[rtkm_node_key_length];
}PACKED NODE_KEY, *PNODE_KEY;
typedef const NODE_KEY * PCNODE_KEY;

typedef struct __tag_GROUP_KEY{
	char	Data[rtkm_group_key_length];
}PACKED GROUP_KEY, *PGROUP_KEY;
typedef const GROUP_KEY * PCGROUP_KEY;

typedef struct __tag_GROUP_NAME{
	NODE_KEY	node;
	GROUP_KEY	group;
}PACKED RTK_GROUP_NAME, *PRTK_GROUP_NAME;
typedef const RTK_GROUP_NAME * PCRTK_GROUP_NAME;

typedef struct __tag_TAG_KEY{
	char	Data[rtkm_tag_key_length];
}PACKED TAG_KEY, *PTAG_KEY;
typedef const TAG_KEY * PCTAG_KEY;

typedef struct __tag_DEVICE_KEY{
	char Data[16];
}PACKED DEVICE_KEY, *PDEVICE_KEY;
typedef const DEVICE_KEY *PCDEVICE_KEY;

#ifdef __cplusplus
typedef CAlphaName<DEVICE_KEY> CDeviceName;
#endif

/* In some operations, the NodeKey field is implied, so only sname.group
and sname.tag will suffice
*/
typedef struct __tag_SHORT_TAG_NAME{
	GROUP_KEY group;
	TAG_KEY   tag;
}PACKED SHORT_TAG_NAME, *PSHORT_TAG_NAME;
typedef const SHORT_TAG_NAME * PCSHORT_TAG_NAME;
/*
	TAG_NAME is used to uniquely identify a tag system wide
*/
typedef struct __tag_TAG_NAME{
	NODE_KEY		node;
	SHORT_TAG_NAME	sname;
}PACKED TAG_NAME, *PTAG_NAME;
typedef const TAG_NAME * PCTAG_NAME;

#define TAGNAME_TEXT_LENGTH \
	(\
	sizeof(NODE_KEY) + sizeof(GROUP_KEY) + sizeof(TAG_KEY) + 2\
	)

struct text_tag_name_t{
	char m_name[TAGNAME_TEXT_LENGTH + 1];
}PACKED;

#define AC_Alarm			0
#define AC_AckAlarm		1
#define AC_CancelAlarm	2
#define AC_WriteDevice	3
#define AC_Online			4
#define AC_Offline			5
#define AC_PowerSwitch	6

typedef __int ALARM_CLASS;

/*
	maybe the alarm type should be renamed 'tag-status'
*/
#define AT_NoAlarm			0
#define AT_HiHi				1
#define AT_Hi					2
#define AT_Lo					3
#define AT_LoLo				4
#define AT_On2Off			5
#define AT_Off2On			6
#define AT_VConflict			7
#define AT_DFault				8
#define AT_SharpChange		9
#define AT_Asynchronized		10
#define AT_SystemUpdate		11
#define AT_WriteDevice			12
#define AT_SwitchPrimary		13
#define AT_SwitchBackup		14

/* more alarm types go here*/
typedef __int ALARM_TYPE;

typedef struct __tag_RTK_NODE{
	NODE_KEY	key;
	void			*itemtable;
	__uint		refcount;
	__uint		context;
	__int		dogcount;
	__u32		reserved[15];
}PACKED RTK_NODE, *PRTK_NODE;
typedef const RTK_NODE *PCRTK_NODE;

#define GF_Dirty	(0x00000001 << 0)
#define GF_System	(0x00000001 << 1)
typedef __i32	GROUP_FLAG;
typedef struct __tag_RTK_GROUP{
	NODE_KEY	node;
	GROUP_KEY	key;	
	char		description[rtkm_description_length];
	__uint		period;	// refresh period
	__u32		reserved_0[13];

	void		*itemtable;	
	__uint		refcount;	
	struct{
		GROUP_FLAG	flags;
		__uint		context;	
	}d;
}PACKED RTK_GROUP, *PRTK_GROUP;
typedef const RTK_GROUP *PCRTK_GROUP;

enum __tag_comp_method_t{
	Compress_None = 0,
	Compress_Const,
	Compress_Linear,
	Compress_Max,
};

typedef __u8 comp_method_t;

/*
	tag's static information (in configuration)
	the client can query about a tag's static information,
	and when querying, this part of information is returned.
*/
typedef struct __tag_STATIC_TAG_DATA{
	TAG_FLAGS	Flags;

	__i32		AlarmRank;	
	r4i32_t		u_deadband;

	r4i32_t 	u_hihi;
	r4i32_t	u_hi;
	r4i32_t	u_lo;
	r4i32_t	u_lolo;
	
	__r4		fltRate;
	SHORT_TAG_NAME CutOffTagName;
	__u32	CutOffMask;

	r4i32_t	u_min, u_max;

#ifndef RTDB_NO_BACKWARD_MACROS
// backward compatibility
#define fltMinValue u_min.flt
#define fltMaxValue u_max.flt
#define fltHiHi u_hihi.flt
#define fltHi u_hi.flt
#define fltLo u_lo.flt
#define fltLoLo u_lolo.flt
#define fltAlarmDeadband u_deadband.flt
#ifndef AlarmMask
#define AlarmMask SFlags
#endif
#endif

	union{
		struct {
			char OnMsg[rtkm_onmsg_length];
			char OffMsg[rtkm_onmsg_length];
		}SwitchMsg;
		struct {
			char EU[rtkm_eu_length];			
		}AnalogMsg;
	};
	char Description[rtkm_description_length];
	DEVICE_KEY	Device;
	char Address[rtkm_tag_address_length];

	// added to version 3.5
	__r4 ExcDev;
	__u16 ExcMax;	// 
	__u16 Access;		// reserved
	__u32 Owner;		// reserved
	__i8 DisplayDigits;
	__u8 Compressing;
	__u16 CompMax;
	__r4 CompDev;

	// now we've used the reserved fields, :-)
	// char Reserved[0];
}PACKED STATIC_TAG_DATA, *PSTATIC_TAG_DATA;
typedef const STATIC_TAG_DATA * PCSTATIC_TAG_DATA;

/*
	tag's dynamic information (changed at runtime)
*/
typedef struct __tag_DYNAMIC_TAG_DATA{	
	struct pmc_value_t Value;
	RTK_TIME	Time;			/* last written time, used in changing rate computing*/
	union{
		RTK_TIME	BroadcastTime;	/* used in exception-based data broadcasting */
		RTK_TIME	CachedTime;		/* client side cache, last updated */
	};
	ALARM_TYPE	AlarmStatus;
	ALARM_CLASS AlarmClass;             //2015.03.23  GM 添加报警确认判断属性
	struct __tag_DEVICE_INFO * DeviceObj; 
	// void 		*DeviceObj;
	char		BinaryAddress[64]; // Zander 32->64
	TAG_VALUE	BroadcastedValue;
	RTK_LIST_ENTRY	DeviceLink;	// reserved
	RTK_LIST_ENTRY	RefreshLink;	// reserved
	__i16	RefreshColor;
}PACKED DYNAMIC_TAG_DATA, *PDYNAMIC_TAG_DATA;
typedef const DYNAMIC_TAG_DATA * PCDYNAMIC_TAG_DATA;

/* tag table in memory */

typedef struct __tag_RTK_TAG{
/*
	whether the tag_table_entry should contain a Key field
	is still a question, for now, we just leave it here because
	it seems not too disturbing
*/
	NODE_KEY			node;
	GROUP_KEY			group;
	TAG_KEY				key;
	__uint				refcount;
	STATIC_TAG_DATA	s;
	DYNAMIC_TAG_DATA	d;
}PACKED RTK_TAG, *PRTK_TAG;
typedef const RTK_TAG * PCRTK_TAG;

#include <poppack.h>

/* function prototypes declaration */
RTDB_API __bool PMC_API init_rtdb(void);
RTDB_API void PMC_API uninit_rtdb(void);

typedef RTK_HANDLE RTK_CURSOR;

RTDB_API __bool PMC_API cursor_move_next(RTK_CURSOR cursor);
RTDB_API __bool PMC_API cursor_is_end(RTK_CURSOR cursor);
RTDB_API __bool PMC_API cursor_move_end(RTK_CURSOR cursor);
RTDB_API RTK_CURSOR PMC_API cursor_open_first_subitem(
	RTK_CURSOR cursor
	);
RTDB_API __bool PMC_API cursor_delete(RTK_CURSOR cursor);
RTDB_API __bool PMC_API cursor_clear_subitems(RTK_CURSOR cursor);
RTDB_API void *	PMC_API cursor_get_item(RTK_CURSOR cursor);
RTDB_API __uint	PMC_API cursor_get_subitem_count(RTK_CURSOR cursor);

#define HNODE_LOCAL_MACHINE open_local_node()
RTDB_API RTK_CURSOR PMC_API create_node(
	const NODE_KEY *key, 
	__uint context
	);
RTDB_API RTK_CURSOR PMC_API open_node(const NODE_KEY *key);
RTDB_API RTK_CURSOR PMC_API open_first_node();
RTDB_API RTK_CURSOR PMC_API open_local_node();

RTDB_API RTK_CURSOR PMC_API create_group(
	RTK_CURSOR node,
	PRTK_GROUP pgrp
	);
RTDB_API RTK_CURSOR PMC_API open_group(
	RTK_CURSOR node,
	const GROUP_KEY * gk
	);
RTDB_API RTK_CURSOR PMC_API open_group_f(
	PCNODE_KEY node,
	PCGROUP_KEY group
	);
RTDB_API RTK_CURSOR PMC_API create_tag(
	RTK_CURSOR group,
	const TAG_KEY * tag, 
	const STATIC_TAG_DATA * sd
	);
RTDB_API RTK_CURSOR PMC_API open_tag(
	RTK_CURSOR node,
	const SHORT_TAG_NAME * tag
	);
RTDB_API RTK_CURSOR PMC_API open_tag_g(
	RTK_CURSOR group,
	const TAG_KEY * tag
	);
RTDB_API RTK_CURSOR PMC_API open_tag_t(
	RTK_CURSOR node,
	const char * short_name	
	);
RTDB_API RTK_CURSOR PMC_API open_tag_f(
	const TAG_NAME * tag
	);

/*
	query_tag/group/node
	--------------- WARNING  ------------------------------------
	this procedure acquires tagtable entry directly, i.e., without
	opening a handle to it, thus no reference counting is involved,
	so you must lock RTDB before this procedure, and unlock rtdb
	after use.
*/
RTDB_API PRTK_NODE PMC_API query_node(
	PCNODE_KEY node
	);
RTDB_API PRTK_GROUP PMC_API query_group(
	RTK_CURSOR node, 
	const GROUP_KEY *key
	);
RTDB_API PRTK_TAG PMC_API query_tag(
	RTK_CURSOR node, 
	const SHORT_TAG_NAME * tag
	);
RTDB_API PRTK_TAG PMC_API query_tag_g(
	RTK_CURSOR group, 
	PCTAG_KEY tag
	);
RTDB_API PRTK_TAG PMC_API query_tag_f(
	const TAG_NAME * tag
	);

RTDB_API int PMC_API validate_tag(PRTK_TAG tte);

RTDB_API __bool PMC_API lock_rtdb(
	__bool exclusive, 
	__uint timeout
	);
RTDB_API __bool PMC_API unlock_rtdb();

RTDB_API int PMC_API string_by_mask(
	__uint att,
	char * str
	);

RTDB_API __uint PMC_API mask_by_string(
	char * nn
	);

RTDB_API void PMC_API tag_trans_error(
	int err
	);

RTDB_API int PMC_API tag_by_textA(
								  PRTK_TAG tte, 
								  const char * strLineBuf, 
								  PCGROUP_KEY gk
								  );
RTDB_API int PMC_API tag_to_textA(
								  char *output, 
								  PCTAG_KEY key, 
								  PCSTATIC_TAG_DATA s
								  );
RTDB_API int PMC_API tag_by_text2A(
								  PRTK_TAG tte, 
								  const char * strLineBuf, 
								  PCGROUP_KEY gk
								  );
RTDB_API int PMC_API tag_to_text2A(
								  char *output, 
								  PCTAG_KEY key, 
								  PCSTATIC_TAG_DATA s
								  );
RTDB_API __bool PMC_API tagname_by_textA(
	PTAG_NAME tn, 
	const char * name
	);

RTDB_API int PMC_API tagname_to_textA(
	PCTAG_NAME tn, 
	char * name, 
	__uint len
	);

RTDB_API int PMC_API tag_by_textW(
								  PRTK_TAG tte, 
								  const wchar_t * strLineBuf, 
								  PCGROUP_KEY gk
								  );

RTDB_API int PMC_API tag_to_textW(
								  wchar_t *output, 
								  PCTAG_KEY key, 
								  PCSTATIC_TAG_DATA s
								  );

RTDB_API int PMC_API tag_by_text2W(
								  PRTK_TAG tte, 
								  const wchar_t * strLineBuf, 
								  PCGROUP_KEY gk
								  );

RTDB_API int PMC_API tag_to_text2W(
								  wchar_t *output, 
								  PCTAG_KEY key, 
								  PCSTATIC_TAG_DATA s
								  );

RTDB_API __bool PMC_API tagname_by_textW(
	PTAG_NAME tn, 
	const wchar_t * name
	);

RTDB_API int PMC_API tagname_to_textW(
									  PCTAG_NAME tn, 
									  wchar_t * name, 
									  __uint len
									  );

#ifdef UNICODE
	#define tagname_by_text tagname_by_textW
	#define tagname_to_text tagname_to_textW
	#define tag_by_text tag_by_textW
	#define tag_to_text tag_to_textW
	#define tag_by_text2 tag_by_text2W
	#define tag_to_text2 tag_to_text2W
#else
	#define tagname_by_text tagname_by_textA
	#define tagname_to_text tagname_to_textA
	#define tag_by_text tag_by_textA
	#define tag_to_text tag_to_textA
	#define tag_by_text2 tag_by_text2A
	#define tag_to_text2 tag_to_text2A
#endif

RTDB_API int PMC_API dbg_dump_tag(
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	);

RTDB_API int PMC_API dbg_random_tag(
	PTAG_NAME tn, 
	PSTATIC_TAG_DATA s
	);

#define buffer_append(p,item) \
	do{\
		memcpy((void*)(p),&(item),sizeof(item));\
		(p)+=sizeof(item);\
	}while(0)
#define buffer_pick(p,item) \
	do{\
		memcpy(&(item),(void*)(p),sizeof(item));\
		(p)+=sizeof(item);\
	}while(0)

#include <pshpack4.h>
typedef enum __e_RTDB_EVENT{
	EV_AddTag = 0,
	EV_DropTag = 1,
	EV_ModifyTag = 2,
	EV_AddGroup = 3,
	EV_DropGroup = 4,
	EV_ModifyGroup = 5,
	EV_AddNode = 6,
	EV_DropNode = 7,
}RTDB_EVENT;

// RTDB PNP hooking support
typedef struct __tag_RTDB_DISPATCH_TABLE{
	void (*OnAddTag)(PRTK_TAG pTag);
	void (*OnDropTag)(PRTK_TAG pTag);
	void (*OnModifyTag)(PRTK_TAG);

	void (*OnAddGroup)(PRTK_GROUP pgrp);
	void (*OnDropGroup)(PRTK_GROUP pgrp);
	void (*OnModifyGroup)(PRTK_GROUP pgrp);
	
	void (*OnAddNode)(PRTK_NODE pnode);
	void (*OnDropNode)(PRTK_NODE pnode);
}RTDB_DISPATCH_TABLE, *PRTDB_DISPATCH_TABLE;
#include <poppack.h>

RTDB_API __bool PMC_API hook_rtdb_events(
	PRTDB_DISPATCH_TABLE table
	);
RTDB_API __bool PMC_API unhook_rtdb_events(
	PRTDB_DISPATCH_TABLE table
	);
RTDB_API void PMC_API fire_rtdb_event(
	RTDB_EVENT id, 
	void * p1 DEFAULT_PARAM(0), 
	void * p2 DEFAULT_PARAM(0)
	);

CDECL_END

#ifdef __cplusplus

inline bool operator < (const TAG_KEY &t1, const TAG_KEY &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}
inline bool operator < (const GROUP_KEY &t1, const GROUP_KEY &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}
inline bool operator < (const NODE_KEY &t1, const NODE_KEY &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}
inline bool operator < (const RTK_GROUP_NAME &t1, const RTK_GROUP_NAME &t2)
{
	int r = rtk_compare_key(t1.node, t2.node);
	if(r < 0){
		return true;
	}else if(r > 0){
		return false;
	}
	return t1.group < t2.group;
}
inline bool operator < (const SHORT_TAG_NAME &t1, const SHORT_TAG_NAME &t2)
{
	int r;
	r = rtk_compare_key(t1.group, t2.group);
	if(r < 0){
		return true;
	}
	if(r > 0){
		return false;
	}
	return t1.tag < t2.tag;
}
inline bool operator < (const TAG_NAME &t1, const TAG_NAME &t2)
{
	int r;
	r = rtk_compare_key(t1.node, t2.node);
	if(r < 0){
		return true;
	}else if(r > 0){
		return false;
	}
	return t1.sname < t2.sname;
}
inline bool operator < (const DEVICE_KEY &t1, const DEVICE_KEY &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}

inline bool operator == (const TAG_KEY &t1, const TAG_KEY &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
inline bool operator == (const GROUP_KEY &t1, const GROUP_KEY &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
inline bool operator == (const NODE_KEY &t1, const NODE_KEY &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
inline bool operator == (const RTK_GROUP_NAME &t1, const RTK_GROUP_NAME &t2)
{
	if(!(t1.node == t2.node)){
		return false;
	}
	return t1.group == t2.group;
}
inline bool operator == (const SHORT_TAG_NAME &t1, const SHORT_TAG_NAME &t2)
{
	if(!(t1.group == t2.group)){
		return false;
	}
	return t1.tag == t2.tag;
}
inline bool operator == (const TAG_NAME &t1, const TAG_NAME &t2)
{
	if(!(t1.node == t2.node)){
		return false;
	}
	return t1.sname == t2.sname;
}
inline bool operator == (const DEVICE_KEY &t1, const DEVICE_KEY &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
typedef CAlphaName<NODE_KEY> CNodeName;
typedef CAlphaName<GROUP_KEY> CGroupName;
typedef CAlphaName<TAG_KEY> CTagName;
typedef CAlphaName<DEVICE_KEY> CDeviceName;

#endif

// forward declaration
#ifdef _WIN32
typedef struct	__tag_RTK_HOST RTK_HOST;
#else
struct __tag_RTK_HOST;
#endif

__inline void node_to_host(PCNODE_KEY node, struct __tag_RTK_HOST * host)	
{
	memcpy(host, node, sizeof(NODE_KEY));
}

__inline void host_to_node(const struct __tag_RTK_HOST * host, PNODE_KEY node)
{
	memcpy(node, host, sizeof(NODE_KEY));
}

#ifndef is_same_key
#define is_same_key(k1, k2)			(!strnicmp((const char *)(k1), (const char *)(k2), sizeof(*(k1))))
#endif

#endif	//_lib_h
