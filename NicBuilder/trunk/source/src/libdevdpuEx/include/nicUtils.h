

#ifndef _NICSYS_UTILS_H__
#define _NICSYS_UTILS_H__


#include "nicPrecomp.h"

#ifdef _WIN32
#ifdef	LIB_UTILS_EXPORTS
#define UTILS_API	  __declspec(dllexport)
#else
#define UTILS_API	  __declspec(dllimport)
#endif
#else
#define UTILS_API
#endif




#ifndef _WIN_HAVE_STRUPR
extern char * strupr(char * str);
#endif

#ifndef _WIN_HAVE_PROFILE_SUPPORT
// add profile support to Non-Win32 platform
#include <Profile.h>

#define GetPrivateProfileString(section,key,_default,buf,len,file)\
	read_string_key(section,key,_default,buf,len,file)
#define GetPrivateProfileInt(section,key,_default,file)\
	read_int_key(section,key,_default,file)
#define WritePrivateProfileString(section,key,buf,file)\
	write_string_key(section,key,buf,file)
#define WritePrivateProfileInt(section,key,_default,file)\
	write_int_key(section,key,_default,file)
#endif


#ifndef _WIN_HAVE_SNPRINTF
#define _snprintf snprintf
#define _vsnprintf vsnprintf
#endif

#ifdef __GNUC__
#define strcmpi  strcasecmp
#define strnicmp strncasecmp
#endif

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#define __CONSTRUCTOR__ __attribute__((constructor))
#define __DESTRUCTOR__ __attribute__((destructor))
#define __FASTCALL__(x)	x __attribute__((regparm(3)))
#define __STDCALL__(x) x __attribute__((stdcall))
#else
#define PACKED
#define __CONSTRUCTOR__
#define __DESTRUCTOR__
#define __FASTCALL__(x) __fastcall x
#define __STDCALL__(x) __stdcall x
#endif




#ifndef NDEBUG
#define DEBUG_PRINTF(__x__) debug_printf __x__
#define DEBUG_PRINTFW(__x__) debug_printfW __x__
#define DEBUG_PRINTFA(__x__) debug_printfA __x__
#else
#define DEBUG_PRINTF(__x__)
#define DEBUG_PRINTFW(__x__)
#define DEBUG_PRINTFA(__x__)
#endif

#ifdef _WIN32
typedef int socklen_t;
#endif

#ifndef _WIN32
#define ZeroMemory(chunk, size) memset(chunk, 0, size)
#endif

 

#endif
