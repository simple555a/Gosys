#ifndef __f8_resource_h__
#define __f8_resource_h__

#include <c_object.h>
#include <streamable.h>
#include <f8_vm.h>

#ifdef _WIN32
#ifdef LIBCPRES_EXPORTS
#define F8RES_API __declspec(dllexport)
#else
#define F8RES_API __declspec(dllimport)
#endif
#else
#define F8RES_API
#endif

#define F8_RES_MAGIC		(('R' << 8) | 'S')
#define F8_RES_MAGIC_2		(('S' << 8) | 'R')
#define F8_MAX_KEY_LENGTH	128

/*
	A resource is a collection of data bound to a specified
	uuid. Like windows's registry hive, but F8's resource is 
	not maintained in a tree, rather like a linked list, all 
	resources are at the same level.
*/

CDECL_BEGIN

F8RES_API void create_f8_uuid(
	f8_uuid * id
	);

F8RES_API void f8_uuid_to_string(
	const f8_uuid * id,
	char * buf,
	int length
	);

F8RES_API void f8_uuid_from_string(
	f8_uuid * id,
	const char * buf
	);

F8RES_API __bool load_resource(
	const f8_uuid * id
	);

F8RES_API __bool load_res_stream(
	IF8Stream * s
	);

F8RES_API __bool load_res_stream_as(
	IF8Stream * s,
	const f8_uuid * newId
	);

F8RES_API __bool save_res_stream(
	const f8_uuid * id,
	IF8Stream * s
	);

F8RES_API __bool save_volatile_res(
	IF8Stream *s
	);

F8RES_API void reset_resources();

F8RES_API __bool unload_resource(
	const f8_uuid * id
	);

F8RES_API __int query_resource(
	const f8_uuid * id
	);

F8RES_API __bool create_resource(
	const f8_uuid * id
	);

F8RES_API __bool set_res_buf_v(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * key,
	...
	);

F8RES_API __bool set_res_buf(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * key
	);

F8RES_API __bool set_res_buf_ex(
	const f8_uuid * id,
	const void * buffer,
	int length,
	const char * key,
	__bool bForce
	);

F8RES_API __bool set_res_name(
	const f8_uuid * id,
	const char * name
	);	

F8RES_API const char * get_res_name(
	const f8_uuid * id
	);

F8RES_API char * get_res_buf_v(
	const f8_uuid * id,
	int *length,
	const char * keyFormat,
	...
	);

F8RES_API char * get_res_buf(
	const f8_uuid * id,
	int *length,
	const char * key
	);

F8RES_API __bool del_res_item(
	const f8_uuid * id, 
	const char * key
	);

F8RES_API __int get_res_int(
	const f8_uuid * id,
	const char * key,
	__int defval
	);

F8RES_API __int get_res_int_v(
	const f8_uuid * id,
	const char * keyFormat,
	__int defval,
	...
	);

F8RES_API __bool set_res_int(
	const f8_uuid * id,
	const char * key,
	__int val
	);

F8RES_API __bool set_res_int_v(
	const f8_uuid * id,
	const char * keyFormat,
	__int val,
	...
	);

typedef __bool (*_ERES_PROC)(
	const f8_uuid * id, 
	__int context
	);

typedef __bool (*_EITEM_PROC)(
	const char * key,							 
	const void * buf, 
	__int length, 
	__int context
	);

typedef void (*_DEL_RES_PROC)(
	const f8_uuid * rid
	);

F8RES_API __int enum_resources(
	_ERES_PROC p, 
	__int context
	);

F8RES_API __int enum_res_items(
	const f8_uuid *id,
	_EITEM_PROC p,
	__int context
	);

F8RES_API int copy_resource(
	const f8_uuid * s, 
	const f8_uuid * t
	);

/*
	move a copy of resource from s to t, note the
	original resource might persist if the reference count
	doesn't reach zero.
*/
F8RES_API int move_resource(
	const f8_uuid * s, 
	const f8_uuid * t
	);

F8RES_API __bool set_del_res_proc(
	f8_uuid * rid, 
	_DEL_RES_PROC p
	);

CDECL_END

#endif

