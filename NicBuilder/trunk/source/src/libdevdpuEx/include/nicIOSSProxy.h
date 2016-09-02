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


// ��ʼ��iossproxy
IOSSPROXY_API void init_notify_iossproxy(int argc, char * argv[]);


// ����ʼ��proxyioss
IOSSPROXY_API void fini_notify_iossproxy();


// ��Ӷ���
//����������¼����ơ����͡��ص�����
IOSSPROXY_API void add_event_iossproxy(char * domain_name, char * type_name, NIC_EVENT_HANDLER_FUNC func);

// ���ٶ���
//����������¼����ơ�����
IOSSPROXY_API void del_event_iossproxy(char * domain_name, char * type_name);


//-------------------------------------------------------------------------------------------------------
//IOSS�ӿڵ��ú���
//--------------------------------------------------------------------------------------------------------
//�ͻ�������豸
//����������豸�����������̡��豸�ͺš���ַ����ʼ������
IOSSPROXY_API nicAPIStatus Proxy_IOSS_AddDevice(nicIN const char * T_devName,
											nicIN const char * T_venDesp,
											nicIN const char * T_devDesp,
											nicIN const char * T_addr,
											nicIN const char * T_initCmd);
//ɾ��һ���豸��Ϣ
//�����������ɾ�����豸����  devName
IOSSPROXY_API nicAPIStatus Proxy_IOSS_DeleteDevice(nicIN const char * T_devName);


//�õ�infs�ļ����г����µ��豸�����б�
//������������г�������
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetAllVenDesp(nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);
//
//
//�õ�infs�ļ����г����µ��豸�����б�
//���������venDesp  ��������
//������������̵������豸������
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevDespByVen(nicIN const char * T_venDesp,
											     nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);
//
//
//�õ���ǰ�����м��ص��豸����
//����������õ������ļ�(*.ini)�е��豸����
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevName(nicOUT ::NICSYS_RTDB::T_nicStringList & T_pVenDesp);

//�õ�ָ���豸��������Ϣ
//����������豸����
//����������豸���̡��豸���͡��豸��ַ����ʼ������
IOSSPROXY_API nicAPIStatus Proxy_IOSS_GetDevInfo(nicIN const char * T_devName,
													nicOUT char *& T_venDesp,
													nicOUT char *& T_devDesp,
													nicOUT char *& T_devAddr,
													nicOUT char *& T_devInit);
//
//
//
//�����豸���޸ĵ�������Ϣ
//����������豸���ơ����޸ĵ�������Ϣ���豸��ַ����ʼ�����
IOSSPROXY_API nicAPIStatus Proxy_IOSS_SaveDevInfo(nicIN const char * T_devName,
													nicIN const char * T_devAddr,
													nicIN const char * T_devInit);

//�����豸
//����������豸����
IOSSPROXY_API nicAPIStatus Proxy_IOSS_StartDev(nicIN const char * T_devName);

//ֹͣ�豸
//����������豸����
IOSSPROXY_API nicAPIStatus Proxy_IOSS_StopDev(nicIN const char * T_devName);



#endif