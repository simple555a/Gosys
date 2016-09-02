/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\rtk\configman.h
	file path:	e:\vss\pmc2\src\include\rtk
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Distributed configuration manager	
	history:
	created:	11/11/2002   14:28
*********************************************************************/
#ifndef __configman_h__
#define __configman_h__

#include <librtdb.h>

#ifdef _WIN32
#ifdef LIBCONFIGMAN_EXPORTS
	#define CONFIGMAN_API _declspec(dllexport)
#else
	#define CONFIGMAN_API _declspec(dllimport)
#endif
#else
#define CONFIGMAN_API
#endif

CDECL_BEGIN

CONFIGMAN_API __bool PMC_API get_config_string(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	char * buf,
	__uint length,
	__uint timeout
	);

CONFIGMAN_API __bool PMC_API set_config_string(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	const char * buf,
	__uint timeout
	);

CONFIGMAN_API __bool PMC_API get_config_string_ex(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	char * buf,
	__uint length,
	__uint timeout
	);

CONFIGMAN_API __bool PMC_API set_config_string_ex(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	const char * buf,
	__uint timeout
	);

CONFIGMAN_API __bool PMC_API get_config_int(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	__int *buf,
	__uint timeout
	);

CONFIGMAN_API __bool PMC_API set_config_int(
	PCNODE_KEY node,
	const char * section,
	const char * key,
	__int value,
	__uint timeout
	);

CDECL_END

#endif
