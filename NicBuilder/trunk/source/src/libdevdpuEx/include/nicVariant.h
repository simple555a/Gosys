
#ifndef __API_NICVARIANT__
#define __API_NICVARIANT__

#include "nicDefs.h"
#include "nicUtils.h"

CDECL_BEGIN

/// <summary>
///		[API] 为nicVariant变量申请空间并初始化,但不设置类型
/// </summary>
UTILS_API nicAPIStatus  NICSYS_API NEW_Variant(nicVariant **ppVariant);
/// <summary>
///		为nicVariant变量申请空间并初始化
/// </summary>
 UTILS_API nicAPIStatus   NICSYS_API Variant_New(   nicIN nicDataTypeEnum nDataType,
							  nicOUT nicVariant **ppVariant);

/// <summary>
///		释放已申请的nicVariant变量空间
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API  Variant_Free(nicIN nicOUT nicVariant **ppVariant);

/// <summary>
///		清空nicVariant变量
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_Clear(nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		复制nicVariant变量
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_Copy(   nicIN nicVariant *pSource,
							   nicIN nicOUT nicVariant *pDestination);

/// <summary>
///		将ANSI字符串赋给nicVariant变量
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_AStrToVariant(	nicIN nicAStr strAStr,
										nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		将Unicode字符串赋给nicVariant变量
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_UStrToVariant(	nicIN nicUStr strUStr,
										nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		将字符串赋给nicVariant变量
/// </summary>
 UTILS_API nicAPIStatus NICSYS_API Variant_StrToVariant(    nicIN nicStr strStr,
									   nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		将二进制串赋给nicVariant变量
/// </summary>
UTILS_API nicAPIStatus NICSYS_API Variant_BlobToVariant(	nicIN nicByte *pByteList,
										nicIN nicUInt32 nLength,
										nicIN nicOUT nicVariant *pVariant);

 /// <summary>
 ///		比较两个nicVariant值是否相等
 /// </summary>
UTILS_API nicBool NICSYS_API Variant_Equal(nicVariant *pFirst, nicVariant *pSecond);

/// <summary>
///		将数据从内存到Variant结构体，必须存入对应类型；
///     调用者负责保证类型统一，如果在一个类型为nicInt 的nicVariant 中存入一个
///     字符串或二进制块，结果是不可预知的！
/// </summary>
UTILS_API nicUInt32 NICSYS_API mem_copyto_Variant( nicVariant *Pdest,
							void * Pmem,const unsigned length);

UTILS_API nicDouble NICSYS_API nicVariant_GetDouble (const nicVariant* value);

 UTILS_API nicAPIStatus NICSYS_API VariantTypeCast(nicUInt8 DataType, nicVariant* var_src,nicVariant* var_dst );


CDECL_END

#endif
