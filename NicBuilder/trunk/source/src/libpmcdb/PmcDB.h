// PmcDB.h : CPmcDB ������

#pragma once
#include "resource.h"       // ������



#include "libpmcdb_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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

