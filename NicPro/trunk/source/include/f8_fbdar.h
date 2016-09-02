/*
	block library management
	by Jackie Hunter(jhunter@tsinghua.org.cn)
	2003/11/12
*/

#ifndef __blklib_h__
#define __blklib_h__

#ifdef _WIN32
#ifdef LIBFBDAR_EXPORTS
#define FBDAR_API __declspec(dllexport)
#else
#define FBDAR_API __declspec(dllimport)
#endif
#define FBDAR_STDCALL __stdcall
#else
#define FBDAR_API
#define FBDAR_STDCALL
#endif

#include <f8_kernel.h>

#include <pshpack1.h>
struct blk_dir_t{
	char	* name;
}PACKED;

struct blk_ent_t{
	f8_u16	magic;
	f8_u16	pad;
	const char	* name;
	const char	* fileName;
	const char * dir;
}PACKED;

struct devlib_ent_t{
	char * name;
	char * description;
	struct io_device dev;
#ifdef _WIN32
	HANDLE configUiDll;
	f8_bool (__cdecl * configProc)(HWND parentWindow, __u8 * data, __uint len, __uint reserved);
#else
	f8_int foo[2];
#endif
};

#include <poppack.h>

typedef __bool (*ENUM_BLKDIR_PROC)(
	const struct blk_dir_t *, 
	__uint context
	);

typedef __bool (*ENUM_BLKENT_PROC)(
	const struct blk_ent_t *, 
	__uint context
	);

CDECL_BEGIN

FBDAR_API __bool is_valid_blk_name(
	const char * name
	);

FBDAR_API __bool is_valid_pin_name(
	const char * name
	);

FBDAR_API f8_status blklib_load_by_type(
	const char *type, 
	IBlk **blk
	);

FBDAR_API f8_status blklib_load_by_file(
	const char * file,
	IBlk **blk
	);

FBDAR_API f8_status blklib_load_by_id(const char *id, IBlk **blk);

FBDAR_API int blklib_save(
	const char *type, 
	IBlk *blk, 
	int reserved
	);

FBDAR_API void unload_user_blklib();

FBDAR_API void reload_user_blklib();

FBDAR_API void set_blklib_paths(
	const char * paths
	);

FBDAR_API const char * get_blklib_paths();

FBDAR_API void enumerate_blk_dir(
	ENUM_BLKDIR_PROC, 
	__uint context
	);

FBDAR_API void enumerate_blk_ent(
	ENUM_BLKENT_PROC, 
	const char * dirName,
	__uint context
	);

FBDAR_API void init_blklib();

FBDAR_API void uninit_blklib();

FBDAR_API __bool blklib_load_res(const f8_uuid * id);

FBDAR_API void probe_blk_resources(IBlk * b);

FBDAR_API __bool blk_to_file(
	IBlk * blk, 
	const char * file
	);

FBDAR_API void set_blklib_project_path(const char * p);

FBDAR_API __bool blklib_query(const f8_uuid * id, struct blk_ent_t * e);

FBDAR_API struct devlib_ent_t * blklib_get_dev_by_name(const char * name);

FBDAR_API struct devlib_ent_t * blklib_get_dev_by_id(const f8_uuid * id);

typedef __bool (FBDAR_STDCALL * ENUM_AVAILABLE_DEV_PROC)(
	struct devlib_ent_t * dev, 
	__uint context
	);

FBDAR_API void blklib_enum_available_devices(
	ENUM_AVAILABLE_DEV_PROC p,
	__uint context
	);

CDECL_END

#endif
