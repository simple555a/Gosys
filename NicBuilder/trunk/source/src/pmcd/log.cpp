#include "precomp.h"

static CLock flock;
static char logbuffer[1024*10], *logpointer=logbuffer;
static int loglength;
static FILE * fp_log;
static __bool g_bLogOpened = __false;
__bool logIsEnabled;

std::string _texttime()
{
#ifdef _WIN32
#define localtime(a,b) *(b) = *localtime(a)
#else
#define localtime localtime_r
#endif
	time_t tmt;
	struct tm ltm;
	time(&tmt);
	localtime(&tmt, &ltm);
	char buf[128];    
	sprintf(
		buf,
		"%dÄê%dÔÂ%dÈÕ %02d:%02d:%02d",
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
	return std::string(buf);
}

static std::string getLogFileName()
{
#ifndef _WIN32
#define localtime localtime_r
#endif
	time_t tmt;
	struct tm ltm;
	time(&tmt);
	localtime(&tmt, &ltm);
	char buf[128];    
	// 2004-10-19, log is commited daily, overwritten last day
	// to save disk space
#if 1
	sprintf(
		buf,
		"%04d-%02d%02d.%02d.log",
		ltm.tm_year+1900,
		ltm.tm_mon+1,
		ltm.tm_mday,
		ltm.tm_hour
		);
#else
	sprintf(buf, "%02d.log", ltm.tm_hour);
#endif
#ifndef _WIN32
#undef localtime
#endif    
	return get_working_dir() + ("var/" + std::string(buf));
}

static void commit_Log()
{
	string logfile = getLogFileName();
	fp_log=fopen(logfile.data(), "at");
	if(fp_log){
		fwrite(logbuffer, 1, loglength, fp_log);
		fclose(fp_log);
	}	
	logpointer = logbuffer;
	loglength = 0;
}

static void emitLog(MSG_LEVEL level, char *s)
{
	fputs(s,stdout);
	
	if(level != msg_debug && g_bLogOpened){
		int  bytes = _snprintf(logpointer, sizeof(logbuffer) - loglength, "%s", s);
		logpointer += bytes;
		loglength += bytes;		
		if(loglength > (sizeof(logbuffer)-512)){
			commit_Log();
		}
	}
}

static void emitTimestamp(MSG_LEVEL level)
{
	char tbuf[64];

	rtk_format_time(0,tbuf,sizeof(tbuf));
	emitLog(level,tbuf);
	switch(level){
		case msg_debug:
			emitLog(level,"[debug]");
			break;
		case msg_trace:
			emitLog(level,"[trace]");
			break;
		case msg_error:
			emitLog(level,"[error]");
			break;
	}
	emitLog(level,": ");
}

static void timedStampedLog(MSG_LEVEL level, char *s)
{
	static char lbuf[1024];
	static char *pb=lbuf;
	char *ps;
	static bool bNewline=true;
	
	// copy to bb upto the first '\r\n'
	ps=s;

	for(ps=s;*ps;ps++,pb++){
		if(*ps=='\n' || pb==lbuf+sizeof(lbuf)-1){
			// emit this portion
			*pb=0;
			emitTimestamp(level);
			emitLog(level,lbuf);
			// start another line 
			emitLog(level,"\n");
			// reset target pointer
			pb=lbuf-1;
		}else
			*pb=*ps;
	}
}

static int PMC_API utils_puts_with_log(MSG_LEVEL level,char *s)
{
	static char last_log[256];
	static int last_count = 1;
	int bytes;

	bytes = strlen(s);

	flock.Lock();

	if(!strcmp(last_log, s)){
		last_count++;
		flock.UnLock();
		return bytes;
	}

	if(last_count > 1){
		sprintf(last_log, "Last log entry appears %d times.\n", last_count);
		timedStampedLog(level, last_log);
		last_count = 1;
	}

	strncpy(last_log,s,sizeof(last_log));
	last_log[ sizeof(last_log) - 1] = 0;

	timedStampedLog(level, last_log);
	
	flock.UnLock();

	return bytes;
}

void close_log()
{
	utils_trace("Log stopped on %s\n", _texttime().data());
	if( g_bLogOpened ){
		flock.Lock();
		commit_Log();
		flock.UnLock();
		g_bLogOpened = __false;
	}
}

void open_log()
{
	flock.Lock();
	if(logIsEnabled){
		utils_puts=::utils_puts_with_log;
		logpointer = logbuffer;
		loglength = 0;
		g_bLogOpened = __true;
	}
	flock.UnLock();
}
