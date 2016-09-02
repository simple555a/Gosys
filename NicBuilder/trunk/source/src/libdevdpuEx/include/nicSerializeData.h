
#ifndef _NICSYS_SERIALIZEDATA_H_
#define _NICSYS_SERIALIZEDATA_H_

#include <memory.h>
#include "nicDefs.h"
#include "nicRefACE.h"

///
/// <summary> 
///  serializeData ���������л�ģ��
/// </summary> 
///
/// <remarks>
/// �����������л�ģ�飬���𹤳��ڲ����еĵײ����л�����
/// �����������л����������ݱ�ͷ����β���л�.
///
/// </remarks>
///
class serializeData
{
#define  DEFAULT_MB_SIZE  1024
public:
		serializeData ()
		{
		}

		virtual ~serializeData()
		{
		}

		nicVoid setMB(ACE_Message_Block * mb)
		{
			this->mBlock = mb;
		}
		ACE_Message_Block * getMB()
		{
			return this->mBlock;
		}
		//���л��������л�����
		nicAPIStatus SerializeMemory (void * ppPara, nicUInt32 nSize)
		{
			ACE_ASSERT (ppPara != NULL);
			ACE_ASSERT (nSize > 0);
			while (mBlock->copy ((char*)ppPara, nSize) == -1)
			{
				if (mBlock->size() == 0)
				{
					this->mBlock->size(DEFAULT_MB_SIZE);
				}
				else
				{
					this->mBlock->size(2*mBlock->size());
				}	
			}
			return nicRET_OK;
		}

		nicAPIStatus UnSerializeMemory (void * ppPara, nicUInt32 nSize)
		{
			ACE_OS::memcpy (ppPara, mBlock->rd_ptr (), nSize);
			mBlock->rd_ptr(nSize);
			return nicRET_OK;
		}

		template <typename T>
		nicAPIStatus Serialize (T* value)
		{
			while (mBlock->copy ((char*)value, sizeof (T)) == -1)
			{
				if (mBlock->size() == 0)
				{
					this->mBlock->size(DEFAULT_MB_SIZE);
				}
				else
				{
					this->mBlock->size(2*mBlock->size());
				}	
			}
			return nicRET_OK;
		}

		template <typename T>
		nicAPIStatus UnSerialize (T* value)
		{
			ACE_OS::memset(value, 0, sizeof(T));
			ACE_OS::memcpy (value, mBlock->rd_ptr(), sizeof (T));
			mBlock->rd_ptr (sizeof(T));
			return nicRET_OK;
		}

		//���ǵ�ͷ��Ϣ��β��Ϣ��ҪĿǰ�Ѿ����л��õ�
		//mBlock�е���Ϣ���ʽ����ݲ��ֺ�ͷ��Ϣ����
		//���ϵ�һ�������л��������ฺ���������������е����л�����
		//����ͷ���л����ڴ��
		//�˺���Ӧ���ڰ�ͷ��������Ժ����

	//	nicInt32 SerializeExtend (msgHeader * hdTmp);
private:
		ACE_Message_Block * mBlock;
};

//inline  nicInt32
//serializeData::SerializeExtend(msgHeader * hdTmp)
//{
//	mBlock->copy((char*)&ENDFLAG, sizeof(nicUInt16));
//	hdTmp->flag = BEGFLAG;
//	hdTmp->msgLength = (nicUInt32)mBlock->length ();
//	char * tmp = const_cast<char*>(mBlock->rd_ptr());
//	ACE_OS::memset (tmp, 0, MSG_HEADER_SIZE);
//	ACE_OS::memcpy(tmp, hdTmp, MSG_HEADER_SIZE);
//	nicUInt16 tmp2 = 0;
//	ACE_OS::memcpy(&tmp2, mBlock->wr_ptr() - 2, sizeof(nicUInt16));
//	return nicRET_OK;
//}

#endif