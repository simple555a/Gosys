/*============================================================================
*              Copyright (C) 1994, CYBERLOGIC Technologies Inc.
*=============================================================================
*
* Module Name :
*       netbios.h 
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
#ifndef NETBIOS_INCLUDED
#define NETBIOS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//
// Structure of Network Control Block (NCB)
//
#ifndef NCB_INCLUDED
#define NCB_INCLUDED

typedef struct _NCB
{
    UCHAR   NCB_COMMAND;            // Command
    UCHAR   NCB_RETCODE;            // Function return code
    UCHAR   NCB_LSN;                // Local session number
    UCHAR   NCB_NUM;                // Number of network name
    PUCHAR  NCB_BUFFER;             // Far pointer to message buffer
    USHORT  NCB_LENGTH;             // Length of message buffer
    UCHAR   NCB_CALLNAME[16];       // Name of session user is talking to
    UCHAR   NCB_NAME[16];           // User's network name
    UCHAR   NCB_RTO;                // Receive time-out in 500 ms. incrs.
    UCHAR   NCB_STO;                // Send time-out - 500 ms. increments
    void  (*NCB_POST_ADDRESS) (struct _NCB_ *);// Address of "no-wait" interrupt call
    UCHAR   NCB_LANA_NUM;           // Adapter number (must be 0 or 1)
    UCHAR   NCB_CMD_CPLT;           // Command completion status
    UCHAR   NCB_RESERVE[14];        // Reserved area for Token-Ring
} NCB,*PNCB;
#else
//
// _NCB structure is already defined in nb30.h file.
// Microsoft uses lower case while Modicon uses upper case letters
// for all names in this structure. The caller can use either convention.
//
#define NCB_COMMAND         ncb_command
#define NCB_RETCODE         ncb_retcode
#define NCB_LSN             ncb_lsn
#define NCB_NUM             ncb_num
#define NCB_BUFFER          ncb_buffer
#define NCB_LENGTH          ncb_length
#define NCB_CALLNAME        ncb_callname
#define NCB_NAME            ncb_name
#define NCB_RTO             ncb_rto
#define NCB_STO             ncb_sto
#define NCB_POST_ADDRESS    ncb_post
#define NCB_LANA_NUM        ncb_lana_num
#define NCB_CMD_CPLT        ncb_cmd_cplt
#define NCB_RESERVE         ncb_reserve
#endif	// NCB_INCLUDED

//
// NetBIOS functions list - "WAIT" calls wait until command completes
// while the others jump to the routine in NCB_POST when the NetBIOS
// command completes and does an interrupt.
//
#define RESET                       0x32    // Reset adapter card and tables
#define CANCEL                      0x35    // Cancel command. NCB_BUFFER = cmd.
#define STATUS                      0xb3    // Status information for adapter
#define STATUS_WAIT                 0x33    //
#define TRACE                       0xf9    // Token-Ring protocol trace
#define TRACE_WAIT                  0x79    //
#define UNLINK                      0x70    // Unlink from IBM Remote Program
#define ADD_NAME                    0xb0    // Add name to name table
#define ADD_NAME_WAIT               0x30    //
#define ADD_GROUP_NAME              0xb6    // Add group name to name table
#define ADD_GROUP_NAME_WAIT         0x36    //
#define DELETE_NAME                 0xb1    // Delete name from name table
#define DELETE_NAME_WAIT            0x31    //
#define CALL                        0x90    // Start session with NCB_NAME name
#define CALL_WAIT                   0x10    //
#define LISTEN                      0x91    // Listen for call
#define LISTEN_WAIT                 0x11    //
#define HANG_UP                     0x92    // End session with NCB_NAME name
#define HANG_UP_WAIT                0x12    //
#define SEND                        0x94    // Send data via NCB_LSN
#define SEND_WAIT                   0x14    //
#define SEND_NO_ACK                 0xf1    // Send data without waiting for ACK
#define SEND_NO_ACK_WAIT            0x71    //
#define CHAIN_SEND                  0x97    // Send multiple data buffers
#define CHAIN_SEND_WAIT             0x17    //
#define CHAIN_SEND_NO_ACK           0xf2    // Send multiple buffers without ACK
#define CHAIN_SEND_NO_ACK_WAIT      0x72    //
#define RECEIVE                     0x95    // Receive data from a session
#define RECEIVE_WAIT                0x15    //
#define RECEIVE_ANY                 0x96    // Receive data from any session
#define RECEIVE_ANY_WAIT            0x16    //
#define SESSION_STATUS              0xb4    // Status of all sessions for name
#define SESSION_STATUS_WAIT         0x34    //
#define SEND_DATAGRAM               0xa0    // Send un-ACKed message
#define SEND_DATAGRAM_WAIT          0x20    //
#define SEND_BCST_DATAGRAM          0xa2    // Send broadcast message
#define SEND_BCST_DATAGRAM_WAIT     0x22    //
#define RECEIVE_DATAGRAM            0xa1    // Receive un-ACKed message
#define RECEIVE_DATAGRAM_WAIT       0x21    //
#define RECEIVE_BCST_DATAGRAM       0xa3    // Receive broadcast message
#define RECEIVE_BCST_DATAGRAM_WAIT  0x23    //
#define SA85_OFF                    0x37    // Turn SA85 driver off (RESET to turn on)
#define SA85_MASK                   0x38    // Return status of mask bit
#define SA85_ION                    0x39    // Enable sa85 interrupts
#define SA85_IOFF                   0x3a    // Disable sa85 interrupts
#define SET_SLAVE_LOGIN             0x3b    // Set/clear slave login status

//
// NetBIOS error return codes - returned in NCB_RETCODE
//
#ifndef ERR_success
#define ERR_success             0       // NetBIOS command completed normally
#define ERR_bad_buffer_length   1       // Bad send or status buffer size
#define ERR_invalid             3       // invalid NetBIOS command
#define ERR_timeout             5       // Command time-out has expired
#define ERR_buffer_too_small    6       // Receive buffer not big enough
#define ERR_bad_session_num     8       // Bad value in NCB_LSN
#define ERR_no_RAM              9       // LAN card doesn't have enough memory
#define ERR_session_closed      0xa     // This session is closed
#define ERR_cancel              0xb     // Command has been closed
#define ERR_dup_local_name      0xd     // Name already exists for this PC
#define ERR_name_table_full     0xe     // Local name table is full
#define ERR_active_session      0xf     // Can't delete name - used in session
#define ERR_sess_table_full     0x11    // Local session table is full
#define ERR_no_listen           0x12    // Remote PC not listening for call
#define ERR_bad_name_num        0x13    // Bad value in NCB_NUM field
#define ERR_no_answer           0x14    // No answer to CALL or no such remote
#define ERR_no_local_name       0x15    // No such name in local name table
#define ERR_duplicate_name      0x16    // Name is in use elsewhere on net
#define ERR_bad_delete          0x17    // Name incorrectly deleted
#define ERR_abnormal_end        0x18    // Session aborted abnormally
#define ERR_name_error          0x19    // 2 or more identical names in use!
#define ERR_bad_packet          0x1a    // Bad NetBIOS packet on network
#define ERR_card_busy           0x21    // network card is busy
#define ERR_too_many_cmds       0x22    // Too many NetBIOS commands queued
#define ERR_bad_card_num        0x23    // bad NCB_LANA_NUM - must be 0 or 1
#define ERR_cancel_done         0x24    // command finished while cancelling
#define ERR_no_cancel           0x26    // Command can't be cancelled
#define ERR_busy                0xff    // Still processing command

#endif // ERR_success

#ifdef __cplusplus
}
#endif

#endif // NETBIOS_INCLUDED
