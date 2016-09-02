/*
  PMC source file
  Version 2.03
  Jackie Hunter, jhunter@tsinghua.org.cn

  This file makes PMC source sustain the disparency between
  different compilers and C libraries. Currently pc-win32-vc and
  pc-linux-gcc are tested platforms, and hopefully other POSIX-
  conforming OSes will also be OK.  

  Note : most of the macros prefixed by HAVE_ ... come from
  pmc_config.h.
  
  History :
  Created 02/20/2003, Jackie
 */
#ifndef __rtk_compat_h__
#define __rtk_compat_h__

#include <rtk_config.h>
#include <rtkdefs.h>

CDECL_BEGIN

#ifndef HAVE_STRUPR
extern char * strupr(char * str);
#endif

#ifndef HAVE_PROFILE_SUPPORT
// add profile support to Non-Win32 platform
#include <profile.h>
#define GetPrivateProfileString(section,key,_default,buf,len,file)\
	read_string_key(section,key,_default,buf,len,file)
#define GetPrivateProfileInt(section,key,_default,file)\
	read_int_key(section,key,_default,file)
#define WritePrivateProfileString(section,key,buf,file)\
	write_string_key(section,key,buf,file)
#define WritePrivateProfileInt(section,key,_default,file)\
	write_int_key(section,key,_default,file)
#endif

#ifndef HAVE_SNPRINTF
//#define _snprintf snprintf
#define _vsnprintf vsnprintf
#endif

#ifdef __GNUC__
#define strcmpi  strcasecmp
#define strnicmp strncasecmp
#endif

#ifndef CDECL
#define CDECL
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

#ifdef _WIN32
typedef int socklen_t;
#endif

CDECL_END

#endif

