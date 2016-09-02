///任何列表的转换，在调用相关链表前必须进行相应的空间分配
///此类转换都为深拷贝
///如nic类型向tao类型转换时，都调用nicFunction.h中的函数进行空间预分配。
/*------------------------------------------------------------------------
修改记录
1. TaoVariant2nicVariant()加入类型的赋值
2. TaoPropValueList2nicPropValueList()，		
	TaoPropValue2nicPropValue(&(nicVal->PropValueList[i]),(T_nicPropValue *)&taoVal[i]);改成
	TaoPropValue2nicPropValue(&(nicVal->PropValueList[i]),&((*taoVal)[i]));
3. TaoTagIdList2nicTagIdList（），
   nicVal->TagIdList[i] = *(nicUInt32*)&taoVal[i];改成
   nicVal->TagIdList[i] = ((*taoVal)[i]);
4. TaoStriList2nicStrList(),
	NewAndCopyStr(&nicVal->List[i],*(T_nicString *)&taoVal[i]);改成
    NewAndCopyStr(&nicVal->List[i],(*taoVal)[i]);
5. nicTagIdList2TaoTagIdList(),
   taoVal[i] = nicVal->TagIdList[i];改成
   (*taoVal)[i] = nicVal->TagIdList[i];
6. nicStrList2TaoStringList(),
	taoVal[i].allocbuf(strlen(nicVal->List[i]));
	memcpy((void*)&taoVal[i],nicVal->List[i],strlen(nicVal->List[i]));两句删除，改成
	(*taoVal)[i] = nicVal->List[i];
7. nicPropValueList2TaoPropValueList(),
	nicPropValue2TaoPropValue(&nicVal->PropValueList[i],(T_nicPropValue *)&(taoVal[i]));改成
	nicPropValue2TaoPropValue(&nicVal->PropValueList[i],&(*taoVal)[i]);
8. TaoPropIdList2nicPropIdList(),
	nicVal->PropIdList[i] = *(nicUInt16*)&taoVal[i];改成
	nicVal->PropIdList[i] = (*taoVal)[i];
9. nicVariant2TaoVariant(),
	加入数据类型赋值语句taoVal->DataType = (T_nicDataTypeEnum)nicVal->DataType;
-------------------------------------------------------------------------*/
#ifndef _NICSYS_TAOFUNCTION_H__
#define _NICSYS_TAOFUNCTION_H__
#include "nicDefs.h"
#include "nicError.h"
#include "nicRefACE.h"
#include "nicRefTAO.h"
#include "nicFunction.h"

#include "nicIDL/NicSys_RTDBS.h"
#include"nicIDL/NicSys_DBS.h"
#include "nicIDL/NicSys_RTDBC.h"
#include"nicIDL/NicSys_DBC.h"
using namespace NICSYS_RTDB;
using namespace NICSYS_DB;
//Variant
//字符串类型的注意是内存共享的,是深拷贝
//ce
inline nicAPIStatus nicVariant2TaoVariant( nicVariant * nicVal,  T_nicVariant * taoVal)
{

	if (nicVal->DataType >nicDataType_Max || nicVal->DataType < nicDataType_Empty)
	{
		return nicRET_Fail;
	}
	taoVal->DataType = (T_nicDataTypeEnum)nicVal->DataType;

	if (nicVal->DataType == nicDataType_AnsiString ||
		nicVal->DataType == nicDataType_String ||
		nicVal->DataType == nicDataType_Blob 
		)
	{
		char *p = CORBA::string_dup(nicVal->String.Data);
		taoVal->val.Str(p);
	}
	else if (nicVal->DataType == nicDataType_UnicodeString )
	{

		CORBA::WChar * p = CORBA::wstring_dup((CORBA::WChar *)nicVal->UnicodeString.Data);
		taoVal->val.UnicodeString(p);
	}
	else
	{
		taoVal->val.Double64(nicVal->Double);
	}
	return nicRET_OK;
}

//ce
inline nicAPIStatus TaoVariant2nicVariant( nicVariant * nicVal,  const T_nicVariant *taoVal)
{
	if (taoVal->DataType >nicDataType_Max || taoVal->DataType < nicDataType_Empty)
	{
		return nicRET_Fail;
	}
	nicVal->DataType = taoVal->DataType;
	if (taoVal->DataType == T_nicDataType_AnsiString ||
		taoVal->DataType == T_nicDataType_String ||
		taoVal->DataType == T_nicDataType_Blob 
		)
	{
		if (IsNull(taoVal->val.Str()))
		{
			return nicRET_Fail;
		}
		NewAndCopyStr(&nicVal->String.Data,taoVal->val.Str());
		nicVal->String.Length = strlen(taoVal->val.Str());
	}
	else if (taoVal->DataType == T_nicDataType_UnicodeString)
	{
		if (IsNull(taoVal->val.UnicodeString()))
		{
			return nicRET_Fail;
		}
		NewAndCopyUStr(&nicVal->UnicodeString.Data,(nicUChar *)taoVal->val.UnicodeString());
		nicVal->UnicodeString.Length = wcslen(taoVal->val.UnicodeString());
	}
	else
	{
		nicVal->Double = taoVal->val.Double64();
	}
	return nicRET_OK;
}
//PropValue
//ce
inline   nicAPIStatus nicPropValue2TaoPropValue( nicPropValue * nicVal,  T_nicPropValue *taoVal)
{
	taoVal->PropId = nicVal->PropId;
	nicVariant2TaoVariant(&nicVal->Value,&taoVal->Value);
	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoPropValue2nicPropValue(nicPropValue * nicVal, const  T_nicPropValue *taoVal)
{
	nicVal->PropId = taoVal->PropId;
	TaoVariant2nicVariant(&nicVal->Value,&taoVal->Value);
	return nicRET_OK;
}

//nicPropValueList必须已经分配空间了,是个实体
//ce
inline   nicAPIStatus nicPropValueList2TaoPropValueList( nicPropValueList * nicVal,   T_nicPropValueList *taoVal)
{
	nicUInt32 i = 0;
	taoVal->length((CORBA::ULong) nicVal->PropCount);
	for (i = 0; i < taoVal->length();i++)
	{
		nicPropValue2TaoPropValue(&nicVal->PropValueList[i],&(*taoVal)[i]);
	}
	return nicRET_OK;
}
//ce
//暂时没问题，还有待下一步调试
inline   nicAPIStatus TaoPropValueList2nicPropValueList(nicPropValueList * nicVal,  const T_nicPropValueList *taoVal)
{
	nicInt32 i = 0;
	for (i = 0; i < nicVal->PropCount;i++)
	{
		TaoPropValue2nicPropValue(&(nicVal->PropValueList[i]),&((*taoVal)[i]));
	}
	return nicRET_OK;
}
//Data
//ce
inline   nicAPIStatus nicData2TaoData( nicData * nicVal, T_nicData *taoVal)
{
	taoVal->time.Millisec = nicVal->Time.Millisec ;
	taoVal->time.Second = nicVal->Time.Second ;
	taoVal->quality = nicVal->Quality;
	nicVariant2TaoVariant(&nicVal->Value,&taoVal->value);
	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoData2nicData(nicData * nicVal, const T_nicData *taoVal)
{
	nicVal->Time.Millisec = taoVal->time.Millisec ;
	nicVal->Time.Second =  taoVal->time.Second;
	nicVal->Quality = taoVal->quality;
	TaoVariant2nicVariant(&nicVal->Value,&taoVal->value);
	return nicRET_OK;
}
//DataList
//ce
inline   nicAPIStatus nicDataList2TaoDataList( nicDataList * nicVal, T_nicDataList *taoVal)
{

	nicUInt32 i = 0;
	taoVal->length(nicVal->DataCount);
	for (i = 0; i < taoVal->length();i++)
	{
		nicData2TaoData(&nicVal->DataList[i],&(*taoVal)[i]);
	}

	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoDataList2nicDataList(nicDataList * nicVal, const T_nicDataList *taoVal)
{
	nicUInt32 i = 0;
	for (i = 0; i < nicVal->DataCount;i++)
	{
		TaoData2nicData(&nicVal->DataList[i],&(*taoVal)[i]);
	}
	return nicRET_OK;
}
//RealData
//ce
inline   nicAPIStatus nicRealData2TaoRealData( nicRealData * nicVal, T_nicRealData *taoVal)
{
	taoVal->tagId = nicVal->TagId;
	nicData2TaoData(&nicVal->RealData,&taoVal->rData);
	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoRealData2nicRealData(nicRealData * nicVal, T_nicRealData *taoVal)
{
	nicVal->TagId = taoVal->tagId;
	TaoData2nicData(&nicVal->RealData,&taoVal->rData);
	return nicRET_OK;
}
//RealDataList
//ce
inline   nicAPIStatus nicRealDataList2TaoRealDataList( nicRealDataList * nicVal, T_nicRealDataList *taoVal)
{
	nicUInt32 i = 0;
	taoVal->length(nicVal->TagCount);
	for (i = 0; i < taoVal->length();i++)
	{
		//nicRealData2TaoRealData(&nicVal->TagListRealData[i],(T_nicRealData *)&taoVal[i]);
		nicRealData2TaoRealData(&nicVal->TagListRealData[i],&(*taoVal)[i]);
	}
	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoRealDataList2nicRealDataList(nicRealDataList * nicVal, T_nicRealDataList *taoVal)
{
	nicUInt32 i = 0;
	for (i = 0; i < nicVal->TagCount;i++)
	{
		//TaoRealData2nicRealData(&nicVal->TagListRealData[i],(T_nicRealData *)&taoVal[i]);
		TaoRealData2nicRealData(&nicVal->TagListRealData[i],&(*taoVal)[i]);
	}
	return nicRET_OK;
}
//ce
inline   nicAPIStatus TaoTagIdList2nicTagIdList( nicTagIdList * nicVal,  const T_nicTagIdList *taoVal)
{
	nicUInt32 i = 0;
	for (i = 0; i < nicVal->TagCount;i++)
	{
		//nicVal->TagIdList[i] = *(nicUInt32*)&taoVal[i];
		nicVal->TagIdList[i] = ((*taoVal)[i]);
	}
	return nicRET_OK;
}
//ce
inline   nicAPIStatus nicTagIdList2TaoTagIdList(nicTagIdList * nicVal, T_nicTagIdList *taoVal)
{
	nicUInt32 i = 0;
	taoVal->length(nicVal->TagCount);
	for (i = 0; i < taoVal->length();i++)
	{
		/*taoVal[i] = nicVal->TagIdList[i];*/
		(*taoVal)[i] = nicVal->TagIdList[i];
	}
	return nicRET_OK;
}
//nicPropIdList
//ce
inline   nicAPIStatus TaoPropIdList2nicPropIdList(nicPropIdList * nicVal, const T_nicPropIdList *taoVal)
{
	nicUInt32 i = 0;
	for (i = 0; i < nicVal->PropCount;i++)
	{
		 /*nicVal->PropIdList[i] = *(nicUInt16*)&taoVal[i];*/
		nicVal->PropIdList[i] = (*taoVal)[i];
	}

	return nicRET_OK;
}
inline   nicAPIStatus nicPropIdList2TaoPropIdList(nicPropIdList * nicVal, T_nicPropIdList *taoVal)
{
	nicUInt32 i = 0;
	taoVal->length(nicVal->PropCount);
	for (i = 0; i < taoVal->length();i++)
	{
		taoVal[i] = nicVal->PropIdList[i];
	}



}
//字符串nicString转换
inline   nicAPIStatus TaoString2nicString( nicString * nicVal, T_nicString *taoVal)
{
	NewAndCopyStr(&nicVal->Data,*taoVal);
	nicVal->Length = strlen(nicVal->Data);
	return nicRET_OK;
}
inline   nicAPIStatus nicString2TaoString(nicString * nicVal, T_nicString *taoVal)
{
	*taoVal = (T_nicString)malloc(nicVal->Length);
	memcpy(taoVal,nicVal->Data,nicVal->Length);
	return nicRET_OK;
}
//nicStrlist中的单体字符串是不带长度length的
//ce
inline   nicAPIStatus TaoStriList2nicStrList( nicStrList * nicVal, const T_nicStringList *taoVal)
{
	nicUInt32 i = 0;
	for (i = 0; i < nicVal->Count;i++)
	{
		//NewAndCopyStr(&nicVal->List[i],*(T_nicString *)&taoVal[i]);
		NewAndCopyStr(&nicVal->List[i],(*taoVal)[i]);
	}
	return nicRET_OK;
}
//ce
inline   nicAPIStatus nicStrList2TaoStringList(nicStrList * nicVal, T_nicStringList *taoVal)
{
	nicUInt32 i = 0;
	taoVal->length(nicVal->Count);
	
	for (i = 0; i < taoVal->length();i++)
	{
		//taoVal[i].allocbuf(strlen(nicVal->List[i]));
		CORBA::String_var tmp = CORBA::string_dup(nicVal->List[i]);
		(*taoVal)[i] = tmp;
		//ACE_OS::memcpy((void*)&taoVal[i],nicVal->List[i],strlen(nicVal->List[i]));
		//(*taoVal)[i] = nicVal->List[i];
		
	}
	return nicRET_OK;
}
#endif