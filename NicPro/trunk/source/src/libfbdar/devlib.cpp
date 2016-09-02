#include "precomp.h"

/*
	2004/8
	the device library is hardcoded for now, a quick & dirty implementation
*/
static devlib_ent_t device_lib[128];

#define NUM_DEVICE_LIB (sizeof(device_lib) / sizeof(device_lib[0]))

FBDAR_API void blklib_enum_available_devices(
	ENUM_AVAILABLE_DEV_PROC p,
	__uint context)
{
	devlib_ent_t * d;
	int i;

	d = device_lib;
	for(i=0; i<NUM_DEVICE_LIB; i++, d++){
		if(!d->name){
			break;
		}
		if(!p(d, context)){
			break;
		}
	}
}

FBDAR_API struct devlib_ent_t * blklib_get_dev_by_name(const char * name)
{
	int i;
	for(i=0; i<NUM_DEVICE_LIB; i++){
		if(device_lib[i].name && !strcmpi(device_lib[i].name, name)){
			return &device_lib[i];
		}
	}
	return 0;
}

FBDAR_API struct devlib_ent_t * blklib_get_dev_by_id(const f8_uuid * id)
{
	int i;
	for(i=0; i<NUM_DEVICE_LIB; i++){
		if(device_lib[i].dev.clsid == *id){
			return &device_lib[i];
		}
	}
	return 0;
}

void blklib_load_devices()
{
	string f;
	char devices[1024];
	char * dName;
	devlib_ent_t * d;
	char str[1024];
	
	f = string(get_working_dir()) + "config/f8-devices.inf";
	GetPrivateProfileSectionNames(devices, sizeof(devices), f.c_str());
	dName=devices;
	d = device_lib;
	while(*dName){
		if(stricmp(dName, "OEM")){
			d->name = (char*)alloc_buffer_v(dName, strlen(dName) + 1, 0);
			
			GetPrivateProfileString(dName, "description", "", str, sizeof(str), f.c_str());
			d->description = (char*)alloc_buffer_v(str, strlen(str) + 1, 0);
			
			GetPrivateProfileString(dName, "id", "", str, sizeof(str), f.c_str());
			f8_uuid_from_string(&d->dev.clsid, str);
			
			GetPrivateProfileString(dName, "n_di", "0", str, sizeof(str), f.c_str());
			d->dev.n_di = atoi(str);
			GetPrivateProfileString(dName, "n_do", "0", str, sizeof(str), f.c_str());
			d->dev.n_do = atoi(str);
			GetPrivateProfileString(dName, "n_ai", "0", str, sizeof(str), f.c_str());
			d->dev.n_ai = atoi(str);
			GetPrivateProfileString(dName, "n_ao", "0", str, sizeof(str), f.c_str());
			d->dev.n_ao = atoi(str);
			GetPrivateProfileString(dName, "ai_width", "0", str, sizeof(str), f.c_str());
			d->dev.ai_width = atoi(str);
			GetPrivateProfileString(dName, "ao_width", "0", str, sizeof(str), f.c_str());
			d->dev.ao_width = atoi(str);

			if(d->dev.n_di != 0|| d->dev.n_ai != 0){
				d->dev.features |= IO_DEVICE_INPUT;
			}
			if(d->dev.n_do != 0|| d->dev.n_ao != 0){
				d->dev.features |= IO_DEVICE_OUTPUT;
			}
			
			
			d++;
		}
		dName += strlen(dName) + 1;
	}
	
	return;
}

void blklib_unload_devices()
{
	int i;
	devlib_ent_t * d;
	for(i = 0, d = device_lib; i<NUM_DEVICE_LIB; i++, d++){
		if(d->name){
			free_buffer(d->name);
			d->name = 0;
		}
		if(d->description){
			free_buffer(d->description);
			d->description = 0;
		}
	}
}

