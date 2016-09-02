/*
	F8 executive entry point.
	This is implementation for the VxWorks target.

	by J.Hunter, jhunter@tsinghua.org.cn

	2003/10/29 a quick and dirty implementation
*/
#include "../precomp.h"
#pragma hdrstop

#include <taskLib.h>
#include <timers.h>
#include <inetLib.h>

// #define ENABLE_IP_AUTO_SWITCH

#if defined(ENABLE_IP_AUTO_SWITCH)

LOCAL struct in_addr primaryIp1, primaryIp2;
LOCAL struct in_addr standbyIp1, standbyIp2;
LOCAL int ipDaemonTaskId;

/*
*/
LOCAL int ipDaemonTask()
{
	char nameBuf[INET_ADDR_LEN];
	struct in_addr curr;
	int err;
	
	while(taskFlag != TSK_STOPPING){
		taskDelay(500);
		
		if(ke_get_flag(FKERN_LED_SOFT_LOCK)){
			/*
				no ip-transition is performed while an agent is attached.
			*/
			continue;
		}
		
		if(g_kernel.status.state == KERN_S_RUNNING){
			ifAddrGet("rtl0", nameBuf);
			curr.s_addr=inet_addr(nameBuf);
			if(curr.s_addr != primaryIp1.s_addr){
				strcpy(nameBuf, inet_ntoa(primaryIp1));
				err=ifAddrSet("rtl0", nameBuf);
				printf("IP-1 changed from %s to %s, code %d.\n", inet_ntoa(curr), nameBuf, err);
			}
#if 0
			ifAddrGet("rtl1", nameBuf);
			curr.s_addr=inet_addr(nameBuf);
			if(curr.s_addr != primaryIp2.s_addr){
				strcpy(nameBuf, inet_ntoa(primaryIp2));
				err=ifAddrSet("rtl1", nameBuf);
				printf("IP-2 changed from %s to %s, code %d.\n", inet_ntoa(curr), nameBuf, err);
			}
#endif
		}else{
			ifAddrGet("rtl0", nameBuf);
			curr.s_addr=inet_addr(nameBuf);
			if(curr.s_addr != standbyIp1.s_addr){
				strcpy(nameBuf, inet_ntoa(standbyIp1));
				err=ifAddrSet("rtl0", nameBuf);
				printf("IP-1 changed from %s to %s, code %d.\n", inet_ntoa(curr), nameBuf, err);
			}
#if 0
			ifAddrGet("rtl1", nameBuf);
			curr.s_addr=inet_addr(nameBuf);
			if(curr.s_addr != standbyIp2.s_addr){
				strcpy(nameBuf, inet_ntoa(standbyIp2));
				err=ifAddrSet("rtl1", nameBuf);
				printf("IP-2 changed from %s to %s, code %d.\n", inet_ntoa(curr), nameBuf, err);
			}
#endif
		}
	}

	ipDaemonTaskId=-1;

	return 0;
}

#endif

#define DIFFTIME_FROM_1601_TO_1900 116444736000000000.0

static void computeTimeBase()
{
	ktime_t t;
	struct tm tmWin32TimeBase;
	time_t tNow, tWin32TimeBase;
	struct tmBuf;
	double diff;

	kern_time_bias = 0;
	t = ke_get_time();
	
	time(&tNow);

	memset(&tmWin32TimeBase, 0, sizeof(tmWin32TimeBase));
	tmWin32TimeBase.tm_mday = 1;
	tmWin32TimeBase.tm_year = 0;
	tmWin32TimeBase.tm_mon = 1;
	tWin32TimeBase = mktime(&tmWin32TimeBase);

	diff = tNow * 1e7;

	kern_time_bias = -t + diff + DIFFTIME_FROM_1601_TO_1900;
}

extern BOOL systemStarted;
/*
	spawn a kernel object
*/
FKERN_API f8_status ex_start(int flags, int port)
{
	char nameBuf[INET_ADDR_LEN];

	flags &= KERN_FEATURES_MASK;
	g_kernel.status.flags = flags;
	taskFlag = TSK_RUNNING;
	kthreadId = taskSpawn(
		"f8Sched",	/* name */
		255,			/* priority */
		VX_FP_TASK,		/* options */
		128*1024,		/* stack size */
		baseCycle,		/* entry point */
		(int)&g_kernel, 0, 0,0,0,0,0,0,0,0	/* arg 1~10 */
		);
	printf("F8 kernel started as 0x%08x\n", kthreadId);

	if(port){
		agentPort = port;
	}
	if(flags & KERN_F_ENABLE_AGENT){
		agentThreadId = taskSpawn(
			"tf8Agent",	/* name */
			51,				/* priority */
			VX_FP_TASK,		/* options */
			128*1024,		/* stack size */
			f8Agent,		/* entry point */
			(int)&g_kernel,0,0,0,0,0,0,0,0,0	/* arg 1~10 */
			);
		printf("F8 agent started as 0x%08x\n", agentThreadId);
	}else{
		agentThreadId=ERROR;
	}

#if defined(ENABLE_IP_AUTO_SWITCH)
#error not support
	printf("Starting IP daemon....\n");
	ifAddrGet("rtl0", nameBuf);
	primaryIp1.s_addr=inet_addr(nameBuf)&~0x1000000;
	ifAddrGet("rtl1", nameBuf);
	primaryIp2.s_addr=inet_addr(nameBuf)&~0x1000000;
	standbyIp1.s_addr=primaryIp1.s_addr+0x1000000;
	standbyIp2.s_addr=primaryIp2.s_addr+0x1000000;
	ipDaemonTaskId=taskSpawn("tIpDaemon", 250, 0, 8*1024, ipDaemonTask, 1,2,3,4,5,6,7,8,9,10);
#endif

	ke_set_flag(FKERN_LED_EX_RUNNING,1);
	return F8_SUCCESS;
}

LOCAL void setupTime()
{
	int count;
	// wait for usrAppInit finish initializing
	while(!systemStarted  && count++<200)
		taskDelay(10);
	/*
		read CMOS clock, maybe windRiver should make this a default
		behaviour.
	*/
	rtc_SetSysTimeFromCmos();
	computeTimeBase();
}

FKERN_API f8_status ex_stop()
{
	taskFlag = TSK_STOPPING;
	
	while(kthreadId != ERROR){
		taskDelay(1);
	}
	
	while(agentThreadId != ERROR){
		taskDelay(1);
	}
#if defined(ENABLE_IP_AUTO_SWITCH)	
	while(ipDaemonTaskId != -1){
		taskDelay(10);
	}
#endif
	ke_set_flag(FKERN_LED_EX_RUNNING,0);
	return F8_SUCCESS;
}

int ex_main_y()
{
	ex_stop();
	ex_uninit();
	return F8_SUCCESS;
}

int ex_main_z(int flags)
{
	setupTime();
	ex_init();
	return ex_start(flags, 0);
}

int ex_main_x()
{
	return ex_main_z(KERN_F_NORMAL);
}

int ex_main_x2()
{
	return ex_main_z(KERN_F_NORMAL & ~KERN_F_ENABLE_WATCHDOG);
}

int ex_main_x3()
{
	return ex_main_z(KERN_F_SINGLE);
}

int ex_main_x4()
{
	return ex_main_z(KERN_F_SINGLE  & ~KERN_F_ENABLE_INPUT & ~KERN_F_ENABLE_WATCHDOG);
}

int rr()
{
	extern char f8ImagePath[256];
	if(ke_get_flag(FKERN_LED_EX_RUNNING))
		ex_main_y();
	if(!strncmp(f8ImagePath,"host://",7))
		hostexec(f8ImagePath+7,"ex_main_z",ke_get_flags(),0,0);
	else if(!strncmp(f8ImagePath,"local://",8))
		localexec(f8ImagePath+8,"ex_main_z",ke_get_flags(),0,0);
}

int yy()
{
	return ex_main_y();
}

/*
	get time relative to Jan 1, 1601 in terms of 100 ns.
	note kernel time will leap upon standby/primary switch
	events.
*/
FKERN_API ktime_t ke_get_time()
{
	struct timespec tp;
	ktime_t t;

	clock_gettime(CLOCK_REALTIME, &tp);
	t = (ktime_t)tp.tv_sec * 10000000 + tp.tv_nsec/100;
	t += kern_time_bias;

	return t;
}

/*
	sleep for n milliseconds
	
	note we cannot use ke_get_time, because when standy/primary
	switch occurs, the kernel clock might leap which may leads to a 
	infinite loop
*/
void miliSleep(unsigned int miliseconds)
{
	//modified by cj @ 2009-9-9
	taskDelay(miliseconds/2+1);	
	/*
	struct timespec tp;
	ktime_t t, t2;
	clock_gettime(CLOCK_REALTIME, &tp);
	t = (ktime_t)tp.tv_sec * 10000000 + tp.tv_nsec/100;
	t2=t+miliseconds*10000;
	while(t < t2){
		taskDelay(1);
		clock_gettime(CLOCK_REALTIME, &tp);
		t = (ktime_t)tp.tv_sec * 10000000 + tp.tv_nsec/100;		
	}
	*/
}

unsigned long dayOfTheYear (int year, int mon, int day) 
{   
    /* Days since Jan. 1 in non-leap yr */ 
    static unsigned short jdays[12] = { 
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }; 

    /* leap year is divisible by 4 but not by 100, or by 400 */ 
    return ( jdays[mon-1] + day 
             + ((( (((year%4) == 0) && ((year%100) != 0)) 
                   || ((year%400) == 0)) 
                && (mon>2)) ? 1 : 0) ); 
} 

void setDateTime(unsigned int year, unsigned int mon, 
                                 unsigned int day, unsigned int hour, 
                                 unsigned int min, unsigned int sec, unsigned int wday)
{
	struct tm t;
	t.tm_hour=hour;
	t.tm_isdst=0;
	t.tm_mday=day;
	t.tm_min=min;
	t.tm_mon=mon-1;
	t.tm_sec=sec;
	t.tm_yday = dayOfTheYear(year,mon,day); 
	t.tm_wday=wday;
	t.tm_year=year-1900;
	rtc_settime(&t);
}

void readCmosTime()
{
	struct tm t;
	rtc_gettime(&t);
	printf("%d-%d-%d %02d:%02d:%02d\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
}


