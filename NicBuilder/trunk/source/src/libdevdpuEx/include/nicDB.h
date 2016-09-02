//=========================================================================
//  Nicsys1000   版本:  2.0
//-------------------------------------------------------------------------
//  
//  实时库的通用变量、数据格式定义文件
//
//-------------------------------------------------------------------------
//  版权 (C) 2012 - 北京中核控制系统工程有限公司(CNCS)
//=========================================================================
// 
//  改动历史：
//  	1. 2012/02/27 : 	CNCS-0136 建立文件
//  	2. 2012/10/25 : 	CNCS-0136 将所有返回值为列表的指针都改为二维指针
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


//实时库启动
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_Start(nicChar* strName);

//实时库停止
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_Stop(nicVoid);

//在实时库中添加一个测点
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_AddTag(nicUInt32 nParentTagId ,
													 nicTagPropList* pTagPropList ,
													 nicUInt32* pnNewTagId);
//在实时库中添加一个测点
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_AddTag_Ex(nicIN nicUInt32 nParentTagId, 
													 nicIN nicPropValueList* pPropValueList, 
													 nicOUT nicUInt32* pnNewTagId);

//在实时库中删除一个测点
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_DelTag(nicUInt32 nTagId);

//根据ID数组删除一批测点
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_DelTagList(nicTagIdList* pTagIdList);

//通过一个标签名得到ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetId(nicChar* strTagName, nicUInt32* pTagId);

//通过一个标签名得到ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagType(nicUInt32 nTagId, nicUInt8 * pTagType);

//通过一批标签名得到一批ID
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetBatchId(nicIN nicStrList* pTagName, nicOUT nicTagIdList** pTagIdList);

//通过ID号得到标签名
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetName(nicUInt32 nTagId, nicStr* pTagName);

//通过一批ID号得到一批标签名
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetBatchName(nicIN nicTagIdList* pTagIdList, nicOUT nicStrList** pTagName);

//得到数据库当前有效的标签个数
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagCount(nicOUT nicUInt32* unCount);

//得到指定标签的子标签点个数，如果是节点型标签，则为子标签数，如果是普通标签，则返回0
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetTagSubCount(nicIN nicUInt32 tagId,nicOUT nicUInt32* unCount);

//得到数据库当前已分配的ID分配的最大计数+1
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetDBSize(nicOUT nicUInt32* unCount);

//判断标签是否存在
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_IsTagExist(nicIN nicUInt32 tagId,nicOUT nicBool *isExist);

//读一个实时值，仅针对当前实时值
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetReal(nicUInt32 nTagId, nicData* pData);

//写一个实时值
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetReal(nicUInt32 nTagId, nicData* pData);

//读一批实时值
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetRealList(nicTagIdList* pTagIdList, nicDataList** pDataList);

//写一批实时值
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetRealList(nicTagIdList* pTagIdList, nicDataList* pDataList);

//获得一个给定ID的一批属性
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_GetPropList(nicIN nicUInt32 unTagId, nicIN nicPropIdList* pPropIdList, nicOUT nicPropValueList** pValueList);

//修改一个给定ID的一批属性
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SetPropList(nicIN nicUInt32 unTagId, nicIN nicPropIdList* pPropIdList, nicIN nicPropValueList* pValueList);


NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_FilterPropListEx(nicPropValue *pFilter,nicTagIdList **pIDlist);

//注册实时值广播同步处理函数
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_RealData(RealDataNotifyCBFunc func);

//注册增加标签广播同步处理函数
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_AddTag(AddTagNotifyCBFunc func);

//注册删除标签广播同步处理函数
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_DelTag(DelTagNotifyCBFunc func);

//注册修改标签广播同步处理函数
NICSYSDLL DB_API nicAPIStatus	NICSYS_API DB_RegNotify_ModifyTag(ModifyTagNotifyCBFunc func);

//内核刷新实时值
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_PutMsgQueue(nicRealDataList *realList);

//内核刷新实时值2
NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_PutMsgQueueEx(ACE_Message_Block *msg);


NICSYSDLL DB_API nicAPIStatus NICSYS_API DB_SearchKeyWord(nicIN nicChar* pKeyWord, nicOUT nicTagIdList** pTagIdList);

#endif