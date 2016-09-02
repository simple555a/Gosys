#include "precomp.h"
//
PROXY_DISPATCH_TABLE	PROXY_API g_ProxyDispatchTable;

//
RTDB_DISPATCH_TABLE	g_DBDispatchTable;

//事件记录结构体？？
typedef struct{
	int			type;                //事件类型
	union{		
		TAG_NAME	tag;             //三段式标签名
		struct{
			int			rank;
			ALARM_CLASS cls;
			char		*msg;			
		}alarm;                      //报警事件信息
		struct{
			__uint	total;
			__uint	completed;
			char	*msg;
		}progress;                   //progress
		DEVICE_INFO	di;              //设备信息
	};
	RTK_ADDR	src;
}_EVENT;

//
static void	 _dispatch_event(_EVENT *e);

//消息队列
class CMessageQueue
#ifndef _WIN32
	: public CLock
#endif
{
public:
	/*构造函数**************************************/
	CMessageQueue()                                       
	{
#ifdef _WIN32			   
		m_hWnd = NULL;
		m_OldProc = NULL;
#endif
		m_bViable = __false;
	}
#ifdef _WIN32
	/***************************************/
	HWND	m_hWnd;
	WNDPROC	m_OldProc;
    /*_WindowProc()*************************************/
	static LRESULT CALLBACK CMessageQueue::_WindowProc(
		HWND hwnd,      // handle to window
		UINT uMsg,      // message identifier
		WPARAM wParam,  // first message parameter
		LPARAM lParam   // second message parameter
		);
#else
	typedef list<_EVENT*> CEventList;
	CEventList m_Events;
#endif
	/*Create()**************************************/
	__bool Create()
	{
#ifdef _WIN32
		m_hWnd = CreateWindow(
			"EDIT",
			"ProxyEventsSink",
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
		m_bViable = __true;
		return __true;
#else
		m_bViable = __true;
#endif
	}
	/*Queue()**************************************/
	__bool Queue(_EVENT * e)
	{
#ifdef _WIN32
		// event is queued and posted to the thread which invoked
		// init_proxy
		return PostMessage(m_hWnd, WM_USER + 426, (WPARAM)e, 0);
#else
		// event is queued but not posted
		Lock();
		m_Events.insert(m_Events.end(), e);
		UnLock();
		return __true;
#endif			
	}
#ifndef _WIN32	 
	__uint Dispatch()
	{
		__uint count=0;
		_EVENT * e;
		CEventList::iterator it;
		while(m_Events.size()){
			e = NULL;
			Lock();
			it = m_Events.begin();
			if(it != m_Events.end()){
				e = *it;
				m_Events.erase(it);
			}
			UnLock();
			if(e != NULL){
				_dispatch_event(e);
				count++;
			}
		}
		return count;
	}
#endif
	/*Destroy()*******************************/
	void Destroy()
	{
		if(!Viable()){
			return;
		}
#ifdef _WIN32
		DestroyWindow(m_hWnd);
#else
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
#endif
		m_bViable = __false;
	}
	/*Viable()*****************************/
	__bool Viable()
	{
		return m_bViable;
	}
	__bool m_bViable;   /*false, 不可行； true, 可行*/
};

static CMessageQueue g_Queue;

/*****************************************************************************
******************************************************************************/
#ifdef _WIN32
/*
 *
 *
 */
LRESULT CALLBACK CMessageQueue::_WindowProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	if(uMsg == (WM_USER + 426)){
		_dispatch_event((_EVENT*)wParam);
		return 1;
	}
	return CallWindowProc(g_Queue.m_OldProc, hwnd, uMsg, wParam, lParam);
}
#endif

/*
 *
 */
static void _OnAddNode(PRTK_NODE pnode)
{
	TAG_NAME tn;
	tn.node = pnode->key;
	memset(&tn.sname, 0, sizeof(tn.sname));
	rtk_queue_event(PT_AddNode, &tn, 0);
}

static void _OnDropNode(PRTK_NODE pnode)
{
	TAG_NAME tn;
	tn.node = pnode->key;
	memset(&tn.sname, 0, sizeof(tn.sname));
	rtk_queue_event(PT_DropNode, &tn, 0);
}

static void _OnAddTag(PRTK_TAG pTag)
{
	TAG_NAME tn;
	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;
	rtk_queue_event(PT_AddTag, &tn, 0);
}

static void _OnDropTag(PRTK_TAG pTag)
{
	TAG_NAME tn;
	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;
	rtk_queue_event(PT_DropTag, &tn, 0);
}

static void _OnModifyTag(PRTK_TAG pTag)
{
	TAG_NAME tn;
	tn.node = pTag->node;
	tn.sname.group = pTag->group;
	tn.sname.tag = pTag->key;
	rtk_queue_event(PT_EditTag, &tn, 0);
}

static void _OnAddGroup(PRTK_GROUP pgrp)
{
	TAG_NAME tn;
	tn.node = pgrp->node;
	tn.sname.group = pgrp->key;
	ZeroMemory(&tn.sname.tag, sizeof(tn.sname.tag));
	rtk_queue_event(PT_AddGroup, &tn, 0);
}

static void _OnDropGroup(PRTK_GROUP pgrp)
{
	TAG_NAME tn;
	tn.node = pgrp->node;
	tn.sname.group = pgrp->key;
	ZeroMemory(&tn.sname.tag, sizeof(tn.sname.tag));
	rtk_queue_event(PT_DropGroup, &tn, 0);
}

static void _OnModifyGroup(PRTK_GROUP pgrp)
{
	TAG_NAME tn;
	tn.node = pgrp->node;
	tn.sname.group = pgrp->key;
	ZeroMemory(&tn.sname.tag, sizeof(tn.sname.tag));
	rtk_queue_event(PT_EditGroup, &tn, 0);
}

/*
 *
 */
__bool start_dispatcher()
{
#define _hook_(item) g_DBDispatchTable.item = _##item;
	_hook_(OnAddNode)               /*g_DBDispatchTable.OnAddNode = _OnAddNode*/
	_hook_(OnDropNode)              /*g_DBDispatchTable.OnDropNode = _OnDropNode*/
#if 0
	_hook_(OnAddGroup)
	_hook_(OnDropGroup)
	_hook_(OnModifyGroup)
	
	_hook_(OnAddTag)
	_hook_(OnDropTag)
	_hook_(OnModifyTag)
#endif
	hook_rtdb_events(&g_DBDispatchTable);   //librtdb.dll

	return g_Queue.Create();
}

void stop_dispatcher()
{
	g_Queue.Destroy();
	unhook_rtdb_events(&g_DBDispatchTable);
}

/*
 *函数功能：对_EVENT的各成员赋值(如成员type、src、tag...)
 */
PROXY_API __bool PMC_API rtk_queue_event(int id, ...)
{
	PTAG_NAME name;
	_EVENT	*e;
	va_list args;

	if(!g_Queue.Viable()){
		return __false;
	}

	e = new _EVENT;
	if(!e){
		return __false;
	}
	e->type = id;
	va_start(args, id);
   /*
	*给_EVENT e->src e->tag赋值
	*/
#define qtag()\
	do{\
		name = va_arg(args, PTAG_NAME);\
		PRTK_ADDR addr = va_arg(args, PRTK_ADDR);\
		if(addr){\
			e->src = *addr;\
		}else{\
			memset(&e->src, 0, sizeof(e->src));\
		}\
		e->tag = *name;\
	}while(0)
	switch(id){
	case PT_AddTag:
		if(g_ProxyDispatchTable.OnAddTag){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_DropTag:
		if(g_ProxyDispatchTable.OnAddTag){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_EditTag:
		if(g_ProxyDispatchTable.OnAddTag){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_AddGroup:
		if(g_ProxyDispatchTable.OnAddGroup){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_DropGroup:
		if(g_ProxyDispatchTable.OnDropGroup){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_EditGroup:
		if(g_ProxyDispatchTable.OnGroupModified){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_Alarm:
		if(g_ProxyDispatchTable.OnAlarm){
			e->alarm.rank = va_arg(args, int);
			e->alarm.cls   = va_arg(args, ALARM_CLASS);			
			e->alarm.msg  = strdup(va_arg(args, char *));
			e->src = *va_arg(args, PRTK_ADDR);
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_Progress:
		if(g_ProxyDispatchTable.OnProgress){
			e->progress.total = va_arg(args, __uint);
			e->progress.completed = va_arg(args, __uint);
			e->progress.msg   = strdup(va_arg(args, char*));
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_AddDevice:
		if(g_ProxyDispatchTable.OnAddDevice){
			e->src = *(va_arg(args, PRTK_ADDR));
			e->di  = *(va_arg(args, PDEVICE_INFO));
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_EditDevice:
		if(g_ProxyDispatchTable.OnDeviceModified){
			e->src = *(va_arg(args, PRTK_ADDR));
			e->di  = *(va_arg(args, PDEVICE_INFO));
		}else{
			delete e;
			e = 0;
		}
		break;	
	case PT_DelDevice:
		if(g_ProxyDispatchTable.OnDropDevice){
			e->src = *(va_arg(args, PRTK_ADDR));
			e->di.k  = *(va_arg(args, PDEVICE_KEY));
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_AddNode:
		if(g_ProxyDispatchTable.OnAddNode){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	case PT_DropNode:
		if(g_ProxyDispatchTable.OnDropNode){
			qtag();
		}else{
			delete e;
			e = 0;
		}
		break;
	}
	if(e){
		if(!g_Queue.Queue(e)){      //发送事件消息？？
			delete e;
			e = 0;
		}
	}

	return e? __true : __false;
}

/*
函数说明：根据_EVENT的类型，执行相应的g_ProxyDispatchTable的操作
*/
static void  _dispatch_event(_EVENT *e)
{
	NODE_KEY nd;
	switch(e->type){
	case PT_AddTag:
		if(g_ProxyDispatchTable.OnAddTag){
			g_ProxyDispatchTable.OnAddTag(&e->tag);
		}
		break;
	case PT_DropTag:
		if(g_ProxyDispatchTable.OnDropTag){
			g_ProxyDispatchTable.OnDropTag(&e->tag);
		}
		break;
	case PT_EditTag:
		if(g_ProxyDispatchTable.OnTagModified){
			g_ProxyDispatchTable.OnTagModified(&e->tag);
		}
		break;
	case PT_AddGroup:
		if(g_ProxyDispatchTable.OnAddGroup){
			g_ProxyDispatchTable.OnAddGroup(&e->tag);
		}
		break;
	case PT_DropGroup:
		if(g_ProxyDispatchTable.OnDropGroup){
			g_ProxyDispatchTable.OnDropGroup(&e->tag);
		}
		break;
	case PT_EditGroup:
		if(g_ProxyDispatchTable.OnGroupModified){
			g_ProxyDispatchTable.OnGroupModified(&e->tag);
		}
		break;
	case PT_Alarm:
		if(g_ProxyDispatchTable.OnAlarm){
			g_ProxyDispatchTable.OnAlarm(
				e->alarm.rank, e->alarm.cls, e->alarm.msg, &e->src
				);				
		}
		free(e->alarm.msg);
		break;
	case PT_Progress:
		if(g_ProxyDispatchTable.OnProgress){
			g_ProxyDispatchTable.OnProgress(
				e->progress.total, e->progress.completed, e->progress.msg
				);				
		}
		utils_debug(
			"Progress %d/%d: %s\n", 
			e->progress.completed, e->progress.total, e->progress.msg
			);
		free(e->progress.msg);
		break;
	case PT_AddDevice:
		if(g_ProxyDispatchTable.OnAddDevice){
			host_to_node(&e->src.host, &nd);
			g_ProxyDispatchTable.OnAddDevice(&nd, &e->di);
		}
		break;
	case PT_EditDevice:
		if(g_ProxyDispatchTable.OnDeviceModified){
			host_to_node(&e->src.host, &nd);
			g_ProxyDispatchTable.OnDeviceModified(&nd, &e->di);
		}
		break;
	case PT_DelDevice:
		if(g_ProxyDispatchTable.OnDropDevice){
			host_to_node(&e->src.host, &nd);
			g_ProxyDispatchTable.OnDropDevice(&nd, &e->di.k);
		}
		break;
	case PT_AddNode:
		if(g_ProxyDispatchTable.OnAddNode){
			g_ProxyDispatchTable.OnAddNode(&e->tag.node);
		}
		break;
	case PT_DropNode:
		if(g_ProxyDispatchTable.OnAddNode){
			g_ProxyDispatchTable.OnDropNode(&e->tag.node);
		}
		break;
	}
	delete e;
}

PROXY_API void	PMC_API rtk_dispatch_events()
{
#ifdef _WIN32
	// do nothing as events are automatically dispatched in 
	// rtk_queue_event
#else
	g_Queue.Dispatch();
#endif
}
