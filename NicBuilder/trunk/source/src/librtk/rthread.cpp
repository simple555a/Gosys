#include "precomp.h"
#pragma hdrstop

class CInternalThread : public CRtkThread
{
public:
	CInternalThread(RTK_THREAD_PROC p, __uint param)
	{
		m_proc = p;
		m_param = param;
	}
	virtual ~CInternalThread()
	{
	}
protected:
	virtual __uint run()
	{
		return m_proc(m_param);
	}
	RTK_THREAD_PROC	m_proc;
	__uint m_param;
};

class CInternalPeriodicThread : public CPeriodicThread
{
public:
	CInternalPeriodicThread(RTK_THREAD_PROC p, __uint param, __uint timeout)
	{
		m_dwPeriod = timeout;
		m_param = param;
		m_proc = p;
	}
	virtual ~CInternalPeriodicThread()
	{
	}
	virtual void task()
	{
		m_proc(m_param);
	}
protected:
	RTK_THREAD_PROC	m_proc;
	__uint m_param;
};

LIBRTK_API RTK_HANDLE RTK_API create_rthread(RTK_THREAD_PROC proc, __uint param)
{
	CInternalThread * inter;
	inter = new CInternalThread(proc, param);
	return (RTK_HANDLE)inter;
}

LIBRTK_API __bool RTK_API start_rthread(RTK_HANDLE h)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	return inter->start();
}

LIBRTK_API __bool RTK_API stop_rthread(RTK_HANDLE h)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	return inter->stop(INFINITE);
}

LIBRTK_API void RTK_API suspend_rthread(RTK_HANDLE h)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	inter->suspend();
}

LIBRTK_API void RTK_API resume_rthread(RTK_HANDLE h)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	inter->resume();
}

LIBRTK_API	void RTK_API wait_rthread(RTK_HANDLE h, __uint timeout)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	inter->wait(CRtkThread::EVT_INIT_COMPLETE, timeout);
}

LIBRTK_API __uint RTK_API id_rthread(RTK_HANDLE h)
{
	CRtkThread * inter;
	inter = dynamic_cast<CRtkThread*>((CInternalThread*)h);
	return inter->id();
}

LIBRTK_API RTK_HANDLE create_periodic_rthread(
	RTK_THREAD_PROC proc,
	__uint param,
	__uint timeout
	)
{
	CInternalPeriodicThread * inter;
	inter = new CInternalPeriodicThread(proc, param, timeout);
	return (RTK_HANDLE)inter;
}

LIBRTK_API RTK_HANDLE RTK_API create_rtk_lock(const char * name)
{
	return create_rtk_mutex(name);
}

LIBRTK_API RTK_HANDLE RTK_API create_rtk_mutex(const char * name)
{
	return (RTK_HANDLE)new CMutexLock(name);	
}

LIBRTK_API RTK_HANDLE RTK_API create_rtk_event(
	const char * name,
	__bool auto_reset,
	__bool initial_state
	)
{
	return new CEvent(name, auto_reset, initial_state);
}

LIBRTK_API RTK_HANDLE RTK_API create_rtk_rwlock(const char * name)
{
	return new CRWLock();
}

LIBRTK_API RTK_HANDLE RTK_API create_rtk_semaphore(
	const char * name, 
	__uint max_value, 
	__uint init_value
	)
{
	return new CSemaphore(name, max_value, init_value);
}

LIBRTK_API void RTK_API set_rtk_event(RTK_HANDLE h)
{
	((CEvent*)h)->Fire();
}

LIBRTK_API __uint RTK_API wait_rtk_event(RTK_HANDLE h, __uint timeout)
{
	return ((CEvent*)h)->Wait(timeout);
}

#ifdef _WIN32
LIBRTK_API __uint RTK_API msg_wait_multiple(
	__uint count, 
	RTK_HANDLE * handles, 
	__bool bWaitall,
	__uint timeout, 
	__uint flags
	)
{
	HANDLE h[MAX_HANDLES_PER_WAIT];
	__uint i;
	if(count > MAX_HANDLES_PER_WAIT){
		return WAIT_FAILED;
	}
	for(i=0; i<count; i++){
		h[i] = ((CWaitable*)handles[i])->Handle();
	}
	return MsgWaitForMultipleObjects(count, h, bWaitall, timeout, flags);
}
#endif

LIBRTK_API void RTK_API reset_rtk_event(RTK_HANDLE h)
{
	((CEvent*)h)->Reset();
}

LIBRTK_API void RTK_API lock_rtk_object(RTK_HANDLE h)
{
	((CWaitable*)h)->Lock();
}

LIBRTK_API void RTK_API unlock_rtk_object(RTK_HANDLE h)
{
	((CWaitable*)h)->UnLock();
}

LIBRTK_API void RTK_API rd_lock_rtk_object(RTK_HANDLE h)
{
	((CRWLock*)h)->ReadLock();
}

LIBRTK_API void RTK_API wr_lock_rtk_object(RTK_HANDLE h)
{
	((CRWLock*)h)->WriteLock();
}

LIBRTK_API void RTK_API rw_unlock_rtk_object(RTK_HANDLE h)
{
	((CRWLock*)h)->Release();
}

LIBRTK_API void RTK_API notify_rthread(RTK_HANDLE h)
{
	((CRtkThread*)h)->notify(CRtkThread::EVT_INIT_COMPLETE);
}

LIBRTK_API RTK_HANDLE RTK_API get_thread_event(RTK_HANDLE h)
{
	CEvent * e;
	e = ((CRtkThread*)h)->stop_event();
	return (RTK_HANDLE)e;
}

LIBRTK_API __bool RTK_API stop_request_pending(RTK_HANDLE thread)
{
	return ((CRtkThread*)thread)->stop_request_pending();
}

