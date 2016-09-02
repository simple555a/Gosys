/*
	F8 kernel service interface

	Jackie Pan <jhunter@tsinghua.org.cn>
	2003/11/2
*/
#ifndef __f8_unistd_h__
#define __f8_unistd_h__

#include <f8_binary.h>
#include <f8_status.h>

// configuration command
#define define_cmd(name,value,exclusive,routing,foo1,foo2,foo3)\
	name=value,
enum _f8_cmd_t{
#include "f8_cmd.h"
};
#undef define_cmd

#define MAX_KERN_COMMAND		256

#include <pshpack1.h>

/*
	Definition of the configuration command packet.

	During configuration, the config parameters are 
	marshelled in such a packet and the packet is then
	transferred to the target machine, executed there,
	results are then returned in another kern_conf_cmd
	packet.
*/
struct kern_conf_cmd{
	__u16 version;
	__u16 flags;
	__u16 cmd;		// INPUT
	f8_status	retcode;	// OUTPUT

	/*	
		uuid of old, new;
		1) old uuid is which the command is only valid for	
		2) new uuid is the one to set if command succeedes
	*/
	f8_uuid	uuids[2];	// INPUT/OUTPUT

	__u32	isize;		// input buffer length
	__u32	osize;		// output buffer length	
	void		*ibuffer;
	void		*obuffer;
	/* version and reserved fields ... */
	f8_u32 reserved[4];
}PACKED;

struct conf_mod_pin{
	f8_u8	binding;
	union{
		__i32	ival;
		__r4	fval;
		__r8	dval;
		struct  mem_addr_t addr;
	}u;
}PACKED;

struct conf_mod_mem{
	struct mem_addr_t	addr;
	__uint	len;
	struct dyn_pin_t	value;	
}PACKED;

#include <poppack.h>

#define OLD_ID(cmd)  ((cmd)->uuids[0])
#define NEW_ID(cmd)  ((cmd)->uuids[1])

#endif

