

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Sep 21 23:01:31 2015
 */
/* Compiler settings for pmcsvr.idl:
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


#ifndef __pmcsvr_h__
#define __pmcsvr_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __OpcServer_FWD_DEFINED__
#define __OpcServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class OpcServer OpcServer;
#else
typedef struct OpcServer OpcServer;
#endif /* __cplusplus */

#endif 	/* __OpcServer_FWD_DEFINED__ */


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
#include "opcda.h"
#include "pmcdb.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PMCSVRLIB_LIBRARY_DEFINED__
#define __PMCSVRLIB_LIBRARY_DEFINED__

/* library PMCSVRLIB */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PMCSVRLIB;

EXTERN_C const CLSID CLSID_OpcServer;

#ifdef __cplusplus

class DECLSPEC_UUID("268F712B-CFC3-4D8A-96B4-706937B61668")
OpcServer;
#endif

EXTERN_C const CLSID CLSID_PmcDB;

#ifdef __cplusplus

class DECLSPEC_UUID("BA7CE1FB-E5DE-4ffc-8044-BF3053955FCF")
PmcDB;
#endif

#endif /* __PMCSVRLIB_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


