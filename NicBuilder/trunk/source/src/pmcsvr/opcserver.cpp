// OpcServer.cpp : Implementation of COpcServer
#include "stdafx.h"
#include "pmcsvr.h"
#include "opcserver.h"

#undef STDMETHOD
#define STDMETHOD(x) HRESULT x

/////////////////////////////////////////////////////////////////////////////
// COpcServer
COpcServer::COpcServer()
{
}

COpcServer::~COpcServer()
{
}

HRESULT STDMETHODCALLTYPE COpcServer::AddGroup( 
	/* [string][in] */ LPCWSTR szName,
	/* [in] */ BOOL bActive,
	/* [in] */ DWORD dwRequestedUpdateRate,
	/* [in] */ OPCHANDLE hClientGroup,
	/* [in][unique] */ LONG *pTimeBias,
	/* [in][unique] */ FLOAT *pPercentDeadband,
	/* [in] */ DWORD dwLCID,
	/* [out] */ OPCHANDLE *phServerGroup,
	/* [out] */ DWORD *pRevisedUpdateRate,
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ LPUNKNOWN *ppUnk
)
{
	HRESULT res;
	USES_CONVERSION;
	char * ansiName;

	if (phServerGroup == NULL)
		return E_POINTER;
	
	if (pRevisedUpdateRate == NULL)
		return E_POINTER;
	
	if (ppUnk == NULL)
		return E_POINTER;
	
	groups_t::iterator it;
	ansiName = W2A(szName);
	it = groups.find(ansiName);
	if(it != groups.end()){
		return E_ACCESSDENIED;
	}

	IUnknown * unk;
	unk = 0;
	res = CDataGroup::CreateInstance(NULL, &unk);
	if(FAILED(res)){
		return res;
	}

	res = unk->QueryInterface(riid, (void**)ppUnk);
	unk->Release();
	if(FAILED(res)){
		return res;
	}

	CDataGroup * grp;
	pair<groups_t::iterator, bool> ib;
	grp = dynamic_cast<CDataGroup*>(*ppUnk);
	if(!grp){
		// assert(0);
		/*
			it's nothing, we just lost connection with this newly
			created group, however this won't affect the functionality
			except for debug purposes.

			for now, this only happens when _ATL_DEBUG_INTERFACES
			is enabled.
		*/
	}else{
		ib = groups.insert(groups_t::value_type(ansiName, grp));
		if(!ib.second){
			return E_OUTOFMEMORY;
		}
		
		grp->AddRef();
		*phServerGroup = (DWORD)(ib.first->first.c_str());
	}	

	return res;
}

HRESULT STDMETHODCALLTYPE COpcServer::GetErrorString( 
/* [in] */ HRESULT dwError,
/* [in] */ LCID dwLocale,
/* [string][out] */ LPWSTR *ppString)
{
	if (ppString == NULL)
		return E_POINTER;
	
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE COpcServer::GetGroupByName( 
/* [string][in] */ LPCWSTR szName,
/* [in] */ REFIID riid,
/* [iid_is][out] */ LPUNKNOWN *ppUnk)
{
	char * ansiName;
	USES_CONVERSION;

	if (ppUnk == NULL)
		return E_POINTER;
	
	groups_t::iterator it;
	ansiName = W2A(szName);
	it = groups.find(ansiName);
	if(it == groups.end()){
		return E_INVALIDARG;
	}
	CDataGroup * grp;
	HRESULT res;
	grp = it->second;
	res = grp->QueryInterface(riid, (void**)ppUnk);

	return res;
}

 HRESULT STDMETHODCALLTYPE COpcServer::GetStatus( 
/* [out] */ OPCSERVERSTATUS **ppServerStatus)
{
	if (ppServerStatus == NULL)
		return E_POINTER;
	
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE COpcServer::RemoveGroup( 
/* [in] */ OPCHANDLE hServerGroup,
/* [in] */ BOOL bForce)
{
	char * ansiName;
	ansiName = (char *)hServerGroup;
	groups_t::iterator it;
	it = groups.find(ansiName);
	if(it == groups.end()){
		return E_POINTER;
	}
	CDataGroup * grp;
	grp = it->second;
	grp->Release();
	groups.erase(it);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE COpcServer::CreateGroupEnumerator( 
/* [in] */ OPCENUMSCOPE dwScope,
/* [in] */ REFIID riid,
/* [iid_is][out] */ LPUNKNOWN *ppUnk)
{
	if (ppUnk == NULL)
		return E_POINTER;
	
	return E_NOTIMPL;
}
