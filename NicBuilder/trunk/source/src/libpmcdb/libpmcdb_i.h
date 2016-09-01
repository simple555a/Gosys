

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Sep 01 19:26:51 2016
 */
/* Compiler settings for libpmcdb.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __libpmcdb_i_h__
#define __libpmcdb_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComponentRegistrar_FWD_DEFINED__
#define __IComponentRegistrar_FWD_DEFINED__
typedef interface IComponentRegistrar IComponentRegistrar;

#endif 	/* __IComponentRegistrar_FWD_DEFINED__ */


#ifndef __IPmcDB_FWD_DEFINED__
#define __IPmcDB_FWD_DEFINED__
typedef interface IPmcDB IPmcDB;

#endif 	/* __IPmcDB_FWD_DEFINED__ */


#ifndef __CompReg_FWD_DEFINED__
#define __CompReg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CompReg CompReg;
#else
typedef struct CompReg CompReg;
#endif /* __cplusplus */

#endif 	/* __CompReg_FWD_DEFINED__ */


#ifndef __PmcDB_FWD_DEFINED__
#define __PmcDB_FWD_DEFINED__

#ifdef __cplusplus
typedef class PmcDB PmcDB;
#else
typedef struct PmcDB PmcDB;
#endif /* __cplusplus */

#endif 	/* __PmcDB_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IComponentRegistrar_INTERFACE_DEFINED__
#define __IComponentRegistrar_INTERFACE_DEFINED__

/* interface IComponentRegistrar */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IComponentRegistrar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a817e7a2-43fa-11d0-9e44-00aa00b6770a")
    IComponentRegistrar : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ BSTR bstrPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetComponents( 
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IComponentRegistrarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponentRegistrar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponentRegistrar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponentRegistrar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponentRegistrar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponentRegistrar * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetComponents )( 
            IComponentRegistrar * This,
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        END_INTERFACE
    } IComponentRegistrarVtbl;

    interface IComponentRegistrar
    {
        CONST_VTBL struct IComponentRegistrarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponentRegistrar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponentRegistrar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponentRegistrar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponentRegistrar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponentRegistrar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponentRegistrar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponentRegistrar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IComponentRegistrar_Attach(This,bstrPath)	\
    ( (This)->lpVtbl -> Attach(This,bstrPath) ) 

#define IComponentRegistrar_RegisterAll(This)	\
    ( (This)->lpVtbl -> RegisterAll(This) ) 

#define IComponentRegistrar_UnregisterAll(This)	\
    ( (This)->lpVtbl -> UnregisterAll(This) ) 

#define IComponentRegistrar_GetComponents(This,pbstrCLSIDs,pbstrDescriptions)	\
    ( (This)->lpVtbl -> GetComponents(This,pbstrCLSIDs,pbstrDescriptions) ) 

#define IComponentRegistrar_RegisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> RegisterComponent(This,bstrCLSID) ) 

#define IComponentRegistrar_UnregisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> UnregisterComponent(This,bstrCLSID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IComponentRegistrar_INTERFACE_DEFINED__ */


#ifndef __IPmcDB_INTERFACE_DEFINED__
#define __IPmcDB_INTERFACE_DEFINED__

/* interface IPmcDB */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPmcDB;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5D2076E3-68A4-4209-B965-45D48E28C65A")
    IPmcDB : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE rtdbLock( 
            /* [in] */ BOOL exclusive,
            /* [in] */ LONG timeOut) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE rtdbUnlock( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE readSnapshot( 
            /* [in] */ BSTR tagName,
            /* [retval][out] */ VARIANT *tagValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE writeDevice( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE readArchiveValue( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT time,
            /* [retval][out] */ VARIANT *tagValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE readSnapshots( 
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [out][in] */ VARIANT *tagValues) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE writeSnapshots( 
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT tagValues) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE readArchiveValues( 
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT startTime,
            /* [in] */ LONG interval,
            /* [in] */ LONG count,
            /* [out][in] */ VARIANT *tagValues) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE sleep( 
            /* [in] */ LONG miliSeconds) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPmcDBVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPmcDB * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPmcDB * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPmcDB * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPmcDB * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPmcDB * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPmcDB * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPmcDB * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *rtdbLock )( 
            IPmcDB * This,
            /* [in] */ BOOL exclusive,
            /* [in] */ LONG timeOut);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *rtdbUnlock )( 
            IPmcDB * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *readSnapshot )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [retval][out] */ VARIANT *tagValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *writeDevice )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValue )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT time,
            /* [retval][out] */ VARIANT *tagValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *readSnapshots )( 
            IPmcDB * This,
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [out][in] */ VARIANT *tagValues);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *writeSnapshots )( 
            IPmcDB * This,
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT tagValues);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValues )( 
            IPmcDB * This,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT startTime,
            /* [in] */ LONG interval,
            /* [in] */ LONG count,
            /* [out][in] */ VARIANT *tagValues);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *sleep )( 
            IPmcDB * This,
            /* [in] */ LONG miliSeconds);
        
        END_INTERFACE
    } IPmcDBVtbl;

    interface IPmcDB
    {
        CONST_VTBL struct IPmcDBVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPmcDB_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPmcDB_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPmcDB_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPmcDB_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPmcDB_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPmcDB_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPmcDB_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPmcDB_rtdbLock(This,exclusive,timeOut)	\
    ( (This)->lpVtbl -> rtdbLock(This,exclusive,timeOut) ) 

#define IPmcDB_rtdbUnlock(This)	\
    ( (This)->lpVtbl -> rtdbUnlock(This) ) 

#define IPmcDB_readSnapshot(This,tagName,tagValue)	\
    ( (This)->lpVtbl -> readSnapshot(This,tagName,tagValue) ) 

#define IPmcDB_writeDevice(This,tagName,tagValue)	\
    ( (This)->lpVtbl -> writeDevice(This,tagName,tagValue) ) 

#define IPmcDB_readArchiveValue(This,tagName,time,tagValue)	\
    ( (This)->lpVtbl -> readArchiveValue(This,tagName,time,tagValue) ) 

#define IPmcDB_readSnapshots(This,count,tagNames,tagValues)	\
    ( (This)->lpVtbl -> readSnapshots(This,count,tagNames,tagValues) ) 

#define IPmcDB_writeSnapshots(This,count,tagNames,tagValues)	\
    ( (This)->lpVtbl -> writeSnapshots(This,count,tagNames,tagValues) ) 

#define IPmcDB_readArchiveValues(This,tagNames,startTime,interval,count,tagValues)	\
    ( (This)->lpVtbl -> readArchiveValues(This,tagNames,startTime,interval,count,tagValues) ) 

#define IPmcDB_sleep(This,miliSeconds)	\
    ( (This)->lpVtbl -> sleep(This,miliSeconds) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPmcDB_INTERFACE_DEFINED__ */



#ifndef __libpmcdbLib_LIBRARY_DEFINED__
#define __libpmcdbLib_LIBRARY_DEFINED__

/* library libpmcdbLib */
/* [custom][version][uuid] */ 


EXTERN_C const IID LIBID_libpmcdbLib;

EXTERN_C const CLSID CLSID_CompReg;

#ifdef __cplusplus

class DECLSPEC_UUID("DBBDBA84-B3BC-4AF9-A532-D595FDB5ED22")
CompReg;
#endif

EXTERN_C const CLSID CLSID_PmcDB;

#ifdef __cplusplus

class DECLSPEC_UUID("6CE09E7F-B339-47B1-9449-2CF3E94C1A65")
PmcDB;
#endif
#endif /* __libpmcdbLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


