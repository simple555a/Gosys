#include "../precomp.h"
#pragma hdrstop
#include <stdlib.h>

static RTK_LIST_ENTRY drivers;
static RTK_LIST_ENTRY devices;
static RTK_HANDLE thread;

#define SECTION_NAME "SOFTPLC"

static struct splc_driver_t * getDriverByName(const char * name)
{
	PRTK_LIST_ENTRY l;
	struct splc_driver_t * d;
	
	l = drivers.Flink;
	while(l != &drivers){
		d = RTK_CONTAINING_RECORD(l, struct splc_driver_t, link);
		if(!strcmpi(d->name, name)){
			return d;
		}
	}

	return 0;
}

static struct splc_driver_t * probeDriver(const char * moduleName)
{
	struct splc_driver_t * drv;
	drv = getDriverByName(moduleName);
	if(drv){
		drv->refCount++;
		return drv;
	}
	drv = (struct splc_driver_t * )malloc(sizeof(struct splc_driver_t));
	if(!drv){
		return 0;
	}

	memset(drv, 0, sizeof(*drv));
	RtkInitializeListHead(&drv->devices);
	
	drv->name = (char *)alloc_buffer_v(moduleName, strlen(moduleName) + 1, 0);
	drv->moduleHandle = LoadLibrary(moduleName);
	if(drv->moduleHandle){
		
#define _find_proc(x, y) \
	(*(FARPROC*)&(x)) = GetProcAddress((HINSTANCE)drv->moduleHandle, #y);

		_find_proc(drv->load, driver_entry)
	}

	if(drv->load){
		if(drv->load(drv) == F8_SUCCESS){
			drv->flags |= PLC_DRV_FLAG_OK;
		}
	}else{
		drv->flags |= PLC_DRV_FLAG_OK;
	}

	RtkInsertTailList(&drivers, &drv->link);
	
	return drv;
}

/*
	the name is given in the form of 'vendor.type'
	for now, only type field is used
	2004/8, jhunter
*/
static f8_int probeDevice(const char * name, struct splc_device_t ** d)
{
	CArgs a;
	char buf[1024];
	char cname[F8_MAX_PATH];
	char devConfigFile[F8_MAX_PATH];
	char configKey[F8_MAX_PATH];
	struct splc_driver_t * drv;
	struct splc_device_t * dev;
	f8_u16 n_di, n_do, n_ai, n_ao, ai_width, ao_width;

	utils_debug("Try loading device %s\n", name);
	
	if(d){
		*d = 0;
	}

	strncpy(cname, name, sizeof(cname));
	cname[sizeof(cname) - 1] = 0;
	
	strcpy(configKey, cname);
	strncat(configKey, ".type", sizeof(configKey));
	configKey[sizeof(configKey) - 1] = 0;
	GetPrivateProfileString(
		SECTION_NAME,
		configKey,
		"",
		buf,
		sizeof(buf),
		get_config_file()
		);
	parse_arg_ex(buf, &a, ".");
	if(a.argc != 2){
		utils_error("Device %s not configured properly.\n", name);
		return F8_INVALID_NAME;
	}
	
	strncpy(devConfigFile, get_working_dir(), sizeof(devConfigFile));
	devConfigFile[sizeof(devConfigFile) - 1] = 0;
	strncat(devConfigFile, "config/f8-devices.inf", sizeof(devConfigFile));
	devConfigFile[sizeof(devConfigFile) - 1] = 0;
	
	// search hardware database
	GetPrivateProfileString(
		a.argv[1],
		"PlcDriver",
		"",
		buf,
		sizeof(buf),
		devConfigFile
		);
	if(!buf[0]){
		utils_error("Device %s not configured properly, PlcDriver entry not found\n", name);
		return F8_OBJECT_NOT_FOUND;
	}
	
	// ok, try load the driver if necessary
	drv = probeDriver(buf);

	// create the device node
	dev = (struct splc_device_t *)malloc(sizeof(struct splc_device_t));
	if(!dev){
		return F8_LOW_MEMORY;
	}

	memset(dev, 0, sizeof(*dev));
	dev->driver = drv;
	
	strcpy(configKey, cname);
	strncat(configKey, ".config", sizeof(configKey));
	configKey[sizeof(configKey) - 1] = 0;
	GetPrivateProfileString(
		SECTION_NAME,
		configKey,
		"",
		buf,
		sizeof(buf),
		get_config_file()
		);
	dev->name = (char *)alloc_buffer_v(
		name, strlen(name) + 1, 
		buf, strlen(buf) + 1, 
		0
		);
	dev->init_string = dev->name + strlen(dev->name) + 1;
	if(drv){
		drv->refCount++;
		RtkInsertTailList(&drv->devices, &dev->link);
	}else{
		RtkInitializeListHead(&dev->link);
	}

#define xx(n) n = GetPrivateProfileInt(a.argv[1], #n, 0, devConfigFile);

	xx(n_ai)
	xx(n_ao)
	xx(n_di)
	xx(n_do)
	xx(ai_width)
	xx(ao_width)

	dev->inputLength = (n_di >> 3) + n_ai * ai_width;
	dev->outputLength = (n_do >> 3) + n_ao * ao_width;

	DEBUG_PRINTF((
		"Device %s loaded, %d input, %d output.\n", 
		name,
		dev->inputLength,
		dev->outputLength
		));
	
	RtkInsertTailList(&devices, &dev->link2);

	if(d){
		*d = dev;
	}

	return F8_SUCCESS;
}

static f8_int startDevice(struct splc_device_t * dev)
{
	struct splc_driver_t * drv;
	f8_int ret = F8_BUSY;

	if(dev->status == SPLC_DEVICE_STARTED){
		return F8_ALREADY_STARTED;
	}
	
	drv = dev->driver;
	if(drv){
		if(drv->start){
			ret = drv->start(dev);
		}else{
			ret = F8_SUCCESS;
		}
	}

	if(F8_SUCCESSED(ret)){
		dev->status = SPLC_DEVICE_STARTED;
	}
	
	return ret;
}

static void probeDevices()
{
	char buf[1024];
	CArgs args;
	int i;
	
	GetPrivateProfileString(
		SECTION_NAME, 
		"devices", 
		"", 
		buf, 
		sizeof(buf), 
		get_config_file()
		);
	parse_arg_ex(buf, &args, ",");
	for(i=0; i<args.argc; i++){
		probeDevice(args.argv[i], 0);
	}
}

static f8_int stopDevice(struct splc_device_t * dev)
{
	f8_int ret = F8_SUCCESS;
	struct splc_driver_t * drv;
	
	drv = dev->driver;

	if(dev->status == SPLC_DEVICE_STOPPED){
		return F8_ALREADY_STOPPED;
	}
	
	if(drv){
		if(drv->stop){
			ret = drv->stop(dev);
		}
	}

	if(F8_SUCCESSED(ret)){
		dev->status = SPLC_DEVICE_STOPPED;
	}
	
	return F8_SUCCESS;
}

static f8_int removeDevice(struct splc_device_t * dev)
{
	struct splc_driver_t * drv;
	
	drv = dev->driver;

	assert(dev->status == SPLC_DEVICE_STOPPED);
	
	if(drv){
		assert(!RtkIsListEmpty(&drv->devices));
		RtkRemoveEntryList(&dev->link);
		
		drv->refCount--;
		if(!drv->refCount){
			assert(RtkIsListEmpty(&drv->devices));
			if(drv->unload){
				drv->unload(drv);
			}
			FreeLibrary(drv->moduleHandle);
			free_buffer(drv->name);
			free(drv);
		}
	}
	
	RtkRemoveEntryList(&dev->link2);
	free_buffer(dev->name);
	free(dev);

	return F8_SUCCESS;
}

static void startDevices()
{
	PRTK_LIST_ENTRY l;
	struct splc_device_t * dev;

	l = devices.Flink;
	while( l != &devices){
		dev = RTK_CONTAINING_RECORD(l, struct splc_device_t, link2);
		startDevice(dev);
		l = l->Flink;
	}
}

void stopDevices()
{
	PRTK_LIST_ENTRY l;
	struct splc_device_t * dev;
		
	l = devices.Flink;
	while( l != &devices){
		dev = RTK_CONTAINING_RECORD(l, struct splc_device_t, link2);
		stopDevice(dev);
		l = l->Flink;
	}
}

static void removeDevices()
{
	PRTK_LIST_ENTRY l;
	struct splc_device_t * dev;
		
	while( !RtkIsListEmpty(&devices)){
		l = devices.Flink;
		dev = RTK_CONTAINING_RECORD(l, struct splc_device_t, link2);
		removeDevice(dev);
	}
}

/*
	synchronizing io memory with device memory
*/
void process_input(struct kernel_t * kernel)
{
	PRTK_LIST_ENTRY l;
	struct splc_device_t * dev;
	f8_byte * io_mem;
	
	l = devices.Flink;
	while(l != &devices){
		dev = RTK_CONTAINING_RECORD(l, struct splc_device_t, link2);
	
		// synchronizing io memory with device memory
		
		io_mem = &kernel->io_memory[dev->memAddr][0];

		// DEBUG_PRINTF(("Synchroning input of device %s, %d bytes.\n", dev->name, dev->inputLength));
		
		memcpy(io_mem, dev->xMemory, dev->inputLength);

		l = l->Flink;
	}
}

/*
	synchronizing device memory with io memory
*/
void process_output(struct kernel_t * kernel)
{
	PRTK_LIST_ENTRY l;
	struct splc_device_t * dev;
	f8_byte * io_mem;
	
	l = devices.Flink;
	while(l != &devices){
		dev = RTK_CONTAINING_RECORD(l, struct splc_device_t, link2);
	
		// synchronizing io memory with device memory
		
		io_mem = &kernel->io_memory[dev->memAddr][0];
		memcpy(
			dev->xMemory + dev->inputLength, 
			io_mem + dev->inputLength, 
			dev->outputLength
			);

		l = l->Flink;
	}
}

f8_int init_io()
{
	utils_debug("Initializing IO layer for softplc\n");
	RtkInitializeListHead(&drivers);
	RtkInitializeListHead(&devices);
	// load devices
	probeDevices();
	// thread = rthre
	return F8_SUCCESS;
}

f8_int uninit_io()
{
	stopDevices();
	removeDevices();
	return F8_SUCCESS;
}

void start_io()
{
	startDevices();
}

void stop_io()
{
	stopDevices();
}

f8_status io_write_config(const struct io_device * d)
{
	return F8_UNIMPLEMENTED;
}

f8_status io_write_status()
{
	return F8_UNIMPLEMENTED;
}
