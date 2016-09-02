// DataGroup.h: interface for the DataGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAGROUP_H__40042E2A_3168_4DEF_8B81_74D9370D898E__INCLUDED_)
#define AFX_DATAGROUP_H__40042E2A_3168_4DEF_8B81_74D9370D898E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pmcsvr.h"

class CDataGroup  :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataGroup>,
	public IOPCSyncIO,
	public IOPCItemMgt
{
public:
	CDataGroup();
	virtual ~CDataGroup();

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataGroup)
	COM_INTERFACE_ENTRY(IOPCSyncIO)
	COM_INTERFACE_ENTRY(IOPCItemMgt)
END_COM_MAP()

// members of IOPCSyncIO
virtual HRESULT STDMETHODCALLTYPE Read( 
/* [in] */ OPCDATASOURCE dwSource,
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][size_is][out] */ OPCITEMSTATE **ppItemValues,
/* [size_is][size_is][out] */ HRESULT **ppErrors
);

// 
virtual HRESULT STDMETHODCALLTYPE Write( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ VARIANT *pItemValues,
/* [size_is][size_is][out] */ HRESULT **ppErrors
);

virtual HRESULT STDMETHODCALLTYPE AddItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCITEMDEF *pItemArray,
/* [size_is][size_is][out] */ OPCITEMRESULT **ppAddResults,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE ValidateItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCITEMDEF *pItemArray,
/* [in] */ BOOL bBlobUpdate,
/* [size_is][size_is][out] */ OPCITEMRESULT **ppValidationResults,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE RemoveItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE SetActiveState( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [in] */ BOOL bActive,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE SetClientHandles( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ OPCHANDLE *phClient,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE SetDatatypes( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ VARTYPE *pRequestedDatatypes,
/* [size_is][size_is][out] */ HRESULT **ppErrors);

virtual HRESULT STDMETHODCALLTYPE CreateEnumerator( 
/* [in] */ REFIID riid,
/* [iid_is][out] */ LPUNKNOWN *ppUnk);
};

#endif // !defined(AFX_DATAGROUP_H__40042E2A_3168_4DEF_8B81_74D9370D898E__INCLUDED_)
