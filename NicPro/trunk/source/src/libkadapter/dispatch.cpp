#include "precomp.h"
#pragma hdrstop
#include <list>

using namespace std;

struct _EVENT{
	int event;
	int p0;
	int p1;
	__bool processed;
};

class CMessageQueue : public CLock
{
public:
	CMessageQueue()
	{
#ifdef _WIN32			   
		m_hWnd = NULL;
		m_OldProc = NULL;
#endif
		m_bViable = __false;
		m_Adapter = 0;
	}
	virtual ~CMessageQueue()
	{
		Destroy();
	}
#ifdef _WIN32	 
	HWND m_hWnd;
	WNDPROC m_OldProc;

	static LRESULT CALLBACK CMessageQueue::_WindowProc(
		HWND hwnd,      // handle to window
		UINT uMsg,      // message identifier
		WPARAM wParam,  // first message parameter
		LPARAM lParam   // second message parameter
		);
#endif
	typedef list<_EVENT*> CEventList;
	CEventList m_Events;

	__bool Create()
	{
#ifdef _WIN32
		m_hWnd = CreateWindow(
			"EDIT",
			"KProxyEventsSink",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			NULL,
			NULL
			);
		if(m_hWnd == NULL){
			wperror("proxy dispatcher -- CreateWindow()");
			return __false;
		}
		m_OldProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)_WindowProc);
		SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
		m_bViable = __true;
		return __true;
#else
		m_bViable = __true;
#endif
	}
	
	__bool Queue(_EVENT * e)
	{
		__bool ret;
		e->processed = __false;
#ifdef _WIN32
		ret = PostMessage(m_hWnd, WM_USER + 426, (WPARAM)e, 0);
#endif
		Lock();
		m_Events.insert(m_Events.end(), e);
		ret = __true;
		UnLock();
		return ret;
	}

	__uint Dispatch(_EVENT * e)
	{
		if(m_Adapter->eventSink){
			m_Adapter->eventSink(
				m_Adapter, 
				e->event, 
				e->p0, 
				e->p1
				);
		}
		e->processed = __true;
		return 0;
	}
	
	void Destroy()
	{
		if(!Viable()){
			return;
		}
		
#ifdef _WIN32
		DestroyWindow(m_hWnd);
#endif
		_EVENT * e;
		CEventList::iterator it;
		Lock();
		it = m_Events.begin();
		while( it != m_Events.end() ){
			e = *it;
			delete e;
			it = m_Events.erase(it);
		}
		UnLock();

		m_bViable = __false;
	}

	__bool Viable()
	{
		return m_bViable;
	}

	void garbageCollector()
	{
		CEventList::iterator it;
		_EVENT * e;
		
		Lock();
		while(m_Events.size()){
			it = m_Events.begin();
			e = *it;
			if(!e->processed){
				break;
			}
			delete e;
			m_Events.erase(it);
		}
		UnLock();
	}
	
	__bool m_bViable;
	IKAdapter * m_Adapter;
};

#ifdef _WIN32
LRESULT CALLBACK CMessageQueue::_WindowProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	CMessageQueue * q;
	q = (CMessageQueue*)GetWindowLong(hwnd, GWL_USERDATA);
	if(uMsg == (WM_USER + 426)){
		q->Dispatch((_EVENT*)wParam);
		if(q->m_Events.size() > 10){
			q->garbageCollector();
		}
		return 1;
	}
	return CallWindowProc(q->m_OldProc, hwnd, uMsg, wParam, lParam);
}
#endif

void * create_msg_queue(IKAdapter * a)
{
	CMessageQueue * q;
	q = new CMessageQueue;
	if(!q){
		return 0;
	}
	if(!q->Create()){
		return 0;
	}
	q->m_Adapter = a;
	return (void *)q;
}

void destroy_msg_queue(void * _q)
{
	CMessageQueue * q = (CMessageQueue*)_q;
	delete q;
}

/*
	put a message in the message queue, in win32, all is
	done here, because the windows will automatically picks
	the message and delivers it to the interested client.
*/
KADAPTER_API __bool IKAdapter_queue_event(IKAdapter * __this, int event, int p0, int p1)
{
	_EVENT * e;
	CMessageQueue * q;
	q = (CMessageQueue * )__this->msgQueue;
	if(!q->Viable()){
		return __false;
	}

	e = new _EVENT;
	if(!e){
		return __false;
	}
	e->event = event;
	e->p0 = p0;
	e->p1 = p1;
	if(!q->Queue(e)){
		delete e;
		e = 0;
	}

	return e? __true : __false;
}

