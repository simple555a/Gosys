#ifndef _NICIOSSPROXY_H_
#define _NICIOSSPROXY_H_


#include "nicidl/NicSys_RTDBC.h"
#include "nicidl/NicSys_IOSSC.h"
#include "orbsvcs/CosNamingC.h"
#include "nicDefs.h"
#include "nicRefTAO.h"

#ifdef _WIN32
#ifdef	NICIOSSPROXY_EXPORTS
#define IOSSPROXY_API	  __declspec(dllexport)
#else
#define IOSSPROXY_API	  __declspec(dllimport)
#endif
#else
#define IOSSPROXY_API
#endif


// 初始化iossproxy
IOSSPROXY_API void init_notify_iossproxy(int argc, char * argv[]);


// 反初始化proxyioss
IOSSPROXY_API void fini_notify_iossproxy();


// 添加订阅
//输入参数：事件名称、类型、回调函数
IOSSPROXY_API void add_event_iossproxy(char * domain_name, char * type_name, NIC_EVENT_HANDLER_FUNC func);

// 减少订阅
//输入参数：事件名称、类型
IOSSPROXY_API void del_event_iossproxy(char * domain_name, char * type_name);


//-------------------------------------------------------------------------------------------------------
//IOSS接口调用函数
//--------------------------------------------------------------------------------------------------------
//客户端添加设备
//输入参数：设备名、生产厂商、设备型号、地址、初始化命令
IOSSPROXY_API nicAPIStatus Proxy_IOSS_AddDevice(nicIN const char * T_devName,
											nicIN const char * T_venDesp,
											nicIN const char * T_devDesp,
											nicIN const char * T_addr,
											nicIN const char * T_initCmd);
//删除一个设备信息
//输入参数：待删除的设备名称  devName
IOSSPROXY_API nicAPIStatus Proxy_IOSS_DeleteDevice(nicIN const char * T_devName);


//得到infs文件夹中厂商下的设备描述列表
//输出参数：所有厂商描述
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetAllVenDesp(nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);
//
//
//得到infs文件夹中厂商下的设备描述列表
//输入参数：venDesp  厂商描述
//输出参数：厂商的所有设备的描述
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevDespByVen(nicIN const char * T_venDesp,
											     nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);
//
//
//得到当前工程有加载的设备名称
//输出参数：得到配置文件(*.ini)中的设备名称
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevName(nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);

//得到指定设备的配置信息
//输入参数：设备名称
//输出参数：设备厂商、设备类型、设备地址、初始化命令
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevInfo(nicIN const char * T_devName,
													nicOUT char *& T_venDesp,
													nicOUT char *& T_devDesp,
													nicOUT char *& T_devAddr,
													nicOUT char *& T_devInit);
//
//
//
//保存设备可修改的配置信息
//输入参数：设备名称、可修改的配置信息（设备地址、初始化命令）
IOSSPROXY_API nicAPIStatus Proxy_IOSS_SaveDevInfo(nicIN const char * T_devName,
													nicIN const char * T_devAddr,
													nicIN const char * T_devInit);

//启动设备
//输入参数：设备名称
IOSSPROXY_API nicAPIStatus Proxy_IOSS_StartDev(nicIN const char * T_devName);

//停止设备
//输入参数：设备名称
IOSSPROXY_API nicAPIStatus Proxy_IOSS_StopDev(nicIN const char * T_devName);



#endif