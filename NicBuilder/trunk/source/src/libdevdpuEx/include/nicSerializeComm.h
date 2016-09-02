

#ifndef __API_COMMFUNCTION__
#define __API_COMMFUNCTION__

#include "nicSerializeData.h"

//=========================================================================
//  公共内联函数定义及实现
//=========================================================================
/// <summary> 
///		序列化时表示空指针
/// </summary> 
#define nicNullFlag 0xFFFFFFFF

/// <summary> 
///		序列化nicStr类型
/// </summary> 
__inline nicAPIStatus SerializeStr(
	serializeData& s, 
	nicStr* pStr)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;
	nicUInt32 nLength;

	_ASSERT(pStr!=nicNULL);

	if (IsNull(*pStr))
	{
		nLength = nicNullFlag;
		RET_ERR(s.Serialize(&nLength));
	}
	else
	{
		nLength = (nicUInt32)strlen(*pStr);
		RET_ERR(s.Serialize(&nLength));
		if (nLength>0)
		{
			nSize = nLength * sizeof(nicChar);
			RET_ERR(s.SerializeMemory(*pStr,nSize));
		}
	}
	return nicRET_OK;
}

/// <summary> 
///		反序列化nicStr类型
/// </summary> 
__inline nicAPIStatus UnSerializeStr(
									serializeData& s, 
									nicStr* pStr)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;
	nicUInt32 nLength;

	_ASSERT(pStr!=nicNULL);

	RET_ERR(s.UnSerialize(&nLength));
	if (nLength==nicNullFlag)
	{
		*pStr = nicNULL;
		return nicRET_OK;
	}
	nSize = nLength*sizeof(nicChar);
	RET_ERR(NewMemory((void**)(pStr),nSize+sizeof(nicChar)));

	**pStr = 0;
	if (nLength>0)
	{
		RET_ERR(s.UnSerializeMemory(*pStr,nSize));
	}
	*(*pStr+nLength)=0;

	return nicRET_OK;
}

__inline nicAPIStatus SerializeAnsiString(
	serializeData& s, 
	nicAnsiString* pAnsiString)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pAnsiString!=nicNULL);

	RET_ERR(s.Serialize(&(pAnsiString->Length)));
	if (pAnsiString->Length>0)
	{
		_ASSERT(pAnsiString->Data!=nicNULL);
		nSize = pAnsiString->Length * sizeof(nicAChar);
		RET_ERR(s.SerializeMemory(pAnsiString->Data,nSize));
	}

	return nicRET_OK;
}

__inline nicAPIStatus UnSerializeAnsiString(
	serializeData& s, 
	nicAnsiString* pAnsiString)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pAnsiString!=nicNULL);
	_ASSERT(pAnsiString->Data==nicNULL);

	RET_ERR(s.UnSerialize(&(pAnsiString->Length)));
	if (pAnsiString->Length>0)
	{
		nSize = pAnsiString->Length * sizeof(nicAChar);
		RET_ERR(NewMemory((void**)(&(pAnsiString->Data)), nSize+sizeof(nicAChar)));
		RET_ERR(s.UnSerializeMemory(pAnsiString->Data,nSize));
		*(pAnsiString->Data+pAnsiString->Length) = 0;
	}
	return nicRET_OK;
}

__inline nicAPIStatus SerializeUnicodeString(
	serializeData& s, 
	nicUnicodeString* pUnicodeString)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pUnicodeString!=nicNULL);

	RET_ERR(s.Serialize(&(pUnicodeString->Length)));
	if (pUnicodeString->Length>0)
	{
		_ASSERT(pUnicodeString->Data!=nicNULL);
		nSize = pUnicodeString->Length * sizeof(nicUChar);
		RET_ERR(s.SerializeMemory(pUnicodeString->Data,nSize));
	}

	return nicRET_OK;
}

__inline nicAPIStatus UnSerializeUnicodeString(
	serializeData& s, 
	nicUnicodeString* pUnicodeString)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pUnicodeString!=nicNULL);
	_ASSERT(pUnicodeString->Data==nicNULL);

	RET_ERR(s.UnSerialize(&(pUnicodeString->Length)));
	if (pUnicodeString->Length>0)
	{
		nSize = pUnicodeString->Length * sizeof(nicUChar);
		RET_ERR(NewMemory((void**)(&(pUnicodeString->Data)), nSize+sizeof(nicUChar)));
		RET_ERR(s.UnSerializeMemory(pUnicodeString->Data,nSize));
		*(pUnicodeString->Data+pUnicodeString->Length) = 0;
	}
	return nicRET_OK;
}

__inline nicAPIStatus SerializeBlob(
								   serializeData& s, 
								   nicBlob* pBlob)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pBlob!=nicNULL);

	RET_ERR(s.Serialize(&(pBlob->Length)));
	if (pBlob->Length>0)
	{
		_ASSERT(pBlob->Data!=nicNULL);
		nSize = pBlob->Length * sizeof(nicByte);
		RET_ERR(s.SerializeMemory(pBlob->Data,nSize));
	}

	return nicRET_OK;
}

__inline nicAPIStatus UnSerializeBlob(
									 serializeData& s, 
									 nicBlob* pBlob)
{
	nicAPIStatus nRet;
	nicUInt32 nSize;

	_ASSERT(pBlob!=nicNULL);
	_ASSERT(pBlob->Data==nicNULL);

	RET_ERR(s.UnSerialize(&(pBlob->Length)));
	if (pBlob->Length>0)
	{
		nSize = pBlob->Length * sizeof(nicByte);
		RET_ERR(NewMemory((void**)(&(pBlob->Data)), nSize));
		RET_ERR(s.UnSerializeMemory(pBlob->Data,nSize));
	}
	return nicRET_OK;
}

__inline nicAPIStatus SerializeVariant(
									  serializeData& s, 
									  nicVariant* pVariant)
{
	nicAPIStatus nRet;
	nicUInt8* pDataType;

	_ASSERT(pVariant!=nicNULL);
	pDataType = &(pVariant->DataType);
	RET_ERR(s.Serialize(pDataType));
	switch(*pDataType)
	{
	case nicDataType_Empty :					// 未定义
		break;
	case nicDataType_Bool :						// 开关量
		RET_ERR(s.Serialize(&(pVariant->Bool)));
		break;
	case nicDataType_Int8 :						// 一字节整数
		RET_ERR(s.Serialize(&(pVariant->Int8)));
		break;
	case nicDataType_UInt8 :						// 一字节无符号整数
		RET_ERR(s.Serialize(&(pVariant->UInt8)));
		break;
	case nicDataType_Int16 :						// 二字节整数
		RET_ERR(s.Serialize(&(pVariant->Int16)));
		break;
	case nicDataType_UInt16 :						// 二字节无符号整数
		RET_ERR(s.Serialize(&(pVariant->UInt16)));
		break;
	case nicDataType_Int32 :						// 四字节整数
		RET_ERR(s.Serialize(&(pVariant->Int32)));
		break;
	case nicDataType_UInt32 :						// 四字节无符号整数
		RET_ERR(s.Serialize(&(pVariant->UInt32)));
		break;
	case nicDataType_Int64 :						// 八字节整数
		RET_ERR(s.Serialize(&(pVariant->Int64)));
		break;
	case nicDataType_UInt64 :						// 八字节无符号整数
		RET_ERR(s.Serialize(&(pVariant->UInt64)));
		break;
	case nicDataType_Float :						// 单精度浮点数
		RET_ERR(s.Serialize(&(pVariant->Float)));
		break;
	case nicDataType_Double :						// 双精度浮点数
		RET_ERR(s.Serialize(&(pVariant->Double)));
		break;
	case nicDataType_Time :						// 时间
		RET_ERR(s.Serialize(&(pVariant->Time)));
		break;
	case nicDataType_AnsiString :					// ANSI字符串
		RET_ERR(SerializeAnsiString(s,&(pVariant->AnsiString)));
		break;
	case nicDataType_String :					// ANSI字符串
		RET_ERR(SerializeAnsiString(s,&(pVariant->AnsiString)));
		break;
	case nicDataType_UnicodeString :				// Unicode字符串
		RET_ERR(SerializeUnicodeString(s,&(pVariant->UnicodeString)));
		break;
	case nicDataType_Blob :						// 二进制数据块
		RET_ERR(SerializeBlob(s,&(pVariant->Blob)));
		break;
	default:
		//_ASSERT(false);
		*pDataType = nicDataType_Empty;
		nicBlob* pBlob = new nicBlob();
		pBlob->Length = 0;
		pBlob->Data = nicNULL;
		RET_ERR(s.Serialize(pDataType));
		RET_ERR(SerializeBlob(s,pBlob));
		if (pBlob != nicNULL)
		{
			delete pBlob;
		}
		break;
	}	
	return nicRET_OK;
}

__inline nicAPIStatus UnSerializeVariant(
										serializeData& s, 
										nicVariant* pVariant)
{
	nicAPIStatus nRet;
	nicUInt8* pDataType;

	_ASSERT(pVariant!=nicNULL);

	ClearVariant(pVariant);

	pDataType = &(pVariant->DataType);

	RET_ERR(s.UnSerialize(pDataType));

	switch(*pDataType)
	{
	case nicDataType_Empty :					// 未定义
		break;
	case nicDataType_Bool :						// 开关量
		RET_ERR(s.UnSerialize(&(pVariant->Bool)));
		break;
	case nicDataType_Int8 :						// 一字节整数
		RET_ERR(s.UnSerialize(&(pVariant->Int8)));
		break;
	case nicDataType_UInt8 :						// 一字节无符号整数
		RET_ERR(s.UnSerialize(&(pVariant->UInt8)));
		break;
	case nicDataType_Int16 :						// 二字节整数
		RET_ERR(s.UnSerialize(&(pVariant->Int16)));
		break;
	case nicDataType_UInt16 :						// 二字节无符号整数
		RET_ERR(s.UnSerialize(&(pVariant->UInt16)));
		break;
	case nicDataType_Int32 :						// 四字节整数
		RET_ERR(s.UnSerialize(&(pVariant->Int32)));
		break;
	case nicDataType_UInt32 :						// 四字节无符号整数
		RET_ERR(s.UnSerialize(&(pVariant->UInt32)));
		break;
	case nicDataType_Int64 :						// 八字节整数
		RET_ERR(s.UnSerialize(&(pVariant->Int64)));
		break;
	case nicDataType_UInt64 :						// 八字节无符号整数
		RET_ERR(s.UnSerialize(&(pVariant->UInt64)));
		break;
	case nicDataType_Float :						// 单精度浮点数
		RET_ERR(s.UnSerialize(&(pVariant->Float)));
		break;
	case nicDataType_Double :						// 双精度浮点数
		RET_ERR(s.UnSerialize(&(pVariant->Double)));
		break;
	case nicDataType_Time :						// 时间
		RET_ERR(s.UnSerialize(&(pVariant->Time)));
		break;
	case nicDataType_AnsiString :					// ANSI字符串
		RET_ERR(UnSerializeAnsiString(s,&(pVariant->AnsiString)));
		break;
	case nicDataType_String :					// ANSI字符串
		RET_ERR(UnSerializeAnsiString(s,&(pVariant->AnsiString)));
		break;
	case nicDataType_UnicodeString :				// Unicode字符串
		RET_ERR(UnSerializeUnicodeString(s,&(pVariant->UnicodeString)));
		break;
	case nicDataType_Blob :						// 二进制数据块
		RET_ERR(UnSerializeBlob(s,&(pVariant->Blob)));
		break;
	default:
		//_ASSERT(false);
		*pDataType = nicDataType_Empty;
		nicBlob* pBlob = new nicBlob();
		pBlob->Length = 0;
		pBlob->Data = nicNULL;
		RET_ERR(s.UnSerialize(pDataType));
		RET_ERR(UnSerializeBlob(s,pBlob));
		if (pBlob != nicNULL)
		{
			delete pBlob;
		}
		break;

	}	
	return nicRET_OK;
}


//=========================================================================
// 
// PropValue结构体的序列化、反序列化等功能定义
// 单个属性的值
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializePropValue(
										serializeData& s, 
										nicPropValue* pPropValue)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pPropValue!=nicNULL);

	RET_ERR(s.Serialize(&(pPropValue->PropId)));
	RET_ERR(SerializeVariant(s,&(pPropValue->Value)));

	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializePropValue(
	serializeData& s, 
	nicPropValue* pPropValue)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pPropValue!=nicNULL);

	RET_ERR(s.UnSerialize(&(pPropValue->PropId)));
	RET_ERR(UnSerializeVariant(s,&(pPropValue->Value)));

	return nicRET_OK;		
}

//=========================================================================
// 
// PropValueList结构体的序列化、反序列化等功能定义
// 属性值列表
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializePropValueList(
	serializeData& s, 
	nicPropValueList* pPropValueList)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pPropValueList!=nicNULL);

	RET_ERR(s.Serialize(&(pPropValueList->PropCount)));
	nicUInt16 nCount = pPropValueList->PropCount;
	nicUInt16 i;
	nicPropValue* pPropValue;
	if (nCount>0)
	{
		ACE_ASSERT(pPropValueList->PropValueList!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pPropValue = pPropValueList->PropValueList+i;
			RET_ERR(SerializePropValue(s,pPropValue));
		}
	}

	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializePropValueList(
	serializeData& s, 
	nicPropValueList* pPropValueList)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pPropValueList!=nicNULL);

	RET_ERR(s.UnSerialize(&(pPropValueList->PropCount)));
	nicUInt32 nSize;
	nicUInt16 nCount = pPropValueList->PropCount;
	nicUInt16 i;
	nicPropValue* pPropValue;
	if (nCount==0)
	{
		pPropValueList->PropValueList=nicNULL;
	}
	else
	{
		nSize = sizeof(nicPropValue)*(nCount) ;
		RET_ERR(NewAndClear((void**)(&(pPropValueList->PropValueList)), nSize));
		ACE_ASSERT(pPropValueList->PropValueList!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pPropValue = pPropValueList->PropValueList+i;
			RET_ERR(UnSerializePropValue(s,pPropValue));
		}
	}

	return nicRET_OK;		
}

//=========================================================================
// 
// Data结构体的序列化、反序列化等功能定义
// 数据记录定义
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializeData(
								   serializeData& s, 
								   nicData* pData )
{
	nicAPIStatus nRet;
	ACE_ASSERT(pData!=nicNULL);


	RET_ERR(s.Serialize(&(pData->Time)));


	RET_ERR(SerializeVariant(s,&(pData->Value)));


	RET_ERR(s.Serialize(&(pData->Quality)));


	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializeData(
									 serializeData& s, 
									 nicData* pData)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pData!=nicNULL);


	RET_ERR(s.UnSerialize(&(pData->Time)));


	RET_ERR(UnSerializeVariant(s,&(pData->Value)));


	RET_ERR(s.UnSerialize(&(pData->Quality)));


	return nicRET_OK;		
}

//=========================================================================
// 
// DataList结构体的序列化、反序列化等功能定义
// 数据记录列表
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializeDataList(
									   serializeData& s, 
									   nicDataList* pDataList)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pDataList!=nicNULL);

	RET_ERR(s.Serialize(&(pDataList->DataCount)));
	nicUInt32 nCount = pDataList->DataCount;
	nicUInt32 i;
	nicData* pData;
	if (nCount>0)
	{
		ACE_ASSERT(pDataList->DataList!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pData = pDataList->DataList+i;
			RET_ERR(SerializeData(s,pData));
		}
	}

	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializeDataList(
	serializeData& s, 
	nicDataList* pDataList)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pDataList!=nicNULL);

	RET_ERR(s.UnSerialize(&(pDataList->DataCount)));
	nicUInt32 nSize;
	nicUInt32 nCount = pDataList->DataCount;
	nicUInt32 i;
	nicData* pData;
	if (nCount==0)
	{
		pDataList->DataList=nicNULL;
	}
	else
	{
		nSize = sizeof(nicData)*(nCount) ;
		RET_ERR(NewAndClear((void**)(&(pDataList->DataList)), nSize));
		ACE_ASSERT(pDataList->DataList!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pData = pDataList->DataList+i;
			RET_ERR(UnSerializeData(s,pData));
		}
	}

	return nicRET_OK;		
}

//=========================================================================
// 
// RealData结构体的序列化、反序列化等功能定义
// 测点实时值
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializeRealData(
	serializeData& s, 
	nicRealData* pTagRealData )
{
	nicAPIStatus nRet;
	ACE_ASSERT(pTagRealData!=nicNULL);

	RET_ERR(s.Serialize(&(pTagRealData->TagId)));
	RET_ERR(SerializeData(s,&(pTagRealData->RealData)));

	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializeRealData(
	serializeData& s, 
	nicRealData* pTagRealData )
{
	nicAPIStatus nRet;
	ACE_ASSERT(pTagRealData!=nicNULL);

	RET_ERR(s.UnSerialize(&(pTagRealData->TagId)));
	RET_ERR(UnSerializeData(s,&(pTagRealData->RealData)));

	return nicRET_OK;		
}

//=========================================================================
// 
// RealDataList结构体的序列化、反序列化等功能定义
// 多个测点值列表
// 
//=========================================================================

/// <summary> 
/// 序列化
/// </summary> 
__inline nicAPIStatus SerializeRealDataList(
	serializeData& s, 
	nicRealDataList* pTagListRealData)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pTagListRealData!=nicNULL);

	RET_ERR(s.Serialize(&(pTagListRealData->TagCount)));
	nicUInt32 nCount = pTagListRealData->TagCount;
	nicUInt32 i;
	nicRealData* pTagRealData;
	if (nCount>0)
	{
		ACE_ASSERT(pTagListRealData->TagListRealData!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pTagRealData = pTagListRealData->TagListRealData+i;
			RET_ERR(SerializeRealData(s,pTagRealData));
		}
	}

	return nicRET_OK;
}

/// <summary> 
/// 反序列化
/// </summary> 
__inline nicAPIStatus UnSerializeRealDataList(
	serializeData& s, 
	nicRealDataList* pTagListRealData)
{
	nicAPIStatus nRet;
	ACE_ASSERT(pTagListRealData!=nicNULL);

	RET_ERR(s.UnSerialize(&(pTagListRealData->TagCount)));
	nicUInt32 nSize;
	nicUInt32 nCount = pTagListRealData->TagCount;
	nicUInt32 i;
	nicRealData* pTagRealData;
	if (nCount==0)
	{
		pTagListRealData->TagListRealData=nicNULL;
	}
	else
	{
		nSize = sizeof(nicRealData)*(nCount) ;
		RET_ERR(NewAndClear((void**)(&(pTagListRealData->TagListRealData)), nSize));
		ACE_ASSERT(pTagListRealData->TagListRealData!=nicNULL);
		for (i=0;i<nCount;++i)
		{
			pTagRealData = pTagListRealData->TagListRealData+i;
			RET_ERR(UnSerializeRealData(s,pTagRealData));
		}
	}

	return nicRET_OK;		
}
#endif