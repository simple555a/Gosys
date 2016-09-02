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
���ܣ��õ�ָ���豸��DRIVER_INFO��DEVICE_INFO������������豸�����ı�ǩ�㣬�����豸
������
       dev_name        ��̬���õ��豸����
	   bValidateTags   �����Ƿ�ִ���豸������ǩ������ӣ�1��ִ�У�0����ִ��
���أ��豸��ϢDEVICE_INFOָ��
      0        *.ini��û���ҵ���Ӧ���豸������Ϣ
	      ��   "cncs.f8"�ַ�������ȷ��
	      ��   ����ʵʱ���ڴ治�ɹ�
		  ��   ���ɵ��豸������Ϣָ��Ϊ0
	  ������Ϣ
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
	
	//1 ��ʵʱ�������ļ�*.ini�ж�ȡ�����豸����Ϣ
	GetPrivateProfileString(
		"devices",
		dev_name,
		"",
		dev_setting,
		sizeof(dev_setting),
		get_config_file()
		);

    //��","Ϊ�ָ��������ַ���dev_setting�ֳɼ��θ���ag_dev�ĳ�Աargv������������Աargc��arg_idx��args��ֵ
	//2 �õ�"CP01 = cncs.f8,127.0.0.1"����ʽ�ұߵ��ַ���
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

	//3 �õ�*.inf�ļ���"cncs"���豸���"f8"
	vt.parse(ag_dev.argv[0], ".");
	if(vt.argc != 2){
		utils_error(
			"Invalid device setting for '%s':%s\n", 
			dev_name, 
			dev_setting
			);
		return 0;
	}
	
	//����
	if(!lock_rtdb(__true, 1000)){
		return 0;
	}
	
	//4 ��infs�ļ���v.inf�ļ�����ȡ��Ϣ������DRIVER_INF,�ٰ�DIRVER_INFO����g_drivers��������
	pDriver = io_load_driver(
			&(VENDOR_KEY)CVendorName(vt.argv[0]),     //*.inf�ļ���cncs
			&(DEVTYPE_KEY)CDevTypeName(vt.argv[1])    //�豸���
			);
	if(!pDriver){
		utils_error("Cannot load IO driver %s\n", ag_dev.argv[0]);
	}

	// 2003/4/21, the device object must be created even if the settings
	// are invalid, thus giving user a chance to correct the error.
	// create the device
	//5 ���豸���ƺ�������Ϣ�����豸��Ϣ������DEVICE_LIST  g_Devices�У�������DEVICE_INFOָ��
	pDevice = io_create_device(
		pDriver,
		&(DEVICE_KEY)CDeviceName(dev_name)
		);
	if(!pDevice){
		utils_error("Cannot create device %s\n", dev_name);
		//����
		unlock_rtdb();
		return 0;
	}
    
	//6 ���Ʋ����DEVICE_INFO
	if(pDriver){
		pDevice->v = pDriver->vendor;
		pDevice->t = pDriver->type;
		strncpy(
			pDevice->address, 
			ag_dev.argv[1], 
			sizeof(pDevice->address)
			);//��¼�豸��ip��ַ��DEVICE_INFO
		pDevice->address[sizeof(pDevice->address) - 1] = 0;
		//�õ�ini�ļ�ip��ַ֮����豸������Ϣ��ĳЩ�豸�У�ĳЩ�豸û�С�����������û�У�
		if(ag_dev.argc > 2){
			strncpy(
				pDevice->parameter,
				dev_setting + ag_dev.arg_idx[2],
				sizeof(pDevice->parameter)
				);
		}		
		pDevice->parameter[sizeof(pDevice->parameter) - 1] = 0;
	}
    
	//7 ������pDevice�豸�����ı�ǩ��
	if(bValidateTags){
		attach_device_tags(pDevice);
	}
    
	//8 �����豸�����ĵ��������������豸
	if(!io_start_device(pDevice)){
		/*
			error happed during device loading.
			we keep this faulty device in memory, so that
			the configurator has a chance to correct the
			error.
		*/
		// io_delete_device(pDevice);			
	}
	
	//����
	unlock_rtdb();
	
	return pDevice;
}

/*
	enumerate the devices to be loaded and load them.
*/
/*
���ܣ�ȡ��*.ini�ļ���load=��������豸���ƣ����������豸��������Ϣ���豸��Ϣ�������豸
��������
���أ�1  __true  �ɹ�
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
���ܣ�ֹͣ�����豸�����ϵͳ�豸�б�g_Devices
��������
���أ�1     �ɹ�
*/
__bool pnp_stop_devices()
{
	PDEVICE_INFO dev;
	DEVICE_LIST::iterator it;
	DEVICE_LIST::iterator tmp;
	
	for(it = g_Devices.begin(); it != g_Devices.end(); ){
		dev = it->second;
		//ֹͣ�豸
		if(io_stop_device(dev)){
			it++;
			io_delete_device(dev);//�豸ֹͣ���ٽ��豸��Ϣ��ϵͳ�豸�б���ɾ��
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

