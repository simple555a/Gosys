// OpcServer.h : Declaration of the COpcServer

#ifndef __OPCSERVER_H_
#define __OPCSERVER_H_

#include "resource.h"       // main symbols
#include "datagroup.h"

typedef map<string, CDataGroup*> groups_t;

/////////////////////////////////////////////////////////////////////////////
// COpcServer
class ATL_NO_VTABLE COpcServer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COpcServer, &CLSID_OpcServer>,
	public IOPCServer
{
public:
	COpcServer();
	virtual ~COpcServer();

DECLARE_REGISTRY_RESOURCEID(IDR_OPCSERVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COpcServer)
	COM_INTERFACE_ENTRY(IOPCServer)
END_COM_MAP()

public:
	virtual HRESULT STDMETHODCALLTYPE AddGroup( 
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
		/* [iid_is][out] */ LPUNKNOWN *ppUnk);
        
        virtual HRESULT STDMETHODCALLTYPE GetErrorString( 
		/* [in] */ HRESULT dwError,
		/* [in] */ LCID dwLocale,
		/* [string][out] */ LPWSTR *ppString);
        
        virtual HRESULT STDMETHODCALLTYPE GetGroupByName( 
		/* [string][in] */ LPCWSTR szName,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ LPUNKNOWN *ppUnk);
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
		/* [out] */ OPCSERVERSTATUS **ppServerStatus);
        
        virtual HRESULT STDMETHODCALLTYPE RemoveGroup( 
		/* [in] */ OPCHANDLE hServerGroup,
		/* [in] */ BOOL bForce);
        
        virtual HRESULT STDMETHODCALLTYPE CreateGroupEnumerator( 
		/* [in] */ OPCENUMSCOPE dwScope,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ LPUNKNOWN *ppUnk);

public:
	groups_t groups;	
};

#endif //__OPCSERVER_H_
