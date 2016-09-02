/*
	defines protocols between Soft-PLC components
	jhunter, 2004/8

	Soft PLC module header is self-contained, driver writer should only
	include this file.
*/
#ifndef __f8_softplc_h__
#define __f8_softplc_h__

#include <librtk.h>
#include <f8_config.h>
#include <f8_vm.h>
#include <f8_status.h>

#define SPLC_API FKERN_API

#ifdef _WIN32
	#define SPLC_EXPORTS __declspec(dllexport)
#else
	#define SPLC_EXPORTS
#endif

struct splc_driver_t;
struct splc_device_t;

#define SPLC_DEVICE_STOPPED	0
#define SPLC_DEVICE_STARTED 	1

struct splc_device_t{
	f8_uuid uuid;
	f8_int flags;
	f8_int status;
	char * name;
	char * init_string;
	void * ownerField;
	struct splc_driver_t * driver;
	RTK_LIST_ENTRY link;
	RTK_LIST_ENTRY link2;

	f8_u16 memAddr;
	f8_u16 inputLength;
	f8_u16 outputLength;
	f8_byte xMemory[IO_SIZE_PER_SLOT];
};

#define PLC_DRV_FLAG_OK	1

struct splc_driver_t{
	char * name;
	f8_int flags;
	f8_int refCount;
	void * moduleHandle;
	void * ownerField;
	f8_int (*start)(struct splc_device_t * d);
	f8_int (*stop)(struct splc_device_t * d);
	f8_int (*load)(struct splc_driver_t * d);
	f8_int (*unload)(struct splc_driver_t * d);

	RTK_LIST_ENTRY devices;
	RTK_LIST_ENTRY link;
	
	f8_int extension_size;
};

#ifdef __cplusplus
extern "C"{
#endif

SPLC_EXPORTS f8_int driver_entry(struct splc_driver_t * d);

#ifdef __cplusplus
}
#endif
	
#endif

