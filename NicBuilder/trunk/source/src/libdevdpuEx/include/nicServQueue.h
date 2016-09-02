#ifndef NICServQueue_H_
#define NICServQueue_H_

#include <ace/Singleton.h>          // for ACE_Unmanaged_Singleton
#include <ace/Message_Block.h>      // for ACE_Message_Block
#include <ace/Message_Queue.h>
#include "nicDefs.h"

#ifdef _WIN32
#ifdef	LIB_SERVQUEUE_EXPORTS
#define SERVQUEUE_API	  __declspec(dllexport)
#else
#define SERVQUEUE_API	  __declspec(dllimport)
#endif
#else
#define SERVQUEUE_API
#endif



//CDECL_BEGIN

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerQueueCreate();

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerQueueClear();

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerRegisterMsgqueue(nicIN nicUInt16 handle,
															 nicIN ACE_Message_Queue<ACE_MT_SYNCH> * msgQueue);

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerNullifyMsgqueue(nicIN nicUInt16 handdlNo);

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerClearMsgqueue();

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerMsgqueueSize(nicOUT nicUInt32 *mqsize);

NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerPutMsg(nicIN nicUInt16 handle,
												   nicIN ACE_Message_Block * msgBlock, 
												   nicIN MsgQueuePriority pri);
NICSYSDLL SERVQUEUE_API nicAPIStatus NICSYS_API ServerBroadMsg(
	nicIN ACE_Message_Block * msgBlock, 
	nicIN nicUInt32 flag);

//CDECL_END
#endif

