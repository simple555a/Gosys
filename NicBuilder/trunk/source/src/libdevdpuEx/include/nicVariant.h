
#ifndef __API_NICVARIANT__
#define __API_NICVARIANT__

#include "nicDefs.h"
#include "nicUtils.h"

CDECL_BEGIN

/// <summary>
///		[API] ΪnicVariant��������ռ䲢��ʼ��,������������
/// </summary>
UTILS_API nicAPIStatus  NICSYS_API NEW_Variant(nicVariant **ppVariant);
/// <summary>
///		ΪnicVariant��������ռ䲢��ʼ��
/// </summary>
 UTILS_API nicAPIStatus   NICSYS_API Variant_New(   nicIN nicDataTypeEnum nDataType,
							  nicOUT nicVariant **ppVariant);

/// <summary>
///		�ͷ��������nicVariant�����ռ�
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API  Variant_Free(nicIN nicOUT nicVariant **ppVariant);

/// <summary>
///		���nicVariant����
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_Clear(nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		����nicVariant����
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_Copy(   nicIN nicVariant *pSource,
							   nicIN nicOUT nicVariant *pDestination);

/// <summary>
///		��ANSI�ַ�������nicVariant����
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_AStrToVariant(	nicIN nicAStr strAStr,
										nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		��Unicode�ַ�������nicVariant����
/// </summary>
UTILS_API  nicAPIStatus NICSYS_API Variant_UStrToVariant(	nicIN nicUStr strUStr,
										nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		���ַ�������nicVariant����
/// </summary>
 UTILS_API nicAPIStatus NICSYS_API Variant_StrToVariant(    nicIN nicStr strStr,
									   nicIN nicOUT nicVariant *pVariant);

/// <summary>
///		�������ƴ�����nicVariant����
/// </summary>
UTILS_API nicAPIStatus NICSYS_API Variant_BlobToVariant(	nicIN nicByte *pByteList,
										nicIN nicUInt32 nLength,
										nicIN nicOUT nicVariant *pVariant);

 /// <summary>
 ///		�Ƚ�����nicVariantֵ�Ƿ����
 /// </summary>
UTILS_API nicBool NICSYS_API Variant_Equal(nicVariant *pFirst, nicVariant *pSecond);

/// <summary>
///		�����ݴ��ڴ浽Variant�ṹ�壬��������Ӧ���ͣ�
///     �����߸���֤����ͳһ�������һ������ΪnicInt ��nicVariant �д���һ��
///     �ַ���������ƿ飬����ǲ���Ԥ֪�ģ�
/// </summary>
UTILS_API nicUInt32 NICSYS_API mem_copyto_Variant( nicVariant *Pdest,
							void * Pmem,const unsigned length);

UTILS_API nicDouble NICSYS_API nicVariant_GetDouble (const nicVariant* value);

 UTILS_API nicAPIStatus NICSYS_API VariantTypeCast(nicUInt8 DataType, nicVariant* var_src,nicVariant* var_dst );


CDECL_END

#endif
