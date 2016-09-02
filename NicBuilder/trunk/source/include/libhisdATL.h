/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jul 12 09:23:37 2012
 */
/* Compiler settings for C:\proj\pmc\INTERFACES\libhisdATL\libhisdATL.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __libhisdATL_h__
#define __libhisdATL_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPmcDB_FWD_DEFINED__
#define __IPmcDB_FWD_DEFINED__
typedef interface IPmcDB IPmcDB;
#endif 	/* __IPmcDB_FWD_DEFINED__ */


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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IPmcDB_INTERFACE_DEFINED__
#define __IPmcDB_INTERFACE_DEFINED__

/* interface IPmcDB */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPmcDB;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("71F0209D-AFD9-4878-942B-C20FCA682411")
    IPmcDB : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE lock( 
            /* [in] */ BOOL exclusive,
            /* [in] */ LONG timeOut) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE unlock( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readSnapshot( 
            /* [in] */ BSTR tagName,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeDevice( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readArchiveValue( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT date,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readArchiveValuesByCount( 
            /* [in] */ BSTR tag,
            /* [in] */ VARIANT sTime,
            /* [in] */ VARIANT eTime,
            /* [in] */ int valueCount,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeTag( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPmcDBVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPmcDB __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPmcDB __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPmcDB __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *lock )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BOOL exclusive,
            /* [in] */ LONG timeOut);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unlock )( 
            IPmcDB __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *readSnapshot )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BSTR tagName,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeDevice )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *readArchiveValue )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT date,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *readArchiveValuesByCount )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BSTR tag,
            /* [in] */ VARIANT sTime,
            /* [in] */ VARIANT eTime,
            /* [in] */ int valueCount,
            /* [out][in] */ VARIANT __RPC_FAR *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeTag )( 
            IPmcDB __RPC_FAR * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        END_INTERFACE
    } IPmcDBVtbl;

    interface IPmcDB
    {
        CONST_VTBL struct IPmcDBVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPmcDB_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPmcDB_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPmcDB_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPmcDB_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPmcDB_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPmcDB_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPmcDB_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPmcDB_lock(This,exclusive,timeOut)	\
    (This)->lpVtbl -> lock(This,exclusive,timeOut)

#define IPmcDB_unlock(This)	\
    (This)->lpVtbl -> unlock(This)

#define IPmcDB_readSnapshot(This,tagName,tagValue)	\
    (This)->lpVtbl -> readSnapshot(This,tagName,tagValue)

#define IPmcDB_writeDevice(This,tagName,tagValue)	\
    (This)->lpVtbl -> writeDevice(This,tagName,tagValue)

#define IPmcDB_readArchiveValue(This,tagName,date,tagValue)	\
    (This)->lpVtbl -> readArchiveValue(This,tagName,date,tagValue)

#define IPmcDB_readArchiveValuesByCount(This,tag,sTime,eTime,valueCount,tagValue)	\
    (This)->lpVtbl -> readArchiveValuesByCount(This,tag,sTime,eTime,valueCount,tagValue)

#define IPmcDB_writeTag(This,tagName,tagValue)	\
    (This)->lpVtbl -> writeTag(This,tagName,tagValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_lock_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BOOL exclusive,
    /* [in] */ LONG timeOut);


void __RPC_STUB IPmcDB_lock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_unlock_Proxy( 
    IPmcDB __RPC_FAR * This);


void __RPC_STUB IPmcDB_unlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_readSnapshot_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BSTR tagName,
    /* [out][in] */ VARIANT __RPC_FAR *tagValue);


void __RPC_STUB IPmcDB_readSnapshot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_writeDevice_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BSTR tagName,
    /* [in] */ VARIANT tagValue);


void __RPC_STUB IPmcDB_writeDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_readArchiveValue_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BSTR tagName,
    /* [in] */ VARIANT date,
    /* [out][in] */ VARIANT __RPC_FAR *tagValue);


void __RPC_STUB IPmcDB_readArchiveValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_readArchiveValuesByCount_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BSTR tag,
    /* [in] */ VARIANT sTime,
    /* [in] */ VARIANT eTime,
    /* [in] */ int valueCount,
    /* [out][in] */ VARIANT __RPC_FAR *tagValue);


void __RPC_STUB IPmcDB_readArchiveValuesByCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPmcDB_writeTag_Proxy( 
    IPmcDB __RPC_FAR * This,
    /* [in] */ BSTR tagName,
    /* [in] */ VARIANT tagValue);


void __RPC_STUB IPmcDB_writeTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPmcDB_INTERFACE_DEFINED__ */



#ifndef __LIBHISDATLLib_LIBRARY_DEFINED__
#define __LIBHISDATLLib_LIBRARY_DEFINED__

/* library LIBHISDATLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LIBHISDATLLib;

EXTERN_C const CLSID CLSID_PmcDB;

#ifdef __cplusplus

class DECLSPEC_UUID("4366947A-0715-4219-920D-89C5C00E9F72")
PmcDB;
#endif
#endif /* __LIBHISDATLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
