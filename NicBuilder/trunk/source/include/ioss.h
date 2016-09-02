/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\rtk\ioss.h
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	IO subsystem	
	history:
	created:	11/20/2002   15:32
*********************************************************************/
#ifndef __rtk_ioss_h__
#define __rtk_ioss_h__


#include <ddk.h>

#ifdef _WIN32
	#if defined(LIBIOSS_EXPORTS)
		#define IO_API _declspec(dllexport)
	#else
		#define IO_API _declspec(dllimport)
	#endif
#else
	#define IO_API extern
#endif

CDECL_BEGIN

IO_API __bool PMC_API init_ioss();

IO_API void PMC_API uninit_ioss();

// PnP support, interface exposed to PMCD
IO_API __uint PMC_API io_get_devices(
	DEVICE_INFO  *buffer,
	__uint *maxcount
	);

IO_API PDRIVER_INFO PMC_API io_load_driver(
	PCVENDOR_KEY v, 
	PCDEVTYPE_KEY t
	);

IO_API __bool PMC_API io_stop_driver(PDRIVER_INFO drv);

IO_API	__bool PMC_API io_unload_driver(PDRIVER_INFO drv);

/* NoN-PnP support, interface exposed to device driver */
IO_API PDEVICE_INFO PMC_API io_create_device(
	PDRIVER_INFO driverObj,
	PCDEVICE_KEY key,
	__uint extension_size DEFAULT_PARAM(0)
	);

IO_API __bool PMC_API io_update_tag(
	PCRTK_TAG tag, 
	Ppmc_value_t new_value,
	PCRTK_TIME now
	);

IO_API __bool PMC_API io_delete_device(PDEVICE_INFO dev);

IO_API __bool PMC_API io_start_device(PDEVICE_INFO dev);

IO_API __bool PMC_API io_stop_device(PDEVICE_INFO dev);

IO_API PDEVICE_INFO PMC_API io_open_device(PCDEVICE_KEY key);

IO_API __bool PMC_API io_write_device(
	PCSHORT_TAG_NAME name, 
	PCTAG_VALUE value
	);

IO_API __bool PMC_API io_write_device_ex(
	PCSHORT_TAG_NAME name, 
	PCpmc_value_t value
	);

IO_API PDEVICE_INFO PMC_API io_probe_device(
	const char * dev_name,
	__bool bValidateTags
	);

CDECL_END

#endif
