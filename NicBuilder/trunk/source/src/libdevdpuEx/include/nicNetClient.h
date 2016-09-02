
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
///		�ͻ��˶Ͽ����ӷ�����
/// </summary> 
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL NETCLIENT_API nicAPIStatus nicAPI  
NetClient_DisconnectServer (nicStr  &ipAddr);	

/// <summary> 
///		�ͻ������������������
/// </summary> 
/// <param name="mBLock">
///		��Ҫ���͵����ݰ���
/// </param>
/// <param name="timeOut">
///		���ͳ�ʱʱ��
/// </param>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
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
