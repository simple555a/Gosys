// PmcDB.h : Declaration of the CPmcDB

#ifndef __PMCDB_H_
#define __PMCDB_H_

#include "resource.h"       // main symbols

#pragma warning(disable:4530)
#include "pmc.h"
#include "hisproxy.h"

#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtkproxy.lib")
#pragma comment(lib, "libhisproxy.lib")
#pragma comment(lib, "librtdb.lib")

/////////////////////////////////////////////////////////////////////////////
//Global Variables
static ULONG count;
//Global Variables
static BOOL isAllAlarmAck = TRUE;
//Functions
void _setupProxy();

// CPmcDB
class ATL_NO_VTABLE CPmcDB : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPmcDB, &CLSID_PmcDB>,
	public IDispatchImpl<IPmcDB, &IID_IPmcDB, &LIBID_LIBHISDATLLib>
{
public:
	CPmcDB();
	~CPmcDB();

DECLARE_REGISTRY_RESOURCEID(IDR_PMCDB)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPmcDB)
	COM_INTERFACE_ENTRY(IPmcDB)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPmcDB
public:
	STDMETHOD(writeTag)(/*[in]*/BSTR tagName, /*[in]*/VARIANT tagValue);
	/*功    能：取一个标签指定时间段内指定数量的数据
	  参数说明：
	      tag          变量名
		          pdate[0]为起始时间、pdate[1]为终止时间
		  valueCount   指定查询的数据个数
		  tagValue     查询得到的数据
	*/
	STDMETHOD(readArchiveValuesByCount)(/*[in]*/BSTR tag, /*[in]*/VARIANT sTime, /*[in]*/VARIANT eTime, /*[in]*/int valueCount, /*[in, out]*/VARIANT* tagValue);
	/*功    能：取一个标签某一时刻的数据
	  参数说明：
	      tagName      变量名
		  date         查询时刻
		  tagValue     查询得到的数据
	*/
	STDMETHOD(readArchiveValue)(/*[in]*/BSTR tagName, /*[in]*/VARIANT date, /*[in, out]*/VARIANT* tagValue);
	STDMETHOD(writeDevice)(/*[in]*/BSTR tagName, /*[in]*/VARIANT tagValue);
	STDMETHOD(readSnapshot)(/*[in]*/BSTR tagName, /*[in,out]*/VARIANT* tagValue);
	STDMETHOD(unlock)();
	STDMETHOD(lock)(/*[in]*/BOOL exclusive, /*[in]*/LONG timeOut);
	STDMETHOD(isAlarmTag)(/*[in]*/BSTR tagName, /*[in,out]*/VARIANT* alarm_sign);
};

#endif //__PMCDB_H_
