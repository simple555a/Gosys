/*============================================================================
*
*       +============================================================+
*       |      Copyright (C) 1997, CYBERLOGIC Technologies Inc.      |
*       +============================================================+
*
* Module Name :
*       MbxIoErr.h
*       (derived from MbxIoErr.mc by the message compiler  )
* 
* Abstract:
*       MBXAPI specific error message definition file.
*
* Author:
*       Paul Mikulski
* 
* ---------+-----+-----------------------------------------------------------
*   DATE   | BY  |  DESCRIPTION / REASON FOR MODIFICATION
* ---------+-----+-----------------------------------------------------------
* 02-05-97 | PM  |  Start of development
* ---------+-----+-----------------------------------------------------------
* ..-..-97 |     |
*==========+=====+==========================================================*/
#ifndef _MBXIOERR_H_
#define _MBXIOERR_H_
//
//
//     Modicon MBX Driver I/O Status error codes and messages
//
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: MBX_BAD_MP_PATHID
//
// MessageText:
//
//  Invalid master path ID.
//
#define MBX_BAD_MP_PATHID                ((DWORD)0xC0000001L)

//
//
// MessageId: MBX_BAD_SP_PATHID
//
// MessageText:
//
//  Invalid slave path ID.
//
#define MBX_BAD_SP_PATHID                ((DWORD)0xC0000002L)

//
//
// MessageId: MBX_OLD_VERSION
//
// MessageText:
//
//  This program was written for an older version of MBXAPI.DLL.
//
#define MBX_OLD_VERSION                  ((DWORD)0xC0000003L)

//
#endif // _MBXIOERR_H_
