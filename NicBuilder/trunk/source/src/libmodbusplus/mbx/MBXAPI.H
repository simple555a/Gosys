/*============================================================================
*
*       +============================================================+
*       |      Copyright (C) 1997, CYBERLOGIC Technologies Inc.      |
*       +============================================================+
*
* Module Name :
*       MbxApi.h 
* 
* Abstract:
*       This file defines an API to the MBXAPI.DLL library module for programs
*       written in C and C++. Every program that makes use of this DLL must
*       include this file.
*
* Author:
*       Paul Mikulski
* 
* ---------+-----+-----------------------------------------------------------
*   DATE   | BY  |  DESCRIPTION / REASON FOR MODIFICATION
* ---------+-----+-----------------------------------------------------------
* 02-04-97 | PM  |  Start of development
* ---------+-----+-----------------------------------------------------------
* ..-..-97 |     |
*==========+=====+==========================================================*/
#ifndef _MBXAPI_H_
#define _MBXAPI_H_

//
// Define API decoration for direct importing of DLL references.
//
#ifdef _DLLEXPORT_MBXAPI_
#define MBXAPI
#else
#define MBXAPI DECLSPEC_IMPORT
#endif // _DLLEXPORT_MBXAPI_

#ifdef __cplusplus
extern "C" {
#endif

#define MBX_MAX_DEVICE_NAME         32      // Max size of DeviceName
#define MBX_MAX_DEVICE_DESCRIPTION  256     // Max size of DeviceDescription
    
#include "MbxCom.h"     // Include common driver definition file

//==========================================================================
// Function prototypes
//==========================================================================

typedef VOID (WINAPI *PFMBXCALLBACK)(
    DWORD   dwIoStatus,             // Completion code 
    DWORD   dwParameter             // User parameter
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxOpenDeviceDescriptionEnum(                        
    OUT PHANDLE     phEnum          // Device description enumerator handle
    );                          

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxCloseDeviceDescriptionEnum(                       
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxResetDeviceDescriptionEnum(
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxDeviceDescriptionEnumNext(
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadDeviceDescriptionA(                        
    IN HANDLE       hEnum,          // Handle from MbxOpenDeviceDescriptionEnum()
    IN OUT PBOOL    pbPhysDevice,   // Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    OUT LPSTR       pszNetworkProtocol,// Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetworkProtocolSize,// Network protocol string size in bytes
    OUT LPSTR       pszAdapterCard,// Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPSTR       pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPSTR       pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );                          

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadDeviceDescriptionW(                        
    IN HANDLE       hEnum,          // Handle from MbxOpenDeviceDescriptionEnum()
    IN OUT PBOOL    pbPhysDevice,   // Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    OUT LPWSTR      pszNetworkProtocol,// Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetworkProtocolSize,// Network protocol string size in bytes
    OUT LPWSTR      pszAdapterCard,// Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPWSTR      pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPWSTR      pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );                          

#ifdef UNICODE
#define MbxReadDeviceDescription  MbxReadDeviceDescriptionW
#else
#define MbxReadDeviceDescription  MbxReadDeviceDescriptionA
#endif // !UNICODE

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadPhysicalDeviceDescriptionA(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPSTR       pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPSTR       pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPSTR       pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPSTR       pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadPhysicalDeviceDescriptionW(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPWSTR      pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPWSTR      pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPWSTR      pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPWSTR      pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxOpen(
    IN  DWORD       dwDeviceNumber, // Device (adapter card) number (0,1,2,...)
    OUT PHANDLE     phDevice        // Ptr to device driver handle
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxClose(
    IN  HANDLE  hDevice             // Device driver handle from MbxOpen()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxOpenMp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN OUT PMBXPATHID pPathID       // Pointer to DM/PM path ID
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxCloseMp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DM/PM path ID from MbxOpenMp()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWriteMpCmdWait(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN OUT PDWORD   pdwReplyPktSize,// Ptr to reply packet size in bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWriteMpCmd(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN  DWORD       dwReplyPktSize, // Reply packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWriteMpCmdCB(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  DWORD       dwParameter,    // Parameter to be passed to the CallBack function
    IN  PFMBXCALLBACK   pCallBack   // Ptr to completion callback routine
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadMpReply(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN OUT PDWORD   pdwReplyPktSize // Ptr to reply packet size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxAbortMp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DM/PM path ID from MbxOpenMp()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxOpenSp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxCloseSp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID as used in MbxOpenSp()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWaitForSpCmd(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT PDWORD      pdwCmdPktSize,  // Ptr to command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWaitForSpCmdCB(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  DWORD       dwParameter,    // Parameter to be passed to the CallBack function
    IN  PFMBXCALLBACK   pCallBack   // Ptr to completion callback routine
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadSpCmdWait(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes + msg source
    IN OUT PDWORD   pdwRoutingSize, // Ptr to routing array size in bytes
    OUT PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN OUT PDWORD   pdwCmdPktSize,  // Ptr to command packet size in bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadSpCmd(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT PMBXSPPKT   pCmdSpPkt,      // Ptr to combined routing array and command packet
    IN  DWORD       dwCmdSpPktSize, // Command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWriteSpReply(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN  DWORD       dwReplyPktSize  // Reply packet size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxAbortSp(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID as used in MbxOpenSp()
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxSetSpLogin(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  UCHAR       byStatus        // 1 = set login, 0 = clear login
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxWriteGlobalData(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  BOOL        bSetMaxSize,    // If TRUE, set the max size of global data array
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    IN  PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN  DWORD       dwByteCount     // Number of global data bytes to write
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadGlobalDataWait(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode,          // Node number
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    OUT PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN OUT PDWORD   pdwByteCount,   // Pointer to number of global data bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadGlobalData(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode,          // Node number
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    OUT PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN  DWORD       dwByteCount,    // Number of global data bytes to be read
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxAbortReadGlobalData(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode           // Node number
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxGetOverlappedResult(
    IN HANDLE       hDevice,        // Device driver handle from MbxOpen()
    IN LPOVERLAPPED lpOverlapped,   // Address of overlapped structure
    OUT PDWORD      pdwBytesTransferred,// Address of actual bytes count
    BOOL            bWait           // Wait flag
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxDevStatistics(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PUCHAR      pStsBuf,        // Pointer to network statistics buffer
    IN OUT PDWORD   pdwStsBufSize   // Pointer to length of buffer in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadDeviceConfig(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PMBXDEVCFG  pDevCfg,        // Ptr to device driver configuration structure
    IN OUT PDWORD   pdwDevCfgSize   // Ptr to driver configuration structure size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxReadDeviceDiagnostics(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PMBXDEVDIAG pDevDiag,       // Ptr to driver diagnostics structure
    IN OUT PDWORD   pdwDevDiagSize  // Ptr to driver diag counters structure size in bytes
    );

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxGetErrorMessageA(
    IN  DWORD       dwIoStatus,     // I/O status code
    IN  DWORD       dwLanguageId,   // language identifier for requested message 
    OUT LPSTR       pszMsgBuf,      // Pointer to message buffer 
    IN OUT PDWORD   pdwMsgBufSize   // Size of message buffer 
    );	

MBXAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
MbxGetErrorMessageW(
    IN  DWORD       dwIoStatus,     // I/O status code
    IN  DWORD       dwLanguageId,   // language identifier for requested message 
    OUT LPWSTR      pszMsgBuf,      // Pointer to message buffer 
    IN OUT PDWORD   pdwMsgBufSize   // Size of message buffer 
    );	

#ifdef UNICODE
#define MbxGetErrorMessage  MbxGetErrorMessageW
#else
#define MbxGetErrorMessage  MbxGetErrorMessageA
#endif // !UNICODE

//***************************************************************************
// The following typedefs define data types of pointers to the API functions
//***************************************************************************
typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxOpenDeviceDescriptionEnum)(
    OUT PHANDLE     phEnum          // Device description enumerator handle
    );                          

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxCloseDeviceDescriptionEnum)(
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxResetDeviceDescriptionEnum)(
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxDeviceDescriptionEnumNext)(
    IN  HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadDeviceDescriptionA)(
    IN HANDLE       hEnum,          // Handle from MbxOpenDeviceDescriptionEnum()
    IN OUT PBOOL    pbPhysDevice,   // Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    OUT LPSTR       pszNetworkProtocol,// Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetworkProtocolSize,// Network protocol string size in bytes
    OUT LPSTR       pszAdapterCard,// Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPSTR       pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPSTR       pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );                          

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadDeviceDescriptionW)(
    IN  HANDLE      hEnum,          // Handle from MbxOpenDeviceDescriptionEnum()
    IN OUT PBOOL    pbPhysDevice,   // Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    OUT LPWSTR      pszNetworkProtocol,// Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetworkProtocolSize,// Network protocol string size in bytes
    OUT LPWSTR      pszAdapterCard,// Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPWSTR      pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPWSTR      pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );                          

#ifdef UNICODE
#define PMbxReadDeviceDescription  PMbxReadDeviceDescriptionW
#else
#define PMbxReadDeviceDescription  PMbxReadDeviceDescriptionA
#endif // !UNICODE

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadPhysicalDeviceDescriptionA)(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPSTR       pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPSTR       pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPSTR       pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPSTR       pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadPhysicalDeviceDescriptionW)(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPWSTR      pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPWSTR      pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPWSTR      pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPWSTR      pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxOpen)(
    IN  DWORD       dwDeviceNumber, // Device (adapter card) number (0,1,2,...)
    OUT PHANDLE     phDevice        // Ptr to device driver handle
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxClose)(
    IN  HANDLE  hDevice             // Device driver handle from MbxOpen()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxOpenMp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN OUT PMBXPATHID pPathID       // Pointer to DM/PM path ID
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxCloseMp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DM/PM path ID from MbxOpenMp()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWriteMpCmdWait)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN OUT PDWORD   pdwReplyPktSize,// Ptr to reply packet size in bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWriteMpCmd)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN  DWORD       dwReplyPktSize, // Reply packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWriteMpCmdCB)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    IN  USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes
    IN  DWORD       dwRoutingSize,  // Routing array size in bytes
    IN  PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN  DWORD       dwCmdPktSize,   // Command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  DWORD       dwParameter,    // Parameter to be passed to the CallBack function
    IN  PFMBXCALLBACK   pCallBack   // Ptr to completion callback routine
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadMpReply)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DM/PM path ID from MbxOpenMp()
    OUT PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN OUT PDWORD   pdwReplyPktSize // Ptr to reply packet size in bytes
   );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxAbortMp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DM/PM path ID from MbxOpenMp()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxOpenSp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxCloseSp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID as used in MbxOpenSp()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWaitForSpCmd)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT PDWORD      pdwCmdPktSize,  // Ptr to command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWaitForSpCmdCB)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  DWORD       dwParameter,    // Parameter to be passed to the CallBack function
    IN  PFMBXCALLBACK   pCallBack   // Ptr to completion callback routine
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadSpCmdWait)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT USHORT      Routing[],      // Ptr to array of 5 (16-bit) routing codes + msg source
    IN OUT PDWORD   pdwRoutingSize, // Ptr to routing array size in bytes
    OUT PMBXPKT     pCmdPkt,        // Ptr to command packet
    IN OUT PDWORD   pdwCmdPktSize,  // Ptr to command packet size in bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadSpCmd)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    OUT PMBXSPPKT   pCmdSpPkt,      // Ptr to combined routing array and command packet
    IN  DWORD       dwCmdSpPktSize, // Command packet size in bytes
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWriteSpReply)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  PMBXPKT     pReplyPkt,      // Ptr to reply packet
    IN  DWORD       dwReplyPktSize  // Reply packet size in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxAbortSp)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID          // DS/PS path ID as used in MbxOpenSp()
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxSetSpLogin)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  MBXPATHID   PathID,         // DS/PS path ID as used in MbxOpenSp()
    IN  UCHAR       byStatus        // 1 = set login, 0 = clear login
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxWriteGlobalData)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  BOOL        bSetMaxSize,    // If TRUE, set the max size of global data array
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    IN  PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN  DWORD       dwByteCount     // Number of global data bytes to write
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadGlobalDataWait)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode,          // Node number
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    OUT PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN OUT PDWORD   pdwByteCount,   // Pointer to number of global data bytes
    IN  DWORD       dwTimeout       // Time-out in milliseconds
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadGlobalData)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode,          // Node number
    IN  DWORD       dwStartOffset,  // Start byte offset into global data buffer
    OUT PUCHAR      pGlbBuf,        // Pointer to global data buffer
    IN  DWORD       dwByteCount,    // Number of global data bytes to be read
    IN  DWORD       dwTimeout,      // Time-out in milliseconds
    IN  LPOVERLAPPED lpOverlapped   // Points to an OVERLAPPED structure
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxAbortReadGlobalData)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    IN  USHORT      wNode           // Node number
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxGetOverlappedResult)(
    IN HANDLE       hDevice,        // Device driver handle from MbxOpen()
    IN LPOVERLAPPED lpOverlapped,   // Address of overlapped structure
    OUT PDWORD      pdwBytesTransferred,// Address of actual bytes count
    BOOL            bWait           // Wait flag
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxDevStatistics)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PUCHAR      pStsBuf,        // Pointer to network statistics buffer
    IN OUT PDWORD   pdwStsBufSize   // Pointer to length of buffer in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadDeviceConfig)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PMBXDEVCFG  pDevCfg,        // Ptr to device driver configuration structure
    IN OUT PDWORD   pdwDevCfgSize   // Ptr to driver configuration structure size in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxReadDeviceDiagnostics)(
    IN  HANDLE      hDevice,        // Device driver handle from MbxOpen()
    OUT PMBXDEVDIAG pDevDiag,       // Ptr to driver diagnostics structure
    IN OUT PDWORD   pdwDevDiagSize  // Ptr to driver diag counters structure size in bytes
    );

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxGetErrorMessageA)(
    IN  DWORD       dwIoStatus,     // I/O status code
    IN  DWORD       dwLanguageId,   // language identifier for requested message 
    OUT LPSTR       pszMsgBuf,      // Pointer to message buffer 
    IN OUT PDWORD   pdwMsgBufSize   // Size of message buffer 
    );	

typedef DWORD                       // MBX_SUCCESS if successful, else error code
(WINAPI *PMbxGetErrorMessageW)(
    IN  DWORD       dwIoStatus,     // I/O status code
    IN  DWORD       dwLanguageId,   // language identifier for requested message 
    OUT LPWSTR      pszMsgBuf,      // Pointer to message buffer 
    IN OUT PDWORD   pdwMsgBufSize   // Size of message buffer 
    );	

#ifdef UNICODE
#define PMbxGetErrorMessage  PMbxGetErrorMessageW
#else
#define PMbxGetErrorMessage  PMbxGetErrorMessageA
#endif // !UNICODE

// Logical Device function table ordinals
//
typedef enum tagMBXLOGDEVFNC {
    LogDev_MbxOpen = 1,                         // Ordinal # 1
    LogDev_MbxClose,                            // Ordinal # 2
    LogDev_MbxOpenMp,                           // Ordinal # 3
    LogDev_MbxCloseMp,                          // Ordinal # 4
    LogDev_MbxWriteMpCmdWait,                   // Ordinal # 5
    LogDev_MbxWriteMpCmd,                       // Ordinal # 6
    LogDev_MbxWriteMpCmdCB,                     // Ordinal # 7
    LogDev_MbxReadMpReply,                      // Ordinal # 8
    LogDev_MbxAbortMp,                          // Ordinal # 9
    LogDev_MbxOpenSp,                           // Ordinal # 10
    LogDev_MbxCloseSp,                          // Ordinal # 11
    LogDev_MbxWaitForSpCmd,                     // Ordinal # 12
    LogDev_MbxWaitForSpCmdCB,                   // Ordinal # 13
    LogDev_MbxAbortSp,                          // Ordinal # 14
    LogDev_MbxReadSpCmdWait,                    // Ordinal # 15
    LogDev_MbxReadSpCmd,                        // Ordinal # 16
    LogDev_MbxWriteSpReply,                     // Ordinal # 17
    LogDev_MbxSetSpLogin,                       // Ordinal # 18
    LogDev_MbxWriteGlobalData,                  // Ordinal # 19
    LogDev_MbxReadGlobalDataWait,               // Ordinal # 20
    LogDev_MbxReadGlobalData,                   // Ordinal # 21
    LogDev_MbxAbortReadGlobalData,              // Ordinal # 22
    LogDev_MbxGetOverlappedResult,              // Ordinal # 23
    LogDev_MbxDevStatistics,                    // Ordinal # 24
    LogDev_MbxReadDeviceConfig,                 // Ordinal # 25
    LogDev_MbxReadDeviceDiagnostics,            // Ordinal # 26
    LogDev_MbxReadPhysicalDeviceDescriptionA,   // Ordinal # 27
    LogDev_MbxReadPhysicalDeviceDescriptionW    // Ordinal # 28
}
MBXLOGDEVFNC; 

#define LogDev_LAST_ORDINAL LogDev_MbxReadPhysicalDeviceDescriptionW

// Logical Device function table structure
//
typedef struct tagMBXLOGDEVFNCTAB
{
    MBXLOGDEVFNC                        dwLastOrdinal;
    PMbxOpen                            MbxOpen;                    // Ordinal # 1 
    PMbxClose                           MbxClose;                   // Ordinal # 2 
    PMbxOpenMp                          MbxOpenMp;                  // Ordinal # 3 
    PMbxCloseMp                         MbxCloseMp;                 // Ordinal # 4 
    PMbxWriteMpCmdWait                  MbxWriteMpCmdWait;          // Ordinal # 5 
    PMbxWriteMpCmd                      MbxWriteMpCmd;              // Ordinal # 6 
    PMbxWriteMpCmdCB                    MbxWriteMpCmdCB;            // Ordinal # 7 
    PMbxReadMpReply                     MbxReadMpReply;             // Ordinal # 8 
    PMbxAbortMp                         MbxAbortMp;                 // Ordinal # 9 
    PMbxOpenSp                          MbxOpenSp;                  // Ordinal # 10
    PMbxCloseSp                         MbxCloseSp;                 // Ordinal # 11
    PMbxWaitForSpCmd                    MbxWaitForSpCmd;            // Ordinal # 12
    PMbxWaitForSpCmdCB                  MbxWaitForSpCmdCB;          // Ordinal # 13
    PMbxAbortSp                         MbxAbortSp;                 // Ordinal # 14
    PMbxReadSpCmdWait                   MbxReadSpCmdWait;           // Ordinal # 15
    PMbxReadSpCmd                       MbxReadSpCmd;               // Ordinal # 16
    PMbxWriteSpReply                    MbxWriteSpReply;            // Ordinal # 17
    PMbxSetSpLogin                      MbxSetSpLogin;              // Ordinal # 18
    PMbxWriteGlobalData                 MbxWriteGlobalData;         // Ordinal # 19
    PMbxReadGlobalDataWait              MbxReadGlobalDataWait;      // Ordinal # 20
    PMbxReadGlobalData                  MbxReadGlobalData;          // Ordinal # 21
    PMbxAbortReadGlobalData             MbxAbortReadGlobalData;     // Ordinal # 22
    PMbxGetOverlappedResult             MbxGetOverlappedResult;     // Ordinal # 23
    PMbxDevStatistics                   MbxDevStatistics;           // Ordinal # 24
    PMbxReadDeviceConfig                MbxReadDeviceConfig;        // Ordinal # 25
    PMbxReadDeviceDiagnostics           MbxReadDeviceDiagnostics;   // Ordinal # 26
    PMbxReadPhysicalDeviceDescriptionA  MbxReadPhysicalDeviceDescriptionA; // Ordinal # 27
    PMbxReadPhysicalDeviceDescriptionW  MbxReadPhysicalDeviceDescriptionW; // Ordinal # 28
}
MBXLOGDEVFNCTAB,*PMBXLOGDEVFNCTAB;

#ifdef __cplusplus
}
#endif

#endif //_MBXAPI_H_
