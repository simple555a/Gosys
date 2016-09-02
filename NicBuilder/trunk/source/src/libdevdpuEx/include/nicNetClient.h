
#ifndef _NICSYS_NETCLIENT_H_
#define _NICSYS_NETCLIENT_H_


#include "nicNet.h"

#ifdef _WIN32
#ifdef	LIB_NETCLIENT_EXPORTS
#define NETCLIENT_API	  __declspec(dllexport)
#else
#define NETCLIENT_API	  __declspec(dllimport)
#endif
#else
#define NETCLIENT_API
#endif

NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_Start ();

NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_Stop ();

NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_ConnectServer (
								nicStr & ipAddr, 
								nicUInt32 timeOut);

/// <summary> 
///		客户端断开连接服务器
/// </summary> 
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_DisconnectServer (nicStr  &ipAddr);	

/// <summary> 
///		客户端向服务器发送数据
/// </summary> 
/// <param name="mBLock">
///		需要发送的数据包。
/// </param>
/// <param name="timeOut">
///		发送超时时间
/// </param>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>							
NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_SendMsg (nicIN nicUInt16 handle,
					nicIN ACE_Message_Block * msgBlock, 
					nicIN MsgQueuePriority pri);

NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_IPToConnectID (nicIN nicStr ip,nicOUT  nicUInt16 *id);
	

NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI 
NetClient_RegisterStatusCallback (
	nicVoid * callBackFuncPtr, 
	nicUInt8 callBackFuncNo) ;
#endif
