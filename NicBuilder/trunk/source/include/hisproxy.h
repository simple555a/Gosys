/*
	access wrapper layer to the archive subsystem.

	created:
	- jhunter@tsinghua.org.cn, 2004/8/9
*/
/*
改动记录：
Date      Version  Author      Modify
2012-7-9  1.0.0.4  Zander Sun  1.queryHisData()的形参修改，用于libhisdATL.dll调用
                               2.添加cacheSetMode()
*/
#ifndef __hisproxy_h_
#define __hisproxy_h_

#include <pmc_hislog.h>
#include <librtk.h>
#include <rtk_object.h>

#ifdef _WIN32
	#ifdef LIBHISPROXY_EXPORTS
		#define LIBHISPROXY_API      __declspec(dllexport)
	#else
		#define LIBHISPROXY_API      __declspec(dllimport)
	#pragma comment (lib, "libhisproxy.lib")
	#endif
#else
	#define LIBHISPROXY_API       
#endif

#include <pshpack8.h>
typedef struct __archive_cache_info_t{
	__uint tagCount;
	__uint valueCount;
	__uint step;
	PCTAG_NAME names;
	hislog_key_t startTime;
	hislog_key_t queryTime;
}archive_cache_info_t;
#include <poppack.h>

#ifdef __cplusplus
extern "C"{
#endif
	LIBHISPROXY_API void PMC_API initHisProxy();
	LIBHISPROXY_API void PMC_API uninitHisProxy();

//2012-7-9
//old
// 	LIBHISPROXY_API void PMC_API queryHisData(
// 		const TAG_NAME *tag, 
// 		const FILETIME *startTime, 
// 		long interval, 
// 		long length
// 		);

//用于COM口libhisdATL.dll调用
	LIBHISPROXY_API void PMC_API queryHisData(
		__u32 mode,
		const TAG_NAME * tag, 
		const FILETIME *Pftimes, 
		__u32 tagCount,
		long valueCount
		);
	LIBHISPROXY_API __bool PMC_API getHisData(hislog_item_t  * value);

	/*
		hash the time value into an integer
	*/
	LIBHISPROXY_API hislog_key_t PMC_API hislogKeyByTime(const FILETIME * tm);
	LIBHISPROXY_API FILETIME PMC_API hislogTimeByKey(hislog_key_t t);
	LIBHISPROXY_API hislog_key_t PMC_API hislogGetCurrentTime();

	LIBHISPROXY_API RTK_HANDLE PMC_API createArchiveCache();

	LIBHISPROXY_API void PMC_API cacheSetTimeSpan(
		RTK_HANDLE cache,
		hislog_key_t t1, 
		__uint step, 
		__uint count
		);

	LIBHISPROXY_API void PMC_API cacheScrollTimeSpan(
		RTK_HANDLE cache,
		hislog_key_t relativeTime
		);

	LIBHISPROXY_API void PMC_API cacheSetNames(
		RTK_HANDLE cache,
		PCTAG_NAME names, 
		__uint count
		);

	LIBHISPROXY_API __bool PMC_API cacheQuery(
		RTK_HANDLE cache,
		__uint timeout,
		archive_cache_info_t * info
		);

	LIBHISPROXY_API __bool PMC_API cacheGetFlag(
		RTK_HANDLE cache,
		__uint tagId
		);

	LIBHISPROXY_API const hislog_item_t * PMC_API cacheGetValue(
		RTK_HANDLE cache,
		__uint tagId,
		__uint valueId
		);

	LIBHISPROXY_API hislog_key_t PMC_API cacheGetStartTime(
		RTK_HANDLE cache
		);
	LIBHISPROXY_API hislog_key_t PMC_API cacheGetEndTime(
		RTK_HANDLE cache
		);
	LIBHISPROXY_API __uint PMC_API cacheGetStep(
		RTK_HANDLE cache
		);
	LIBHISPROXY_API __uint PMC_API cacheGetValueCount(
		RTK_HANDLE cache
		);
	LIBHISPROXY_API __uint PMC_API cacheGetReturnedValueCount(
		RTK_HANDLE cache, 
		int idx
		);
	LIBHISPROXY_API __uint PMC_API cacheGetTagCount(
		RTK_HANDLE cache
		);

	LIBHISPROXY_API hislog_key_t PMC_API cacheGetQueryTime(
		RTK_HANDLE cache
		);

	LIBHISPROXY_API void PMC_API cacheGetInfo(
		RTK_HANDLE cache,
		archive_cache_info_t * info
		);

	LIBHISPROXY_API __bool PMC_API cacheSetInfo(
		RTK_HANDLE cache,
		const archive_cache_info_t * info
		);
	//2012-7-9
	LIBHISPROXY_API void PMC_API cacheSetMode(RTK_HANDLE cache, __int queryMode);
	
#ifdef __cplusplus
}
#endif

#endif
