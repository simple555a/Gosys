#ifndef _NICSYS_TAGNAMEMAP_H__
#define _NICSYS_TAGNAMEMAP_H__
#include "nicPrecomp.h"
#include "nicRefACE.h"
#include "nicDefs.h"
#include "nicError.h"
#include "nicUtils.h"
#include "nicVariant.h"
#include "nicStringFunction.h"
#include "deelx.h"


class CNameMap
{
private:
	Hash_Map < nicChar *, nicUInt32 > hash_;
public:
	CNameMap(nicVoid)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::CNameMap"));

		hash_.open(DEFAULT_NAMEHASH_SIZE);
		this->init();
	}
	CNameMap(nicUInt32 size)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::CNameMap"));

		hash_.open(size);
		this->init();
	}
	~CNameMap(nicVoid)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::~CNameMap"));
		delete_all_ext_id();
		hash_.close();
	}

	nicAPIStatus bind(nicChar *name, nicUInt32 &id)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::bind"));

		//����ռ䣬�������ַ���
		// ������תΪ��д
		nicChar  *pdata;
		pdata=reinterpret_cast<nicChar*>(ACE_OS::malloc( ACE_OS::strlen(name) + sizeof(nicChar)));
		if (pdata == nicNULL)
		{
			return nicERR_COMMON_NO_MEMORY;
		}
		ACE_OS::strcpy(pdata,name);
		pdata = strupr(pdata);
		nicAPIStatus nRet = nicRET_OK;
		switch (hash_.bind(pdata, id))
		{
		case 0:
			break;
		case 1: //���Ѵ��ڵ�����
			ACE_DEBUG((LM_TRACE, ACE_TEXT(" binding an existing entry!!")));
			free(pdata);
			return nicERR_TAG_NAME_REDUPLICATE;
		case  - 1: //�󶨳���
			ACE_DEBUG((LM_TRACE, ACE_TEXT(" failure occurs!!")));
			free(pdata);
			return nicERR_TAG_NAMEHASH_FAILURE;
		default:
			ACE_ASSERT(nicFALSE);
			break;
		}
		return nicRET_OK;
	}
	nicAPIStatus find(nicChar *name, nicUInt32 &id)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::find"));
		// ������תΪ��д
		nicChar  *pdata;
		pdata = reinterpret_cast<nicChar*>(malloc(ACE_OS::strlen(name) + sizeof(nicChar)) );
		ACE_OS::strcpy(pdata,name);
		pdata = strupr(pdata);

		if (hash_.find(pdata, id) ==  - 1)
		{
			free(pdata);
			return  nicERR_TAG_NAME_NOT_EXIST; //û���ҵ�
		}
		free(pdata);
		return nicRET_OK; //
	}	
	nicAPIStatus find(nicChar *name) //����	
	{
		nicUInt32 nTempId;
		return find(name,nTempId);
	}
	nicAPIStatus unbind(nicChar *name, nicUInt32 id=0)
	{
		ACE_TRACE(ACE_TEXT("CNameMap::unbind"));

		// ������תΪ��д
		nicChar  *pdata;
		pdata=reinterpret_cast<nicChar*>( ACE_OS::malloc(ACE_OS::strlen(name) + sizeof(nicChar)));
		ACE_OS::strcpy(pdata,name);
		pdata = strupr(pdata);

		nicAPIStatus nRet=nicRET_OK;

		Hash_Map<nicChar*,nicUInt32>::ENTRY * entry;
		if (hash_.find(pdata, entry) ==  - 1)
		{ 
			//û���ҵ�����
			free(pdata);
			return nicERR_TAG_NAME_NOT_EXIST;

		}
		if (id != 0&&id != entry->int_id_)
		{	
			//���ƶ�Ӧid����
			free(pdata);
			return nicERR_TAG_NAMEHASH_FAILURE;
		}
		nicChar * temnictr = entry->ext_id_;
		if (hash_.unbind(pdata) ==  - 1)
		{	
			//unbindʧ��
			free(pdata);
			return nicERR_TAG_NAMEHASH_FAILURE; 
		}
		free(temnictr);
		free(pdata);
		return nRet; //
	}
	nicAPIStatus init()
	{
		ACE_TRACE(ACE_TEXT("CNameMap::init"));

		return nicRET_OK;
	}
	nicAPIStatus delete_all_ext_id()
	{
		ACE_TRACE(ACE_TEXT("hash map unbind all!!"));

		for (Hash_Map<nicChar*,nicUInt32>::iterator iter = hash_.begin(); iter != hash_.end(); iter++)
		{
			ACE_TRACE(ACE_TEXT("end"));
			free((*iter).ext_id_);
		}
		return nicRET_OK;
	}
};


#endif