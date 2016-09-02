
#ifndef NICSYS_IPCSERVER_H__
#define NICSYS_IPCSERVER_H__


#include "nicNet.h"
#include "nicDefs.h"


#ifdef _WIN32
#ifdef	LIB_IPCSERVER_EXPORTS
#define IPCSERV_API	  __declspec(dllexport)
#else
#define IPCSERV_API	  __declspec(dllimport)
#endif
#else
#define IPCSERV_API
#endif







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
NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API  ServerIPC_Start(
	nicIN nicChar* strPathName);

/// <summary> 
///		服务器端停止
/// </summary> 
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API  ServerIPC_Stop();

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
NICSYSDLL IPCSERV_API nicAPIStatus NICSYS_API ServerIPC_PutMsgBlock(
	nicHandle nNetID,
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
NICSYSDLL IPCSERV_API nicAPIStatus NICSYS_API ServerIPC_RegisterStatusCallback (
	nicVoid * callBackFuncPtr, nicUInt8 callBackFuncNo) ;


NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API ServerIPC_UnRegisterStatusCallback (nicVoid) ;
#endif						