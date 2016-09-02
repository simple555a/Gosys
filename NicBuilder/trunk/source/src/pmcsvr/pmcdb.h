// PmcDB.h : CPmcDB µÄÉùÃ÷
#pragma once
#include "resource.h"       // Ö÷·ûºÅ

EXTERN_C const IID DIID_IPmcDBEvents;
EXTERN_C const IID DIID_IAppEvents;

#include "pmcsvr.h"
#include "pmcsvrCP.h"
#include "pmcsvr_i.h"

// CPmcDB

class ATL_NO_VTABLE CPmcDB : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPmcDB, &CLSID_PmcDB>,
	public IConnectionPointContainerImpl<CPmcDB>,
	public IDispatchImpl<IPmcDB, &IID_IPmcDB, &LIBID_PMCSVRLIB, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IProvideClassInfoImpl<&CLSID_PmcDB>,
	public CProxyIPmcDBEvents< CPmcDB >
{
public:
	DECLARE_CLASSFACTORY_SINGLETON(CPmcDB)
		
	CPmcDB();
	virtual ~CPmcDB();

DECLARE_REGISTRY_RESOURCEID(IDR_PMCDB)

// DECLARE_CLASSFACTORY_SINGLETON(CPmcDB)

BEGIN_COM_MAP(CPmcDB)
	COM_INTERFACE_ENTRY(IPmcDB)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPmcDB)
	CONNECTION_POINT_ENTRY(DIID_IPmcDBEvents)
END_CONNECTION_POINT_MAP()

DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(lock)(BOOL exclusive, LONG timeOut);
	STDMETHOD(unlock)(void);
	STDMETHOD(readSnapshot)(BSTR tagName, VARIANT* tagValue);
	STDMETHOD(writeDevice)(BSTR tagName, VARIANT tagValue);
	STDMETHOD(readArchiveValue)(
		BSTR tagName, 
		VARIANT date, 
		VARIANT* tagValue
		);
	STDMETHOD(sleep)(LONG miliSeconds);

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
};

