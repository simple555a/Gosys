

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Jan 19 16:31:03 2016
 */
/* Compiler settings for libhisdATL.idl:
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

#ifndef __libhisdATL_h_h__
#define __libhisdATL_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
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

#ifdef __cplusplus
extern "C"{
#endif 


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
            /* [out][in] */ VARIANT *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeDevice( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readArchiveValue( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT date,
            /* [out][in] */ VARIANT *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readArchiveValuesByCount( 
            /* [in] */ BSTR tag,
            /* [in] */ VARIANT sTime,
            /* [in] */ VARIANT eTime,
            /* [in] */ int valueCount,
            /* [out][in] */ VARIANT *tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeTag( 
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isAlarmTag( 
            /* [in] */ BSTR tagName,
            /* [out][in] */ VARIANT *alarm_sign) = 0;
        
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
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *lock )( 
            IPmcDB * This,
            /* [in] */ BOOL exclusive,
            /* [in] */ LONG timeOut);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *unlock )( 
            IPmcDB * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *readSnapshot )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [out][in] */ VARIANT *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeDevice )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValue )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT date,
            /* [out][in] */ VARIANT *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *readArchiveValuesByCount )( 
            IPmcDB * This,
            /* [in] */ BSTR tag,
            /* [in] */ VARIANT sTime,
            /* [in] */ VARIANT eTime,
            /* [in] */ int valueCount,
            /* [out][in] */ VARIANT *tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeTag )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [in] */ VARIANT tagValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isAlarmTag )( 
            IPmcDB * This,
            /* [in] */ BSTR tagName,
            /* [out][in] */ VARIANT *alarm_sign);
        
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

#define IPmcDB_readArchiveValue(This,tagName,date,tagValue)	\
    ( (This)->lpVtbl -> readArchiveValue(This,tagName,date,tagValue) ) 

#define IPmcDB_readArchiveValuesByCount(This,tag,sTime,eTime,valueCount,tagValue)	\
    ( (This)->lpVtbl -> readArchiveValuesByCount(This,tag,sTime,eTime,valueCount,tagValue) ) 

#define IPmcDB_writeTag(This,tagName,tagValue)	\
    ( (This)->lpVtbl -> writeTag(This,tagName,tagValue) ) 

#define IPmcDB_isAlarmTag(This,tagName,alarm_sign)	\
    ( (This)->lpVtbl -> isAlarmTag(This,tagName,alarm_sign) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




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


