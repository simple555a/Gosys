#include "precomp.h"
#pragma hdrstop

double g_TransactionLife = 300;

#ifndef _WIN32

static void InterlockedIncrement(__long * value)
{
	(*value)++;
}

static void InterlockedDecrement(__long * value)
{
	(*value)--;
}

#define DebugBreak()

typedef __ulong *  PULONG;
typedef __long * PLONG;

#endif

CTransactionContainer::CTransactionContainer()
{
}

CTransactionContainer::~CTransactionContainer()
{
	WriteLock();
	CTList::iterator it;
	while(tlist.size()){
		it = tlist.begin();
		delete *it;
	}
	tlist.clear();
	Release();
}

void CTransactionContainer::reap()
{
	WriteLock();
	CTList::iterator p;
	p=tlist.begin();	
	double diff;
	RTK_TIME tm;
	CTransaction *t;
	rtk_time_mark(&tm);
	while(p!=tlist.end()){			  
		t = *p;
		assert(&(*p)->birth == &((*p)->birth));
		diff = rtk_time_diff(&tm,&((*p)->birth));
		if(diff > t->life && !t->m_iRefcount){
			p=tlist.erase(p);
			char guid_name[128];
			t->Guid.to_string(guid_name);
			DEBUG_PRINTF((
				">>T %s,too old(%.0f>%.0f),removing.\n",
				guid_name,diff, t->life
				));
			t->OnTimeout();
			delete t;
		}else{
			p++;
		}
	}
	Release();
}

int CTransactionContainer::add(CTransaction * t)
{	 
	WriteLock();
	// insert at begging, MRU favorable
	tlist.insert(tlist.begin(),t);
	t->m_Parent = this;
	Release();
	return 1;
}

int CTransactionContainer::remove(CTransaction * t)
{		
	return remove(t->Guid); 
}

/*
	2004/11/9, jackie
	remove is called only when lock is held already.
*/
int CTransactionContainer::remove(const RTK_GUID & guid)
{
	CTList::iterator p;
	// WriteLock();
	p=tlist.begin();
	while(p!=tlist.end()){		  
		if( (*p)->Guid == guid ){
			tlist.erase(p);
			Release();
			return 1;
		}
		p++;
	}
	// Release();
	return 0;
}

CTransaction * CTransactionContainer::_borrow(const RTK_GUID & guid)
{
	CTList::iterator p;
	ReadLock();
	p=tlist.begin();
	CTransaction * ret = 0;
	while(p!=tlist.end()){
		if( (*p)->Guid == guid ){
			ret = *p;
			InterlockedIncrement((__long*)&ret->m_iRefcount);
			break;
		}
		p++;
	}
#if 0
	if(!ret){
		Release();
	}
#else
	Release();
#endif
	return ret;
}

void CTransactionContainer::_return(CTransaction *t)
{
	InterlockedDecrement((PLONG)&t->m_iRefcount);
	if(t->m_iRefcount < 0){
		utils_debug("Transaction refcount below zero.\n");
		// DebugBreak();
	}
#if 0
	Release();
#endif
}

CTransaction::CTransaction()
{
	Guid.create();
	rtk_time_mark(&birth);
	m_iRefcount = 0;
	m_Parent = 0;
	life = g_TransactionLife;
}

CTransaction::~CTransaction()
{	 
	if(m_Parent){
		m_Parent->remove(Guid);
	}
}

CWaitableTransaction::CWaitableTransaction()
{	 
	m_bWaitResult = __true;
}

CWaitableTransaction::~CWaitableTransaction()
{
}

__bool CWaitableTransaction::Wait(__uint TimeOut, __bool ProcessMessage)
{
#ifdef _WIN32
	DWORD res;

	InterlockedIncrement((PLONG)&m_iRefcount);
	if(ProcessMessage){		
		bool bExit = false;
		RTK_TIME tm, now;
		rtk_time_mark(&tm);
		HANDLE hEvent = Event.Handle();
		while(!bExit){
			res = MsgWaitForMultipleObjects(
				1, &hEvent, FALSE, TimeOut, QS_ALLEVENTS
				);
			switch(res){
			case WAIT_OBJECT_0:
				bExit = true;
				break;
			case WAIT_TIMEOUT:
				bExit = true;
				break;
			case WAIT_OBJECT_0 + 1:
				rtk_time_mark(&now);
				double elapsed = rtk_time_diff(&now, &tm) * 1000;
				tm = now;
				if(TimeOut > elapsed){
					TimeOut -= (__uint)elapsed;
				}else{
					bExit = true;
					res = WAIT_TIMEOUT;
					break;
				}
				MSG msg;
				while(PeekMessage(&msg, 0, 0, 0, TRUE)){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				break;
			}
		}		
	}else{
		res = Event.Wait(TimeOut);
	}
	m_bWaitResult = res == WAIT_OBJECT_0? __true : __false;
	InterlockedDecrement((PLONG)&m_iRefcount);
	return m_bWaitResult;
#else
	m_bWaitResult =  Event.Wait(TimeOut) ==WAIT_OBJECT_0? __true : __false;
#endif
}

void CWaitableTransaction::SetEvent()
{
	Event.Fire();	 
}

void CWaitableTransaction::Cancel()
{
	SetEvent();
}

RTKNET_API __bool PMC_API add_transaction(
	HVBUS _bus, 
	CTransaction *t
	)
{
	if(!_bus){
		return 0;
	}
	CRtkVBus *bus = (CRtkVBus*)_bus;
	return bus->broker.add(t);
}
