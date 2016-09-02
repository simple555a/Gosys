#include "iossComm.h"

ACE_THR_FUNC_RETURN tp_event_loop_0 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}
//外部接口///////////////////////////////////////////////////////////////////////////////////////////////
ACE_THR_FUNC_RETURN tp_event_loop_1 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}
//外部接口///////////////////////////////////////////////////////////////////////////////////////////////
ACE_THR_FUNC_RETURN tp_event_loop_2 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}
//外部接口///////////////////////////////////////////////////////////////////////////////////////////////
ACE_THR_FUNC_RETURN tp_event_loop_3 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}
//外部接口///////////////////////////////////////////////////////////////////////////////////////////////
ACE_THR_FUNC_RETURN tp_event_loop_4 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}
//外部接口///////////////////////////////////////////////////////////////////////////////////////////////
ACE_THR_FUNC_RETURN tp_event_loop_5 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}

	}
	return 0;
}