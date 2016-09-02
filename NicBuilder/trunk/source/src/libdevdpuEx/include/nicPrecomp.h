//=========================================================================
//  Nicsys1000   版本:  2.0
//-------------------------------------------------------------------------
//  
//  实时库的通用操作宏定义定义文件
//
//-------------------------------------------------------------------------
//  版权 (C) 2012 - 北京中核控制系统工程有限公司(CNCS)
//=========================================================================
// 
//  改动历史：
//  	1. 2012/02/27 : 	CNCS-0136 建立文件
//=========================================================================
#ifndef __NICSYS_PROCOMP_H__
#define __NICSYS_PROCOMP_H__

//屏蔽C语言注释格式与.net的XML注释不符合警告
#pragma warning(disable:4635)

//屏蔽VS平台运用自带的安全C函数警告
#pragma warning(disable:4996)

#ifdef  __cplusplus
#define CDECL_BEGIN extern "C" {
#define CDECL_END }
#define DEFAULT_PARAM(val) = val
#else
#define CDECL_BEGIN 
#define CDECL_END 
#define DEFAULT_PARAM(val) 
#endif



/// <summary>
///	输出函数格式定义
/// </summary>
#ifdef _WIN32
#define NICSYS_API _stdcall
#define nicAPI  _stdcall
#else
#define NICSYS_API
#define nicAPI 
#endif


#define NICSYSDLL	
#define nicRET_OK	0 							// 返回成功
#define nicRET_Fail (-1)							// 返回失败，不同于返回错误
#define nicOk(ret)	(!((nicAPIStatus)(ret)))		// 判断返回值是否正确
#define nicErr(ret)	(((nicAPIStatus)(ret)))		// 判断返回值是否错误
#define nicIN								// 传入参数
#define nicOUT								// 传出参数
#define nicNULL			0					// 空


/// <summary>
///		判断指针是否为空
/// </summary>
#define IsNull(s) (!(s))

/// <summary>
///		判断指针是否非空
/// </summary>
#define IsNotNull(s) (s)


/// <summary>
///		如果返回结果不是nicRET_OK，则返回
/// </summary>
#define RET_ERR(s)				\
	nRet = (s);					\
	if (nicErr(nRet))			\
	{							\
	return nRet;			\
	}


#ifdef _WIN32
#define _WIN_HAVE_STRUPR
#define _WIN_HAVE_MILISECOND_SLEEP
#define _WIN_HAVE_SNPRINTF
#define _WIN_HAVE_PROFILE_SUPPORT
#endif


#ifdef __GNUC__
#define strcmpi  strcasecmp
#define strnicmp strncasecmp
#endif

#ifndef CDECL
#define CDECL
#endif

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#define __CONSTRUCTOR__ __attribute__((constructor))
#define __DESTRUCTOR__ __attribute__((destructor))
#define __FASTCALL__(x)	x __attribute__((regparm(3)))
#define __STDCALL__(x) x __attribute__((stdcall))
#else
#define PACKED
#define __CONSTRUCTOR__
#define __DESTRUCTOR__
#define __FASTCALL__(x) __fastcall x
#define __STDCALL__(x) __stdcall x
#endif

/* backward compatible */
#define CONFIG_FILE				"config/NicDB.ini"




//标签名长度
#define NAME_SIZE 32
//标签描述长度
#define TAG_DESC_SIZE 64
//文件名长度
#define MAX_FILE_NAME_LENGTH 64
//文件路径长度
#define MAX_FILE_PATH_LENGTH 512
//自定义增长长度
//#define DEFAULT_INCREASE_STEP 1024
#define DEFAULT_INCREASE_STEP 1024*512
//名字Map中的最大容量
//#define DEFAULT_NAMEHASH_SIZE (DEFAULT_INCREASE_STEP * 4)
#define DEFAULT_NAMEHASH_SIZE (DEFAULT_INCREASE_STEP )
//自定义的长度
#define SIZE_LENGTH_16  16
#define SIZE_LENGTH_08  8
#define SIZE_LENGTH_04  4
#define SIZE_LENGTH_32  32
#define SIZE_LENGTH_64  64
#define SIZE_LENGTH_128  128
#define SIZE_LENGTH_256  256
#define SIZE_LENGTH_512  512
#define SIZE_LENGTH_1024  1024
#define SIZE_LENGTH_2048	2048



//网络中通知事件的批处理最大容量
#define BATCH_NOTIFY_SIZE 512
#endif
