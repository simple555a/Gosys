#include "precomp.h"
#pragma hdrstop

/*
	hash the time value into an integer
*/
LIBHISPROXY_API hislog_key_t PMC_API hislogKeyByTime(const FILETIME * tm)
{
	hislog_key_t ret;
	
	assert(sizeof(ret) == sizeof(*tm));
	memcpy(&ret, tm, sizeof(ret));
	
	/*
		FILETIME is in term of 100ns
		we round it up to multiple of 1 second.
	*/
	//2 Note this must be an integer division
	
	ret /= 10000000;
	
	return ret;
}

LIBHISPROXY_API hislog_key_t PMC_API hislogGetCurrentTime()
{
	FILETIME now;
	SYSTEMTIME st;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &now);
	return hislogKeyByTime(&now);
}

LIBHISPROXY_API FILETIME PMC_API hislogTimeByKey(hislog_key_t t)
{
	FILETIME ft;
	*((__i64*)&ft) = __i64(t * 1e7);
	return ft;
}


