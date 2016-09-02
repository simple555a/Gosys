
#ifndef _NICSYS_SERIALIZEDATA_H_
#define _NICSYS_SERIALIZEDATA_H_

#include <memory.h>
#include "nicDefs.h"
#include "nicRefACE.h"

///
/// <summary> 
///  serializeData ：工程序列化模块
/// </summary> 
///
/// <remarks>
/// 工程整体序列化模块，负责工程内部所有的底层序列化操作
/// 包括参数序列化，网络数据报头，报尾序列化.
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
		//序列化，反序列化函数
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

		//考虑到头信息和尾信息需要目前已经序列化好的
		//mBlock中的信息，故将数据部分和头信息部分
		//集合到一起做序列化处理，此类负责整个工程中所有的序列化功能
		//将包头序列化到内存块
		//此函数应该在包头设置完毕以后调用

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