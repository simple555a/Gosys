/*
	PMC 2.0 source
	by J.Hunter, jhunter@tsinghua.org.cn, May 6, 2003
	Power state controlling variables and procedures.
*/

#ifndef __powermgr_h__
#define __powermgr_h__

#include <pmc_config.h>

#ifdef _WIN32
#ifdef LIBPOWERMGR_EXPORTS
#define POWERMGR_API __declspec(dllexport)
#else
#define POWERMGR_API __declspec(dllimport)
#endif
#else
#define POWERMGR_API
#endif

#define PWR_STOPPED	0
#define PWR_RUNNING 	1
#define PWR_BACKUP		2

typedef __bool (PMC_API*POWERMGR_CALLBACK)(int newState, int context);

#ifdef __cplusplus
class POWERMGR_API CPowerAware{
public:
	CPowerAware();
	virtual ~CPowerAware();
	virtual __bool OnPowerStateChanged(int newState) = 0;
};
#endif

CDECL_BEGIN

POWERMGR_API int PMC_API 
init_powermgr();

POWERMGR_API __bool PMC_API 
set_power_state(int newstate);

POWERMGR_API int PMC_API 
get_power_state();

POWERMGR_API __bool PMC_API 
register_power_callback(
	POWERMGR_CALLBACK cb, 
	int context
	);

POWERMGR_API __bool PMC_API 
unregister_power_callback(
	POWERMGR_CALLBACK cb, 
	int context
	);

CDECL_END

#endif

