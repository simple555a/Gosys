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
���ܣ������豸������DLL�����������Ϣ����DRIVER_INFO
������
      driver     �����洢���غ����õ�������Ϣ
���أ�
      0      ��������DLLʧ��
	  1      �ɹ�
*/
__bool _load_module(DRIVER_INFO & driver)
{
	HINSTANCE h;

	utils_trace("Loading IO driver %s\n", driver.dllname);
    //1 ����ǰ���Ƚ������ı�ʶ����Ϊδ����
	driver.flags &= ~DRIVER_FLAG_LOADED;
	
	//2 �����豸������dll
	h = LoadLibrary(driver.dllname);
	
	//3 �����ز��ɹ�������dll������Ӻ�׺�������һ�Σ�����ʲô���ã�
	if( !h ){
		strncat(driver.dllname, DLL_SUFFIX, sizeof(driver.dllname));
		driver.dllname[ sizeof(driver.dllname) - 1] = 0;
		h = LoadLibrary(driver.dllname);
	}
    
	if( !h ){
		utils_error("Cannot load driver %s.\n", driver.dllname); 
		return __false;
	}
	
	driver.plugin_handle = (void*)h;  //hΪ����LoadLibrary()���ص��豸����dll�ľ��
	//4 ������Ϣ��ʶ��Ϊ�Ѽ���
	driver.flags |= DRIVER_FLAG_LOADED;

#define check_old(p) assert(!GetProcAddress(h, #p))
	check_old(init);
	check_old(start);
	check_old(stop);
    
    //��dll��������ָ�븳��DRIVER_INFO   driver��
	(FARPROC&)driver.load = GetProcAddress(h, "load");//�õ��豸����dll��������load()��ָ��
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
	//ȡ���豸�����汾������������DRIVER_INFO��
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
���ܣ����豸�б�g_Devices�õ�ָ���豸��������Ϣ
������
      key   �豸����
���أ�
      �豸key��������ϢDRIVER_INFO��ָ��     �ɹ�
	  0                                      ʧ��
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
���ܣ���infs�ļ���v.inf�ļ�����ȡ��Ϣ������driver,�ٰ�driver����g_drivers
������v--*.inf�ļ���
           t--[]����豸�������
���أ��豸�������t��Ӧ��������Ϣ
*/
IO_API PDRIVER_INFO PMC_API io_load_driver(PCVENDOR_KEY v, PCDEVTYPE_KEY t)
{		
	DRIVER_LIST::iterator it;
	DRIVER_INFO driver;
	string		infofile;
    
	//1 ���������Ϣ�ṹ��
	ZeroMemory(&driver, sizeof(DRIVER_INFO));
    
    //2 �ҵ�*.inf�ļ���ȡ������DLL����
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
	
	//3 ��������Ϣ���������б�g_Drivers�д��ڣ��򷵻ظ�������Ϣ
	for(it = g_Drivers.begin(); it != g_Drivers.end(); it++){
		if( !strnicmp(it->dllname, driver.dllname, sizeof(it->dllname)) ){
			return &(*it);  
		}
	}	
    
	//4 ��inf�ļ����������������DRIVER_INFO--driver
	driver.vendor.key = *v;
	driver.type.key = *t;
    
	//5 ȡ����˾����
	GetPrivateProfileString(
		"oem",
		"vendor",
		"",
		driver.vendor.description,
		sizeof(driver.vendor.description),
		infofile.data()
		);
	//6 ȡ���豸��ƶ�Ӧ��ȥ��������
	GetPrivateProfileString(
		(char*)CDevTypeName(*t),
		"description",
		"",
		driver.type.description,
		sizeof(driver.type.description),
		infofile.data()
		);
	
	//7 ��������dll����������Ϣ����driver������������dll��������load()��������
	if(_load_module(driver)){
		if(driver.load){
			driver.load(&driver);
		}
	}
	
	//8 �����õ�������Ϣdriver���������б�g_Drivers��
	it = g_Drivers.insert(g_Drivers.end(), driver);
	//??Ϊʲô�¼����������Ҫж��һ�£�
	if(it == g_Drivers.end()){
		io_unload_driver(&driver);
		return 0;
	}
	
	return &(*it);
}

/*
���ܣ�ж���豸��������ɾ�����������б��е���Ϣ
������
      driver   ����ж�ص�������Ϣָ��
���أ�
      0  __false     �����б���û�и�������Ϣ
	             ��  ʵʱ���������Ÿ�������Ӧ���豸
				 ��  ����dll������ж�غ���unload()ִ��ʧ��
	  1  __true      �ɹ�
*/
IO_API __bool PMC_API io_unload_driver(PDRIVER_INFO driver)
{
	DRIVER_LIST::iterator it;

	utils_trace("Unloading driver %s\n", driver->dllname);
	//1 �������б����ҵ�������Ϣ
	for(it = g_Drivers.begin(); it != g_Drivers.end(); it++){
		if(&(*it) == driver){      //ָ����ȣ�����
			break;
		}
	}
	//2 �ų������б����Ҳ���������Ϣ�����
	if(it == g_Drivers.end()){
		DEBUG_PRINTF((
			"Warning : unload a non-existing driver : 0x%08x\n", driver
			));
		return __false;
	}
    
	//3 ��ϵͳ�л������������Ӧ���豸������ж������
	if(driver->device_count){
		utils_error(
            "Driver %s cannot unload with %d devices active.\n",
            driver->dllname,
            driver->device_count
            );
		return __false;
	}
    
	//4 ��������dll�ĵ�������unload()��ж������
	if(driver->unload){
		//unload()����0�����ʾж�ز��ɹ�
		if(!driver->unload()){  //����ʲô�������豸������dll����
			DEBUG_PRINTF((
				"Error : driver %s rejected unload request.\n",
				driver->dllname
				));
			return __false;
		}
	}
    
    //5 ж������dll
	if(driver->plugin_handle){
		FreeLibrary((HINSTANCE)driver->plugin_handle);//decrements the reference count of the loaded dynamic-link library (DLL) module
	}

	//6 �ͷ�g_Drivers����driver��ص���Ϣ
	g_Drivers.erase(it);
	return __true;
}

