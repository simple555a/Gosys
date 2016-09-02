#include "precomp.h"

// by default, archive writer is invoked every 30 minutes
// to debug, change to a smaller value
__u32 hislogArchiveTimeSpan = 30;

/* hislogRecordColors must be recipocal of hislogRecordInterval */
__u32 hislogRecordColors = 10;
__u32 hislogRecordInterval = 100;
__u32 hislogAutoSaveInterval = 6;
__u32 hislogMaxArchiveSize = 512*1024;
__uint hislogNumQueryThreads = 1;
__uint hislogDiskWaterLevel = 1024;
__uint hislogDay = 31;
__u32 hislogMaxActiveArchive = 8;
__uint activeArchives;

__bool g_bNoArchive = __false;
__bool g_bNoBuffer = __false;

/*
	hash the time value into an integer
*/
hislog_key_t getKeyByTime(FILETIME * tm)
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

FILETIME getTimeByKey(hislog_key_t t)
{
	FILETIME ft;
	*((__i64*)&ft) = __i64(t * 1e7);
	return ft;
}

hislog_key_t hislogGetCurrentTime()
{
	FILETIME now;
	SYSTEMTIME st;
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &now);
	return getKeyByTime(&now);
}

string timeAsString(hislog_key_t t)
{
	FILETIME ft;
	SYSTEMTIME stm;
	string str;
	char buf[128];
	
	*((__i64*)&ft) = __i64(t * 1e7);
	FileTimeToSystemTime(&ft, &stm);
	sprintf(
		buf,
		"%04d-%02d-%02d %02d:%02d:%02d ",
		stm.wYear,
		stm.wMonth,
		stm.wDay,
		stm.wHour,
		stm.wMinute,
		stm.wSecond
		);
	str = buf;
	return str;
}

void interpolate(
	const hislog_item_t * v1, 
	const hislog_item_t * v2, 
	hislog_item_t * v, 
	const hislog_key_t t1,
	const hislog_key_t t2,
	const hislog_key_t t
	)
{
	*v = *v1;
	return;

	/*
	__r8 alpha;

	alpha = (__r8)(t2 - t)/(t2 - t1);
	
	if(!(v1->Flags & TF_Valid) || 
		!(v2->Flags & TF_Valid) || 
		get_value_type(v1->Flags) == dt_bool
	){
		if(alpha > 0.5){
			*v = *v1;
			v->Flags = v1->Flags;
		}else{
			*v = *v2;
			v->Flags = v2->Flags;
		}
		return;
	}

	v->Flags = v1->Flags;
	
	switch(get_value_type(v1->Flags)){
	case dt_int8:
	case dt_int16:
	case dt_int32:
		v->Value.i32 = alpha * v1->Value.i32
			+ (1 - alpha) * v2->Value.i32;
		break;
		
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		v->Value.u32 = alpha * v1->Value.u32
			+ (1 - alpha) * v2->Value.u32;
		break;
		
	case dt_real4:
		v->Value.flt = alpha * v1->Value.flt 
			+ (1 - alpha) * v2->Value.flt;
		break;
		
	case dt_real8:
	case dt_date:
		v->Value.dbl = alpha * v1->Value.dbl 
			+ (1 - alpha) * v2->Value.dbl;
		break;
		
	//case dt_bool:
	//	v->Value.b = v1->Value.b;
	//	break;

	default:
		if(alpha > 0.5){
			*v = *v1;
		}else{
			*v = *v2;
			v->Flags = v2->Flags;
		}
		break;
	}
	*/
}

string valueAsString(hislog_item_t * v)
{
	char buf[64];
	if(v->Flags & TF_Valid){
		switch(get_value_type(v->Flags)){
		case dt_real4:
			sprintf(buf, "%f", v->Value.fltValue);
			break;
		case dt_real8:
			sprintf(buf, "%f", v->Value.dblValue);
			break;
		case dt_int32:
			sprintf(buf, "%d", v->Value.iValue);
			break;
		case dt_bool:
			sprintf(buf, "%s", v->Value.b? "On" : "Off");
			break;			
		default:
			sprintf(buf, "%I64d", v->Value.i64Value);
			break;
		}
	}else{
		sprintf(buf, "N.A.");
	}
	return string(buf);
}

