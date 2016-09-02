/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename : E:\vss\pmc2\src\include\rtk\object.h
	file path : E:\vss\pmc2\src\include\rtk
	author   : Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose : Basic object service support	
	history:
	created :	10:9:2002	9:33
*********************************************************************/
#ifndef __object_h__
#define __object_h__

#include <librtk.h>

#ifdef __cplusplus
#include <string>
using namespace std;
#endif

#ifdef __LINUX__
#include <pthread.h>
#include <semaphore.h>
#endif

#ifdef __cplusplus

/*
  pure virtual base object class type
*/
class LIBRTK_API CRTKObject{
public:
	CRTKObject();
	virtual ~CRTKObject();
	virtual __uint GetType()
	{
		return m_iObjectType;
	}
public:
	__uint	m_iTag;
	__uint	m_iObjectType;
public:
	static void CreateObject(CRTKObject * obj);
	static void DestroyObject(CRTKObject * obj);
};

class LIBRTK_API CWaitable : public CRTKObject
{
public:
	void * Handle()
	{
		return LockData;
	}	 
protected:
	CWaitable()
	{
		LockData = NULL;
	}	 
	/*
	   The actual type of LockData is implementation-dependant
	   on Win32 platform,it might be a CriticalSection object
	*/
	void * LockData;
public:
	void Duplicate(const CWaitable *item);
	virtual void Lock(){};
	virtual void UnLock(){};
};

#ifdef _WIN32
/*
Base class used in mutual exclusion
*/
class LIBRTK_API CLock : public CWaitable{
public:
	CLock();
	CLock(const CLock & lock)
	{
		Duplicate(&lock);
	}
	virtual ~CLock();
	virtual void Lock();
	virtual void UnLock();
};
#else
#define CLock CMutexLock
#endif

class LIBRTK_API CEvent : public CWaitable{
public:
	CEvent(
		const char * name = 0, 
		__bool manual_reset = __false, 
		__bool initial_state = __false
		);
	CEvent(const CEvent & event)
	{
		Duplicate(&event);
	}
	virtual ~CEvent();
	void Fire();
	void Reset();
	__uint Wait(__uint timeout);
};

class LIBRTK_API CMutexLock : public CWaitable{
public:
	CMutexLock(const char * name = 0);
	CMutexLock(const CMutexLock & lock)
	{
		Duplicate(&lock);
	}
	virtual ~CMutexLock();
	virtual void Lock();
	virtual void UnLock();
};

#include <pshpack4.h>

#ifdef _MSC_VER
// turn off warning : class '' needs to have dll interface to be used by clients of ...
#pragma warning(disable : 4251)
#endif

/*
read/write lock support
*/
class LIBRTK_API CRWLock : public CRTKObject
{
public:
	CRWLock();
	virtual ~CRWLock();
	void		ReadLock();
	void		WriteLock();
	void		Release();
#ifdef __LINUX__
	pthread_rwlock_t LockData;	 
#elif defined(_WIN32)
	inline __uint	GetLockCount()
	{
		return m_LockCount;
	}
protected:
	CLock		inner_lock;
	void		ReadRelease();
	void		WriteRelease();
protected:
	void		GetLockState(__bool & exclusive, int & count);
	void		SetLockState(__bool exclusive, int count);
	typedef void *WAIT_LIST;
	WAIT_LIST	_m_XWaitList;
	WAIT_LIST	_m_WaitList;
	__bool		m_bXLocked;
	__uint		m_TlsIndex;
	__uint		m_LockCount;
	__uint		m_Owner;
	__uint		m_PreviousOwner;
#endif
};

class LIBRTK_API CSemaphore : public CWaitable{
public:
	CSemaphore(const char * name, __int max, __int init);
	~CSemaphore();
	virtual void Lock();
	virtual void UnLock();
};

#include <poppack.h>

#ifdef __LINUX__
#define GetCurrentThreadId() pthread_self()
#endif

class LIBRTK_API CRtkThread : public CRTKObject
{
public:
	CRtkThread();
	virtual ~CRtkThread();
	 
	__bool start(void * security = 0, __uint stackSize = 0, __uint initFlags = 0);
	__bool stop(__uint dwWaitMS=1000);
	void suspend();
	void resume();
	__bool post_message(__uint msg, __uint wParam, __uint lParam);
	 
	enum event{
		EVT_INIT_COMPLETE = 0,
		EVT_EXIT,
	};

	void	notify(event e);	 
	__uint wait(event e, __uint timeout);

	inline bool started()
	{
		return m_hThread==NULL? false : true;
	}
	
	inline __uint id()
	{
		return (__uint)m_idThread;
	}

	inline void * handle()
	{
		return m_hThread;
	}
	 
	inline CEvent * stop_event()
	{
		return &m_EvtStop;
	}

	inline __bool stop_pending()
	{
		return m_bStopPending;
	}

	inline __bool stop_request_pending()
	{
		return m_bStopRequestPending;
	}
protected:
	virtual __uint run()=0;
	void cleanup();

protected:

#ifdef _WIN32
	typedef __uint pthread_t;
#else
	void	* m_MsgQueue;
#endif	 
	pthread_t m_idThread;

	void *	m_hThread;
	CEvent m_EvtStop;
	CEvent m_EvtInitComplete;

#ifdef _WIN32
	static __uint RTK_API RunProc(void * cokkie);
#elif defined(__LINUX__)
	static void *RunProc(void * cokkie);
#endif

#if defined(__LINUX__)
	CSemaphore m_SuspendSem;
#endif
	__bool m_bStopRequestPending;
	__bool m_bStopPending;	 
};

class LIBRTK_API CPeriodicThread : public CRtkThread
{
public:
	CPeriodicThread(
		__uint dwPeriod=1000, 
		__bool bProcessMessages = __false
		);
protected:
	virtual void pre_run(){};
	virtual void post_run(){};
	virtual void task()=0;
	virtual __uint on_msg(
		__uint msg, 
		__uint wparam, 
		__uint lparam
		)
	{
		return 0;
	}
private:
	virtual __uint run();
public:
	__uint	m_dwPeriod;
	__bool	m_bMsgLoops;
};

/*
    shit! namespace collision
*/
#ifndef HIDE_RTK_TREE_DEFINITION
class LIBRTK_API CTreeNode
{
public:
	__uint	m_iData;
public:
	CTreeNode();
	virtual ~CTreeNode(){};

	inline __uint GetItems()
	{
		return m_iSubItems;
	}
	inline CTreeNode * GetParent()
	{
		return m_Parent;
	}
	CTreeNode * GetFirstChild();
	CTreeNode * GetNextChild(CTreeNode *);
	CTreeNode * GetNextSibling();
	CTreeNode * GetChild(__uint index);
	inline CTreeNode * operator [](__uint index)
	{
		return GetChild(index);
	}
	void AddChild(
		CTreeNode *brother,
		CTreeNode *newcomer
		);
	void AddChild(CTreeNode *newcomer)
	{
		AddChild(0, newcomer);
	}
	CTreeNode * RemoveChild(__uint index);
	CTreeNode * RemoveChild(CTreeNode *brother);
	void RemoveChildren();
	CTreeNode * Search(const string & RelativeUrl);
	const string & Key()
	{
		return m_Key;
	}

	typedef __bool (*CAction)(CTreeNode *nd, __uint context);
	enum TraverseMethod{
		pre_order = 0,
		post_order = 1,
		exclude = 2,
	};
	__uint ForEach(
		CAction action,
		__bool bRecursive=__false,
		TraverseMethod m=pre_order,
		__uint context = 0
		);

	virtual void OnAddChild(CTreeNode * NewChild);
	virtual void OnDeleteChild(CTreeNode *node);
	virtual void OnUpdate();
protected:
	CTreeNode * m_Parent;
	CTreeNode * m_FirstChild, *m_LastChild;
	CTreeNode * m_PreviousSibling, * m_NextSibling;
	__uint		m_iSubItems;
	string		m_Key;
private:
	CTreeNode & operator =(const CTreeNode & other);

private:
	__uint _ForEachProc(
		CAction action,
		__bool bRecursive,
		TraverseMethod m,
		__bool &bContinue,
		__uint context
		);
};
#endif // HIDE_RTK_TREE_DEFINITION

#if defined(_WIN32) && !defined(HIDE_SHARED_MEMORY_OBJECT)
/*
	implemente a win32 shared memory object
	configurable parameters include:
	a.file image name
	b.memory size and position within the file
	c.file mode (create/open)
	d.share mode(read/write)
	e.disired access(read/write)
*/

class LIBRTK_API CSharedMemory
{
public:
	CSharedMemory();
#if 1
	CSharedMemory(
		const char * fname,
		unsigned offset=0,
		unsigned size=0,
		const char * shared_name=0,
		DWORD create_mode=OPEN_ALWAYS,
		DWORD share_mode=FILE_SHARE_READ,
		DWORD disired_access=GENERIC_WRITE | GENERIC_READ
	);
#endif
	~CSharedMemory();

	inline void * GetPointer()
	{
		return lpBlob;
	}

	operator PVOID ()
	{
		return lpBlob;
	}
	
	void	UnInit();
	__bool Flush();
	int StaticInit(
		const char * fname = NULL,
		unsigned offset=0,
		unsigned size=0,
		const char * shared_name=0,
		DWORD create_mode=OPEN_ALWAYS,
		DWORD share_mode=FILE_SHARE_READ,
		DWORD disired_access=GENERIC_WRITE | GENERIC_READ
	);
	DWORD FileSize;
	//TEST
	__uint oFNum;
private:
	CRITICAL_SECTION cs;
	inline	void	Lock(){EnterCriticalSection(&cs);};
	inline	void	Unlock(){LeaveCriticalSection(&cs);};
protected:
	HANDLE mhFile;
	HANDLE mhMapped;
	LPVOID lpBlob;
	int mErr;
	int mOpenAsReadonly;
};

#endif /* _WIN32 */

#endif /* __cplusplus */

#endif
