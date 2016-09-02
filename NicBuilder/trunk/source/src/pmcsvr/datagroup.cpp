// CDataGroup.cpp: implementation of the CDataGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "datagroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataGroup::CDataGroup()
{

}

CDataGroup::~CDataGroup()
{
}

// members of IOPCSyncIO
HRESULT STDMETHODCALLTYPE CDataGroup::Read( 
/* [in] */ OPCDATASOURCE dwSource,
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][size_is][out] */ OPCITEMSTATE **ppItemValues,
/* [size_is][size_is][out] */ HRESULT **ppErrors
)
{
	return E_NOTIMPL;
}

// 
HRESULT STDMETHODCALLTYPE CDataGroup::Write( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ VARIANT *pItemValues,
/* [size_is][size_is][out] */ HRESULT **ppErrors
)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDataGroup::AddItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCITEMDEF *pItemArray,
/* [size_is][size_is][out] */ OPCITEMRESULT **ppAddResults,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDataGroup::ValidateItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCITEMDEF *pItemArray,
/* [in] */ BOOL bBlobUpdate,
/* [size_is][size_is][out] */ OPCITEMRESULT **ppValidationResults,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDataGroup::RemoveItems( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE CDataGroup::SetActiveState( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [in] */ BOOL bActive,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE CDataGroup::SetClientHandles( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ OPCHANDLE *phClient,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDataGroup::SetDatatypes( 
/* [in] */ DWORD dwCount,
/* [size_is][in] */ OPCHANDLE *phServer,
/* [size_is][in] */ VARTYPE *pRequestedDatatypes,
/* [size_is][size_is][out] */ HRESULT **ppErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDataGroup::CreateEnumerator( 
/* [in] */ REFIID riid,
/* [iid_is][out] */ LPUNKNOWN *ppUnk)
{
	return E_NOTIMPL;
}


