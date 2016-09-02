/********************************************************************
	created:	2003/03/20
	created:	20:3:2003   21:28
	filename: D:\cvs\pmc2\ctrller\fbin\global.h
	file path:	D:\cvs\pmc2\ctrller\fbin
	file base:	global
	file ext:	h
	author:	Jackie
	
	purpose:	API's private to fbin library
*********************************************************************/
#ifdef _WIN32
#define HAVE_STDIO
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef HAVE_STDIO
#include <stdio.h>
#include <assert.h>
#else
#define assert(s)
#endif

#include <string.h>
#include "f8.h"

#ifdef __LINUX__
#define stricmp strcasecmp
#define strcmpi strcasecmp
#endif

#ifndef BLK_HEAP_SIZE
	#ifdef _WIN32
		#define BLK_HEAP_SIZE	(10*1024*1024)
	#else
		#define BLK_HEAP_SIZE	(32*1024*1024)
	#endif
#endif

extern HF8_HEAP g_hBlkHeap;

__bool IBlk_connect(struct blk_pin_t * source, struct blk_pin_t * target);

__bool IBlk_disconnect	(struct blk_pin_t *t);

__bool IBlk_connectable(struct blk_pin_t * p);

__bool IBlk_compatible(struct blk_pin_t * s, struct blk_pin_t * t);

IBBlk * IBBlk_from_stream(IF8Stream * s);

ICBlk * ICBlk_from_stream(IF8Stream * s);

/*
	block library should not use the general purpose malloc/free
	procedures, instead, dynamic memory used here should always
	be maintained in the global heap: g_hBlkHeap.
	
	use mm_alloc/mm_free to access the heap.
*/
#undef __malloc__
#undef __free__

