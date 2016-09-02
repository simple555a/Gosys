#include "precomp.h"
#pragma hdrstop
#include <stdarg.h>
#include <stdio.h>

#ifdef __LINUX__
#define _vsnprintf vsnprintf
#endif

int kd_print(const char * fmt, ...)
{
#ifdef __VXWORKS__
/* buffer overflow possible */
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	return printf("%s", buf);
#else
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	return printf("%s", buf);
#endif
}

