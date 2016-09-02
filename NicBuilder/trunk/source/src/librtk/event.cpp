#include "precomp.h"
#pragma hdrstop

#ifdef __LINUX__

#include <sys/time.h>

#ifndef timeradd
#define timeradd(a, b, result)							  \
	do {										  \
	(result)->tv_sec = (a)->tv_sec + (b)->tv_sec;				  \
	(result)->tv_usec = (a)->tv_usec + (b)->tv_usec;				  \
	if ((result)->tv_usec >= 1000000)						  \
	  {										  \
	  ++(result)->tv_sec;							  \
	  (result)->tv_usec -= 1000000;						  \
	  }										  \
	} while (0)
#define timersub(a, b, result)							  \
	do {										  \
	(result)->tv_sec = (a)->tv_sec - (b)->tv_sec;				  \
	(result)->tv_usec = (a)->tv_usec - (b)->tv_usec;				  \
	if ((result)->tv_usec < 0) {						  \
	--(result)->tv_sec;							  \
	(result)->tv_usec += 1000000;						  \
	}										  \
	} while (0)
#endif

class CInnerEvent : public CMutexLock{
public:
	CInnerEvent(__bool bManualReset, __bool bInitState) : CMutexLock(NULL)
		  {
		// create the accopanying mutex object
		pthread_cond_init(&m_cond, NULL);
		m_bManualReset = bManualReset;
		m_bState = bInitState;
		  }
	
	virtual ~CInnerEvent()
		  {
		pthread_cond_destroy(&m_cond);
		  }
	
	void signal()
		  {
		Lock();
		m_bState = __true;			   
		pthread_cond_broadcast(&m_cond);
		UnLock();
		  }
	
	int wait(int timeout)
		  {
		int wr;
		struct timespec tm;
		
		Lock();
		if( m_bState ){
			UnLock();
			return WAIT_OBJECT_0;
		}			   
		if( INFINITE ==  timeout ){      
			wr = pthread_cond_wait(&m_cond, (pthread_mutex_t*)LockData);
		}else{
			struct timeval now;
			struct timeval offset;
			offset.tv_sec = timeout / 1000;
			offset.tv_usec = (timeout%1000) * 1000;
			gettimeofday(&now, NULL);
			timeradd(&now, &offset, &now);		 
			tm.tv_sec = now.tv_sec;
			tm.tv_nsec = now.tv_usec * 1000;
			wr = pthread_cond_timedwait(&m_cond, (pthread_mutex_t*)LockData, &tm);
		}
		if( !wr && !m_bManualReset){
			m_bState = __false;
		}   
		UnLock();
		
		switch(wr){
		case ETIMEDOUT:
			return WAIT_TIMEOUT;
		case 0:
			return WAIT_SUCCESS;
		}
		return WAIT_FAILED;
		  }
	
	void reset()
		  {
		Lock();
		m_bState = __false;
		UnLock();			   
		  }
protected:
	pthread_cond_t m_cond;
	__bool m_bManualReset;
	__bool m_bState;	 
};
#endif

CEvent::CEvent(const char * name, __bool manual_reset, __bool initial_state)
{
#ifdef _WIN32
#ifndef UNDER_CE
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.bInheritHandle = FALSE; 
	sa.lpSecurityDescriptor = &sd;
	
	LockData = CreateEvent(&sa, manual_reset, initial_state, name);
#else
	LockData = CreateEvent(NULL, manual_reset, initial_state, name);
#endif
#elif defined(__LINUX__)
	CInnerEvent *e = new CInnerEvent(manual_reset, initial_state);
	if( !e ){
		throw(this);
	}
	LockData = (void*)e;
#endif
}

CEvent::~CEvent()
{
	if(LockData){
#ifdef _WIN32
		CloseHandle(LockData);
#elif defined(__LINUX_)
		delete (CInnerEvent*)LockData;
#endif
	}
}

void CEvent::Fire()
{
	if(LockData){
#ifdef _WIN32
		SetEvent(LockData);
#elif defined(__LINUX__)
		((CInnerEvent*)LockData)->signal();
#endif
	}	
}

__uint CEvent::Wait(__uint timeout)
{
	if(LockData){
#ifdef _WIN32
		return WaitForSingleObject(LockData, timeout);
#elif defined(__LINUX__)
		return ((CInnerEvent*)LockData)->wait(timeout);
#endif
	}
	
	return WAIT_FAILED;
}


LIBRTK_API void CEvent::Reset()
{
#ifdef _WIN32
	ResetEvent(LockData);
#elif defined(__LINUX__)
	((CInnerEvent*)LockData) -> reset();
#endif
	
}

