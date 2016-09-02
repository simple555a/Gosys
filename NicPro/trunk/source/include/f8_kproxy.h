/********************************************************************
	created:	2002:10:12   15:23
	author:		Jackie, jhunter@tsinghua.org.cn
	
	purpose:	F8 kernel
*********************************************************************/
#ifndef __f8kproxy_h__
#define __f8kproxy_h__

#include <f8_kernel.h>
#include <f8_kadapter.h>

#include <pshpack1.h>

#ifdef _WIN32
#ifdef LIBKPROXY_EXPORTS
#define KPROXY_API __declspec(dllexport)
#else
#define KPROXY_API __declspec(dllimport)
#endif
#else
#define KPROXY_API
#endif

CDECL_BEGIN
	
KPROXY_API f8_bool kproxy_init(int leakDetect);

KPROXY_API void kproxy_uninit();

KPROXY_API f8_int kproxy_shell(const char * cmd);

KPROXY_API f8_int kproxy_shell_ex(
	const char * cmd, 
	__uint timeout, 
	void * reserved
	);

KPROXY_API f8_int kproxy_shell_v(const char * cmd, ...);


/*
	2004/3/2 soycola, temporary interface to delphi clients
*/
KPROXY_API HF8_KERNEL kproxy_get_kernel();

KPROXY_API IKAdapter * kproxy_get_adapter();

KPROXY_API f8_uint query_var_list(
	IBlk * blk,
	struct blk_var_t * buffer,
	f8_uint * items
	);

KPROXY_API struct blk_var_t * query_var(
	IBlk * blk,
	const char * name
	);

KPROXY_API struct blk_var_t * query_var_by_pin(
	struct blk_pin_t * p
	);

KPROXY_API struct blk_var_t * query_var_by_addr(
	struct mem_addr_t * addr
	);

KPROXY_API f8_status set_var(
	f8_byte scope,
	const char * name,
	struct blk_pin_t * p
	);

KPROXY_API f8_status uset_var(
	f8_byte scope,
	const char * name
	);

KPROXY_API f8_uint get_var_count(IBlk * blk);

typedef __bool (*_ENUM_VAR_PROC)(
	struct blk_var_t * v, 
	__uint context
	);

KPROXY_API __uint enum_variables(
	IBlk * blk, 
	_ENUM_VAR_PROC p, 
	__uint context
	);

KPROXY_API f8_int get_var_id(const struct blk_var_t * v);

KPROXY_API struct blk_var_t * query_var_by_fullname(
	const char * name
	);

KPROXY_API struct blk_var_t * query_var_by_id(
	IBlk * b, 
	f8_int id
	);

struct paste_obj_t{
	void	* obj;
	f8_u16	magic;
}PACKED;

struct copy_paste_info{
	f8_uuid	source;
	f8_uuid	target;
	int		objCount;
	struct paste_obj_t	objects[1];
}PACKED;

KPROXY_API int kproxy_copy(
	f8_uuid *source, 
	f8_u16 *magic, 
	void **src, 
	int count
	);

KPROXY_API void * kproxy_paste(IBlk * target);

KPROXY_API void kproxy_delete_paste(void * h);

KPROXY_API struct paste_obj_t * kproxy_get_pasted_object(
	void * h, 
	int index
	);

KPROXY_API void kproxy_get_mem_usage(int sizes[KERN_NUM_SECTIONS][2]);

extern KPROXY_API IKAdapter * proxy_adapter;
extern KPROXY_API __bool proxy_silent;

CDECL_END

#include <poppack.h>

#endif	/**/

