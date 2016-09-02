/*
改动记录：
Date       Version   Author       Modify
2012-7-9   1.0.0.2   Zander Sun   1.CArchiveCache增加查询模式设置的成员变量m_queryMode和成员函数SetMode()
*/
#include <pmc.h>
#include <pmc_hislog.h>
#include "hisproxy.h"
#include "transaction.h"
#include "rtk_packet.h"

/*
	a client side cache
	note this class is directly exported, thus we make a tight-coupling
	between implementation and application, a bit dirty.
*/
class CArchiveCache : public CRTKObject
{
public:
	CArchiveCache();
	virtual ~CArchiveCache();
	
	__bool setTimeSpan(hislog_key_t t1, __uint step, __uint count);
	__bool setNames(PCTAG_NAME names, __uint count);
	__bool query(__uint timeout = 0);
	void scrollTimeSpan(hislog_key_t relativeTime);
	//2012-7-9
	void setMode(__int queryMode);

	PCTAG_NAME names()
	{
		return m_names;
	}
	
	hislog_key_t startTime()
	{
		return m_startTime;
	}
	hislog_key_t endTime()
	{
		return m_endTime;
	}
	__uint step()
	{
		return m_step;
	}

	__uint valueCount()
	{
		return m_valueCount;
	}

	__uint returnedValueCount(int idx)
	{
		return m_valueCounts[idx];
	}
	
	__uint tagCount()
	{
		return m_tagCount;
	}

	bool cacheFlag(__uint i)
	{
		return m_flags[i];
	}

	PCpmc_value_t cacheValue(__uint i, __uint j)
	{
		return &m_value[i][j];
	}

	hislog_key_t queryTime()
	{
		return m_queryTime;
	}
	
protected:
	__uint m_tagCount;
	__uint m_step;
	__uint m_valueCount;
	hislog_key_t m_startTime;
	hislog_key_t m_endTime;
	hislog_key_t m_queryTime;
	TAG_NAME m_names[HISLOG_MAX_QUERY_TAG_COUNT];

	//2012-7-9
	__int m_queryMode;

	/*
		the actual cache and its flags
	*/
	bool m_flags[HISLOG_MAX_QUERY_TAG_COUNT];
	__uint m_valueCounts[HISLOG_MAX_QUERY_TAG_COUNT];
	hislog_item_t m_value[HISLOG_MAX_QUERY_TAG_COUNT][HISLOG_MAX_QUERY_COUNT];

	friend class CArchiveTransaction;
	class CArchiveTransaction * m_transaction;

	void onData(const hisack_pkt_t * d);
	void invalidateCache();
};

void initArchiveCache();
void uninitArchiveCache();


