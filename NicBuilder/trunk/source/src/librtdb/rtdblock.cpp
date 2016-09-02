/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\librtdb\rtdblock.cpp
	file path:	E:\vss\pmc2\src\librtdb
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	RTDB's group table lock	
	history:
	created:	10:10:2002   13:59
*********************************************************************/
#include "precomp.h"

// #define DBLOCK_DEBUG

struct CRtdbLock : public CRWLock{
	friend __bool PMC_API lock_rtdb(__bool, __uint);
	friend __bool PMC_API unlock_rtdb();
};
static CRtdbLock spin;

__bool RTDB_API PMC_API lock_rtdb(__bool exclusive, __uint timeout)
{
#ifdef DBLOCK_DEBUG
	DEBUG_PRINTF((
		"Thread %d(0x%08x) attempts to acquire rtdb lock %sexclusively(%d locks).\n", 
		GetCurrentThreadId(), GetCurrentThreadId(), exclusive? "" : "non",
		spin.GetLockCount()
		));
#endif	
	if(exclusive){
		spin.WriteLock();
	}else{
		spin.ReadLock();
	}
#ifdef DBLOCK_DEBUG	
	DEBUG_PRINTF((
		"Thread %d(0x%08x) acquired rtdb lock %sexclusively(%d locks).\n", 
		GetCurrentThreadId(), GetCurrentThreadId(), exclusive? "" : "non",
		spin.GetLockCount()
		));
#endif	
	return __true;
}

__bool RTDB_API PMC_API unlock_rtdb()
{
#ifdef DBLOCK_DEBUG
	__bool bExclusivelyLocked;
	int    iLockCount;
	spin.GetLockState(bExclusivelyLocked, iLockCount);	
	DEBUG_PRINTF((
		"Thread %d(0x%08x) released rtdb lock %sexclusively(%d locks).\n", 
		GetCurrentThreadId(), GetCurrentThreadId(), 
		bExclusivelyLocked? "" : "non",
		iLockCount
		));
#endif
	
	spin.Release();
	return __true;
}
