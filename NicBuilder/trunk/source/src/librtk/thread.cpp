/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtk\thread.cpp
	file path:	E:\vss\pmc2\src\librtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn

	purpose:	Thread management	
	history:
	created:	10:11:2002   10:36
*********************************************************************/
#include "precomp.h"

CRtkThread::CRtkThread() 
#ifdef __LINUX__
: m_SuspendSem(NULL, 0, 0)
#endif
{
	m_hThread	= NULL;
	m_bStopRequestPending = __false;
	m_bStopPending = __false;	 
}

CRtkThread::~CRtkThread()
{
	if(m_hThread){
		stop(INFINITE);
	}
	cleanup();
}

void CRtkThread::suspend()
{
#ifdef _WIN32
	SuspendThread(m_hThread);
#else
	m_SuspendSem.Lock();	 
#endif
}

void CRtkThread::resume()
{
#ifdef _WIN32
	ResumeThread(m_hThread);	 
#else
	m_SuspendSem.UnLock();
#endif	 
}

/*
*/
__bool CRtkThread::start(void * security, __uint stackSize, __uint initFlags)
{
	if(m_hThread!=NULL){
		return __false;
	}
	m_EvtStop.Reset();
	m_EvtInitComplete.Reset();
	m_bStopRequestPending = __false;
	m_bStopPending = __false;
#ifdef _WIN32
	m_hThread = (HANDLE)_beginthreadex(
		security,
		stackSize,
		RunProc,
		this,
		initFlags,
		&m_idThread
		);
#elif defined(__LINUX__)
	m_hThread = !pthread_create(&m_idThread, NULL, RunProc, this)? (void*)1 : NULL;
#endif
	return m_hThread?__true:__false;
}

__bool CRtkThread::stop(__uint dwWaitMS)
{
	__bool ret;
	
	if(m_hThread==NULL){
		return __true;
	}
#ifdef _WIN32
	PostThreadMessage(this->m_idThread, WM_QUIT, 0, 0);
#endif
	m_bStopRequestPending = __true;	 
	m_EvtStop.Fire();
	if(GetCurrentThreadId() == m_idThread){
		return __true;
	}
#ifdef _WIN32
	/*
	__ulong dwCode;
	if( !GetExitCodeThread(m_hThread, &dwCode) ){		
		  ret = __false;
		  }else if(dwCode != STILL_ACTIVE){
		  ret = __true;
		  }else
	*/
	if( WaitForSingleObject(m_hThread, dwWaitMS) == WAIT_OBJECT_0){
		ret = __true;
	}else{
		ret = __false;
	}
#elif defined(__LINUX__)
	pthread_join(m_idThread, NULL);
	ret = __true;
#endif
	
	if(ret){
		cleanup();
	}
	
	return ret;
}

#ifdef _WIN32
__uint RTK_API CRtkThread::RunProc(void * Cokkie)
#else
void * CRtkThread::RunProc(void * Cokkie)
#endif
{
	CRtkThread * th;
	th = (CRtkThread*)Cokkie;
#ifdef __LINUX__
	return (void*)th->run();
#else
	return th->run();
#endif
}

void CRtkThread::cleanup()
{
	if(m_hThread){
#ifdef _WIN32
		CloseHandle(m_hThread);
#endif
		m_hThread=NULL;	
	}
}

__uint CRtkThread::wait(CRtkThread::event e, __uint timeout)
{
	switch(e){
	case EVT_INIT_COMPLETE:
		return m_EvtInitComplete.Wait(timeout);  
		break;
	case EVT_EXIT:
		if(!this->m_hThread){
			return WAIT_FAILED;
		}else{
#ifdef _WIN32
			return WaitForSingleObject(m_hThread, timeout);
#elif defined(__LINUX__)
			return pthread_join(m_idThread, NULL)? WAIT_FAILED : WAIT_OBJECT_0;
#endif
		}		
		break;
	}
	return WAIT_FAILED;
}

/*
notify must be called from context of the newly created thread, not
the creator thread.
*/
void CRtkThread::notify(CRtkThread::event e)
{
	switch(e){
	case EVT_INIT_COMPLETE:
		m_EvtInitComplete.Fire();
		break;
		/*
		// commented out by Jackie, 2003/2/14, ridiculous code!
		case EVT_EXIT:
		if(m_hStopEvent){
		SetEvent(m_hStopEvent);
		}		
		break;
		*/
	}
}

CPeriodicThread::CPeriodicThread(__uint delay, __bool bProcessMessages)
{
	m_bMsgLoops = bProcessMessages;
	m_dwPeriod	= delay;
}

__uint CPeriodicThread::run()
{
#ifdef _WIN32
	MSG			msg;
	HANDLE hStop = m_EvtStop.Handle();
#endif
	RTK_TIME	marker, now;
	__uint		dwWait;
	
	pre_run();
	
	rtk_time_mark(&marker);	
#ifndef _WIN32
	m_bMsgLoops = __false;
#endif
	while(!m_bStopRequestPending){
		if(m_bMsgLoops){
#ifdef _WIN32
			dwWait = MsgWaitForMultipleObjects(
				1, &hStop, __false, m_dwPeriod, QS_ALLEVENTS
				);
#endif
		}else{
			dwWait = m_EvtStop.Wait(m_dwPeriod);
		}
		switch(dwWait){
		case WAIT_OBJECT_0:
			m_bStopRequestPending = __true;
			break;
#ifdef _WIN32
		case WAIT_OBJECT_0 + 1:
		case WAIT_TIMEOUT:
			{
				rtk_time_mark(&now);
				if(dwWait==WAIT_TIMEOUT || rtk_time_diff(&now, &marker) > m_dwPeriod){
					marker = now;
					task();
				}
				while(PeekMessage(&msg, 0, 0, 0, TRUE)){
					TranslateMessage(&msg);
					if(!msg.hwnd){
						on_msg(msg.message, msg.wParam, msg.lParam);
					}					
					DispatchMessage(&msg);
				}
			}
			break;
#else
		case WAIT_TIMEOUT:
			task();
			break;
#endif
		}
	}
	
	post_run();
	
	return 0;
}

__bool CRtkThread::post_message(__uint msg, __uint wParam, __uint lParam)
{
	if( !started() ){
		return __false;
	}
#ifdef _WIN32
	return PostThreadMessage( id(), msg, wParam, lParam);
#else
	// #error source incomplete!
	assert(__false);
	return 0;
#endif
}
