/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\rtkdefs.h
	file path:	e:\vss\pmc2\src\include
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Generic typedefs and classes used by the rtk library
	history:
	created:	10:9:2002   11:39
*********************************************************************/
#ifndef __rtkdefs_h__
#define __rtkdefs_h__

#include <cpu_features.h>

#define __true	1
#define __false	0

#ifdef __cplusplus
	#define CDECL_BEGIN	extern "C" {
	#define CDECL_END	}
	#define DEFAULT_PARAM(val)	= val
#else
	#define CDECL_BEGIN
	#define CDECL_END
	#define DEFAULT_PARAM(val)
#endif

#ifdef _WIN32
	#include <tchar.h>
#else
#endif

#endif // __rtkdefs_h__
