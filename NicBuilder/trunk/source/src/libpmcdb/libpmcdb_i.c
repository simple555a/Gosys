

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IPmcDB,0x5D2076E3,0x68A4,0x4209,0xB9,0x65,0x45,0xD4,0x8E,0x28,0xC6,0x5A);


MIDL_DEFINE_GUID(IID, LIBID_libpmcdbLib,0x0E2A80B6,0x5F30,0x46A4,0x9D,0x25,0x79,0xD9,0x5A,0xD4,0x15,0x4E);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0xDBBDBA84,0xB3BC,0x4AF9,0xA5,0x32,0xD5,0x95,0xFD,0xB5,0xED,0x22);


MIDL_DEFINE_GUID(CLSID, CLSID_PmcDB,0x6CE09E7F,0xB339,0x47B1,0x94,0x49,0x2C,0xF3,0xE9,0x4C,0x1A,0x65);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



