/*
	created:	2002:10:12   15:23
	filename: 	E:\vss\pmc2\src\include\f8\f8vm.h
	author:	Jackie, jhunter@tsinghua.org.cn
	
	purpose:	F8 virtual machine types and procedures

	F8 virtual machine is an abstract layer on which the f8 executive
	runs, it is characterized by the following features:

	1) 32-bit integer
	2) support 4-byte and 8-byte float arithmetic operation
	3) multiple-byte information stored in little endian fashion
*/
#ifndef __f8vm_h__
#define __f8vm_h__

#include <f8_defs.h>
/* import host machine basic types */
#include <cpu_features.h>

#ifndef F8_BASE_TYPE_DEFINED
#define F8_BASE_TYPE_DEFINED

/* define f8 virtual machine basic data-types */
typedef __i8	f8_char;
typedef __i32   f8_int;
typedef __i32	f8_long;
typedef __u8	f8_byte;

typedef __i64	f8_i64;

typedef __i8	f8_i8;
typedef __i16	f8_i16;
typedef __i32	f8_i32;

typedef __u8	f8_u8;
typedef __u16 f8_u16;
typedef __u32 f8_u32;	
typedef __u64 f8_u64;

typedef __u8	f8_uchar;
typedef __u16 f8_ushort;
typedef __u32 f8_uint;
typedef __u32 f8_ulong;

typedef float f8_r4;

typedef double f8_r8;

typedef f8_u16 f8_code;

typedef __u32 f8_ptr;

typedef __u64 f8_uuid;

typedef union{
	 f8_int	i;
#ifdef SUPPORT_UNSIGNED_INT
	 f8_uint	ui;
#endif
	 f8_r4	f;
}f8_reg;

#ifndef F8_LOWORD
#define F8_LOWORD(l)           ((f8_u16)(l))
#define F8_HIWORD(l)           ((f8_u16)(((f8_u32)(l) >> 16) & 0xFFFF))
#endif

typedef f8_u8		f8_bool;
#define f8_true	1
#define f8_false	0

typedef unsigned int F8_SOCKET;

#endif

#endif	/* __f8vm_h__ */

