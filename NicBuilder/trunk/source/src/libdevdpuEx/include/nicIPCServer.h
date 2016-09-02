
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
///		������������
/// </summary> 
/// <param name="startElem">
///		����������������
/// </param>
/// <param name="timeOut">
///		���ݷ��ͳ�ʱʱ�䣬��ʱ����
/// </param>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
///     ����filterFlag ,0��������ip��1�������ѡ��ip
//                      2�����������ѡ��ip
/// </returns>
NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API  ServerIPC_Start(
	nicIN nicChar* strPathName);

/// <summary> 
///		��������ֹͣ
/// </summary> 
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API  ServerIPC_Stop();

/// <summary> 
///		�������ݸ��ͻ���
/// </summary> 
/// <param name="mBlock">
///		���������ݡ�
/// </param>
/// <param name="timeOut">
///		���ݷ��ͳ�ʱʱ�䣬��ʱ����
/// </param>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL IPCSERV_API nicAPIStatus NICSYS_API ServerIPC_PutMsgBlock(
	nicHandle nNetID,
	ACE_Message_Block * mBlock,
	MsgQueuePriority pri  );



/// <summary> 
///		[API] ע��������������Ҫ�Ļص�����
/// </summary>
/// <param name="CallbackFunction">
///		�ص�������
///		���ص���������ΪnicNULLʱ���򲻴���ص�
/// </param>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL IPCSERV_API nicAPIStatus NICSYS_API ServerIPC_RegisterStatusCallback (
	nicVoid * callBackFuncPtr, nicUInt8 callBackFuncNo) ;


NICSYSDLL IPCSERV_API  nicAPIStatus NICSYS_API ServerIPC_UnRegisterStatusCallback (nicVoid) ;
#endif						