/*
	Locking mechanism is provided in case FBlock kernel engine might be 
	drived from more than one processes, full implementation is not available
	yet, further more this code is CPP based, thus making it vulnerable
	to hostile platforms such as linux kernel environment. The CLock version
	is commented out, because CLock depends on librtk.dll, which is not
	available in most embedded platforms.

	A C-based and transplantable version is under development.

	Jackie, 2003/7/5
*/
#include "../precomp.h"
#pragma hdrstop
#include <rtk_thread.h>

void * create_lock()
{
	return create_rtk_rwlock(0);
}

void delete_lock(void * lock)
{
	close_handle(lock);
}

FKERN_API f8_bool ke_lock(HF8_KERNEL _kernel, int writeMode)
{
	// DEBUG_PRINTF(("%08x:T%08x ke_lock(%d,%08x)\n", GetTickCount(),GetCurrentThreadId(),writeMode,((struct kernel_t*)_kernel)->lock));
	if(writeMode && ke_get_flag(FKERN_KEY_LOCK)){
		return f8_false;
	}
	if(writeMode){
		wr_lock_rtk_object(((struct kernel_t*)_kernel)->lock);
	}else{
		rd_lock_rtk_object(((struct kernel_t*)_kernel)->lock);
	}
	// DEBUG_PRINTF(("%08x:T%08x ke_lock(%d,%08x) ok.\n", GetTickCount(),GetCurrentThreadId(),writeMode,((struct kernel_t*)_kernel)->lock));
	return f8_true;
}

FKERN_API void ke_unlock(HF8_KERNEL _kernel, int writeMode)
{
	rw_unlock_rtk_object(((struct kernel_t*)_kernel)->lock);
	// DEBUG_PRINTF(("%08x:T%08x ke_unlock(%d,%08x)\n", GetTickCount(),GetCurrentThreadId(),writeMode,((struct kernel_t*)_kernel)->lock));
}

