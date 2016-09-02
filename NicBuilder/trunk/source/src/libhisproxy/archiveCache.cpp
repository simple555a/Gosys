/*
改动记录
Date      Version  Author      Modify
2012-7-9  1.0.0.4  Zander Sun  1.增加查询模式设置函数setMode()
                               2.数据报头的查询模式赋值--CArchiveCache::query()
*/


#include "precomp.h"
#pragma hdrstop

#ifndef NDEBUG
#include <string>
using namespace std;
#endif

static HVBUS vbus;

string timeAsString(hislog_key_t t)
{
	FILETIME ft;
	SYSTEMTIME stm;
	string str;
	char buf[128];
	
	*((__i64*)&ft) = __i64(t * 1e7);
	FileTimeToSystemTime(&ft, &stm);
	sprintf(
		buf,
		"%04d-%02d-%02d %02d:%02d:%02d",
		stm.wYear,
		stm.wMonth,
		stm.wDay,
		stm.wHour,
		stm.wMinute,
		stm.wSecond
		);
	str = buf;
	return str;
}

class CArchiveTransaction : public CWaitableTransaction
{
public:
	CArchiveTransaction(CArchiveCache * cache)
	{
		m_cache = cache;
		left = cache->tagCount();
	}

	CArchiveCache * m_cache;

	virtual void OnEvent(PCRTK_PACKET p)
	{
		if(p->data_size < sizeof(hisack_pkt_t)){
			return;
		}
		
		if(m_cache && m_cache->m_transaction == this){
			m_cache->onData((hisack_pkt_t*)p->data);
		}
		left--;
		if(!left){
			SetEvent();
		}
	}

	virtual void OnTimeout()
	{
		// detach from the archive cache
		if(m_cache && m_cache->m_transaction == this){
			m_cache->m_transaction = NULL;
		}
	}
private:
	int left;
};

__bool CArchiveCache::setTimeSpan(hislog_key_t t1, __uint step, __uint count)
{
	if(count > HISLOG_MAX_QUERY_COUNT){
		return __false;
	}
	m_startTime = t1;
	m_endTime = t1 + step*count;
	m_valueCount = count;
	m_step = step;
	invalidateCache();
	return __true;
}

void CArchiveCache::scrollTimeSpan(hislog_key_t relativeTime)
{
	m_startTime += relativeTime;
	m_endTime += relativeTime;
	invalidateCache();
}

__bool CArchiveCache::setNames(PCTAG_NAME names, __uint count)
{
	if(count > HISLOG_MAX_QUERY_TAG_COUNT){
		return __false;
	}
	memcpy(m_names, names, sizeof(TAG_NAME)*count);
	m_tagCount = count;
	invalidateCache();
	return __true;
}

//2012-7-9
void CArchiveCache::setMode(__int queryMode)
{
	m_queryMode = queryMode;
}

__bool CArchiveCache::query(__uint timeout)
{
/*	DEBUG_PRINTF((
		"Quering %d tags  %d values, from %s to %s\n",
		m_tagCount,
		m_valueCount,
		timeAsString(m_startTime).c_str(),
		timeAsString(m_endTime).c_str()
		));
*/
	m_queryTime = hislogGetCurrentTime();
		
	// detach from previous transaction
	if(m_transaction){
		m_transaction->m_cache = 0;
		m_transaction = 0;
	}

	invalidateCache();
	
	m_transaction = new CArchiveTransaction(this);
	if(!m_transaction){
		return __false;
	}
	
	// send a query request to archive subsystem
	// and register a transaction callback
	char buf[4096];
	hisqry_pkt_t * p;
	RTK_ADDR addr;
	__uint dataLen ;

	dataLen = m_tagCount*sizeof(TAG_NAME) + sizeof(*p);
	assert(sizeof(buf) > dataLen);
    		
	p = (hisqry_pkt_t*)buf;
	p->m_queryMode = m_queryMode;//2012-7-9
	p->m_count = m_valueCount;
	p->m_gid = m_transaction->Guid;
	p->m_step = m_step;
	p->m_tagcount = m_tagCount;
	p->m_time = m_startTime;
	memcpy(&p[1], m_names, sizeof(TAG_NAME)*m_tagCount);
	
	mk_bcast_addr(&addr.host);
	addr.port = PORT_ALL_SERVER;

	add_transaction(vbus, m_transaction);
	
	send_rtk_data_with_guid(
		vbus,
		&addr,
		&m_transaction->Guid,
		PT_QueryArchivedData | PF_PacketGroup,
		p,
		dataLen
		);
	m_transaction->Wait(timeout);

	return __true;
}

void CArchiveCache::onData(const hisack_pkt_t * d)
{
	__uint  idx;
	__uint count;
	
	if(!m_transaction){
		// assert(0);
		return;
	}
	
	if(!(d->m_gid == m_transaction->Guid)){
		// this is quite possible
		return;
	}

	for(idx = 0; idx < m_tagCount; idx++){
		if(m_names[idx] == d->m_tagName){
			break;
		}
	}
	
	if(idx >= m_tagCount){
		return;
	}

	count = d->m_count > m_valueCount? m_valueCount : d->m_count;
    	
	if(count){
		//memset(m_value[idx], 0, HISLOG_MAX_QUERY_COUNT*sizeof(hislog_item_t));//2012-7-9
		m_flags[idx] = true;
		memcpy(m_value[idx], &d[1], count * sizeof(hislog_item_t));
	}

	memset(
		m_value[idx] + count, 
		0, 
		sizeof(hislog_item_t)*(HISLOG_MAX_QUERY_COUNT - count)
		);

	m_valueCounts[idx] = count;
}

void CArchiveCache::invalidateCache()
{
	memset(m_flags, 0, sizeof(m_flags));
	memset(m_valueCounts, 0, sizeof(m_valueCounts));
}

CArchiveCache::CArchiveCache()
{
	m_transaction = 0;
	m_tagCount = 0;
	m_valueCount = 0;
	invalidateCache();
}

CArchiveCache::~CArchiveCache()
{
	if(m_transaction){
		m_transaction->m_cache = 0;
		m_transaction = 0;
	}
}

void initArchiveCache()
{
	if(!vbus){
		vbus = connect_vbus(BUSID_HISLOG, 0, 0, default_client_filter);
	}
}

void uninitArchiveCache()
{
	if(vbus){
		disconnect_vbus(vbus);
	}
	vbus = 0;
}
