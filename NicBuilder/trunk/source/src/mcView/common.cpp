#include "stdafx.h"
#pragma hdrstop
#include "common.h"

extern CString fake_TagName;
extern CString m_sCurUser;

CString timeAsString(hislog_key_t t)
{
	FILETIME ft;
	SYSTEMTIME stm;
	CString str;
	
	*((__i64*)&ft) = __i64(t * 1e7);
	FileTimeToSystemTime(&ft, &stm);
	str.Format(
		"%04d-%02d-%02d\n  %02d:%02d:%02d ",
		stm.wYear,
		stm.wMonth,
		stm.wDay,
		stm.wHour,
		stm.wMinute,
		stm.wSecond
		);
	return str;
}

FILETIME timeAsFileTime(hislog_key_t t)
{
	FILETIME ft;
	*((__i64*)&ft) = __i64(t * 1e7);
	return ft;
}

void writeAnalog(const char * name, __r4 v)
{
	PRTK_TAG p;
	pmc_value_t fltVal, val;
	TAG_NAME tn;
	tagname_by_text(&tn, name);
	if(!lock_rtdb(__false, 1000)){
		return;
	}
	p = query_tag_f(&tn);
	memset(&fltVal, 0, sizeof(fltVal));
	memset(&val, 0, sizeof(val));
	set_value_type(fltVal.Flags, dt_real4);
	fltVal.Value.flt = v;
	val.Flags = p->s.Flags;
	pmc_type_cast(&fltVal, &val);
	proxy_write_tags(1, &tn, &val.Value);
	unlock_rtdb();
}

//mox20141015记录模拟量置值的操作记录
void writeAnalog_x(const char * name, __r4 v)
{
	PRTK_TAG p;
	pmc_value_t fltVal, val;
	TAG_NAME tn;
	tagname_by_text(&tn, name);
	if(!lock_rtdb(__false, 1000)){
		return;
	}
	p = query_tag_f(&tn);
	memset(&fltVal, 0, sizeof(fltVal));
	memset(&val, 0, sizeof(val));
	set_value_type(fltVal.Flags, dt_real4);
	fltVal.Value.flt = v;
	val.Flags = p->s.Flags;
	pmc_type_cast(&fltVal, &val);
	//proxy_write_tags(1, &tn, &val.Value);
	proxy_write_tags_z(1, &tn, &val.Value, m_sCurUser.GetBuffer());//此处将"用户名"传入
	unlock_rtdb();
}

__r4 getFloat(const pmc_value_t & t)
{
	pmc_value_t o;
	set_value_type(o.Flags, dt_real4);
	pmc_type_cast(&t, &o);
	return o.Value.flt;
}
		

