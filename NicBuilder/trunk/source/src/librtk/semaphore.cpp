#include "precomp.h"
#pragma hdrstop

#if defined(__LINUX__)
#include <semaphore.h>
#endif

CSemaphore::CSemaphore(const char * name, __int max, __int init)
{
#ifdef _WIN32
	 LockData = CreateSemaphore(NULL, init, max, name);
#elif defined(__LINUX__)
	 sem_t * s;
	 s = new sem_t;
	 ZeroMemory(s, sizeof(sem_t));
	 LockData = s;
	 sem_init(s, __false, init);
#endif
}

CSemaphore::~CSemaphore()
{
#ifdef _WIN32
	 CloseHandle(LockData);
#elif defined(__LINUX__)
	 if(LockData){
		  sem_destroy((sem_t*)LockData);		  
		  delete (sem_t*)LockData;
	 }
#endif
	 LockData = NULL;
}

void CSemaphore::Lock()
{
#ifdef _WIN32
	 WaitForSingleObject(LockData, INFINITE);
#elif defined(__LINUX__)
	 sem_wait((sem_t*)LockData);
#endif
}

void CSemaphore::UnLock()
{
#ifdef _WIN32
	 ReleaseSemaphore(LockData, 1, NULL);
#elif defined(__LINUX__)
	 sem_post((sem_t*)LockData);
#endif
}
