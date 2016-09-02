#include "precomp.h"
#pragma hdrstop

#ifdef _WIN32

static volatile int method;

#define _LOCK_EXCLUSIVE		0x80000000
#define IS_EXCLUSIVE(st)	((st) & _LOCK_EXCLUSIVE ? __true : __false)
#define LOCK_COUNT(st)		((st) & 0xffffff)
#define INC_LOCK_COUNT(st)	((st) = (LOCK_COUNT(st)+1) | ((st) & 0xff000000) )
#define DEC_LOCK_COUNT(st)	((st) = (LOCK_COUNT(st)-1) | ((st) & 0xff000000) )

typedef std::list<DWORD> _WAIT_LIST;

static HANDLE (WINAPI *_OpenThread)(
	DWORD dwDesiredAccess,  // access right
	BOOL bInheritHandle,    // handle inheritance option
	DWORD dwThreadId        // thread identifier
);

CRWLock::CRWLock()
{
	if(!method){
		method = GetPrivateProfileInt("PMC", "RWLOCK", 2, get_config_file());
		// utils_trace("Using RWLOCK method %d.\n", method);
	}
	if(method == 1){
		// use old method of RWLOCK
		m_TlsIndex = TlsAlloc();
		m_LockCount = 0;
		m_bXLocked = false;
		(FARPROC&)_OpenThread = GetProcAddress(
			GetModuleHandle("kernel32.dll"), "OpenThread"
			);
		_m_WaitList = (WAIT_LIST)new _WAIT_LIST;
		_m_XWaitList = (WAIT_LIST)new _WAIT_LIST;
		m_Owner = 0;
		m_PreviousOwner = 0;
	}else{
		char	tp[MAX_PATH];
		char	fn[MAX_PATH];
		DWORD n;
		
		GetTempPath(sizeof(tp), tp);
		GetTempFileName(tp, 0, 0, fn);
		m_Owner = (__uint)CreateFile(
			fn,
			GENERIC_ALL,
			FILE_SHARE_READ,
			0,
			OPEN_ALWAYS,
			FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY,
			0
			);
		WriteFile((HANDLE)m_Owner, tp, sizeof(tp), &n, 0);
	}
}

#define m_WaitList (*((_WAIT_LIST*)_m_WaitList))
#define m_XWaitList (*((_WAIT_LIST*)_m_XWaitList))

CRWLock::~CRWLock()
{
	if(method == 1){
		if(m_TlsIndex != (DWORD)-1){
			TlsFree(m_TlsIndex);
		}
#ifndef NDEBUG
		if(m_XWaitList.size() || m_WaitList.size()){
			assert(false);
		}
#endif
		if(_m_WaitList){
			delete &m_WaitList;
		}
		if(_m_XWaitList){
			delete &m_XWaitList;
		}
	}else{
		// use new method
		if(m_Owner){
			CloseHandle((HANDLE)m_Owner);
		}
	}
}

void CRWLock::GetLockState(__bool &bExclusive, int &iLockCount)
{
	if(method == 1){
		int flag;
		flag = (int)TlsGetValue(m_TlsIndex);
		bExclusive = IS_EXCLUSIVE(flag);
		iLockCount = LOCK_COUNT(flag);
	}else{
		bExclusive = __false;
		iLockCount = 0;
	}
}

void CRWLock::SetLockState(__bool bExclusive, int iLockCount)
{
	if(method == 1){
		int flag;
		flag = iLockCount;
		if(bExclusive){
			flag |= _LOCK_EXCLUSIVE;
		}
		TlsSetValue(m_TlsIndex, (void *)flag);
	}else{
		//
	}
}

void CRWLock::WriteLock()
{
	if(method == 1){
		__bool ret, bExclusiveLocked;
		int    iLockCount;

		GetLockState(bExclusiveLocked, iLockCount);
		/* different lock can not be acquired recursively */
		assert(!(!bExclusiveLocked && iLockCount));
		assert(iLockCount >= 0);
		
		if(iLockCount){
			SetLockState(__true, iLockCount + 1);
			return;
		}

		inner_lock.Lock();
		if(m_bXLocked || m_XWaitList.size()){
			ret = __false;
		}else if(m_WaitList.size() || m_LockCount){
			ret = __false;
		}else{
			ret = __true;
		}
		if(!ret){
			m_XWaitList.insert(m_XWaitList.end(), GetCurrentThreadId());
		}else{
			assert(!m_LockCount);
			m_LockCount++;
			m_bXLocked = __true;
		}
		inner_lock.UnLock();

		if(!ret){
			SuspendThread(GetCurrentThread());
		}

		SetLockState(true, iLockCount + 1);
	}else{
		OVERLAPPED o;
		ZeroMemory(&o, sizeof(o));
		BOOL r=LockFileEx((HANDLE)m_Owner, LOCKFILE_EXCLUSIVE_LOCK, 0, 32, 0, &o);
		assert(r);
	}
}

void CRWLock::ReadLock()
{
	if(method == 1){
		__bool ret, bExclusiveLocked;
		int iLockCount;

		GetLockState(bExclusiveLocked, iLockCount);

		/* lock can not be acquired recursively */
		assert(iLockCount >= 0);
		assert(!bExclusiveLocked);	
		if(iLockCount){
			SetLockState(__false, iLockCount+1);
			return;
		}

		inner_lock.Lock();
		if(m_bXLocked || m_XWaitList.size()){
			ret = __false;
			m_WaitList.insert(m_WaitList.end(), GetCurrentThreadId());
		}else{
			assert(!m_WaitList.size());
			ret = __true;
			m_LockCount++;
		}
		inner_lock.UnLock();
		
		if(!ret){
			SuspendThread(GetCurrentThread());
		}
		
		SetLockState(__false, iLockCount+1);
	}else{
		OVERLAPPED o;
		ZeroMemory(&o, sizeof(o));
		BOOL r=LockFileEx((HANDLE)m_Owner, 0, 0, 32, 0, &o);
		assert(r);
	}
}

#define release(tid)\
do{\
	HANDLE hThread = _OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid);\
	if(hThread){\
		m_LockCount++;\
		m_Owner = tid;\
		ResumeThread(hThread);\
		CloseHandle(hThread);\
	}\
}while(0)

/*
	Release first thread on the X-Waiting list, if any.
	Called internally by CRWLock::Release(). Caller must 
	have already locked the inner lock.
*/
void CRWLock::WriteRelease()
{
	_WAIT_LIST::iterator p;
	DWORD tid;

	p = m_XWaitList.begin();
	tid = *p;
	m_XWaitList.erase(p);
	m_bXLocked = __true;
	m_PreviousOwner = GetCurrentThreadId();

	release(tid);	
}

/*
	Release all threads on the Waiting list, if any.
	Called internally by CRWLock::Release(). Caller must 
	have already locked the inner lock.
*/
void CRWLock::ReadRelease()
{
	_WAIT_LIST::iterator p;
	DWORD tid;
	
	// release all waiting non-exclusive requests
	p = m_WaitList.begin();
	while(p != m_WaitList.end()){
		tid = *p;
		p = m_WaitList.erase(p);
		release(tid);
	}
}

void CRWLock::Release()
{
	if(method == 1){
		int		iLockCount;
		__bool	bExclusiveLocked;
		
		GetLockState(bExclusiveLocked, iLockCount);
		assert(iLockCount >= 1);
		iLockCount--;	

		if(iLockCount){
			// still locked
			SetLockState(bExclusiveLocked, iLockCount);
			return;
		}else{
			SetLockState(__false, 0);
		}

		inner_lock.Lock();	
		m_LockCount--;
		if(m_bXLocked){
			assert(bExclusiveLocked);
			assert(!m_LockCount);
			if(m_XWaitList.size()){
				WriteRelease();
			}else{
				m_bXLocked = __false;
				ReadRelease();
			}		
		}else{
			if(m_XWaitList.size()){
				if(!m_LockCount){
					WriteRelease();
				}			
			}else{
				assert(!m_WaitList.size());
			}
		}	
		inner_lock.UnLock();	
	}else{
		UnlockFile((HANDLE)m_Owner, 0, 0, 32, 0);
	}
}

#endif

