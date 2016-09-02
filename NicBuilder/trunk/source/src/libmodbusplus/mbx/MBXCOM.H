/*============================================================================
*
*       +============================================================+
*       |      Copyright (C) 1997, Cyberlogic Technologies Inc.	     |
*       +============================================================+
*
* Module Name :
*       MbxCom.h
* 
* Abstract:
*       This file provides all common definitions that are needed by both, the
*       CLMbx kernel mode device driver as well as the MbxApi DLL.
*
* Author:
*       Paul Mikulski
* 
* ---------+-----+-----------------------------------------------------------
*   DATE   | BY  |  DESCRIPTION / REASON FOR MODIFICATION
* ---------+-----+-----------------------------------------------------------
* 06-10-97 | PM  |  Start of development
* ---------+-----+-----------------------------------------------------------
* ..-..-97 |     |
*==========+=====+==========================================================*/
#ifndef _MBXCOM_H_
#define _MBXCOM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "MbxError.h"   // Include error codes definition file

//
// The following structures must use 1 byte packing!!!.
//
#pragma pack(push,1)

#include "MbxGnPkt.h"           // Include generic Modbus packet definition file

#ifndef _MBXPKT_
#define _MBXPKT_
    
//==============================
// Modbus packet union
//==============================
typedef union
{
    MBXGENPKT       GenMb;          // Generic Modbus packet

    // The above elements of this union must always be present!!!
    // If you want to add your own message structures, define your
    // own MBXPKT union. Make sure that it is defined before
    // this file is included and also define the _MBXPKT_ symbol.
}
MBXPKT, *PMBXPKT;
#endif  // _MBXPKT_

// Combined routing array and
// command packet structure
//
typedef struct tagMBXSPPKT
{
    ULONG       dwRoutingSize;      // Routing array size in bytes
    ULONG       dwCmdPktSize;       // Command packet size in bytes
    UCHAR       Data[1];            // Routing array followed by MBXPKT type packet
}
MBXSPPKT,*PMBXSPPKT;

//
// Mudbus Plus device statistics buffer
//
typedef struct tagMBXDEVSTATISTICS
{
    UCHAR               byDiag1;    // Diagnostic pattern set to 0xAA
    UCHAR               byDiag2;    // Diagnostic pattern set to 0x55
    MBPSTATISTICS       MbpStat;    // Peer processor status buffer
}
MBXDEVSTATISTICS,*PMBXDEVSTATISTICS;

// Put this after the last structure definition that
// needs 1 byte packing. This will restore the original
// packing.
#pragma pack(pop)

//
// Examples of using the MBXPKT union:
//
//  MBXPKT  pkt;
//
//  pkt.GenMb.Fnc = 3;
//  pkt.GenMb.Data[0] = 5;
//

#define PATHTYPE_DX   0     // MB+ DM/DS path
#define PATHTYPE_PX   1     // MB+ PM/PS path

// Path ID structure
//
typedef struct tagMBXPATHID
{
    USHORT  wPath;          // DM/PM/DS/PS path number
    USHORT  wPathType;      // 0 = MB+ DM/DS path, 1 = MB+ PM/PS path,
}
MBXPATHID,*PMBXPATHID;

// Define the I/O bus interface types.
//
typedef enum _MBX_INTERFACE_TYPE {
    MbxInterfaceTypeUndefined = -1,
    MbxInterfaceInternal,
    MbxInterfaceIsa,
    MbxInterfaceEisa,
    MbxInterfaceMicroChannel,
    MbxInterfaceTurboChannel,
    MbxInterfacePCIBus,
    MbxInterfaceVMEBus,
    MbxInterfaceNuBus,
    MbxInterfacePCMCIABus,
    MbxInterfaceCBus,
    MbxInterfaceMPIBus,
    MbxInterfaceMPSABus,
    MbxInterfaceProcessorInternal,
    MbxInterfaceInternalPowerBus,
    MbxInterfacePNPISABus,
    MbxInterfaceMaximumInterfaceType
}MBX_INTERFACE_TYPE, *PMBX_INTERFACE_TYPE;

//
// Protocol ID codes
//
#define MBXNET_NONE         0           // Not configured network protocol ID code
#define MBXNET_MB           1           // MB network protocol ID code
#define MBXNET_MBP          2           // MB+ network protocol ID code
#define MBXNET_TCPIP        3           // TCP/IP network protocol ID code

//
// Adapter card ID codes
//
#define MBXMOD_NONE                 0   // Id code for not configured adapter card
#define MBXMOD_SA85                 1   // Id code for SA85
#define MBXMOD_AT984                2   // Id code for AT984
#define MBXMOD_SM85                 3   // Id code for SM85
#define MBXMOD_MC984                4   // Id code for MC984
#define MBXMOD_SA85D                5   // Id code for SA85-DUAL
#define MBXMOD_PCMCIA_416NHM21200   6   // Id code for PCMCIA 416NHM21200
#define MBXMOD_TCPIP				7   // Id code for a TCP/IP device

//
// Device driver configuration structure
//
typedef struct tagMBXDEVCFG
{
    ULONG   dwAdapterID;        // Adapter card model (MBXMOD_SA85, etc.)
    ULONG   dwProtocolID;       // Device protocol (MBXNET_MBP, etc.)
    ULONG   dwDeviceNumber;     // Device (adapter card) number (0,1,2,...)

    ULONG   dwMemoryAddress;    // Adapter board base address
    ULONG   dwMemorySize;       // Adapter board memory size in bytes
    ULONG   dwIoPortAddress;    // I/O port address
    ULONG   dwIoPortSize;       // I/O port range in bytes
    ULONG   dwBusIRQLevel;      // Adapter board's IRQ level
    ULONG   dwPollingInterval;  // Polling interval

    ULONG   dwBusNumber;        // Bus number
    ULONG   dwInterfaceType;    // Isa, Eisa, MicroChannel ...
    ULONG   dwSlotNumber;       // MCA only: slot number of card
                                // note: user gives 1 to 8 but
                                // system registers use 0 to 7
    ULONG   dwBaudRate;         // MB/MB+ baud rate
    ULONG   dwMaxNodes;         // Max number of nodes
    ULONG   dwNodeAddress;      // Adapter board's node address

    ULONG   dwSpare[2];         // Spare
}
MBXDEVCFG,*PMBXDEVCFG;

//
// Device driver diagnostic counters structure
//
#define MBXFLG_ADAPTER_ONLINE       0x00000001  // Adapter card is on line flag
#define MBXFLG_ADAPTER_FAULTED      0x00000002  // Adapter card is faulted flag
#define MBXFLG_ADAPTER_DEAD         0x00000004  // Adapter card is dead flag

typedef struct tagMBXDEVDIAG
{
    ULONG   dwStatusFlags;          // Adapter status flag bits (See MBXFLG_xxx bits)

    ULONG   dwDevOpenCount;         // Total # of device driver open's

    ULONG   dwDmOpenCount;          // Total # of opened DM paths
    ULONG   dwDsOpenCount;          // Total # of opened DS paths
    ULONG   dwPmOpenCount;          // Total # of opened PM paths
    ULONG   dwPsOpenCount;          // Total # of opened PS paths

    ULONG   dwPendingDmReqCount;    // Current # of pending DM path requests
    ULONG   dwPendingDsReqCount;    // Current # of pending DS path requests
    ULONG   dwPendingPmReqCount;    // Current # of pending PM path requests
    ULONG   dwPendingPsReqCount;    // Current # of pending PS path requests

    ULONG   dwPendingGlbWrReqCount; // Current # of pending global write requests
    ULONG   dwPendingGlbRdReqCount; // Current # of pending global read requests
    ULONG   dwPendingDevStatReqCount;//Current # of pending device statistics requests

    ULONG   dwAdapterFifoReqCount;  // Number of requests in adapter card FIFO
    ULONG   dwTotalInterruptCount;  // Total # of interrupts
    ULONG   dwTotalLostInterrupts;  // Total # of lost interrupts
    ULONG   dwTotalXmtPktCount;     // Total # of transmitted packets
    ULONG   dwTotalRcvPktCount;     // Total # of received packets

    ULONG   dwTotalDmCmdPkt;        // Total # of DM command packets
    ULONG   dwTotalDmReplyPkt;      // Total # of DM reply packets
    ULONG   dwTotalDmCmdTimeouts;   // Total # of no replies to DM cmd packets

    ULONG   dwTotalDsCmdPkt;        // Total # of DS command packets
    ULONG   dwTotalDsReplyPkt;      // Total # of DS reply packets
    ULONG   dwTotalLostDsCmdPkt;    // Total # of lost DS command packets in all paths

    ULONG   dwTotalPmCmdPkt;        // Total # of PM command packets
    ULONG   dwTotalPmReplyPkt;      // Total # of PM reply packets
    ULONG   dwTotalPmCmdTimeouts;   // Total # of no replies to PM cmd packets

    ULONG   dwTotalPsCmdPkt;        // Total # of PS command packets
    ULONG   dwTotalPsReplyPkt;      // Total # of PS reply packets
    ULONG   dwTotalLostPsCmdPkt;    // Total # of lost PS command packets in all paths

    ULONG   dwTotalGlbWrCount;      // Total # of global write requests
    ULONG   dwTotalGlbRdCount;      // Total # of global read requests
    ULONG   dwTotalDevStatCount;    // Total # of device statistics requests
    
    ULONG   dwTotalDevDriverCalls;  // Total # of calls to the device driver
    ULONG   dwTotalDevErrorCalls;   // Total # of calls to the device driver
                                    // with error return
    ULONG   dwTotalAdapterFaults;   // Total # of adapter board faults
    ULONG   dwLastCrashCode;        // Last crash code
    ULONG   dwSpare[2];             // Spare counters
}
MBXDEVDIAG,*PMBXDEVDIAG;

#ifdef __cplusplus
}
#endif

#endif //_MBXCOM_H_
