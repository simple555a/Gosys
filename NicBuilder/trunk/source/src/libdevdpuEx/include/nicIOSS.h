
#ifndef _NICSYS_IOSS_H__
#define _NICSYS_IOSS_H__

#include "nicDefs.h"
#include "nicPrecomp.h"
#include "nicIOSSComm.h"



#ifdef _WIN32
#ifdef	LIB_IOSS_EXPORTS
#define IOSS_API	  __declspec(dllexport)
#else
#define IOSS_API	  __declspec(dllimport)
#endif
#else
#define IOSS_API
#endif



//内部接口//////////////////////////////////////////////////////////////////////////////
/*
* 函数功能：新增一个标签点
* 输入参数：
待增加标签点的属性
* 输出参数：无
* 返回值：只能返回0。。
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_AddTag(nicIN nicPropValueList*  pPropValueList);

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_AddTagEx( nicIN nicUInt32 unTagId,nicIN nicPropValueList*  pPropValueList);


NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_DelTag( nicIN nicUInt32 unTagId);
/*
* 函数功能：得到一个标签点的值
* 输入参数：
标签id
* 输出参数：
标签的值
* 返回值：只能返回0。。。
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_GetTagValue(nicIN nicUInt32 tagId, nicOUT nicData * val);

/*
* 函数功能：修改一个标签点设备驱动配置信息
* 输入参数：带有ID号的标签点驱动配置信息结构体
* 输出参数：无
* 返回值：只有0.。。
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_ModifyTag(nicIN nicUInt32 tagId, nicIN nicPropValueList* pPropValueList);

/*2012-7-25
* 函数功能：写入一个标签点的实时值到设备
* 输入参数：ID
* 输出参数：无
* 返回值：只有0.。。
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_WriteDevice(nicIN nicUInt32 tagId, nicIN nicVariant* pData);

/* 2012-7-26
* 函数功能：创建设备信息。在设备列表中创建一个新的设备信息。
* 输入参数：
*           设备中的驱动信息   pDriverObj
*           设备名称           key
*           扩展空间           extSize
* 输出参数：
*           设备信息     生成的设备信息
* 返回值：           
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_CreateDevice(
				  nicIN nicDriverInfo_Ptr pDriverObj,
				  nicIN nicDeviceKey_Ptr key,
				  nicIN nicUInt32 extSize,
				  nicOUT nicDeviceInfo_Ptr pDeviceObj
				  );

/*
* 函数功能：从设备列表中取出指定数量的设备信息。从设备列表的第一个设备信息开始取出。
* 输入参数：
*           count 取出的设备信息的个数，count足够大则可便利设备信息列表。
*           pDevBuff 存储取出的设备信息的空数组指针，数组大小>=count
* 输出参数：
*           pDevBuff 取出的设备信息数组
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevice(nicOUT nicDeviceInfo_Ptr pDevBuff, nicIN nicUInt32* count);

/* 
* 函数功能：加载设备。
* 输入参数：
*           dev_name  设备名称
*           bValidateTags  是否关联设备的所有标签 
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_ProbeDevice(nicIN const char * dev_name, 
				 nicIN nicBool bValidateTags, 
				 nicOUT nicDeviceInfo_Ptr pDevInfo);

/* 函数功能：找到设备信息
* 输入参数：
*           key  设备名称
* 输出参数：
*           dev  设备信息
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_OpenDevice(nicIN nicDeviceKey_CPtr key, nicOUT nicDeviceInfo_Ptr dev);

/* 函数功能：启动设备
* 输入参数：
*           dev  待启动设备的配置信息
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_StartDevice(nicIN nicDeviceInfo_Ptr dev);

/* 函数功能：停止设备
* 输入参数：
*           dev  待停止设备的配置信息
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_StopDevice(nicIN nicDeviceInfo_Ptr dev);

/* 函数功能：加载驱动
* 输入参数：
*           ven  厂商名
*           devType  设备类型名
* 输出参数：
*           pDriver  设备驱动信息
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_LoadDriver(nicIN nicVendorKey_CPtr ven,
				nicIN nicDeviceTypeKey_CPtr devType,
				nicOUT nicDriverInfo_Ptr pDriver);

/*
* 函数功能：卸载驱动
* 输入参数：
*           pDriver  设备驱动信息
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UnloadDriver(nicIN nicDriverInfo_Ptr pDriver);

/* 函数功能：更新标签
* 输入参数：
*           tag  标签信息
*           newDate  更新的标签数据
*           now  更新时间戳
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UpdateTag(nicIN nicIOSSBase_Ptr tag,
			   nicIN nicData *newData,
			   nicIN nicTime* now);
/*
* 函数功能：IOSS模块启动。创建相关的类，给相关变量赋值
* 输入参数：
strName   暂时没有用？？
* 输出参数：无
* 返回值：0 成功；1 创建CIOTagTask或CIOManager失败
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_Start(nicChar* strName);

/*
* 函数功能：IOSS模块停止。删除相关的类，释放其内存。
* 输入参数：无	
* 输出参数：无
* 返回值：0 成功；1 删除CIOTagTask或CIOManager失败
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_Stop();
/*
* 函数功能：IOSS注册实时值广播回调函数。
* 输入参数：无	
* 输出参数：无
* 返回值：0 成功
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_RegNotify_RealData(RealDataNotifyCBFunc func);


//外部接口/////////////////////////////////////////////////////////////////////////////////////////////////

/*
* 函数功能：客户端添加设备  proxy_add_device --> _on_add_device
* 输入参数：设备名、生产厂商、设备型号、地址、初始化命令
* 输出参数：
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_AddDevice(nicIN nicStr devName, 
			   nicIN nicStr venDesp,
			   nicIN nicStr devDesp,
			   nicIN nicStr addr,
			   nicIN nicStr initCmd
			   );

/*
* 函数功能：删除一个设备信息。
* 输入参数：
*           待删除的设备名称  devName
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_DeleteDevice(nicIN nicStr devName);

/* 
* 函数功能：得到infs文件夹中所有的厂商描述
* 输入参数：无
* 输出参数：所有厂商描述
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetAllVenDesp(nicOUT nicStrList* &pVenDesp);

/* 
* 函数功能：得到infs文件夹中厂商下的设备描述列表
* 输入参数：venDesp  厂商描述
* 输出参数：厂商的所有设备的描述
* 返回值：
*/

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevDespByVen(nicIN nicStr venDesp, nicOUT nicStrList* &pDevDesp);


/* 
* 函数功能：得到当前工程有加载的设备名称
* 输入参数：无
* 输出参数：得到配置文件(*.ini)中的设备名称
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevName(nicOUT nicStrList* &pDevName);

/* 函数功能：得到指定设备的配置信息
* 输入参数：设备名称
* 输出参数：设备厂商、设备类型、设备地址、初始化命令
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevInfo(nicIN nicStr devName, 
				nicOUT nicStr &venDesp,
				nicOUT nicStr &devDesp,
				nicOUT nicStr &devAddr,
				nicOUT nicStr &devInit);

/* 函数功能：保存设备可修改的配置信息
* 输入参数：设备名称、可修改的配置信息（设备地址、初始化命令）
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_SaveDevInfo(nicIN nicStr devName,
				 nicIN nicStr devAddr,
				 nicIN nicStr devInit);

/*
* 函数功能：启动设备
* 输入参数：设备名称
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_StartDev(nicIN nicStr devName);


/*
* 函数功能：停止设备
* 输入参数：设备名称
* 输出参数：无
* 返回值：
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_StopDev(nicIN nicStr devName);

// 函数功能：取出指定ID标签的值
// 输入参数：
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UpdateTagByID(nicIN nicUInt32 tagId,
				   nicIN nicOUT nicData *newData,
				   nicIN nicTime* now);

/*2012-7-25 2012-10-17
* 函数功能：写入一个标签点的实时值到设备
* 输入参数：ID
* 输出参数：无
* 返回值：只有0.。。
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_WriteDevice(nicIN nicUInt32 tagId, nicIN nicVariant* pData);

// 2012-10-17
// 函数功能：写入多个标签的值
// 输入参数：标签个数、ID号、值
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_WriteTags(
			   nicIN nicUInt32 num, 
			   nicIN string* pTagNames,
			   nicIN nicVariant* pValues
			   );

//2012-10-17
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetTagNameByID(nicIN nicUInt32 tagId, nicIN nicOUT nicStr & pName);

// 2012-11-12
// 输入参数： filePath为标签点文件的全路径
// 标签名不能以#号开头
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_AddTagsFile(nicIN string & filePath, nicOUT nicInt32* tagNum);

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetTagIOSSBass(nicIN nicUInt32 tagId, nicIN nicOUT nicIOSSBase_Ptr tag);

#endif