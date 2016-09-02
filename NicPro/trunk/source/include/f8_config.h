/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	e:\vss\pmc2\src\include\f8\f8_config.h
	file path:	e:\vss\pmc2\src\include\f8
	author:	Jackie Pan, jhunter@mail.tsinghua.org.cn
	
	purpose:	F8 module configuration	
	history:
	created:	12/4/2002   14:11
*********************************************************************/
#ifndef __f8_config_h__
#define __f8_config_h__

#ifndef F8_TIME_QUANTUM
#define F8_TIME_QUANTUM		5		/* 5 milliseconds per loop, win32 only */
#endif

#define F8_COMPILER_VER		0x1000

#define F8_REGISTRY_ROOT	"SOFTWARE\\ACTECH\\CONDUCTOR"

/*
	sizeof shared memory used for io buffers
*/
#ifndef IO_SIZE_PER_SLOT
#define IO_SIZE_PER_SLOT	64
#endif

#ifndef IO_MAX_DEVICES
#define IO_MAX_DEVICES		128
#endif

#ifndef IO_MAX_STATIONS
#define IO_MAX_STATIONS 16
#endif

#ifndef IO_DEVICE_PER_STATION
#define IO_DEVICE_PER_STATION 8
#endif

#define ENABLE_61499
// #define ENABLE_USER_BLOCKS

/*
ENABLE_HOT_STANDBY
if enabled, the executive runs the standby phase each
cycle, exchaning information with the peer site.
*/
#define ENABLE_HOT_STANDBY

/*
ENABLE_TREE_BLOCKS, if enabled, user can insert composite
block recursively, thus forming a tree-like block structure; if 
disabled, user can only insert blank composite block at top level,
allowing two-level block structure only.
*/
// #define ENABLE_TREE_BLOCKS

#define F8_CPU_586	1
#define F8_CPU_486	2
#define F8_CPU_ARM9 3

#ifndef KERN_CPU_TYPE
#define KERN_CPU_TYPE F8_CPU_586
#endif

#define F8_MAX_VAR_NAME_LEN 32

#endif

