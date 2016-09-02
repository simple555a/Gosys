#ifndef _NICDBPROXY_H_
#define _NICDBPROXY_H_

#include "nicidl/NicSys_RTDB_simC.h"
#include "nicidl/NicSys_DB_simC.h"
#include "nicDefs.h"
#include "nicRefTAO.h"

#ifdef _WIN32
#ifdef	NICDBPROXY_EXPORTS
#define DBPROXY_API	  __declspec(dllexport)
#else
#define DBPROXY_API	  __declspec(dllimport)
#endif
#else
#define DBPROXY_API
#endif

// ��ʼ��dbproxy
DBPROXY_API void init_notify_dbproxy(int argc, char * argv[]);

// ����ʼ��dbproxy
DBPROXY_API void fini_notify_dbproxy();



//-------------------------------------------------------------------------------------------------------
//DB�ӿڵ��ú���
//--------------------------------------------------------------------------------------------------------




//�½���ǩ��Ex
//������������ڵ�id�� ��ǩ����ֵ�б�
//����������½���ǩ��id
DBPROXY_API int Proxy_DB_AddTag_Ex(nicIN ::NICSYS_RTDB::T_nicUInt32 nParentTagId,
										nicIN const ::NICSYS_RTDB::T_nicPropValueList & pPropValueList,
										nicOUT ::NICSYS_RTDB::T_nicUInt32_out pnNewTagId);

//ɾ����ǩ��
//�����������ǩid
DBPROXY_API int Proxy_DB_DelTag(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId);

//ɾ����ǩ���б�
//�����������ǩ�б�id
DBPROXY_API int Proxy_DB_DelTagList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

//��ȡ��ǩ��id
//�����������ǩ��
//�����������ǩ��id
DBPROXY_API int Proxy_DB_GetId(nicIN const char * strTagName,
										nicOUT ::NICSYS_RTDB::T_nicUInt32_out pTagId);


//��ȡ��ǩ�б�id
//�����������ǩ���б�
//�����������ǩ�б�id
DBPROXY_API int Proxy_DB_GetBatchId(nicIN const ::NICSYS_RTDB::T_nicStringList & pTagName,
										nicOUT ::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

//��ȡ��ǩ��
//�����������ǩ��id
//�����������ǩ��
DBPROXY_API int Proxy_DB_GetName(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicOUT ::CORBA::String_out pTagName);

//��ȡ��ǩ���б�
//�����������ǩ���б�id
//�����������ǩ���б�
DBPROXY_API int Proxy_DB_GetBatchName(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
										nicOUT ::NICSYS_RTDB::T_nicStringList & pTagName);


//��ȡ�����б�
//�����������ǩid������id�б�
//��������������б�
DBPROXY_API int Proxy_DB_GetPropList(nicIN ::NICSYS_RTDB::T_nicUInt32 unTagId,
											  nicIN const ::NICSYS_RTDB::T_nicPropIdList & pPropIdList,
											  nicOUT ::NICSYS_RTDB::T_nicPropValueList & pValueList);

//���������б�
//�����������ǩid������id�б������б�
DBPROXY_API int Proxy_DB_SetPropList(nicIN ::NICSYS_RTDB::T_nicUInt32 unTagId,
											  nicIN const ::NICSYS_RTDB::T_nicPropIdList & pPropIdList,
											  nicIN ::NICSYS_RTDB::T_nicPropValueList & pValueList);


//��ȡʵʱֵ
//�����������ǩ��id
//���������ʵʱֵ
DBPROXY_API int Proxy_DB_GetReal(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicOUT ::NICSYS_RTDB::T_nicData & vData);

//����ʵʱֵ
//�����������ǩ��id,ʵʱֵ
DBPROXY_API int Proxy_DB_SetReal(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicIN ::NICSYS_RTDB::T_nicData & vData);

//��ȡʵʱֵ�б�
//�����������ǩ��id�б�
//���������ʵʱֵ�б�
DBPROXY_API int Proxy_DB_GetRealList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
										nicIN ::NICSYS_RTDB::T_nicDataList & vDataList);


//����ʵʱֵ�б�
//�����������ǩ��id�б�ʵʱֵ�б�
DBPROXY_API int Proxy_DB_SetRealList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
												nicIN const ::NICSYS_RTDB::T_nicDataList & vDataList);

//����ʵʱֵ�б�
//�����������ǩ��id�б�ʵʱֵ�б�
DBPROXY_API int Proxy_DB_FilterPropListEx(nicIN const ::NICSYS_RTDB::T_nicPropValue & filter,
												nicOUT ::NICSYS_RTDB::T_nicTagIdList & idlist);

DBPROXY_API int Proxy_DB_GetTagCount (::NICSYS_RTDB::T_nicUInt32_out unCount);

DBPROXY_API int Proxy_DB_SearchKeyWord (const char * pKeyWord,
										::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

#endif