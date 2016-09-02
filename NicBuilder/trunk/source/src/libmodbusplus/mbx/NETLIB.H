/*============================================================================
*              Copyright (C) 1994, CYBERLOGIC Technologies Inc.
*=============================================================================
*
* Module Name :
*       netlib.h 
* 
* Abstract:
*       This file should be included by every module that makes calls to
*       functions in the netlib.obj interface library file.
*
* Author:
*       Paul Mikulski
* 
* ---------+-----+-----------------------------------------------------------
*   DATE   | BY  |  DESCRIPTION / REASON FOR MODIFICATION
* ---------+-----+-----------------------------------------------------------
* 03-09-94 | PM  |  Start of development
* ---------+-----+-----------------------------------------------------------
* ..-..-94 |     |
*==========+=====+==========================================================*/
#ifndef _NETLIB_H_
#define _NETLIB_H_

//
// Define API decoration for direct importing of DLL references.
//
#ifdef _DLLEXPORT_NETLIB_
#define NETLIBAPI
#else
#define NETLIBAPI DECLSPEC_IMPORT
#endif // _DLLEXPORT_NETLIB_


#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
/* Include files
/*==========================================================================*/
#ifndef NETBIOS_INCLUDED
#include "netbios.h"
#endif

/*==========================================================================*/
/* Function prototypes
/*==========================================================================*/
NETLIBAPI
int
APIENTRY
ncb_reset(
    int adaptno
    );

NETLIBAPI
int
APIENTRY
ncb_sa85off(
    int adaptno
    );

NETLIBAPI
int
APIENTRY
ncb_status(
    PNCB ncbp,
    int adaptno
    );

NETLIBAPI
int
APIENTRY
ncb_send(
    PNCB ncbp,
    int length,
    PCHAR buffer,
    UCHAR timeout
    );

NETLIBAPI
int
APIENTRY
ncb_receive_wait(
    PNCB ncbp,
    PCHAR buffer,
    UCHAR timeout
    );

NETLIBAPI
PNCB
APIENTRY
ncb_open(
    PCHAR name,
    int lan
    );

NETLIBAPI
int
APIENTRY
ncb_receive(
    PNCB ncbp,
    PCHAR buffer
    );

NETLIBAPI
int
APIENTRY
ncb_close(
    PNCB ncbp
    );

NETLIBAPI
int
APIENTRY
ncb_send_datagram(
    PNCB ncbp,
    int length,
    PCHAR buffer,
    UCHAR timeout,
    int adaptno
    );

NETLIBAPI
int
APIENTRY
ncb_receive_datagram(
    PNCB ncbp,
    int node,
    PCHAR buffer,
    UCHAR timeout,
    int adaptno
    );

NETLIBAPI
int
APIENTRY
ncb_cancel(
    PNCB ncbp
    );

NETLIBAPI
int
APIENTRY
ncb_set_slave_login(
    PNCB ncbp,
    UCHAR login_status
    );

NETLIBAPI
int
APIENTRY
ncb_set_sw_interrupt(
    int swInterrupt
    );

#ifdef __cplusplus
}
#endif

#endif //_NETLIB_H_
