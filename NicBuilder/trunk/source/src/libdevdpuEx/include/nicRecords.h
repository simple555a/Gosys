#ifndef _NICSYS_RECORDS_H__
#define _NICSYS_RECORDS_H__

#include "nicDefs.h"
#include "nicPrecomp.h"


#ifdef _WIN32
#ifdef	LIB_RECORDS_EXPORTS
#define RECORDS_API	  __declspec(dllexport)
#else
#define RECORDS_API	  __declspec(dllimport)
#endif
#else
#define RECORDS_API
#endif

/// <summary> 
///	��ʼ��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_Init(nicStr envPath);

/// <summary> 
///	����ʼ��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_UnInit();

/// <summary> 
///	����һ�����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CreateTable(nicStr tableName);

/// <summary> 
///	�򿪱��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_OpenTable(nicStr tableName);

/// <summary> 
///	�򿪱����������в���
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddColumnItem(nicStr tableName, nicStr columnName, nicUInt8 valueType, nicUInt8 columnSize);

/// <summary> 
///	�򿪱������ɾ���в���
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelColumnItem(nicStr tableName, nicStr columnName);

/// <summary> 
///	�رձ��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CloseTable(nicStr tableName);

/// <summary> 
///	ɾ�����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DeleteTable(nicStr tableName);

/// <summary> 
///	�ر����б��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CloseAllTab();

/// <summary> 
///	ͬ�������Ѿ��򿪵ı��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_SyncAllOpenTab();

/// <summary> 
///	ɾ�����б��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelAllTab();

/// <summary> 
///	�Ա���ĳ�����ֶ���Ӽ�¼
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddRecord(nicStr tableName, nicStr nFieldName, nicVariant *pValue);

/// <summary> 
///	�Ա�����һ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddRow(nicStr tableName, nicStrList *nFieldList, nicValueList *nValueList); 

/// <summary> 
///	�����к�ɾ�����еļ�¼,��ɾ���ĵ��������ڵ�ǰ�������ʱ,Ҳ��ɹ�,��Ϊ���ֲ���Ҳ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelRecord(nicStr tableName, nicStr fieldName, nicUInt32 recno);

/// <summary> 
///	�����к�ɾ�����е�һ����,��ɾ���ĵ��������ڵ�ǰ�������ʱ,Ҳ��ɹ�,��Ϊ���ֲ���Ҳ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelRow(nicStr tableName, nicUInt32 recno);

/// <summary> 
///	�޸ı��ļ�¼
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_ModifyReccord(nicStr tableName, nicStr fieldName, nicUInt32 recno, nicVariant *nval);

/// <summary> 
///	�����к����޸Ķ�Ӧ��һ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_ModifyRow(nicStr tableName, nicUInt32 recno, nicStrList *nFieldList, nicValueList *nValueList);

/// <summary> 
///	��ȡĳ����ĳ�ֶε���������
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetColumnType(nicStr tableName, nicStr fieldName, nicOUT nicUInt8* fieldType);

/// <summary> 
///	�����кŻ�ȡ��ĸ��ֶ��¼�¼
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetRecord(nicStr tableName, nicStr fieldName, nicUInt32 recno, nicOUT nicVariant **pValue);

/// <summary> 
///	�����кŻ�ȡ���һ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetRow(nicStr tableName, nicStrList *fieldList, nicUInt32 recno, nicOUT nicValueList **nValueList);

/// <summary> 
///	��ȡĳ��ĳ�ֶ��ض�ֵ��Ӧ�����м�¼��
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterEqual(nicStr tableName, nicStr fieldName, nicVariant *nval, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	��ȡĳ��ĳ�ֶ�ֵ��Χ��Ӧ�����м�¼��//��Χ��ѯ����ǰ�պ�����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterScope(nicStr tableName, nicStr fieldName, nicVariant *nvBegin, nicVariant *nvEnd, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	��ȡĳ��ĳ�ֶ�ΪĳЩֵʱ�����м�¼�ţ��ֶ�ֵ֮�������ǽ���
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterIntersection(nicStr tableName, nicStrList* columnNameList, nicValueList *nvList, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	��ȡĳ���У�����ĳЩ�ֶ�ֵ�����м�¼�ţ��ֶ�ֵ֮�������ǲ���
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterUnion(nicStr tableName, nicStrList *columnNameList, nicValueList *nValueList, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	�жϱ���Ƿ����
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_IsTableExist(nicStr tableName, nicOUT nicBool* isTabExist);


#endif
