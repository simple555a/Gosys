/*
	Locking mechanism is provided in case FBlock kernel engine might be 
	drived from more than one processes, full implementation is not available
	yet, further more this code is CPP based, thus making it vulnerable
	to hostile platforms such as linux kernel environment. The CLock version
	is commented out, because CLock depends on librtk.dll, which is not
	available in most embedded platforms.

	A C-based and transplantable version is under development.

	Jackie, 2003/7/5

	2005/9/27 use FCFS's read/write lock implementation.
*/
#include "../precomp.h"
#pragma hdrstop

#include <semLib.h>
#include <pthread/rwlock_fcfs.h>

void * create_lock()
{
	pthread_rwlock_fcfs_t * lk;
	lk = pthread_rwlock_fcfs_alloc();
	return lk;
}

void delete_lock(void * lock)
{
	pthread_rwlock_fcfs_t * lk;
	lk = (pthread_rwlock_fcfs_t *)lock;
	pthread_rwlock_fcfs_destroy(lk);
}

FKERN_API f8_bool ke_lock(HF8_KERNEL _kernel, int writeMode)
{
	pthread_rwlock_fcfs_t * lk;
	if(writeMode && ke_get_flag(FKERN_KEY_LOCK)){
		return f8_false;
	}
	lk = (pthread_rwlock_fcfs_t * )((struct kernel_t*)_kernel)->lock;
	if(writeMode){
		pthread_rwlock_fcfs_gain_write(lk);
	}else{
		pthread_rwlock_fcfs_gain_read(lk);
	}
	return f8_true;
}

FKERN_API void ke_unlock(HF8_KERNEL _kernel, int writeMode)
{
	pthread_rwlock_fcfs_t * lk;
	lk = (pthread_rwlock_fcfs_t * )((struct kernel_t*)_kernel)->lock;
	pthread_rwlock_fcfs_release(lk);
}


