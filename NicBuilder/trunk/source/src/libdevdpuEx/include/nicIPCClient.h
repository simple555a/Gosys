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
///		�ͻ�����������״̬֪ͨ
/// </summary> 
/// <param name="status">
///		����״̬
///		��״ֻ̬��ʾ��������״̬
/// </param>
/// <returns>
///		������
/// </returns>									
typedef nicAPIStatus (nicAPI * IPCClientStatusCallBack)(
	nicConnectionStatus &status);

/// <summary> 
///		�ͻ����������ݷ���֪ͨ
/// </summary> 
/// <param name="status">
///		����״̬
///		��״ֻ̬��ʾ��������״̬
/// </param>
/// <returns>
///		������
/// </returns>	
typedef nicAPIStatus(nicAPI * IPCClientGetMsgCallBack)(
	ACE_Message_Block * mqBack);
/// <summary> 
///		�ͻ������ӷ�����
/// </summary> 
/// <param name="Addr">
///		��������������,�����˿ں�IP��ַ��
/// </param>
/// <param name="timeOut">
///		���ӳ�ʱʱ��
/// </param>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_ConnectIPCServer (
	nicUInt16  IDFlag);
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>

/// <summary> 
///		�ͻ��˶Ͽ����ӷ�����
/// </summary> 
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_DisconnectIPCServer ();	

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
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_SendMsg(
	ACE_Message_Block * mBlock);

/// <summary> 
///		�ͻ�����������״̬֪ͨ�ص�����ע�ắ��
/// </summary> 
/// <param name="CallbackFunction">
///		�ص�������
///		���ص���������ΪnicNULLʱ���򲻴���ص�
/// </param>
/// <returns>
///		������
/// </returns>	
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_RegisterStatusCallback (
	IPCClientStatusCallBack callBackFunc);

/// <summary> 
///		�ͻ����������ݷ��ػص�����ע�ắ��
/// </summary> 
/// <param name="CallbackFunction">
///		�ص�������
///		���ص���������ΪnicNULLʱ���򲻴���ص�
/// </param>
/// <returns>
///		������
/// </returns>	
IPCCLIENT_API nicAPIStatus nicAPI  IPCClient_RegisterDataCallback (
	IPCClientGetMsgCallBack callBackFunc);

#endif