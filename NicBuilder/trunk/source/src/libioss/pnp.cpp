#include "precomp.h"

__bool pnp_init()
{
	return __true;
}

__bool pnp_uninit()
{
	return __true;
}

/*
	io_probe_device

	The function is named after a linux kernel function, mod_probe, :-)

	Parameters:

	dev_name --- gives the name of the device to be loaded. the device's
	configuration is loaded from the config registry.
	
	bValidateTags	--- if specified, the database is searched, the tags
	pertaining to this device are attached to this device. this allows
	us to mount a device at runtime, well after the database subsystem
	is already started.

	NOTE : the bValidateTags is specified in the case of delayed device
	loading, i.e., loading device after the database is already started. the
	device loader has to take the responsibility to attach the tags to this
	device. While in earlier stage of system initialization, the pnp_probe_devices
	don't use this flag, because the database subsystem will make the 
	attachment at a later phase.

	For now, we don't see any need to load a device in a way other than
	the two above stated patterns.

	If you're familiar with the Windows Driver Model, you may have correctly
	sensed that what we implement here is just the equivalence of NT's
	unkown bus (enumerator) driver.
*/
/*
功能：得到指定设备的DRIVER_INFO和DEVICE_INFO，并连接与该设备关联的标签点，启动设备
参数：
       dev_name        组态设置的设备名称
	   bValidateTags   决定是否执行设备关联标签点的连接，1：执行；0：不执行
返回：设备信息DEVICE_INFO指针
      0        *.ini中没有找到对应的设备配置信息
	      或   "cncs.f8"字符串不正确：
	      或   锁定实时库内存不成功
		  或   生成的设备驱动信息指针为0
	  驱动信息
*/
IO_API PDEVICE_INFO PMC_API io_probe_device(
	const char * dev_name, 
	__bool bValidateTags
	)
{	
	CArgsEx ag_dev;
	PDRIVER_INFO pDriver;
	PDEVICE_INFO pDevice;
	CArgsEx vt; // vendor and type
	char dev_setting[256];
	
	//1 从实时库配置文件*.ini中读取所有设备的信息
	GetPrivateProfileString(
		"devices",
		dev_name,
		"",
		dev_setting,
		sizeof(dev_setting),
		get_config_file()
		);

    //以","为分隔符，将字符串dev_setting分成几段赋给ag_dev的成员argv，并给其他成员argc、arg_idx、args赋值
	//2 得到"CP01 = cncs.f8,127.0.0.1"，等式右边的字符串
	ag_dev.parse(dev_setting, ",", MAX_ARGC, MAX_LINE_LENGTH, false);
	if(ag_dev.argc < 1){
		// line syntax: vendor.type, bus-address, parameters
		/*
			anything beyond the third ',' segmented part constitutes
			the device parameters.
		*/
		utils_error(
			"Invalid device setting for '%s':%s\n", 
			dev_name, 
			dev_setting
			);
		return 0;
	}

	//3 得到*.inf文件名"cncs"和设备简称"f8"
	vt.parse(ag_dev.argv[0], ".");
	if(vt.argc != 2){
		utils_error(
			"Invalid device setting for '%s':%s\n", 
			dev_name, 
			dev_setting
			);
		return 0;
	}
	
	//锁定
	if(!lock_rtdb(__true, 1000)){
		return 0;
	}
	
	//4 在infs文件的v.inf文件中提取信息，赋给DRIVER_INF,再把DIRVER_INFO插入g_drivers，并返回
	pDriver = io_load_driver(
			&(VENDOR_KEY)CVendorName(vt.argv[0]),     //*.inf文件名cncs
			&(DEVTYPE_KEY)CDevTypeName(vt.argv[1])    //设备简称
			);
	if(!pDriver){
		utils_error("Cannot load IO driver %s\n", ag_dev.argv[0]);
	}

	// 2003/4/21, the device object must be created even if the settings
	// are invalid, thus giving user a chance to correct the error.
	// create the device
	//5 由设备名称和驱动信息生成设备信息，插入DEVICE_LIST  g_Devices中，并返回DEVICE_INFO指针
	pDevice = io_create_device(
		pDriver,
		&(DEVICE_KEY)CDeviceName(dev_name)
		);
	if(!pDevice){
		utils_error("Cannot create device %s\n", dev_name);
		//解锁
		unlock_rtdb();
		return 0;
	}
    
	//6 完善并填充DEVICE_INFO
	if(pDriver){
		pDevice->v = pDriver->vendor;
		pDevice->t = pDriver->type;
		strncpy(
			pDevice->address, 
			ag_dev.argv[1], 
			sizeof(pDevice->address)
			);//记录设备的ip地址到DEVICE_INFO
		pDevice->address[sizeof(pDevice->address) - 1] = 0;
		//得到ini文件ip地址之后的设备参数信息，某些设备有，某些设备没有。（控制器就没有）
		if(ag_dev.argc > 2){
			strncpy(
				pDevice->parameter,
				dev_setting + ag_dev.arg_idx[2],
				sizeof(pDevice->parameter)
				);
		}		
		pDevice->parameter[sizeof(pDevice->parameter) - 1] = 0;
	}
    
	//7 连接与pDevice设备关联的标签点
	if(bValidateTags){
		attach_device_tags(pDevice);
	}
    
	//8 调用设备驱动的导出函数，启动设备
	if(!io_start_device(pDevice)){
		/*
			error happed during device loading.
			we keep this faulty device in memory, so that
			the configurator has a chance to correct the
			error.
		*/
		// io_delete_device(pDevice);			
	}
	
	//解锁
	unlock_rtdb();
	
	return pDevice;
}

/*
	enumerate the devices to be loaded and load them.
*/
/*
功能：取出*.ini文件中load=后的所有设备名称，声称所有设备的驱动信息和设备信息，启动设备
参数：无
返回：1  __true  成功
*/
__bool pnp_probe_devices()
{
	int i;
	char dev_list[1024], *dev_name;
	CArgsEx ag_loaded;	

	GetPrivateProfileString(
		"devices",
		"load",
		"",
		dev_list,
		sizeof(dev_list),
		get_config_file()
		);
	ag_loaded.parse(dev_list);

	for(i=0; i<ag_loaded.argc; i++){
		dev_name = ag_loaded.argv[i];
		io_probe_device(dev_name, __false);
	}

	return __true;
}

/*
功能：停止所有设备，清空系统设备列表g_Devices
参数：无
返回：1     成功
*/
__bool pnp_stop_devices()
{
	PDEVICE_INFO dev;
	DEVICE_LIST::iterator it;
	DEVICE_LIST::iterator tmp;
	
	for(it = g_Devices.begin(); it != g_Devices.end(); ){
		dev = it->second;
		//停止设备
		if(io_stop_device(dev)){
			it++;
			io_delete_device(dev);//设备停止后，再将设备信息从系统设备列表中删除
		}
	}

	if(g_Drivers.size()){
		utils_error(
			"%d drivers are still in memory after stopping command.\n",
			g_Drivers.size()
			);
	}

	return __true;
}

