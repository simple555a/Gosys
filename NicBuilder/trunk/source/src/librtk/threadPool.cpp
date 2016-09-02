#pragma warning(disable:4018)
#include "precomp.h"

#define MSG_QUEUE_ITEM (WM_USER+1)

class CPoolWorkerThread : public CRtkThread{
public:
	CPoolWorkerThread();
	virtual ~CPoolWorkerThread();

	__bool queueWorkItem(
		RTHREAD_POOL_ITEM item, 
		__uint parameter
		);
	virtual __uint run();
	inline long items()
	{
		return itemCount;
	}
private:
	long itemCount;
};

CPoolWorkerThread::CPoolWorkerThread()
{
	itemCount = 0;
}

CPoolWorkerThread::~CPoolWorkerThread()
{
	//
}

__bool CPoolWorkerThread::queueWorkItem(
	RTHREAD_POOL_ITEM item, 
	__uint parameter
	)
{
	if(post_message(MSG_QUEUE_ITEM, (__uint)item, (__uint)parameter)){
		InterlockedIncrement(&itemCount);
		return __true;
	}
	return __false;
}

__uint CPoolWorkerThread::run()
{
	MSG msg;
	RTHREAD_POOL_ITEM item;
	__uint param;
	
	notify(EVT_INIT_COMPLETE);

	while(GetMessage(&msg, 0, 0, 0)){
		TranslateMessage(&msg);
		if(msg.message == MSG_QUEUE_ITEM){
			item = (RTHREAD_POOL_ITEM)msg.wParam;
			param = (__uint)msg.lParam;
			__try{
				item(param);
			}__except(EXCEPTION_EXECUTE_HANDLER){
				;
			}
			InterlockedDecrement(&itemCount);
		}
		DispatchMessage(&msg);
	}
	return 0;
}

class CThreadPool{
public:
	CThreadPool(__uint size);
	virtual ~CThreadPool();

	void startAll();

	CPoolWorkerThread * threads;
	__uint count;
};

CThreadPool::CThreadPool(__uint size)
{
	count = size;
	threads = new CPoolWorkerThread[size];
	if(!threads){
		throw(this);
	}
}

CThreadPool::~CThreadPool()
{
	// stop all threads
	__uint i;
	for(i=0; i<count; i++){
		threads[i].stop(INFINITE);
	}
	delete []threads;
	threads = 0;
	count = 0;
}

void CThreadPool::startAll()
{
	__uint i;
	for(i=0; i<count; i++){
		threads[i].start();
		threads[i].wait(CRtkThread::EVT_INIT_COMPLETE, INFINITE);
	}
}

LIBRTK_API RTK_HANDLE RTK_API create_rthread_pool(__uint size)
{
	CThreadPool * p = 0;
	try{
		p = new CThreadPool(size);
	}catch(CThreadPool*){
		if(p){
			delete p;
		}
		return 0;
	}
	p->startAll();
	return (RTK_HANDLE)p;
}

LIBRTK_API RTK_HANDLE RTK_API get_thread_from_pool(
	RTK_HANDLE pool, 
	__uint index
	)
{
	CThreadPool * p;
	p = (CThreadPool*)pool;
	if(index >= p->count){
		return 0;
	}
	return (RTK_HANDLE)&p->threads[index];
}

LIBRTK_API __uint RTK_API query_pool_items(RTK_HANDLE pool)
{
	CThreadPool * p;
	CPoolWorkerThread * w;
	int affinity;
		
	p = (CThreadPool*)pool;

	__uint items=0;
	
	for(affinity = 0, w = &p->threads[0]; affinity < p->count; affinity++, w++){
		items+=w->items();
	}
	return items;
}

LIBRTK_API __bool RTK_API queue_pool_item(
	RTK_HANDLE pool, 
	int affinity,
	RTHREAD_POOL_ITEM item, 
	__uint parameter
	)
{
	CThreadPool * p;
	CPoolWorkerThread * w;
		
	p = (CThreadPool*)pool;
	w = 0;
	if(affinity == (int)-1){
		// select an worker with a lowest work load
		CPoolWorkerThread * wm = 0;
		__uint minItems;
		
		minItems = (__uint)-1;
		for(affinity = 0, w = &p->threads[0]; affinity < p->count; affinity++, w++){
			if(w->items() < minItems){
				minItems = w->items();
				wm = w;
			}
		}
		w = wm;
	}else{
		if(affinity < p->count){
			w = &p->threads[affinity];
		}
	}

	if(!w){
		/* 2005-10-27 we should be more error tolerant */
		// return __false;
		
		// select an worker with a lowest work load
		CPoolWorkerThread * wm = 0;
		__uint minItems;
		
		minItems = (__uint)-1;
		for(affinity = 0, w = &p->threads[0]; affinity < p->count; affinity++, w++){
			if(w->items() < minItems){
				minItems = w->items();
				wm = w;
			}
		}
		w = wm;
	}
	
	return w->queueWorkItem(item, parameter);
}

