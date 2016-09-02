/********************************************************************
	created:	2001/10/23
	created:	23:10:2001	 0:05
	filename:	E:\eguard\libutils\utils.cpp
	file path:	E:\eguard\libutils
	file base:	utils
	file ext:	cpp
	author: 	Jackie Hunter
	
	purpose:	常用函数库
*********************************************************************/
#pragma warning(disable:4996)
#include "precomp.h"
#pragma hdrstop 

#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __BORLANDC__
#pragma warn -asc
#pragma warn -asm
#endif

#ifndef __BORLANDC__
static void randomize()
{
	UTILS_TIME t;
	time_mark(&t);
	srand(t.lo);
}
#endif

static int builtin_utils_traceA(char * s,...);
static int builtin_utils_debugA(char * s,...);
static int builtin_utils_errorA(char * s,...);
static int UTILS_API builtin_utils_putsA(MSG_LEVEL level,char *);
static char * UTILS_API builtin_utils_getsA(char *,int max_len);

static int UTILS_API builtin_utils_log_putsA(char *,int leve);
static int builtin_utils_logA(char *,...);

#ifdef _WIN32
static int builtin_utils_traceW(wchar_t * s,...);
static int builtin_utils_debugW(wchar_t * s,...);
static int builtin_utils_errorW(wchar_t * s,...);
static int UTILS_API builtin_utils_putsW(MSG_LEVEL level,wchar_t *);
static wchar_t * UTILS_API builtin_utils_getsW(wchar_t *,int max_len);

static int UTILS_API builtin_utils_log_putsW(wchar_t *,int leve);
static int builtin_utils_logW(wchar_t *,...);
#endif

static int UTILS_API builtin_utils_getc(void);

LIBUTILS_API double cpu_speed;//cpu_calibrate();   //MHz为单
LIBUTILS_API int debug_level=dbg_level_verbose;

LIBUTILS_API int (* utils_debugA)(char * ,...)=builtin_utils_debugA;
LIBUTILS_API int (* utils_traceA)(char *, ...)=builtin_utils_traceA;
LIBUTILS_API int (* utils_errorA)(char *, ...)=builtin_utils_errorA;
LIBUTILS_API int (UTILS_API *utils_log_putsA)(char *s,int level)=builtin_utils_log_putsA;
LIBUTILS_API int (UTILS_API * utils_putsA)(MSG_LEVEL level,char *)=builtin_utils_putsA;
LIBUTILS_API int (*utils_logA)(char *,...)=builtin_utils_logA;
LIBUTILS_API char * (UTILS_API *utils_getsA)(char *,int max_len)=builtin_utils_getsA;

#ifdef _WIN32
LIBUTILS_API int (* utils_debugW)(wchar_t *,...)=builtin_utils_debugW;
LIBUTILS_API int (* utils_traceW)(wchar_t *,...)=builtin_utils_traceW;
LIBUTILS_API int (* utils_errorW)(wchar_t *,...)=builtin_utils_errorW;
LIBUTILS_API int (UTILS_API *utils_log_putsW)(wchar_t *s,int level)=builtin_utils_log_putsW;
LIBUTILS_API int (UTILS_API * utils_putsW)(MSG_LEVEL level,wchar_t *)=builtin_utils_putsW;
LIBUTILS_API int (*utils_logW)(wchar_t *,...)=builtin_utils_logW;
LIBUTILS_API wchar_t * (UTILS_API *utils_getsW)(wchar_t *,int max_len)=builtin_utils_getsW;
#endif

LIBUTILS_API int (UTILS_API *utils_getc)(void)=builtin_utils_getc;

FILE *fp_trace;
FILE *fp_error;
FILE *fp_debug;

static double pp()
{
	static char inited;  
	static double ppp;
	if(!inited){
		ppp = pow(2,32);
		inited = 1;
	}	
	return ppp;
}

#ifdef _WIN32
LIBUTILS_API double UTILS_API cpu_calibrate(void)
{
	UTILS_TIME t1,t2;
	LARGE_INTEGER ts,te,f;
	double dt;

	DEBUG_PRINTF(("#CPU calibrating..."));

	QueryPerformanceFrequency(&f);

	QueryPerformanceCounter(&ts);
	time_mark(&t1);

	Sleep(200);

	QueryPerformanceCounter(&te);
	time_mark(&t2);

	t2.hi -= t1.hi;
	if(t2.lo < t1.lo){
		t2.hi--;
	}
	t2.lo-=t1.lo;

	if(f.QuadPart){
		dt=(double)( (__int64)(te.QuadPart) - (__int64)(ts.QuadPart))/
			(__int64)(f.QuadPart);
		cpu_speed=(t2.hi*pp()+t2.lo)/1e6/dt;
		DEBUG_PRINTF(("%.2f MHz calibrated.\n",cpu_speed));
	}else{
		DEBUG_PRINTF(("Unable to determine CPU clock, possibly running on LAPTOP, 100MHz assumed.\n"));
		cpu_speed=100;
	}
	
	return cpu_speed;
}
#elif defined(__GNUC__)
LIBUTILS_API double UTILS_API cpu_calibrate(void)
{
	UTILS_TIME t1,t2;
	double dt;

	DEBUG_PRINTF(("#CPU calibrating..."));

	time_mark(&t1);
	sleep(1);
	time_mark(&t2);

	t2.hi -= t1.hi;
	if(t2.lo < t1.lo){
		t2.hi--;
	}
	t2.lo-=t1.lo;

	dt=1;
	cpu_speed = (t2.hi* pp() + t2.lo) / 1e6 / dt;
	DEBUG_PRINTF(("%.2f MHz calibrated.\n",cpu_speed));
	return cpu_speed;
}
#else
#error "Un-expected platform."
#endif

LIBUTILS_API int UTILS_API rng_rand(int min,int max)
{
	static char randomized=0;
	if(!randomized){
		randomize();
		randomized=1;
	}
	return min + (int)( (double)(max-min)*rand() / RAND_MAX );
}

LIBUTILS_API void UTILS_API set_error_stream(FILE * s)
{
	fp_error=s;
}

LIBUTILS_API void UTILS_API set_trace_stream(FILE * s)
{
	fp_trace=s;
}

LIBUTILS_API double UTILS_API time_mark_d(void)
{
	unsigned a=5,d;
#ifdef _WIN32
	__asm{
		rdtsc
			mov a,eax
			mov d,edx
	}
#elif defined(__GNUC__)
	__asm__ __volatile__(
		"\trdtsc\n"
		"\tmov %%eax, %0\n"
		"\tmov %%edx, %1\n"
		:"=g"(a), "=g"(d)
		:
	:"%eax", "%edx"
		);
#else
#error "Un-expected platform."
#endif
	return d*pp()+a;
}

LIBUTILS_API void UTILS_API set_debug_stream(FILE * s)
{
	fp_debug=s;
}

LIBUTILS_API  void	 UTILS_API time_mark(UTILS_TIME * t)
{		
#ifdef _WIN32
	__asm{
		mov ebx,[t]
		rdtsc		 
			mov [ebx],eax
			add ebx,4
			mov [ebx],edx
	}
#elif defined(__GNUC__)
	__asm__ __volatile__( 
		"\trdtsc\n" 
		"\tmovl %%eax, %0\n" 
		"\tmovl %%edx, %1\n"
		:"=g"(t->lo), "=g"(t->hi)
		:
	:"%eax", "%edx"
		); 
#else
#error "Un-expected platform."
#endif
}

LIBUTILS_API  double UTILS_API time_diff(const UTILS_TIME * _t1, const UTILS_TIME * t2)
{
	UTILS_TIME t1=*_t1;
	t1.hi -= t2->hi;
	if( (unsigned)t1.lo < (unsigned)t2->lo )
		t1.hi--;
	t1.lo-=t2->lo;
	return (t1.hi * pp() + t1.lo)/cpu_speed/1e6;
}

static int builtin_utils_traceA(char * s,...)
{
	char ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsA(msg_trace,ss);
};

static int builtin_utils_errorA(char * s,...)
{
	char ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsA(msg_error,ss);
}

static int builtin_utils_debugA(char * s,...)
{
	char ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsA(msg_debug,ss);
};

#ifdef _WIN32
LIBUTILS_API FILE * UTILS_API utils_fopenA (char * file_name,char * mode)
{
	return fopen(file_name,mode);
}

LIBUTILS_API int UTILS_API utils_fclose(FILE *fp)
{
	fflush(fp);
	return fclose(fp);
}
#endif

static int UTILS_API builtin_utils_putsA(MSG_LEVEL level,char * s)
{	
	switch(level){
	case msg_debug:
		// Found bug here,
		// should not use .printf routines to output the string
		// because printf series function treat the input string
		// in its own way, which might cause problem when s contains
		// such characters as '%', '\'
		// should use fputs instead.
		// by J.Hunter,
		// 05-3-2001
		//if(fp_debug) res=fprintf(fp_debug,s);
		if(fp_debug) fputs(s,fp_debug);
		break;
	case msg_error:
		// if(fp_error) res=fprintf(fp_error,s);
		if(fp_error) fputs(s,fp_error);
		break;
	case msg_trace:
		// if(fp_trace) res=fprintf(fp_trace,s);
		if(fp_trace) fputs(s,fp_trace);
		break;
	default:
		break;
	}
	// return res;
	return strlen(s);
}

static char * UTILS_API builtin_utils_getsA(char * s,int max_len)
{
	return fgets(s,max_len,stdin);
}

static int UTILS_API builtin_utils_getc(void)
{
	return fgetc(stdin);
}

static int builtin_utils_logA(char * s,...)
{
	char ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_log_putsA(ss,debug_level);
}

static int UTILS_API builtin_utils_log_putsA(char * s,int level)
{
	int res=fputs(s,stdout);
	return res;
}

#ifdef _WIN32
static int builtin_utils_traceW(wchar_t * s,...)
{
	wchar_t ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnwprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsW(msg_trace,ss);
};

static int builtin_utils_errorW(wchar_t * s,...)
{
	wchar_t ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnwprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsW(msg_error,ss);
}

static int builtin_utils_debugW(wchar_t * s,...)
{
	wchar_t ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnwprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_putsW(msg_debug,ss);
}

LIBUTILS_API FILE * UTILS_API utils_fopenW(wchar_t * file_name,wchar_t * mode)
{
	return _wfopen(file_name,mode);
}

static int UTILS_API builtin_utils_putsW(MSG_LEVEL level, wchar_t * s)
{	
	switch(level){
	case msg_debug:
		// Found bug here,
		// should not use .printf routines to output the string
		// because printf series function treat the input string
		// in its own way, which might cause problem when s contains
		// such characters as '%', '\'
		// should use fputs instead.
		// by J.Hunter,
		// 05-3-2001
		//if(fp_debug) res=fprintf(fp_debug,s);
		if(fp_debug) fputws(s,fp_debug);
		break;
	case msg_error:
		// if(fp_error) res=fprintf(fp_error,s);
		if(fp_error) fputws(s,fp_error);
		break;
	case msg_trace:
		// if(fp_trace) res=fprintf(fp_trace,s);
		if(fp_trace) fputws(s,fp_trace);
		break;
	default:
		break;
	}
	// return res;
	return wcslen(s);
}

static wchar_t * UTILS_API builtin_utils_getsW(wchar_t * s,int max_len)
{
	return fgetws(s,max_len,stdin);
}

static int builtin_utils_logW(wchar_t * s,...)
{
	wchar_t ss[MAX_LINE_LENGTH];
	va_list arg;
	va_start(arg,s);
	_vsnwprintf(ss, sizeof(ss), s, arg);
	va_end(arg);
	return utils_log_putsW(ss,debug_level);
}

static int UTILS_API builtin_utils_log_putsW(wchar_t * s,int level)
{
	int res=fputws(s,stdout);
	return res;
}
#endif

