#ifndef _NICSYS_FUNCTION_H__
#define _NICSYS_FUNCTION_H__
#include "nicDefs.h"
#include "nicError.h"
#include "nicRefACE.h"
__inline nicAPIStatus NewMemory(void** ppDest, nicUInt32 nSize)
{
	ACE_ASSERT(ppDest !=nicNULL);
	ACE_ASSERT(nSize>0);

	*ppDest = nicNULL;

	*ppDest = ACE_OS::malloc(nSize);
	if (*ppDest==nicNULL)
	{
		return nicERR_COMMON_NO_MEMORY;
	}
	return nicRET_OK;
}

__inline nicAPIStatus NewAndClear(void** ppDest, nicUInt32 nSize)
{
	if (nicErr(NewMemory(ppDest,nSize)))
	{
		return nicERR_COMMON_NO_MEMORY;
	}
	ACE_OS::memset(*ppDest,0,nSize);
	return nicRET_OK;
}

__inline void FreeAndNull(void** ppDest)
{
	if (ppDest==nicNULL)
	{
		return;
	}
	if (*ppDest!=nicNULL)
	{
		ACE_OS::free(*ppDest);
		*ppDest = nicNULL;
	}
}

__inline void FreeStr(nicStr* nictr)
{
	FreeAndNull((void**)nictr);
}

__inline void ClearStr(nicStr* nictr)
{
	ACE_ASSERT(nictr!=nicNULL);
	if (*nictr!=nicNULL)
	{
		ACE_OS::free(*nictr);
	}
	*nictr = nicNULL;
}

__inline void ClearStr(nicStr nictr)
{
	ClearStr(&nictr);
}

__inline nicAPIStatus NewAndCopyStr(nicStr* nictrDest,const nicChar * strSource)
{
	nicAPIStatus nRet = nicRET_OK;
	RET_ERR(NewMemory((nicVoid**)nictrDest,
		(nicUInt32)(ACE_OS::strlen(strSource)+1)*sizeof(nicChar)));  /*修改原因:定义unique情况下申请空间长度不够*/
	ACE_OS::strcpy(*nictrDest,strSource);
	return nicRET_OK;
}

__inline nicAPIStatus NewAndCopyUStr(nicUStr* nictrDest,const nicUChar * strSource)
{
	nicAPIStatus nRet = nicRET_OK;
	RET_ERR(NewMemory((nicVoid**)nictrDest,
		(nicUInt32)(ACE_OS::strlen((const ACE_WCHAR_T *)strSource)+1)*sizeof(nicUChar)));  /*修改原因:定义unique情况下申请空间长度不够*/
	ACE_OS::strcpy((ACE_WCHAR_T *)*nictrDest,(const ACE_WCHAR_T *)strSource);
	return nicRET_OK;
}

__inline nicAPIStatus nicStrRplace (nicStr* nictrDest,nicStr strSource)
{
	nicAPIStatus nRet = nicRET_OK;
	ACE_ASSERT(nictrDest!=nicNULL);
	if ( IsNotNull(*nictrDest) )
	{
		ClearStr(nictrDest);
	}
	if (strSource)
	{
		RET_ERR(NewAndCopyStr(nictrDest,strSource));
	}
	return nicRET_OK;
}

__inline nicBool IsVariantVariable(nicVariant* pVariant)
{
	return (pVariant->DataType == nicDataType_AnsiString ||
		pVariant->DataType == nicDataType_UnicodeString ||
		pVariant->DataType == nicDataType_String ||
		pVariant->DataType == nicDataType_Blob);
}

__inline nicBool IsVariantFixed(nicVariant* pVariant)
{
	return (!IsVariantVariable(pVariant));
}

__inline void ClearVariant(nicVariant* pVariant)
{
	ACE_ASSERT(pVariant!=nicNULL);
	if (IsVariantVariable(pVariant) && (pVariant->Blob.Length!=0))
	{
		ACE_ASSERT(pVariant->Blob.Data!=nicNULL);
		ACE_OS::free(pVariant->Blob.Data);
	}
	ACE_OS::memset(pVariant,0,sizeof(nicVariant));
}

__inline void FreeVariant(nicVariant** ppVariant)
{
	if (ppVariant==nicNULL)
	{
		return;
	}
	if (*ppVariant==nicNULL)
	{
		return;
	}
	ClearVariant(*ppVariant);
	FreeAndNull((void**)ppVariant);
}


__inline void nicTime_localtime(nicTime* nTime)
{
	nTime->Millisec=0;	
	time_t t_time=ACE_OS::time();
	nTime->Second=static_cast<nicUInt32>(t_time);
}
//处理时间差
__inline nicTime nicTime_difftime(nicTime* sTime,nicTime *eTime)
{	
	nicTime dTime ;

	nicUInt64 time = eTime->Second *1000+eTime->Millisec - sTime->Second*1000-sTime->Millisec;

	dTime.Second= (nicUInt32)(time/1000);
	dTime.Millisec = time %1000;
	return dTime;
}



//=========================================================================
// 
// StrList结构体的序列化、反序列化等功能定义
// 字段串列表
// 
//=========================================================================

__inline void ClearStrList(nicStrList* nictrList)
{
	if (nictrList==nicNULL)
	{
		return ;
	}

	nicUInt32 i;
	nicStr* p;
	if (nictrList->Count>0)
	{
		for (i=0;i<nictrList->Count;++i)
		{
			p = nictrList->List+i;
			ClearStr(p);
		}
		ACE_OS::free(nictrList->List);
	}

	ACE_OS::memset(nictrList,0,sizeof(nicStrList));
}
//ce
__inline void FreeStrList(nicStrList** pnictrList)
{
	if (pnictrList==nicNULL)
	{
		return;
	}
	if (*pnictrList==nicNULL)
	{
		return;
	}
	ClearStrList(*pnictrList);
	ACE_OS::free(*pnictrList);
	*pnictrList = nicNULL;
}

__inline nicAPIStatus NewStrList(nicStrList** pnictrList)
{
	return NewAndClear((nicVoid**)pnictrList,(nicUInt32)sizeof(nicStrList));
}
//ce
__inline nicAPIStatus NewStrListEx(nicStrList **pnictrList,nicUInt32 nCount)
{

	ACE_ASSERT(pnictrList!=nicNULL);
	ACE_ASSERT(*pnictrList==nicNULL);
	nicAPIStatus nRet = nicRET_OK;
	RET_ERR(NewAndClear((nicVoid**)pnictrList,sizeof(nicStrList)));
	nicStrList* nictrList = *pnictrList;
	nictrList->Count = nCount;
	nRet=(NewAndClear((nicVoid**)&(nictrList->List),sizeof(nicStr)*nCount));
	if (nicErr(nRet))
	{
		FreeStrList(pnictrList);
		return nRet;
	}
	return nRet;
}



//=========================================================================
// 
// PropValue结构体的序列化、反序列化等功能定义
// 单个属性的值
// 
//=========================================================================

__inline void ClearPropValue(nicPropValue* pPropValue)
{
	if (pPropValue==nicNULL)
	{
		return ;
	}

	ClearVariant(&(pPropValue->Value));

	ACE_OS::memset(pPropValue,0,sizeof(nicPropValue));
}

__inline void FreePropValue(nicPropValue** ppPropValue)
{
	if (ppPropValue==nicNULL)
	{
		return;
	}
	if (*ppPropValue==nicNULL)
	{
		return;
	}
	ClearPropValue(*ppPropValue);
	ACE_OS::free(*ppPropValue);
	*ppPropValue = nicNULL;
}

__inline nicAPIStatus NewPropValue(nicPropValue** ppPropValue)
{
	return NewAndClear((nicVoid**)ppPropValue,(nicUInt32)sizeof(nicPropValue));
}


//=========================================================================
// 
// PropValueList结构体的序列化、反序列化等功能定义
// 属性值列表
// 
//=========================================================================

__inline void ClearPropValueList(nicPropValueList* pPropValueList)
{
	if (pPropValueList==nicNULL)
	{
		return ;
	}

	nicUInt16 i;
	nicPropValue* p;
	if (pPropValueList->PropCount>0)
	{
		for (i=0;i<pPropValueList->PropCount;++i)
		{
			p = pPropValueList->PropValueList+i;
			ClearPropValue(p);
		}
		ACE_OS::free(pPropValueList->PropValueList);
	}

	ACE_OS::memset(pPropValueList,0,sizeof(nicPropValueList));
}
//ce
__inline void FreePropValueList(nicPropValueList** ppPropValueList)
{
	if (ppPropValueList==nicNULL)
	{
		return;
	}
	if (*ppPropValueList==nicNULL)
	{
		return;
	}
	ClearPropValueList(*ppPropValueList);
	ACE_OS::free(*ppPropValueList);
	*ppPropValueList = nicNULL;
}

__inline nicAPIStatus NewPropValueList(nicPropValueList** ppPropValueList)
{
	return NewAndClear((nicVoid**)ppPropValueList,(nicUInt32)sizeof(nicPropValueList));
}
//ce
__inline nicAPIStatus NewPropValueListEx(nicPropValueList **ppPropValueList,nicUInt16 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppPropValueList!=nicNULL);
	ACE_ASSERT(*ppPropValueList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppPropValueList,sizeof(nicPropValueList)));
	nicPropValueList* pPropValueList = *ppPropValueList;
	pPropValueList->PropCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pPropValueList->PropValueList),sizeof(nicPropValue)*nCount);
	if(nicErr(nRet))
	{
		FreePropValueList(ppPropValueList);
	}
	return nRet;
}


//=========================================================================
// 
// APIStatusList结构体的序列化、反序列化等功能定义
// 返回状态列表
// 
//=========================================================================

__inline void ClearAPIStatusList(nicAPIStatusList* pAPIStatusList)
{
	if (pAPIStatusList==nicNULL)
	{
		return ;
	}

	ACE_OS::free(pAPIStatusList->List);


	ACE_OS::memset(pAPIStatusList,0,sizeof(nicAPIStatusList));
}

__inline void FreeAPIStatusList(nicAPIStatusList** ppAPIStatusList)
{
	if (ppAPIStatusList==nicNULL)
	{
		return;
	}
	if (*ppAPIStatusList==nicNULL)
	{
		return;
	}
	ClearAPIStatusList(*ppAPIStatusList);
	ACE_OS::free(*ppAPIStatusList);
	*ppAPIStatusList = nicNULL;
}

__inline nicAPIStatus NewAPIStatusList(nicAPIStatusList** ppAPIStatusList)
{
	return NewAndClear((nicVoid**)ppAPIStatusList,(nicUInt32)sizeof(nicAPIStatusList));
}

__inline nicAPIStatus NewAPIStatusListEx(nicAPIStatusList **ppAPIStatusList,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppAPIStatusList!=nicNULL);
	ACE_ASSERT(*ppAPIStatusList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppAPIStatusList,sizeof(nicAPIStatusList)));
	nicAPIStatusList* pAPIStatusList = *ppAPIStatusList;
	pAPIStatusList->Count = nCount;
	nRet = NewAndClear((nicVoid**)&(pAPIStatusList->List),sizeof(nicAPIStatus)*nCount);
	if (nicErr(nRet))
	{
		FreeAPIStatusList(ppAPIStatusList);
	}
	return nRet;
}


//=========================================================================
// 
// TagIdList结构体的序列化、反序列化等功能定义
// 测点ID列表
// 
//=========================================================================

__inline void ClearTagIdList(nicTagIdList* pTagIdList)
{
	if (pTagIdList==nicNULL)
	{
		return ;
	}

	ACE_OS::free(pTagIdList->TagIdList);


	ACE_OS::memset(pTagIdList,0,sizeof(nicTagIdList));
}
//ce
__inline void FreeTagIdList(nicTagIdList** ppTagIdList)
{
	if (ppTagIdList==nicNULL)
	{
		return;
	}
	if (*ppTagIdList==nicNULL)
	{
		return;
	}
	ClearTagIdList(*ppTagIdList);
	ACE_OS::free(*ppTagIdList);
	*ppTagIdList = nicNULL;
}
//ce
__inline nicAPIStatus NewTagIdList(nicTagIdList** ppTagIdList)
{
	return NewAndClear((nicVoid**)ppTagIdList,(nicUInt32)sizeof(nicTagIdList));
}
//ce
__inline nicAPIStatus NewTagIdListEx(nicTagIdList **ppTagIdList,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppTagIdList!=nicNULL);
	ACE_ASSERT(*ppTagIdList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppTagIdList,sizeof(nicTagIdList)));
	nicTagIdList* pTagIdList = *ppTagIdList;
	pTagIdList->TagCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pTagIdList->TagIdList),sizeof(nicUInt32)*nCount);
	if (nicErr(nRet))
	{
		FreeTagIdList(ppTagIdList);
	}
	return nRet;
}



//=========================================================================
// 
// PropIdList结构体的序列化、反序列化等功能定义
// 属性ID列表
// 
//=========================================================================

__inline void ClearPropIdList(nicPropIdList* pPropIdList)
{
	if (pPropIdList==nicNULL)
	{
		return ;
	}

	ACE_OS::free(pPropIdList->PropIdList);


	ACE_OS::memset(pPropIdList,0,sizeof(nicPropIdList));
}

__inline void FreePropIdList(nicPropIdList** ppPropIdList)
{
	if (ppPropIdList==nicNULL)
	{
		return;
	}
	if (*ppPropIdList==nicNULL)
	{
		return;
	}
	ClearPropIdList(*ppPropIdList);
	ACE_OS::free(*ppPropIdList);
	*ppPropIdList = nicNULL;
}

__inline nicAPIStatus NewPropIdList(nicPropIdList** ppPropIdList)
{
	return NewAndClear((nicVoid**)ppPropIdList,(nicUInt32)sizeof(nicPropIdList));
}

__inline nicAPIStatus NewPropIdListEx(nicPropIdList **ppPropIdList,nicUInt16 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppPropIdList!=nicNULL);
	ACE_ASSERT(*ppPropIdList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppPropIdList,sizeof(nicPropIdList)));
	nicPropIdList* pPropIdList = *ppPropIdList;
	pPropIdList->PropCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pPropIdList->PropIdList),sizeof(nicUInt16)*nCount);
	if (nicErr(nRet))
	{
		FreePropIdList(ppPropIdList);
	}
	return nRet;
}

//=========================================================================
// 
// ValueList结构体的序列化、反序列化等功能定义
// 值列表
// 
//=========================================================================

__inline void ClearValueList(nicValueList* pValueList)
{
	if (pValueList==nicNULL)
	{
		return ;
	}

	nicUInt32 i;
	nicVariant* p;
	if (pValueList->ValueCount>0)
	{
		for (i=0;i<pValueList->ValueCount;++i)
		{
			p = pValueList->ValueList+i;
			ClearVariant(p);
		}
		ACE_OS::free(pValueList->ValueList);
	}

	ACE_OS::memset(pValueList,0,sizeof(nicValueList));
}

__inline void FreeValueList(nicValueList** ppValueList)
{
	if (ppValueList==nicNULL)
	{
		return;
	}
	if (*ppValueList==nicNULL)
	{
		return;
	}
	ClearValueList(*ppValueList);
	ACE_OS::free(*ppValueList);
	*ppValueList = nicNULL;
}

__inline nicAPIStatus NewValueList(nicValueList** ppValueList)
{
	return NewAndClear((nicVoid**)ppValueList,(nicUInt32)sizeof(nicValueList));
}

__inline nicAPIStatus NewValueListEx(nicValueList **ppValueList,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppValueList!=nicNULL);
	ACE_ASSERT(*ppValueList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppValueList,sizeof(nicValueList)));
	nicValueList* pValueList = *ppValueList;
	pValueList->ValueCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pValueList->ValueList),sizeof(nicVariant)*nCount);
	if(nicErr(nRet))
	{
		FreeValueList(ppValueList);
	}
	return nRet;
}

//=========================================================================
// 
// Data结构体的序列化、反序列化等功能定义
// 数据记录定义
// 
//=========================================================================

__inline void ClearData(nicData* pData)
{
	if (pData==nicNULL)
	{
		return ;
	}

	ClearVariant(&(pData->Value));

	ACE_OS::memset(pData,0,sizeof(nicData));
}

__inline void FreeData(nicData** ppData)
{
	if (ppData==nicNULL)
	{
		return;
	}
	if (*ppData==nicNULL)
	{
		return;
	}
	ClearData(*ppData);
	ACE_OS::free(*ppData);
	*ppData = nicNULL;
}

__inline nicAPIStatus NewData(nicData** ppData)
{
	return NewAndClear((nicVoid**)ppData,(nicUInt32)sizeof(nicData));
}



//=========================================================================
// 
// DataList结构体的序列化、反序列化等功能定义
// 数据记录列表
// 
//=========================================================================

__inline void ClearDataList(nicDataList* pDataList)
{
	if (pDataList==nicNULL)
	{
		return ;
	}

	nicUInt32 i;
	nicData* p;
	if (pDataList->DataCount>0)
	{
		for (i=0;i<pDataList->DataCount;++i)
		{
			p = pDataList->DataList+i;
			ClearData(p);
		}
		ACE_OS::free(pDataList->DataList);
	}

	ACE_OS::memset(pDataList,0,sizeof(nicDataList));
}
//free字符串有问题
__inline void FreeDataList(nicDataList** ppDataList)
{
	if (ppDataList==nicNULL)
	{
		return;
	}
	if (*ppDataList==nicNULL)
	{
		return;
	}
	ClearDataList(*ppDataList);
	ACE_OS::free(*ppDataList);
	*ppDataList = nicNULL;
}

__inline nicAPIStatus NewDataList(nicDataList** ppDataList)
{
	return NewAndClear((nicVoid**)ppDataList,(nicUInt32)sizeof(nicDataList));
}

__inline nicAPIStatus NewDataListEx(nicDataList **ppDataList,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppDataList!=nicNULL);
	ACE_ASSERT(*ppDataList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppDataList,sizeof(nicDataList)));
	nicDataList* pDataList = *ppDataList;
	pDataList->DataCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pDataList->DataList),sizeof(nicData)*nCount);
	if(nicErr(nRet))
	{
		FreeDataList(ppDataList);
	}
	return nRet;
}



//=========================================================================
// 
// TagRealData结构体的序列化、反序列化等功能定义
// 测点实时值
// 
//=========================================================================

__inline void ClearTagRealData(nicRealData* pTagRealData)
{
	if (pTagRealData==nicNULL)
	{
		return ;
	}

	ClearData(&(pTagRealData->RealData));

	ACE_OS::memset(pTagRealData,0,sizeof(nicRealData));
}

__inline void FreeTagRealData(nicRealData** ppTagRealData)
{
	if (ppTagRealData==nicNULL)
	{
		return;
	}
	if (*ppTagRealData==nicNULL)
	{
		return;
	}
	ClearTagRealData(*ppTagRealData);
	ACE_OS::free(*ppTagRealData);
	*ppTagRealData = nicNULL;
}

__inline nicAPIStatus NewTagRealData(nicRealData** ppTagRealData)
{
	return NewAndClear((nicVoid**)ppTagRealData,(nicUInt32)sizeof(nicRealData));
}



//=========================================================================
// 
// TagListRealData结构体的序列化、反序列化等功能定义
// 多个测点值列表
// 
//=========================================================================

__inline void ClearRealDataList(nicRealDataList* pTagListRealData)
{
	if (pTagListRealData==nicNULL)
	{
		return ;
	}

	nicUInt32 i;
	nicRealData* p;
	if (pTagListRealData->TagCount>0)
	{
		for (i=0;i<pTagListRealData->TagCount;++i)
		{
			p = pTagListRealData->TagListRealData+i;
			ClearTagRealData(p);
		}
		ACE_OS::free(pTagListRealData->TagListRealData);
	}

	ACE_OS::memset(pTagListRealData,0,sizeof(nicRealDataList));
}

__inline void FreeRealDataList(nicRealDataList** ppTagListRealData)
{
	if (ppTagListRealData==nicNULL)
	{
		return;
	}
	if (*ppTagListRealData==nicNULL)
	{
		return;
	}
	ClearRealDataList(*ppTagListRealData);
	ACE_OS::free(*ppTagListRealData);
	*ppTagListRealData = nicNULL;
}

__inline nicAPIStatus NewRealDataList(nicRealDataList** ppTagListRealData)
{
	return NewAndClear((nicVoid**)ppTagListRealData,(nicUInt32)sizeof(nicRealDataList));
}

__inline nicAPIStatus NewRealDataListEx(nicRealDataList **ppTagListRealData,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppTagListRealData!=nicNULL);
	ACE_ASSERT(*ppTagListRealData==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppTagListRealData,sizeof(nicRealDataList)));
	nicRealDataList* pTagListRealData = *ppTagListRealData;
	pTagListRealData->TagCount = nCount;
	nRet = NewAndClear((nicVoid**)&(pTagListRealData->TagListRealData),sizeof(nicRealData)*nCount);
	if(nicErr(nRet))
	{
		FreeRealDataList(ppTagListRealData);
	}

	return nRet;
}


//=========================================================================
// 
// ReconList结构体的序列化、反序列化等功能定义
// 测点ID列表
// 
//=========================================================================

__inline void ClearReconList(nicRecnoList * pReconList)
{
	if (pReconList==nicNULL)
	{
		return ;
	}

	ACE_OS::free(pReconList->RecnoList);


	ACE_OS::memset(pReconList,0,sizeof(nicRecnoList));
}
//ce
__inline void FreeReconList(nicRecnoList** ppReconList)
{
	if (ppReconList==nicNULL)
	{
		return;
	}
	if (*ppReconList==nicNULL)
	{
		return;
	}
	ClearReconList(*ppReconList);
	ACE_OS::free(*ppReconList);
	*ppReconList = nicNULL;
}
//ce
__inline nicAPIStatus NewReconList(nicRecnoList** ppReconList)
{
	return NewAndClear((nicVoid**)ppReconList,(nicUInt32)sizeof(nicRecnoList));
}
//ce
__inline nicAPIStatus NewReconListEx(nicRecnoList **ppReconList,nicUInt32 nCount)
{
	nicAPIStatus nRet = nicRET_OK;

	ACE_ASSERT(ppReconList!=nicNULL);
	ACE_ASSERT(*ppReconList==nicNULL);

	RET_ERR(NewAndClear((nicVoid**)ppReconList,sizeof(nicRecnoList)));
	nicRecnoList* pReconList = *ppReconList;
	pReconList-> RecnoCount= nCount;
	nRet = NewAndClear((nicVoid**)&(pReconList->RecnoList),sizeof(nicUInt32)*nCount);
	if (nicErr(nRet))
	{
		FreeReconList(ppReconList);
	}
	return nRet;
}


#endif
