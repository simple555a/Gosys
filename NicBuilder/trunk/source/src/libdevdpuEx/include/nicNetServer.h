
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
/// [Struct] �������������
/// </summary> 
typedef struct 
{
	nicUInt16			port ;				// �����˿�
	nicUInt16			timeOut ;			// ��ʱʱ��
	nicUInt16            maxLink ;			// ��������������Ŀ
	nicUInt32			queueBufferSize ;	// ��Ϣ���л����С
	nicUInt32			sockBufferSize ;    // ���绺������С
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
NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  ServerNet_Start(
	nicIN nicChar* strPathName);

/// <summary> 
///		��������ֹͣ
/// </summary> 
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL NETSERV_API  nicAPIStatus NICSYS_API  ServerNet_Stop();

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
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_MsgBlock(
	nicUInt16 nNetID,
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
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_RegisterStatusCallback (
	nicVoid * callBackFuncPtr, nicUInt8 callBackFuncNo) ;

/// <summary> 
///		[API] עע����������������˻ص�����
/// </summary>
/// <returns>
///		�ɹ�ʱ����nicRET_OK��ʧ��ʱ������Ӧ�Ĵ�����롣
/// </returns>
NICSYSDLL NETSERV_API nicAPIStatus NICSYS_API ServerNet_UnRegisterStatusCallback ();
CDECL_END

#endif						