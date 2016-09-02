#ifndef NICSYS_IPCCLIENT_H_
#define NICSYS_IPCCLIENT_H_

#include "nicNet.h"
#include "nicDefs.h"
#include "nicUtils.h"


#ifdef _WIN32
#ifdef	LIB_IPCClIENT_EXPORTS
#define IPCCLIENT_API	  __declspec(dllexport)
#else
#define IPCCLIENT_API	  __declspec(dllimport)
#endif
#else
#define IPCCLIENT_API
#endif



/// <summary> 
///		客户端网络连接状态通知
/// </summary> 
/// <param name="status">
///		连接状态
///		此状态只表示网络连接状态
/// </param>
/// <returns>
///		不返回
/// </returns>									
typedef nicAPIStatus (nicAPI * IPCClientStatusCallBack)(
	nicConnectionStatus &status);

/// <summary> 
///		客户端网络数据返回通知
/// </summary> 
/// <param name="status">
///		连接状态
///		此状态只表示网络连接状态
/// </param>
/// <returns>
///		不返回
/// </returns>	
typedef nicAPIStatus(nicAPI * IPCClientGetMsgCallBack)(
	ACE_Message_Block * mqBack);
/// <summary> 
///		客户端连接服务器
/// </summary> 
/// <param name="Addr">
///		启动服务器参数,包括端口和IP地址。
/// </param>
/// <param name="timeOut">
///		连接超时时间
/// </param>
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_ConnectIPCServer (
	nicUInt16  IDFlag);
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>

/// <summary> 
///		客户端断开连接服务器
/// </summary> 
/// <returns>
///		成功时返回nicRET_OK，失败时返回相应的错误代码。
/// </returns>
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_DisconnectIPCServer ();	

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
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_SendMsg(
	ACE_Message_Block * mBlock);

/// <summary> 
///		客户端网络连接状态通知回调函数注册函数
/// </summary> 
/// <param name="CallbackFunction">
///		回调处理函数
///		当回调处理函数设为nicNULL时，则不处理回调
/// </param>
/// <returns>
///		不返回
/// </returns>	
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_RegisterStatusCallback (
	IPCClientStatusCallBack callBackFunc);

/// <summary> 
///		客户端网络数据返回回调函数注册函数
/// </summary> 
/// <param name="CallbackFunction">
///		回调处理函数
///		当回调处理函数设为nicNULL时，则不处理回调
/// </param>
/// <returns>
///		不返回
/// </returns>	
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_RegisterDataCallback (
	IPCClientGetMsgCallBack callBackFunc);

#endif