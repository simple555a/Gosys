
#ifndef _NICSYS_IOSSCOMM_H__
#define _NICSYS_IOSSCOMM_H__

#include "nicDefs.h"
#include "nicListEntry.h"
#include "nicStrFunc.h"

/*2012-8-29*/
#include<list>
#include<map>
using namespace std;
//

#define DISP_POWER_STATE_CHANGED		0x00000001

#define DISP_DB_ADD_TAG					0x01000000
#define DISP_DB_DROP_TAG				0x01000001

#define DISP_RESERVED					0x00000030

#define DRIVER_FLAG_LOADED	(0x1<<0) 

#define DF_Active		(0x1 << 0)
#define DF_Deleted	(0x1 << 1)


#define TagRecordIOSSFileName ACE_TEXT("TagIOSSRec.db")

#ifdef _MSC_VER
typedef struct __st_DRIVER_INFO nicDriverInfo, *nicDriverInfo_Ptr;
typedef struct __st_DEVICE_INFO nicDeviceInfo, *nicDeviceInfo_Ptr;
typedef struct __st_DEVICE_KEY  nicDeviceKey,  *nicDeviceKey_Ptr;
#else
struct __st_DRIVER_INFO;
struct __st_DEVICE_INFO;
struct __st_DEVICE_KEY;
#endif

//这是几个要从DB刷新初始化的属性编号，同时也接收在线修改
typedef enum 
{
	IOSS_Prop_Min =0,
	IOSS_Prop_TagType =IOSS_Prop_Min,
	IOSS_Prop_Name,
	IOSS_Prop_Desc,
	IOSS_Prop_Id,
	IOSS_Prop_DataType,
	IOSS_Prop_OperateClass,//操作级别
	IOSS_Prop_EnableWrite,
	IOSS_Prop_IODriveDev,
	IOSS_Prop_IODriveAddr,
	IOSS_Prop_RefreshPeriod,
	IOSS_Prop_OldData,
	IOSS_Prop_Max,
}nicIOPropEnum;

//设备名
typedef struct __st_DEVICE_KEY{
	nicChar Data[16];
}PACKED nicDeviceKey, *nicDeviceKey_Ptr;

typedef const nicDeviceKey *nicDeviceKey_CPtr;
#ifdef __cplusplus
typedef CAlphaName<nicDeviceKey> CDeviceName;
#endif
//重载Map模板类中的比较函数
inline bool operator < (const nicDeviceKey &t1, const nicDeviceKey &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}
inline bool operator == (const nicDeviceKey &t1, const nicDeviceKey &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
//厂商名
typedef struct __st_VENDOR_KEY{
	nicChar Data[16];
}nicVendorKey, *nicVendorKey_Ptr;

typedef const nicVendorKey * nicVendorKey_CPtr;

#ifdef __cplusplus
typedef CAlphaName<nicVendorKey> CVendorName;
#endif

//厂商信息
typedef struct __st_VENDOR_INFO{
	nicVendorKey	key;
	nicChar		description[128];
	nicChar		reserved[16];
}PACKED nicVendorInfo, *nicVendorInfo_Ptr;

//设备类型名
typedef struct __st_DEVTYPE_KEY{
	nicChar Data[16];
}nicDeviceTypeKey, *nicDeviceTypeKey_Ptr;

typedef const nicDeviceTypeKey * nicDeviceTypeKey_CPtr;
#ifdef __cplusplus
typedef CAlphaName<nicDeviceTypeKey> CDevTypeName;
#endif

//设备类型信息
typedef struct __st_DEVTYPE_INFO{
	nicDeviceTypeKey	key;
	nicChar		description[64];
	nicChar		reserved[16];
}PACKED nicDeviceTypeInfo, *nicDeviceTypeInfo_Ptr;


//设备信息
typedef struct __st_DEVICE_INFO{
	nicDeviceKey		k;
	struct __st_DRIVER_INFO * d;
	nicVendorInfo		v;
	nicDeviceTypeInfo	t;
	nicChar			parameter[256];
	nicChar			address[64];
	//驱动程序内部使用，非常有用
	void			*OwnerField;
	nicUInt32			flags;
	RTK_LIST_ENTRY	tags;
	nicInt32			error;
	nicChar			reserved[32];
#if defined(_MSC_VER) && 0
#pragma warning(disable : 4200)
	nicChar		extension[0];
#define DEVICE_EXTENSION(d) d->extension
#else
#define DEVICE_EXTENSION(d) ((nicChar*)d + sizeof(nicDeviceInfo))
#endif
}PACKED nicDeviceInfo, *nicDeviceInfo_Ptr;
typedef const nicDeviceInfo * nicDeviceInfo_CPtr;



//驱动的基本设计思想是将静态的设备名IODriveDev和标签点在设备中的驱动地址IODriveAddr两项，在驱动程序中解析，然后得到一个在驱动中的一个标志BinAddr
//刷新时，就可以跟据这个BinAddr时进行处理，从驱动中得到想要的信息
typedef struct _st_IOSSTag
{
	//名称属性
	nicTagTypeEnum  TagType;							/// 点类型
	nicChar		Name[NAME_SIZE];				/// 点名
	nicChar		Desc[TAG_DESC_SIZE];			/// 点描述


	nicUInt32	Id;									/// 结点ID号
	nicTagStatusFlag       Flag;						/// 状态标志
	nicDataTypeEnum   DataType;							///数据类型	
	nicUInt8	OperateClass;						///操作级别
	nicBool		EnableWrite;						///是否可写入操作
	//驱动属性
	nicDeviceKey	IODriveDev;					///软件IO驱动设备
	nicChar		IODriveAddr[SIZE_LENGTH_32];					///软件IO驱动地址
	nicUInt32	RefreshPeriod;					///刷新周期

	nicBool     IsNotNotify;					//已经发送过
	//驱动相关的动态属性
	nicDeviceInfo_Ptr DeviceObj;			//标签的对应的设备指针
	nicChar		BinAddr[SIZE_LENGTH_32];				//这个是片内存空间，是驱动的关键字段，
											//在不同的驱动转化中作用不同
	nicData CurData;						//当前值
	nicData OldData;						//先前值

	RTK_LIST_ENTRY	DeviceLink;				// 
	RTK_LIST_ENTRY	RefreshLink;			// 保留
}PACKED nicIOSSBase,*nicIOSSBase_Ptr;

//这是所有能挂载在IOSS系统下的驱动程序中的Stub。
//满足Nicsys设备驱动的程序必须包含以下全部或者部分接口
typedef nicAPIStatus (*IOSS_LOAD_PROC)(nicDriverInfo * hdriver);

typedef nicAPIStatus (*IOSS_UNLOAD_PROC)();

typedef nicAPIStatus (*IOSS_START_DEVICE_PROC)(nicDeviceInfo_Ptr dev );

typedef nicAPIStatus (*IOSS_STOP_DEVICE_PROC)(nicDeviceInfo_Ptr dev);

typedef nicAPIStatus (*IOSS_ADDRESS_TRANSLATE_PROC)(nicIOSSBase_Ptr tte);

typedef nicVoid (*IOSS_UPDATE_TAG_PROC)(nicIOSSBase_Ptr tte, nicData* new_value,nicTime_Ptr now);

typedef nicAPIStatus	(*IOSS_WRITE_DEVICE_PROC)(nicIOSSBase_Ptr tte, nicVariant* value);

typedef nicAPIStatus ( NICSYS_API *  IOSS_DISPATCH_PROC)(
	nicDeviceInfo_Ptr device,
	nicInt32 majorCode,
	nicInt32 param);
//Zander, 2012-10-12
typedef nicAPIStatus(*IOSS_WRITE_TAGS_PROC)(
	nicIN nicUInt32 num,
	nicIN nicIOSSBase_Ptr ttes,
	nicIN nicVariant* values);
typedef struct __st_DRIVER_INFO{
	void						*plugin_handle;
	IOSS_LOAD_PROC				load;
	IOSS_UNLOAD_PROC				unload;
	IOSS_START_DEVICE_PROC		start_device;
	IOSS_STOP_DEVICE_PROC			stop_device;	
	IOSS_ADDRESS_TRANSLATE_PROC	address_translate;
	IOSS_UPDATE_TAG_PROC			update_tag;
	IOSS_WRITE_DEVICE_PROC		write_device;
	IOSS_WRITE_TAGS_PROC        write_tags;  //Zander, 2012-10-12
	nicUInt32					device_count;
	nicVendorInfo				vendor;
	nicDeviceTypeInfo			type;
	nicChar						description[128];	
	nicChar						parameter[256];
	nicChar						dllname[256];
	nicUInt64					version;
	IOSS_DISPATCH_PROC			dispatch;
	nicUInt32					flags;
	nicChar						reserved[56];
}PACKED nicDriverInfo, *nicDriverInfo_Ptr;

#ifdef _WIN32

#define DEVICE_API	  __declspec(dllexport)
#else
#define DEVICE_API	 
#endif

//这部分声明是驱动程序的声明，即驱动程序必须包含此头文件，并且驱动程序只管实现。
CDECL_BEGIN

DEVICE_API nicAPIStatus load(nicDriverInfo_Ptr driverObj);

DEVICE_API nicAPIStatus unload();

DEVICE_API nicAPIStatus start_device(nicDeviceInfo_Ptr handle);

DEVICE_API nicAPIStatus stop_device(nicDeviceInfo_Ptr handle);

DEVICE_API nicAPIStatus write_device(nicIOSSBase_Ptr tte, nicVariant* value);

DEVICE_API nicAPIStatus update_tag(nicIOSSBase_Ptr tte, nicData* new_value,nicTime_Ptr now);

DEVICE_API nicAPIStatus address_translate(nicIOSSBase_Ptr tte);

DEVICE_API nicAPIStatus NICSYS_API dispatch(nicDeviceInfo_Ptr device,nicUInt32 majorCode,nicUInt32 param);

CDECL_END;

//
#define DEV_NAME_LENGTH 16
#define DEV_TYPE_KEY_LENGTH 16
#define DEV_DESP_LENGTH 64
#define DEV_VER_LENGTH 64
#define DRIVE_NAME_LENGTH 256
#define VEN_DESP_LENGTH 128
#define VEN_KEY_LENGTH 16
#define DEV_ADDR_LENGTH 64
#define DEV_INIT_LENGTH 256

//简短设备信息，存储文件中的内容
typedef struct __st_SHORT_DEVTYPE_INFO{
	nicDeviceTypeKey devType;//设备简写
	nicChar description[64];//设备描述
	nicChar	dllname[256];//设备驱动dll名称
	nicChar version[64];//版本
}PACKED nicShortDevTypeInfo, *nicShortDevTypeInfo_Ptr;
typedef const nicShortDevTypeInfo * nicShortDevTypeInfo_CPtr;

typedef struct __st_SHORT_DEVTYPE_INFO_LIST{
	nicShortDevTypeInfo_Ptr pSDevInfo;
	nicInt32 count;
}PACKED nicShortDevInfoList, *nicShortDevInfoList_Ptr;//简短设备类型信息数组

typedef struct __st_SHORT_VENDOR_INFO{
	nicVendorInfo venInfo;//厂商信息
	nicShortDevInfoList sDIList;//简短设备类型信息数组
}PACKED nicShortVenInfo, *nicShortVenInfo_Ptr;
typedef const nicShortVenInfo * nicShortVenInfo_CPtr;

typedef std::list<nicShortVenInfo>nicSVenInfoList;//厂商设备类型列表

#define NIC_TAG_FILE_NAME "nicTags.csv" // nicIOSS加载的标签文件名   2012-11-22 Zander

#endif // _NICSYS_IOSSCOMM_H__