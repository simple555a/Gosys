/*
	F8 executive entry point, this is implementation
	for the simulator.

	by J.Hunter, jhunter@tsinghua.org.cn

	2003/10/29 a quick and dirty implementation
*/
#include "../precomp.h"
#pragma hdrstop

#include <rtk_thread.h>
#include <crtdbg.h>

#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "libzlib_shared.lib")
#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "librtknet.lib")
#pragma comment (lib, "libcobj.lib")
#pragma comment (lib, "libCPmm.lib")
#pragma comment (lib, "libfbd.lib")

/*
	ex_start()

	start the executive as a whole, aber the ki_start only start the
	user program.
*/
FKERN_API f8_status ex_start(int flags, int port)
{
	struct kernel_t * kernel = (struct kernel_t *)&g_kernel;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	printf("Starting F8 runtime kernel...\n");
	
	kernel->status.flags = flags;

	if(flags & KERN_F_ENABLE_SCHEDULER){
		kthreadId = (__u32)create_rthread((RTK_THREAD_PROC)baseCycle, (__uint)&g_kernel);
		if(kthreadId){
			start_rthread((RTK_HANDLE)kthreadId);
		}
	}

	if(flags & KERN_F_ENABLE_AGENT){
		if(port){
			agentPort = port;
		}
		agentThreadId = (__u32)create_rthread(
			f8Agent, 
			(__uint)&g_kernel
			);
		if(agentThreadId){
			start_rthread((RTK_HANDLE)agentThreadId);
		}
	}
	
	printf("F8 runtime kernel started.\n");
	
	return kthreadId? F8_SUCCESS : F8_LOW_MEMORY;
}

FKERN_API f8_status ex_stop()
{
	struct kernel_t * kernel = (struct kernel_t*)&g_kernel;
	
	printf("Shutting down F8 runtime kernel...\n");

	taskFlag = TSK_STOPPING;
	
	if(kthreadId){
		stop_rthread((RTK_HANDLE)kthreadId);
		close_handle((RTK_HANDLE)kthreadId);
	}
	if(agentThreadId){
		taskFlag = TSK_STOPPING;
		stop_rthread((RTK_HANDLE)agentThreadId);
		close_handle((RTK_HANDLE)agentThreadId);
	}
	kthreadId = 0;
	agentThreadId = 0;
	printf("F8 runtime kernel stopped.\n");
	ke_lock(kernel, 0);
	ki_save_nvram(kernel);
	ke_unlock(kernel,0);

	return F8_SUCCESS;
}

/*
	get time relative to Jan 1, 1601
	in terms of 100 ns.
*/
FKERN_API ktime_t ke_get_time()
{
	FILETIME ft,ft2;
	GetSystemTimeAsFileTime(&ft);
	FileTimeToLocalFileTime(&ft, &ft2);
	return *((ktime_t*)&ft2);
}

FKERN_API void ke_time_to_systime(
	ktime_t * ftime,
	SYSTEMTIME * st
)
{
	FileTimeToSystemTime((FILETIME*)ftime,st);
}


