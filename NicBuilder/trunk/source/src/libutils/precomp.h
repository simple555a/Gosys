#ifndef __precomp_h__
#define __precomp_h__

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __GNUC__
#include <fcntl.h>
#endif

#if defined(_WIN32)
#include <fcntl.h>
#include <io.h>
#include <shlobj.h>
#endif

#ifdef _WIN32
extern DWORD	g_dwObsfucator;
extern HMODULE	g_hModule;
#else
#define _vsnprintf vsnprintf
#endif

#include <malloc.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

#include <utils.h>

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#define BOOL	char
#endif

#endif
