/*
    PC based Monitoring System source file
    version 1.0
    by J.Hunter,jhunter@263.net,May 2001 

    This is where the PMC's IO subsystem and device drivers communicate with
    each other.

    Revision history : 
    Created March 2001, by Jackie Hunter, jhunter@263.net

    First workable version came May,2001, by Jackie Hunter, jhunter@263.net

	02/28/2003, Version 2.03, libnpu.h and npu.h and plugins.h merged into
	a single file : ddk.h, Jackie, jhunter@tsinghua.org.cn.
*/

#ifndef __pmc_ddk_h__
#define __pmc_ddk_h__

#include <librtdb.h>
#include <rtk_compat.h>
#include <powermgr.h>

/*	serial port access coordinator support is built in by default
 	if you don't feel like it(as in some extremely simple case 
	where only one driver is communicating via the seiral ports), 
	just discard the APIs included.
	See manual ##.## for more.
*/
#include <libspac.h>

#define IOSS_MODULE_NUM 'IO'

#define IOSS_ERROR(code) PMC_ERROR(IOSS_MODULE_NUM, (code))

#define IOSS_OBJECT_NOT_FOUND \
	IOSS_ERROR(-1)
	
#define IOSS_DEVICE_NOT_STARTED\
	IOSS_ERROR(-2)

#define IOSS_DRIVER_NOT_LOADED\
	IOSS_ERROR(-3)
	
#include <pshpack1.h>

#ifdef _MSC_VER
typedef struct __tag_DRIVER_INFO DRIVER_INFO, *PDRIVER_INFO;
#else
struct __tag_DRIVER_INFO;
#endif

typedef struct __tag_VENDOR_KEY{
	char Data[16];
}VENDOR_KEY, *PVENDOR_KEY;
typedef const VENDOR_KEY * PCVENDOR_KEY;

#ifdef __cplusplus
typedef CAlphaName<VENDOR_KEY> CVendorName;
#endif

typedef struct __tag_VENDOR_INFO{
	VENDOR_KEY	key;
	char		description[128];
	char		reserved[16];
}PACKED VENDOR_INFO, *PVENDOR_INFO;

typedef struct __tag_DEVTYPE_KEY{
	char Data[16];
}DEVTYPE_KEY, *PDEVTYPE_KEY;
typedef const DEVTYPE_KEY * PCDEVTYPE_KEY;

#ifdef __cplusplus
typedef CAlphaName<DEVTYPE_KEY> CDevTypeName;
#endif

typedef struct __tag_DEVTYPE_INFO{
	DEVTYPE_KEY	key;
	char		description[64];
	char		reserved[16];
}PACKED DEVTYPE_INFO, *PDEVTYPE_INFO;

#define DF_Active		(0x1 << 0)
#define DF_Deleted	(0x1 << 1)

//
typedef struct __tag_DEVICE_INFO{
	DEVICE_KEY		k;
	
	struct __tag_DRIVER_INFO * d;//驱动信息指针
	VENDOR_INFO		v;
	DEVTYPE_INFO	t;

	char			parameter[256];
	char			address[64];

/* this is the field that device drivers can use at their
specific will, the IO subsystem won't interprete this field
nor modify it.*/
	void			*OwnerField;

	__u32			flags;
	RTK_LIST_ENTRY	tags;
	__i32			error;
	char			reserved[44];

#if defined(_MSC_VER) && 0
#pragma warning(disable : 4200)
	char		extension[0];
#define DEVICE_EXTENSION(d) d->extension
#else
#define DEVICE_EXTENSION(d) ((char*)d + sizeof(DEVICE_INFO))
#endif

}PACKED DEVICE_INFO, *PDEVICE_INFO;

typedef const DEVICE_INFO * PCDEVICE_INFO;

// typedef void * HRTK_DEVICE;
// typedef void * HRTK_DRIVER;

#include <poppack.h>

typedef int IOSS_STATUS;

#define IOSS_SUCCESS	0
#define IOSS_FAILED		-1

/*
	-------------- Device Driver Stubs Section Part I ------------------
	below are function prototypes that must be exported by device driver
	implementations(all of them may be defaulted to none).
	the functions are c-decl-ed, thus the implementation will be 
	less affected by changes on the interfaces.
	To source maintainer: Note they must conform to those prototypes listed
 	in Part II of Device Driver Stubs Section.
*/
typedef __bool (*IO_LOAD_PROC)(
	struct __tag_DRIVER_INFO * hdriver
	);

typedef __bool (*IO_UNLOAD_PROC)();

typedef __bool (*IO_START_DEVICE_PROC)(
	PDEVICE_INFO dev
	);

typedef __bool (*IO_STOP_DEVICE_PROC)(
	PDEVICE_INFO	dev
	);

typedef __bool (*IO_ADDRESS_TRANSLATE_PROC)(PRTK_TAG tte);

typedef void (*IO_UPDATE_TAG_PROC)(
	PCRTK_TAG tag, 
	Ppmc_value_t new_value,
	PCRTK_TIME now
	);

typedef __bool	(*IO_WRITE_DEVICE_PROC)(
	PRTK_TAG tte, 
	PCTAG_VALUE value
	);

#define DISP_POWER_STATE_CHANGED		0x00000001

#define DISP_DB_ADD_TAG					0x01000000
#define DISP_DB_DROP_TAG				0x01000001

#define DISP_RESERVED					0x00000030

typedef IOSS_STATUS (PMC_API * IO_DISPATCH_PROC)(
	PDEVICE_INFO device,
	int majorCode,
	int param
	);


#define DRIVER_FLAG_LOADED	(0x1<<0) 

#include <pshpack1.h>
typedef struct __tag_DRIVER_INFO{
	void						*plugin_handle;
	IO_LOAD_PROC				load;
	IO_UNLOAD_PROC				unload;
	IO_START_DEVICE_PROC		start_device;
	IO_STOP_DEVICE_PROC			stop_device;	
	IO_ADDRESS_TRANSLATE_PROC	address_translate;
	IO_UPDATE_TAG_PROC			update_tag;
	IO_WRITE_DEVICE_PROC		write_device;
	__uint						device_count;
	VENDOR_INFO					vendor;
	DEVTYPE_INFO				type;
	char						description[128];	
	char						parameter[256];
	char						dllname[256];
	__u64					version;
	IO_DISPATCH_PROC		dispatch;
	__u32					flags;
	char						reserved[56];
}PACKED DRIVER_INFO, *PDRIVER_INFO;
#include <poppack.h>

#ifdef _WIN32
#define IOSS_DLL_EXPORT __declspec(dllexport)
#else
#define IOSS_DLL_EXPORT
#endif

/*
	-------------- Device Driver Stubs Section Part II -----------------
	the prototypes listed here make sure that functions defined
	in your device-driver inplementation file are of the correct
	type and attributes. So remember to include this file into 
	your device driver.
*/

CDECL_BEGIN

IOSS_DLL_EXPORT __bool load(
	PDRIVER_INFO driverObj
	);

IOSS_DLL_EXPORT __bool unload();

IOSS_DLL_EXPORT __bool start_device(
	PDEVICE_INFO handle
	);

IOSS_DLL_EXPORT __bool stop_device(
	PDEVICE_INFO handle
	);

IOSS_DLL_EXPORT __bool write_device(
	PRTK_TAG tte, 
	PCTAG_VALUE value
	);

IOSS_DLL_EXPORT void update_tag(
	PCRTK_TAG tte, 
	struct pmc_value_t * pt, 
	PCRTK_TIME now
	);

IOSS_DLL_EXPORT __bool address_translate(
	PRTK_TAG tte
	);

IOSS_DLL_EXPORT IOSS_STATUS PMC_API dispatch(
	PDEVICE_INFO device,
	int majorCode,
	int param
	);

CDECL_END

#endif
