/*============================================================================
*
*       +============================================================+
*       |      Copyright (C) 1997, Cyberlogic Technologies Inc.      |
*       +============================================================+
*
* Module Name :
*       MbxError.h
* 
* Abstract:
*       Error code definitions for MBXAPI.
*
* Author:
*       Paul Mikulski
* 
* ---------+-----+-----------------------------------------------------------
*   DATE   | BY  |  DESCRIPTION / REASON FOR MODIFICATION
* ---------+-----+-----------------------------------------------------------
* 12-08-96 | PM  |  Start of development
* ---------+-----+-----------------------------------------------------------
* ..-..-96 |     |
*==========+=====+==========================================================*/
#ifndef _MBXERROR_
#define _MBXERROR_

#include "MbxIoErr.h"       // Custom error code definitions

//
// MessageId: MBX_SUCCESS
//
// MessageText:
//
//  The operation completed successfully.
//
#define MBX_SUCCESS                    0L

//
// MessageId: MBX_INVALID_FUNCTION
//
// MessageText:
//
//  Incorrect function.
//
#define MBX_INVALID_FUNCTION           1L    // dderror

//
// MessageId: MBX_FILE_NOT_FOUND
//
// MessageText:
//
//  The system cannot find the file specified.
//
#define MBX_FILE_NOT_FOUND             2L

//
// MessageId: MBX_PATH_NOT_FOUND
//
// MessageText:
//
//  The system cannot find the path specified.
//
#define MBX_PATH_NOT_FOUND             3L

//
// MessageId: MBX_TOO_MANY_OPEN_FILES
//
// MessageText:
//
//  The system cannot open the file.
//
#define MBX_TOO_MANY_OPEN_FILES        4L

//
// MessageId: MBX_ACCESS_DENIED
//
// MessageText:
//
//  Access is denied.
//
#define MBX_ACCESS_DENIED              5L

//
// MessageId: MBX_INVALID_HANDLE
//
// MessageText:
//
//  The handle is invalid.
//
#define MBX_INVALID_HANDLE             6L

//
// MessageId: MBX_ARENA_TRASHED
//
// MessageText:
//
//  The storage control blocks were destroyed.
//
#define MBX_ARENA_TRASHED              7L

//
// MessageId: MBX_NOT_ENOUGH_MEMORY
//
// MessageText:
//
//  Not enough storage is available to process this command.
//
#define MBX_NOT_ENOUGH_MEMORY          8L    // dderror

//
// MessageId: MBX_INVALID_BLOCK
//
// MessageText:
//
//  The storage control block address is invalid.
//
#define MBX_INVALID_BLOCK              9L

//
// MessageId: MBX_BAD_ENVIRONMENT
//
// MessageText:
//
//  The environment is incorrect.
//
#define MBX_BAD_ENVIRONMENT            10L

//
// MessageId: MBX_BAD_FORMAT
//
// MessageText:
//
//  An attempt was made to load a program with an
//  incorrect format.
//
#define MBX_BAD_FORMAT                 11L

//
// MessageId: MBX_INVALID_ACCESS
//
// MessageText:
//
//  The access code is invalid.
//
#define MBX_INVALID_ACCESS             12L

//
// MessageId: MBX_INVALID_DATA
//
// MessageText:
//
//  The data is invalid.
//
#define MBX_INVALID_DATA               13L

//
// MessageId: MBX_OUTOFMEMORY
//
// MessageText:
//
//  Not enough storage is available to complete this operation.
//
#define MBX_OUTOFMEMORY                14L

//
// MessageId: MBX_INVALID_DRIVE
//
// MessageText:
//
//  The system cannot find the drive specified.
//
#define MBX_INVALID_DRIVE              15L

//
// MessageId: MBX_CURRENT_DIRECTORY
//
// MessageText:
//
//  The directory cannot be removed.
//
#define MBX_CURRENT_DIRECTORY          16L

//
// MessageId: MBX_NOT_SAME_DEVICE
//
// MessageText:
//
//  The system cannot move the file
//  to a different disk drive.
//
#define MBX_NOT_SAME_DEVICE            17L

//
// MessageId: MBX_NO_MORE_FILES
//
// MessageText:
//
//  There are no more files.
//
#define MBX_NO_MORE_FILES              18L

//
// MessageId: MBX_WRITE_PROTECT
//
// MessageText:
//
//  The media is write protected.
//
#define MBX_WRITE_PROTECT              19L

//
// MessageId: MBX_BAD_UNIT
//
// MessageText:
//
//  The system cannot find the device specified.
//
#define MBX_BAD_UNIT                   20L

//
// MessageId: MBX_NOT_READY
//
// MessageText:
//
//  The device is not ready.
//
#define MBX_NOT_READY                  21L

//
// MessageId: MBX_BAD_COMMAND
//
// MessageText:
//
//  The device does not recognize the command.
//
#define MBX_BAD_COMMAND                22L

//
// MessageId: MBX_CRC
//
// MessageText:
//
//  Data error (cyclic redundancy check)
//
#define MBX_CRC                        23L

//
// MessageId: MBX_BAD_LENGTH
//
// MessageText:
//
//  The program issued a command but the
//  command length is incorrect.
//
#define MBX_BAD_LENGTH                 24L

//
// MessageId: MBX_SEEK
//
// MessageText:
//
//  The drive cannot locate a specific
//  area or track on the disk.
//
#define MBX_SEEK                       25L

//
// MessageId: MBX_NOT_DOS_DISK
//
// MessageText:
//
//  The specified disk or diskette cannot be accessed.
//
#define MBX_NOT_DOS_DISK               26L

//
// MessageId: MBX_SECTOR_NOT_FOUND
//
// MessageText:
//
//  The drive cannot find the sector requested.
//
#define MBX_SECTOR_NOT_FOUND           27L

//
// MessageId: MBX_OUT_OF_PAPER
//
// MessageText:
//
//  The printer is out of paper.
//
#define MBX_OUT_OF_PAPER               28L

//
// MessageId: MBX_WRITE_FAULT
//
// MessageText:
//
//  The system cannot write to the specified device.
//
#define MBX_WRITE_FAULT                29L

//
// MessageId: MBX_READ_FAULT
//
// MessageText:
//
//  The system cannot read from the specified device.
//
#define MBX_READ_FAULT                 30L

//
// MessageId: MBX_GEN_FAILURE
//
// MessageText:
//
//  A device attached to the system is not functioning.
//
#define MBX_GEN_FAILURE                31L

//
// MessageId: MBX_SHARING_VIOLATION
//
// MessageText:
//
//  The process cannot access the file because
//  it is being used by another process.
//
#define MBX_SHARING_VIOLATION          32L

//
// MessageId: MBX_LOCK_VIOLATION
//
// MessageText:
//
//  The process cannot access the file because
//  another process has locked a portion of the file.
//
#define MBX_LOCK_VIOLATION             33L

//
// MessageId: MBX_WRONG_DISK
//
// MessageText:
//
//  The wrong diskette is in the drive.
//  Insert %2 (Volume Serial Number: %3)
//  into drive %1.
//
#define MBX_WRONG_DISK                 34L

//
// MessageId: MBX_SHARING_BUFFER_EXCEEDED
//
// MessageText:
//
//  Too many files opened for sharing.
//
#define MBX_SHARING_BUFFER_EXCEEDED    36L

//
// MessageId: MBX_HANDLE_EOF
//
// MessageText:
//
//  Reached end of file.
//
#define MBX_HANDLE_EOF                 38L

//
// MessageId: MBX_HANDLE_DISK_FULL
//
// MessageText:
//
//  The disk is full.
//
#define MBX_HANDLE_DISK_FULL           39L

//
// MessageId: MBX_NOT_SUPPORTED
//
// MessageText:
//
//  The network request is not supported.
//
#define MBX_NOT_SUPPORTED              50L

//
// MessageId: MBX_REM_NOT_LIST
//
// MessageText:
//
//  The remote computer is not available.
//
#define MBX_REM_NOT_LIST               51L

//
// MessageId: MBX_DUP_NAME
//
// MessageText:
//
//  A duplicate name exists on the network.
//
#define MBX_DUP_NAME                   52L

//
// MessageId: MBX_BAD_NETPATH
//
// MessageText:
//
//  The network path was not found.
//
#define MBX_BAD_NETPATH                53L

//
// MessageId: MBX_NETWORK_BUSY
//
// MessageText:
//
//  The network is busy.
//
#define MBX_NETWORK_BUSY               54L

//
// MessageId: MBX_DEV_NOT_EXIST
//
// MessageText:
//
//  The specified network resource or device is no longer
//  available.
//
#define MBX_DEV_NOT_EXIST              55L    // dderror

//
// MessageId: MBX_TOO_MANY_CMDS
//
// MessageText:
//
//  The network BIOS command limit has been reached.
//
#define MBX_TOO_MANY_CMDS              56L

//
// MessageId: MBX_ADAP_HDW_ERR
//
// MessageText:
//
//  A network adapter hardware error occurred.
//
#define MBX_ADAP_HDW_ERR               57L

//
// MessageId: MBX_BAD_NET_RESP
//
// MessageText:
//
//  The specified server cannot perform the requested
//  operation.
//
#define MBX_BAD_NET_RESP               58L

//
// MessageId: MBX_UNEXP_NET_ERR
//
// MessageText:
//
//  An unexpected network error occurred.
//
#define MBX_UNEXP_NET_ERR              59L

//
// MessageId: MBX_BAD_REM_ADAP
//
// MessageText:
//
//  The remote adapter is not compatible.
//
#define MBX_BAD_REM_ADAP               60L

//
// MessageId: MBX_PRINTQ_FULL
//
// MessageText:
//
//  The printer queue is full.
//
#define MBX_PRINTQ_FULL                61L

//
// MessageId: MBX_NO_SPOOL_SPACE
//
// MessageText:
//
//  Space to store the file waiting to be printed is
//  not available on the server.
//
#define MBX_NO_SPOOL_SPACE             62L

//
// MessageId: MBX_PRINT_CANCELLED
//
// MessageText:
//
//  Your file waiting to be printed was deleted.
//
#define MBX_PRINT_CANCELLED            63L

//
// MessageId: MBX_NETNAME_DELETED
//
// MessageText:
//
//  The specified network name is no longer available.
//
#define MBX_NETNAME_DELETED            64L

//
// MessageId: MBX_NETWORK_ACCESS_DENIED
//
// MessageText:
//
//  Network access is denied.
//
#define MBX_NETWORK_ACCESS_DENIED      65L

//
// MessageId: MBX_BAD_DEV_TYPE
//
// MessageText:
//
//  The network resource type is not correct.
//
#define MBX_BAD_DEV_TYPE               66L

//
// MessageId: MBX_BAD_NET_NAME
//
// MessageText:
//
//  The network name cannot be found.
//
#define MBX_BAD_NET_NAME               67L

//
// MessageId: MBX_TOO_MANY_NAMES
//
// MessageText:
//
//  The name limit for the local computer network
//  adapter card was exceeded.
//
#define MBX_TOO_MANY_NAMES             68L

//
// MessageId: MBX_TOO_MANY_SESS
//
// MessageText:
//
//  The network BIOS session limit was exceeded.
//
#define MBX_TOO_MANY_SESS              69L

//
// MessageId: MBX_SHARING_PAUSED
//
// MessageText:
//
//  The remote server has been paused or is in the
//  process of being started.
//
#define MBX_SHARING_PAUSED             70L

//
// MessageId: MBX_REQ_NOT_ACCEP
//
// MessageText:
//
//  No more connections can be made to this remote computer at this time
//  because there are already as many connections as the computer can accept.
//
#define MBX_REQ_NOT_ACCEP              71L

//
// MessageId: MBX_REDIR_PAUSED
//
// MessageText:
//
//  The specified printer or disk device has been paused.
//
#define MBX_REDIR_PAUSED               72L

//
// MessageId: MBX_FILE_EXISTS
//
// MessageText:
//
//  The file exists.
//
#define MBX_FILE_EXISTS                80L

//
// MessageId: MBX_CANNOT_MAKE
//
// MessageText:
//
//  The directory or file cannot be created.
//
#define MBX_CANNOT_MAKE                82L

//
// MessageId: MBX_FAIL_I24
//
// MessageText:
//
//  Fail on INT 24
//
#define MBX_FAIL_I24                   83L

//
// MessageId: MBX_OUT_OF_STRUCTURES
//
// MessageText:
//
//  Storage to process this request is not available.
//
#define MBX_OUT_OF_STRUCTURES          84L

//
// MessageId: MBX_ALREADY_ASSIGNED
//
// MessageText:
//
//  The local device name is already in use.
//
#define MBX_ALREADY_ASSIGNED           85L

//
// MessageId: MBX_INVALID_PASSWORD
//
// MessageText:
//
//  The specified network password is not correct.
//
#define MBX_INVALID_PASSWORD           86L

//
// MessageId: MBX_INVALID_PARAMETER
//
// MessageText:
//
//  The parameter is incorrect.
//
#define MBX_INVALID_PARAMETER          87L    // dderror

//
// MessageId: MBX_NET_WRITE_FAULT
//
// MessageText:
//
//  A write fault occurred on the network.
//
#define MBX_NET_WRITE_FAULT            88L

//
// MessageId: MBX_NO_PROC_SLOTS
//
// MessageText:
//
//  The system cannot start another process at
//  this time.
//
#define MBX_NO_PROC_SLOTS              89L

//
// MessageId: MBX_TOO_MANY_SEMAPHORES
//
// MessageText:
//
//  Cannot create another system semaphore.
//
#define MBX_TOO_MANY_SEMAPHORES        100L

//
// MessageId: MBX_EXCL_SEM_ALREADY_OWNED
//
// MessageText:
//
//  The exclusive semaphore is owned by another process.
//
#define MBX_EXCL_SEM_ALREADY_OWNED     101L

//
// MessageId: MBX_SEM_IS_SET
//
// MessageText:
//
//  The semaphore is set and cannot be closed.
//
#define MBX_SEM_IS_SET                 102L

//
// MessageId: MBX_TOO_MANY_SEM_REQUESTS
//
// MessageText:
//
//  The semaphore cannot be set again.
//
#define MBX_TOO_MANY_SEM_REQUESTS      103L

//
// MessageId: MBX_INVALID_AT_INTERRUPT_TIME
//
// MessageText:
//
//  Cannot request exclusive semaphores at interrupt time.
//
#define MBX_INVALID_AT_INTERRUPT_TIME  104L

//
// MessageId: MBX_SEM_OWNER_DIED
//
// MessageText:
//
//  The previous ownership of this semaphore has ended.
//
#define MBX_SEM_OWNER_DIED             105L

//
// MessageId: MBX_SEM_USER_LIMIT
//
// MessageText:
//
//  Insert the diskette for drive %1.
//
#define MBX_SEM_USER_LIMIT             106L

//
// MessageId: MBX_DISK_CHANGE
//
// MessageText:
//
//  Program stopped because alternate diskette was not inserted.
//
#define MBX_DISK_CHANGE                107L

//
// MessageId: MBX_DRIVE_LOCKED
//
// MessageText:
//
//  The disk is in use or locked by
//  another process.
//
#define MBX_DRIVE_LOCKED               108L

//
// MessageId: MBX_BROKEN_PIPE
//
// MessageText:
//
//  The pipe has been ended.
//
#define MBX_BROKEN_PIPE                109L

//
// MessageId: MBX_OPEN_FAILED
//
// MessageText:
//
//  The system cannot open the
//  device or file specified.
//
#define MBX_OPEN_FAILED                110L

//
// MessageId: MBX_BUFFER_OVERFLOW
//
// MessageText:
//
//  The file name is too long.
//
#define MBX_BUFFER_OVERFLOW            111L

//
// MessageId: MBX_DISK_FULL
//
// MessageText:
//
//  There is not enough space on the disk.
//
#define MBX_DISK_FULL                  112L

//
// MessageId: MBX_NO_MORE_SEARCH_HANDLES
//
// MessageText:
//
//  No more internal file identifiers available.
//
#define MBX_NO_MORE_SEARCH_HANDLES     113L

//
// MessageId: MBX_INVALID_TARGET_HANDLE
//
// MessageText:
//
//  The target internal file identifier is incorrect.
//
#define MBX_INVALID_TARGET_HANDLE      114L

//
// MessageId: MBX_INVALID_CATEGORY
//
// MessageText:
//
//  The IOCTL call made by the application program is
//  not correct.
//
#define MBX_INVALID_CATEGORY           117L

//
// MessageId: MBX_INVALID_VERIFY_SWITCH
//
// MessageText:
//
//  The verify-on-write switch parameter value is not
//  correct.
//
#define MBX_INVALID_VERIFY_SWITCH      118L

//
// MessageId: MBX_BAD_DRIVER_LEVEL
//
// MessageText:
//
//  The system does not support the command requested.
//
#define MBX_BAD_DRIVER_LEVEL           119L

//
// MessageId: MBX_CALL_NOT_IMPLEMENTED
//
// MessageText:
//
//  This function is only valid in Windows NT mode.
//
#define MBX_CALL_NOT_IMPLEMENTED       120L

//
// MessageId: MBX_SEM_TIMEOUT
//
// MessageText:
//
//  The semaphore timeout period has expired.
//
#define MBX_SEM_TIMEOUT                121L

//
// MessageId: MBX_INSUFFICIENT_BUFFER
//
// MessageText:
//
//  The data area passed to a system call is too
//  small.
//
#define MBX_INSUFFICIENT_BUFFER        122L    // dderror

//
// MessageId: MBX_INVALID_NAME
//
// MessageText:
//
//  The filename, directory name, or volume label syntax is incorrect.
//
#define MBX_INVALID_NAME               123L

//
// MessageId: MBX_INVALID_LEVEL
//
// MessageText:
//
//  The system call level is not correct.
//
#define MBX_INVALID_LEVEL              124L

//
// MessageId: MBX_NO_VOLUME_LABEL
//
// MessageText:
//
//  The disk has no volume label.
//
#define MBX_NO_VOLUME_LABEL            125L

//
// MessageId: MBX_MOD_NOT_FOUND
//
// MessageText:
//
//  The specified module could not be found.
//
#define MBX_MOD_NOT_FOUND              126L

//
// MessageId: MBX_PROC_NOT_FOUND
//
// MessageText:
//
//  The specified procedure could not be found.
//
#define MBX_PROC_NOT_FOUND             127L

//
// MessageId: MBX_WAIT_NO_CHILDREN
//
// MessageText:
//
//  There are no child processes to wait for.
//
#define MBX_WAIT_NO_CHILDREN           128L

//
// MessageId: MBX_CHILD_NOT_COMPLETE
//
// MessageText:
//
//  The %1 application cannot be run in Windows NT mode.
//
#define MBX_CHILD_NOT_COMPLETE         129L

//
// MessageId: MBX_DIRECT_ACCESS_HANDLE
//
// MessageText:
//
//  Attempt to use a file handle to an open disk partition for an
//  operation other than raw disk I/O.
//
#define MBX_DIRECT_ACCESS_HANDLE       130L

//
// MessageId: MBX_NEGATIVE_SEEK
//
// MessageText:
//
//  An attempt was made to move the file pointer before the beginning of the file.
//
#define MBX_NEGATIVE_SEEK              131L

//
// MessageId: MBX_SEEK_ON_DEVICE
//
// MessageText:
//
//  The file pointer cannot be set on the specified device or file.
//
#define MBX_SEEK_ON_DEVICE             132L

//
// MessageId: MBX_IS_JOIN_TARGET
//
// MessageText:
//
//  A JOIN or SUBST command
//  cannot be used for a drive that
//  contains previously joined drives.
//
#define MBX_IS_JOIN_TARGET             133L

//
// MessageId: MBX_IS_JOINED
//
// MessageText:
//
//  An attempt was made to use a
//  JOIN or SUBST command on a drive that has
//  already been joined.
//
#define MBX_IS_JOINED                  134L

//
// MessageId: MBX_IS_SUBSTED
//
// MessageText:
//
//  An attempt was made to use a
//  JOIN or SUBST command on a drive that has
//  already been substituted.
//
#define MBX_IS_SUBSTED                 135L

//
// MessageId: MBX_NOT_JOINED
//
// MessageText:
//
//  The system tried to delete
//  the JOIN of a drive that is not joined.
//
#define MBX_NOT_JOINED                 136L

//
// MessageId: MBX_NOT_SUBSTED
//
// MessageText:
//
//  The system tried to delete the
//  substitution of a drive that is not substituted.
//
#define MBX_NOT_SUBSTED                137L

//
// MessageId: MBX_JOIN_TO_JOIN
//
// MessageText:
//
//  The system tried to join a drive
//  to a directory on a joined drive.
//
#define MBX_JOIN_TO_JOIN               138L

//
// MessageId: MBX_SUBST_TO_SUBST
//
// MessageText:
//
//  The system tried to substitute a
//  drive to a directory on a substituted drive.
//
#define MBX_SUBST_TO_SUBST             139L

//
// MessageId: MBX_JOIN_TO_SUBST
//
// MessageText:
//
//  The system tried to join a drive to
//  a directory on a substituted drive.
//
#define MBX_JOIN_TO_SUBST              140L

//
// MessageId: MBX_SUBST_TO_JOIN
//
// MessageText:
//
//  The system tried to SUBST a drive
//  to a directory on a joined drive.
//
#define MBX_SUBST_TO_JOIN              141L

//
// MessageId: MBX_BUSY_DRIVE
//
// MessageText:
//
//  The system cannot perform a JOIN or SUBST at this time.
//
#define MBX_BUSY_DRIVE                 142L

//
// MessageId: MBX_SAME_DRIVE
//
// MessageText:
//
//  The system cannot join or substitute a
//  drive to or for a directory on the same drive.
//
#define MBX_SAME_DRIVE                 143L

//
// MessageId: MBX_DIR_NOT_ROOT
//
// MessageText:
//
//  The directory is not a subdirectory of the root directory.
//
#define MBX_DIR_NOT_ROOT               144L

//
// MessageId: MBX_DIR_NOT_EMPTY
//
// MessageText:
//
//  The directory is not empty.
//
#define MBX_DIR_NOT_EMPTY              145L

//
// MessageId: MBX_IS_SUBST_PATH
//
// MessageText:
//
//  The path specified is being used in
//  a substitute.
//
#define MBX_IS_SUBST_PATH              146L

//
// MessageId: MBX_IS_JOIN_PATH
//
// MessageText:
//
//  Not enough resources are available to
//  process this command.
//
#define MBX_IS_JOIN_PATH               147L

//
// MessageId: MBX_PATH_BUSY
//
// MessageText:
//
//  The path specified cannot be used at this time.
//
#define MBX_PATH_BUSY                  148L

//
// MessageId: MBX_IS_SUBST_TARGET
//
// MessageText:
//
//  An attempt was made to join
//  or substitute a drive for which a directory
//  on the drive is the target of a previous
//  substitute.
//
#define MBX_IS_SUBST_TARGET            149L

//
// MessageId: MBX_SYSTEM_TRACE
//
// MessageText:
//
//  System trace information was not specified in your
//  CONFIG.SYS file, or tracing is disallowed.
//
#define MBX_SYSTEM_TRACE               150L

//
// MessageId: MBX_INVALID_EVENT_COUNT
//
// MessageText:
//
//  The number of specified semaphore events for
//  DosMuxSemWait is not correct.
//
#define MBX_INVALID_EVENT_COUNT        151L

//
// MessageId: MBX_TOO_MANY_MUXWAITERS
//
// MessageText:
//
//  DosMuxSemWait did not execute; too many semaphores
//  are already set.
//
#define MBX_TOO_MANY_MUXWAITERS        152L

//
// MessageId: MBX_INVALID_LIST_FORMAT
//
// MessageText:
//
//  The DosMuxSemWait list is not correct.
//
#define MBX_INVALID_LIST_FORMAT        153L

//
// MessageId: MBX_LABEL_TOO_LONG
//
// MessageText:
//
//  The volume label you entered exceeds the label character
//  limit of the target file system.
//
#define MBX_LABEL_TOO_LONG             154L

//
// MessageId: MBX_TOO_MANY_TCBS
//
// MessageText:
//
//  Cannot create another thread.
//
#define MBX_TOO_MANY_TCBS              155L

//
// MessageId: MBX_SIGNAL_REFUSED
//
// MessageText:
//
//  The recipient process has refused the signal.
//
#define MBX_SIGNAL_REFUSED             156L

//
// MessageId: MBX_DISCARDED
//
// MessageText:
//
//  The segment is already discarded and cannot be locked.
//
#define MBX_DISCARDED                  157L

//
// MessageId: MBX_NOT_LOCKED
//
// MessageText:
//
//  The segment is already unlocked.
//
#define MBX_NOT_LOCKED                 158L

//
// MessageId: MBX_BAD_THREADID_ADDR
//
// MessageText:
//
//  The address for the thread ID is not correct.
//
#define MBX_BAD_THREADID_ADDR          159L

//
// MessageId: MBX_BAD_ARGUMENTS
//
// MessageText:
//
//  The argument string passed to DosExecPgm is not correct.
//
#define MBX_BAD_ARGUMENTS              160L

//
// MessageId: MBX_BAD_PATHNAME
//
// MessageText:
//
//  The specified path is invalid.
//
#define MBX_BAD_PATHNAME               161L

//
// MessageId: MBX_SIGNAL_PENDING
//
// MessageText:
//
//  A signal is already pending.
//
#define MBX_SIGNAL_PENDING             162L

//
// MessageId: MBX_MAX_THRDS_REACHED
//
// MessageText:
//
//  No more threads can be created in the system.
//
#define MBX_MAX_THRDS_REACHED          164L

//
// MessageId: MBX_LOCK_FAILED
//
// MessageText:
//
//  Unable to lock a region of a file.
//
#define MBX_LOCK_FAILED                167L

//
// MessageId: MBX_BUSY
//
// MessageText:
//
//  The requested resource is in use.
//
#define MBX_BUSY                       170L

//
// MessageId: MBX_CANCEL_VIOLATION
//
// MessageText:
//
//  A lock request was not outstanding for the supplied cancel region.
//
#define MBX_CANCEL_VIOLATION           173L

//
// MessageId: MBX_ATOMIC_LOCKS_NOT_SUPPORTED
//
// MessageText:
//
//  The file system does not support atomic changes to the lock type.
//
#define MBX_ATOMIC_LOCKS_NOT_SUPPORTED 174L

//
// MessageId: MBX_INVALID_SEGMENT_NUMBER
//
// MessageText:
//
//  The system detected a segment number that was not correct.
//
#define MBX_INVALID_SEGMENT_NUMBER     180L

//
// MessageId: MBX_INVALID_ORDINAL
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_ORDINAL            182L

//
// MessageId: MBX_ALREADY_EXISTS
//
// MessageText:
//
//  Cannot create a file when that file already exists.
//
#define MBX_ALREADY_EXISTS             183L

//
// MessageId: MBX_INVALID_FLAG_NUMBER
//
// MessageText:
//
//  The flag passed is not correct.
//
#define MBX_INVALID_FLAG_NUMBER        186L

//
// MessageId: MBX_SEM_NOT_FOUND
//
// MessageText:
//
//  The specified system semaphore name was not found.
//
#define MBX_SEM_NOT_FOUND              187L

//
// MessageId: MBX_INVALID_STARTING_CODESEG
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_STARTING_CODESEG   188L

//
// MessageId: MBX_INVALID_STACKSEG
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_STACKSEG           189L

//
// MessageId: MBX_INVALID_MODULETYPE
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_MODULETYPE         190L

//
// MessageId: MBX_INVALID_EXE_SIGNATURE
//
// MessageText:
//
//  Cannot run %1 in Windows NT mode.
//
#define MBX_INVALID_EXE_SIGNATURE      191L

//
// MessageId: MBX_EXE_MARKED_INVALID
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_EXE_MARKED_INVALID         192L

//
// MessageId: MBX_BAD_EXE_FORMAT
//
// MessageText:
//
//  %1 is not a valid Windows NT application.
//
#define MBX_BAD_EXE_FORMAT             193L

//
// MessageId: MBX_ITERATED_DATA_EXCEEDS_64k
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_ITERATED_DATA_EXCEEDS_64k  194L

//
// MessageId: MBX_INVALID_MINALLOCSIZE
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_MINALLOCSIZE       195L

//
// MessageId: MBX_DYNLINK_FROM_INVALID_RING
//
// MessageText:
//
//  The operating system cannot run this
//  application program.
//
#define MBX_DYNLINK_FROM_INVALID_RING  196L

//
// MessageId: MBX_IOPL_NOT_ENABLED
//
// MessageText:
//
//  The operating system is not presently
//  configured to run this application.
//
#define MBX_IOPL_NOT_ENABLED           197L

//
// MessageId: MBX_INVALID_SEGDPL
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INVALID_SEGDPL             198L

//
// MessageId: MBX_AUTODATASEG_EXCEEDS_64k
//
// MessageText:
//
//  The operating system cannot run this
//  application program.
//
#define MBX_AUTODATASEG_EXCEEDS_64k    199L

//
// MessageId: MBX_RING2SEG_MUST_BE_MOVABLE
//
// MessageText:
//
//  The code segment cannot be greater than or equal to 64KB.
//
#define MBX_RING2SEG_MUST_BE_MOVABLE   200L

//
// MessageId: MBX_RELOC_CHAIN_XEEDS_SEGLIM
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_RELOC_CHAIN_XEEDS_SEGLIM   201L

//
// MessageId: MBX_INFLOOP_IN_RELOC_CHAIN
//
// MessageText:
//
//  The operating system cannot run %1.
//
#define MBX_INFLOOP_IN_RELOC_CHAIN     202L

//
// MessageId: MBX_ENVVAR_NOT_FOUND
//
// MessageText:
//
//  The system could not find the environment
//  option that was entered.
//
#define MBX_ENVVAR_NOT_FOUND           203L

//
// MessageId: MBX_NO_SIGNAL_SENT
//
// MessageText:
//
//  No process in the command subtree has a
//  signal handler.
//
#define MBX_NO_SIGNAL_SENT             205L

//
// MessageId: MBX_FILENAME_EXCED_RANGE
//
// MessageText:
//
//  The filename or extension is too long.
//
#define MBX_FILENAME_EXCED_RANGE       206L

//
// MessageId: MBX_RING2_STACK_IN_USE
//
// MessageText:
//
//  The ring 2 stack is in use.
//
#define MBX_RING2_STACK_IN_USE         207L

//
// MessageId: MBX_META_EXPANSION_TOO_LONG
//
// MessageText:
//
//  The global filename characters, * or ?, are entered
//  incorrectly or too many global filename characters are specified.
//
#define MBX_META_EXPANSION_TOO_LONG    208L

//
// MessageId: MBX_INVALID_SIGNAL_NUMBER
//
// MessageText:
//
//  The signal being posted is not correct.
//
#define MBX_INVALID_SIGNAL_NUMBER      209L

//
// MessageId: MBX_THREAD_1_INACTIVE
//
// MessageText:
//
//  The signal handler cannot be set.
//
#define MBX_THREAD_1_INACTIVE          210L

//
// MessageId: MBX_LOCKED
//
// MessageText:
//
//  The segment is locked and cannot be reallocated.
//
#define MBX_LOCKED                     212L

//
// MessageId: MBX_TOO_MANY_MODULES
//
// MessageText:
//
//  Too many dynamic link modules are attached to this
//  program or dynamic link module.
//
#define MBX_TOO_MANY_MODULES           214L

//
// MessageId: MBX_NESTING_NOT_ALLOWED
//
// MessageText:
//
//  Can't nest calls to LoadModule.
//
#define MBX_NESTING_NOT_ALLOWED        215L

//
// MessageId: MBX_EXE_MACHINE_TYPE_MISMATCH
//
// MessageText:
//
//  The image file %1 is valid, but is for a machine type other
//  than the current machine.
//
#define MBX_EXE_MACHINE_TYPE_MISMATCH  216L

//
// MessageId: MBX_BAD_PIPE
//
// MessageText:
//
//  The pipe state is invalid.
//
#define MBX_BAD_PIPE                   230L

//
// MessageId: MBX_PIPE_BUSY
//
// MessageText:
//
//  All pipe instances are busy.
//
#define MBX_PIPE_BUSY                  231L

//
// MessageId: MBX_NO_DATA
//
// MessageText:
//
//  The pipe is being closed.
//
#define MBX_NO_DATA                    232L

//
// MessageId: MBX_PIPE_NOT_CONNECTED
//
// MessageText:
//
//  No process is on the other end of the pipe.
//
#define MBX_PIPE_NOT_CONNECTED         233L

//
// MessageId: MBX_MORE_DATA
//
// MessageText:
//
//  More data is available.
//
#define MBX_MORE_DATA                  234L    // dderror

//
// MessageId: MBX_VC_DISCONNECTED
//
// MessageText:
//
//  The session was cancelled.
//
#define MBX_VC_DISCONNECTED            240L

//
// MessageId: MBX_INVALID_EA_NAME
//
// MessageText:
//
//  The specified extended attribute name was invalid.
//
#define MBX_INVALID_EA_NAME            254L

//
// MessageId: MBX_EA_LIST_INCONSISTENT
//
// MessageText:
//
//  The extended attributes are inconsistent.
//
#define MBX_EA_LIST_INCONSISTENT       255L

//
// MessageId: MBX_NO_MORE_ITEMS
//
// MessageText:
//
//  No more data is available.
//
#define MBX_NO_MORE_ITEMS              259L

//
// MessageId: MBX_CANNOT_COPY
//
// MessageText:
//
//  The Copy API cannot be used.
//
#define MBX_CANNOT_COPY                266L

//
// MessageId: MBX_DIRECTORY
//
// MessageText:
//
//  The directory name is invalid.
//
#define MBX_DIRECTORY                  267L

//
// MessageId: MBX_EAS_DIDNT_FIT
//
// MessageText:
//
//  The extended attributes did not fit in the buffer.
//
#define MBX_EAS_DIDNT_FIT              275L

//
// MessageId: MBX_EA_FILE_CORRUPT
//
// MessageText:
//
//  The extended attribute file on the mounted file system is corrupt.
//
#define MBX_EA_FILE_CORRUPT            276L

//
// MessageId: MBX_EA_TABLE_FULL
//
// MessageText:
//
//  The extended attribute table file is full.
//
#define MBX_EA_TABLE_FULL              277L

//
// MessageId: MBX_INVALID_EA_HANDLE
//
// MessageText:
//
//  The specified extended attribute handle is invalid.
//
#define MBX_INVALID_EA_HANDLE          278L

//
// MessageId: MBX_EAS_NOT_SUPPORTED
//
// MessageText:
//
//  The mounted file system does not support extended attributes.
//
#define MBX_EAS_NOT_SUPPORTED          282L

//
// MessageId: MBX_NOT_OWNER
//
// MessageText:
//
//  Attempt to release mutex not owned by caller.
//
#define MBX_NOT_OWNER                  288L

//
// MessageId: MBX_TOO_MANY_POSTS
//
// MessageText:
//
//  Too many posts were made to a semaphore.
//
#define MBX_TOO_MANY_POSTS             298L

//
// MessageId: MBX_PARTIAL_COPY
//
// MessageText:
//
//  Only part of a Read/WriteProcessMemory request was completed.
//
#define MBX_PARTIAL_COPY               299L

//
// MessageId: MBX_MR_MID_NOT_FOUND
//
// MessageText:
//
//  The system cannot find message for message number 0x%1
//  in message file for %2.
//
#define MBX_MR_MID_NOT_FOUND           317L

//
// MessageId: MBX_INVALID_ADDRESS
//
// MessageText:
//
//  Attempt to access invalid address.
//
#define MBX_INVALID_ADDRESS            487L

//
// MessageId: MBX_ARITHMETIC_OVERFLOW
//
// MessageText:
//
//  Arithmetic result exceeded 32 bits.
//
#define MBX_ARITHMETIC_OVERFLOW        534L

//
// MessageId: MBX_PIPE_CONNECTED
//
// MessageText:
//
//  There is a process on other end of the pipe.
//
#define MBX_PIPE_CONNECTED             535L

//
// MessageId: MBX_PIPE_LISTENING
//
// MessageText:
//
//  Waiting for a process to open the other end of the pipe.
//
#define MBX_PIPE_LISTENING             536L

//
// MessageId: MBX_EA_ACCESS_DENIED
//
// MessageText:
//
//  Access to the extended attribute was denied.
//
#define MBX_EA_ACCESS_DENIED           994L

//
// MessageId: MBX_OPERATION_ABORTED
//
// MessageText:
//
//  The I/O operation has been aborted because of either a thread exit
//  or an application request.
//
#define MBX_OPERATION_ABORTED          995L

//
// MessageId: MBX_IO_INCOMPLETE
//
// MessageText:
//
//  Overlapped I/O event is not in a signalled state.
//
#define MBX_IO_INCOMPLETE              996L

//
// MessageId: MBX_IO_PENDING
//
// MessageText:
//
//  Overlapped I/O operation is in progress.
//
#define MBX_IO_PENDING                 997L    // dderror

//
// MessageId: MBX_NOACCESS
//
// MessageText:
//
//  Invalid access to memory location.
//
#define MBX_NOACCESS                   998L

//
// MessageId: MBX_SWAPERROR
//
// MessageText:
//
//  Error performing inpage operation.
//
#define MBX_SWAPERROR                  999L

//
// MessageId: MBX_STACK_OVERFLOW
//
// MessageText:
//
//  Recursion too deep, stack overflowed.
//
#define MBX_STACK_OVERFLOW             1001L

//
// MessageId: MBX_INVALID_MESSAGE
//
// MessageText:
//
//  The window cannot act on the sent message.
//
#define MBX_INVALID_MESSAGE            1002L

//
// MessageId: MBX_CAN_NOT_COMPLETE
//
// MessageText:
//
//  Cannot complete this function.
//
#define MBX_CAN_NOT_COMPLETE           1003L

//
// MessageId: MBX_INVALID_FLAGS
//
// MessageText:
//
//  Invalid flags.
//
#define MBX_INVALID_FLAGS              1004L

//
// MessageId: MBX_UNRECOGNIZED_VOLUME
//
// MessageText:
//
//  The volume does not contain a recognized file system.
//  Please make sure that all required file system drivers are loaded and that the
//  volume is not corrupt.
//
#define MBX_UNRECOGNIZED_VOLUME        1005L

//
// MessageId: MBX_FILE_INVALID
//
// MessageText:
//
//  The volume for a file has been externally altered such that the
//  opened file is no longer valid.
//
#define MBX_FILE_INVALID               1006L

//
// MessageId: MBX_FULLSCREEN_MODE
//
// MessageText:
//
//  The requested operation cannot be performed in full-screen mode.
//
#define MBX_FULLSCREEN_MODE            1007L

//
// MessageId: MBX_NO_TOKEN
//
// MessageText:
//
//  An attempt was made to reference a token that does not exist.
//
#define MBX_NO_TOKEN                   1008L

//
// MessageId: MBX_BADDB
//
// MessageText:
//
//  The configuration registry database is corrupt.
//
#define MBX_BADDB                      1009L

//
// MessageId: MBX_BADKEY
//
// MessageText:
//
//  The configuration registry key is invalid.
//
#define MBX_BADKEY                     1010L

//
// MessageId: MBX_CANTOPEN
//
// MessageText:
//
//  The configuration registry key could not be opened.
//
#define MBX_CANTOPEN                   1011L

//
// MessageId: MBX_CANTREAD
//
// MessageText:
//
//  The configuration registry key could not be read.
//
#define MBX_CANTREAD                   1012L

//
// MessageId: MBX_CANTWRITE
//
// MessageText:
//
//  The configuration registry key could not be written.
//
#define MBX_CANTWRITE                  1013L

//
// MessageId: MBX_REGISTRY_RECOVERED
//
// MessageText:
//
//  One of the files in the Registry database had to be recovered
//  by use of a log or alternate copy.  The recovery was successful.
//
#define MBX_REGISTRY_RECOVERED         1014L

//
// MessageId: MBX_REGISTRY_CORRUPT
//
// MessageText:
//
//  The Registry is corrupt. The structure of one of the files that contains
//  Registry data is corrupt, or the system's image of the file in memory
//  is corrupt, or the file could not be recovered because the alternate
//  copy or log was absent or corrupt.
//
#define MBX_REGISTRY_CORRUPT           1015L

//
// MessageId: MBX_REGISTRY_IO_FAILED
//
// MessageText:
//
//  An I/O operation initiated by the Registry failed unrecoverably.
//  The Registry could not read in, or write out, or flush, one of the files
//  that contain the system's image of the Registry.
//
#define MBX_REGISTRY_IO_FAILED         1016L

//
// MessageId: MBX_NOT_REGISTRY_FILE
//
// MessageText:
//
//  The system has attempted to load or restore a file into the Registry, but the
//  specified file is not in a Registry file format.
//
#define MBX_NOT_REGISTRY_FILE          1017L

//
// MessageId: MBX_KEY_DELETED
//
// MessageText:
//
//  Illegal operation attempted on a Registry key which has been marked for deletion.
//
#define MBX_KEY_DELETED                1018L

//
// MessageId: MBX_NO_LOG_SPACE
//
// MessageText:
//
//  System could not allocate the required space in a Registry log.
//
#define MBX_NO_LOG_SPACE               1019L

//
// MessageId: MBX_KEY_HAS_CHILDREN
//
// MessageText:
//
//  Cannot create a symbolic link in a Registry key that already
//  has subkeys or values.
//
#define MBX_KEY_HAS_CHILDREN           1020L

//
// MessageId: MBX_CHILD_MUST_BE_VOLATILE
//
// MessageText:
//
//  Cannot create a stable subkey under a volatile parent key.
//
#define MBX_CHILD_MUST_BE_VOLATILE     1021L

//
// MessageId: MBX_NOTIFY_ENUM_DIR
//
// MessageText:
//
//  A notify change request is being completed and the information
//  is not being returned in the caller's buffer. The caller now
//  needs to enumerate the files to find the changes.
//
#define MBX_NOTIFY_ENUM_DIR            1022L

//
// MessageId: MBX_DEPENDENT_SERVICES_RUNNING
//
// MessageText:
//
//  A stop control has been sent to a service which other running services
//  are dependent on.
//
#define MBX_DEPENDENT_SERVICES_RUNNING 1051L

//
// MessageId: MBX_INVALID_SERVICE_CONTROL
//
// MessageText:
//
//  The requested control is not valid for this service
//
#define MBX_INVALID_SERVICE_CONTROL    1052L

//
// MessageId: MBX_SERVICE_REQUEST_TIMEOUT
//
// MessageText:
//
//  The service did not respond to the start or control request in a timely
//  fashion.
//
#define MBX_SERVICE_REQUEST_TIMEOUT    1053L

//
// MessageId: MBX_SERVICE_NO_THREAD
//
// MessageText:
//
//  A thread could not be created for the service.
//
#define MBX_SERVICE_NO_THREAD          1054L

//
// MessageId: MBX_SERVICE_DATABASE_LOCKED
//
// MessageText:
//
//  The service database is locked.
//
#define MBX_SERVICE_DATABASE_LOCKED    1055L

//
// MessageId: MBX_SERVICE_ALREADY_RUNNING
//
// MessageText:
//
//  An instance of the service is already running.
//
#define MBX_SERVICE_ALREADY_RUNNING    1056L

//
// MessageId: MBX_INVALID_SERVICE_ACCOUNT
//
// MessageText:
//
//  The account name is invalid or does not exist.
//
#define MBX_INVALID_SERVICE_ACCOUNT    1057L

//
// MessageId: MBX_SERVICE_DISABLED
//
// MessageText:
//
//  The specified service is disabled and cannot be started.
//
#define MBX_SERVICE_DISABLED           1058L

//
// MessageId: MBX_CIRCULAR_DEPENDENCY
//
// MessageText:
//
//  Circular service dependency was specified.
//
#define MBX_CIRCULAR_DEPENDENCY        1059L

//
// MessageId: MBX_SERVICE_DOES_NOT_EXIST
//
// MessageText:
//
//  The specified service does not exist as an installed service.
//
#define MBX_SERVICE_DOES_NOT_EXIST     1060L

//
// MessageId: MBX_SERVICE_CANNOT_ACCEPT_CTRL
//
// MessageText:
//
//  The service cannot accept control messages at this time.
//
#define MBX_SERVICE_CANNOT_ACCEPT_CTRL 1061L

//
// MessageId: MBX_SERVICE_NOT_ACTIVE
//
// MessageText:
//
//  The service has not been started.
//
#define MBX_SERVICE_NOT_ACTIVE         1062L

//
// MessageId: MBX_FAILED_SERVICE_CONTROLLER_CONNECT
//
// MessageText:
//
//  The service process could not connect to the service controller.
//
#define MBX_FAILED_SERVICE_CONTROLLER_CONNECT 1063L

//
// MessageId: MBX_EXCEPTION_IN_SERVICE
//
// MessageText:
//
//  An exception occurred in the service when handling the control request.
//
#define MBX_EXCEPTION_IN_SERVICE       1064L

//
// MessageId: MBX_DATABASE_DOES_NOT_EXIST
//
// MessageText:
//
//  The database specified does not exist.
//
#define MBX_DATABASE_DOES_NOT_EXIST    1065L

//
// MessageId: MBX_SERVICE_SPECIFIC_ERROR
//
// MessageText:
//
//  The service has returned a service-specific error code.
//
#define MBX_SERVICE_SPECIFIC_ERROR     1066L

//
// MessageId: MBX_PROCESS_ABORTED
//
// MessageText:
//
//  The process terminated unexpectedly.
//
#define MBX_PROCESS_ABORTED            1067L

//
// MessageId: MBX_SERVICE_DEPENDENCY_FAIL
//
// MessageText:
//
//  The dependency service or group failed to start.
//
#define MBX_SERVICE_DEPENDENCY_FAIL    1068L

//
// MessageId: MBX_SERVICE_LOGON_FAILED
//
// MessageText:
//
//  The service did not start due to a logon failure.
//
#define MBX_SERVICE_LOGON_FAILED       1069L

//
// MessageId: MBX_SERVICE_START_HANG
//
// MessageText:
//
//  After starting, the service hung in a start-pending state.
//
#define MBX_SERVICE_START_HANG         1070L

//
// MessageId: MBX_INVALID_SERVICE_LOCK
//
// MessageText:
//
//  The specified service database lock is invalid.
//
#define MBX_INVALID_SERVICE_LOCK       1071L

//
// MessageId: MBX_SERVICE_MARKED_FOR_DELETE
//
// MessageText:
//
//  The specified service has been marked for deletion.
//
#define MBX_SERVICE_MARKED_FOR_DELETE  1072L

//
// MessageId: MBX_SERVICE_EXISTS
//
// MessageText:
//
//  The specified service already exists.
//
#define MBX_SERVICE_EXISTS             1073L

//
// MessageId: MBX_ALREADY_RUNNING_LKG
//
// MessageText:
//
//  The system is currently running with the last-known-good configuration.
//
#define MBX_ALREADY_RUNNING_LKG        1074L

//
// MessageId: MBX_SERVICE_DEPENDENCY_DELETED
//
// MessageText:
//
//  The dependency service does not exist or has been marked for
//  deletion.
//
#define MBX_SERVICE_DEPENDENCY_DELETED 1075L

//
// MessageId: MBX_BOOT_ALREADY_ACCEPTED
//
// MessageText:
//
//  The current boot has already been accepted for use as the
//  last-known-good control set.
//
#define MBX_BOOT_ALREADY_ACCEPTED      1076L

//
// MessageId: MBX_SERVICE_NEVER_STARTED
//
// MessageText:
//
//  No attempts to start the service have been made since the last boot.
//
#define MBX_SERVICE_NEVER_STARTED      1077L

//
// MessageId: MBX_DUPLICATE_SERVICE_NAME
//
// MessageText:
//
//  The name is already in use as either a service name or a service display
//  name.
//
#define MBX_DUPLICATE_SERVICE_NAME     1078L

//
// MessageId: MBX_DIFFERENT_SERVICE_ACCOUNT
//
// MessageText:
//
//  The account specified for this service is different from the account
//  specified for other services running in the same process.
//
#define MBX_DIFFERENT_SERVICE_ACCOUNT  1079L

//
// MessageId: MBX_END_OF_MEDIA
//
// MessageText:
//
//  The physical end of the tape has been reached.
//
#define MBX_END_OF_MEDIA               1100L

//
// MessageId: MBX_FILEMARK_DETECTED
//
// MessageText:
//
//  A tape access reached a filemark.
//
#define MBX_FILEMARK_DETECTED          1101L

//
// MessageId: MBX_BEGINNING_OF_MEDIA
//
// MessageText:
//
//  Beginning of tape or partition was encountered.
//
#define MBX_BEGINNING_OF_MEDIA         1102L

//
// MessageId: MBX_SETMARK_DETECTED
//
// MessageText:
//
//  A tape access reached the end of a set of files.
//
#define MBX_SETMARK_DETECTED           1103L

//
// MessageId: MBX_NO_DATA_DETECTED
//
// MessageText:
//
//  No more data is on the tape.
//
#define MBX_NO_DATA_DETECTED           1104L

//
// MessageId: MBX_PARTITION_FAILURE
//
// MessageText:
//
//  Tape could not be partitioned.
//
#define MBX_PARTITION_FAILURE          1105L

//
// MessageId: MBX_INVALID_BLOCK_LENGTH
//
// MessageText:
//
//  When accessing a new tape of a multivolume partition, the current
//  blocksize is incorrect.
//
#define MBX_INVALID_BLOCK_LENGTH       1106L

//
// MessageId: MBX_DEVICE_NOT_PARTITIONED
//
// MessageText:
//
//  Tape partition information could not be found when loading a tape.
//
#define MBX_DEVICE_NOT_PARTITIONED     1107L

//
// MessageId: MBX_UNABLE_TO_LOCK_MEDIA
//
// MessageText:
//
//  Unable to lock the media eject mechanism.
//
#define MBX_UNABLE_TO_LOCK_MEDIA       1108L

//
// MessageId: MBX_UNABLE_TO_UNLOAD_MEDIA
//
// MessageText:
//
//  Unable to unload the media.
//
#define MBX_UNABLE_TO_UNLOAD_MEDIA     1109L

//
// MessageId: MBX_MEDIA_CHANGED
//
// MessageText:
//
//  Media in drive may have changed.
//
#define MBX_MEDIA_CHANGED              1110L

//
// MessageId: MBX_BUS_RESET
//
// MessageText:
//
//  The I/O bus was reset.
//
#define MBX_BUS_RESET                  1111L

//
// MessageId: MBX_NO_MEDIA_IN_DRIVE
//
// MessageText:
//
//  No media in drive.
//
#define MBX_NO_MEDIA_IN_DRIVE          1112L

//
// MessageId: MBX_NO_UNICODE_TRANSLATION
//
// MessageText:
//
//  No mapping for the Unicode character exists in the target multi-byte code page.
//
#define MBX_NO_UNICODE_TRANSLATION     1113L

//
// MessageId: MBX_DLL_INIT_FAILED
//
// MessageText:
//
//  A dynamic link library (DLL) initialization routine failed.
//
#define MBX_DLL_INIT_FAILED            1114L

//
// MessageId: MBX_SHUTDOWN_IN_PROGRESS
//
// MessageText:
//
//  A system shutdown is in progress.
//
#define MBX_SHUTDOWN_IN_PROGRESS       1115L

//
// MessageId: MBX_NO_SHUTDOWN_IN_PROGRESS
//
// MessageText:
//
//  Unable to abort the system shutdown because no shutdown was in progress.
//
#define MBX_NO_SHUTDOWN_IN_PROGRESS    1116L

//
// MessageId: MBX_IO_DEVICE
//
// MessageText:
//
//  The request could not be performed because of an I/O device error.
//
#define MBX_IO_DEVICE                  1117L

//
// MessageId: MBX_SERIAL_NO_DEVICE
//
// MessageText:
//
//  No serial device was successfully initialized.  The serial driver will unload.
//
#define MBX_SERIAL_NO_DEVICE           1118L

//
// MessageId: MBX_IRQ_BUSY
//
// MessageText:
//
//  Unable to open a device that was sharing an interrupt request (IRQ)
//  with other devices. At least one other device that uses that IRQ
//  was already opened.
//
#define MBX_IRQ_BUSY                   1119L

//
// MessageId: MBX_MORE_WRITES
//
// MessageText:
//
//  A serial I/O operation was completed by another write to the serial port.
//  (The IOCTL_SERIAL_XOFF_COUNTER reached zero.)
//
#define MBX_MORE_WRITES                1120L

//
// MessageId: MBX_COUNTER_TIMEOUT
//
// MessageText:
//
//  A serial I/O operation completed because the time-out period expired.
//  (The IOCTL_SERIAL_XOFF_COUNTER did not reach zero.)
//
#define MBX_COUNTER_TIMEOUT            1121L

//
// MessageId: MBX_FLOPPY_ID_MARK_NOT_FOUND
//
// MessageText:
//
//  No ID address mark was found on the floppy disk.
//
#define MBX_FLOPPY_ID_MARK_NOT_FOUND   1122L

//
// MessageId: MBX_FLOPPY_WRONG_CYLINDER
//
// MessageText:
//
//  Mismatch between the floppy disk sector ID field and the floppy disk
//  controller track address.
//
#define MBX_FLOPPY_WRONG_CYLINDER      1123L

//
// MessageId: MBX_FLOPPY_UNKNOWN_ERROR
//
// MessageText:
//
//  The floppy disk controller reported an error that is not recognized
//  by the floppy disk driver.
//
#define MBX_FLOPPY_UNKNOWN_ERROR       1124L

//
// MessageId: MBX_FLOPPY_BAD_REGISTERS
//
// MessageText:
//
//  The floppy disk controller returned inconsistent results in its registers.
//
#define MBX_FLOPPY_BAD_REGISTERS       1125L

//
// MessageId: MBX_DISK_RECALIBRATE_FAILED
//
// MessageText:
//
//  While accessing the hard disk, a recalibrate operation failed, even after retries.
//
#define MBX_DISK_RECALIBRATE_FAILED    1126L

//
// MessageId: MBX_DISK_OPERATION_FAILED
//
// MessageText:
//
//  While accessing the hard disk, a disk operation failed even after retries.
//
#define MBX_DISK_OPERATION_FAILED      1127L

//
// MessageId: MBX_DISK_RESET_FAILED
//
// MessageText:
//
//  While accessing the hard disk, a disk controller reset was needed, but
//  even that failed.
//
#define MBX_DISK_RESET_FAILED          1128L

//
// MessageId: MBX_EOM_OVERFLOW
//
// MessageText:
//
//  Physical end of tape encountered.
//
#define MBX_EOM_OVERFLOW               1129L

//
// MessageId: MBX_NOT_ENOUGH_SERVER_MEMORY
//
// MessageText:
//
//  Not enough server storage is available to process this command.
//
#define MBX_NOT_ENOUGH_SERVER_MEMORY   1130L

//
// MessageId: MBX_POSSIBLE_DEADLOCK
//
// MessageText:
//
//  A potential deadlock condition has been detected.
//
#define MBX_POSSIBLE_DEADLOCK          1131L

//
// MessageId: MBX_MAPPED_ALIGNMENT
//
// MessageText:
//
//  The base address or the file offset specified does not have the proper
//  alignment.
//
#define MBX_MAPPED_ALIGNMENT           1132L

//
// MessageId: MBX_SET_POWER_STATE_VETOED
//
// MessageText:
//
//  An attempt to change the system power state was vetoed by another
//  application or driver.
//
#define MBX_SET_POWER_STATE_VETOED     1140L

//
// MessageId: MBX_SET_POWER_STATE_FAILED
//
// MessageText:
//
//  The system BIOS failed an attempt to change the system power state.
//
#define MBX_SET_POWER_STATE_FAILED     1141L

//
// MessageId: MBX_TOO_MANY_LINKS
//
// MessageText:
//
//  An attempt was made to create more links on a file than
//  the file system supports.
//
#define MBX_TOO_MANY_LINKS             1142L

//
// MessageId: MBX_OLD_WIN_VERSION
//
// MessageText:
//
//  The specified program requires a newer version of Windows.
//
#define MBX_OLD_WIN_VERSION            1150L

//
// MessageId: MBX_APP_WRONG_OS
//
// MessageText:
//
//  The specified program is not a Windows or MS-DOS program.
//
#define MBX_APP_WRONG_OS               1151L

//
// MessageId: MBX_SINGLE_INSTANCE_APP
//
// MessageText:
//
//  Cannot start more than one instance of the specified program.
//
#define MBX_SINGLE_INSTANCE_APP        1152L

//
// MessageId: MBX_RMODE_APP
//
// MessageText:
//
//  The specified program was written for an older version of Windows.
//
#define MBX_RMODE_APP                  1153L

//
// MessageId: MBX_INVALID_DLL
//
// MessageText:
//
//  One of the library files needed to run this application is damaged.
//
#define MBX_INVALID_DLL                1154L

//
// MessageId: MBX_NO_ASSOCIATION
//
// MessageText:
//
//  No application is associated with the specified file for this operation.
//
#define MBX_NO_ASSOCIATION             1155L

//
// MessageId: MBX_DDE_FAIL
//
// MessageText:
//
//  An error occurred in sending the command to the application.
//
#define MBX_DDE_FAIL                   1156L

//
// MessageId: MBX_DLL_NOT_FOUND
//
// MessageText:
//
//  One of the library files needed to run this application cannot be found.
//
#define MBX_DLL_NOT_FOUND              1157L




///////////////////////////
//                       //
// Winnet32 Status Codes //
//                       //
///////////////////////////


//
// MessageId: MBX_BAD_USERNAME
//
// MessageText:
//
//  The specified username is invalid.
//
#define MBX_BAD_USERNAME               2202L

//
// MessageId: MBX_NOT_CONNECTED
//
// MessageText:
//
//  This network connection does not exist.
//
#define MBX_NOT_CONNECTED              2250L

//
// MessageId: MBX_OPEN_FILES
//
// MessageText:
//
//  This network connection has files open or requests pending.
//
#define MBX_OPEN_FILES                 2401L

//
// MessageId: MBX_ACTIVE_CONNECTIONS
//
// MessageText:
//
//  Active connections still exist.
//
#define MBX_ACTIVE_CONNECTIONS         2402L

//
// MessageId: MBX_DEVICE_IN_USE
//
// MessageText:
//
//  The device is in use by an active process and cannot be disconnected.
//
#define MBX_DEVICE_IN_USE              2404L

//
// MessageId: MBX_BAD_DEVICE
//
// MessageText:
//
//  The specified device name is invalid.
//
#define MBX_BAD_DEVICE                 1200L

//
// MessageId: MBX_CONNECTION_UNAVAIL
//
// MessageText:
//
//  The device is not currently connected but it is a remembered connection.
//
#define MBX_CONNECTION_UNAVAIL         1201L

//
// MessageId: MBX_DEVICE_ALREADY_REMEMBERED
//
// MessageText:
//
//  An attempt was made to remember a device that had previously been remembered.
//
#define MBX_DEVICE_ALREADY_REMEMBERED  1202L

//
// MessageId: MBX_NO_NET_OR_BAD_PATH
//
// MessageText:
//
//  No network provider accepted the given network path.
//
#define MBX_NO_NET_OR_BAD_PATH         1203L

//
// MessageId: MBX_BAD_PROVIDER
//
// MessageText:
//
//  The specified network provider name is invalid.
//
#define MBX_BAD_PROVIDER               1204L

//
// MessageId: MBX_CANNOT_OPEN_PROFILE
//
// MessageText:
//
//  Unable to open the network connection profile.
//
#define MBX_CANNOT_OPEN_PROFILE        1205L

//
// MessageId: MBX_BAD_PROFILE
//
// MessageText:
//
//  The network connection profile is corrupt.
//
#define MBX_BAD_PROFILE                1206L

//
// MessageId: MBX_NOT_CONTAINER
//
// MessageText:
//
//  Cannot enumerate a non-container.
//
#define MBX_NOT_CONTAINER              1207L

//
// MessageId: MBX_EXTENDED_ERROR
//
// MessageText:
//
//  An extended error has occurred.
//
#define MBX_EXTENDED_ERROR             1208L

//
// MessageId: MBX_INVALID_GROUPNAME
//
// MessageText:
//
//  The format of the specified group name is invalid.
//
#define MBX_INVALID_GROUPNAME          1209L

//
// MessageId: MBX_INVALID_COMPUTERNAME
//
// MessageText:
//
//  The format of the specified computer name is invalid.
//
#define MBX_INVALID_COMPUTERNAME       1210L

//
// MessageId: MBX_INVALID_EVENTNAME
//
// MessageText:
//
//  The format of the specified event name is invalid.
//
#define MBX_INVALID_EVENTNAME          1211L

//
// MessageId: MBX_INVALID_DOMAINNAME
//
// MessageText:
//
//  The format of the specified domain name is invalid.
//
#define MBX_INVALID_DOMAINNAME         1212L

//
// MessageId: MBX_INVALID_SERVICENAME
//
// MessageText:
//
//  The format of the specified service name is invalid.
//
#define MBX_INVALID_SERVICENAME        1213L

//
// MessageId: MBX_INVALID_NETNAME
//
// MessageText:
//
//  The format of the specified network name is invalid.
//
#define MBX_INVALID_NETNAME            1214L

//
// MessageId: MBX_INVALID_SHARENAME
//
// MessageText:
//
//  The format of the specified share name is invalid.
//
#define MBX_INVALID_SHARENAME          1215L

//
// MessageId: MBX_INVALID_PASSWORDNAME
//
// MessageText:
//
//  The format of the specified password is invalid.
//
#define MBX_INVALID_PASSWORDNAME       1216L

//
// MessageId: MBX_INVALID_MESSAGENAME
//
// MessageText:
//
//  The format of the specified message name is invalid.
//
#define MBX_INVALID_MESSAGENAME        1217L

//
// MessageId: MBX_INVALID_MESSAGEDEST
//
// MessageText:
//
//  The format of the specified message destination is invalid.
//
#define MBX_INVALID_MESSAGEDEST        1218L

//
// MessageId: MBX_SESSION_CREDENTIAL_CONFLICT
//
// MessageText:
//
//  The credentials supplied conflict with an existing set of credentials.
//
#define MBX_SESSION_CREDENTIAL_CONFLICT 1219L

//
// MessageId: MBX_REMOTE_SESSION_LIMIT_EXCEEDED
//
// MessageText:
//
//  An attempt was made to establish a session to a network server, but there
//  are already too many sessions established to that server.
//
#define MBX_REMOTE_SESSION_LIMIT_EXCEEDED 1220L

//
// MessageId: MBX_DUP_DOMAINNAME
//
// MessageText:
//
//  The workgroup or domain name is already in use by another computer on the
//  network.
//
#define MBX_DUP_DOMAINNAME             1221L

//
// MessageId: MBX_NO_NETWORK
//
// MessageText:
//
//  The network is not present or not started.
//
#define MBX_NO_NETWORK                 1222L

//
// MessageId: MBX_CANCELLED
//
// MessageText:
//
//  The operation was cancelled by the user.
//
#define MBX_CANCELLED                  1223L

//
// MessageId: MBX_USER_MAPPED_FILE
//
// MessageText:
//
//  The requested operation cannot be performed on a file with a user mapped section open.
//
#define MBX_USER_MAPPED_FILE           1224L

//
// MessageId: MBX_CONNECTION_REFUSED
//
// MessageText:
//
//  The remote system refused the network connection.
//
#define MBX_CONNECTION_REFUSED         1225L

//
// MessageId: MBX_GRACEFUL_DISCONNECT
//
// MessageText:
//
//  The network connection was gracefully closed.
//
#define MBX_GRACEFUL_DISCONNECT        1226L

//
// MessageId: MBX_ADDRESS_ALREADY_ASSOCIATED
//
// MessageText:
//
//  The network transport endpoint already has an address associated with it.
//
#define MBX_ADDRESS_ALREADY_ASSOCIATED 1227L

//
// MessageId: MBX_ADDRESS_NOT_ASSOCIATED
//
// MessageText:
//
//  An address has not yet been associated with the network endpoint.
//
#define MBX_ADDRESS_NOT_ASSOCIATED     1228L

//
// MessageId: MBX_CONNECTION_INVALID
//
// MessageText:
//
//  An operation was attempted on a non-existent network connection.
//
#define MBX_CONNECTION_INVALID         1229L

//
// MessageId: MBX_CONNECTION_ACTIVE
//
// MessageText:
//
//  An invalid operation was attempted on an active network connection.
//
#define MBX_CONNECTION_ACTIVE          1230L

//
// MessageId: MBX_NETWORK_UNREACHABLE
//
// MessageText:
//
//  The remote network is not reachable by the transport.
//
#define MBX_NETWORK_UNREACHABLE        1231L

//
// MessageId: MBX_HOST_UNREACHABLE
//
// MessageText:
//
//  The remote system is not reachable by the transport.
//
#define MBX_HOST_UNREACHABLE           1232L

//
// MessageId: MBX_PROTOCOL_UNREACHABLE
//
// MessageText:
//
//  The remote system does not support the transport protocol.
//
#define MBX_PROTOCOL_UNREACHABLE       1233L

//
// MessageId: MBX_PORT_UNREACHABLE
//
// MessageText:
//
//  No service is operating at the destination network endpoint
//  on the remote system.
//
#define MBX_PORT_UNREACHABLE           1234L

//
// MessageId: MBX_REQUEST_ABORTED
//
// MessageText:
//
//  The request was aborted.
//
#define MBX_REQUEST_ABORTED            1235L

//
// MessageId: MBX_CONNECTION_ABORTED
//
// MessageText:
//
//  The network connection was aborted by the local system.
//
#define MBX_CONNECTION_ABORTED         1236L

//
// MessageId: MBX_RETRY
//
// MessageText:
//
//  The operation could not be completed.  A retry should be performed.
//
#define MBX_RETRY                      1237L

//
// MessageId: MBX_CONNECTION_COUNT_LIMIT
//
// MessageText:
//
//  A connection to the server could not be made because the limit on the number of
//  concurrent connections for this account has been reached.
//
#define MBX_CONNECTION_COUNT_LIMIT     1238L

//
// MessageId: MBX_LOGIN_TIME_RESTRICTION
//
// MessageText:
//
//  Attempting to login during an unauthorized time of day for this account.
//
#define MBX_LOGIN_TIME_RESTRICTION     1239L

//
// MessageId: MBX_LOGIN_WKSTA_RESTRICTION
//
// MessageText:
//
//  The account is not authorized to login from this station.
//
#define MBX_LOGIN_WKSTA_RESTRICTION    1240L

//
// MessageId: MBX_INCORRECT_ADDRESS
//
// MessageText:
//
//  The network address could not be used for the operation requested.
//
#define MBX_INCORRECT_ADDRESS          1241L

//
// MessageId: MBX_ALREADY_REGISTERED
//
// MessageText:
//
//  The service is already registered.
//
#define MBX_ALREADY_REGISTERED         1242L

//
// MessageId: MBX_SERVICE_NOT_FOUND
//
// MessageText:
//
//  The specified service does not exist.
//
#define MBX_SERVICE_NOT_FOUND          1243L

//
// MessageId: MBX_NOT_AUTHENTICATED
//
// MessageText:
//
//  The operation being requested was not performed because the user
//  has not been authenticated.
//
#define MBX_NOT_AUTHENTICATED          1244L

//
// MessageId: MBX_NOT_LOGGED_ON
//
// MessageText:
//
//  The operation being requested was not performed because the user
//  has not logged on to the network.
//  The specified service does not exist.
//
#define MBX_NOT_LOGGED_ON              1245L

//
// MessageId: MBX_CONTINUE
//
// MessageText:
//
//  Return that wants caller to continue with work in progress.
//
#define MBX_CONTINUE                   1246L

//
// MessageId: MBX_ALREADY_INITIALIZED
//
// MessageText:
//
//  An attempt was made to perform an initialization operation when
//  initialization has already been completed.
//
#define MBX_ALREADY_INITIALIZED        1247L

//
// MessageId: MBX_NO_MORE_DEVICES
//
// MessageText:
//
//  No more local devices.
//
#define MBX_NO_MORE_DEVICES            1248L

///////////////////////////
//                       //
// Security Status Codes //
//                       //
///////////////////////////

//
// MessageId: MBX_NOT_ALL_ASSIGNED
//
// MessageText:
//
//  Not all privileges referenced are assigned to the caller.
//
#define MBX_NOT_ALL_ASSIGNED           1300L

//
// MessageId: MBX_SOME_NOT_MAPPED
//
// MessageText:
//
//  Some mapping between account names and security IDs was not done.
//
#define MBX_SOME_NOT_MAPPED            1301L

//
// MessageId: MBX_NO_QUOTAS_FOR_ACCOUNT
//
// MessageText:
//
//  No system quota limits are specifically set for this account.
//
#define MBX_NO_QUOTAS_FOR_ACCOUNT      1302L

//
// MessageId: MBX_LOCAL_USER_SESSION_KEY
//
// MessageText:
//
//  No encryption key is available.  A well-known encryption key was returned.
//
#define MBX_LOCAL_USER_SESSION_KEY     1303L

//
// MessageId: MBX_NULL_LM_PASSWORD
//
// MessageText:
//
//  The NT password is too complex to be converted to a LAN Manager
//  password.  The LAN Manager password returned is a NULL string.
//
#define MBX_NULL_LM_PASSWORD           1304L

//
// MessageId: MBX_UNKNOWN_REVISION
//
// MessageText:
//
//  The revision level is unknown.
//
#define MBX_UNKNOWN_REVISION           1305L

//
// MessageId: MBX_REVISION_MISMATCH
//
// MessageText:
//
//  Indicates two revision levels are incompatible.
//
#define MBX_REVISION_MISMATCH          1306L

//
// MessageId: MBX_INVALID_OWNER
//
// MessageText:
//
//  This security ID may not be assigned as the owner of this object.
//
#define MBX_INVALID_OWNER              1307L

//
// MessageId: MBX_INVALID_PRIMARY_GROUP
//
// MessageText:
//
//  This security ID may not be assigned as the primary group of an object.
//
#define MBX_INVALID_PRIMARY_GROUP      1308L

//
// MessageId: MBX_NO_IMPERSONATION_TOKEN
//
// MessageText:
//
//  An attempt has been made to operate on an impersonation token
//  by a thread that is not currently impersonating a client.
//
#define MBX_NO_IMPERSONATION_TOKEN     1309L

//
// MessageId: MBX_CANT_DISABLE_MANDATORY
//
// MessageText:
//
//  The group may not be disabled.
//
#define MBX_CANT_DISABLE_MANDATORY     1310L

//
// MessageId: MBX_NO_LOGON_SERVERS
//
// MessageText:
//
//  There are currently no logon servers available to service the logon
//  request.
//
#define MBX_NO_LOGON_SERVERS           1311L

//
// MessageId: MBX_NO_SUCH_LOGON_SESSION
//
// MessageText:
//
//   A specified logon session does not exist.  It may already have
//   been terminated.
//
#define MBX_NO_SUCH_LOGON_SESSION      1312L

//
// MessageId: MBX_NO_SUCH_PRIVILEGE
//
// MessageText:
//
//   A specified privilege does not exist.
//
#define MBX_NO_SUCH_PRIVILEGE          1313L

//
// MessageId: MBX_PRIVILEGE_NOT_HELD
//
// MessageText:
//
//   A required privilege is not held by the client.
//
#define MBX_PRIVILEGE_NOT_HELD         1314L

//
// MessageId: MBX_INVALID_ACCOUNT_NAME
//
// MessageText:
//
//  The name provided is not a properly formed account name.
//
#define MBX_INVALID_ACCOUNT_NAME       1315L

//
// MessageId: MBX_USER_EXISTS
//
// MessageText:
//
//  The specified user already exists.
//
#define MBX_USER_EXISTS                1316L

//
// MessageId: MBX_NO_SUCH_USER
//
// MessageText:
//
//  The specified user does not exist.
//
#define MBX_NO_SUCH_USER               1317L

//
// MessageId: MBX_GROUP_EXISTS
//
// MessageText:
//
//  The specified group already exists.
//
#define MBX_GROUP_EXISTS               1318L

//
// MessageId: MBX_NO_SUCH_GROUP
//
// MessageText:
//
//  The specified group does not exist.
//
#define MBX_NO_SUCH_GROUP              1319L

//
// MessageId: MBX_MEMBER_IN_GROUP
//
// MessageText:
//
//  Either the specified user account is already a member of the specified
//  group, or the specified group cannot be deleted because it contains
//  a member.
//
#define MBX_MEMBER_IN_GROUP            1320L

//
// MessageId: MBX_MEMBER_NOT_IN_GROUP
//
// MessageText:
//
//  The specified user account is not a member of the specified group account.
//
#define MBX_MEMBER_NOT_IN_GROUP        1321L

//
// MessageId: MBX_LAST_ADMIN
//
// MessageText:
//
//  The last remaining administration account cannot be disabled
//  or deleted.
//
#define MBX_LAST_ADMIN                 1322L

//
// MessageId: MBX_WRONG_PASSWORD
//
// MessageText:
//
//  Unable to update the password.  The value provided as the current
//  password is incorrect.
//
#define MBX_WRONG_PASSWORD             1323L

//
// MessageId: MBX_ILL_FORMED_PASSWORD
//
// MessageText:
//
//  Unable to update the password.  The value provided for the new password
//  contains values that are not allowed in passwords.
//
#define MBX_ILL_FORMED_PASSWORD        1324L

//
// MessageId: MBX_PASSWORD_RESTRICTION
//
// MessageText:
//
//  Unable to update the password because a password update rule has been
//  violated.
//
#define MBX_PASSWORD_RESTRICTION       1325L

//
// MessageId: MBX_LOGON_FAILURE
//
// MessageText:
//
//  Logon failure: unknown user name or bad password.
//
#define MBX_LOGON_FAILURE              1326L

//
// MessageId: MBX_ACCOUNT_RESTRICTION
//
// MessageText:
//
//  Logon failure: user account restriction.
//
#define MBX_ACCOUNT_RESTRICTION        1327L

//
// MessageId: MBX_INVALID_LOGON_HOURS
//
// MessageText:
//
//  Logon failure: account logon time restriction violation.
//
#define MBX_INVALID_LOGON_HOURS        1328L

//
// MessageId: MBX_INVALID_WORKSTATION
//
// MessageText:
//
//  Logon failure: user not allowed to log on to this computer.
//
#define MBX_INVALID_WORKSTATION        1329L

//
// MessageId: MBX_PASSWORD_EXPIRED
//
// MessageText:
//
//  Logon failure: the specified account password has expired.
//
#define MBX_PASSWORD_EXPIRED           1330L

//
// MessageId: MBX_ACCOUNT_DISABLED
//
// MessageText:
//
//  Logon failure: account currently disabled.
//
#define MBX_ACCOUNT_DISABLED           1331L

//
// MessageId: MBX_NONE_MAPPED
//
// MessageText:
//
//  No mapping between account names and security IDs was done.
//
#define MBX_NONE_MAPPED                1332L

//
// MessageId: MBX_TOO_MANY_LUIDS_REQUESTED
//
// MessageText:
//
//  Too many local user identifiers (LUIDs) were requested at one time.
//
#define MBX_TOO_MANY_LUIDS_REQUESTED   1333L

//
// MessageId: MBX_LUIDS_EXHAUSTED
//
// MessageText:
//
//  No more local user identifiers (LUIDs) are available.
//
#define MBX_LUIDS_EXHAUSTED            1334L

//
// MessageId: MBX_INVALID_SUB_AUTHORITY
//
// MessageText:
//
//  The subauthority part of a security ID is invalid for this particular use.
//
#define MBX_INVALID_SUB_AUTHORITY      1335L

//
// MessageId: MBX_INVALID_ACL
//
// MessageText:
//
//  The access control list (ACL) structure is invalid.
//
#define MBX_INVALID_ACL                1336L

//
// MessageId: MBX_INVALID_SID
//
// MessageText:
//
//  The security ID structure is invalid.
//
#define MBX_INVALID_SID                1337L

//
// MessageId: MBX_INVALID_SECURITY_DESCR
//
// MessageText:
//
//  The security descriptor structure is invalid.
//
#define MBX_INVALID_SECURITY_DESCR     1338L

//
// MessageId: MBX_BAD_INHERITANCE_ACL
//
// MessageText:
//
//  The inherited access control list (ACL) or access control entry (ACE)
//  could not be built.
//
#define MBX_BAD_INHERITANCE_ACL        1340L

//
// MessageId: MBX_SERVER_DISABLED
//
// MessageText:
//
//  The server is currently disabled.
//
#define MBX_SERVER_DISABLED            1341L

//
// MessageId: MBX_SERVER_NOT_DISABLED
//
// MessageText:
//
//  The server is currently enabled.
//
#define MBX_SERVER_NOT_DISABLED        1342L

//
// MessageId: MBX_INVALID_ID_AUTHORITY
//
// MessageText:
//
//  The value provided was an invalid value for an identifier authority.
//
#define MBX_INVALID_ID_AUTHORITY       1343L

//
// MessageId: MBX_ALLOTTED_SPACE_EXCEEDED
//
// MessageText:
//
//  No more memory is available for security information updates.
//
#define MBX_ALLOTTED_SPACE_EXCEEDED    1344L

//
// MessageId: MBX_INVALID_GROUP_ATTRIBUTES
//
// MessageText:
//
//  The specified attributes are invalid, or incompatible with the
//  attributes for the group as a whole.
//
#define MBX_INVALID_GROUP_ATTRIBUTES   1345L

//
// MessageId: MBX_BAD_IMPERSONATION_LEVEL
//
// MessageText:
//
//  Either a required impersonation level was not provided, or the
//  provided impersonation level is invalid.
//
#define MBX_BAD_IMPERSONATION_LEVEL    1346L

//
// MessageId: MBX_CANT_OPEN_ANONYMOUS
//
// MessageText:
//
//  Cannot open an anonymous level security token.
//
#define MBX_CANT_OPEN_ANONYMOUS        1347L

//
// MessageId: MBX_BAD_VALIDATION_CLASS
//
// MessageText:
//
//  The validation information class requested was invalid.
//
#define MBX_BAD_VALIDATION_CLASS       1348L

//
// MessageId: MBX_BAD_TOKEN_TYPE
//
// MessageText:
//
//  The type of the token is inappropriate for its attempted use.
//
#define MBX_BAD_TOKEN_TYPE             1349L

//
// MessageId: MBX_NO_SECURITY_ON_OBJECT
//
// MessageText:
//
//  Unable to perform a security operation on an object
//  which has no associated security.
//
#define MBX_NO_SECURITY_ON_OBJECT      1350L

//
// MessageId: MBX_CANT_ACCESS_DOMAIN_INFO
//
// MessageText:
//
//  Indicates a Windows NT Server could not be contacted or that
//  objects within the domain are protected such that necessary
//  information could not be retrieved.
//
#define MBX_CANT_ACCESS_DOMAIN_INFO    1351L

//
// MessageId: MBX_INVALID_SERVER_STATE
//
// MessageText:
//
//  The security account manager (SAM) or local security
//  authority (LSA) server was in the wrong state to perform
//  the security operation.
//
#define MBX_INVALID_SERVER_STATE       1352L

//
// MessageId: MBX_INVALID_DOMAIN_STATE
//
// MessageText:
//
//  The domain was in the wrong state to perform the security operation.
//
#define MBX_INVALID_DOMAIN_STATE       1353L

//
// MessageId: MBX_INVALID_DOMAIN_ROLE
//
// MessageText:
//
//  This operation is only allowed for the Primary Domain Controller of the domain.
//
#define MBX_INVALID_DOMAIN_ROLE        1354L

//
// MessageId: MBX_NO_SUCH_DOMAIN
//
// MessageText:
//
//  The specified domain did not exist.
//
#define MBX_NO_SUCH_DOMAIN             1355L

//
// MessageId: MBX_DOMAIN_EXISTS
//
// MessageText:
//
//  The specified domain already exists.
//
#define MBX_DOMAIN_EXISTS              1356L

//
// MessageId: MBX_DOMAIN_LIMIT_EXCEEDED
//
// MessageText:
//
//  An attempt was made to exceed the limit on the number of domains per server.
//
#define MBX_DOMAIN_LIMIT_EXCEEDED      1357L

//
// MessageId: MBX_INTERNAL_DB_CORRUPTION
//
// MessageText:
//
//  Unable to complete the requested operation because of either a
//  catastrophic media failure or a data structure corruption on the disk.
//
#define MBX_INTERNAL_DB_CORRUPTION     1358L

//
// MessageId: MBX_INTERNAL_ERROR
//
// MessageText:
//
//  The security account database contains an internal inconsistency.
//
#define MBX_INTERNAL_ERROR             1359L

//
// MessageId: MBX_GENERIC_NOT_MAPPED
//
// MessageText:
//
//  Generic access types were contained in an access mask which should
//  already be mapped to non-generic types.
//
#define MBX_GENERIC_NOT_MAPPED         1360L

//
// MessageId: MBX_BAD_DESCRIPTOR_FORMAT
//
// MessageText:
//
//  A security descriptor is not in the right format (absolute or self-relative).
//
#define MBX_BAD_DESCRIPTOR_FORMAT      1361L

//
// MessageId: MBX_NOT_LOGON_PROCESS
//
// MessageText:
//
//  The requested action is restricted for use by logon processes
//  only.  The calling process has not registered as a logon process.
//
#define MBX_NOT_LOGON_PROCESS          1362L

//
// MessageId: MBX_LOGON_SESSION_EXISTS
//
// MessageText:
//
//  Cannot start a new logon session with an ID that is already in use.
//
#define MBX_LOGON_SESSION_EXISTS       1363L

//
// MessageId: MBX_NO_SUCH_PACKAGE
//
// MessageText:
//
//  A specified authentication package is unknown.
//
#define MBX_NO_SUCH_PACKAGE            1364L

//
// MessageId: MBX_BAD_LOGON_SESSION_STATE
//
// MessageText:
//
//  The logon session is not in a state that is consistent with the
//  requested operation.
//
#define MBX_BAD_LOGON_SESSION_STATE    1365L

//
// MessageId: MBX_LOGON_SESSION_COLLISION
//
// MessageText:
//
//  The logon session ID is already in use.
//
#define MBX_LOGON_SESSION_COLLISION    1366L

//
// MessageId: MBX_INVALID_LOGON_TYPE
//
// MessageText:
//
//  A logon request contained an invalid logon type value.
//
#define MBX_INVALID_LOGON_TYPE         1367L

//
// MessageId: MBX_CANNOT_IMPERSONATE
//
// MessageText:
//
//  Unable to impersonate via a named pipe until data has been read
//  from that pipe.
//
#define MBX_CANNOT_IMPERSONATE         1368L

//
// MessageId: MBX_RXACT_INVALID_STATE
//
// MessageText:
//
//  The transaction state of a Registry subtree is incompatible with the
//  requested operation.
//
#define MBX_RXACT_INVALID_STATE        1369L

//
// MessageId: MBX_RXACT_COMMIT_FAILURE
//
// MessageText:
//
//  An internal security database corruption has been encountered.
//
#define MBX_RXACT_COMMIT_FAILURE       1370L

//
// MessageId: MBX_SPECIAL_ACCOUNT
//
// MessageText:
//
//  Cannot perform this operation on built-in accounts.
//
#define MBX_SPECIAL_ACCOUNT            1371L

//
// MessageId: MBX_SPECIAL_GROUP
//
// MessageText:
//
//  Cannot perform this operation on this built-in special group.
//
#define MBX_SPECIAL_GROUP              1372L

//
// MessageId: MBX_SPECIAL_USER
//
// MessageText:
//
//  Cannot perform this operation on this built-in special user.
//
#define MBX_SPECIAL_USER               1373L

//
// MessageId: MBX_MEMBERS_PRIMARY_GROUP
//
// MessageText:
//
//  The user cannot be removed from a group because the group
//  is currently the user's primary group.
//
#define MBX_MEMBERS_PRIMARY_GROUP      1374L

//
// MessageId: MBX_TOKEN_ALREADY_IN_USE
//
// MessageText:
//
//  The token is already in use as a primary token.
//
#define MBX_TOKEN_ALREADY_IN_USE       1375L

//
// MessageId: MBX_NO_SUCH_ALIAS
//
// MessageText:
//
//  The specified local group does not exist.
//
#define MBX_NO_SUCH_ALIAS              1376L

//
// MessageId: MBX_MEMBER_NOT_IN_ALIAS
//
// MessageText:
//
//  The specified account name is not a member of the local group.
//
#define MBX_MEMBER_NOT_IN_ALIAS        1377L

//
// MessageId: MBX_MEMBER_IN_ALIAS
//
// MessageText:
//
//  The specified account name is already a member of the local group.
//
#define MBX_MEMBER_IN_ALIAS            1378L

//
// MessageId: MBX_ALIAS_EXISTS
//
// MessageText:
//
//  The specified local group already exists.
//
#define MBX_ALIAS_EXISTS               1379L

//
// MessageId: MBX_LOGON_NOT_GRANTED
//
// MessageText:
//
//  Logon failure: the user has not been granted the requested
//  logon type at this computer.
//
#define MBX_LOGON_NOT_GRANTED          1380L

//
// MessageId: MBX_TOO_MANY_SECRETS
//
// MessageText:
//
//  The maximum number of secrets that may be stored in a single system has been
//  exceeded.
//
#define MBX_TOO_MANY_SECRETS           1381L

//
// MessageId: MBX_SECRET_TOO_LONG
//
// MessageText:
//
//  The length of a secret exceeds the maximum length allowed.
//
#define MBX_SECRET_TOO_LONG            1382L

//
// MessageId: MBX_INTERNAL_DB_ERROR
//
// MessageText:
//
//  The local security authority database contains an internal inconsistency.
//
#define MBX_INTERNAL_DB_ERROR          1383L

//
// MessageId: MBX_TOO_MANY_CONTEXT_IDS
//
// MessageText:
//
//  During a logon attempt, the user's security context accumulated too many
//  security IDs.
//
#define MBX_TOO_MANY_CONTEXT_IDS       1384L

//
// MessageId: MBX_LOGON_TYPE_NOT_GRANTED
//
// MessageText:
//
//  Logon failure: the user has not been granted the requested logon type
//  at this computer.
//
#define MBX_LOGON_TYPE_NOT_GRANTED     1385L

//
// MessageId: MBX_NT_CROSS_ENCRYPTION_REQUIRED
//
// MessageText:
//
//  A cross-encrypted password is necessary to change a user password.
//
#define MBX_NT_CROSS_ENCRYPTION_REQUIRED 1386L

//
// MessageId: MBX_NO_SUCH_MEMBER
//
// MessageText:
//
//  A new member could not be added to a local group because the member does
//  not exist.
//
#define MBX_NO_SUCH_MEMBER             1387L

//
// MessageId: MBX_INVALID_MEMBER
//
// MessageText:
//
//  A new member could not be added to a local group because the member has the
//  wrong account type.
//
#define MBX_INVALID_MEMBER             1388L

//
// MessageId: MBX_TOO_MANY_SIDS
//
// MessageText:
//
//  Too many security IDs have been specified.
//
#define MBX_TOO_MANY_SIDS              1389L

//
// MessageId: MBX_LM_CROSS_ENCRYPTION_REQUIRED
//
// MessageText:
//
//  A cross-encrypted password is necessary to change this user password.
//
#define MBX_LM_CROSS_ENCRYPTION_REQUIRED 1390L

//
// MessageId: MBX_NO_INHERITANCE
//
// MessageText:
//
//  Indicates an ACL contains no inheritable components
//
#define MBX_NO_INHERITANCE             1391L

//
// MessageId: MBX_FILE_CORRUPT
//
// MessageText:
//
//  The file or directory is corrupt and non-readable.
//
#define MBX_FILE_CORRUPT               1392L

//
// MessageId: MBX_DISK_CORRUPT
//
// MessageText:
//
//  The disk structure is corrupt and non-readable.
//
#define MBX_DISK_CORRUPT               1393L

//
// MessageId: MBX_NO_USER_SESSION_KEY
//
// MessageText:
//
//  There is no user session key for the specified logon session.
//
#define MBX_NO_USER_SESSION_KEY        1394L

//
// MessageId: MBX_LICENSE_QUOTA_EXCEEDED
//
// MessageText:
//
//  The service being accessed is licensed for a particular number of connections.
//  No more connections can be made to the service at this time
//  because there are already as many connections as the service can accept.
//
#define MBX_LICENSE_QUOTA_EXCEEDED     1395L

// End of security error codes


///////////////////////////
//                       //
// WinUser Error Codes   //
//                       //
///////////////////////////


//
// MessageId: MBX_INVALID_WINDOW_HANDLE
//
// MessageText:
//
//  Invalid window handle.
//
#define MBX_INVALID_WINDOW_HANDLE      1400L

//
// MessageId: MBX_INVALID_MENU_HANDLE
//
// MessageText:
//
//  Invalid menu handle.
//
#define MBX_INVALID_MENU_HANDLE        1401L

//
// MessageId: MBX_INVALID_CURSOR_HANDLE
//
// MessageText:
//
//  Invalid cursor handle.
//
#define MBX_INVALID_CURSOR_HANDLE      1402L

//
// MessageId: MBX_INVALID_ACCEL_HANDLE
//
// MessageText:
//
//  Invalid accelerator table handle.
//
#define MBX_INVALID_ACCEL_HANDLE       1403L

//
// MessageId: MBX_INVALID_HOOK_HANDLE
//
// MessageText:
//
//  Invalid hook handle.
//
#define MBX_INVALID_HOOK_HANDLE        1404L

//
// MessageId: MBX_INVALID_DWP_HANDLE
//
// MessageText:
//
//  Invalid handle to a multiple-window position structure.
//
#define MBX_INVALID_DWP_HANDLE         1405L

//
// MessageId: MBX_TLW_WITH_WSCHILD
//
// MessageText:
//
//  Cannot create a top-level child window.
//
#define MBX_TLW_WITH_WSCHILD           1406L

//
// MessageId: MBX_CANNOT_FIND_WND_CLASS
//
// MessageText:
//
//  Cannot find window class.
//
#define MBX_CANNOT_FIND_WND_CLASS      1407L

//
// MessageId: MBX_WINDOW_OF_OTHER_THREAD
//
// MessageText:
//
//  Invalid window, belongs to other thread.
//
#define MBX_WINDOW_OF_OTHER_THREAD     1408L

//
// MessageId: MBX_HOTKEY_ALREADY_REGISTERED
//
// MessageText:
//
//  Hot key is already registered.
//
#define MBX_HOTKEY_ALREADY_REGISTERED  1409L

//
// MessageId: MBX_CLASS_ALREADY_EXISTS
//
// MessageText:
//
//  Class already exists.
//
#define MBX_CLASS_ALREADY_EXISTS       1410L

//
// MessageId: MBX_CLASS_DOES_NOT_EXIST
//
// MessageText:
//
//  Class does not exist.
//
#define MBX_CLASS_DOES_NOT_EXIST       1411L

//
// MessageId: MBX_CLASS_HAS_WINDOWS
//
// MessageText:
//
//  Class still has open windows.
//
#define MBX_CLASS_HAS_WINDOWS          1412L

//
// MessageId: MBX_INVALID_INDEX
//
// MessageText:
//
//  Invalid index.
//
#define MBX_INVALID_INDEX              1413L

//
// MessageId: MBX_INVALID_ICON_HANDLE
//
// MessageText:
//
//  Invalid icon handle.
//
#define MBX_INVALID_ICON_HANDLE        1414L

//
// MessageId: MBX_PRIVATE_DIALOG_INDEX
//
// MessageText:
//
//  Using private DIALOG window words.
//
#define MBX_PRIVATE_DIALOG_INDEX       1415L

//
// MessageId: MBX_LISTBOX_ID_NOT_FOUND
//
// MessageText:
//
//  The listbox identifier was not found.
//
#define MBX_LISTBOX_ID_NOT_FOUND       1416L

//
// MessageId: MBX_NO_WILDCARD_CHARACTERS
//
// MessageText:
//
//  No wildcards were found.
//
#define MBX_NO_WILDCARD_CHARACTERS     1417L

//
// MessageId: MBX_CLIPBOARD_NOT_OPEN
//
// MessageText:
//
//  Thread does not have a clipboard open.
//
#define MBX_CLIPBOARD_NOT_OPEN         1418L

//
// MessageId: MBX_HOTKEY_NOT_REGISTERED
//
// MessageText:
//
//  Hot key is not registered.
//
#define MBX_HOTKEY_NOT_REGISTERED      1419L

//
// MessageId: MBX_WINDOW_NOT_DIALOG
//
// MessageText:
//
//  The window is not a valid dialog window.
//
#define MBX_WINDOW_NOT_DIALOG          1420L

//
// MessageId: MBX_CONTROL_ID_NOT_FOUND
//
// MessageText:
//
//  Control ID not found.
//
#define MBX_CONTROL_ID_NOT_FOUND       1421L

//
// MessageId: MBX_INVALID_COMBOBOX_MESSAGE
//
// MessageText:
//
//  Invalid message for a combo box because it does not have an edit control.
//
#define MBX_INVALID_COMBOBOX_MESSAGE   1422L

//
// MessageId: MBX_WINDOW_NOT_COMBOBOX
//
// MessageText:
//
//  The window is not a combo box.
//
#define MBX_WINDOW_NOT_COMBOBOX        1423L

//
// MessageId: MBX_INVALID_EDIT_HEIGHT
//
// MessageText:
//
//  Height must be less than 256.
//
#define MBX_INVALID_EDIT_HEIGHT        1424L

//
// MessageId: MBX_DC_NOT_FOUND
//
// MessageText:
//
//  Invalid device context (DC) handle.
//
#define MBX_DC_NOT_FOUND               1425L

//
// MessageId: MBX_INVALID_HOOK_FILTER
//
// MessageText:
//
//  Invalid hook procedure type.
//
#define MBX_INVALID_HOOK_FILTER        1426L

//
// MessageId: MBX_INVALID_FILTER_PROC
//
// MessageText:
//
//  Invalid hook procedure.
//
#define MBX_INVALID_FILTER_PROC        1427L

//
// MessageId: MBX_HOOK_NEEDS_HMOD
//
// MessageText:
//
//  Cannot set non-local hook without a module handle.
//
#define MBX_HOOK_NEEDS_HMOD            1428L

//
// MessageId: MBX_GLOBAL_ONLY_HOOK
//
// MessageText:
//
//  This hook procedure can only be set globally.
//
#define MBX_GLOBAL_ONLY_HOOK           1429L

//
// MessageId: MBX_JOURNAL_HOOK_SET
//
// MessageText:
//
//  The journal hook procedure is already installed.
//
#define MBX_JOURNAL_HOOK_SET           1430L

//
// MessageId: MBX_HOOK_NOT_INSTALLED
//
// MessageText:
//
//  The hook procedure is not installed.
//
#define MBX_HOOK_NOT_INSTALLED         1431L

//
// MessageId: MBX_INVALID_LB_MESSAGE
//
// MessageText:
//
//  Invalid message for single-selection listbox.
//
#define MBX_INVALID_LB_MESSAGE         1432L

//
// MessageId: MBX_SETCOUNT_ON_BAD_LB
//
// MessageText:
//
//  LB_SETCOUNT sent to non-lazy listbox.
//
#define MBX_SETCOUNT_ON_BAD_LB         1433L

//
// MessageId: MBX_LB_WITHOUT_TABSTOPS
//
// MessageText:
//
//  This list box does not support tab stops.
//
#define MBX_LB_WITHOUT_TABSTOPS        1434L

//
// MessageId: MBX_DESTROY_OBJECT_OF_OTHER_THREAD
//
// MessageText:
//
//  Cannot destroy object created by another thread.
//
#define MBX_DESTROY_OBJECT_OF_OTHER_THREAD 1435L

//
// MessageId: MBX_CHILD_WINDOW_MENU
//
// MessageText:
//
//  Child windows cannot have menus.
//
#define MBX_CHILD_WINDOW_MENU          1436L

//
// MessageId: MBX_NO_SYSTEM_MENU
//
// MessageText:
//
//  The window does not have a system menu.
//
#define MBX_NO_SYSTEM_MENU             1437L

//
// MessageId: MBX_INVALID_MSGBOX_STYLE
//
// MessageText:
//
//  Invalid message box style.
//
#define MBX_INVALID_MSGBOX_STYLE       1438L

//
// MessageId: MBX_INVALID_SPI_VALUE
//
// MessageText:
//
//  Invalid system-wide (SPI_*) parameter.
//
#define MBX_INVALID_SPI_VALUE          1439L

//
// MessageId: MBX_SCREEN_ALREADY_LOCKED
//
// MessageText:
//
//  Screen already locked.
//
#define MBX_SCREEN_ALREADY_LOCKED      1440L

//
// MessageId: MBX_HWNDS_HAVE_DIFF_PARENT
//
// MessageText:
//
//  All handles to windows in a multiple-window position structure must
//  have the same parent.
//
#define MBX_HWNDS_HAVE_DIFF_PARENT     1441L

//
// MessageId: MBX_NOT_CHILD_WINDOW
//
// MessageText:
//
//  The window is not a child window.
//
#define MBX_NOT_CHILD_WINDOW           1442L

//
// MessageId: MBX_INVALID_GW_COMMAND
//
// MessageText:
//
//  Invalid GW_* command.
//
#define MBX_INVALID_GW_COMMAND         1443L

//
// MessageId: MBX_INVALID_THREAD_ID
//
// MessageText:
//
//  Invalid thread identifier.
//
#define MBX_INVALID_THREAD_ID          1444L

//
// MessageId: MBX_NON_MDICHILD_WINDOW
//
// MessageText:
//
//  Cannot process a message from a window that is not a multiple document
//  interface (MDI) window.
//
#define MBX_NON_MDICHILD_WINDOW        1445L

//
// MessageId: MBX_POPUP_ALREADY_ACTIVE
//
// MessageText:
//
//  Popup menu already active.
//
#define MBX_POPUP_ALREADY_ACTIVE       1446L

//
// MessageId: MBX_NO_SCROLLBARS
//
// MessageText:
//
//  The window does not have scroll bars.
//
#define MBX_NO_SCROLLBARS              1447L

//
// MessageId: MBX_INVALID_SCROLLBAR_RANGE
//
// MessageText:
//
//  Scroll bar range cannot be greater than 0x7FFF.
//
#define MBX_INVALID_SCROLLBAR_RANGE    1448L

//
// MessageId: MBX_INVALID_SHOWWIN_COMMAND
//
// MessageText:
//
//  Cannot show or remove the window in the way specified.
//
#define MBX_INVALID_SHOWWIN_COMMAND    1449L

//
// MessageId: MBX_NO_SYSTEM_RESOURCES
//
// MessageText:
//
//  Insufficient system resources exist to complete the requested service.
//
#define MBX_NO_SYSTEM_RESOURCES        1450L

//
// MessageId: MBX_NONPAGED_SYSTEM_RESOURCES
//
// MessageText:
//
//  Insufficient system resources exist to complete the requested service.
//
#define MBX_NONPAGED_SYSTEM_RESOURCES  1451L

//
// MessageId: MBX_PAGED_SYSTEM_RESOURCES
//
// MessageText:
//
//  Insufficient system resources exist to complete the requested service.
//
#define MBX_PAGED_SYSTEM_RESOURCES     1452L

//
// MessageId: MBX_WORKING_SET_QUOTA
//
// MessageText:
//
//  Insufficient quota to complete the requested service.
//
#define MBX_WORKING_SET_QUOTA          1453L

//
// MessageId: MBX_PAGEFILE_QUOTA
//
// MessageText:
//
//  Insufficient quota to complete the requested service.
//
#define MBX_PAGEFILE_QUOTA             1454L

//
// MessageId: MBX_COMMITMENT_LIMIT
//
// MessageText:
//
//  The paging file is too small for this operation to complete.
//
#define MBX_COMMITMENT_LIMIT           1455L

//
// MessageId: MBX_MENU_ITEM_NOT_FOUND
//
// MessageText:
//
//  A menu item was not found.
//
#define MBX_MENU_ITEM_NOT_FOUND        1456L

//
// MessageId: MBX_INVALID_KEYBOARD_HANDLE
//
// MessageText:
//
//  Invalid keyboard layout handle.
//
#define MBX_INVALID_KEYBOARD_HANDLE    1457L

//
// MessageId: MBX_HOOK_TYPE_NOT_ALLOWED
//
// MessageText:
//
//  Hook type not allowed.
//
#define MBX_HOOK_TYPE_NOT_ALLOWED      1458L

// End of WinUser error codes

///////////////////////////
//                       //
// Eventlog Status Codes //
//                       //
///////////////////////////

//
// MessageId: MBX_EVENTLOG_FILE_CORRUPT
//
// MessageText:
//
//  The event log file is corrupt.
//
#define MBX_EVENTLOG_FILE_CORRUPT      1500L

//
// MessageId: MBX_EVENTLOG_CANT_START
//
// MessageText:
//
//  No event log file could be opened, so the event logging service did not start.
//
#define MBX_EVENTLOG_CANT_START        1501L

//
// MessageId: MBX_LOG_FILE_FULL
//
// MessageText:
//
//  The event log file is full.
//
#define MBX_LOG_FILE_FULL              1502L

//
// MessageId: MBX_EVENTLOG_FILE_CHANGED
//
// MessageText:
//
//  The event log file has changed between reads.
//
#define MBX_EVENTLOG_FILE_CHANGED      1503L

// End of eventlog error codes


///////////////////////////
//                       //
//   RPC Status Codes    //
//                       //
///////////////////////////


//
// MessageId: MBX_RPC_S_INVALID_STRING_BINDING
//
// MessageText:
//
//  The string binding is invalid.
//
#define MBX_RPC_S_INVALID_STRING_BINDING     1700L

//
// MessageId: MBX_RPC_S_WRONG_KIND_OF_BINDING
//
// MessageText:
//
//  The binding handle is not the correct type.
//
#define MBX_RPC_S_WRONG_KIND_OF_BINDING      1701L

//
// MessageId: MBX_RPC_S_INVALID_BINDING
//
// MessageText:
//
//  The binding handle is invalid.
//
#define MBX_RPC_S_INVALID_BINDING            1702L

//
// MessageId: MBX_RPC_S_PROTSEQ_NOT_SUPPORTED
//
// MessageText:
//
//  The RPC protocol sequence is not supported.
//
#define MBX_RPC_S_PROTSEQ_NOT_SUPPORTED      1703L

//
// MessageId: MBX_RPC_S_INVALID_RPC_PROTSEQ
//
// MessageText:
//
//  The RPC protocol sequence is invalid.
//
#define MBX_RPC_S_INVALID_RPC_PROTSEQ        1704L

//
// MessageId: MBX_RPC_S_INVALID_STRING_UUID
//
// MessageText:
//
//  The string universal unique identifier (UUID) is invalid.
//
#define MBX_RPC_S_INVALID_STRING_UUID        1705L

//
// MessageId: MBX_RPC_S_INVALID_ENDPOINT_FORMAT
//
// MessageText:
//
//  The endpoint format is invalid.
//
#define MBX_RPC_S_INVALID_ENDPOINT_FORMAT    1706L

//
// MessageId: MBX_RPC_S_INVALID_NET_ADDR
//
// MessageText:
//
//  The network address is invalid.
//
#define MBX_RPC_S_INVALID_NET_ADDR           1707L

//
// MessageId: MBX_RPC_S_NO_ENDPOINT_FOUND
//
// MessageText:
//
//  No endpoint was found.
//
#define MBX_RPC_S_NO_ENDPOINT_FOUND          1708L

//
// MessageId: MBX_RPC_S_INVALID_TIMEOUT
//
// MessageText:
//
//  The timeout value is invalid.
//
#define MBX_RPC_S_INVALID_TIMEOUT            1709L

//
// MessageId: MBX_RPC_S_OBJECT_NOT_FOUND
//
// MessageText:
//
//  The object universal unique identifier (UUID) was not found.
//
#define MBX_RPC_S_OBJECT_NOT_FOUND           1710L

//
// MessageId: MBX_RPC_S_ALREADY_REGISTERED
//
// MessageText:
//
//  The object universal unique identifier (UUID) has already been registered.
//
#define MBX_RPC_S_ALREADY_REGISTERED         1711L

//
// MessageId: MBX_RPC_S_TYPE_ALREADY_REGISTERED
//
// MessageText:
//
//  The type universal unique identifier (UUID) has already been registered.
//
#define MBX_RPC_S_TYPE_ALREADY_REGISTERED    1712L

//
// MessageId: MBX_RPC_S_ALREADY_LISTENING
//
// MessageText:
//
//  The RPC server is already listening.
//
#define MBX_RPC_S_ALREADY_LISTENING          1713L

//
// MessageId: MBX_RPC_S_NO_PROTSEQS_REGISTERED
//
// MessageText:
//
//  No protocol sequences have been registered.
//
#define MBX_RPC_S_NO_PROTSEQS_REGISTERED     1714L

//
// MessageId: MBX_RPC_S_NOT_LISTENING
//
// MessageText:
//
//  The RPC server is not listening.
//
#define MBX_RPC_S_NOT_LISTENING              1715L

//
// MessageId: MBX_RPC_S_UNKNOWN_MGR_TYPE
//
// MessageText:
//
//  The manager type is unknown.
//
#define MBX_RPC_S_UNKNOWN_MGR_TYPE           1716L

//
// MessageId: MBX_RPC_S_UNKNOWN_IF
//
// MessageText:
//
//  The interface is unknown.
//
#define MBX_RPC_S_UNKNOWN_IF                 1717L

//
// MessageId: MBX_RPC_S_NO_BINDINGS
//
// MessageText:
//
//  There are no bindings.
//
#define MBX_RPC_S_NO_BINDINGS                1718L

//
// MessageId: MBX_RPC_S_NO_PROTSEQS
//
// MessageText:
//
//  There are no protocol sequences.
//
#define MBX_RPC_S_NO_PROTSEQS                1719L

//
// MessageId: MBX_RPC_S_CANT_CREATE_ENDPOINT
//
// MessageText:
//
//  The endpoint cannot be created.
//
#define MBX_RPC_S_CANT_CREATE_ENDPOINT       1720L

//
// MessageId: MBX_RPC_S_OUT_OF_RESOURCES
//
// MessageText:
//
//  Not enough resources are available to complete this operation.
//
#define MBX_RPC_S_OUT_OF_RESOURCES           1721L

//
// MessageId: MBX_RPC_S_SERVER_UNAVAILABLE
//
// MessageText:
//
//  The RPC server is unavailable.
//
#define MBX_RPC_S_SERVER_UNAVAILABLE         1722L

//
// MessageId: MBX_RPC_S_SERVER_TOO_BUSY
//
// MessageText:
//
//  The RPC server is too busy to complete this operation.
//
#define MBX_RPC_S_SERVER_TOO_BUSY            1723L

//
// MessageId: MBX_RPC_S_INVALID_NETWORK_OPTIONS
//
// MessageText:
//
//  The network options are invalid.
//
#define MBX_RPC_S_INVALID_NETWORK_OPTIONS    1724L

//
// MessageId: MBX_RPC_S_NO_CALL_ACTIVE
//
// MessageText:
//
//  There is not a remote procedure call active in this thread.
//
#define MBX_RPC_S_NO_CALL_ACTIVE             1725L

//
// MessageId: MBX_RPC_S_CALL_FAILED
//
// MessageText:
//
//  The remote procedure call failed.
//
#define MBX_RPC_S_CALL_FAILED                1726L

//
// MessageId: MBX_RPC_S_CALL_FAILED_DNE
//
// MessageText:
//
//  The remote procedure call failed and did not execute.
//
#define MBX_RPC_S_CALL_FAILED_DNE            1727L

//
// MessageId: MBX_RPC_S_PROTOCOL_ERROR
//
// MessageText:
//
//  A remote procedure call (RPC) protocol error occurred.
//
#define MBX_RPC_S_PROTOCOL_ERROR             1728L

//
// MessageId: MBX_RPC_S_UNSUPPORTED_TRANS_SYN
//
// MessageText:
//
//  The transfer syntax is not supported by the RPC server.
//
#define MBX_RPC_S_UNSUPPORTED_TRANS_SYN      1730L

//
// MessageId: MBX_RPC_S_UNSUPPORTED_TYPE
//
// MessageText:
//
//  The universal unique identifier (UUID) type is not supported.
//
#define MBX_RPC_S_UNSUPPORTED_TYPE           1732L

//
// MessageId: MBX_RPC_S_INVALID_TAG
//
// MessageText:
//
//  The tag is invalid.
//
#define MBX_RPC_S_INVALID_TAG                1733L

//
// MessageId: MBX_RPC_S_INVALID_BOUND
//
// MessageText:
//
//  The array bounds are invalid.
//
#define MBX_RPC_S_INVALID_BOUND              1734L

//
// MessageId: MBX_RPC_S_NO_ENTRY_NAME
//
// MessageText:
//
//  The binding does not contain an entry name.
//
#define MBX_RPC_S_NO_ENTRY_NAME              1735L

//
// MessageId: MBX_RPC_S_INVALID_NAME_SYNTAX
//
// MessageText:
//
//  The name syntax is invalid.
//
#define MBX_RPC_S_INVALID_NAME_SYNTAX        1736L

//
// MessageId: MBX_RPC_S_UNSUPPORTED_NAME_SYNTAX
//
// MessageText:
//
//  The name syntax is not supported.
//
#define MBX_RPC_S_UNSUPPORTED_NAME_SYNTAX    1737L

//
// MessageId: MBX_RPC_S_UUID_NO_ADDRESS
//
// MessageText:
//
//  No network address is available to use to construct a universal
//  unique identifier (UUID).
//
#define MBX_RPC_S_UUID_NO_ADDRESS            1739L

//
// MessageId: MBX_RPC_S_DUPLICATE_ENDPOINT
//
// MessageText:
//
//  The endpoint is a duplicate.
//
#define MBX_RPC_S_DUPLICATE_ENDPOINT         1740L

//
// MessageId: MBX_RPC_S_UNKNOWN_AUTHN_TYPE
//
// MessageText:
//
//  The authentication type is unknown.
//
#define MBX_RPC_S_UNKNOWN_AUTHN_TYPE         1741L

//
// MessageId: MBX_RPC_S_MAX_CALLS_TOO_SMALL
//
// MessageText:
//
//  The maximum number of calls is too small.
//
#define MBX_RPC_S_MAX_CALLS_TOO_SMALL        1742L

//
// MessageId: MBX_RPC_S_STRING_TOO_LONG
//
// MessageText:
//
//  The string is too long.
//
#define MBX_RPC_S_STRING_TOO_LONG            1743L

//
// MessageId: MBX_RPC_S_PROTSEQ_NOT_FOUND
//
// MessageText:
//
//  The RPC protocol sequence was not found.
//
#define MBX_RPC_S_PROTSEQ_NOT_FOUND          1744L

//
// MessageId: MBX_RPC_S_PROCNUM_OUT_OF_RANGE
//
// MessageText:
//
//  The procedure number is out of range.
//
#define MBX_RPC_S_PROCNUM_OUT_OF_RANGE       1745L

//
// MessageId: MBX_RPC_S_BINDING_HAS_NO_AUTH
//
// MessageText:
//
//  The binding does not contain any authentication information.
//
#define MBX_RPC_S_BINDING_HAS_NO_AUTH        1746L

//
// MessageId: MBX_RPC_S_UNKNOWN_AUTHN_SERVICE
//
// MessageText:
//
//  The authentication service is unknown.
//
#define MBX_RPC_S_UNKNOWN_AUTHN_SERVICE      1747L

//
// MessageId: MBX_RPC_S_UNKNOWN_AUTHN_LEVEL
//
// MessageText:
//
//  The authentication level is unknown.
//
#define MBX_RPC_S_UNKNOWN_AUTHN_LEVEL        1748L

//
// MessageId: MBX_RPC_S_INVALID_AUTH_IDENTITY
//
// MessageText:
//
//  The security context is invalid.
//
#define MBX_RPC_S_INVALID_AUTH_IDENTITY      1749L

//
// MessageId: MBX_RPC_S_UNKNOWN_AUTHZ_SERVICE
//
// MessageText:
//
//  The authorization service is unknown.
//
#define MBX_RPC_S_UNKNOWN_AUTHZ_SERVICE      1750L

//
// MessageId: MBX_EPT_S_INVALID_ENTRY
//
// MessageText:
//
//  The entry is invalid.
//
#define MBX_EPT_S_INVALID_ENTRY              1751L

//
// MessageId: MBX_EPT_S_CANT_PERFORM_OP
//
// MessageText:
//
//  The server endpoint cannot perform the operation.
//
#define MBX_EPT_S_CANT_PERFORM_OP            1752L

//
// MessageId: MBX_EPT_S_NOT_REGISTERED
//
// MessageText:
//
//  There are no more endpoints available from the endpoint mapper.
//
#define MBX_EPT_S_NOT_REGISTERED             1753L

//
// MessageId: MBX_RPC_S_NOTHING_TO_EXPORT
//
// MessageText:
//
//  No interfaces have been exported.
//
#define MBX_RPC_S_NOTHING_TO_EXPORT          1754L

//
// MessageId: MBX_RPC_S_INCOMPLETE_NAME
//
// MessageText:
//
//  The entry name is incomplete.
//
#define MBX_RPC_S_INCOMPLETE_NAME            1755L

//
// MessageId: MBX_RPC_S_INVALID_VERS_OPTION
//
// MessageText:
//
//  The version option is invalid.
//
#define MBX_RPC_S_INVALID_VERS_OPTION        1756L

//
// MessageId: MBX_RPC_S_NO_MORE_MEMBERS
//
// MessageText:
//
//  There are no more members.
//
#define MBX_RPC_S_NO_MORE_MEMBERS            1757L

//
// MessageId: MBX_RPC_S_NOT_ALL_OBJS_UNEXPORTED
//
// MessageText:
//
//  There is nothing to unexport.
//
#define MBX_RPC_S_NOT_ALL_OBJS_UNEXPORTED    1758L

//
// MessageId: MBX_RPC_S_INTERFACE_NOT_FOUND
//
// MessageText:
//
//  The interface was not found.
//
#define MBX_RPC_S_INTERFACE_NOT_FOUND        1759L

//
// MessageId: MBX_RPC_S_ENTRY_ALREADY_EXISTS
//
// MessageText:
//
//  The entry already exists.
//
#define MBX_RPC_S_ENTRY_ALREADY_EXISTS       1760L

//
// MessageId: MBX_RPC_S_ENTRY_NOT_FOUND
//
// MessageText:
//
//  The entry is not found.
//
#define MBX_RPC_S_ENTRY_NOT_FOUND            1761L

//
// MessageId: MBX_RPC_S_NAME_SERVICE_UNAVAILABLE
//
// MessageText:
//
//  The name service is unavailable.
//
#define MBX_RPC_S_NAME_SERVICE_UNAVAILABLE   1762L

//
// MessageId: MBX_RPC_S_INVALID_NAF_ID
//
// MessageText:
//
//  The network address family is invalid.
//
#define MBX_RPC_S_INVALID_NAF_ID             1763L

//
// MessageId: MBX_RPC_S_CANNOT_SUPPORT
//
// MessageText:
//
//  The requested operation is not supported.
//
#define MBX_RPC_S_CANNOT_SUPPORT             1764L

//
// MessageId: MBX_RPC_S_NO_CONTEXT_AVAILABLE
//
// MessageText:
//
//  No security context is available to allow impersonation.
//
#define MBX_RPC_S_NO_CONTEXT_AVAILABLE       1765L

//
// MessageId: MBX_RPC_S_INTERNAL_ERROR
//
// MessageText:
//
//  An internal error occurred in a remote procedure call (RPC).
//
#define MBX_RPC_S_INTERNAL_ERROR             1766L

//
// MessageId: MBX_RPC_S_ZERO_DIVIDE
//
// MessageText:
//
//  The RPC server attempted an integer division by zero.
//
#define MBX_RPC_S_ZERO_DIVIDE                1767L

//
// MessageId: MBX_RPC_S_ADDRESS_ERROR
//
// MessageText:
//
//  An addressing error occurred in the RPC server.
//
#define MBX_RPC_S_ADDRESS_ERROR              1768L

//
// MessageId: MBX_RPC_S_FP_DIV_ZERO
//
// MessageText:
//
//  A floating-point operation at the RPC server caused a division by zero.
//
#define MBX_RPC_S_FP_DIV_ZERO                1769L

//
// MessageId: MBX_RPC_S_FP_UNDERFLOW
//
// MessageText:
//
//  A floating-point underflow occurred at the RPC server.
//
#define MBX_RPC_S_FP_UNDERFLOW               1770L

//
// MessageId: MBX_RPC_S_FP_OVERFLOW
//
// MessageText:
//
//  A floating-point overflow occurred at the RPC server.
//
#define MBX_RPC_S_FP_OVERFLOW                1771L

//
// MessageId: MBX_RPC_X_NO_MORE_ENTRIES
//
// MessageText:
//
//  The list of RPC servers available for the binding of auto handles
//  has been exhausted.
//
#define MBX_RPC_X_NO_MORE_ENTRIES            1772L

//
// MessageId: MBX_RPC_X_SS_CHAR_TRANS_OPEN_FAIL
//
// MessageText:
//
//  Unable to open the character translation table file.
//
#define MBX_RPC_X_SS_CHAR_TRANS_OPEN_FAIL    1773L

//
// MessageId: MBX_RPC_X_SS_CHAR_TRANS_SHORT_FILE
//
// MessageText:
//
//  The file containing the character translation table has fewer than
//  512 bytes.
//
#define MBX_RPC_X_SS_CHAR_TRANS_SHORT_FILE   1774L

//
// MessageId: MBX_RPC_X_SS_IN_NULL_CONTEXT
//
// MessageText:
//
//  A null context handle was passed from the client to the host during
//  a remote procedure call.
//
#define MBX_RPC_X_SS_IN_NULL_CONTEXT         1775L

//
// MessageId: MBX_RPC_X_SS_CONTEXT_DAMAGED
//
// MessageText:
//
//  The context handle changed during a remote procedure call.
//
#define MBX_RPC_X_SS_CONTEXT_DAMAGED         1777L

//
// MessageId: MBX_RPC_X_SS_HANDLES_MISMATCH
//
// MessageText:
//
//  The binding handles passed to a remote procedure call do not match.
//
#define MBX_RPC_X_SS_HANDLES_MISMATCH        1778L

//
// MessageId: MBX_RPC_X_SS_CANNOT_GET_CALL_HANDLE
//
// MessageText:
//
//  The stub is unable to get the remote procedure call handle.
//
#define MBX_RPC_X_SS_CANNOT_GET_CALL_HANDLE  1779L

//
// MessageId: MBX_RPC_X_NULL_REF_POINTER
//
// MessageText:
//
//  A null reference pointer was passed to the stub.
//
#define MBX_RPC_X_NULL_REF_POINTER           1780L

//
// MessageId: MBX_RPC_X_ENUM_VALUE_OUT_OF_RANGE
//
// MessageText:
//
//  The enumeration value is out of range.
//
#define MBX_RPC_X_ENUM_VALUE_OUT_OF_RANGE    1781L

//
// MessageId: MBX_RPC_X_BYTE_COUNT_TOO_SMALL
//
// MessageText:
//
//  The byte count is too small.
//
#define MBX_RPC_X_BYTE_COUNT_TOO_SMALL       1782L

//
// MessageId: MBX_RPC_X_BAD_STUB_DATA
//
// MessageText:
//
//  The stub received bad data.
//
#define MBX_RPC_X_BAD_STUB_DATA              1783L

//
// MessageId: MBX_INVALID_USER_BUFFER
//
// MessageText:
//
//  The supplied user buffer is not valid for the requested operation.
//
#define MBX_INVALID_USER_BUFFER        1784L

//
// MessageId: MBX_UNRECOGNIZED_MEDIA
//
// MessageText:
//
//  The disk media is not recognized.  It may not be formatted.
//
#define MBX_UNRECOGNIZED_MEDIA         1785L

//
// MessageId: MBX_NO_TRUST_LSA_SECRET
//
// MessageText:
//
//  The workstation does not have a trust secret.
//
#define MBX_NO_TRUST_LSA_SECRET        1786L

//
// MessageId: MBX_NO_TRUST_SAM_ACCOUNT
//
// MessageText:
//
//  The SAM database on the Windows NT Server does not have a computer
//  account for this workstation trust relationship.
//
#define MBX_NO_TRUST_SAM_ACCOUNT       1787L

//
// MessageId: MBX_TRUSTED_DOMAIN_FAILURE
//
// MessageText:
//
//  The trust relationship between the primary domain and the trusted
//  domain failed.
//
#define MBX_TRUSTED_DOMAIN_FAILURE     1788L

//
// MessageId: MBX_TRUSTED_RELATIONSHIP_FAILURE
//
// MessageText:
//
//  The trust relationship between this workstation and the primary
//  domain failed.
//
#define MBX_TRUSTED_RELATIONSHIP_FAILURE 1789L

//
// MessageId: MBX_TRUST_FAILURE
//
// MessageText:
//
//  The network logon failed.
//
#define MBX_TRUST_FAILURE              1790L

//
// MessageId: MBX_RPC_S_CALL_IN_PROGRESS
//
// MessageText:
//
//  A remote procedure call is already in progress for this thread.
//
#define MBX_RPC_S_CALL_IN_PROGRESS           1791L

//
// MessageId: MBX_NETLOGON_NOT_STARTED
//
// MessageText:
//
//  An attempt was made to logon, but the network logon service was not started.
//
#define MBX_NETLOGON_NOT_STARTED       1792L

//
// MessageId: MBX_ACCOUNT_EXPIRED
//
// MessageText:
//
//  The user's account has expired.
//
#define MBX_ACCOUNT_EXPIRED            1793L

//
// MessageId: MBX_REDIRECTOR_HAS_OPEN_HANDLES
//
// MessageText:
//
//  The redirector is in use and cannot be unloaded.
//
#define MBX_REDIRECTOR_HAS_OPEN_HANDLES 1794L

//
// MessageId: MBX_PRINTER_DRIVER_ALREADY_INSTALLED
//
// MessageText:
//
//  The specified printer driver is already installed.
//
#define MBX_PRINTER_DRIVER_ALREADY_INSTALLED 1795L

//
// MessageId: MBX_UNKNOWN_PORT
//
// MessageText:
//
//  The specified port is unknown.
//
#define MBX_UNKNOWN_PORT               1796L

//
// MessageId: MBX_UNKNOWN_PRINTER_DRIVER
//
// MessageText:
//
//  The printer driver is unknown.
//
#define MBX_UNKNOWN_PRINTER_DRIVER     1797L

//
// MessageId: MBX_UNKNOWN_PRINTPROCESSOR
//
// MessageText:
//
//  The print processor is unknown.
//
#define MBX_UNKNOWN_PRINTPROCESSOR     1798L

//
// MessageId: MBX_INVALID_SEPARATOR_FILE
//
// MessageText:
//
//  The specified separator file is invalid.
//
#define MBX_INVALID_SEPARATOR_FILE     1799L

//
// MessageId: MBX_INVALID_PRIORITY
//
// MessageText:
//
//  The specified priority is invalid.
//
#define MBX_INVALID_PRIORITY           1800L

//
// MessageId: MBX_INVALID_PRINTER_NAME
//
// MessageText:
//
//  The printer name is invalid.
//
#define MBX_INVALID_PRINTER_NAME       1801L

//
// MessageId: MBX_PRINTER_ALREADY_EXISTS
//
// MessageText:
//
//  The printer already exists.
//
#define MBX_PRINTER_ALREADY_EXISTS     1802L

//
// MessageId: MBX_INVALID_PRINTER_COMMAND
//
// MessageText:
//
//  The printer command is invalid.
//
#define MBX_INVALID_PRINTER_COMMAND    1803L

//
// MessageId: MBX_INVALID_DATATYPE
//
// MessageText:
//
//  The specified datatype is invalid.
//
#define MBX_INVALID_DATATYPE           1804L

//
// MessageId: MBX_INVALID_ENVIRONMENT
//
// MessageText:
//
//  The Environment specified is invalid.
//
#define MBX_INVALID_ENVIRONMENT        1805L

//
// MessageId: MBX_RPC_S_NO_MORE_BINDINGS
//
// MessageText:
//
//  There are no more bindings.
//
#define MBX_RPC_S_NO_MORE_BINDINGS           1806L

//
// MessageId: MBX_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT
//
// MessageText:
//
//  The account used is an interdomain trust account.  Use your global user account or local user account to access this server.
//
#define MBX_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT 1807L

//
// MessageId: MBX_NOLOGON_WORKSTATION_TRUST_ACCOUNT
//
// MessageText:
//
//  The account used is a Computer Account.  Use your global user account or local user account to access this server.
//
#define MBX_NOLOGON_WORKSTATION_TRUST_ACCOUNT 1808L

//
// MessageId: MBX_NOLOGON_SERVER_TRUST_ACCOUNT
//
// MessageText:
//
//  The account used is an server trust account.  Use your global user account or local user account to access this server.
//
#define MBX_NOLOGON_SERVER_TRUST_ACCOUNT 1809L

//
// MessageId: MBX_DOMAIN_TRUST_INCONSISTENT
//
// MessageText:
//
//  The name or security ID (SID) of the domain specified is inconsistent
//  with the trust information for that domain.
//
#define MBX_DOMAIN_TRUST_INCONSISTENT  1810L

//
// MessageId: MBX_SERVER_HAS_OPEN_HANDLES
//
// MessageText:
//
//  The server is in use and cannot be unloaded.
//
#define MBX_SERVER_HAS_OPEN_HANDLES    1811L

//
// MessageId: MBX_RESOURCE_DATA_NOT_FOUND
//
// MessageText:
//
//  The specified image file did not contain a resource section.
//
#define MBX_RESOURCE_DATA_NOT_FOUND    1812L

//
// MessageId: MBX_RESOURCE_TYPE_NOT_FOUND
//
// MessageText:
//
//  The specified resource type can not be found in the image file.
//
#define MBX_RESOURCE_TYPE_NOT_FOUND    1813L

//
// MessageId: MBX_RESOURCE_NAME_NOT_FOUND
//
// MessageText:
//
//  The specified resource name can not be found in the image file.
//
#define MBX_RESOURCE_NAME_NOT_FOUND    1814L

//
// MessageId: MBX_RESOURCE_LANG_NOT_FOUND
//
// MessageText:
//
//  The specified resource language ID cannot be found in the image file.
//
#define MBX_RESOURCE_LANG_NOT_FOUND    1815L

//
// MessageId: MBX_NOT_ENOUGH_QUOTA
//
// MessageText:
//
//  Not enough quota is available to process this command.
//
#define MBX_NOT_ENOUGH_QUOTA           1816L

//
// MessageId: MBX_RPC_S_NO_INTERFACES
//
// MessageText:
//
//  No interfaces have been registered.
//
#define MBX_RPC_S_NO_INTERFACES              1817L

//
// MessageId: MBX_RPC_S_CALL_CANCELLED
//
// MessageText:
//
//  The server was altered while processing this call.
//
#define MBX_RPC_S_CALL_CANCELLED             1818L

//
// MessageId: MBX_RPC_S_BINDING_INCOMPLETE
//
// MessageText:
//
//  The binding handle does not contain all required information.
//
#define MBX_RPC_S_BINDING_INCOMPLETE         1819L

//
// MessageId: MBX_RPC_S_COMM_FAILURE
//
// MessageText:
//
//  Communications failure.
//
#define MBX_RPC_S_COMM_FAILURE               1820L

//
// MessageId: MBX_RPC_S_UNSUPPORTED_AUTHN_LEVEL
//
// MessageText:
//
//  The requested authentication level is not supported.
//
#define MBX_RPC_S_UNSUPPORTED_AUTHN_LEVEL    1821L

//
// MessageId: MBX_RPC_S_NO_PRINC_NAME
//
// MessageText:
//
//  No principal name registered.
//
#define MBX_RPC_S_NO_PRINC_NAME              1822L

//
// MessageId: MBX_RPC_S_NOT_RPC_ERROR
//
// MessageText:
//
//  The error specified is not a valid Windows NT RPC error code.
//
#define MBX_RPC_S_NOT_RPC_ERROR              1823L

//
// MessageId: MBX_RPC_S_UUID_LOCAL_ONLY
//
// MessageText:
//
//  A UUID that is valid only on this computer has been allocated.
//
#define MBX_RPC_S_UUID_LOCAL_ONLY            1824L

//
// MessageId: MBX_RPC_S_SEC_PKG_ERROR
//
// MessageText:
//
//  A security package specific error occurred.
//
#define MBX_RPC_S_SEC_PKG_ERROR              1825L

//
// MessageId: MBX_RPC_S_NOT_CANCELLED
//
// MessageText:
//
//  Thread is not cancelled.
//
#define MBX_RPC_S_NOT_CANCELLED              1826L

//
// MessageId: MBX_RPC_X_INVALID_ES_ACTION
//
// MessageText:
//
//  Invalid operation on the encoding/decoding handle.
//
#define MBX_RPC_X_INVALID_ES_ACTION          1827L

//
// MessageId: MBX_RPC_X_WRONG_ES_VERSION
//
// MessageText:
//
//  Incompatible version of the serializing package.
//
#define MBX_RPC_X_WRONG_ES_VERSION           1828L

//
// MessageId: MBX_RPC_X_WRONG_STUB_VERSION
//
// MessageText:
//
//  Incompatible version of the RPC stub.
//
#define MBX_RPC_X_WRONG_STUB_VERSION         1829L

//
// MessageId: MBX_RPC_X_INVALID_PIPE_OBJECT
//
// MessageText:
//
//  The idl pipe object is invalid or corrupted.
//
#define MBX_RPC_X_INVALID_PIPE_OBJECT        1830L

//
// MessageId: MBX_RPC_X_INVALID_PIPE_OPERATION
//
// MessageText:
//
//  The operation is invalid for a given idl pipe object.
//
#define MBX_RPC_X_INVALID_PIPE_OPERATION     1831L

//
// MessageId: MBX_RPC_X_WRONG_PIPE_VERSION
//
// MessageText:
//
//  The idl pipe version is not supported.
//
#define MBX_RPC_X_WRONG_PIPE_VERSION         1832L

//
// MessageId: MBX_RPC_S_GROUP_MEMBER_NOT_FOUND
//
// MessageText:
//
//  The group member was not found.
//
#define MBX_RPC_S_GROUP_MEMBER_NOT_FOUND     1898L

//
// MessageId: MBX_EPT_S_CANT_CREATE
//
// MessageText:
//
//  The endpoint mapper database could not be created.
//
#define MBX_EPT_S_CANT_CREATE                1899L

//
// MessageId: MBX_RPC_S_INVALID_OBJECT
//
// MessageText:
//
//  The object universal unique identifier (UUID) is the nil UUID.
//
#define MBX_RPC_S_INVALID_OBJECT             1900L

//
// MessageId: MBX_INVALID_TIME
//
// MessageText:
//
//  The specified time is invalid.
//
#define MBX_INVALID_TIME               1901L

//
// MessageId: MBX_INVALID_FORM_NAME
//
// MessageText:
//
//  The specified Form name is invalid.
//
#define MBX_INVALID_FORM_NAME          1902L

//
// MessageId: MBX_INVALID_FORM_SIZE
//
// MessageText:
//
//  The specified Form size is invalid
//
#define MBX_INVALID_FORM_SIZE          1903L

//
// MessageId: MBX_ALREADY_WAITING
//
// MessageText:
//
//  The specified Printer handle is already being waited on
//
#define MBX_ALREADY_WAITING            1904L

//
// MessageId: MBX_PRINTER_DELETED
//
// MessageText:
//
//  The specified Printer has been deleted
//
#define MBX_PRINTER_DELETED            1905L

//
// MessageId: MBX_INVALID_PRINTER_STATE
//
// MessageText:
//
//  The state of the Printer is invalid
//
#define MBX_INVALID_PRINTER_STATE      1906L

//
// MessageId: MBX_PASSWORD_MUST_CHANGE
//
// MessageText:
//
//  The user must change his password before he logs on the first time.
//
#define MBX_PASSWORD_MUST_CHANGE       1907L

//
// MessageId: MBX_DOMAIN_CONTROLLER_NOT_FOUND
//
// MessageText:
//
//  Could not find the domain controller for this domain.
//
#define MBX_DOMAIN_CONTROLLER_NOT_FOUND 1908L

//
// MessageId: MBX_ACCOUNT_LOCKED_OUT
//
// MessageText:
//
//  The referenced account is currently locked out and may not be logged on to.
//
#define MBX_ACCOUNT_LOCKED_OUT         1909L

//
// MessageId: MBX_OR_INVALID_OXID
//
// MessageText:
//
//  The object exporter specified was not found.
//
#define MBX_OR_INVALID_OXID                  1910L

//
// MessageId: MBX_OR_INVALID_OID
//
// MessageText:
//
//  The object specified was not found.
//
#define MBX_OR_INVALID_OID                   1911L

//
// MessageId: MBX_OR_INVALID_SET
//
// MessageText:
//
//  The object resolver set specified was not found.
//
#define MBX_OR_INVALID_SET                   1912L

//
// MessageId: MBX_RPC_S_SEND_INCOMPLETE
//
// MessageText:
//
//  Some data remains to be sent in the request buffer.
//
#define MBX_RPC_S_SEND_INCOMPLETE            1913L

//
// MessageId: MBX_NO_BROWSER_SERVERS_FOUND
//
// MessageText:
//
//  The list of servers for this workgroup is not currently available
//
#define MBX_NO_BROWSER_SERVERS_FOUND   6118L

///////////////////////////
//                       //
//   OpenGL Error Code   //
//                       //
///////////////////////////

//
// MessageId: MBX_INVALID_PIXEL_FORMAT
//
// MessageText:
//
//  The pixel format is invalid.
//
#define MBX_INVALID_PIXEL_FORMAT       2000L

//
// MessageId: MBX_BAD_DRIVER
//
// MessageText:
//
//  The specified driver is invalid.
//
#define MBX_BAD_DRIVER                 2001L

//
// MessageId: MBX_INVALID_WINDOW_STYLE
//
// MessageText:
//
//  The window style or class attribute is invalid for this operation.
//
#define MBX_INVALID_WINDOW_STYLE       2002L

//
// MessageId: MBX_METAFILE_NOT_SUPPORTED
//
// MessageText:
//
//  The requested metafile operation is not supported.
//
#define MBX_METAFILE_NOT_SUPPORTED     2003L

//
// MessageId: MBX_TRANSFORM_NOT_SUPPORTED
//
// MessageText:
//
//  The requested transformation operation is not supported.
//
#define MBX_TRANSFORM_NOT_SUPPORTED    2004L

//
// MessageId: MBX_CLIPPING_NOT_SUPPORTED
//
// MessageText:
//
//  The requested clipping operation is not supported.
//
#define MBX_CLIPPING_NOT_SUPPORTED     2005L

// End of OpenGL error codes


////////////////////////////////////
//                                //
//     Win32 Spooler Error Codes  //
//                                //
////////////////////////////////////
//
// MessageId: MBX_UNKNOWN_PRINT_MONITOR
//
// MessageText:
//
//  The specified print monitor is unknown.
//
#define MBX_UNKNOWN_PRINT_MONITOR      3000L

//
// MessageId: MBX_PRINTER_DRIVER_IN_USE
//
// MessageText:
//
//  The specified printer driver is currently in use.
//
#define MBX_PRINTER_DRIVER_IN_USE      3001L

//
// MessageId: MBX_SPOOL_FILE_NOT_FOUND
//
// MessageText:
//
//  The spool file was not found.
//
#define MBX_SPOOL_FILE_NOT_FOUND       3002L

//
// MessageId: MBX_SPL_NO_STARTDOC
//
// MessageText:
//
//  A StartDocPrinter call was not issued.
//
#define MBX_SPL_NO_STARTDOC            3003L

//
// MessageId: MBX_SPL_NO_ADDJOB
//
// MessageText:
//
//  An AddJob call was not issued.
//
#define MBX_SPL_NO_ADDJOB              3004L

//
// MessageId: MBX_PRINT_PROCESSOR_ALREADY_INSTALLED
//
// MessageText:
//
//  The specified print processor has already been installed.
//
#define MBX_PRINT_PROCESSOR_ALREADY_INSTALLED 3005L

//
// MessageId: MBX_PRINT_MONITOR_ALREADY_INSTALLED
//
// MessageText:
//
//  The specified print monitor has already been installed.
//
#define MBX_PRINT_MONITOR_ALREADY_INSTALLED 3006L

//
// MessageId: MBX_INVALID_PRINT_MONITOR
//
// MessageText:
//
//  The specified print monitor does not have the required functions.
//
#define MBX_INVALID_PRINT_MONITOR      3007L

//
// MessageId: MBX_PRINT_MONITOR_IN_USE
//
// MessageText:
//
//  The specified print monitor is currently in use.
//
#define MBX_PRINT_MONITOR_IN_USE       3008L

//
// MessageId: MBX_PRINTER_HAS_JOBS_QUEUED
//
// MessageText:
//
//  The requested operation is not allowed when there are jobs queued to the printer.
//
#define MBX_PRINTER_HAS_JOBS_QUEUED    3009L

//
// MessageId: MBX_SUCCESS_REBOOT_REQUIRED
//
// MessageText:
//
//  The requested operation is successful.  Changes will not be effective until the system is rebooted.
//
#define MBX_SUCCESS_REBOOT_REQUIRED    3010L

//
// MessageId: MBX_SUCCESS_RESTART_REQUIRED
//
// MessageText:
//
//  The requested operation is successful.  Changes will not be effective until the service is restarted.
//
#define MBX_SUCCESS_RESTART_REQUIRED   3011L

////////////////////////////////////
//                                //
//     Wins Error Codes           //
//                                //
////////////////////////////////////
//
// MessageId: MBX_WINS_INTERNAL
//
// MessageText:
//
//  WINS encountered an error while processing the command.
//
#define MBX_WINS_INTERNAL              4000L

//
// MessageId: MBX_CAN_NOT_DEL_LOCAL_WINS
//
// MessageText:
//
//  The local WINS can not be deleted.
//
#define MBX_CAN_NOT_DEL_LOCAL_WINS     4001L

//
// MessageId: MBX_STATIC_INIT
//
// MessageText:
//
//  The importation from the file failed.
//
#define MBX_STATIC_INIT                4002L

//
// MessageId: MBX_INC_BACKUP
//
// MessageText:
//
//  The backup Failed.  Was a full backup done before ?
//
#define MBX_INC_BACKUP                 4003L

//
// MessageId: MBX_FULL_BACKUP
//
// MessageText:
//
//  The backup Failed.  Check the directory that you are backing the database to.
//
#define MBX_FULL_BACKUP                4004L

//
// MessageId: MBX_REC_NON_EXISTENT
//
// MessageText:
//
//  The name does not exist in the WINS database.
//
#define MBX_REC_NON_EXISTENT           4005L

//
// MessageId: MBX_RPL_NOT_ALLOWED
//
// MessageText:
//
//  Replication with a non-configured partner is not allowed.
//
#define MBX_RPL_NOT_ALLOWED            4006L

//
// Codes 0x4000-0x40ff are reserved for OLE
//
//
// Error codes
//
//
// MessageId: MBX_E_UNEXPECTED
//
// MessageText:
//
//  Catastrophic failure
//
#define MBX_E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFFL)

#ifdef _WIN32
//
// MessageId: MBX_E_NOTIMPL
//
// MessageText:
//
//  Not implemented
//
#define MBX_E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)

//
// MessageId: MBX_E_OUTOFMEMORY
//
// MessageText:
//
//  Ran out of memory
//
#define MBX_E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)

//
// MessageId: MBX_E_INVALIDARG
//
// MessageText:
//
//  One or more arguments are invalid
//
#define MBX_E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)

//
// MessageId: MBX_E_NOINTERFACE
//
// MessageText:
//
//  No such interface supported
//
#define MBX_E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)

//
// MessageId: MBX_E_POINTER
//
// MessageText:
//
//  Invalid pointer
//
#define MBX_E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)

//
// MessageId: MBX_E_HANDLE
//
// MessageText:
//
//  Invalid handle
//
#define MBX_E_HANDLE                         _HRESULT_TYPEDEF_(0x80070006L)

//
// MessageId: MBX_E_ABORT
//
// MessageText:
//
//  Operation aborted
//
#define MBX_E_ABORT                          _HRESULT_TYPEDEF_(0x80004004L)

//
// MessageId: MBX_E_FAIL
//
// MessageText:
//
//  Unspecified error
//
#define MBX_E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)

//
// MessageId: MBX_E_ACCESSDENIED
//
// MessageText:
//
//  General access denied error
//
#define MBX_E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80070005L)

#else
//
// MessageId: MBX_E_NOTIMPL
//
// MessageText:
//
//  Not implemented
//
#define MBX_E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80000001L)

//
// MessageId: MBX_E_OUTOFMEMORY
//
// MessageText:
//
//  Ran out of memory
//
#define MBX_E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x80000002L)

//
// MessageId: MBX_E_INVALIDARG
//
// MessageText:
//
//  One or more arguments are invalid
//
#define MBX_E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80000003L)

//
// MessageId: MBX_E_NOINTERFACE
//
// MessageText:
//
//  No such interface supported
//
#define MBX_E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80000004L)

//
// MessageId: MBX_E_POINTER
//
// MessageText:
//
//  Invalid pointer
//
#define MBX_E_POINTER                        _HRESULT_TYPEDEF_(0x80000005L)

//
// MessageId: MBX_E_HANDLE
//
// MessageText:
//
//  Invalid handle
//
#define MBX_E_HANDLE                         _HRESULT_TYPEDEF_(0x80000006L)

//
// MessageId: MBX_E_ABORT
//
// MessageText:
//
//  Operation aborted
//
#define MBX_E_ABORT                          _HRESULT_TYPEDEF_(0x80000007L)

//
// MessageId: MBX_E_FAIL
//
// MessageText:
//
//  Unspecified error
//
#define MBX_E_FAIL                           _HRESULT_TYPEDEF_(0x80000008L)

//
// MessageId: MBX_E_ACCESSDENIED
//
// MessageText:
//
//  General access denied error
//
#define MBX_E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80000009L)

#endif //WIN32
//
// MessageId: MBX_E_PENDING
//
// MessageText:
//
//  The data necessary to complete this operation is not yet available. 
//
#define MBX_E_PENDING                        _HRESULT_TYPEDEF_(0x8000000AL)

//
// MessageId: MBX_CO_E_INIT_TLS
//
// MessageText:
//
//  Thread local storage failure
//
#define MBX_CO_E_INIT_TLS                    _HRESULT_TYPEDEF_(0x80004006L)

//
// MessageId: MBX_CO_E_INIT_SHARED_ALLOCATOR
//
// MessageText:
//
//  Get shared memory allocator failure
//
#define MBX_CO_E_INIT_SHARED_ALLOCATOR       _HRESULT_TYPEDEF_(0x80004007L)

//
// MessageId: MBX_CO_E_INIT_MEMORY_ALLOCATOR
//
// MessageText:
//
//  Get memory allocator failure
//
#define MBX_CO_E_INIT_MEMORY_ALLOCATOR       _HRESULT_TYPEDEF_(0x80004008L)

//
// MessageId: MBX_CO_E_INIT_CLASS_CACHE
//
// MessageText:
//
//  Unable to initialize class cache
//
#define MBX_CO_E_INIT_CLASS_CACHE            _HRESULT_TYPEDEF_(0x80004009L)

//
// MessageId: MBX_CO_E_INIT_RPC_CHANNEL
//
// MessageText:
//
//  Unable to initialize RPC services
//
#define MBX_CO_E_INIT_RPC_CHANNEL            _HRESULT_TYPEDEF_(0x8000400AL)

//
// MessageId: MBX_CO_E_INIT_TLS_SET_CHANNEL_CONTROL
//
// MessageText:
//
//  Cannot set thread local storage channel control
//
#define MBX_CO_E_INIT_TLS_SET_CHANNEL_CONTROL _HRESULT_TYPEDEF_(0x8000400BL)

//
// MessageId: MBX_CO_E_INIT_TLS_CHANNEL_CONTROL
//
// MessageText:
//
//  Could not allocate thread local storage channel control
//
#define MBX_CO_E_INIT_TLS_CHANNEL_CONTROL    _HRESULT_TYPEDEF_(0x8000400CL)

//
// MessageId: MBX_CO_E_INIT_UNACCEPTED_USER_ALLOCATOR
//
// MessageText:
//
//  The user supplied memory allocator is unacceptable
//
#define MBX_CO_E_INIT_UNACCEPTED_USER_ALLOCATOR _HRESULT_TYPEDEF_(0x8000400DL)

//
// MessageId: MBX_CO_E_INIT_SCM_MUTEX_EXISTS
//
// MessageText:
//
//  The OLE service mutex already exists
//
#define MBX_CO_E_INIT_SCM_MUTEX_EXISTS       _HRESULT_TYPEDEF_(0x8000400EL)

//
// MessageId: MBX_CO_E_INIT_SCM_FILE_MAPPING_EXISTS
//
// MessageText:
//
//  The OLE service file mapping already exists
//
#define MBX_CO_E_INIT_SCM_FILE_MAPPING_EXISTS _HRESULT_TYPEDEF_(0x8000400FL)

//
// MessageId: MBX_CO_E_INIT_SCM_MAP_VIEW_OF_FILE
//
// MessageText:
//
//  Unable to map view of file for OLE service
//
#define MBX_CO_E_INIT_SCM_MAP_VIEW_OF_FILE   _HRESULT_TYPEDEF_(0x80004010L)

//
// MessageId: MBX_CO_E_INIT_SCM_EXEC_FAILURE
//
// MessageText:
//
//  Failure attempting to launch OLE service
//
#define MBX_CO_E_INIT_SCM_EXEC_FAILURE       _HRESULT_TYPEDEF_(0x80004011L)

//
// MessageId: MBX_CO_E_INIT_ONLY_SINGLE_THREADED
//
// MessageText:
//
//  There was an attempt to call CoInitialize a second time while single threaded
//
#define MBX_CO_E_INIT_ONLY_SINGLE_THREADED   _HRESULT_TYPEDEF_(0x80004012L)

//
// MessageId: MBX_CO_E_CANT_REMOTE
//
// MessageText:
//
//  A Remote activation was necessary but was not allowed
//
#define MBX_CO_E_CANT_REMOTE                 _HRESULT_TYPEDEF_(0x80004013L)

//
// MessageId: MBX_CO_E_BAD_SERVER_NAME
//
// MessageText:
//
//  A Remote activation was necessary but the server name provided was invalid
//
#define MBX_CO_E_BAD_SERVER_NAME             _HRESULT_TYPEDEF_(0x80004014L)

//
// MessageId: MBX_CO_E_RUNAS_INCOMPATIBLE
//
// MessageText:
//
//  The class is configured to run as a security id different from the caller
//
#define MBX_CO_E_RUNAS_INCOMPATIBLE          _HRESULT_TYPEDEF_(0x80004015L)

//
// MessageId: MBX_CO_E_OLE1DDE_DISABLED
//
// MessageText:
//
//  Use of Ole1 services requiring DDE windows is disabled
//
#define MBX_CO_E_OLE1DDE_DISABLED            _HRESULT_TYPEDEF_(0x80004016L)

//
// MessageId: MBX_CO_E_RUNAS_SYNTAX
//
// MessageText:
//
//  A RunAs specification must be <domain name>\<user name> or simply <user name>
//
#define MBX_CO_E_RUNAS_SYNTAX                _HRESULT_TYPEDEF_(0x80004017L)

//
// MessageId: MBX_CO_E_CREATEPROCESS_FAILURE
//
// MessageText:
//
//  The server process could not be started.  The pathname may be incorrect.
//
#define MBX_CO_E_CREATEPROCESS_FAILURE       _HRESULT_TYPEDEF_(0x80004018L)

//
// MessageId: MBX_CO_E_RUNAS_CREATEPROCESS_FAILURE
//
// MessageText:
//
//  The server process could not be started as the configured identity.  The pathname may be incorrect or unavailable.
//
#define MBX_CO_E_RUNAS_CREATEPROCESS_FAILURE _HRESULT_TYPEDEF_(0x80004019L)

//
// MessageId: MBX_CO_E_RUNAS_LOGON_FAILURE
//
// MessageText:
//
//  The server process could not be started because the configured identity is incorrect.  Check the username and password.
//
#define MBX_CO_E_RUNAS_LOGON_FAILURE         _HRESULT_TYPEDEF_(0x8000401AL)

//
// MessageId: MBX_CO_E_LAUNCH_PERMSSION_DENIED
//
// MessageText:
//
//  The client is not allowed to launch this server.
//
#define MBX_CO_E_LAUNCH_PERMSSION_DENIED     _HRESULT_TYPEDEF_(0x8000401BL)

//
// MessageId: MBX_CO_E_START_SERVICE_FAILURE
//
// MessageText:
//
//  The service providing this server could not be started.
//
#define MBX_CO_E_START_SERVICE_FAILURE       _HRESULT_TYPEDEF_(0x8000401CL)

//
// MessageId: MBX_CO_E_REMOTE_COMMUNICATION_FAILURE
//
// MessageText:
//
//  This computer was unable to communicate with the computer providing the server.
//
#define MBX_CO_E_REMOTE_COMMUNICATION_FAILURE _HRESULT_TYPEDEF_(0x8000401DL)

//
// MessageId: MBX_CO_E_SERVER_START_TIMEOUT
//
// MessageText:
//
//  The server did not respond after being launched.
//
#define MBX_CO_E_SERVER_START_TIMEOUT        _HRESULT_TYPEDEF_(0x8000401EL)

//
// MessageId: MBX_CO_E_CLSREG_INCONSISTENT
//
// MessageText:
//
//  The registration information for this server is inconsistent or incomplete.
//
#define MBX_CO_E_CLSREG_INCONSISTENT         _HRESULT_TYPEDEF_(0x8000401FL)

//
// MessageId: MBX_CO_E_IIDREG_INCONSISTENT
//
// MessageText:
//
//  The registration information for this interface is inconsistent or incomplete.
//
#define MBX_CO_E_IIDREG_INCONSISTENT         _HRESULT_TYPEDEF_(0x80004020L)

//
//
// MessageId: MBX_OLE_E_OLEVERB
//
// MessageText:
//
//  Invalid OLEVERB structure
//
#define MBX_OLE_E_OLEVERB                    _HRESULT_TYPEDEF_(0x80040000L)

//
// MessageId: MBX_OLE_E_ADVF
//
// MessageText:
//
//  Invalid advise flags
//
#define MBX_OLE_E_ADVF                       _HRESULT_TYPEDEF_(0x80040001L)

//
// MessageId: MBX_OLE_E_ENUM_NOMORE
//
// MessageText:
//
//  Can't enumerate any more, because the associated data is missing
//
#define MBX_OLE_E_ENUM_NOMORE                _HRESULT_TYPEDEF_(0x80040002L)

//
// MessageId: MBX_OLE_E_ADVISENOTSUPPORTED
//
// MessageText:
//
//  This implementation doesn't take advises
//
#define MBX_OLE_E_ADVISENOTSUPPORTED         _HRESULT_TYPEDEF_(0x80040003L)

//
// MessageId: MBX_OLE_E_NOCONNECTION
//
// MessageText:
//
//  There is no connection for this connection ID
//
#define MBX_OLE_E_NOCONNECTION               _HRESULT_TYPEDEF_(0x80040004L)

//
// MessageId: MBX_OLE_E_NOTRUNNING
//
// MessageText:
//
//  Need to run the object to perform this operation
//
#define MBX_OLE_E_NOTRUNNING                 _HRESULT_TYPEDEF_(0x80040005L)

//
// MessageId: MBX_OLE_E_NOCACHE
//
// MessageText:
//
//  There is no cache to operate on
//
#define MBX_OLE_E_NOCACHE                    _HRESULT_TYPEDEF_(0x80040006L)

//
// MessageId: MBX_OLE_E_BLANK
//
// MessageText:
//
//  Uninitialized object
//
#define MBX_OLE_E_BLANK                      _HRESULT_TYPEDEF_(0x80040007L)

//
// MessageId: MBX_OLE_E_CLASSDIFF
//
// MessageText:
//
//  Linked object's source class has changed
//
#define MBX_OLE_E_CLASSDIFF                  _HRESULT_TYPEDEF_(0x80040008L)

//
// MessageId: MBX_OLE_E_CANT_GETMONIKER
//
// MessageText:
//
//  Not able to get the moniker of the object
//
#define MBX_OLE_E_CANT_GETMONIKER            _HRESULT_TYPEDEF_(0x80040009L)

//
// MessageId: MBX_OLE_E_CANT_BINDTOSOURCE
//
// MessageText:
//
//  Not able to bind to the source
//
#define MBX_OLE_E_CANT_BINDTOSOURCE          _HRESULT_TYPEDEF_(0x8004000AL)

//
// MessageId: MBX_OLE_E_STATIC
//
// MessageText:
//
//  Object is static; operation not allowed
//
#define MBX_OLE_E_STATIC                     _HRESULT_TYPEDEF_(0x8004000BL)

//
// MessageId: MBX_OLE_E_PROMPTSAVECANCELLED
//
// MessageText:
//
//  User cancelled out of save dialog
//
#define MBX_OLE_E_PROMPTSAVECANCELLED        _HRESULT_TYPEDEF_(0x8004000CL)

//
// MessageId: MBX_OLE_E_INVALIDRECT
//
// MessageText:
//
//  Invalid rectangle
//
#define MBX_OLE_E_INVALIDRECT                _HRESULT_TYPEDEF_(0x8004000DL)

//
// MessageId: MBX_OLE_E_WRONGCOMPOBJ
//
// MessageText:
//
//  compobj.dll is too old for the ole2.dll initialized
//
#define MBX_OLE_E_WRONGCOMPOBJ               _HRESULT_TYPEDEF_(0x8004000EL)

//
// MessageId: MBX_OLE_E_INVALIDHWND
//
// MessageText:
//
//  Invalid window handle
//
#define MBX_OLE_E_INVALIDHWND                _HRESULT_TYPEDEF_(0x8004000FL)

//
// MessageId: MBX_OLE_E_NOT_INPLACEACTIVE
//
// MessageText:
//
//  Object is not in any of the inplace active states
//
#define MBX_OLE_E_NOT_INPLACEACTIVE          _HRESULT_TYPEDEF_(0x80040010L)

//
// MessageId: MBX_OLE_E_CANTCONVERT
//
// MessageText:
//
//  Not able to convert object
//
#define MBX_OLE_E_CANTCONVERT                _HRESULT_TYPEDEF_(0x80040011L)

//
// MessageId: MBX_OLE_E_NOSTORAGE
//
// MessageText:
//
//  Not able to perform the operation because object is not given storage yet
//  
//
#define MBX_OLE_E_NOSTORAGE                  _HRESULT_TYPEDEF_(0x80040012L)

//
// MessageId: MBX_DV_E_FORMATETC
//
// MessageText:
//
//  Invalid FORMATETC structure
//
#define MBX_DV_E_FORMATETC                   _HRESULT_TYPEDEF_(0x80040064L)

//
// MessageId: MBX_DV_E_DVTARGETDEVICE
//
// MessageText:
//
//  Invalid DVTARGETDEVICE structure
//
#define MBX_DV_E_DVTARGETDEVICE              _HRESULT_TYPEDEF_(0x80040065L)

//
// MessageId: MBX_DV_E_STGMEDIUM
//
// MessageText:
//
//  Invalid STDGMEDIUM structure
//
#define MBX_DV_E_STGMEDIUM                   _HRESULT_TYPEDEF_(0x80040066L)

//
// MessageId: MBX_DV_E_STATDATA
//
// MessageText:
//
//  Invalid STATDATA structure
//
#define MBX_DV_E_STATDATA                    _HRESULT_TYPEDEF_(0x80040067L)

//
// MessageId: MBX_DV_E_LINDEX
//
// MessageText:
//
//  Invalid lindex
//
#define MBX_DV_E_LINDEX                      _HRESULT_TYPEDEF_(0x80040068L)

//
// MessageId: MBX_DV_E_TYMED
//
// MessageText:
//
//  Invalid tymed
//
#define MBX_DV_E_TYMED                       _HRESULT_TYPEDEF_(0x80040069L)

//
// MessageId: MBX_DV_E_CLIPFORMAT
//
// MessageText:
//
//  Invalid clipboard format
//
#define MBX_DV_E_CLIPFORMAT                  _HRESULT_TYPEDEF_(0x8004006AL)

//
// MessageId: MBX_DV_E_DVASPECT
//
// MessageText:
//
//  Invalid aspect(s)
//
#define MBX_DV_E_DVASPECT                    _HRESULT_TYPEDEF_(0x8004006BL)

//
// MessageId: MBX_DV_E_DVTARGETDEVICE_SIZE
//
// MessageText:
//
//  tdSize parameter of the DVTARGETDEVICE structure is invalid
//
#define MBX_DV_E_DVTARGETDEVICE_SIZE         _HRESULT_TYPEDEF_(0x8004006CL)

//
// MessageId: MBX_DV_E_NOIVIEWOBJECT
//
// MessageText:
//
//  Object doesn't support IViewObject interface
//
#define MBX_DV_E_NOIVIEWOBJECT               _HRESULT_TYPEDEF_(0x8004006DL)

#define MBX_DRAGDROP_E_FIRST 0x80040100L
#define MBX_DRAGDROP_E_LAST  0x8004010FL
#define MBX_DRAGDROP_S_FIRST 0x00040100L
#define MBX_DRAGDROP_S_LAST  0x0004010FL
//
// MessageId: MBX_DRAGDROP_E_NOTREGISTERED
//
// MessageText:
//
//  Trying to revoke a drop target that has not been registered
//
#define MBX_DRAGDROP_E_NOTREGISTERED         _HRESULT_TYPEDEF_(0x80040100L)

//
// MessageId: MBX_DRAGDROP_E_ALREADYREGISTERED
//
// MessageText:
//
//  This window has already been registered as a drop target
//
#define MBX_DRAGDROP_E_ALREADYREGISTERED     _HRESULT_TYPEDEF_(0x80040101L)

//
// MessageId: MBX_DRAGDROP_E_INVALIDHWND
//
// MessageText:
//
//  Invalid window handle
//
#define MBX_DRAGDROP_E_INVALIDHWND           _HRESULT_TYPEDEF_(0x80040102L)

#define MBX_CLASSFACTORY_E_FIRST  0x80040110L
#define MBX_CLASSFACTORY_E_LAST   0x8004011FL
#define MBX_CLASSFACTORY_S_FIRST  0x00040110L
#define MBX_CLASSFACTORY_S_LAST   0x0004011FL
//
// MessageId: MBX_CLASS_E_NOAGGREGATION
//
// MessageText:
//
//  Class does not support aggregation (or class object is remote)
//
#define MBX_CLASS_E_NOAGGREGATION            _HRESULT_TYPEDEF_(0x80040110L)

//
// MessageId: MBX_CLASS_E_CLASSNOTAVAILABLE
//
// MessageText:
//
//  ClassFactory cannot supply requested class
//
#define MBX_CLASS_E_CLASSNOTAVAILABLE        _HRESULT_TYPEDEF_(0x80040111L)

#define MBX_MARSHAL_E_FIRST  0x80040120L
#define MBX_MARSHAL_E_LAST   0x8004012FL
#define MBX_MARSHAL_S_FIRST  0x00040120L
#define MBX_MARSHAL_S_LAST   0x0004012FL
#define MBX_DATA_E_FIRST     0x80040130L
#define MBX_DATA_E_LAST      0x8004013FL
#define MBX_DATA_S_FIRST     0x00040130L
#define MBX_DATA_S_LAST      0x0004013FL
#define MBX_VIEW_E_FIRST     0x80040140L
#define MBX_VIEW_E_LAST      0x8004014FL
#define MBX_VIEW_S_FIRST     0x00040140L
#define MBX_VIEW_S_LAST      0x0004014FL
//
// MessageId: MBX_VIEW_E_DRAW
//
// MessageText:
//
//  Error drawing view
//
#define MBX_VIEW_E_DRAW                      _HRESULT_TYPEDEF_(0x80040140L)

#define MBX_REGDB_E_FIRST     0x80040150L
#define MBX_REGDB_E_LAST      0x8004015FL
#define MBX_REGDB_S_FIRST     0x00040150L
#define MBX_REGDB_S_LAST      0x0004015FL
//
// MessageId: MBX_REGDB_E_READREGDB
//
// MessageText:
//
//  Could not read key from registry
//
#define MBX_REGDB_E_READREGDB                _HRESULT_TYPEDEF_(0x80040150L)

//
// MessageId: MBX_REGDB_E_WRITEREGDB
//
// MessageText:
//
//  Could not write key to registry
//
#define MBX_REGDB_E_WRITEREGDB               _HRESULT_TYPEDEF_(0x80040151L)

//
// MessageId: MBX_REGDB_E_KEYMISSING
//
// MessageText:
//
//  Could not find the key in the registry
//
#define MBX_REGDB_E_KEYMISSING               _HRESULT_TYPEDEF_(0x80040152L)

//
// MessageId: MBX_REGDB_E_INVALIDVALUE
//
// MessageText:
//
//  Invalid value for registry
//
#define MBX_REGDB_E_INVALIDVALUE             _HRESULT_TYPEDEF_(0x80040153L)

//
// MessageId: MBX_REGDB_E_CLASSNOTREG
//
// MessageText:
//
//  Class not registered
//
#define MBX_REGDB_E_CLASSNOTREG              _HRESULT_TYPEDEF_(0x80040154L)

//
// MessageId: MBX_REGDB_E_IIDNOTREG
//
// MessageText:
//
//  Interface not registered
//
#define MBX_REGDB_E_IIDNOTREG                _HRESULT_TYPEDEF_(0x80040155L)

#define MBX_CACHE_E_FIRST     0x80040170L
#define MBX_CACHE_E_LAST      0x8004017FL
#define MBX_CACHE_S_FIRST     0x00040170L
#define MBX_CACHE_S_LAST      0x0004017FL
//
// MessageId: MBX_CACHE_E_NOCACHE_UPDATED
//
// MessageText:
//
//  Cache not updated
//
#define MBX_CACHE_E_NOCACHE_UPDATED          _HRESULT_TYPEDEF_(0x80040170L)

#define MBX_OLEOBJ_E_FIRST     0x80040180L
#define MBX_OLEOBJ_E_LAST      0x8004018FL
#define MBX_OLEOBJ_S_FIRST     0x00040180L
#define MBX_OLEOBJ_S_LAST      0x0004018FL
//
// MessageId: MBX_OLEOBJ_E_NOVERBS
//
// MessageText:
//
//  No verbs for OLE object
//
#define MBX_OLEOBJ_E_NOVERBS                 _HRESULT_TYPEDEF_(0x80040180L)

//
// MessageId: MBX_OLEOBJ_E_INVALIDVERB
//
// MessageText:
//
//  Invalid verb for OLE object
//
#define MBX_OLEOBJ_E_INVALIDVERB             _HRESULT_TYPEDEF_(0x80040181L)

#define MBX_CLIENTSITE_E_FIRST     0x80040190L
#define MBX_CLIENTSITE_E_LAST      0x8004019FL
#define MBX_CLIENTSITE_S_FIRST     0x00040190L
#define MBX_CLIENTSITE_S_LAST      0x0004019FL
//
// MessageId: MBX_INPLACE_E_NOTUNDOABLE
//
// MessageText:
//
//  Undo is not available
//
#define MBX_INPLACE_E_NOTUNDOABLE            _HRESULT_TYPEDEF_(0x800401A0L)

//
// MessageId: MBX_INPLACE_E_NOTOOLSPACE
//
// MessageText:
//
//  Space for tools is not available
//
#define MBX_INPLACE_E_NOTOOLSPACE            _HRESULT_TYPEDEF_(0x800401A1L)

#define MBX_INPLACE_E_FIRST     0x800401A0L
#define MBX_INPLACE_E_LAST      0x800401AFL
#define MBX_INPLACE_S_FIRST     0x000401A0L
#define MBX_INPLACE_S_LAST      0x000401AFL
#define MBX_ENUM_E_FIRST        0x800401B0L
#define MBX_ENUM_E_LAST         0x800401BFL
#define MBX_ENUM_S_FIRST        0x000401B0L
#define MBX_ENUM_S_LAST         0x000401BFL
#define MBX_CONVERT10_E_FIRST        0x800401C0L
#define MBX_CONVERT10_E_LAST         0x800401CFL
#define MBX_CONVERT10_S_FIRST        0x000401C0L
#define MBX_CONVERT10_S_LAST         0x000401CFL
//
// MessageId: MBX_CONVERT10_E_OLESTREAM_GET
//
// MessageText:
//
//  OLESTREAM Get method failed
//
#define MBX_CONVERT10_E_OLESTREAM_GET        _HRESULT_TYPEDEF_(0x800401C0L)

//
// MessageId: MBX_CONVERT10_E_OLESTREAM_PUT
//
// MessageText:
//
//  OLESTREAM Put method failed
//
#define MBX_CONVERT10_E_OLESTREAM_PUT        _HRESULT_TYPEDEF_(0x800401C1L)

//
// MessageId: MBX_CONVERT10_E_OLESTREAM_FMT
//
// MessageText:
//
//  Contents of the OLESTREAM not in correct format
//
#define MBX_CONVERT10_E_OLESTREAM_FMT        _HRESULT_TYPEDEF_(0x800401C2L)

//
// MessageId: MBX_CONVERT10_E_OLESTREAM_BITMAP_TO_DIB
//
// MessageText:
//
//  There was an error in a Windows GDI call while converting the bitmap to a DIB
//
#define MBX_CONVERT10_E_OLESTREAM_BITMAP_TO_DIB _HRESULT_TYPEDEF_(0x800401C3L)

//
// MessageId: MBX_CONVERT10_E_STG_FMT
//
// MessageText:
//
//  Contents of the IStorage not in correct format
//
#define MBX_CONVERT10_E_STG_FMT              _HRESULT_TYPEDEF_(0x800401C4L)

//
// MessageId: MBX_CONVERT10_E_STG_NO_STD_STREAM
//
// MessageText:
//
//  Contents of IStorage is missing one of the standard streams
//
#define MBX_CONVERT10_E_STG_NO_STD_STREAM    _HRESULT_TYPEDEF_(0x800401C5L)

//
// MessageId: MBX_CONVERT10_E_STG_DIB_TO_BITMAP
//
// MessageText:
//
//  There was an error in a Windows GDI call while converting the DIB to a bitmap.
//  
//
#define MBX_CONVERT10_E_STG_DIB_TO_BITMAP    _HRESULT_TYPEDEF_(0x800401C6L)

#define MBX_CLIPBRD_E_FIRST        0x800401D0L
#define MBX_CLIPBRD_E_LAST         0x800401DFL
#define MBX_CLIPBRD_S_FIRST        0x000401D0L
#define MBX_CLIPBRD_S_LAST         0x000401DFL
//
// MessageId: MBX_CLIPBRD_E_CANT_OPEN
//
// MessageText:
//
//  OpenClipboard Failed
//
#define MBX_CLIPBRD_E_CANT_OPEN              _HRESULT_TYPEDEF_(0x800401D0L)

//
// MessageId: MBX_CLIPBRD_E_CANT_EMPTY
//
// MessageText:
//
//  EmptyClipboard Failed
//
#define MBX_CLIPBRD_E_CANT_EMPTY             _HRESULT_TYPEDEF_(0x800401D1L)

//
// MessageId: MBX_CLIPBRD_E_CANT_SET
//
// MessageText:
//
//  SetClipboard Failed
//
#define MBX_CLIPBRD_E_CANT_SET               _HRESULT_TYPEDEF_(0x800401D2L)

//
// MessageId: MBX_CLIPBRD_E_BAD_DATA
//
// MessageText:
//
//  Data on clipboard is invalid
//
#define MBX_CLIPBRD_E_BAD_DATA               _HRESULT_TYPEDEF_(0x800401D3L)

//
// MessageId: MBX_CLIPBRD_E_CANT_CLOSE
//
// MessageText:
//
//  CloseClipboard Failed
//
#define MBX_CLIPBRD_E_CANT_CLOSE             _HRESULT_TYPEDEF_(0x800401D4L)

#define MBX_MK_E_FIRST        0x800401E0L
#define MBX_MK_E_LAST         0x800401EFL
#define MBX_MK_S_FIRST        0x000401E0L
#define MBX_MK_S_LAST         0x000401EFL
//
// MessageId: MBX_MK_E_CONNECTMANUALLY
//
// MessageText:
//
//  Moniker needs to be connected manually
//
#define MBX_MK_E_CONNECTMANUALLY             _HRESULT_TYPEDEF_(0x800401E0L)

//
// MessageId: MBX_MK_E_EXCEEDEDDEADLINE
//
// MessageText:
//
//  Operation exceeded deadline
//
#define MBX_MK_E_EXCEEDEDDEADLINE            _HRESULT_TYPEDEF_(0x800401E1L)

//
// MessageId: MBX_MK_E_NEEDGENERIC
//
// MessageText:
//
//  Moniker needs to be generic
//
#define MBX_MK_E_NEEDGENERIC                 _HRESULT_TYPEDEF_(0x800401E2L)

//
// MessageId: MBX_MK_E_UNAVAILABLE
//
// MessageText:
//
//  Operation unavailable
//
#define MBX_MK_E_UNAVAILABLE                 _HRESULT_TYPEDEF_(0x800401E3L)

//
// MessageId: MBX_MK_E_SYNTAX
//
// MessageText:
//
//  Invalid syntax
//
#define MBX_MK_E_SYNTAX                      _HRESULT_TYPEDEF_(0x800401E4L)

//
// MessageId: MBX_MK_E_NOOBJECT
//
// MessageText:
//
//  No object for moniker
//
#define MBX_MK_E_NOOBJECT                    _HRESULT_TYPEDEF_(0x800401E5L)

//
// MessageId: MBX_MK_E_INVALIDEXTENSION
//
// MessageText:
//
//  Bad extension for file
//
#define MBX_MK_E_INVALIDEXTENSION            _HRESULT_TYPEDEF_(0x800401E6L)

//
// MessageId: MBX_MK_E_INTERMEDIATEINTERFACENOTSUPPORTED
//
// MessageText:
//
//  Intermediate operation failed
//
#define MBX_MK_E_INTERMEDIATEINTERFACENOTSUPPORTED _HRESULT_TYPEDEF_(0x800401E7L)

//
// MessageId: MBX_MK_E_NOTBINDABLE
//
// MessageText:
//
//  Moniker is not bindable
//
#define MBX_MK_E_NOTBINDABLE                 _HRESULT_TYPEDEF_(0x800401E8L)

//
// MessageId: MBX_MK_E_NOTBOUND
//
// MessageText:
//
//  Moniker is not bound
//
#define MBX_MK_E_NOTBOUND                    _HRESULT_TYPEDEF_(0x800401E9L)

//
// MessageId: MBX_MK_E_CANTOPENFILE
//
// MessageText:
//
//  Moniker cannot open file
//
#define MBX_MK_E_CANTOPENFILE                _HRESULT_TYPEDEF_(0x800401EAL)

//
// MessageId: MBX_MK_E_MUSTBOTHERUSER
//
// MessageText:
//
//  User input required for operation to succeed
//
#define MBX_MK_E_MUSTBOTHERUSER              _HRESULT_TYPEDEF_(0x800401EBL)

//
// MessageId: MBX_MK_E_NOINVERSE
//
// MessageText:
//
//  Moniker class has no inverse
//
#define MBX_MK_E_NOINVERSE                   _HRESULT_TYPEDEF_(0x800401ECL)

//
// MessageId: MBX_MK_E_NOSTORAGE
//
// MessageText:
//
//  Moniker does not refer to storage
//
#define MBX_MK_E_NOSTORAGE                   _HRESULT_TYPEDEF_(0x800401EDL)

//
// MessageId: MBX_MK_E_NOPREFIX
//
// MessageText:
//
//  No common prefix
//
#define MBX_MK_E_NOPREFIX                    _HRESULT_TYPEDEF_(0x800401EEL)

//
// MessageId: MBX_MK_E_ENUMERATION_FAILED
//
// MessageText:
//
//  Moniker could not be enumerated
//
#define MBX_MK_E_ENUMERATION_FAILED          _HRESULT_TYPEDEF_(0x800401EFL)

#define MBX_CO_E_FIRST        0x800401F0L
#define MBX_CO_E_LAST         0x800401FFL
#define MBX_CO_S_FIRST        0x000401F0L
#define MBX_CO_S_LAST         0x000401FFL
//
// MessageId: MBX_CO_E_NOTINITIALIZED
//
// MessageText:
//
//  CoInitialize has not been called.
//
#define MBX_CO_E_NOTINITIALIZED              _HRESULT_TYPEDEF_(0x800401F0L)

//
// MessageId: MBX_CO_E_ALREADYINITIALIZED
//
// MessageText:
//
//  CoInitialize has already been called.
//
#define MBX_CO_E_ALREADYINITIALIZED          _HRESULT_TYPEDEF_(0x800401F1L)

//
// MessageId: MBX_CO_E_CANTDETERMINECLASS
//
// MessageText:
//
//  Class of object cannot be determined
//
#define MBX_CO_E_CANTDETERMINECLASS          _HRESULT_TYPEDEF_(0x800401F2L)

//
// MessageId: MBX_CO_E_CLASSSTRING
//
// MessageText:
//
//  Invalid class string
//
#define MBX_CO_E_CLASSSTRING                 _HRESULT_TYPEDEF_(0x800401F3L)

//
// MessageId: MBX_CO_E_IIDSTRING
//
// MessageText:
//
//  Invalid interface string
//
#define MBX_CO_E_IIDSTRING                   _HRESULT_TYPEDEF_(0x800401F4L)

//
// MessageId: MBX_CO_E_APPNOTFOUND
//
// MessageText:
//
//  Application not found
//
#define MBX_CO_E_APPNOTFOUND                 _HRESULT_TYPEDEF_(0x800401F5L)

//
// MessageId: MBX_CO_E_APPSINGLEUSE
//
// MessageText:
//
//  Application cannot be run more than once
//
#define MBX_CO_E_APPSINGLEUSE                _HRESULT_TYPEDEF_(0x800401F6L)

//
// MessageId: MBX_CO_E_ERRORINAPP
//
// MessageText:
//
//  Some error in application program
//
#define MBX_CO_E_ERRORINAPP                  _HRESULT_TYPEDEF_(0x800401F7L)

//
// MessageId: MBX_CO_E_DLLNOTFOUND
//
// MessageText:
//
//  DLL for class not found
//
#define MBX_CO_E_DLLNOTFOUND                 _HRESULT_TYPEDEF_(0x800401F8L)

//
// MessageId: MBX_CO_E_ERRORINDLL
//
// MessageText:
//
//  Error in the DLL
//
#define MBX_CO_E_ERRORINDLL                  _HRESULT_TYPEDEF_(0x800401F9L)

//
// MessageId: MBX_CO_E_WRONGOSFORAPP
//
// MessageText:
//
//  Wrong OS or OS version for application
//
#define MBX_CO_E_WRONGOSFORAPP               _HRESULT_TYPEDEF_(0x800401FAL)

//
// MessageId: MBX_CO_E_OBJNOTREG
//
// MessageText:
//
//  Object is not registered
//
#define MBX_CO_E_OBJNOTREG                   _HRESULT_TYPEDEF_(0x800401FBL)

//
// MessageId: MBX_CO_E_OBJISREG
//
// MessageText:
//
//  Object is already registered
//
#define MBX_CO_E_OBJISREG                    _HRESULT_TYPEDEF_(0x800401FCL)

//
// MessageId: MBX_CO_E_OBJNOTCONNECTED
//
// MessageText:
//
//  Object is not connected to server
//
#define MBX_CO_E_OBJNOTCONNECTED             _HRESULT_TYPEDEF_(0x800401FDL)

//
// MessageId: MBX_CO_E_APPDIDNTREG
//
// MessageText:
//
//  Application was launched but it didn't register a class factory
//
#define MBX_CO_E_APPDIDNTREG                 _HRESULT_TYPEDEF_(0x800401FEL)

//
// MessageId: MBX_CO_E_RELEASED
//
// MessageText:
//
//  Object has been released
//
#define MBX_CO_E_RELEASED                    _HRESULT_TYPEDEF_(0x800401FFL)

//
// Old OLE Success Codes
//
//
// MessageId: MBX_OLE_S_USEREG
//
// MessageText:
//
//  Use the registry database to provide the requested information
//
#define MBX_OLE_S_USEREG                     _HRESULT_TYPEDEF_(0x00040000L)

//
// MessageId: MBX_OLE_S_STATIC
//
// MessageText:
//
//  Success, but static
//
#define MBX_OLE_S_STATIC                     _HRESULT_TYPEDEF_(0x00040001L)

//
// MessageId: MBX_OLE_S_MAC_CLIPFORMAT
//
// MessageText:
//
//  Macintosh clipboard format
//
#define MBX_OLE_S_MAC_CLIPFORMAT             _HRESULT_TYPEDEF_(0x00040002L)

//
// MessageId: MBX_DRAGDROP_S_DROP
//
// MessageText:
//
//  Successful drop took place
//
#define MBX_DRAGDROP_S_DROP                  _HRESULT_TYPEDEF_(0x00040100L)

//
// MessageId: MBX_DRAGDROP_S_CANCEL
//
// MessageText:
//
//  Drag-drop operation canceled
//
#define MBX_DRAGDROP_S_CANCEL                _HRESULT_TYPEDEF_(0x00040101L)

//
// MessageId: MBX_DRAGDROP_S_USEDEFAULTCURSORS
//
// MessageText:
//
//  Use the default cursor
//
#define MBX_DRAGDROP_S_USEDEFAULTCURSORS     _HRESULT_TYPEDEF_(0x00040102L)

//
// MessageId: MBX_DATA_S_SAMEFORMATETC
//
// MessageText:
//
//  Data has same FORMATETC
//
#define MBX_DATA_S_SAMEFORMATETC             _HRESULT_TYPEDEF_(0x00040130L)

//
// MessageId: MBX_VIEW_S_ALREADY_FROZEN
//
// MessageText:
//
//  View is already frozen
//
#define MBX_VIEW_S_ALREADY_FROZEN            _HRESULT_TYPEDEF_(0x00040140L)

//
// MessageId: MBX_CACHE_S_FORMATETC_NOTSUPPORTED
//
// MessageText:
//
//  FORMATETC not supported
//
#define MBX_CACHE_S_FORMATETC_NOTSUPPORTED   _HRESULT_TYPEDEF_(0x00040170L)

//
// MessageId: MBX_CACHE_S_SAMECACHE
//
// MessageText:
//
//  Same cache
//
#define MBX_CACHE_S_SAMECACHE                _HRESULT_TYPEDEF_(0x00040171L)

//
// MessageId: MBX_CACHE_S_SOMECACHES_NOTUPDATED
//
// MessageText:
//
//  Some cache(s) not updated
//
#define MBX_CACHE_S_SOMECACHES_NOTUPDATED    _HRESULT_TYPEDEF_(0x00040172L)

//
// MessageId: MBX_OLEOBJ_S_INVALIDVERB
//
// MessageText:
//
//  Invalid verb for OLE object
//
#define MBX_OLEOBJ_S_INVALIDVERB             _HRESULT_TYPEDEF_(0x00040180L)

//
// MessageId: MBX_OLEOBJ_S_CANNOT_DOVERB_NOW
//
// MessageText:
//
//  Verb number is valid but verb cannot be done now
//
#define MBX_OLEOBJ_S_CANNOT_DOVERB_NOW       _HRESULT_TYPEDEF_(0x00040181L)

//
// MessageId: MBX_OLEOBJ_S_INVALIDHWND
//
// MessageText:
//
//  Invalid window handle passed
//
#define MBX_OLEOBJ_S_INVALIDHWND             _HRESULT_TYPEDEF_(0x00040182L)

//
// MessageId: MBX_INPLACE_S_TRUNCATED
//
// MessageText:
//
//  Message is too long; some of it had to be truncated before displaying
//
#define MBX_INPLACE_S_TRUNCATED              _HRESULT_TYPEDEF_(0x000401A0L)

//
// MessageId: MBX_CONVERT10_S_NO_PRESENTATION
//
// MessageText:
//
//  Unable to convert OLESTREAM to IStorage
//
#define MBX_CONVERT10_S_NO_PRESENTATION      _HRESULT_TYPEDEF_(0x000401C0L)

//
// MessageId: MBX_MK_S_REDUCED_TO_SELF
//
// MessageText:
//
//  Moniker reduced to itself
//
#define MBX_MK_S_REDUCED_TO_SELF             _HRESULT_TYPEDEF_(0x000401E2L)

//
// MessageId: MBX_MK_S_ME
//
// MessageText:
//
//  Common prefix is this moniker
//
#define MBX_MK_S_ME                          _HRESULT_TYPEDEF_(0x000401E4L)

//
// MessageId: MBX_MK_S_HIM
//
// MessageText:
//
//  Common prefix is input moniker
//
#define MBX_MK_S_HIM                         _HRESULT_TYPEDEF_(0x000401E5L)

//
// MessageId: MBX_MK_S_US
//
// MessageText:
//
//  Common prefix is both monikers
//
#define MBX_MK_S_US                          _HRESULT_TYPEDEF_(0x000401E6L)

//
// MessageId: MBX_MK_S_MONIKERALREADYREGISTERED
//
// MessageText:
//
//  Moniker is already registered in running object table
//
#define MBX_MK_S_MONIKERALREADYREGISTERED    _HRESULT_TYPEDEF_(0x000401E7L)

// ******************
// FACILITY_WINDOWS
// ******************
//
// Codes 0x0-0x01ff are reserved for the OLE group of
// interfaces.
//
//
// MessageId: MBX_CO_E_CLASS_CREATE_FAILED
//
// MessageText:
//
//  Attempt to create a class object failed
//
#define MBX_CO_E_CLASS_CREATE_FAILED         _HRESULT_TYPEDEF_(0x80080001L)

//
// MessageId: MBX_CO_E_SCM_ERROR
//
// MessageText:
//
//  OLE service could not bind object
//
#define MBX_CO_E_SCM_ERROR                   _HRESULT_TYPEDEF_(0x80080002L)

//
// MessageId: MBX_CO_E_SCM_RPC_FAILURE
//
// MessageText:
//
//  RPC communication failed with OLE service
//
#define MBX_CO_E_SCM_RPC_FAILURE             _HRESULT_TYPEDEF_(0x80080003L)

//
// MessageId: MBX_CO_E_BAD_PATH
//
// MessageText:
//
//  Bad path to object
//
#define MBX_CO_E_BAD_PATH                    _HRESULT_TYPEDEF_(0x80080004L)

//
// MessageId: MBX_CO_E_SERVER_EXEC_FAILURE
//
// MessageText:
//
//  Server execution failed
//
#define MBX_CO_E_SERVER_EXEC_FAILURE         _HRESULT_TYPEDEF_(0x80080005L)

//
// MessageId: MBX_CO_E_OBJSRV_RPC_FAILURE
//
// MessageText:
//
//  OLE service could not communicate with the object server
//
#define MBX_CO_E_OBJSRV_RPC_FAILURE          _HRESULT_TYPEDEF_(0x80080006L)

//
// MessageId: MBX_MK_E_NO_NORMALIZED
//
// MessageText:
//
//  Moniker path could not be normalized
//
#define MBX_MK_E_NO_NORMALIZED               _HRESULT_TYPEDEF_(0x80080007L)

//
// MessageId: MBX_CO_E_SERVER_STOPPING
//
// MessageText:
//
//  Object server is stopping when OLE service contacts it
//
#define MBX_CO_E_SERVER_STOPPING             _HRESULT_TYPEDEF_(0x80080008L)

//
// MessageId: MBX_MEM_E_INVALID_ROOT
//
// MessageText:
//
//  An invalid root block pointer was specified
//
#define MBX_MEM_E_INVALID_ROOT               _HRESULT_TYPEDEF_(0x80080009L)

//
// MessageId: MBX_MEM_E_INVALID_LINK
//
// MessageText:
//
//  An allocation chain contained an invalid link pointer
//
#define MBX_MEM_E_INVALID_LINK               _HRESULT_TYPEDEF_(0x80080010L)

//
// MessageId: MBX_MEM_E_INVALID_SIZE
//
// MessageText:
//
//  The requested allocation size was too large
//
#define MBX_MEM_E_INVALID_SIZE               _HRESULT_TYPEDEF_(0x80080011L)

//
// MessageId: MBX_CO_S_NOTALLINTERFACES
//
// MessageText:
//
//  Not all the requested interfaces were available
//
#define MBX_CO_S_NOTALLINTERFACES            _HRESULT_TYPEDEF_(0x00080012L)

// ******************
// FACILITY_DISPATCH
// ******************
//
// MessageId: MBX_DISP_E_UNKNOWNINTERFACE
//
// MessageText:
//
//  Unknown interface.
//
#define MBX_DISP_E_UNKNOWNINTERFACE          _HRESULT_TYPEDEF_(0x80020001L)

//
// MessageId: MBX_DISP_E_MEMBERNOTFOUND
//
// MessageText:
//
//  Member not found.
//
#define MBX_DISP_E_MEMBERNOTFOUND            _HRESULT_TYPEDEF_(0x80020003L)

//
// MessageId: MBX_DISP_E_PARAMNOTFOUND
//
// MessageText:
//
//  Parameter not found.
//
#define MBX_DISP_E_PARAMNOTFOUND             _HRESULT_TYPEDEF_(0x80020004L)

//
// MessageId: MBX_DISP_E_TYPEMISMATCH
//
// MessageText:
//
//  Type mismatch.
//
#define MBX_DISP_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80020005L)

//
// MessageId: MBX_DISP_E_UNKNOWNNAME
//
// MessageText:
//
//  Unknown name.
//
#define MBX_DISP_E_UNKNOWNNAME               _HRESULT_TYPEDEF_(0x80020006L)

//
// MessageId: MBX_DISP_E_NONAMEDARGS
//
// MessageText:
//
//  No named arguments.
//
#define MBX_DISP_E_NONAMEDARGS               _HRESULT_TYPEDEF_(0x80020007L)

//
// MessageId: MBX_DISP_E_BADVARTYPE
//
// MessageText:
//
//  Bad variable type.
//
#define MBX_DISP_E_BADVARTYPE                _HRESULT_TYPEDEF_(0x80020008L)

//
// MessageId: MBX_DISP_E_EXCEPTION
//
// MessageText:
//
//  Exception occurred.
//
#define MBX_DISP_E_EXCEPTION                 _HRESULT_TYPEDEF_(0x80020009L)

//
// MessageId: MBX_DISP_E_OVERFLOW
//
// MessageText:
//
//  Out of present range.
//
#define MBX_DISP_E_OVERFLOW                  _HRESULT_TYPEDEF_(0x8002000AL)

//
// MessageId: MBX_DISP_E_BADINDEX
//
// MessageText:
//
//  Invalid index.
//
#define MBX_DISP_E_BADINDEX                  _HRESULT_TYPEDEF_(0x8002000BL)

//
// MessageId: MBX_DISP_E_UNKNOWNLCID
//
// MessageText:
//
//  Unknown language.
//
#define MBX_DISP_E_UNKNOWNLCID               _HRESULT_TYPEDEF_(0x8002000CL)

//
// MessageId: MBX_DISP_E_ARRAYISLOCKED
//
// MessageText:
//
//  Memory is locked.
//
#define MBX_DISP_E_ARRAYISLOCKED             _HRESULT_TYPEDEF_(0x8002000DL)

//
// MessageId: MBX_DISP_E_BADPARAMCOUNT
//
// MessageText:
//
//  Invalid number of parameters.
//
#define MBX_DISP_E_BADPARAMCOUNT             _HRESULT_TYPEDEF_(0x8002000EL)

//
// MessageId: MBX_DISP_E_PARAMNOTOPTIONAL
//
// MessageText:
//
//  Parameter not optional.
//
#define MBX_DISP_E_PARAMNOTOPTIONAL          _HRESULT_TYPEDEF_(0x8002000FL)

//
// MessageId: MBX_DISP_E_BADCALLEE
//
// MessageText:
//
//  Invalid callee.
//
#define MBX_DISP_E_BADCALLEE                 _HRESULT_TYPEDEF_(0x80020010L)

//
// MessageId: MBX_DISP_E_NOTACOLLECTION
//
// MessageText:
//
//  Does not support a collection.
//
#define MBX_DISP_E_NOTACOLLECTION            _HRESULT_TYPEDEF_(0x80020011L)

//
// MessageId: MBX_TYPE_E_BUFFERTOOSMALL
//
// MessageText:
//
//  Buffer too small.
//
#define MBX_TYPE_E_BUFFERTOOSMALL            _HRESULT_TYPEDEF_(0x80028016L)

//
// MessageId: MBX_TYPE_E_INVDATAREAD
//
// MessageText:
//
//  Old format or invalid type library.
//
#define MBX_TYPE_E_INVDATAREAD               _HRESULT_TYPEDEF_(0x80028018L)

//
// MessageId: MBX_TYPE_E_UNSUPFORMAT
//
// MessageText:
//
//  Old format or invalid type library.
//
#define MBX_TYPE_E_UNSUPFORMAT               _HRESULT_TYPEDEF_(0x80028019L)

//
// MessageId: MBX_TYPE_E_REGISTRYACCESS
//
// MessageText:
//
//  Error accessing the OLE registry.
//
#define MBX_TYPE_E_REGISTRYACCESS            _HRESULT_TYPEDEF_(0x8002801CL)

//
// MessageId: MBX_TYPE_E_LIBNOTREGISTERED
//
// MessageText:
//
//  Library not registered.
//
#define MBX_TYPE_E_LIBNOTREGISTERED          _HRESULT_TYPEDEF_(0x8002801DL)

//
// MessageId: MBX_TYPE_E_UNDEFINEDTYPE
//
// MessageText:
//
//  Bound to unknown type.
//
#define MBX_TYPE_E_UNDEFINEDTYPE             _HRESULT_TYPEDEF_(0x80028027L)

//
// MessageId: MBX_TYPE_E_QUALIFIEDNAMEDISALLOWED
//
// MessageText:
//
//  Qualified name disallowed.
//
#define MBX_TYPE_E_QUALIFIEDNAMEDISALLOWED   _HRESULT_TYPEDEF_(0x80028028L)

//
// MessageId: MBX_TYPE_E_INVALIDSTATE
//
// MessageText:
//
//  Invalid forward reference, or reference to uncompiled type.
//
#define MBX_TYPE_E_INVALIDSTATE              _HRESULT_TYPEDEF_(0x80028029L)

//
// MessageId: MBX_TYPE_E_WRONGTYPEKIND
//
// MessageText:
//
//  Type mismatch.
//
#define MBX_TYPE_E_WRONGTYPEKIND             _HRESULT_TYPEDEF_(0x8002802AL)

//
// MessageId: MBX_TYPE_E_ELEMENTNOTFOUND
//
// MessageText:
//
//  Element not found.
//
#define MBX_TYPE_E_ELEMENTNOTFOUND           _HRESULT_TYPEDEF_(0x8002802BL)

//
// MessageId: MBX_TYPE_E_AMBIGUOUSNAME
//
// MessageText:
//
//  Ambiguous name.
//
#define MBX_TYPE_E_AMBIGUOUSNAME             _HRESULT_TYPEDEF_(0x8002802CL)

//
// MessageId: MBX_TYPE_E_NAMECONFLICT
//
// MessageText:
//
//  Name already exists in the library.
//
#define MBX_TYPE_E_NAMECONFLICT              _HRESULT_TYPEDEF_(0x8002802DL)

//
// MessageId: MBX_TYPE_E_UNKNOWNLCID
//
// MessageText:
//
//  Unknown LCID.
//
#define MBX_TYPE_E_UNKNOWNLCID               _HRESULT_TYPEDEF_(0x8002802EL)

//
// MessageId: MBX_TYPE_E_DLLFUNCTIONNOTFOUND
//
// MessageText:
//
//  Function not defined in specified DLL.
//
#define MBX_TYPE_E_DLLFUNCTIONNOTFOUND       _HRESULT_TYPEDEF_(0x8002802FL)

//
// MessageId: MBX_TYPE_E_BADMODULEKIND
//
// MessageText:
//
//  Wrong module kind for the operation.
//
#define MBX_TYPE_E_BADMODULEKIND             _HRESULT_TYPEDEF_(0x800288BDL)

//
// MessageId: MBX_TYPE_E_SIZETOOBIG
//
// MessageText:
//
//  Size may not exceed 64K.
//
#define MBX_TYPE_E_SIZETOOBIG                _HRESULT_TYPEDEF_(0x800288C5L)

//
// MessageId: MBX_TYPE_E_DUPLICATEID
//
// MessageText:
//
//  Duplicate ID in inheritance hierarchy.
//
#define MBX_TYPE_E_DUPLICATEID               _HRESULT_TYPEDEF_(0x800288C6L)

//
// MessageId: MBX_TYPE_E_INVALIDID
//
// MessageText:
//
//  Incorrect inheritance depth in standard OLE hmember.
//
#define MBX_TYPE_E_INVALIDID                 _HRESULT_TYPEDEF_(0x800288CFL)

//
// MessageId: MBX_TYPE_E_TYPEMISMATCH
//
// MessageText:
//
//  Type mismatch.
//
#define MBX_TYPE_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80028CA0L)

//
// MessageId: MBX_TYPE_E_OUTOFBOUNDS
//
// MessageText:
//
//  Invalid number of arguments.
//
#define MBX_TYPE_E_OUTOFBOUNDS               _HRESULT_TYPEDEF_(0x80028CA1L)

//
// MessageId: MBX_TYPE_E_IOERROR
//
// MessageText:
//
//  I/O Error.
//
#define MBX_TYPE_E_IOERROR                   _HRESULT_TYPEDEF_(0x80028CA2L)

//
// MessageId: MBX_TYPE_E_CANTCREATETMPFILE
//
// MessageText:
//
//  Error creating unique tmp file.
//
#define MBX_TYPE_E_CANTCREATETMPFILE         _HRESULT_TYPEDEF_(0x80028CA3L)

//
// MessageId: MBX_TYPE_E_CANTLOADLIBRARY
//
// MessageText:
//
//  Error loading type library/DLL.
//
#define MBX_TYPE_E_CANTLOADLIBRARY           _HRESULT_TYPEDEF_(0x80029C4AL)

//
// MessageId: MBX_TYPE_E_INCONSISTENTPROPFUNCS
//
// MessageText:
//
//  Inconsistent property functions.
//
#define MBX_TYPE_E_INCONSISTENTPROPFUNCS     _HRESULT_TYPEDEF_(0x80029C83L)

//
// MessageId: MBX_TYPE_E_CIRCULARTYPE
//
// MessageText:
//
//  Circular dependency between types/modules.
//
#define MBX_TYPE_E_CIRCULARTYPE              _HRESULT_TYPEDEF_(0x80029C84L)

// ******************
// FACILITY_STORAGE
// ******************
//
// MessageId: MBX_STG_E_INVALIDFUNCTION
//
// MessageText:
//
//  Unable to perform requested operation.
//
#define MBX_STG_E_INVALIDFUNCTION            _HRESULT_TYPEDEF_(0x80030001L)

//
// MessageId: MBX_STG_E_FILENOTFOUND
//
// MessageText:
//
//  %1 could not be found.
//
#define MBX_STG_E_FILENOTFOUND               _HRESULT_TYPEDEF_(0x80030002L)

//
// MessageId: MBX_STG_E_PATHNOTFOUND
//
// MessageText:
//
//  The path %1 could not be found.
//
#define MBX_STG_E_PATHNOTFOUND               _HRESULT_TYPEDEF_(0x80030003L)

//
// MessageId: MBX_STG_E_TOOMANYOPENFILES
//
// MessageText:
//
//  There are insufficient resources to open another file.
//
#define MBX_STG_E_TOOMANYOPENFILES           _HRESULT_TYPEDEF_(0x80030004L)

//
// MessageId: MBX_STG_E_ACCESSDENIED
//
// MessageText:
//
//  Access Denied.
//
#define MBX_STG_E_ACCESSDENIED               _HRESULT_TYPEDEF_(0x80030005L)

//
// MessageId: MBX_STG_E_INVALIDHANDLE
//
// MessageText:
//
//  Attempted an operation on an invalid object.
//
#define MBX_STG_E_INVALIDHANDLE              _HRESULT_TYPEDEF_(0x80030006L)

//
// MessageId: MBX_STG_E_INSUFFICIENTMEMORY
//
// MessageText:
//
//  There is insufficient memory available to complete operation.
//
#define MBX_STG_E_INSUFFICIENTMEMORY         _HRESULT_TYPEDEF_(0x80030008L)

//
// MessageId: MBX_STG_E_INVALIDPOINTER
//
// MessageText:
//
//  Invalid pointer error.
//
#define MBX_STG_E_INVALIDPOINTER             _HRESULT_TYPEDEF_(0x80030009L)

//
// MessageId: MBX_STG_E_NOMOREFILES
//
// MessageText:
//
//  There are no more entries to return.
//
#define MBX_STG_E_NOMOREFILES                _HRESULT_TYPEDEF_(0x80030012L)

//
// MessageId: MBX_STG_E_DISKISWRITEPROTECTED
//
// MessageText:
//
//  Disk is write-protected.
//
#define MBX_STG_E_DISKISWRITEPROTECTED       _HRESULT_TYPEDEF_(0x80030013L)

//
// MessageId: MBX_STG_E_SEEKERROR
//
// MessageText:
//
//  An error occurred during a seek operation.
//
#define MBX_STG_E_SEEKERROR                  _HRESULT_TYPEDEF_(0x80030019L)

//
// MessageId: MBX_STG_E_WRITEFAULT
//
// MessageText:
//
//  A disk error occurred during a write operation.
//
#define MBX_STG_E_WRITEFAULT                 _HRESULT_TYPEDEF_(0x8003001DL)

//
// MessageId: MBX_STG_E_READFAULT
//
// MessageText:
//
//  A disk error occurred during a read operation.
//
#define MBX_STG_E_READFAULT                  _HRESULT_TYPEDEF_(0x8003001EL)

//
// MessageId: MBX_STG_E_SHAREVIOLATION
//
// MessageText:
//
//  A share violation has occurred.
//
#define MBX_STG_E_SHAREVIOLATION             _HRESULT_TYPEDEF_(0x80030020L)

//
// MessageId: MBX_STG_E_LOCKVIOLATION
//
// MessageText:
//
//  A lock violation has occurred.
//
#define MBX_STG_E_LOCKVIOLATION              _HRESULT_TYPEDEF_(0x80030021L)

//
// MessageId: MBX_STG_E_FILEALREADYEXISTS
//
// MessageText:
//
//  %1 already exists.
//
#define MBX_STG_E_FILEALREADYEXISTS          _HRESULT_TYPEDEF_(0x80030050L)

//
// MessageId: MBX_STG_E_INVALIDPARAMETER
//
// MessageText:
//
//  Invalid parameter error.
//
#define MBX_STG_E_INVALIDPARAMETER           _HRESULT_TYPEDEF_(0x80030057L)

//
// MessageId: MBX_STG_E_MEDIUMFULL
//
// MessageText:
//
//  There is insufficient disk space to complete operation.
//
#define MBX_STG_E_MEDIUMFULL                 _HRESULT_TYPEDEF_(0x80030070L)

//
// MessageId: MBX_STG_E_PROPSETMISMATCHED
//
// MessageText:
//
//  Illegal write of non-simple property to simple property set.
//
#define MBX_STG_E_PROPSETMISMATCHED          _HRESULT_TYPEDEF_(0x800300F0L)

//
// MessageId: MBX_STG_E_ABNORMALAPIEXIT
//
// MessageText:
//
//  An API call exited abnormally.
//
#define MBX_STG_E_ABNORMALAPIEXIT            _HRESULT_TYPEDEF_(0x800300FAL)

//
// MessageId: MBX_STG_E_INVALIDHEADER
//
// MessageText:
//
//  The file %1 is not a valid compound file.
//
#define MBX_STG_E_INVALIDHEADER              _HRESULT_TYPEDEF_(0x800300FBL)

//
// MessageId: MBX_STG_E_INVALIDNAME
//
// MessageText:
//
//  The name %1 is not valid.
//
#define MBX_STG_E_INVALIDNAME                _HRESULT_TYPEDEF_(0x800300FCL)

//
// MessageId: MBX_STG_E_UNKNOWN
//
// MessageText:
//
//  An unexpected error occurred.
//
#define MBX_STG_E_UNKNOWN                    _HRESULT_TYPEDEF_(0x800300FDL)

//
// MessageId: MBX_STG_E_UNIMPLEMENTEDFUNCTION
//
// MessageText:
//
//  That function is not implemented.
//
#define MBX_STG_E_UNIMPLEMENTEDFUNCTION      _HRESULT_TYPEDEF_(0x800300FEL)

//
// MessageId: MBX_STG_E_INVALIDFLAG
//
// MessageText:
//
//  Invalid flag error.
//
#define MBX_STG_E_INVALIDFLAG                _HRESULT_TYPEDEF_(0x800300FFL)

//
// MessageId: MBX_STG_E_INUSE
//
// MessageText:
//
//  Attempted to use an object that is busy.
//
#define MBX_STG_E_INUSE                      _HRESULT_TYPEDEF_(0x80030100L)

//
// MessageId: MBX_STG_E_NOTCURRENT
//
// MessageText:
//
//  The storage has been changed since the last commit.
//
#define MBX_STG_E_NOTCURRENT                 _HRESULT_TYPEDEF_(0x80030101L)

//
// MessageId: MBX_STG_E_REVERTED
//
// MessageText:
//
//  Attempted to use an object that has ceased to exist.
//
#define MBX_STG_E_REVERTED                   _HRESULT_TYPEDEF_(0x80030102L)

//
// MessageId: MBX_STG_E_CANTSAVE
//
// MessageText:
//
//  Can't save.
//
#define MBX_STG_E_CANTSAVE                   _HRESULT_TYPEDEF_(0x80030103L)

//
// MessageId: MBX_STG_E_OLDFORMAT
//
// MessageText:
//
//  The compound file %1 was produced with an incompatible version of storage.
//
#define MBX_STG_E_OLDFORMAT                  _HRESULT_TYPEDEF_(0x80030104L)

//
// MessageId: MBX_STG_E_OLDDLL
//
// MessageText:
//
//  The compound file %1 was produced with a newer version of storage.
//
#define MBX_STG_E_OLDDLL                     _HRESULT_TYPEDEF_(0x80030105L)

//
// MessageId: MBX_STG_E_SHAREREQUIRED
//
// MessageText:
//
//  Share.exe or equivalent is required for operation.
//
#define MBX_STG_E_SHAREREQUIRED              _HRESULT_TYPEDEF_(0x80030106L)

//
// MessageId: MBX_STG_E_NOTFILEBASEDSTORAGE
//
// MessageText:
//
//  Illegal operation called on non-file based storage.
//
#define MBX_STG_E_NOTFILEBASEDSTORAGE        _HRESULT_TYPEDEF_(0x80030107L)

//
// MessageId: MBX_STG_E_EXTANTMARSHALLINGS
//
// MessageText:
//
//  Illegal operation called on object with extant marshallings.
//
#define MBX_STG_E_EXTANTMARSHALLINGS         _HRESULT_TYPEDEF_(0x80030108L)

//
// MessageId: MBX_STG_E_DOCFILECORRUPT
//
// MessageText:
//
//  The docfile has been corrupted.
//
#define MBX_STG_E_DOCFILECORRUPT             _HRESULT_TYPEDEF_(0x80030109L)

//
// MessageId: MBX_STG_E_BADBASEADDRESS
//
// MessageText:
//
//  OLE32.DLL has been loaded at the wrong address.
//
#define MBX_STG_E_BADBASEADDRESS             _HRESULT_TYPEDEF_(0x80030110L)

//
// MessageId: MBX_STG_E_INCOMPLETE
//
// MessageText:
//
//  The file download was aborted abnormally.  The file is incomplete.
//
#define MBX_STG_E_INCOMPLETE                 _HRESULT_TYPEDEF_(0x80030201L)

//
// MessageId: MBX_STG_E_TERMINATED
//
// MessageText:
//
//  The file download has been terminated.
//
#define MBX_STG_E_TERMINATED                 _HRESULT_TYPEDEF_(0x80030202L)

//
// MessageId: MBX_STG_S_CONVERTED
//
// MessageText:
//
//  The underlying file was converted to compound file format.
//
#define MBX_STG_S_CONVERTED                  _HRESULT_TYPEDEF_(0x00030200L)

//
// MessageId: MBX_STG_S_BLOCK
//
// MessageText:
//
//  The storage operation should block until more data is available.
//
#define MBX_STG_S_BLOCK                      _HRESULT_TYPEDEF_(0x00030201L)

//
// MessageId: MBX_STG_S_RETRYNOW
//
// MessageText:
//
//  The storage operation should retry immediately.
//
#define MBX_STG_S_RETRYNOW                   _HRESULT_TYPEDEF_(0x00030202L)

//
// MessageId: MBX_STG_S_MONITORING
//
// MessageText:
//
//  The notified event sink will not influence the storage operation.
//
#define MBX_STG_S_MONITORING                 _HRESULT_TYPEDEF_(0x00030203L)

// ******************
// FACILITY_RPC
// ******************
//
// Codes 0x0-0x11 are propogated from 16 bit OLE.
//
//
// MessageId: MBX_RPC_E_CALL_REJECTED
//
// MessageText:
//
//  Call was rejected by callee.
//
#define MBX_RPC_E_CALL_REJECTED              _HRESULT_TYPEDEF_(0x80010001L)

//
// MessageId: MBX_RPC_E_CALL_CANCELED
//
// MessageText:
//
//  Call was canceled by the message filter.
//
#define MBX_RPC_E_CALL_CANCELED              _HRESULT_TYPEDEF_(0x80010002L)

//
// MessageId: MBX_RPC_E_CANTPOST_INSENDCALL
//
// MessageText:
//
//  The caller is dispatching an intertask SendMessage call and
//  cannot call out via PostMessage.
//
#define MBX_RPC_E_CANTPOST_INSENDCALL        _HRESULT_TYPEDEF_(0x80010003L)

//
// MessageId: MBX_RPC_E_CANTCALLOUT_INASYNCCALL
//
// MessageText:
//
//  The caller is dispatching an asynchronous call and cannot
//  make an outgoing call on behalf of this call.
//
#define MBX_RPC_E_CANTCALLOUT_INASYNCCALL    _HRESULT_TYPEDEF_(0x80010004L)

//
// MessageId: MBX_RPC_E_CANTCALLOUT_INEXTERNALCALL
//
// MessageText:
//
//  It is illegal to call out while inside message filter.
//
#define MBX_RPC_E_CANTCALLOUT_INEXTERNALCALL _HRESULT_TYPEDEF_(0x80010005L)

//
// MessageId: MBX_RPC_E_CONNECTION_TERMINATED
//
// MessageText:
//
//  The connection terminated or is in a bogus state
//  and cannot be used any more. Other connections
//  are still valid.
//
#define MBX_RPC_E_CONNECTION_TERMINATED      _HRESULT_TYPEDEF_(0x80010006L)

//
// MessageId: MBX_RPC_E_SERVER_DIED
//
// MessageText:
//
//  The callee (server [not server application]) is not available
//  and disappeared; all connections are invalid.  The call may
//  have executed.
//
#define MBX_RPC_E_SERVER_DIED                _HRESULT_TYPEDEF_(0x80010007L)

//
// MessageId: MBX_RPC_E_CLIENT_DIED
//
// MessageText:
//
//  The caller (client) disappeared while the callee (server) was
//  processing a call.
//
#define MBX_RPC_E_CLIENT_DIED                _HRESULT_TYPEDEF_(0x80010008L)

//
// MessageId: MBX_RPC_E_INVALID_DATAPACKET
//
// MessageText:
//
//  The data packet with the marshalled parameter data is incorrect.
//
#define MBX_RPC_E_INVALID_DATAPACKET         _HRESULT_TYPEDEF_(0x80010009L)

//
// MessageId: MBX_RPC_E_CANTTRANSMIT_CALL
//
// MessageText:
//
//  The call was not transmitted properly; the message queue
//  was full and was not emptied after yielding.
//
#define MBX_RPC_E_CANTTRANSMIT_CALL          _HRESULT_TYPEDEF_(0x8001000AL)

//
// MessageId: MBX_RPC_E_CLIENT_CANTMARSHAL_DATA
//
// MessageText:
//
//  The client (caller) cannot marshall the parameter data - low memory, etc.
//
#define MBX_RPC_E_CLIENT_CANTMARSHAL_DATA    _HRESULT_TYPEDEF_(0x8001000BL)

//
// MessageId: MBX_RPC_E_CLIENT_CANTUNMARSHAL_DATA
//
// MessageText:
//
//  The client (caller) cannot unmarshall the return data - low memory, etc.
//
#define MBX_RPC_E_CLIENT_CANTUNMARSHAL_DATA  _HRESULT_TYPEDEF_(0x8001000CL)

//
// MessageId: MBX_RPC_E_SERVER_CANTMARSHAL_DATA
//
// MessageText:
//
//  The server (callee) cannot marshall the return data - low memory, etc.
//
#define MBX_RPC_E_SERVER_CANTMARSHAL_DATA    _HRESULT_TYPEDEF_(0x8001000DL)

//
// MessageId: MBX_RPC_E_SERVER_CANTUNMARSHAL_DATA
//
// MessageText:
//
//  The server (callee) cannot unmarshall the parameter data - low memory, etc.
//
#define MBX_RPC_E_SERVER_CANTUNMARSHAL_DATA  _HRESULT_TYPEDEF_(0x8001000EL)

//
// MessageId: MBX_RPC_E_INVALID_DATA
//
// MessageText:
//
//  Received data is invalid; could be server or client data.
//
#define MBX_RPC_E_INVALID_DATA               _HRESULT_TYPEDEF_(0x8001000FL)

//
// MessageId: MBX_RPC_E_INVALID_PARAMETER
//
// MessageText:
//
//  A particular parameter is invalid and cannot be (un)marshalled.
//
#define MBX_RPC_E_INVALID_PARAMETER          _HRESULT_TYPEDEF_(0x80010010L)

//
// MessageId: MBX_RPC_E_CANTCALLOUT_AGAIN
//
// MessageText:
//
//  There is no second outgoing call on same channel in DDE conversation.
//
#define MBX_RPC_E_CANTCALLOUT_AGAIN          _HRESULT_TYPEDEF_(0x80010011L)

//
// MessageId: MBX_RPC_E_SERVER_DIED_DNE
//
// MessageText:
//
//  The callee (server [not server application]) is not available
//  and disappeared; all connections are invalid.  The call did not execute.
//
#define MBX_RPC_E_SERVER_DIED_DNE            _HRESULT_TYPEDEF_(0x80010012L)

//
// MessageId: MBX_RPC_E_SYS_CALL_FAILED
//
// MessageText:
//
//  System call failed.
//
#define MBX_RPC_E_SYS_CALL_FAILED            _HRESULT_TYPEDEF_(0x80010100L)

//
// MessageId: MBX_RPC_E_OUT_OF_RESOURCES
//
// MessageText:
//
//  Could not allocate some required resource (memory, events, ...)
//
#define MBX_RPC_E_OUT_OF_RESOURCES           _HRESULT_TYPEDEF_(0x80010101L)

//
// MessageId: MBX_RPC_E_ATTEMPTED_MULTITHREAD
//
// MessageText:
//
//  Attempted to make calls on more than one thread in single threaded mode.
//
#define MBX_RPC_E_ATTEMPTED_MULTITHREAD      _HRESULT_TYPEDEF_(0x80010102L)

//
// MessageId: MBX_RPC_E_NOT_REGISTERED
//
// MessageText:
//
//  The requested interface is not registered on the server object.
//
#define MBX_RPC_E_NOT_REGISTERED             _HRESULT_TYPEDEF_(0x80010103L)

//
// MessageId: MBX_RPC_E_FAULT
//
// MessageText:
//
//  RPC could not call the server or could not return the results of calling the server.
//
#define MBX_RPC_E_FAULT                      _HRESULT_TYPEDEF_(0x80010104L)

//
// MessageId: MBX_RPC_E_SERVERFAULT
//
// MessageText:
//
//  The server threw an exception.
//
#define MBX_RPC_E_SERVERFAULT                _HRESULT_TYPEDEF_(0x80010105L)

//
// MessageId: MBX_RPC_E_CHANGED_MODE
//
// MessageText:
//
//  Cannot change thread mode after it is set.
//
#define MBX_RPC_E_CHANGED_MODE               _HRESULT_TYPEDEF_(0x80010106L)

//
// MessageId: MBX_RPC_E_INVALIDMETHOD
//
// MessageText:
//
//  The method called does not exist on the server.
//
#define MBX_RPC_E_INVALIDMETHOD              _HRESULT_TYPEDEF_(0x80010107L)

//
// MessageId: MBX_RPC_E_DISCONNECTED
//
// MessageText:
//
//  The object invoked has disconnected from its clients.
//
#define MBX_RPC_E_DISCONNECTED               _HRESULT_TYPEDEF_(0x80010108L)

//
// MessageId: MBX_RPC_E_RETRY
//
// MessageText:
//
//  The object invoked chose not to process the call now.  Try again later.
//
#define MBX_RPC_E_RETRY                      _HRESULT_TYPEDEF_(0x80010109L)

//
// MessageId: MBX_RPC_E_SERVERCALL_RETRYLATER
//
// MessageText:
//
//  The message filter indicated that the application is busy.
//
#define MBX_RPC_E_SERVERCALL_RETRYLATER      _HRESULT_TYPEDEF_(0x8001010AL)

//
// MessageId: MBX_RPC_E_SERVERCALL_REJECTED
//
// MessageText:
//
//  The message filter rejected the call.
//
#define MBX_RPC_E_SERVERCALL_REJECTED        _HRESULT_TYPEDEF_(0x8001010BL)

//
// MessageId: MBX_RPC_E_INVALID_CALLDATA
//
// MessageText:
//
//  A call control interfaces was called with invalid data.
//
#define MBX_RPC_E_INVALID_CALLDATA           _HRESULT_TYPEDEF_(0x8001010CL)

//
// MessageId: MBX_RPC_E_CANTCALLOUT_ININPUTSYNCCALL
//
// MessageText:
//
//  An outgoing call cannot be made since the application is dispatching an input-synchronous call.
//
#define MBX_RPC_E_CANTCALLOUT_ININPUTSYNCCALL _HRESULT_TYPEDEF_(0x8001010DL)

//
// MessageId: MBX_RPC_E_WRONG_THREAD
//
// MessageText:
//
//  The application called an interface that was marshalled for a different thread.
//
#define MBX_RPC_E_WRONG_THREAD               _HRESULT_TYPEDEF_(0x8001010EL)

//
// MessageId: MBX_RPC_E_THREAD_NOT_INIT
//
// MessageText:
//
//  CoInitialize has not been called on the current thread.
//
#define MBX_RPC_E_THREAD_NOT_INIT            _HRESULT_TYPEDEF_(0x8001010FL)

//
// MessageId: MBX_RPC_E_VERSION_MISMATCH
//
// MessageText:
//
//  The version of OLE on the client and server machines does not match.
//
#define MBX_RPC_E_VERSION_MISMATCH           _HRESULT_TYPEDEF_(0x80010110L)

//
// MessageId: MBX_RPC_E_INVALID_HEADER
//
// MessageText:
//
//  OLE received a packet with an invalid header.
//
#define MBX_RPC_E_INVALID_HEADER             _HRESULT_TYPEDEF_(0x80010111L)

//
// MessageId: MBX_RPC_E_INVALID_EXTENSION
//
// MessageText:
//
//  OLE received a packet with an invalid extension.
//
#define MBX_RPC_E_INVALID_EXTENSION          _HRESULT_TYPEDEF_(0x80010112L)

//
// MessageId: MBX_RPC_E_INVALID_IPID
//
// MessageText:
//
//  The requested object or interface does not exist.
//
#define MBX_RPC_E_INVALID_IPID               _HRESULT_TYPEDEF_(0x80010113L)

//
// MessageId: MBX_RPC_E_INVALID_OBJECT
//
// MessageText:
//
//  The requested object does not exist.
//
#define MBX_RPC_E_INVALID_OBJECT             _HRESULT_TYPEDEF_(0x80010114L)

//
// MessageId: MBX_RPC_S_CALLPENDING
//
// MessageText:
//
//  OLE has sent a request and is waiting for a reply.
//
#define MBX_RPC_S_CALLPENDING                _HRESULT_TYPEDEF_(0x80010115L)

//
// MessageId: MBX_RPC_S_WAITONTIMER
//
// MessageText:
//
//  OLE is waiting before retrying a request.
//
#define MBX_RPC_S_WAITONTIMER                _HRESULT_TYPEDEF_(0x80010116L)

//
// MessageId: MBX_RPC_E_CALL_COMPLETE
//
// MessageText:
//
//  Call context cannot be accessed after call completed.
//
#define MBX_RPC_E_CALL_COMPLETE              _HRESULT_TYPEDEF_(0x80010117L)

//
// MessageId: MBX_RPC_E_UNSECURE_CALL
//
// MessageText:
//
//  Impersonate on unsecure calls is not supported.
//
#define MBX_RPC_E_UNSECURE_CALL              _HRESULT_TYPEDEF_(0x80010118L)

//
// MessageId: MBX_RPC_E_TOO_LATE
//
// MessageText:
//
//  Security must be initialized before any interfaces are marshalled or
//  unmarshalled.  It cannot be changed once initialized.
//
#define MBX_RPC_E_TOO_LATE                   _HRESULT_TYPEDEF_(0x80010119L)

//
// MessageId: MBX_RPC_E_NO_GOOD_SECURITY_PACKAGES
//
// MessageText:
//
//  No security packages are installed on this machine or the user is not logged
//  on or there are no compatible security packages between the client and server.
//
#define MBX_RPC_E_NO_GOOD_SECURITY_PACKAGES  _HRESULT_TYPEDEF_(0x8001011AL)

//
// MessageId: MBX_RPC_E_ACCESS_DENIED
//
// MessageText:
//
//  Access is denied.
//
#define MBX_RPC_E_ACCESS_DENIED              _HRESULT_TYPEDEF_(0x8001011BL)

//
// MessageId: MBX_RPC_E_REMOTE_DISABLED
//
// MessageText:
//
//  Remote calls are not allowed for this process.
//
#define MBX_RPC_E_REMOTE_DISABLED            _HRESULT_TYPEDEF_(0x8001011CL)

//
// MessageId: MBX_RPC_E_UNEXPECTED
//
// MessageText:
//
//  An internal error occurred.
//
#define MBX_RPC_E_UNEXPECTED                 _HRESULT_TYPEDEF_(0x8001FFFFL)


 /////////////////
 //
 //  FACILITY_SSPI
 //
 /////////////////

//
// MessageId: MBX_NTE_BAD_UID
//
// MessageText:
//
//  Bad UID.
//
#define MBX_NTE_BAD_UID                      _HRESULT_TYPEDEF_(0x80090001L)

//
// MessageId: MBX_NTE_BAD_HASH
//
// MessageText:
//
//  Bad Hash.
//
#define MBX_NTE_BAD_HASH                     _HRESULT_TYPEDEF_(0x80090002L)

//
// MessageId: MBX_NTE_BAD_KEY
//
// MessageText:
//
//  Bad Key.
//
#define MBX_NTE_BAD_KEY                      _HRESULT_TYPEDEF_(0x80090003L)

//
// MessageId: MBX_NTE_BAD_LEN
//
// MessageText:
//
//  Bad Length.
//
#define MBX_NTE_BAD_LEN                      _HRESULT_TYPEDEF_(0x80090004L)

//
// MessageId: MBX_NTE_BAD_DATA
//
// MessageText:
//
//  Bad Data.
//
#define MBX_NTE_BAD_DATA                     _HRESULT_TYPEDEF_(0x80090005L)

//
// MessageId: MBX_NTE_BAD_SIGNATURE
//
// MessageText:
//
//  Invalid Signature.
//
#define MBX_NTE_BAD_SIGNATURE                _HRESULT_TYPEDEF_(0x80090006L)

//
// MessageId: MBX_NTE_BAD_VER
//
// MessageText:
//
//  Bad Version of provider.
//
#define MBX_NTE_BAD_VER                      _HRESULT_TYPEDEF_(0x80090007L)

//
// MessageId: MBX_NTE_BAD_ALGID
//
// MessageText:
//
//  Invalid algorithm specified.
//
#define MBX_NTE_BAD_ALGID                    _HRESULT_TYPEDEF_(0x80090008L)

//
// MessageId: MBX_NTE_BAD_FLAGS
//
// MessageText:
//
//  Invalid flags specified.
//
#define MBX_NTE_BAD_FLAGS                    _HRESULT_TYPEDEF_(0x80090009L)

//
// MessageId: MBX_NTE_BAD_TYPE
//
// MessageText:
//
//  Invalid type specified.
//
#define MBX_NTE_BAD_TYPE                     _HRESULT_TYPEDEF_(0x8009000AL)

//
// MessageId: MBX_NTE_BAD_KEY_STATE
//
// MessageText:
//
//  Key not valid for use in specified state.
//
#define MBX_NTE_BAD_KEY_STATE                _HRESULT_TYPEDEF_(0x8009000BL)

//
// MessageId: MBX_NTE_BAD_HASH_STATE
//
// MessageText:
//
//  Hash not valid for use in specified state.
//
#define MBX_NTE_BAD_HASH_STATE               _HRESULT_TYPEDEF_(0x8009000CL)

//
// MessageId: MBX_NTE_NO_KEY
//
// MessageText:
//
//  Key does not exist.
//
#define MBX_NTE_NO_KEY                       _HRESULT_TYPEDEF_(0x8009000DL)

//
// MessageId: MBX_NTE_NO_MEMORY
//
// MessageText:
//
//  Insufficient memory available for the operation.
//
#define MBX_NTE_NO_MEMORY                    _HRESULT_TYPEDEF_(0x8009000EL)

//
// MessageId: MBX_NTE_EXISTS
//
// MessageText:
//
//  Object already exists.
//
#define MBX_NTE_EXISTS                       _HRESULT_TYPEDEF_(0x8009000FL)

//
// MessageId: MBX_NTE_PERM
//
// MessageText:
//
//  Access denied.
//
#define MBX_NTE_PERM                         _HRESULT_TYPEDEF_(0x80090010L)

//
// MessageId: MBX_NTE_NOT_FOUND
//
// MessageText:
//
//  Object was not found.
//
#define MBX_NTE_NOT_FOUND                    _HRESULT_TYPEDEF_(0x80090011L)

//
// MessageId: MBX_NTE_DOUBLE_ENCRYPT
//
// MessageText:
//
//  Data already encrypted.
//
#define MBX_NTE_DOUBLE_ENCRYPT               _HRESULT_TYPEDEF_(0x80090012L)

//
// MessageId: MBX_NTE_BAD_PROVIDER
//
// MessageText:
//
//  Invalid provider specified.
//
#define MBX_NTE_BAD_PROVIDER                 _HRESULT_TYPEDEF_(0x80090013L)

//
// MessageId: MBX_NTE_BAD_PROV_TYPE
//
// MessageText:
//
//  Invalid provider type specified.
//
#define MBX_NTE_BAD_PROV_TYPE                _HRESULT_TYPEDEF_(0x80090014L)

//
// MessageId: MBX_NTE_BAD_PUBLIC_KEY
//
// MessageText:
//
//  Provider's public key is invalid.
//
#define MBX_NTE_BAD_PUBLIC_KEY               _HRESULT_TYPEDEF_(0x80090015L)

//
// MessageId: MBX_NTE_BAD_KEYSET
//
// MessageText:
//
//  Keyset does not exist
//
#define MBX_NTE_BAD_KEYSET                   _HRESULT_TYPEDEF_(0x80090016L)

//
// MessageId: MBX_NTE_PROV_TYPE_NOT_DEF
//
// MessageText:
//
//  Provider type not defined.
//
#define MBX_NTE_PROV_TYPE_NOT_DEF            _HRESULT_TYPEDEF_(0x80090017L)

//
// MessageId: MBX_NTE_PROV_TYPE_ENTRY_BAD
//
// MessageText:
//
//  Provider type as registered is invalid.
//
#define MBX_NTE_PROV_TYPE_ENTRY_BAD          _HRESULT_TYPEDEF_(0x80090018L)

//
// MessageId: MBX_NTE_KEYSET_NOT_DEF
//
// MessageText:
//
//  The keyset is not defined.
//
#define MBX_NTE_KEYSET_NOT_DEF               _HRESULT_TYPEDEF_(0x80090019L)

//
// MessageId: MBX_NTE_KEYSET_ENTRY_BAD
//
// MessageText:
//
//  Keyset as registered is invalid.
//
#define MBX_NTE_KEYSET_ENTRY_BAD             _HRESULT_TYPEDEF_(0x8009001AL)

//
// MessageId: MBX_NTE_PROV_TYPE_NO_MATCH
//
// MessageText:
//
//  Provider type does not match registered value.
//
#define MBX_NTE_PROV_TYPE_NO_MATCH           _HRESULT_TYPEDEF_(0x8009001BL)

//
// MessageId: MBX_NTE_SIGNATURE_FILE_BAD
//
// MessageText:
//
//  The digital signature file is corrupt.
//
#define MBX_NTE_SIGNATURE_FILE_BAD           _HRESULT_TYPEDEF_(0x8009001CL)

//
// MessageId: MBX_NTE_PROVIDER_DLL_FAIL
//
// MessageText:
//
//  Provider DLL failed to initialize correctly.
//
#define MBX_NTE_PROVIDER_DLL_FAIL            _HRESULT_TYPEDEF_(0x8009001DL)

//
// MessageId: MBX_NTE_PROV_DLL_NOT_FOUND
//
// MessageText:
//
//  Provider DLL could not be found.
//
#define MBX_NTE_PROV_DLL_NOT_FOUND           _HRESULT_TYPEDEF_(0x8009001EL)

//
// MessageId: MBX_NTE_BAD_KEYSET_PARAM
//
// MessageText:
//
//  The Keyset parameter is invalid.
//
#define MBX_NTE_BAD_KEYSET_PARAM             _HRESULT_TYPEDEF_(0x8009001FL)

//
// MessageId: MBX_NTE_FAIL
//
// MessageText:
//
//  An internal error occurred.
//
#define MBX_NTE_FAIL                         _HRESULT_TYPEDEF_(0x80090020L)

//
// MessageId: MBX_NTE_SYS_ERR
//
// MessageText:
//
//  A base error occurred.
//
#define MBX_NTE_SYS_ERR                      _HRESULT_TYPEDEF_(0x80090021L)

#define MBX_NTE_OP_OK 0

//
// Note that additional FACILITY_SSPI errors are in issperr.h
//
// ******************
// FACILITY_CERT
// ******************
//
// MessageId: MBX_TRUST_E_PROVIDER_UNKNOWN
//
// MessageText:
//
//  The specified trust provider is not known on this system.
//
#define MBX_TRUST_E_PROVIDER_UNKNOWN         _HRESULT_TYPEDEF_(0x800B0001L)

//
// MessageId: MBX_TRUST_E_ACTION_UNKNOWN
//
// MessageText:
//
//  The trust verification action specified is not supported by the specified trust provider.
//
#define MBX_TRUST_E_ACTION_UNKNOWN           _HRESULT_TYPEDEF_(0x800B0002L)

//
// MessageId: MBX_TRUST_E_SUBJECT_FORM_UNKNOWN
//
// MessageText:
//
//  The form specified for the subject is not one supported or known by the specified trust provider.
//
#define MBX_TRUST_E_SUBJECT_FORM_UNKNOWN     _HRESULT_TYPEDEF_(0x800B0003L)

//
// MessageId: MBX_TRUST_E_SUBJECT_NOT_TRUSTED
//
// MessageText:
//
//  The subject is not trusted for the specified action.
//
#define MBX_TRUST_E_SUBJECT_NOT_TRUSTED      _HRESULT_TYPEDEF_(0x800B0004L)

#endif // _MBXERROR_
