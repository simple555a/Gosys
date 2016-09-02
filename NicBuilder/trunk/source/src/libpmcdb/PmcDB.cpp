// PmcDB.cpp : CPmcDB 的实现

#include "stdafx.h"
#include "PmcDB.h"
#include "dllmain.h"

#include <pmc.h>
#include <hisproxy.h>


#pragma comment(lib, "librtkproxy.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libutils.lib")




static LONG icount = 0;

static void _setupProxy()
{
	wchar_t path[_MAX_PATH];
	USES_CONVERSION;
	GetModuleFileName(_AtlBaseModule.m_hInst, path, sizeof(path));
	extract_file_path(path);
	set_working_dir(W2A(path));
	init_proxy(
		0, 
		PROXY_SILENT | PROXY_ENABLE_BGLOADER
		);
	initHisProxy();
}

CPmcDB::CPmcDB()
{
	//
	if(!icount){
		icount++;
		_setupProxy();
	}
}

CPmcDB::~CPmcDB()
{
	icount--;
	if(!icount){
		uninitHisProxy();
		uninit_proxy();
	}
}
// CPmcDB

STDMETHODIMP CPmcDB::rtdbLock(BOOL exclusive, LONG timeOut)
{
	// TODO: 在此添加实现代码
	lock_rtdb(exclusive, timeOut);
	return S_OK;
}

STDMETHODIMP CPmcDB::rtdbUnlock(void)
{
	// TODO: 在此添加实现代码

	unlock_rtdb();
	return S_OK;
}

STDMETHODIMP CPmcDB::readSnapshot(BSTR tagName, VARIANT* tagValue)
{
	TAG_NAME tn;
	tagname_by_textW(&tn, tagName);
	PRTK_TAG p;

	USES_CONVERSION;
#if 0
	COleDateTime dt;
	FILETIME ft;
#endif

	if(!lock_rtdb(__false, 500)){
		return S_FALSE;
	}

	p = query_tag_f(&tn);
	
	if(!p){
		tagValue->bstrVal = SysAllocString(L"N.A.");
		tagValue->vt = VT_BSTR;
		unlock_rtdb();
		return S_OK;
	}

	pmc_value_to_variant(&p->d.Value, tagValue);

	unlock_rtdb();
	
	return S_OK;
}

STDMETHODIMP CPmcDB::writeDevice(BSTR tagName, VARIANT tagValue)
{
	TAG_NAME tn;
	pmc_value_t pt;

	tagname_by_textW(&tn, tagName);
	if(!pmc_variant_to_value(&tagValue, &pt)){
		return E_INVALIDARG;
	}
	proxy_write_tags_ex(1, &tn, &pt);
	pmc_clear(&pt);

	return S_OK;
}

STDMETHODIMP CPmcDB::readArchiveValue(
	BSTR tagName, 
	VARIANT date, 
	VARIANT * tagValue
	)
{
	SYSTEMTIME st;
	FILETIME ft;
	TAG_NAME tn;
	hislog_item_t value;
	VARIANT dd;
	HRESULT hr;
	
	if(date.vt != VT_DATE){
		// BUGBUG: not tested
		hr = VariantChangeType(&dd, &date, 0, VT_DATE);
		if(FAILED(hr)){
			return hr;
		}
		date = dd;
	}
	VariantTimeToSystemTime(date.date, &st);
	SystemTimeToFileTime(&st, &ft);
	tagname_by_textW(&tn, tagName);
	queryHisData(PT_QueryMode_BYCOUNT,&tn, &ft, 1, 1);
	getHisData(&value);
	pmc_value_to_variant(&value, tagValue);
	return S_OK;
}

HRESULT CPmcDB::sleep(LONG seconds)
{
	rtk_sleep(seconds);
	return S_OK;
}

static SAFEARRAY * getArrayParam(const VARIANT * v)
{
	SAFEARRAY * a;
	switch(v->vt){
	case VT_VARIANT | VT_BYREF:
		a=getArrayParam(v->pvarVal);
		break;
	case VT_ARRAY | VT_VARIANT:
		a=v->parray;
		break;
	case VT_ARRAY | VT_VARIANT | VT_BYREF:
		a=*v->pparray;
		break;
	default:
		a=0;
	}
	return a;
}

/* 2006-3-14 batch read/write added */
HRESULT STDMETHODCALLTYPE CPmcDB::readSnapshots( 
	LONG count,
	VARIANT _tagNames,
	VARIANT * _tagValues)
{
	TAG_NAME *names,*n;
	pmc_value_t *values,*v;
	void * heap;
	__uint heapSize;
	HRESULT hr;
	VARIANT * pn;
	VARIANT * pv;
	int i;

	SAFEARRAY * tagNames=getArrayParam(&_tagNames);
	SAFEARRAY * tagValues=getArrayParam(_tagValues);
	if(!tagNames || !tagValues){
		return E_INVALIDARG;
	}
	if(SafeArrayGetDim(tagNames) != 1 ||SafeArrayGetDim(tagValues) != 1 ||
		tagNames->rgsabound[0].cElements < count || tagValues->rgsabound[0].cElements < count){
		return E_INVALIDARG;
	}

	hr = SafeArrayAccessData(tagNames, (void **)&pn);
	if(FAILED(hr)){
		return hr;
	}
	hr = SafeArrayAccessData(tagValues, (void **)&pv);
	if(FAILED(hr)){
		return hr;
	}

	heapSize=(sizeof(TAG_NAME)*+ sizeof(pmc_value_t))*count;
	heap=malloc(heapSize);
	if(!heap){
		SafeArrayUnaccessData(tagNames);
		SafeArrayUnaccessData(tagValues);
		return E_OUTOFMEMORY;
	}
	ZeroMemory(heap,heapSize);
	names=(TAG_NAME*)heap;
	values=(pmc_value_t*)(names+count);
	
	for(i=0,n=names; i<count; i++,n++,pn++){
		// TODO:
		if(pn->vt == VT_BSTR){
			tagname_by_textW(n, pn->bstrVal);
		}
	}
	
	proxy_read_tags(count, names, values, 0);
	
	for(i=0,v=values; i<count; i++,v++,pv++){
		pmc_value_to_variant(v, pv);
	}

	free(heap);
	SafeArrayUnaccessData(tagNames);
	SafeArrayUnaccessData(tagValues);
	
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CPmcDB::writeSnapshots( 
	LONG count,
	VARIANT _tagNames,
	VARIANT _tagValues)
{
	TAG_NAME *names,*n;
	pmc_value_t *values,*v;
	void * heap;
	__uint heapSize;
	HRESULT hr;
	VARIANT * pn;
	VARIANT * pv;
	int i;

	SAFEARRAY * tagNames=getArrayParam(&_tagNames);
	SAFEARRAY * tagValues=getArrayParam(&_tagValues);
	if(!tagNames || !tagValues){
		return E_INVALIDARG;
	}
	if(SafeArrayGetDim(tagNames) != 1 ||SafeArrayGetDim(tagValues) != 1 ||
		tagNames->rgsabound[0].cElements < count || tagValues->rgsabound[0].cElements < count){
		return E_INVALIDARG;
	}

	hr = SafeArrayAccessData(tagNames, (void **)&pn);
	if(FAILED(hr)){
		return hr;
	}
	hr = SafeArrayAccessData(tagValues, (void **)&pv);
	if(FAILED(hr)){
		return hr;
	}

	heapSize=(sizeof(TAG_NAME)*count+ sizeof(pmc_value_t))*count;
	heap=malloc(heapSize);
	if(!heap){
		SafeArrayUnaccessData(tagNames);
		SafeArrayUnaccessData(tagValues);
		return E_OUTOFMEMORY;
	}
	ZeroMemory(heap,heapSize);
	names=(TAG_NAME*)heap;
	values=(pmc_value_t*)(names+count);
	
	for(i=0,n=names,v=values; i<count; i++,n++,pn++,pv++,v++){
		// TODO:
		if(pn->vt == VT_BSTR){
			tagname_by_textW(n, pn->bstrVal);
			pmc_variant_to_value(pv, v);
		}
	}
	
	proxy_write_tags_ex(count, names, values);
	
	free(heap);
	SafeArrayUnaccessData(tagNames);
	SafeArrayUnaccessData(tagValues);
	
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CPmcDB::readArchiveValues( 
			VARIANT tagName, 
			VARIANT startTime,
			LONG interval,
			LONG count,
			VARIANT * _hisValues
			)
{
	SYSTEMTIME st;
	FILETIME ft;
	VARIANT dd;
	TAG_NAME tn;
	hislog_item_t value[600];
	HRESULT hr;
	VARIANT * pv;
	int i;


	SAFEARRAY * hisValues=getArrayParam(_hisValues);
	if(!hisValues){
		return E_INVALIDARG;
	}
	if(hisValues->rgsabound[0].cElements < count){
		return E_INVALIDARG;
	}

	hr = SafeArrayAccessData(hisValues, (void **)&pv);
	if(FAILED(hr)){
		return hr;
	}
	
	if(startTime.vt != VT_DATE){
		// BUGBUG: not tested
		hr = VariantChangeType(&dd, &startTime, 0, VT_DATE);
		if(FAILED(hr)){
			return hr;
		}
		startTime = dd;
	}
	VariantTimeToSystemTime(startTime.date, &st);
	SystemTimeToFileTime(&st, &ft);
	tagname_by_textW(&tn, tagName.bstrVal);
	if(count > 600){
		count = 600;
	}
	queryHisData(PT_QueryMode_BYCOUNT,&tn, &ft, interval, count);
	getHisData(value);
	
	for(i=0; i < count; i++, pv++){
		pmc_value_to_variant(&value[i], pv);
	}

	SafeArrayUnaccessData(hisValues);
	
	return S_OK;
}
// CPmcDB

