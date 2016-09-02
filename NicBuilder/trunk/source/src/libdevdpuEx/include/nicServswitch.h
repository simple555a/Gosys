#ifndef NICSYS_SERVSWITCH_H_
#define NICSYS_SERVSWITCH_H_


#include "nicDefs.h"





#ifdef _WIN32
#ifdef	LIB_SERVSWITCH_EXPORTS
#define SERVSWITCH_API	  __declspec(dllexport)
#else
#define SERVSWITCH_API	  __declspec(dllimport)
#endif
#else
#define SERVQUEUE_API
#endif

//CDECL_BEGIN

///ע�⣺�˻ص�ֻ������ʾ������Ϣ���ã��ڻص��������в��ܸı�msg���κ�ֵ��
typedef void (nicAPI* RECSHOWCALLBACK)(void * App,nicStr msg);

NICSYSDLL SERVSWITCH_API 
nicAPIStatus nicAPI ServSwitch_Start();

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_Stop();

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_GetSwitchTaskState(nicSwitchTaskStatus *status);

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_GetSwitchStatistic(nicSwitchStatistic *count);

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_GetServerState(nicServerStatusEnum *status);

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_GetRecvCallBackStatus(nicBool *isCB);

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_SetRecvCallBackStatus(nicBool isCB);

NICSYSDLL SERVSWITCH_API
nicAPIStatus nicAPI ServSwitch_RegRecvCallBack(RECSHOWCALLBACK msgCBFunc,void * ptrApp);



//CDECL_END
#endif