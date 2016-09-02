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
///	初始化
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_Init(nicStr envPath);

/// <summary> 
///	反初始化
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_UnInit();

/// <summary> 
///	创建一个表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CreateTable(nicStr tableName);

/// <summary> 
///	打开表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_OpenTable(nicStr tableName);

/// <summary> 
///	打开表格后进行添加列操作
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddColumnItem(nicStr tableName, nicStr columnName, nicUInt8 valueType, nicUInt8 columnSize);

/// <summary> 
///	打开表格后进行删除列操作
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelColumnItem(nicStr tableName, nicStr columnName);

/// <summary> 
///	关闭表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CloseTable(nicStr tableName);

/// <summary> 
///	删除表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DeleteTable(nicStr tableName);

/// <summary> 
///	关闭所有表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_CloseAllTab();

/// <summary> 
///	同步所有已经打开的表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_SyncAllOpenTab();

/// <summary> 
///	删除所有表格
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelAllTab();

/// <summary> 
///	对表格的某个列字段添加记录
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddRecord(nicStr tableName, nicStr nFieldName, nicVariant *pValue);

/// <summary> 
///	对表格添加一整行
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_AddRow(nicStr tableName, nicStrList *nFieldList, nicValueList *nValueList); 

/// <summary> 
///	根据行号删除表中的记录,当删除的的行数大于当前最大行数时,也算成功,因为这种操作也合理
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelRecord(nicStr tableName, nicStr fieldName, nicUInt32 recno);

/// <summary> 
///	根据行号删除表中的一整行,当删除的的行数大于当前最大行数时,也算成功,因为这种操作也合理
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_DelRow(nicStr tableName, nicUInt32 recno);

/// <summary> 
///	修改表格的记录
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_ModifyReccord(nicStr tableName, nicStr fieldName, nicUInt32 recno, nicVariant *nval);

/// <summary> 
///	根据行号来修改对应的一整行
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_ModifyRow(nicStr tableName, nicUInt32 recno, nicStrList *nFieldList, nicValueList *nValueList);

/// <summary> 
///	获取某表中某字段的数据类型
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetColumnType(nicStr tableName, nicStr fieldName, nicOUT nicUInt8* fieldType);

/// <summary> 
///	根据行号获取表的格字段下记录
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetRecord(nicStr tableName, nicStr fieldName, nicUInt32 recno, nicOUT nicVariant **pValue);

/// <summary> 
///	根据行号获取表的一整行
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_GetRow(nicStr tableName, nicStrList *fieldList, nicUInt32 recno, nicOUT nicValueList **nValueList);

/// <summary> 
///	获取某表某字段特定值对应的所有记录号
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterEqual(nicStr tableName, nicStr fieldName, nicVariant *nval, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	获取某表某字段值范围对应的所有记录号//范围查询采用前闭后开区间
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterScope(nicStr tableName, nicStr fieldName, nicVariant *nvBegin, nicVariant *nvEnd, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	获取某表某字段为某些值时的所有记录号，字段值之间条件是交集
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterIntersection(nicStr tableName, nicStrList* columnNameList, nicValueList *nvList, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	获取某表中，等于某些字段值的所有记录号，字段值之间条件是并集
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_QueryRecordsAdapterUnion(nicStr tableName, nicStrList *columnNameList, nicValueList *nValueList, nicOUT nicRecnoList **recnoList);

/// <summary> 
///	判断表格是否存在
/// </summary> 
NICSYSDLL RECORDS_API nicAPIStatus NICSYS_API REC_IsTableExist(nicStr tableName, nicOUT nicBool* isTabExist);


#endif
