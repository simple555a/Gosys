/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\include\pmc.h
	file path:	E:\vss\pmc2\src\include
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Master include file for PMC 2.0	
	history:
	created:	10:9:2002   14:18
*********************************************************************/
#ifndef __pmc_h__
#define __pmc_h__

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include <pmc_config.h>
#include <utils.h>
#include <librtk.h>
#include <librtdb.h>

#include <rtk_thread.h>
#include <ddk.h>

#if !defined(PMC_NO_LIBS) && 0
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libpowermgr.lib")
#pragma comment(lib, "librtkproxy.lib")
#endif

#include <additional_headers.h>

#define _define_property(type, name)\
	type name;\
	STDMETHOD(get_##name)(type * val)\
{\
	*val = this->name;\
	return S_OK;\
}\
	STDMETHOD(put_##name)(type newVal)\
{\
	this->name = newVal;\
	return S_OK;\
}

#endif // __pmc_h__
