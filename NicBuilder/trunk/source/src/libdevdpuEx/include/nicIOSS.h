
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



//�ڲ��ӿ�//////////////////////////////////////////////////////////////////////////////
/*
* �������ܣ�����һ����ǩ��
* ���������
�����ӱ�ǩ�������
* �����������
* ����ֵ��ֻ�ܷ���0����
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_AddTag(nicIN nicPropValueList*  pPropValueList);

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_AddTagEx( nicIN nicUInt32 unTagId,nicIN nicPropValueList*  pPropValueList);


NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_DelTag( nicIN nicUInt32 unTagId);
/*
* �������ܣ��õ�һ����ǩ���ֵ
* ���������
��ǩid
* ���������
��ǩ��ֵ
* ����ֵ��ֻ�ܷ���0������
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_GetTagValue(nicIN nicUInt32 tagId, nicOUT nicData * val);

/*
* �������ܣ��޸�һ����ǩ���豸����������Ϣ
* �������������ID�ŵı�ǩ������������Ϣ�ṹ��
* �����������
* ����ֵ��ֻ��0.����
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_ModifyTag(nicIN nicUInt32 tagId, nicIN nicPropValueList* pPropValueList);

/*2012-7-25
* �������ܣ�д��һ����ǩ���ʵʱֵ���豸
* ���������ID
* �����������
* ����ֵ��ֻ��0.����
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_WriteDevice(nicIN nicUInt32 tagId, nicIN nicVariant* pData);

/* 2012-7-26
* �������ܣ������豸��Ϣ�����豸�б��д���һ���µ��豸��Ϣ��
* ���������
*           �豸�е�������Ϣ   pDriverObj
*           �豸����           key
*           ��չ�ռ�           extSize
* ���������
*           �豸��Ϣ     ���ɵ��豸��Ϣ
* ����ֵ��           
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_CreateDevice(
				  nicIN nicDriverInfo_Ptr pDriverObj,
				  nicIN nicDeviceKey_Ptr key,
				  nicIN nicUInt32 extSize,
				  nicOUT nicDeviceInfo_Ptr pDeviceObj
				  );

/*
* �������ܣ����豸�б���ȡ��ָ���������豸��Ϣ�����豸�б�ĵ�һ���豸��Ϣ��ʼȡ����
* ���������
*           count ȡ�����豸��Ϣ�ĸ�����count�㹻����ɱ����豸��Ϣ�б�
*           pDevBuff �洢ȡ�����豸��Ϣ�Ŀ�����ָ�룬�����С>=count
* ���������
*           pDevBuff ȡ�����豸��Ϣ����
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevice(nicOUT nicDeviceInfo_Ptr pDevBuff, nicIN nicUInt32* count);

/* 
* �������ܣ������豸��
* ���������
*           dev_name  �豸����
*           bValidateTags  �Ƿ�����豸�����б�ǩ 
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_ProbeDevice(nicIN const char * dev_name, 
				 nicIN nicBool bValidateTags, 
				 nicOUT nicDeviceInfo_Ptr pDevInfo);

/* �������ܣ��ҵ��豸��Ϣ
* ���������
*           key  �豸����
* ���������
*           dev  �豸��Ϣ
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_OpenDevice(nicIN nicDeviceKey_CPtr key, nicOUT nicDeviceInfo_Ptr dev);

/* �������ܣ������豸
* ���������
*           dev  �������豸��������Ϣ
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_StartDevice(nicIN nicDeviceInfo_Ptr dev);

/* �������ܣ�ֹͣ�豸
* ���������
*           dev  ��ֹͣ�豸��������Ϣ
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_StopDevice(nicIN nicDeviceInfo_Ptr dev);

/* �������ܣ���������
* ���������
*           ven  ������
*           devType  �豸������
* ���������
*           pDriver  �豸������Ϣ
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_LoadDriver(nicIN nicVendorKey_CPtr ven,
				nicIN nicDeviceTypeKey_CPtr devType,
				nicOUT nicDriverInfo_Ptr pDriver);

/*
* �������ܣ�ж������
* ���������
*           pDriver  �豸������Ϣ
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UnloadDriver(nicIN nicDriverInfo_Ptr pDriver);

/* �������ܣ����±�ǩ
* ���������
*           tag  ��ǩ��Ϣ
*           newDate  ���µı�ǩ����
*           now  ����ʱ���
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UpdateTag(nicIN nicIOSSBase_Ptr tag,
			   nicIN nicData *newData,
			   nicIN nicTime* now);
/*
* �������ܣ�IOSSģ��������������ص��࣬����ر�����ֵ
* ���������
strName   ��ʱû���ã���
* �����������
* ����ֵ��0 �ɹ���1 ����CIOTagTask��CIOManagerʧ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_Start(nicChar* strName);

/*
* �������ܣ�IOSSģ��ֹͣ��ɾ����ص��࣬�ͷ����ڴ档
* �����������	
* �����������
* ����ֵ��0 �ɹ���1 ɾ��CIOTagTask��CIOManagerʧ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_Stop();
/*
* �������ܣ�IOSSע��ʵʱֵ�㲥�ص�������
* �����������	
* �����������
* ����ֵ��0 �ɹ�
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API 
IOSS_RegNotify_RealData(RealDataNotifyCBFunc func);


//�ⲿ�ӿ�/////////////////////////////////////////////////////////////////////////////////////////////////

/*
* �������ܣ��ͻ�������豸  proxy_add_device --> _on_add_device
* ����������豸�����������̡��豸�ͺš���ַ����ʼ������
* ���������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_AddDevice(nicIN nicStr devName, 
			   nicIN nicStr venDesp,
			   nicIN nicStr devDesp,
			   nicIN nicStr addr,
			   nicIN nicStr initCmd
			   );

/*
* �������ܣ�ɾ��һ���豸��Ϣ��
* ���������
*           ��ɾ�����豸����  devName
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_DeleteDevice(nicIN nicStr devName);

/* 
* �������ܣ��õ�infs�ļ��������еĳ�������
* �����������
* ������������г�������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetAllVenDesp(nicOUT nicStrList* &pVenDesp);

/* 
* �������ܣ��õ�infs�ļ����г����µ��豸�����б�
* ���������venDesp  ��������
* ������������̵������豸������
* ����ֵ��
*/

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevDespByVen(nicIN nicStr venDesp, nicOUT nicStrList* &pDevDesp);


/* 
* �������ܣ��õ���ǰ�����м��ص��豸����
* �����������
* ����������õ������ļ�(*.ini)�е��豸����
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevName(nicOUT nicStrList* &pDevName);

/* �������ܣ��õ�ָ���豸��������Ϣ
* ����������豸����
* ����������豸���̡��豸���͡��豸��ַ����ʼ������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetDevInfo(nicIN nicStr devName, 
				nicOUT nicStr &venDesp,
				nicOUT nicStr &devDesp,
				nicOUT nicStr &devAddr,
				nicOUT nicStr &devInit);

/* �������ܣ������豸���޸ĵ�������Ϣ
* ����������豸���ơ����޸ĵ�������Ϣ���豸��ַ����ʼ�����
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_SaveDevInfo(nicIN nicStr devName,
				 nicIN nicStr devAddr,
				 nicIN nicStr devInit);

/*
* �������ܣ������豸
* ����������豸����
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_StartDev(nicIN nicStr devName);


/*
* �������ܣ�ֹͣ�豸
* ����������豸����
* �����������
* ����ֵ��
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_StopDev(nicIN nicStr devName);

// �������ܣ�ȡ��ָ��ID��ǩ��ֵ
// ���������
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_UpdateTagByID(nicIN nicUInt32 tagId,
				   nicIN nicOUT nicData *newData,
				   nicIN nicTime* now);

/*2012-7-25 2012-10-17
* �������ܣ�д��һ����ǩ���ʵʱֵ���豸
* ���������ID
* �����������
* ����ֵ��ֻ��0.����
*/
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_WriteDevice(nicIN nicUInt32 tagId, nicIN nicVariant* pData);

// 2012-10-17
// �������ܣ�д������ǩ��ֵ
// �����������ǩ������ID�š�ֵ
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
// ��������� filePathΪ��ǩ���ļ���ȫ·��
// ��ǩ��������#�ſ�ͷ
NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_AddTagsFile(nicIN string & filePath, nicOUT nicInt32* tagNum);

NICSYSDLL IOSS_API nicAPIStatus NICSYS_API
IOSS_GetTagIOSSBass(nicIN nicUInt32 tagId, nicIN nicOUT nicIOSSBase_Ptr tag);

#endif