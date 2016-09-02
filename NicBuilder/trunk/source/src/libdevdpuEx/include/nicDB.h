//=========================================================================
//  Nicsys1000   �汾:  2.0
//-------------------------------------------------------------------------
//  
//  ʵʱ���ͨ�ñ��������ݸ�ʽ�����ļ�
//
//-------------------------------------------------------------------------
//  ��Ȩ (C) 2012 - �����к˿���ϵͳ�������޹�˾(CNCS)
//=========================================================================
// 
//  �Ķ���ʷ��
//  	1. 2012/02/27 : 	CNCS-0136 �����ļ�
//  	2. 2012/10/25 : 	CNCS-0136 �����з���ֵΪ�б��ָ�붼��Ϊ��άָ��
//=========================================================================
#ifndef _NICSYS_DB_H__
#define _NICSYS_DB_H__

#include "nicDefs.h"
#include <ace/Message_Block.h>

#ifdef _WIN32
#if defined	LIB_DB_EXPORTS 
#define DB_API	  __declspec(dllexport)
#else
#define DB_API	  __declspec(dllimport)
#endif
#else
#define DB_API
#endif


//ʵʱ������
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_Start(nicChar* strName);

//ʵʱ��ֹͣ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_Stop(nicVoid);

//��ʵʱ�������һ�����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_AddTag(nicUInt32 nParentTagId ,
													 nicTagPropList* pTagPropList ,
													 nicUInt32* pnNewTagId);
//��ʵʱ�������һ�����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_AddTag_Ex(nicIN nicUInt32 nParentTagId, 
													 nicIN nicPropValueList* pPropValueList, 
													 nicOUT nicUInt32* pnNewTagId);

//��ʵʱ����ɾ��һ�����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_DelTag(nicUInt32 nTagId);

//����ID����ɾ��һ�����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_DelTagList(nicTagIdList* pTagIdList);

//ͨ��һ����ǩ���õ�ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetId(nicChar* strTagName, nicUInt32* pTagId);

//ͨ��һ����ǩ���õ�ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagType(nicUInt32 nTagId, nicUInt8 * pTagType);

//ͨ��һ����ǩ���õ�һ��ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetBatchId(nicIN nicStrList* pTagName, nicOUT nicTagIdList** pTagIdList);

//ͨ��ID�ŵõ���ǩ��
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetName(nicUInt32 nTagId, nicStr* pTagName);

//ͨ��һ��ID�ŵõ�һ����ǩ��
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetBatchName(nicIN nicTagIdList* pTagIdList, nicOUT nicStrList** pTagName);

//�õ����ݿ⵱ǰ��Ч�ı�ǩ����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagCount(nicOUT nicUInt32* unCount);

//�õ�ָ����ǩ���ӱ�ǩ�����������ǽڵ��ͱ�ǩ����Ϊ�ӱ�ǩ�����������ͨ��ǩ���򷵻�0
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagSubCount(nicIN nicUInt32 tagId,nicOUT nicUInt32* unCount);

//�õ����ݿ⵱ǰ�ѷ����ID�����������+1
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetDBSize(nicOUT nicUInt32* unCount);

//�жϱ�ǩ�Ƿ����
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_IsTagExist(nicIN nicUInt32 tagId,nicOUT nicBool *isExist);

//��һ��ʵʱֵ������Ե�ǰʵʱֵ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetReal(nicUInt32 nTagId, nicData* pData);

//дһ��ʵʱֵ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetReal(nicUInt32 nTagId, nicData* pData);

//��һ��ʵʱֵ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetRealList(nicTagIdList* pTagIdList, nicDataList** pDataList);

//дһ��ʵʱֵ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetRealList(nicTagIdList* pTagIdList, nicDataList* pDataList);

//���һ������ID��һ������
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetPropList(nicIN nicUInt32 unTagId, nicIN nicPropIdList* pPropIdList, nicOUT nicPropValueList** pValueList);

//�޸�һ������ID��һ������
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetPropList(nicIN nicUInt32 unTagId, nicIN nicPropIdList* pPropIdList, nicIN nicPropValueList* pValueList);


NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_FilterPropListEx(nicPropValue *pFilter,nicTagIdList **pIDlist);

//ע��ʵʱֵ�㲥ͬ��������
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_RealData(RealDataNotifyCBFunc func);

//ע�����ӱ�ǩ�㲥ͬ��������
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_AddTag(AddTagNotifyCBFunc func);

//ע��ɾ����ǩ�㲥ͬ��������
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_DelTag(DelTagNotifyCBFunc func);

//ע���޸ı�ǩ�㲥ͬ��������
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_ModifyTag(ModifyTagNotifyCBFunc func);

//�ں�ˢ��ʵʱֵ
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_PutMsgQueue(nicRealDataList *realList);

//�ں�ˢ��ʵʱֵ2
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_PutMsgQueueEx(ACE_Message_Block *msg);


NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SearchKeyWord(nicIN nicChar* pKeyWord, nicOUT nicTagIdList** pTagIdList);

#endif