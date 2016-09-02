/*
 * 改动历史
 * Date      Version  Modifed by  Modify
 * 2012-7-9  1.0.0.4  Zander Sun  1.由于数据报包头hisqry_pkt_t的修改，修改了queryHisData()的数据报报头的
 *                                   赋值过程--queryHisData()
 */
#include "precomp.h"

static CArchiveCache * cache;

LIBHISPROXY_API void PMC_API initHisProxy()
{
	init_network(RTK_SKIP_STANDARD_BUSES, 0);
	initArchiveCache();
	cache = new CArchiveCache;
}

LIBHISPROXY_API void PMC_API uninitHisProxy()
{
	if(cache){
		delete cache;
		cache = 0;
	}
	uninitArchiveCache();
	uninit_network();
}

//2012-7-9
// LIBHISPROXY_API void PMC_API queryHisData(
// 	const TAG_NAME * tag, 
// 	const FILETIME *startTime, 
// 	long interval, 
// 	long length
// 	)
// {
// 	
// 	if(!cache){
// 		return;
// 	}
// 	cache->setNames(tag, 1);
// 	hislog_key_t st;
// 	st = hislogKeyByTime(startTime);
// 	cache->setTimeSpan(st, interval, length);
// 	cache->query(1000);
// }

//2012-7-9
LIBHISPROXY_API void PMC_API queryHisData(
										  __u32 mode,
										  const TAG_NAME * tag, 
										  const FILETIME *Pftimes, 
										  __u32 tagCount,
										  long valueCount
										  )
{
	if(!cache){
		return;
	}
	cache->setMode(mode);
	cache->setNames(tag, tagCount);
	hislog_key_t st,et;
	st = hislogKeyByTime(&Pftimes[0]);
	et = hislogKeyByTime(&Pftimes[1]);
	__uint step = (__uint)((et-st)/(hislog_key_t)valueCount);
	cache->setTimeSpan(st, step, valueCount);
	cache->query(1000);
}

LIBHISPROXY_API __bool PMC_API getHisData(hislog_item_t * value)
{
	if(!cache->cacheFlag(0)){
		value->Flags &= ~TF_Valid;
		return __false;
	}
	memcpy(
		value, 
		cache->cacheValue(0, 0), 
		cache->returnedValueCount(0) * sizeof(hislog_item_t)
		);
	return __true;
}
