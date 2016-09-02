/*
	the portable portion of the controller executive
	07/08/2005 panxh@actech-mc.com
*/
#include "precomp.h"

BLK_SERVICE_PROC blk_services[256];
HF8_HEAP	kern_heap;

f8_u16	type_size_map[256];

struct kernel_t g_kernel;
/* runtime variables */
ktime_t current_time;
ktime_t kern_time_bias;

int taskFlag;
f8_uint kthreadId;
f8_uint agentThreadId;
int sysWatchDogInterval=3;

extern void init_fbin();

FKERN_API void ex_init()
{
#ifdef __VXWORKS__
	init_fbin();
#endif
	
#define _set_size(t,s) type_size_map[t] = s;
	_set_size(PIN_T_INTEGER, 4);
	_set_size(PIN_T_BOOL, 1);
	_set_size(PIN_T_FLOAT, 4);
	_set_size(PIN_T_TIMER, 4);
	_set_size(PIN_T_BYTE, 1);
	_set_size(PIN_T_WORD, 2);
	_set_size(PIN_T_DOUBLE, 8);
	_set_size(PIN_T_DATE, 8);
	_set_size(PIN_T_CHAR, 1);
	_set_size(PIN_T_SHORT, 2);
	_set_size(PIN_T_DWORD, 4);
	_set_size(PIN_T_POINTER, 4);

	init_services();
	init_config();
	kern_heap = mm_create_heap(HEAP_T_DATA, KERN_HEAP_SIZE);
	init_kernel(&g_kernel);

	init_peer();
}

FKERN_API void ex_uninit()
{
	uninit_peer();
	uninit_kernel(&g_kernel);
	if(kern_heap){
		mm_destroy_heap(kern_heap);
		kern_heap = 0;
	}
}

double kiCycleCount;
struct average_array_t phaseTimes[5];

LOCAL void _exec_prog()
{
	int jret;
	jret = setjmp(g_kernel.g_jmpbuf);
	current_time = ke_get_time();
	if(!jret){
		ki_exec_61499();
		ki_exec_fbd();
		ki_exec_ld();
		ki_exec_sfc();
	}else{
		/* exception happened */
//		printf("Exception happed.\n");
	}
}

void cycleTimesShow()
{
	printf(
		"F8KERNEL : %.0f cycles, %.1f per BC(%.1f+%.1f+%.1f+%.1f), timeunit=millisecond\n", 
		kiCycleCount,
		phaseTimes[4].a/10000,
		phaseTimes[0].a/10000,
		phaseTimes[1].a/10000,
		phaseTimes[2].a/10000,
		phaseTimes[3].a/10000
		);
}

int iCycleCounter,outputEnabled;

int RTK_API baseCycle(struct kernel_t * unused)
{
	ktime_t t3, t4, t5;
	int i;
	static f8_r8 phaseTimeBuffers[5][20];

	ki_load_nvram(&g_kernel);

	kiCycleCount = 0;

	g_kernel.status.state = KERN_S_STOPPED;
	g_kernel.status.firmwareVersion=f8FirmwareVersion;
	
	if(ke_get_flags() & KERN_F_ENABLE_IO){
		init_io();
		start_io();
	}
	
	memset(phaseTimes, 0, sizeof(phaseTimes));
	for(i=0;i<5;i++){
		ki_init_average(&phaseTimes[i],phaseTimeBuffers[i],20,1e8,0);
	}
	
	g_kernel.status.upTime=ke_get_time();
	/* base processing cycle */
	outputEnabled=0;
#ifdef __VXWORKS__
	/*
	if(ke_get_flags() & KERN_F_ENABLE_WATCHDOG){
		sysEnableWatchdog(128-sysWatchDogInterval);
	}
	*/
#endif
	
	while(taskFlag != TSK_STOPPING){
		ki_update_flags();
		if(g_kernel.status.state != KERN_S_RUNNING){
			iCycleCounter=0;
		}else{
			if(iCycleCounter < 10){
				iCycleCounter++;
			}else{
				outputEnabled=1;
			}
		}
		t3 = t5 = ke_get_time();
		t4=t3;
		if(ke_get_flags() & KERN_F_ENABLE_INPUT){
			process_input(&g_kernel);
			t4 = ke_get_time();
			ki_average(&phaseTimes[0], (f8_r8)(t4-t3));
			t3 = t4;
		}else{
			ki_average(&phaseTimes[0], 0.);
		}
		
		if(g_kernel.status.state==KERN_S_RUNNING && (ke_get_flags()&KERN_F_ENABLE_SCHEDULER)){
			ke_lock(&g_kernel, 0);
			_exec_prog();
			ke_unlock(&g_kernel, 0);			
			t4 = ke_get_time();
			ki_average(&phaseTimes[1], (f8_r8)(t4-t3));
			t3 = t4;
		}else{
			ki_average(&phaseTimes[1], 0.);
		}

		/*
			note we don't write any output until the 11th base-cycle
			after switching to RUNNING state, the time is for the output 
			values to becone stablized.
		*/
		if((ke_get_flags() & KERN_F_ENABLE_OUTPUT) && outputEnabled){
			ke_set_flag(FKERN_LED_BUSA, !ke_get_flag(FKERN_LED_BUSA));
			ke_set_flag(FKERN_LED_BUSB, !ke_get_flag(FKERN_LED_BUSA));
			
			process_output(&g_kernel);
			t4 = ke_get_time();
			ki_average(&phaseTimes[2], (f8_r8)(t4-t3));
			t3 = t4;
		}else{
			ki_average(&phaseTimes[2], 0.);
		}
			
		if(ke_get_flags() & KERN_F_ENABLE_STANDBY){
			ke_lock(&g_kernel, 0);
			peer_sync(&g_kernel);
			ke_unlock(&g_kernel, 0);
			t4 = ke_get_time();
			ki_average(&phaseTimes[3], (f8_r8)(t4-t3));
			t3 = t4;
		}else{
			ki_average(&phaseTimes[3], 0.);
		}
		
		kiCycleCount = kiCycleCount + 1;
		ki_average(&phaseTimes[4], (f8_r8)(t4-t5));
		
#ifdef _WIN32
		rtk_sleep(20);
#endif
		for(i=0;i<5;i++)
			g_kernel.status.n_phaseTimes[i]=(f8_u16)(phaseTimes[i].a/10/100);
		g_kernel.status.n_cycles=(f8_r4)kiCycleCount;
#ifdef __VXWORKS__
		if(ke_get_flags() & KERN_F_ENABLE_WATCHDOG){
			// sysRefreshWatchdog(128-sysWatchDogInterval);
		}else{
		/* changed flags take immediate effect, comment this out for release version
		**/
			// sysDisableWatchdog();
		}
#endif
	}


#ifdef __VXWORKS__
	/*
	if(ke_get_flags() & KERN_F_ENABLE_WATCHDOG){
		sysDisableWatchdog();
	}
	*/
#endif
	
	ki_switch_to(KERN_S_STOPPED);
	
	if(ke_get_flags() & KERN_F_ENABLE_IO){
		stop_io();
		uninit_io();
	}

	cycleTimesShow();
#ifndef _WIN32
	kthreadId = ERROR;
#endif
	return 0;
}

void cpu_show()
{
	int i;
	for(i=0;i<KERN_NUM_SECTIONS;i++){
		printf("x_mem[%d]=%08x~%08x,length=0x%08x\n",i,g_kernel.x_mem_pointers[i],
			g_kernel.x_mem_pointers[i]+g_kernel.x_mem_sizes[i],g_kernel.x_mem_sizes[i]);
	}
	printf("IO_mem  =%08x~%08x,length=%08x\n",g_kernel.io_memory,
		g_kernel.io_memory+sizeof(g_kernel.io_memory),sizeof(g_kernel.io_memory));
}

