

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Sep 28 21:20:43 2015
 */
/* Compiler settings for pmcdb_idl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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

#ifndef __pmcdb_idl_i_h__
#define __pmcdb_idl_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPmcDB_FWD_DEFINED__
#define __IPmcDB_FWD_DEFINED__
typedef interface IPmcDB IPmcDB;
#endif 	/* __IPmcDB_FWD_DEFINED__ */


#ifndef __IPmcDBEvents_FWD_DEFINED__
#define __IPmcDBEvents_FWD_DEFINED__
typedef interface IPmcDBEvents IPmcDBEvents;
#endif 	/* __IPmcDBEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_pmcdb_idl_0000_0000 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_pmcdb_idl_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_pmcdb_idl_0000_0000_v0_0_s_ifspec;

#ifndef __IPmcDB_INTERFACE_DEFINED__
#define __IPmcDB_INTERFACE_DEFINED__

/* interface IPmcDB */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPmcDB;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F35F7568-07D5-4137-A9C9-E8737A394536")
    IPmcDB : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE lock( 
            BOOL exclusive,
            LONG timeOut) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE unlock( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readSnapshot( 
            /* [in] */ BSTR tagName,
            /* [retval][out] */ VARIANT *tagValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE writeDevice( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readArchiveValue( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT time,
            /* [retval][out] */ VARIANT *tagValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readSnapshots( 
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [out][in] */ VARIANT *tagValues) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE writeSnapshots( 
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT tagValues) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE readArchiveValues( 
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT startTime,
            /* [in] */ LONG interval,
            /* [in] */ LONG count,
            /* [out][in] */ VARIANT *tagValues) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE sleep( 
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
            __RPC__deref_out  void **ppvObject);
        
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
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *lock )( 
            IPmcDB * This,
            BOOL exclusive,
            LONG timeOut);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *unlock )( 
            IPmcDB * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *readSnapshot )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [retval][out] */ VARIANT *tagValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *writeDevice )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValue )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT time,
            /* [retval][out] */ VARIANT *tagValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *readSnapshots )( 
            IPmcDB * This,
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [out][in] */ VARIANT *tagValues);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *writeSnapshots )( 
            IPmcDB * This,
            /* [in] */ LONG count,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT tagValues);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValues )( 
            IPmcDB * This,
            /* [in] */ VARIANT tagNames,
            /* [in] */ VARIANT startTime,
            /* [in] */ LONG interval,
            /* [in] */ LONG count,
            /* [out][in] */ VARIANT *tagValues);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *sleep )( 
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


#define IPmcDB_lock(This,exclusive,timeOut)	\
    ( (This)->lpVtbl -> lock(This,exclusive,timeOut) ) 

#define IPmcDB_unlock(This)	\
    ( (This)->lpVtbl -> unlock(This) ) 

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


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


