/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	ioss.cpp
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	IO subsystem
	history:
	created:	10:9:2002	14:37
*********************************************************************/
#include "precomp.h"




#ifdef _WIN32
#pragma comment(linker, "/base:0x76560000")
#endif

__r4	g_fltMaxChangeRate;
__r4	g_fltTagLife;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
功能：改变所有设备的power state
参数：
      newState     设备将要达到的power state
	  context      函数体中木有用到。。。。。？？？？？
返回：0   __flase    失败
      1   __true     成功
*/
static __bool PMC_API _power_callback(int newState, int context)
{
	PDEVICE_INFO dev;
	DEVICE_LIST::iterator it;
	PDRIVER_INFO drv;
	IOSS_STATUS code;
    
	//遍历设备列表中的所有设备
	for(it = g_Devices.begin(); it != g_Devices.end(); it++){
		dev = it->second;//设备信息
		drv = dev->d;//驱动信息
		
		//调用驱动dll导出函数改变power state
		if( (dev->flags & DF_Active) && (drv->dispatch)){
			code = drv->dispatch(dev, DISP_POWER_STATE_CHANGED, newState);
			if(code){
				utils_error(
					"Error : device %s vetoed `power-state-changed' command,"
					"error code 0x%08x.\n"
					"This may cause unpredictable result.\n",
					(char*)CDeviceName(dev->k),
					code
					);
				return __false;
			}
		}
	}

	return __true;
}

/*
功能：读取配置文件[PMC]下的MaxChangeRate和TagLife
参数：无
返回：无
*/
static void _load_settings()
{
	char buf[32];

	*buf = 0;
	GetPrivateProfileString(
		"PMC",
		"MaxChangeRate",
		"0.01",
		buf,
		sizeof(buf),
		get_config_file()
		);
	g_fltMaxChangeRate = (__r4)atof(buf);
	if(g_fltMaxChangeRate < 0.0001 || g_fltMaxChangeRate > 5){
		utils_error(
			"MaxChangeRate=%.3f%% is not valid.\n",
			g_fltMaxChangeRate
			);
		g_fltMaxChangeRate = (__r4)rtkm_default_change_rate;
	}
	utils_trace("MaxChangeRate is set to %.3f%% \n", g_fltMaxChangeRate);
	
	*buf = 0;
	GetPrivateProfileString(
		"PMC",
		"TagLife",
		"60",
		buf,
		sizeof(buf),
		get_config_file()
		);
	g_fltTagLife = (__r4)atof(buf);
	if(g_fltTagLife < 5){
		utils_error(
			"TagLife=%.1fs is not valid, reset to 5s.\n", 
			g_fltTagLife
			);
	}
	utils_trace("TagLife is set to %.1fs\n", g_fltTagLife);
}

/*
功能：初始化IOSS模块
参数：无
*/
IO_API __bool PMC_API init_ioss()
{


	_load_settings();//通过pmc.in或程序默认，给几个相关变量赋初值g_fltMaxChangeRate、g_fltTagLife

	register_power_callback(_power_callback, 0);//用形参对CALLBACK_ITEM的成员赋值，并链接到RTK_LIST_ENTRY
	
	pnp_probe_devices();//加载设备驱动DLL，并得到其导出函数的指针

	if(!CDBRefresher::init()){//add本地节点、组和标签；对g_Handlers[]中的函数指针赋值；开启一个线程
		utils_error("Global initialization of CRefresher failed.\n");
		return __false;
	}	

	return __true;
}

/*
功能：注销IOSS模块
参数：无
返回：无
*/
IO_API void PMC_API uninit_ioss()
{
	//1 锁定实时库内存
	if(!lock_rtdb(__true, 1000)){
		return;
	}

	//2 注销数据刷新过程--删除实时库分派结构、关闭线程、清空类CDBRefresher中的组信息列表
	g_Refresher.uninit();

	//3 停止所有设备，清空系统设备列表g_Devices
	pnp_stop_devices();

	//4 解除回调项CALLBACK_ITEM
	unregister_power_callback(_power_callback, 0);
	
	//5 解锁实时库内存
	unlock_rtdb();



//	ACE_OS::sleep(5);

}

/*
功能：调用设备驱动导出函数，将单点单实时值写入设备
参数：
      p       待写入的指定标签点数据结构RTK_TAG指针
	  value   待写入的指定标签点的实时值
返回：即设备驱动导出函数write_device()的返回值
*/
static __bool _writeDevice(PRTK_TAG p, PCTAG_VALUE value)
{
	PDRIVER_INFO npu;
	__bool retval = false;
	
	// p->d.DeviceObj = io_open_device(&p->s.Device);
	if(p->d.DeviceObj){
		npu = ((PDEVICE_INFO)p->d.DeviceObj)->d;
		if(npu){
			if(npu->write_device){
				retval = npu->write_device(p, value);
			}
		}
	}

	return retval;
}

/*
功能：将TAG_VALUE实时值写入设备。
参数：
      name  待写入实时值对应的   组名.标签名
	  value 待写入的实时值
返回：即设备驱动导出函数write_device()的返回值
*/
IO_API __bool PMC_API io_write_device(
	PCSHORT_TAG_NAME name, 
	PCTAG_VALUE value
	)
{
	__bool retval = __false;
    
	//1 锁定
	if(!lock_rtdb(__false, 100)){
		return __false;
	}
	//2 找到本地节点中的指定标签点的RTK_TAG，其中存有该标签点对应的设备信息
	PRTK_TAG p = query_tag(HNODE_LOCAL_MACHINE, name);
	
	//3 将单点单实时值写入设备
	if(p && !(p->d.Value.Flags & TF_ReadOnly)){
	#if 0
		{
			char valbuf[32];
			if(get_value_type(p->s.Flags) == dt_real4){
				sprintf(valbuf, "%f", value->fltValue);
			}else{
				sprintf(valbuf, "%s", value->iValue? "On" : "Off");
			}
			utils_debug(
				"WriteDevice(%s.%s)=%s\n", 
				(char *)CGroupName(name->group),
				(char *)CTagName(name->tag),
				valbuf
				);
		}
	#endif
		//调用设备驱动导出函数，将单点单实时值写入设备
		retval = _writeDevice(p, value);
	}
	//4 解锁
	unlock_rtdb();

	return retval;
}

/*
功能：将pmc_value_t实时值写入设备
参数：
      name    待写入实时值对应的   组名.标签名
	  value   待写入的实时值
返回：即设备驱动导出函数write_device()的返回值
*/
IO_API __bool PMC_API io_write_device_ex(
	PCSHORT_TAG_NAME name, 
	PCpmc_value_t value
	)
{
	pmc_value_t value2;
	__bool retval = __false;

	if(!lock_rtdb(__false, 100)){
		return __false;
	}
	PRTK_TAG p = query_tag(HNODE_LOCAL_MACHINE, name);
	if(p && !(p->d.Value.Flags & TF_ReadOnly)){
		ZeroMemory(&value2, sizeof(value2));
		set_value_type(value2.Flags, get_value_type(p->s.Flags));
		if(pmc_type_cast(value, &value2)){
			retval = _writeDevice(p, &value2.Value);
		}
	}
	unlock_rtdb();

	return retval;
}

/*
功能：从设备驱动中更新并得到实时值
参数：
      tag       更新的标签结构体，包含更新实时值所需要的配置信息[in]
	  new_value  实时值指针，用于存储更新所得到的实时值[in, out]
	  now        更新时间[in]
返回：
      1        成功
	  0       tag中动态属性的设备信息丢失
	       或 tag中动态属性设备信息的驱动信息丢失
		   或 tag中驱动信息中的更新tag的导出函数丢失
*/
IO_API __bool PMC_API io_update_tag(
	PCRTK_TAG tag, 
	Ppmc_value_t new_value,
	PCRTK_TIME now
	)
{
	PDRIVER_INFO drv;
    
	//1 设置新实时值的Flags
	new_value->Flags = 0;
	set_value_type(new_value->Flags,  get_value_type(tag->s.Flags));
	
	//2 将实时值的union清0，u64是union中的最大元素，即可表示union的大小
	/* 
	pre-set all unused bytes to zero, this will make it easier to
	write code depending on the tag types
	*/
	new_value->Value.u64 = 0;
	
	// assert(tag->d.DeviceObj == io_open_device(&tag->s.Device));

    //3 从标签的动态属性中得到设备驱动信息的结构体
	if(!tag->d.DeviceObj){
		return __false;
	}
	drv = ((PDEVICE_INFO)tag->d.DeviceObj)->d;
	if(!drv){
		return __false;
	}

	//4 调用设备驱动DLL的导出函数更新实时值
	if(!drv->update_tag){
		return __false;
	}
	
	if(drv->update_tag){
		drv->update_tag(tag, new_value, now);
	}
    
	//5 某些设备驱动可能会重写实时值的类型标识，需要再用标签静态属性中的类型标识设置一下
	// some ill-behavioured driver will overwrite the type field
	set_value_type(new_value->Flags, get_value_type(tag->s.Flags));
	
	//6 若静态属性配置为开关量，则将true转化为1，false转化为0
	if(get_value_type(tag->s.Flags) == dt_bool){
		/* make digital variable cannonical */
		new_value->Value.b = new_value->Value.b? 1 : 0;
	}
	
	return __true;
}

