#include "precomp.h"

DRIVER_LIST g_Drivers;
static RTDB_DISPATCH_TABLE s_Handler;

#ifndef _WIN32
#define LoadLibrary(name) dlopen(name, 0)
#define HINSTANCE void *
#define GetProcAddress(handle, name) dlsym(handle, name)
#define FARPROC void *
#define FreeLibrary(handle) dlclose(handle)
#endif

#ifdef _WIN32
#define DLL_SUFFIX ".DLL"
#else
#define DLL_SUFFIX ".so"
#endif

/*
功能：加载设备驱动的DLL，并将相关信息填入DRIVER_INFO
参数：
      driver     用来存储加载后所得的驱动信息
返回：
      0      加载驱动DLL失败
	  1      成功
*/
__bool _load_module(DRIVER_INFO & driver)
{
	HINSTANCE h;

	utils_trace("Loading IO driver %s\n", driver.dllname);
    //1 加载前，先将驱动的标识设置为未加载
	driver.flags &= ~DRIVER_FLAG_LOADED;
	
	//2 加载设备驱动的dll
	h = LoadLibrary(driver.dllname);
	
	//3 若加载不成功？？对dll名称添加后缀，再添加一次（？？什么作用）
	if( !h ){
		strncat(driver.dllname, DLL_SUFFIX, sizeof(driver.dllname));
		driver.dllname[ sizeof(driver.dllname) - 1] = 0;
		h = LoadLibrary(driver.dllname);
	}
    
	if( !h ){
		utils_error("Cannot load driver %s.\n", driver.dllname); 
		return __false;
	}
	
	driver.plugin_handle = (void*)h;  //h为函数LoadLibrary()返回的设备驱动dll的句柄
	//4 驱动信息标识设为已加载
	driver.flags |= DRIVER_FLAG_LOADED;

#define check_old(p) assert(!GetProcAddress(h, #p))
	check_old(init);
	check_old(start);
	check_old(stop);
    
    //将dll导出函数指针赋给DRIVER_INFO   driver中
	(FARPROC&)driver.load = GetProcAddress(h, "load");//得到设备驱动dll导出函数load()的指针
	(FARPROC&)driver.unload  = GetProcAddress(h, "unload");
	(FARPROC&)driver.start_device = GetProcAddress(h, "start_device");
	(FARPROC&)driver.stop_device = GetProcAddress(h, "stop_device");
	(FARPROC&)driver.address_translate = GetProcAddress(h, "address_translate");
	(FARPROC&)driver.update_tag = GetProcAddress(h, "update_tag");
	(FARPROC&)driver.write_device = GetProcAddress(h, "write_device");
	(FARPROC&)driver.dispatch = GetProcAddress(h, "dispatch");
	if(!driver.dispatch){
		(FARPROC&)driver.dispatch = GetProcAddress(h, "_dispatch@12");
	}
#ifdef _WIN32
	//取出设备驱动版本和描述，赋到DRIVER_INFO中
	get_file_version(driver.dllname, (PLARGE_INTEGER)&driver.version);
	get_file_description(
        driver.dllname,
        driver.description,
        sizeof(driver.description)
        );
#else
    //
#endif
	utils_trace("IO driver %s loaded at 0x%08x\n", driver.dllname, driver.plugin_handle);

	return __true;
}

/*
功能：从设备列表g_Devices得到指定设备的配置信息
参数：
      key   设备名称
返回：
      设备key的配置信息DRIVER_INFO的指针     成功
	  0                                      失败
*/
PDRIVER_INFO get_driver(PCDEVICE_KEY key)
{
	PDEVICE_INFO dev;
	dev = io_open_device(key);
	if(!dev){
		return 0;
	}
	return dev->d;
}

/*
功能：在infs文件的v.inf文件中提取信息，赋给driver,再把driver插入g_drivers
参数：v--*.inf文件名
           t--[]里的设备驱动简称
返回：设备驱动简称t对应的驱动信息
*/
IO_API PDRIVER_INFO PMC_API io_load_driver(PCVENDOR_KEY v, PCDEVTYPE_KEY t)
{		
	DRIVER_LIST::iterator it;
	DRIVER_INFO driver;
	string		infofile;
    
	//1 清空驱动信息结构体
	ZeroMemory(&driver, sizeof(DRIVER_INFO));
    
    //2 找到*.inf文件，取出驱动DLL名称
	infofile = get_exe_dir();
	infofile += "infs/";
	infofile += (char*)CVendorName(*v);
	infofile += ".inf";
	GetPrivateProfileString(
		(char*)CDevTypeName(*t),
		"module", 
		"",
		driver.dllname,
		sizeof(driver.dllname),
		infofile.data()
		 );	
	
	//3 若驱动信息已在驱动列表g_Drivers中存在，则返回该驱动信息
	for(it = g_Drivers.begin(); it != g_Drivers.end(); it++){
		if( !strnicmp(it->dllname, driver.dllname, sizeof(it->dllname)) ){
			return &(*it);  
		}
	}	
    
	//4 将inf文件名和驱动简称填入DRIVER_INFO--driver
	driver.vendor.key = *v;
	driver.type.key = *t;
    
	//5 取出公司描述
	GetPrivateProfileString(
		"oem",
		"vendor",
		"",
		driver.vendor.description,
		sizeof(driver.vendor.description),
		infofile.data()
		);
	//6 取出设备简称对应的去驱动描述
	GetPrivateProfileString(
		(char*)CDevTypeName(*t),
		"description",
		"",
		driver.type.description,
		sizeof(driver.type.description),
		infofile.data()
		);
	
	//7 加载驱动dll，将驱动信息填入driver，并调用驱动dll导出函数load()下载驱动
	if(_load_module(driver)){
		if(driver.load){
			driver.load(&driver);
		}
	}
	
	//8 将填充好的驱动信息driver插入驱动列表g_Drivers中
	it = g_Drivers.insert(g_Drivers.end(), driver);
	//??为什么新加入的驱动需要卸载一下？
	if(it == g_Drivers.end()){
		io_unload_driver(&driver);
		return 0;
	}
	
	return &(*it);
}

/*
功能：卸载设备驱动，并删除其在驱动列表中的信息
参数：
      driver   待是卸载的驱动信息指针
返回：
      0  __false     驱动列表中没有该驱动信息
	             或  实时库仍连接着该驱动对应的设备
				 或  驱动dll导出的卸载函数unload()执行失败
	  1  __true      成功
*/
IO_API __bool PMC_API io_unload_driver(PDRIVER_INFO driver)
{
	DRIVER_LIST::iterator it;

	utils_trace("Unloading driver %s\n", driver->dllname);
	//1 在驱动列表中找到驱动信息
	for(it = g_Drivers.begin(); it != g_Drivers.end(); it++){
		if(&(*it) == driver){      //指针相等？？？
			break;
		}
	}
	//2 排除驱动列表中找不到驱动信息的情况
	if(it == g_Drivers.end()){
		DEBUG_PRINTF((
			"Warning : unload a non-existing driver : 0x%08x\n", driver
			));
		return __false;
	}
    
	//3 若系统中还有与该驱动对应的设备，则不能卸载驱动
	if(driver->device_count){
		utils_error(
            "Driver %s cannot unload with %d devices active.\n",
            driver->dllname,
            driver->device_count
            );
		return __false;
	}
    
	//4 调用驱动dll的导出函数unload()，卸载驱动
	if(driver->unload){
		//unload()返回0，则表示卸载不成功
		if(!driver->unload()){  //具体什么函数有设备驱动的dll而定
			DEBUG_PRINTF((
				"Error : driver %s rejected unload request.\n",
				driver->dllname
				));
			return __false;
		}
	}
    
    //5 卸载驱动dll
	if(driver->plugin_handle){
		FreeLibrary((HINSTANCE)driver->plugin_handle);//decrements the reference count of the loaded dynamic-link library (DLL) module
	}

	//6 释放g_Drivers中与driver相关的信息
	g_Drivers.erase(it);
	return __true;
}

