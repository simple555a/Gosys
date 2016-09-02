
#ifndef __PSNETSERVER_H__
#define __PSNETSERVER_H__



#include "nicNet.h"
#include "nicDefs.h"

#ifdef _WIN32
#ifdef	LIB_NETSERVER_EXPORTS
#define NETSERV_API	  __declspec(dllexport)
#else
#define NETSERV_API	  __declspec(dllimport)
#endif
#else
#define NETSERV_API
#endif


CDECL_BEGIN
/// <summary> 
/// [Struct] 网络端启动参数
/// </summary> 
typedef struct 
{
	nicUInt16			port ;				// 监听端口
	nicUInt16			timeOut ;			// 超时时间
	nicUInt16            maxLink ;			// 允许的最大连接数目
	nicUInt32			queueBufferSize ;	// 消息队列缓冲大小
	nicUInt32			sockBufferSize ;    // 网络缓冲区大小
} nicServerStartElem ;	



NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_SetFilter(nicIN nicStrList* ipList,
					nicIN IPFilterMode &mode);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_SetFilterList(nicIN nicStrList* ipList);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_GetFilterList(nicOUT nicStrList* ipList);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_SetFilterMode(nicIN  IPFilterMode &mode);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_GetFilterMode(nicOUT  IPFilterMode *mode);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_SetPort(nicOUT  nicUInt16 port);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API 
ServerNet_GetPort(nicOUT  nicUInt16 *port);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API 
ServerNet_GetContCount(nicOUT  nicInt32 *count);

NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  
ServerNet_GetContAddrList(nicStrList *addrList);


/// <summary> 
///		服务器端启动
/// </summary> 
/// <param name="startElem">
///		启动服务器参数。
/// </param>
/// <param name="timeOut">
///		数据发送超时时间，超时则丢弃
/// </param>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
///     参数filterFlag ,0代表不过滤ip，1代表过滤选定ip
//                      2代表过不过滤选定ip
/// </returns>
NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  ServerNet_Start(
	nicIN nicChar* strPathName);

/// <summary> 
///		服务器端停止
/// </summary> 
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  ServerNet_Stop();

/// <summary> 
///		发送数据给客户端
/// </summary> 
/// <param name="mBlock">
///		待发送数据。
/// </param>
/// <param name="timeOut">
///		数据发送超时时间，超时则丢弃
/// </param>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_MsgBlock(
	nicUInt16 nNetID,
	ACE_Message_Block * mBlock,
	MsgQueuePriority pri  );




/// <summary> 
///		[API] 注册服务器网络段需要的回调函数
/// </summary>
/// <param name="CallbackFunction">
///		回调处理函数
///		当回调处理函数设为nicNULL时，则不处理回调
/// </param>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_RegisterStatusCallback (
	nicVoid * callBackFuncPtr, nicUInt8 callBackFuncNo) ;

/// <summary> 
///		[API] 注注销所有网络服务器端回调函数
/// </summary>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_UnRegisterStatusCallback ();
CDECL_END

#endif						