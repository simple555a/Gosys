#pragma warning(disable:4244)

/*
	2005-8-2 
	fixed a bug, previous implementation of pmc_variant_to_value 
	fails on converting VT_BYREF | VT_VARIANT type.
*/
#include "precomp.h"
#pragma hdrstop

#include <math.h>

#define cast(srcType, dstType, s, d)\
	*((dstType*)(d)) = *((srcType*)s)

#define handle_cast(stName, st, dt, s, d)\
	case stName:\
		cast(st, dt, s, d);\
		return __true;
	
#define handle_cast_self(tName, t, s, d)\
	case tName:\
		*((t*)d) = *((t*)s);\
		return __true;

#define handle_bool(st, s, d)\
	case dt_bool:\
		*((__bool*)d) = *((st*)s)? __true : __false;\
		return __true;

#define invalid_cast()\
	default:\
		return __false;

#define src_handler(s)\
	case s:\
		switch(destType){

#define src_handler_end()\
		}

#ifdef _MSC_VER
static const __r8 _pow_2_63 = 9223372036854775808.0;
static inline __r8 u64ToDouble(__u64 u)
{
	if((__i64)u < 0){
		return (__r8)(-(__i64)u) + _pow_2_63;
	}else{
		return (__r8)((__i64)u);
	}
}
#else
#define u64ToDouble(u) ((__r8)(u))
#endif

/*
	de-allocate memory associated with a tag value
	and zeroing it, the tag type is keeped
*/
RTDB_API void PMC_API pmc_clear(Ppmc_value_t v)
{
	if(!(v->Flags & TF_StaticAlloc)){
		switch(get_value_type(v->Flags)){
		case dt_string:
			if(v->Value.str){
				free_buffer(v->Value.str);
			}
			break;
		case dt_bstr:
			if(v->Value.wstr){
				SysFreeString(v->Value.wstr);
			}
			break;
		}
	}
	v->Value.i64 = 0;
}

/*
	nullify a tag value
*/
RTDB_API void PMC_API pmc_empty(Ppmc_value_t v)
{
	pmc_clear(v);
	set_value_type(v->Flags, dt_null);
}

static string timeAsString(__r8 t)
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

/*
函数功能：将实时值转化为字符串
参数说明：
          _s  待转化的实时值结构体
返 回 值：
          转化后得到的实时值字符串
*/
static char * _valueToString(PCpmc_value_t _s)
{
	TAG_TYPE srcType = get_value_type(_s->Flags);
	char buf[64];
	
	switch(srcType){
		//float--db_real4
	case dt_real4:
		sprintf(buf, "%g", _s->Value.flt);
		break;
        //double--db_real8
	case dt_real8:
		sprintf(buf, "%g", _s->Value.dbl);
		break;

	case dt_int8:
		sprintf(buf, "%i", _s->Value.i8);
		break;
	case dt_int16:
		sprintf(buf, "%i", _s->Value.i16);
		break;
	case dt_int32:
		sprintf(buf, "%i", _s->Value.i32);
		break;
		
	case dt_uint8:
		sprintf(buf, "%i", _s->Value.u8);
		break;
	case dt_uint16:
		sprintf(buf, "%i", _s->Value.u16);
		break;
	case dt_uint32:
		sprintf(buf, "%i", _s->Value.u32);
		break;

	case dt_int64:
		sprintf(buf, "%I64d", _s->Value.i64);
		break;
	case dt_uint64:
		sprintf(buf, "%I64d", _s->Value.u64);

	case dt_bool:
		sprintf(buf, "%s", _s->Value.b? "On" : "Off");
		break;

	case dt_date:
		sprintf(buf, "%s", timeAsString(_s->Value.date).c_str());
		break;

	case dt_string:
		return rtk_alloc_string(_s->Value.str);
		
	default:
		sprintf(buf, "NA.", srcType);
		break;
	}

	return (char *)alloc_buffer_v(buf, strlen(buf) + 1, 0);
}

/*
	if the incoming string is invalid, this function might throw an OS
	exception.
*/
static __bool _stringToValue(const char * p, Ppmc_value_t d)
{
	pmc_clear(d);
	switch(get_value_type(d->Flags)){
	case dt_bool:
#define _map_(__s, __v) if(!strcmpi(p, #__s)){d->Value.b = __v;}
		_map_(on, __true)
		else _map_(true, __true)
		else _map_(t, __true)
		else _map_(off, __false)
		else _map_(false, __false)
		else _map_(f, __false)
		else{
			d->Value.b = atoi(p)? __true : __false;
		}
		break;
	case dt_int8:
	case dt_int16:
	case dt_int32:
		// we use the negative logic, since msvc doesn't have the __unlikely() extension
		// as in gcc 2.96+
		if(*p != '0' || !(p[1] == 'x' || p[1] == 'X')){
			d->Value.i32 = atoi(p);
		}else{
			sscanf(p, "%x", &d->Value.i32);
		}
		break;
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		if(*p != '0' || !(p[1] == 'x' || p[1] == 'X')){
			d->Value.u32 = atoi(p);
		}else{
			sscanf(p, "%x", &d->Value.i32);
		}
		break;
	case dt_int64:
	case dt_uint64:
		// BUGBUG: we dont support converting a large unsigned int64 for now
		if(*p != '0' || !(p[1] == 'x' || p[1] == 'X')){
			sscanf(p, "%I64d", &d->Value.i64);
		}else{
			sscanf(p, "%I64x", &d->Value.i64);
		}
		break;
	case dt_real4:
		d->Value.flt = (__r4)atof(p);
		break;
	case dt_real8:
		d->Value.dbl = atof(p);
		break;
	case dt_string:
		d->Value.str = (char *)alloc_buffer_v(p, strlen(p) + 1, 0);
		break;
	case dt_date:
		{
			CArgs args;
			SYSTEMTIME st;
			FILETIME ft;
			parse_arg_ex(p, &args, " -:/");
			if(args.argc == 6){
				ZeroMemory(&st, sizeof(st));
				// a date-time in the form of 'xxxx-xx-xx xx:xx:xx'
				st.wYear = atoi(args.argv[0]);
				st.wMonth = atoi(args.argv[1]);
				st.wDay = atoi(args.argv[2]);
				st.wHour = atoi(args.argv[3]);
				st.wMinute = atoi(args.argv[4]);
				st.wSecond = atoi(args.argv[5]);
				SystemTimeToFileTime(&st, &ft);
				d->Value.dbl = *((__i64*)&ft) / 1e7;
			}else if(args.argc == 3){
				// guess if it's a date or just a time
				st.wYear = atoi(args.argv[0]);
				if(st.wYear > 1000){
				}
				// TODO: source incomplete
				return __false;
			}
		}
		break;
	default:
		return __false;
	}
	return __true;
}

/*
	if the incoming string is invalid, this function might throw an OS
	exception.
*/
static __bool _bstrToValue(const wchar_t * p, Ppmc_value_t d)
{
	char buffer[128];
	WideCharToMultiByte(0, 0, p, -1, buffer, sizeof(buffer), NULL, NULL);
	return _stringToValue(buffer, d);
}

/*
函数功能：实时值结构体赋值，将_s赋给_d
参数说明：
          _s   已知的实时值结构体
		  _d   被_s赋值的目标实时值结构体，指定了数据类型
返 回 值：
          __true   赋值成功
		  __false  赋值失败
*/
RTDB_API __bool PMC_API pmc_type_cast(PCpmc_value_t _s, Ppmc_value_t _d)
{
	void * s = (void*)&_s->Value;
	void * d = (void*)&_d->Value;
	
	TAG_TYPE destType = get_value_type(_d->Flags);
    
	//1 _d字符串型
	if(destType == dt_string){
		_d->Value.str = _valueToString(_s);
		return _d->Value.str? __true : __false;
	}
    
	//2 _s和_d的数据类型一致，则直接赋值
	TAG_TYPE srcType = get_value_type(_s->Flags);
	if(destType == srcType){
		*_d = *_s;
		return __true;
	}
	
	//3 _s和_d的数据类型不一致，则分类型强制转换赋给_d
	switch(get_value_type(_s->Flags)){
    
    //3.1 _s为布尔型
	src_handler(dt_bool)
		handle_cast_self(dt_bool, __bool, s, d)
			
		handle_cast(dt_int8, __bool, __i8, s, d)
		handle_cast(dt_int16, __bool, __i16, s, d)
		handle_cast(dt_int32, __bool, __i32, s, d)
		handle_cast(dt_int64, __bool, __i64, s, d)
		
		handle_cast(dt_uint8, __bool, __u8, s, d)
		handle_cast(dt_uint16, __bool, __u16, s, d)
		handle_cast(dt_uint32, __bool, __u32, s, d)
		handle_cast(dt_uint64, __bool, __u64, s, d)
		
		handle_cast(dt_real4, __bool, __r4, s, d)
		handle_cast(dt_real8, __bool, __r8, s, d)

		invalid_cast();
	src_handler_end();
	
	//3.2 _s为float型
	src_handler(dt_real4)
		handle_cast_self(dt_real4, __r4, s, d)
			
		handle_cast(dt_int8, __r4, __i8, s, d)
		handle_cast(dt_int16, __r4, __i16, s, d)
		handle_cast(dt_int32, __r4, __i32, s, d)
		handle_cast(dt_int64, __r4, __i64, s, d)
		
		handle_cast(dt_uint8, __r4, __u8, s, d)
		handle_cast(dt_uint16, __r4, __u16, s, d)
		handle_cast(dt_uint32, __r4, __u32, s, d)
		handle_cast(dt_uint64, __r4, __u64, s, d)

		handle_cast(dt_real8, __r4, __r8, s, d)
		handle_cast(dt_date, __r4, __r8, s, d)
		
		case dt_bool:
			*((__bool*)d) = fabs(*((__r4*)s)) > 1e-5? 1 : 0;
			return __true;

		invalid_cast();
	src_handler_end();
    
	//3.3 _s为double型
	src_handler(dt_real8)
		handle_cast_self(dt_real8, __r8, s, d)
			
		handle_cast(dt_int8, __r8, __i8, s, d)
		handle_cast(dt_int16, __r8, __i16, s, d)
		handle_cast(dt_int32, __r8, __i32, s, d)
		handle_cast(dt_int64, __r8, __i64, s, d)
		
		handle_cast(dt_uint8, __r8, __u8, s, d)
		handle_cast(dt_uint16, __r8, __u16, s, d)
		handle_cast(dt_uint32, __r8, __u32, s, d)
		handle_cast(dt_uint64, __r8, __u64, s, d)

		handle_cast(dt_real4, __r8, __r4, s, d)
		handle_cast(dt_date, __r8, __r8, s, d)
		
		case dt_bool:
			*((__bool*)d) = fabs(*((__r8*)s)) > 1e-5? __true : __false;
			return __true;

		invalid_cast();
	src_handler_end();
    
	//3.4 _s为8位整型
	src_handler(dt_int8)
		handle_bool(__i8, s, d)

		handle_cast(dt_int8, __i8, __i8, s, d)
		handle_cast(dt_int16, __i8, __i16, s, d)
		handle_cast(dt_int32, __i8, __i32, s, d)
		handle_cast(dt_int64, __i8, __i64, s, d)
		
		handle_cast(dt_uint8, __i8, __u8, s, d)
		handle_cast(dt_uint16, __i8, __u16, s, d)
		handle_cast(dt_uint32, __i8, __u32, s, d)
		handle_cast(dt_uint64, __i8, __u64, s, d)
	
		handle_cast(dt_real4, __i8, __r4, s, d)
		handle_cast(dt_real8, __i8, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.5 _s为16位整型
	src_handler(dt_int16)
		handle_bool(__i16, s, d)
		
		handle_cast(dt_int8, __i16, __i8, s, d)
		handle_cast(dt_int16, __i16, __i16, s, d)
		handle_cast(dt_int32, __i16, __i32, s, d)
		handle_cast(dt_int64, __i16, __i64, s, d)
		
		handle_cast(dt_uint8, __i16, __u8, s, d)
		handle_cast(dt_uint16, __i16, __u16, s, d)
		handle_cast(dt_uint32, __i16, __u32, s, d)
		handle_cast(dt_uint64, __i16, __u64, s, d)
	
		handle_cast(dt_real4, __i16, __r4, s, d)
		handle_cast(dt_real8, __i16, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.7 _s为32位整型
	src_handler(dt_int32)
		handle_bool(__i32, s, d)
		
		handle_cast(dt_int8, __i32, __i8, s, d)
		handle_cast(dt_int16, __i32, __i16, s, d)
		handle_cast(dt_int32, __i32, __i32, s, d)
		handle_cast(dt_int64, __i32, __i64, s, d)
		
		handle_cast(dt_uint8, __i32, __u8, s, d)
		handle_cast(dt_uint16, __i32, __u16, s, d)
		handle_cast(dt_uint32, __i32, __u32, s, d)
		handle_cast(dt_uint64, __i32, __u64, s, d)
	
		handle_cast(dt_real4, __i32, __r4, s, d)
		handle_cast(dt_real8, __i32, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.8 _s为64位整型
	src_handler(dt_int64)
		handle_bool(__i64, s, d)
		
		handle_cast(dt_int8, __i64, __i8, s, d)
		handle_cast(dt_int16, __i64, __i16, s, d)
		handle_cast(dt_int32, __i64, __i32, s, d)
		handle_cast(dt_int64, __i64, __i64, s, d)
		
		handle_cast(dt_uint8, __i64, __u8, s, d)
		handle_cast(dt_uint16, __i64, __u16, s, d)
		handle_cast(dt_uint32, __i64, __u32, s, d)
		handle_cast(dt_uint64, __i64, __u64, s, d)
	
		handle_cast(dt_real4, __i64, __r4, s, d)
		handle_cast(dt_real8, __i64, __r8, s, d)
		handle_cast(dt_date, __i64, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.9 _s为无符号8位整型
	src_handler(dt_uint8)
		handle_bool(__u8, s, d)
		
		handle_cast(dt_int8, __u8, __i8, s, d)
		handle_cast(dt_int16, __u8, __i16, s, d)
		handle_cast(dt_int32, __u8, __i32, s, d)
		handle_cast(dt_int64, __u8, __i64, s, d)
		
		handle_cast(dt_uint8, __u8, __u8, s, d)
		handle_cast(dt_uint16, __u8, __u16, s, d)
		handle_cast(dt_uint32, __u8, __u32, s, d)
		handle_cast(dt_uint64, __u8, __u64, s, d)
	
		handle_cast(dt_real4, __u8, __r4, s, d)
		handle_cast(dt_real8, __u8, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.10 _s为无符号16位整型
	src_handler(dt_uint16)
		handle_bool(__u16, s, d)
		
		handle_cast(dt_int8, __u16, __i8, s, d)
		handle_cast(dt_int16, __u16, __i16, s, d)
		handle_cast(dt_int32, __u16, __i32, s, d)
		handle_cast(dt_int64, __u16, __i64, s, d)
		
		handle_cast(dt_uint8, __u16, __u8, s, d)
		handle_cast(dt_uint16, __u16, __u16, s, d)
		handle_cast(dt_uint32, __u16, __u32, s, d)
		handle_cast(dt_uint64, __u16, __u64, s, d)
	
		handle_cast(dt_real4, __u16, __r4, s, d)
		handle_cast(dt_real8, __u16, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.11 _s为无符号32位整型
	src_handler(dt_uint32)
		handle_bool(__u32, s, d)
		
		handle_cast(dt_int8, __u32, __i8, s, d)
		handle_cast(dt_int16, __u32, __i16, s, d)
		handle_cast(dt_int32, __u32, __i32, s, d)
		handle_cast(dt_int64, __u32, __i64, s, d)
		
		handle_cast(dt_uint8, __u32, __u8, s, d)
		handle_cast(dt_uint16, __u32, __u16, s, d)
		handle_cast(dt_uint32, __u32, __u32, s, d)
		handle_cast(dt_uint64, __u32, __u64, s, d)
	
		handle_cast(dt_real4, __u32, __r4, s, d)
		handle_cast(dt_real8, __u32, __r8, s, d)
		
		invalid_cast();
	src_handler_end();
	
	//3.12 _s为无符号64位整型
	src_handler(dt_uint64)
		handle_bool(__u64, s, d)
		
		handle_cast(dt_int8, __u64, __i8, s, d)
		handle_cast(dt_int16, __u64, __i16, s, d)
		handle_cast(dt_int32, __u64, __i32, s, d)
		handle_cast(dt_int64, __u64, __i64, s, d)
		
		handle_cast(dt_uint8, __u64, __u8, s, d)
		handle_cast(dt_uint16, __u64, __u16, s, d)
		handle_cast(dt_uint32, __u64, __u32, s, d)
		handle_cast(dt_uint64, __u64, __u64, s, d)
	
		case dt_real4:
			*((__r4*)d) = u64ToDouble(*((__u64*)s));
			return __true;
			
		case dt_real8:
		case dt_date:
			*((__r8*)d) = u64ToDouble(*((__u64*)s));
			return __true;
		
		invalid_cast();
	src_handler_end();
	
	//3.13 _s为日期型
	src_handler(dt_date)
		handle_cast(dt_real4, __r8, __r4, s, d)
		handle_cast(dt_real8, __r8, __r8, s, d)
		handle_cast(dt_date, __r8, __r8, s, d)
		handle_cast(dt_int64, __r8, __i64, s, d)
		handle_cast(dt_uint64, __r8, __u64, s, d)

		case dt_bool:
			*((__bool*)d) = fabs(*((__r8*)s)) > 1e-5? __true : __false;
			return __true;
		
		invalid_cast();
	src_handler_end();
    
	//3.14 _s为字符串型
	case dt_string:
		return _stringToValue(_s->Value.str, _d);
    
	//3.15 _s为宽字符串型？？
	case dt_bstr:
		return _bstrToValue(_s->Value.wstr, _d);
	}

	return __false;
}

RTDB_API char * PMC_API pmc_value_string(PCpmc_value_t v)
{
	pmc_value_t t;
	set_value_type(t.Flags, dt_string);
	pmc_type_cast(v, &t);
	return t.Value.str;
}

RTDB_API __bool PMC_API pmc_value_to_variant(
	PCpmc_value_t p, 
	VARIANT* tagValue
	)
{
	FILETIME ft;
	SYSTEMTIME st;
	
	USES_CONVERSION;

	if(!(p->Flags & TF_Valid)){
		tagValue->bstrVal = SysAllocString(L"N.A.");
		tagValue->vt = VT_BSTR;
		return __true;
	}
	
	switch(get_value_type(p->Flags)){
	case dt_bool:
		tagValue->lVal = p->Value.b? 1 : 0;
		tagValue->vt = VT_BOOL;
		break;
	case dt_real4:
		tagValue->fltVal = p->Value.flt;
		tagValue->vt = VT_R4;
		break;
	case dt_int32:	
		tagValue->lVal = p->Value.i32;
		tagValue->vt = VT_I4;
		break;
	case dt_real8:
		tagValue->dblVal = p->Value.dbl;
		tagValue->vt = VT_R8;
		break;
		
	case dt_date:
		*((__i64*)&ft) = p->Value.dbl*1e7;
		FileTimeToSystemTime(&ft, &st);
		SystemTimeToVariantTime(&st, &tagValue->date);
		tagValue->vt = VT_DATE;
		break;

	case dt_int8:
		tagValue->bVal = p->Value.i8;
		tagValue->vt = VT_I1;
		break;
	case dt_int16:
		tagValue->iVal = p->Value.i16;
		tagValue->vt = VT_I2;
		break;
	case dt_int64:
		tagValue->lVal = p->Value.i64;
		tagValue->vt = VT_I4;
		break;
	
	case dt_uint8:
		tagValue->bVal = p->Value.u8;
		tagValue->vt = VT_UI1;
		break;
	case dt_uint16:
		tagValue->lVal = p->Value.i32;
		tagValue->vt = VT_UI2;
		break;
	case dt_uint32:
		tagValue->lVal = p->Value.i32;
		tagValue->vt = VT_UI4;
		break;
	case dt_uint64:
		tagValue->lVal = p->Value.i32;
		tagValue->vt = VT_UI4;
		break;
	
	case dt_string:
		tagValue->bstrVal = SysAllocString(A2W(p->Value.str));
		tagValue->vt = VT_BSTR;
		break;
#if 0
	case dt_bstr:
		tagValue->bstrVal = SysAllocString(p->Value.bstr);
		tagValue->vt = VT_BSTR;
#endif
		break;
	default:
		tagValue->bstrVal = SysAllocString(L"Unsupported data type");
		tagValue->vt = VT_BSTR;
	}

	return __true;
}

RTDB_API __bool PMC_API pmc_variant_to_value(
	const VARIANT* tagValue,
	Ppmc_value_t p
	)
{
	SYSTEMTIME st;
	FILETIME ft;
	
	ZeroMemory(p, sizeof(*p));
	switch(tagValue->vt){
	case VT_BOOL:
		set_value_type(p->Flags, dt_bool);
		p->Value.b = tagValue->bVal? 1 : 0;
	break;
	case VT_UI1:
		set_value_type(p->Flags, dt_uint8);
		p->Value.u8 = tagValue->bVal;
	break;
	case VT_UI2:
		set_value_type(p->Flags, dt_uint16);
		p->Value.u16 = tagValue->uiVal;
	break;
	case VT_UI4:
		set_value_type(p->Flags, dt_uint32);
		p->Value.u32 = tagValue->ulVal;
	break;
	case VT_I1:
		set_value_type(p->Flags, dt_int8);
		p->Value.i8 = tagValue->cVal;
	break;
	case VT_I2:
		set_value_type(p->Flags, dt_int16);
		p->Value.i16 = tagValue->iVal;
	break;
	case VT_I4:
		set_value_type(p->Flags, dt_int32);
		p->Value.i32 = tagValue->lVal;
	break;
	case VT_R4:
		set_value_type(p->Flags, dt_real4);
		p->Value.flt = tagValue->fltVal;
	break;
	case VT_R8:
		set_value_type(p->Flags, dt_real8);
		p->Value.dbl = tagValue->dblVal;
	break;
	case VT_BSTR:
		set_value_type(p->Flags, dt_bstr);
		p->Value.wstr = SysAllocString(tagValue->bstrVal);
	break;
	case VT_DATE:
		VariantTimeToSystemTime(tagValue->date, &st);
		SystemTimeToFileTime(&st, &ft);
		set_value_type(p->Flags, dt_date);
		p->Value.dbl = u64ToDouble(*(__u64*)&ft);
	break;

	case VT_VARIANT | VT_BYREF:
		if(tagValue->pvarVal->vt & VT_BYREF){
			return __false;
		}
		return pmc_variant_to_value(tagValue->pvarVal, p);
	
	case VT_BOOL | VT_BYREF:
		set_value_type(p->Flags, dt_bool);
		p->Value.b = *tagValue->pbVal? 1 : 0;
	break;
	case VT_UI1 | VT_BYREF:
		set_value_type(p->Flags, dt_uint8);
		p->Value.u8 = *tagValue->pbVal;
	break;
	case VT_UI2 | VT_BYREF:
		set_value_type(p->Flags, dt_uint16);
		p->Value.u16 = *tagValue->puiVal;
	break;
	case VT_UI4 | VT_BYREF:
		set_value_type(p->Flags, dt_uint32);
		p->Value.u32 = *tagValue->pulVal;
	break;
	case VT_I1 | VT_BYREF:
		set_value_type(p->Flags, dt_int8);
		p->Value.i8 = *tagValue->pcVal;
	break;
	case VT_I2 | VT_BYREF:
		set_value_type(p->Flags, dt_int16);
		p->Value.i16 = *tagValue->piVal;
	break;
	case VT_I4 | VT_BYREF:
		set_value_type(p->Flags, dt_int32);
		p->Value.i32 = *tagValue->plVal;
	break;
	case VT_R4 | VT_BYREF:
		set_value_type(p->Flags, dt_real4);
		p->Value.flt = *tagValue->pfltVal;
	break;
	case VT_R8 | VT_BYREF:
		set_value_type(p->Flags, dt_real8);
		p->Value.dbl = *tagValue->pdblVal;
	break;
	case VT_BSTR | VT_BYREF:
		set_value_type(p->Flags, dt_bstr);
		p->Value.wstr = SysAllocString(*tagValue->pbstrVal);
	break;
	case VT_DATE | VT_BYREF:
		VariantTimeToSystemTime(*tagValue->pdate, &st);
		SystemTimeToFileTime(&st, &ft);
		set_value_type(p->Flags, dt_date);
		p->Value.dbl = u64ToDouble(*(__u64*)&ft);
	break;	
	/*
	case VT_FILETIME:
		set_value_type(p->Flags, dt_date);
		p->Value.dbl = *((__u64*)&ft);
	break;
	*/
	default:
		return __false;
	}
	
	return __true;
}

/*
	this should be made a library function
*/
RTDB_API __u16 PMC_API pmc_type_to_variant_type(__u8 t)
{
	static VARENUM typeTable[]={
		VT_EMPTY, 
		VT_BOOL, 
		VT_R4, 
		VT_I4, 
		VT_R8, 
		VT_DATE, 
		VT_I1, 
		VT_I2, 
		VT_I8, 
		VT_UI1,
		VT_UI2, 
		VT_UI4, 
		VT_UI8, 
		VT_LPSTR, 
		VT_BSTR
	};
	if(t >= sizeof(typeTable) / sizeof(typeTable[0])){
		return VT_EMPTY;
	}
	return typeTable[t];
}

RTDB_API __u8 PMC_API pmc_type_from_variant_type(__u16 vt)
{
	static __u8 typeTable[]={
	dt_null, // VT_EMPTY 		  = 0,
	dt_null, // VT_NULL			  = 1,
	dt_int16, // VT_I2			  = 2,
	dt_int32, // VT_I4			  = 3,
	dt_real4, // VT_R4			  = 4,
	dt_real8, // VT_R8			  = 5,
	dt_null, // VT_CY			  = 6,
	dt_date, // VT_DATE			  = 7,
	dt_bstr, // VT_BSTR			  = 8,
	dt_null, // VT_DISPATCH		  = 9,
	dt_null, // VT_ERROR 		  = 10,
	dt_bool, // VT_BOOL			  = 11,
	dt_null, // VT_VARIANT		  = 12,
	dt_null, // VT_UNKNOWN		  = 13,
	dt_null, // VT_DECIMAL		  = 14,
	// // VBA reserves 15 for future use
	dt_int8, // VT_I1			  = 16,
	dt_uint8, // VT_UI1			  = 17,
	dt_uint16, // VT_UI2			  = 18,
	dt_uint32, // VT_UI4			  = 19,
	dt_int64, // VT_I8			  = 20,
	dt_uint64, // VT_UI8			  = 21,
	dt_int, // VT_INT			  = 22,
	dt_uint, // VT_UINT			  = 23,
	dt_null, // VT_VOID			  = 24,
	dt_null, // VT_HRESULT		  = 25,
	dt_null, // VT_PTR			  = 26,
	dt_null, // VT_SAFEARRAY 	  = 27,
	dt_null, // VT_CARRAY		  = 28,
	dt_null, // VT_USERDEFINED	  = 29,
	dt_string, // VT_LPSTR 		  = 30,
	dt_null, // VT_LPWSTR		  = 31,
	};

	if(vt >= sizeof(typeTable) / sizeof(typeTable[0])){
		return dt_null;
	}
	return typeTable[vt];
}


