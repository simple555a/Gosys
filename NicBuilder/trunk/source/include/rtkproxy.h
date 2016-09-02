/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\rtkproxy.h
	file path:	E:\vss\pmc2\src\include
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Client side proxy	
	history:
	created:	10:18:2002   14:58
*********************************************************************/
#ifndef __rtkproxy_h__
#define __rtkproxy_h__

#include <librtdb.h>
#include <ddk.h>

#ifdef _WIN32
    #ifdef LIBRTKPROXY_EXPORTS
        #define PROXY_API      __declspec(dllexport)
    #else
        #define PROXY_API      __declspec(dllimport)
    #endif  
#else
    #define PROXY_API
#endif

CDECL_BEGIN

#include <pshpack4.h>

// callback routines
typedef struct __tag_PROXY_DISPATCH_TABLE{
	void (CDECL *OnAddTag)(PCTAG_NAME name);
	void (CDECL *OnDropTag)(PCTAG_NAME names);
	void (CDECL *OnTagModified)(PCTAG_NAME name);
	void (CDECL *OnAddGroup)(PCTAG_NAME name);
	void (CDECL *OnDropGroup)(PCTAG_NAME name);
	void (CDECL *OnGroupModified)(PCTAG_NAME names);
	void (CDECL *OnAlarm)(int rank, ALARM_CLASS ac, const char * msg, PCRTK_ADDR src);//
	void (CDECL *OnProgress)(__uint total, __uint completed, const char * msg);//
	void (CDECL *OnAddDevice)(PCNODE_KEY nd, PDEVICE_INFO dev);//
	void (CDECL *OnDropDevice)(PCNODE_KEY nd, PCDEVICE_KEY dev);//
	void (CDECL *OnDeviceModified)(PCNODE_KEY nd, PDEVICE_INFO dev);//
	void (CDECL *OnAddNode)(PCNODE_KEY nd);
	void (CDECL *OnDropNode)(PCNODE_KEY nd);
	__int	reserved[14];
}PACKED PROXY_DISPATCH_TABLE, *PPROXY_DISPATCH_TABLE;

typedef struct __tag_PICT_KEY{
	char	Data[rtkm_pict_key_length];
}PACKED PICT_KEY, *PPICT_KEY;
typedef const PICT_KEY * PCPICT_KEY;

#ifdef __cplusplus
typedef CAlphaName<PICT_KEY> CPictName;
#endif

#include <poppack.h>

extern PROXY_DISPATCH_TABLE	PROXY_API g_ProxyDispatchTable;

#define PROXY_ENABLE_BGLOADER	(0x00000001 << 0)
#define PROXY_ENABLE_DISPATCHER	(0x00000001 << 1)
#define PROXY_SILENT			(0x00000001 << 2)
#define PROXY_INIT_AS_SERVER	(0x00000001 << 3)

PROXY_API __bool PMC_API init_proxy(
	HWND hWnd,
	__uint init_flags DEFAULT_PARAM(0)
	);
PROXY_API void   PMC_API uninit_proxy();

PROXY_API __uint PMC_API proxy_read_tags(
	__uint count,
	PCTAG_NAME names,
	Ppmc_value_t values,
	__uint timeout
	);

PROXY_API __uint PMC_API proxy_read_tags_ex(
	__uint count, 
	PCTAG_NAME names, 
	Ppmc_value_t values, 
	__uint timeout,
	__uint *existed
	);

PROXY_API __uint PMC_API proxy_write_tags(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	);
PROXY_API __uint PMC_API proxy_write_tags_x(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	);
PROXY_API __uint PMC_API proxy_write_tags_y(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values
	);
PROXY_API __uint PMC_API proxy_write_tags_z(
	__uint count,
	PCTAG_NAME names, 
	PCTAG_VALUE values,
	char * usrname
	);


PROXY_API __uint PMC_API proxy_write_tags_ex(
	__uint count,
	PCTAG_NAME names, 
	PCpmc_value_t values
	);

PROXY_API __uint PMC_API update_changed_data(
	PCRTK_PACKET packet,
	__bool bThisIsAServer
	);
/*
	success/fail, timeout, progress
*/
PROXY_API __uint PMC_API discover_nodes(__uint timeout);
PROXY_API __uint PMC_API discover_groups(PCNODE_KEY node, __uint timeout);
PROXY_API __uint PMC_API discover_tags(
	PCNODE_KEY node, 
	PCGROUP_KEY group, 
	__uint timeout
	);

PROXY_API __bool PMC_API discover_nodes_ex(
	__uint timeout, 
	__uint * count,
	__bool bDiscardOld DEFAULT_PARAM(__false)
	);
PROXY_API __bool PMC_API discover_groups_ex(
	PCNODE_KEY node, 
	__uint timeout,
	__uint * count,
	__bool bDiscardOld DEFAULT_PARAM(__false)
	);
PROXY_API __bool PMC_API discover_tags_ex(
	PCNODE_KEY node, 
	PCGROUP_KEY group, 
	__uint timeout,
	__uint * count,
	__bool bDiscardOld DEFAULT_PARAM(__false)
	);

PROXY_API __uint PMC_API discover_devices(
	PCNODE_KEY node, 
	DEVICE_INFO *buf, 
	__uint *maxcount,
	__uint timeout
	);
PROXY_API __uint PMC_API discover_pictures(
	PCNODE_KEY node,
	PPICT_KEY buffer,
	__uint *maxcount,
	__uint timeout
	);
PROXY_API __uint PMC_API discover_configfiles(
	PCNODE_KEY node,
	char *buffer,
	__uint buffersize,
	__uint timeout
	);
PROXY_API __uint PMC_API discover_files(
	PCNODE_KEY node,
	const char * path,
	const char * swildcards,
	char *buffer,
	__uint buffersize,
	__uint timeout
	);
PROXY_API __uint PMC_API reload_tags(
	__uint count, 
	PCTAG_NAME names, 
	__uint timeout
	);
PROXY_API __uint PMC_API add_tags(
	__uint count, 
	PCTAG_NAME names, 
	STATIC_TAG_DATA *s, 
	__uint timeout
	);
PROXY_API __uint PMC_API edit_tags(
	__uint count, 
	PCTAG_NAME names, 
	STATIC_TAG_DATA *s, 
	__uint timeout
	);
PROXY_API __uint PMC_API drop_tags(
	__uint count, 
	PCTAG_NAME names,
	__uint timeout
	);
PROXY_API __uint PMC_API add_groups(
	__uint count,
	PCNODE_KEY nodes,
	PCRTK_GROUP groups,
	__uint timeout
	);
PROXY_API __uint PMC_API edit_groups(
	__uint count,
	PCNODE_KEY nodes,
	PCRTK_GROUP groups,
	__uint timeout
	);
PROXY_API __uint PMC_API drop_groups(
	__uint count,
	PCNODE_KEY nodes,
	PCGROUP_KEY groups,
	__uint timeout
	);
PROXY_API __uint PMC_API commit_changes(
	__uint timeout
	);

PROXY_API __bool PMC_API proxy_get_file_size(
	PCNODE_KEY node,
	const char * path,
	__uint *size
	);

PROXY_API __bool PMC_API download_file(
	PCNODE_KEY node,
	const char * path,
	char **buffer,
	__uint *buffersize
	);

PROXY_API __bool PMC_API upload_file(
	PCNODE_KEY node,
	const char * path,
	char *buffer,
	__uint buffersize,
	__uint timeout
	);

PROXY_API __bool PMC_API proxy_delete_file(
	PCNODE_KEY node,
	const char * path,
	__uint timeout
	);

PROXY_API __bool PMC_API proxy_add_device(
	PCNODE_KEY node,
	PCDEVICE_KEY key,
	PCVENDOR_KEY v,
	PCDEVTYPE_KEY t,	
	const char * addr,
	const char * init_string, 
	__uint timeout
	);

PROXY_API __bool PMC_API proxy_edit_device(
	PCNODE_KEY node,							
	PCDEVICE_KEY key,
	const char * addr,
	const char * init_string, 
	__uint timeout
	);

PROXY_API __bool PMC_API proxy_del_device(
	PCNODE_KEY node,							
	PCDEVICE_KEY key,
	__uint timeout
	);

PROXY_API void PMC_API proxy_release_data(void * buf);

PROXY_API __bool PMC_API rtk_queue_event(int id, ...);

PROXY_API void PMC_API rtk_dispatch_events();

CDECL_END

#endif  // __rtkproxy_h__
