#include "precomp.h"

#ifdef _WIN32

CLock::CLock()
{
    LockData=new CRITICAL_SECTION;
    if(LockData){
        InitializeCriticalSection((LPCRITICAL_SECTION)LockData); 
    }else{
        utils_error("Creating lock failed.\n");
    }
}

CLock::~CLock()
{
    if(LockData){
        UnLock();
        DeleteCriticalSection((LPCRITICAL_SECTION)LockData);
        delete LockData;
        LockData=NULL;
    }
}

void CLock::Lock()
{
    if(LockData) EnterCriticalSection((LPCRITICAL_SECTION)LockData);
}

void CLock::UnLock()
{
    if(LockData) LeaveCriticalSection((LPCRITICAL_SECTION)LockData);
}

#endif

