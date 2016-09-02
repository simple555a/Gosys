#include "precomp.h"
#pragma hdrstop

CMutexLock::CMutexLock(const char * name)
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

	LockData = CreateMutex(&sa, FALSE, name);
#else
	LockData = CreateMutex(NULL, FALSE, name);
#endif
#elif defined(__LINUX__)
	pthread_mutex_t * m = new pthread_mutex_t;
	LockData = (void*)m;
	if( LockData ){
		pthread_mutex_init(m, NULL);
	}
#endif
    if(!LockData){
        utils_error("Creating mutex lock %s failed.\n", name);
    }
}

CMutexLock::~CMutexLock()
{
#ifdef _WIN32
    if(LockData){
		CloseHandle(LockData);
        LockData=NULL;
    }
#elif defined(__LINUX__)
	pthread_mutex_destroy((pthread_mutex_t*)LockData);
	delete (pthread_mutex_t*)LockData;
#endif
}

void CMutexLock::Lock()
{
#ifdef _WIN32
    if(LockData) {
		WaitForSingleObject(LockData, INFINITE);
    }
#elif defined(__LINUX__)
	pthread_mutex_lock((pthread_mutex_t*)LockData);
#endif
}

void CMutexLock::UnLock()
{
#ifdef _WIN32
    if(LockData) {
		ReleaseMutex(LockData);
	}
#elif defined(__LINUX__)
	if(LockData) {
		pthread_mutex_unlock((pthread_mutex_t*)LockData);
	}
#endif
}

