/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\rtk\object.h
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	provide C-based rtk thread support

	history:
	created:	11:7:2003   9:33
	
	jhunter, 2004/8/8
	- thread pooling and worker item support added
*********************************************************************/
#ifndef __rthread_h__
#define __rthread_h__

#include <librtk.h>

typedef __uint (RTK_API *RTK_THREAD_PROC)(__uint parameters);

CDECL_BEGIN

LIBRTK_API RTK_HANDLE RTK_API create_rthread(RTK_THREAD_PROC, __uint);

LIBRTK_API RTK_HANDLE RTK_API get_thread_event(RTK_HANDLE h);

LIBRTK_API __bool RTK_API start_rthread(RTK_HANDLE h);

LIBRTK_API __bool RTK_API stop_rthread(RTK_HANDLE);

LIBRTK_API void RTK_API suspend_rthread(RTK_HANDLE);

LIBRTK_API void RTK_API resume_rthread(RTK_HANDLE);

LIBRTK_API void RTK_API notify_rthread(RTK_HANDLE h);

LIBRTK_API	void RTK_API wait_rthread(RTK_HANDLE, __uint timeout);

LIBRTK_API __uint RTK_API id_rthread(RTK_HANDLE);

LIBRTK_API RTK_HANDLE create_periodic_rthread(
	RTK_THREAD_PROC,
	__uint param,
	__uint timeout
	);

LIBRTK_API RTK_HANDLE RTK_API create_rtk_lock(const char * name);

LIBRTK_API RTK_HANDLE RTK_API create_rtk_mutex(const char * name);

LIBRTK_API RTK_HANDLE RTK_API create_rtk_event(
	const char * name,
	__bool auto_reset,
	__bool initial_state
	);

LIBRTK_API RTK_HANDLE RTK_API create_rtk_rwlock(const char * name);

LIBRTK_API RTK_HANDLE RTK_API create_rtk_semaphore(
	const char * name, 
	__uint max_value, 
	__uint init_value
	);

LIBRTK_API void RTK_API set_rtk_event(RTK_HANDLE);

LIBRTK_API void RTK_API reset_rtk_event(RTK_HANDLE);

LIBRTK_API __uint RTK_API wait_rtk_event(
	RTK_HANDLE h, 
	__uint timeout
	);

LIBRTK_API void RTK_API lock_rtk_object(RTK_HANDLE);

LIBRTK_API void RTK_API unlock_rtk_object(RTK_HANDLE);

LIBRTK_API void RTK_API rd_lock_rtk_object(RTK_HANDLE);

LIBRTK_API void RTK_API wr_lock_rtk_object(RTK_HANDLE);

LIBRTK_API void RTK_API rw_unlock_rtk_object(RTK_HANDLE);

#define MAX_HANDLES_PER_WAIT 64

LIBRTK_API __uint RTK_API msg_wait_multiple(
	__uint count, 
	RTK_HANDLE * handles, 
	__bool bWaitall,
	__uint timeout, 
	__uint flags
	);

/*
	thread pooling and worker item support
*/
LIBRTK_API RTK_HANDLE RTK_API create_rthread_pool(__uint size);

LIBRTK_API RTK_HANDLE RTK_API get_thread_from_pool(RTK_HANDLE pool, __uint index);

typedef void (RTK_API *RTHREAD_POOL_ITEM)(__uint parameters);

LIBRTK_API __bool RTK_API queue_pool_item(
	RTK_HANDLE pool, 
	int affinity,
	RTHREAD_POOL_ITEM item, 
	__uint parameter
	);

LIBRTK_API __uint RTK_API query_pool_items(RTK_HANDLE pool);

LIBRTK_API __bool RTK_API stop_request_pending(RTK_HANDLE thread);
	
CDECL_END

#endif
