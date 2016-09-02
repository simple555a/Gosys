#ifndef _NICSYS_REFACE_H__
#define _NICSYS_REFACE_H__

#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/SOCK_Stream.h>
#include <ace/Svc_Handler.h>
#include <ace/Synch.h>
#include <ace/Task.h>
#include <ace/streams.h>
#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Module.h>
#include <ace/Null_Condition.h>
#include <ace/config-lite.h>
#include <ace/Asynch_Acceptor.h>
#include <ace/Asynch_Connector.h>
#include <ace/Message_Queue.h>
#include <ace/Guard_T.h>
#include <ace/Synch_Traits.h>
#include <ace/Map_Manager.h> 
#include <ace/Null_Mutex.h>
#include <ace/OS_Memory.h>
#include <ace/Asynch_IO.h>                    
#include <ace/Recursive_Thread_Mutex.h>        
#include <ace/Proactor.h>
#include <ace/Asynch_IO_Impl.h>                 
#include <ace/Singleton.h>          
#include <ace/Time_Value.h>         
#include <ace/Thread_Mutex.h>          
#include <ace/OS_NS_string.h>
#include <ace/Assert.h>
#include <ace/Global_Macros.h>
#include <ace/Auto_Ptr.h>
#include <ace/Thread_Manager.h>
#include <ace/svc_export.h>
#include <ace/OS_main.h> 
#include <ace/MEM_Stream.h> 
#include <ace/Hash_Cache_Map_Manager_T.h>
#include <ace/MEM_Addr.h> 
#include <ace/MEM_IO.h> 
#include <ace/Select_Reactor.h>
#include <ace/Vector_T.h> 
#include <ace/MEM_Connector.h> 
#include <ace/OS_NS_time.h>
#include <ace/os_ns_string.h>
#include <ace/OS_NS_ctype.h>
#include <ace/OS_NS_Thread.h>
#include <ace/Event_Handler.h>
#include <ace/Process_Manager.h>
#include <ace/OS_NS_dirent.h>
#include "ace/Task_T.h"

//ACE_hash_Map“˝”√
template <class EXT_ID, class INT_ID> class Hash_Map:
public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID> ,
ACE_Equal_To<EXT_ID> , ACE_Thread_Mutex> {}  ;

#endif