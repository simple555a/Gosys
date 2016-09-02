
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

//���Ǽ���Ҫ��DBˢ�³�ʼ�������Ա�ţ�ͬʱҲ���������޸�
typedef enum 
{
	IOSS_Prop_Min =0,
	IOSS_Prop_TagType =IOSS_Prop_Min,
	IOSS_Prop_Name,
	IOSS_Prop_Desc,
	IOSS_Prop_Id,
	IOSS_Prop_DataType,
	IOSS_Prop_OperateClass,//��������
	IOSS_Prop_EnableWrite,
	IOSS_Prop_IODriveDev,
	IOSS_Prop_IODriveAddr,
	IOSS_Prop_RefreshPeriod,
	IOSS_Prop_OldData,
	IOSS_Prop_Max,
}nicIOPropEnum;

//�豸��
typedef struct __st_DEVICE_KEY{
	nicChar Data[16];
}PACKED nicDeviceKey, *nicDeviceKey_Ptr;

typedef const nicDeviceKey *nicDeviceKey_CPtr;
#ifdef __cplusplus
typedef CAlphaName<nicDeviceKey> CDeviceName;
#endif
//����Mapģ�����еıȽϺ���
inline bool operator < (const nicDeviceKey &t1, const nicDeviceKey &t2)
{
	return rtk_compare_key(t1, t2)<0? true : false;
}
inline bool operator == (const nicDeviceKey &t1, const nicDeviceKey &t2)
{
	return rtk_compare_key(t1, t2)==0? true : false;
}
//������
typedef struct __st_VENDOR_KEY{
	nicChar Data[16];
}nicVendorKey, *nicVendorKey_Ptr;

typedef const nicVendorKey * nicVendorKey_CPtr;

#ifdef __cplusplus
typedef CAlphaName<nicVendorKey> CVendorName;
#endif

//������Ϣ
typedef struct __st_VENDOR_INFO{
	nicVendorKey	key;
	nicChar		description[128];
	nicChar		reserved[16];
}PACKED nicVendorInfo, *nicVendorInfo_Ptr;

//�豸������
typedef struct __st_DEVTYPE_KEY{
	nicChar Data[16];
}nicDeviceTypeKey, *nicDeviceTypeKey_Ptr;

typedef const nicDeviceTypeKey * nicDeviceTypeKey_CPtr;
#ifdef __cplusplus
typedef CAlphaName<nicDeviceTypeKey> CDevTypeName;
#endif

//�豸������Ϣ
typedef struct __st_DEVTYPE_INFO{
	nicDeviceTypeKey	key;
	nicChar		description[64];
	nicChar		reserved[16];
}PACKED nicDeviceTypeInfo, *nicDeviceTypeInfo_Ptr;


//�豸��Ϣ
typedef struct __st_DEVICE_INFO{
	nicDeviceKey		k;
	struct __st_DRIVER_INFO * d;
	nicVendorInfo		v;
	nicDeviceTypeInfo	t;
	nicChar			parameter[256];
	nicChar			address[64];
	//���������ڲ�ʹ�ã��ǳ�����
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



//�����Ļ������˼���ǽ���̬���豸��IODriveDev�ͱ�ǩ�����豸�е�������ַIODriveAddr��������������н�����Ȼ��õ�һ���������е�һ����־BinAddr
//ˢ��ʱ���Ϳ��Ը������BinAddrʱ���д����������еõ���Ҫ����Ϣ
typedef struct _st_IOSSTag
{
	//��������
	nicTagTypeEnum  TagType;							/// ������
	nicChar		Name[NAME_SIZE];				/// ����
	nicChar		Desc[TAG_DESC_SIZE];			/// ������


	nicUInt32	Id;									/// ���ID��
	nicTagStatusFlag       Flag;						/// ״̬��־
	nicDataTypeEnum   DataType;							///��������	
	nicUInt8	OperateClass;						///��������
	nicBool		EnableWrite;						///�Ƿ��д�����
	//��������
	nicDeviceKey	IODriveDev;					///���IO�����豸
	nicChar		IODriveAddr[SIZE_LENGTH_32];					///���IO������ַ
	nicUInt32	RefreshPeriod;					///ˢ������

	nicBool     IsNotNotify;					//�Ѿ����͹�
	//������صĶ�̬����
	nicDeviceInfo_Ptr DeviceObj;			//��ǩ�Ķ�Ӧ���豸ָ��
	nicChar		BinAddr[SIZE_LENGTH_32];				//�����Ƭ�ڴ�ռ䣬�������Ĺؼ��ֶΣ�
											//�ڲ�ͬ������ת�������ò�ͬ
	nicData CurData;						//��ǰֵ
	nicData OldData;						//��ǰֵ

	RTK_LIST_ENTRY	DeviceLink;				// 
	RTK_LIST_ENTRY	RefreshLink;			// ����
}PACKED nicIOSSBase,*nicIOSSBase_Ptr;

//���������ܹ�����IOSSϵͳ�µ����������е�Stub��
//����Nicsys�豸�����ĳ�������������ȫ�����߲��ֽӿ�
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

//�ⲿ�����������������������������������������ͷ�ļ���������������ֻ��ʵ�֡�
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

//����豸��Ϣ���洢�ļ��е�����
typedef struct __st_SHORT_DEVTYPE_INFO{
	nicDeviceTypeKey devType;//�豸��д
	nicChar description[64];//�豸����
	nicChar	dllname[256];//�豸����dll����
	nicChar version[64];//�汾
}PACKED nicShortDevTypeInfo, *nicShortDevTypeInfo_Ptr;
typedef const nicShortDevTypeInfo * nicShortDevTypeInfo_CPtr;

typedef struct __st_SHORT_DEVTYPE_INFO_LIST{
	nicShortDevTypeInfo_Ptr pSDevInfo;
	nicInt32 count;
}PACKED nicShortDevInfoList, *nicShortDevInfoList_Ptr;//����豸������Ϣ����

typedef struct __st_SHORT_VENDOR_INFO{
	nicVendorInfo venInfo;//������Ϣ
	nicShortDevInfoList sDIList;//����豸������Ϣ����
}PACKED nicShortVenInfo, *nicShortVenInfo_Ptr;
typedef const nicShortVenInfo * nicShortVenInfo_CPtr;

typedef std::list<nicShortVenInfo>nicSVenInfoList;//�����豸�����б�

#define NIC_TAG_FILE_NAME "nicTags.csv" // nicIOSS���صı�ǩ�ļ���   2012-11-22 Zander

#endif // _NICSYS_IOSSCOMM_H__