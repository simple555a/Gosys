/*

f8 configuration command attributes table, use macro expansion to access
data encoded in this table.

field specification

define_cmd(cmd_name, enum_value, exclusive_access, cmd_routing, foo1, foo2, foo3)

KAdapter will use `cmd_routing' field to determine whether to route the command
to a remote target system or just executed it on local control database cache.
The value is interpreted as :

0 -> cmd must be executed on both target(if online) and local control database cache
1 -> cmd is valid only for target
2 -> cmd is valid only for local control database cache

Note : if IKadapter_exec_remote is called with a local-only cmd, the cmd will be
executed on target system forcefully, i.e., cmd_routing is only checked in 
IKAdapter_exec.
*/

#ifndef F8CMD_attr_exclusive

#define F8CMD_attr_nonexclusive 0
#define F8CMD_attr_exclusive 1
#define F8CMD_attr_nolock 2

#define F8CMD_route_both 0
#define F8CMD_route_remote 1
#define F8CMD_route_local 2

#define MAX_KERN_COMMAND 256

#define F8CMD_Compressed_Buffer 0x8000

#endif

/* template 
define_cmd(name,value,exclusive,routing,foo1,foo2,foo3)
*/
define_cmd(F8_NULL_COMMAND,0,nonexclusive,both,0,0,0)
define_cmd(F8_MOUNT_BLOCK,1,exclusive,both,0,0,0)
define_cmd(F8_UMOUNT_BLOCK,2,exclusive,both,0,0,0)

define_cmd(F8_START,3,nonexclusive,remote,0,0,0)
define_cmd(F8_STOP,4,exclusive,remote,0,0,0)

define_cmd(F8_GET_PROG,7,nonexclusive,local,0,0,0)
define_cmd(F8_PUT_PROG,8,exclusive,both,0,0,0)

define_cmd(F8_LINK,11,exclusive,both,0,0,0)
define_cmd(F8_UNLINK,12,exclusive,both,0,0,0)
define_cmd(F8_RESET,13,exclusive,both,0,0,0)		// delete all program

define_cmd(F8_EXPORT,14,exclusive,both,0,0,0)
define_cmd(F8_UNEXPORT,15,exclusive,both,0,0,0)

define_cmd(F8_MODIFY_PIN,16,exclusive,both,0,0,0)
define_cmd(F8_SET_PIN_VALUE,17,exclusive,both,0,0,0)

define_cmd(F8_GET_VERSION,18,nonexclusive,local,0,0,0)
define_cmd(F8_SET_VERSION,19,exclusive,both,0,0,0)

/* debug interface */
define_cmd(F8_SINGLE_STEP,21,nonexclusive,remote,0,0,0)
define_cmd(F8_CONTINUE,22,nonexclusive,remote,0,0,0)
define_cmd(F8_BREAK,23,nonexclusive,remote,0,0,0)
define_cmd(F8_BREAKPOINT,24,nonexclusive,remote,0,0,0)
define_cmd(F8_ENABLE_BP,25,nonexclusive,remote,0,0,0)
define_cmd(F8_DISABLE_BP,26,nonexclusive,remote,0,0,0)
define_cmd(F8_DEL_BREAK,27,nonexclusive,remote,0,0,0)

define_cmd(F8_GET_SYS_CONFIG,28,nonexclusive,local,0,0,0)
define_cmd(F8_SET_SYS_CONFIG,29,exclusive,both,0,0,0)

define_cmd(F8_GET_LOG_BUF,30,nonexclusive,remote,0,0,0)
define_cmd(F8_CLEAR_LOG_BUF,31,exclusive,remote,0,0,0)

define_cmd(F8_SET_INVERT,32,exclusive,both,0,0,0)
define_cmd(F8_SET_MEM_VALUE,33,nonexclusive,both,0,0,0)
define_cmd(F8_GET_MEM_VALUE,34,nonexclusive,both,0,0,0)

define_cmd(F8_GET_X_MEMORY,35,nonexclusive,local,0,0,0)
define_cmd(F8_SET_X_MEMORY,36,exclusive,both,0,0,0)

define_cmd(F8_GET_STATUS,37,nonexclusive,local,0,0,0)
define_cmd(F8_SET_STATUS,38,exclusive,both,0,0,0)

define_cmd(F8_GET_DEVICE,39,nonexclusive,local,0,0,0)
define_cmd(F8_SET_DEVICE,40,exclusive,both,0,0,0)

define_cmd(F8_LOCK_CONTROLLER,41,exclusive,remote,0,0,0)
define_cmd(F8_GET_LOCK_STATUS,42,nonexclusive,remote,0,0,0)

define_cmd(F8_UPLOAD,43,nonexclusive,remote,0,0,0)
define_cmd(F8_DOWNLOAD,44,exclusive,remote,0,0,0)

define_cmd(F8_DOWNLOAD_RESOURCE,45,exclusive,remote,0,0,0)
define_cmd(F8_UPLOAD_RESOURCE,46,nonexclusive,remote,0,0,0)

define_cmd(F8_DOWNLOAD_VARTABLE,47,exclusive,remote,0,0,0)
define_cmd(F8_UPLOAD_VARTABLE,48,nonexclusive,remote,0,0,0)
define_cmd(F8_ADD_VARIABLE,49,exclusive,remote,0,0,0)
define_cmd(F8_DEL_VARIABLE,50,exclusive,remote,0,0,0)
define_cmd(F8_EDIT_VARIABLE,51,exclusive,remote,0,0,0)

define_cmd(F8_SUSPEND_STATE,52,exclusive,remote,0,0,0)
define_cmd(F8_RESUME_STATE,53,exclusive,both,0,0,0)

define_cmd(F8_GET_DEV_STATUS,54,nonexclusive,remote,0,0,0)
define_cmd(F8_TARGET_EXEC,55,exclusive,remote,0,0,0)

