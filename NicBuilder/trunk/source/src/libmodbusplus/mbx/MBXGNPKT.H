/*============================================================================
*
*       +===========================================================+
*       |      Copyright (C) 1997, CYBERLOGIC Technologies Inc.	     |
*       +===========================================================+
*
* Module Name :
*       MbxGnPkt.h
* 
* Abstract:
*       This file provides definitions for all generic MB packets.
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
#ifndef _MBXGNPKT_H_
#define _MBXGNPKT_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// The following structures must use 1 byte packing!!!.
//
#pragma pack(push,1)

//
// Modbus Plus Network Statistics buffer structure
//
// (Refer to Modicon IBM Host Based Devices User's Guide, Get/Clear Network
// Statistics (Function 08, Subfunction 21), Cat. No.: GM-HBDS-001)
//
typedef struct tagMBPSTATISTICS
{   //                                  Word/Byte (hex)
    USHORT  wNodeId;                    //00/00 Node type ID:
                                        //      0   Unknown node type
                                        //      1   Standard PLC node
                                        //      2   Modbus bridge node
                                        //      3   PC Plus node
                                        //      4   Bridge Plus node
    USHORT  wPeerVersion;               //01/02 Communication processor version.
                                        //      Version 1.00 shows as 0100 hex.
    UCHAR   byNodeAddress;              //02/04 Network address for this station
    UCHAR   byNodeAddressHi;            //02/05 
    USHORT  wMacState;                  //03/06 MAC state variable:
                                        //      0   Power up state
                                        //      1   Monitor offline state
                                        //      2   Duplicate offline state
                                        //      3   Idle state
                                        //      4   Use token state
                                        //      5   Work response state
                                        //      6   Pass token state
                                        //      7   Solicit response state
                                        //      8   Check pass state
                                        //      9   Claim token state
                                        //      10  Claim response state
    USHORT  wPeerStatus;                //04/08 Peer status (LED code)
                                        //      0   Monitor link operation
                                        //      32  Normal link operation
                                        //      64  Never getting token
                                        //      96  Sole station
                                        //      128 Duplicate station
    USHORT  wTokenPassCounter;          //05/0A Token pass counter. Increments each
                                        //      time this station gets the token.
    USHORT  wTokenRotationTime;         //06/0C Token rotation time in ms.

    UCHAR   byDmFaildBitMap;            //07/0E Data master failed during token
                                        //      ownership bit map.
    UCHAR   byPmFailBitMap;             //07/0F Program master failed during token
                                        //      ownership bit map.
    UCHAR   byDmTokenOwnerBitMap;       //08/10 Data master token owner work bit map
    UCHAR   byPmTokenOwnerBitMap;       //08/11 Program master token owner work bit map

    UCHAR   byDsTokenOwnerBitMap;       //09/12 Data slave token owner work bit map
    UCHAR   byPsTokenOwnerBitMap;       //09/13 Program slave token owner work bit map

    UCHAR   byDmRspXferReqBitMap;       //0A/14 Data master/get master response
                                        //      transfer request bit map.
    UCHAR   byDsCmdXferReqBitMap;       //0A/15 Data slave/get slave command
                                        //      transfer request bit map.
    UCHAR   byPmRspXferReqBitMap;       //0B/16 Program master/get master response
                                        //      transfer request bit map.
    UCHAR   byPsCmdXferReqBitMap;       //0B/17 Program slave/get slave command
                                        //      transfer request bit map.
    UCHAR   byPmConnectStatusBitMap;    //0C/18 Program master connect status bit map
    UCHAR   byPsConnectStatusBitMap;    //0C/19 Program slave automatic logout
                                        //      request bit map.
    UCHAR   byPreXmtDefErrCounter;      //0D/1A Pretransmit deferral error counter
    UCHAR   byRcvBufDmaOvrErrCounter;   //0D/1B Receive buffer DMA overrun error counter

    UCHAR   byRepeatedCmdRcvCounter;    //0E/1C Repeated command received counter
    UCHAR   byFrameSizeErrCounter;      //0E/1D Frame size error counter

    UCHAR   byRcvCollisionErrCounter;   //0F/1E Receiver collision-abort error counter
    UCHAR   byRcvAlignErrCounter;       //0F/1F Receiver alignment error counter

    UCHAR   byRcvCrcErrCounter;         //10/20 Receiver CRC error counter
    UCHAR   byBadPktLenErrCounter;      //10/21 Bad packet length error counter

    UCHAR   byBadLinkAdrErrCounter;     //11/22 Bad link-address error counter
    UCHAR   byXmtBufDmaUnrErrCounter;   //11/23 Transmit buffer DMA-underrun error counter

    UCHAR   byBadIntPktLenErrCounter;   //12/24 Bad internal packet length error counter
    UCHAR   byBadMacFncErrCounter;      //12/25 Bad MAC function code error counter

    UCHAR   byCommRetryCounter;         //13/26 Communication retry counter
    UCHAR   byCommFailedErrCounter;     //13/27 Communication failed error counter

    UCHAR   byGoodRcvPktCounter;        //14/28 Good receive packet success counter
    UCHAR   byNoRspRcvErrCounter;       //14/29 No response received error counter

    UCHAR   byExcRspRcvErrCounter;      //15/2A Exception response received error counter
    UCHAR   byUnexpPathErrCounter;      //15/2B Unexpected path error counter

    UCHAR   byUnexpRspErrCounter;       //16/2C Unexpected response error counter
    UCHAR   byForgotTransErrCounter;    //16/2D Forgotten transaction error counter

    UCHAR   byActiveStationTable[8];    //17/2E Active station table bit map nodes 1-64

    UCHAR   byTokenStationTable[8];     //1B/36 Token station table bit map nodes 1-64

    UCHAR   byGlobalDataPresent[8];     //1F/3E Global data present table bit map nodes 1-64

    UCHAR   byRcvBufInUseBitMap[5];     //23/46 Receive buffer in use bit map, buf 1-40
    UCHAR   byStaMgrCmdProcIniCounter;  //25/4B Station management command processed
                                        //      initiation counter.
    UCHAR   byDmPathCmdIniCounter[8];   //26/4C Data master output path 1-8 command
                                        //      initiation counter.
    UCHAR   byDsPathCmdProcCounter[8];  //2A/54 Data slave input path 41-48 command
                                        //      processed counter.
    UCHAR   byPmPathCmdIniCounter[8];   //2E/5C Program master output path 81-88
                                        //      command initiation counter.
    UCHAR   byPsPathCmdProcCounter[8];  //32/64 Program slave input path C1-C8
                                        //      command processed counter.
}
MBPSTATISTICS,*PMBPSTATISTICS;

// Get/Clear Network Statistics (Function 08, Subfunction 21)
// command packet
//
// (Refer to Modicon IBM Host Based Devices User's Guide, Get/Clear Network
// Statistics (Function 08, Subfunction 21), Cat. No.: GM-HBDS-001)
//
typedef struct tagMBXGETCLRSTATCMD
{
    UCHAR       Fnc;                    // Function code (0x08)
    UCHAR       SubFncHi;               // Subfunction high byte (0x00)
    UCHAR       SubFncLo;               // Subfunction low byte (0x15)
    UCHAR       CmdHi;                  // Command high byte (0x00)
    UCHAR       CmdLo;                  // Command low byte (0x03/0x04)
}
MBXGETCLRSTATCMD,*PMBXGETCLRSTATCMD;

// Get/Clear Network Statistics (Function 08, Subfunction 21)
// response packet
//
// (Refer to Modicon IBM Host Based Devices User's Guide, Get/Clear Network
// Statistics (Function 08, Subfunction 21), Cat. No.: GM-HBDS-001)
//
typedef struct tagMBXGETSTATRSP
{
    UCHAR           Fnc;                // Function code (0x08)
    UCHAR           SubFncHi;           // Subfunction high byte (0x00)
    UCHAR           SubFncLo;           // Subfunction low byte (0x15)
    UCHAR           CmdHi;              // Command high byte (0x00)
    UCHAR           CmdLo;              // Command low byte (0x03)
    MBPSTATISTICS   MbpStat;            // Statistics buffer
}
MBXGETSTATRSP,*PMBXGETSTATRSP;

//
// Generic MB command packet structure
// (Refer to "Modbus Commands for Modbus Plus" (Appendix F) of Modicon
// IBM Host Based Devices, User's Guide; Publication GM-HBDS-001 Rev. F)
//
#define MAX_GENPKT_DATA    252          // Max size of data block
typedef struct tagMBXGENPKT
{
    UCHAR       Fnc;                    // Function code
    UCHAR       Data[MAX_GENPKT_DATA];  // Variable packet data
}
MBXGENPKT,*PMBXGENPKT;
    
// Put this after the last structure definition that
// needs 1 byte packing. This will restore the original
// packing.
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //_MBXGNPKT_H_
