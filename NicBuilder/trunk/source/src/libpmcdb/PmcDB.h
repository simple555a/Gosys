// PmcDB.h : CPmcDB 的声明

#pragma once
#include "resource.h"       // 主符号



#include "libpmcdb_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CPmcDB

class ATL_NO_VTABLE CPmcDB :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPmcDB, &CLSID_PmcDB>,
	public IDispatchImpl<IPmcDB, &IID_IPmcDB, &LIBID_libpmcdbLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPmcDB();
	~CPmcDB();
DECLARE_REGISTRY_RESOURCEID(IDR_PMCDB)


BEGIN_COM_MAP(CPmcDB)
	COM_INTERFACE_ENTRY(IPmcDB)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(rtdbLock)(BOOL exclusive, LONG timeOut);
	STDMETHOD(rtdbUnlock)(void);
	STDMETHOD(readSnapshot)(BSTR tagName, VARIANT* tagValue);
	STDMETHOD(writeDevice)(BSTR tagName, VARIANT tagValue);
	STDMETHOD(readArchiveValue)(
		BSTR tagName, 
		VARIANT date, 
		VARIANT* tagValue
		);

	/* 2006-3-14 batch read/write for performance */
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readSnapshots( 
        	LONG count,
            /* [in] */ VARIANT tagNames,
            /* [out][in] */ VARIANT * tagValues);
                
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE writeSnapshots( 
			LONG count,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT tagValues);
                
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readArchiveValues( 
			VARIANT tagName,
			VARIANT startTime,
			LONG interval,
			LONG count,
			VARIANT * values
			);
		
	STDMETHOD(sleep)(LONG seconds);


};

OBJECT_ENTRY_AUTO(__uuidof(PmcDB), CPmcDB)

