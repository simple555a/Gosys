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

// 初始化dbproxy
DBPROXY_API void init_notify_dbproxy(int argc, char * argv[]);

// 反初始化dbproxy
DBPROXY_API void fini_notify_dbproxy();



//-------------------------------------------------------------------------------------------------------
//DB接口调用函数
//--------------------------------------------------------------------------------------------------------




//新建标签点Ex
//输入参数：父节点id， 标签属性值列表
//输出参数：新建标签点id
DBPROXY_API int Proxy_DB_AddTag_Ex(nicIN ::NICSYS_RTDB::T_nicUInt32 nParentTagId,
										nicIN const ::NICSYS_RTDB::T_nicPropValueList & pPropValueList,
										nicOUT ::NICSYS_RTDB::T_nicUInt32_out pnNewTagId);

//删除标签点
//输入参数：标签id
DBPROXY_API int Proxy_DB_DelTag(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId);

//删除标签点列表
//输入参数：标签列表id
DBPROXY_API int Proxy_DB_DelTagList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

//获取标签点id
//输入参数：标签名
//输出参数：标签点id
DBPROXY_API int Proxy_DB_GetId(nicIN const char * strTagName,
										nicOUT ::NICSYS_RTDB::T_nicUInt32_out pTagId);


//获取标签列表id
//输入参数：标签名列表
//输出参数：标签列表id
DBPROXY_API int Proxy_DB_GetBatchId(nicIN const ::NICSYS_RTDB::T_nicStringList & pTagName,
										nicOUT ::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

//获取标签名
//输入参数：标签名id
//输出参数：标签点
DBPROXY_API int Proxy_DB_GetName(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicOUT ::CORBA::String_out pTagName);

//获取标签名列表
//输入参数：标签名列表id
//输出参数：标签名列表
DBPROXY_API int Proxy_DB_GetBatchName(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
										nicOUT ::NICSYS_RTDB::T_nicStringList & pTagName);


//获取属性列表
//输入参数：标签id，属性id列表
//输出参数：属性列表
DBPROXY_API int Proxy_DB_GetPropList(nicIN ::NICSYS_RTDB::T_nicUInt32 unTagId,
											  nicIN const ::NICSYS_RTDB::T_nicPropIdList & pPropIdList,
											  nicOUT ::NICSYS_RTDB::T_nicPropValueList & pValueList);

//设置属性列表
//输入参数：标签id，属性id列表，属性列表
DBPROXY_API int Proxy_DB_SetPropList(nicIN ::NICSYS_RTDB::T_nicUInt32 unTagId,
											  nicIN const ::NICSYS_RTDB::T_nicPropIdList & pPropIdList,
											  nicIN ::NICSYS_RTDB::T_nicPropValueList & pValueList);


//获取实时值
//输入参数：标签名id
//输出参数：实时值
DBPROXY_API int Proxy_DB_GetReal(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicOUT ::NICSYS_RTDB::T_nicData & vData);

//设置实时值
//输入参数：标签名id,实时值
DBPROXY_API int Proxy_DB_SetReal(nicIN ::NICSYS_RTDB::T_nicUInt32 nTagId,
										nicIN ::NICSYS_RTDB::T_nicData & vData);

//获取实时值列表
//输入参数：标签名id列表
//输出参数：实时值列表
DBPROXY_API int Proxy_DB_GetRealList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
										nicIN ::NICSYS_RTDB::T_nicDataList & vDataList);


//设置实时值列表
//输入参数：标签名id列表，实时值列表
DBPROXY_API int Proxy_DB_SetRealList(nicIN const ::NICSYS_RTDB::T_nicTagIdList & pTagIdList,
												nicIN const ::NICSYS_RTDB::T_nicDataList & vDataList);

//设置实时值列表
//输入参数：标签名id列表，实时值列表
DBPROXY_API int Proxy_DB_FilterPropListEx(nicIN const ::NICSYS_RTDB::T_nicPropValue & filter,
												nicOUT ::NICSYS_RTDB::T_nicTagIdList & idlist);

DBPROXY_API int Proxy_DB_GetTagCount (::NICSYS_RTDB::T_nicUInt32_out unCount);

DBPROXY_API int Proxy_DB_SearchKeyWord (const char * pKeyWord,
										::NICSYS_RTDB::T_nicTagIdList & pTagIdList);

#endif