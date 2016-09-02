/* 改动历史
 * Date      Version  Modified by  Modify
 * 2012-7-9  1.0.0.4  Zander Sun   1.添加cacheSetMode()
 */
#include "precomp.h"

#define _prolog() CArchiveCache * c = (CArchiveCache*)cache

LIBHISPROXY_API RTK_HANDLE PMC_API createArchiveCache()
{

	return new CArchiveCache();
}

LIBHISPROXY_API void PMC_API cacheSetTimeSpan(
	RTK_HANDLE cache,
	hislog_key_t t1, 
	__uint step, 
	__uint count
	)
{
	_prolog();
	c->setTimeSpan(t1, step, count);
}

LIBHISPROXY_API void PMC_API cacheScrollTimeSpan(
	RTK_HANDLE cache,
	hislog_key_t relativeTime
	)
{
	_prolog();
	c->scrollTimeSpan(relativeTime);
}

LIBHISPROXY_API void PMC_API cacheSetNames(
	RTK_HANDLE cache,
	PCTAG_NAME names, 
	__uint count
	)
{
	_prolog();
	c->setNames(names, count);
}

LIBHISPROXY_API __bool PMC_API cacheQuery(
	RTK_HANDLE cache,
	__uint timeout,
	archive_cache_info_t * info
	)
{
	__bool ret;
	_prolog();
	ret = c->query(timeout);
	if(info){
		cacheGetInfo(cache, info);
	}
	return ret;
}

LIBHISPROXY_API __bool PMC_API cacheGetFlag(
	RTK_HANDLE cache,
	__uint tagId
	)
{
	_prolog();
	return c->cacheFlag(tagId);
}

LIBHISPROXY_API const hislog_item_t * PMC_API cacheGetValue(
	RTK_HANDLE cache,
	__uint tagId,
	__uint valueId
	)
{
	_prolog();
	return c->cacheValue(tagId, valueId);
}

LIBHISPROXY_API hislog_key_t PMC_API cacheGetStartTime(RTK_HANDLE cache)
{
	_prolog();
	return c->startTime();
}

LIBHISPROXY_API hislog_key_t PMC_API cacheGetEndTime(RTK_HANDLE cache)
{
	_prolog();
	return c->endTime();
}

LIBHISPROXY_API __uint PMC_API cacheGetStep(RTK_HANDLE cache)
{
	_prolog();
	return c->step();
}

LIBHISPROXY_API __uint PMC_API cacheGetValueCount(RTK_HANDLE cache)
{
	_prolog();
	return c->valueCount();
}

LIBHISPROXY_API __uint PMC_API cacheGetReturnedValueCount(
	RTK_HANDLE cache, 
	int idx
	)
{
	_prolog();
	return c->returnedValueCount(idx);
}

LIBHISPROXY_API __uint PMC_API cacheGetTagCount(RTK_HANDLE cache)
{
	_prolog();
	return c->tagCount();
}

LIBHISPROXY_API hislog_key_t PMC_API cacheGetQueryTime(RTK_HANDLE cache)
{
	_prolog();
	return c->queryTime();
}

LIBHISPROXY_API void PMC_API cacheGetInfo(
	RTK_HANDLE cache,
	archive_cache_info_t * info
	)
{
	_prolog();
	
	info->tagCount = c->tagCount();
	info->valueCount = c->valueCount();
	info->startTime = c->startTime();
	info->step = c->step();
	info->names = c->names();
	info->queryTime = c->queryTime();
}

LIBHISPROXY_API __bool PMC_API cacheSetInfo(
	RTK_HANDLE cache,
	const archive_cache_info_t * info
	)
{
	_prolog();
	
	if(!c->setNames(info->names, info->tagCount)){
		return __false;
	}

	if(!c->setTimeSpan(info->startTime, info->step, info->valueCount)){
		return __false;
	}
	
	return __true;
}

//2012-7-9
LIBHISPROXY_API void PMC_API cacheSetMode(RTK_HANDLE cache, __int queryMode)
{
	_prolog();
	c->setMode(queryMode);	
}


