#include "precomp.h"
#pragma hdrstop

#include <time.h>

static CLock flock;

LIBRTK_API int RTK_API rtk_logged_puts(MSG_LEVEL level, char *s)
{
	static char last_log[256];
	static int last_count = 1;
	int bytes;

	flock.Lock();

	bytes = strlen(s);

	if( !strcmp(last_log, s) ){
		last_count++;		
		if(last_count < 1024)
		{
			flock.UnLock();
			return bytes;
		}		
	}

	if(last_count > 1){
		sprintf(last_log, "Last log entry appeared %d times.\n", last_count);
		fputs(last_log, stdout);
		last_count = 1;
	}
	
	fputs(s, stdout);
	strncpy(last_log, s, sizeof(last_log));
	last_log[ sizeof(last_log) - 1] = 0;

	flock.UnLock();

	return bytes;
}

LIBRTK_API int RTK_API rtk_format_time(time_t tmt,char *buf,int len)
{
#ifdef _WIN32
	#define localtime(a,b) *(b)=*localtime(a)
#else	
	#define localtime(a,b) localtime_r(a,b)
#endif
	struct tm ltm;
	if(!tmt)
		time(&tmt);
	localtime(&tmt, &ltm);
	return _snprintf(
		buf,
		len,
		"%dƒÍ%d‘¬%d»’ %02d:%02d:%02d",
		ltm.tm_year+1900,
		ltm.tm_mon+1,
		ltm.tm_mday,
		ltm.tm_hour,
		ltm.tm_min,
		ltm.tm_sec
		);
#ifndef _WIN32
	#undef localtime
#endif	  
}


