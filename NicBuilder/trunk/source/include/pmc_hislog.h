/*
	data structures and types shared by archive subsystem and
	its clients.

	created
	-jhunter@tsinghua.org.cn, 2004/8/9
*/
/*
改动历史
Date       Version    Modified by   Modify
2012-7-9   1.0.0.4    Zander Sun    1.历史数据报包头结构hisqry_pkt_t的修改
*/
#ifndef __pmc_hislog_h__
#define __pmc_hislog_h__

#include <librtdb.h>

#define HISLOG_SERVICE_NAME "hisd"
#define HISLOG_SERVICE_COMMENT "PMC Archive Subsystem"

/*
	error numbers
*/
#define HISLOG_MODULE_NUM 'HS'

#define HISLOG_ERROR(code) PMC_ERROR(HISLOG_MODULE_NUM, (code))

#define HISLOG_TIME_OUT_OF_RANGE \
	HISLOG_ERROR(-1)
	
#define HISLOG_INVALID_ARCHIVE \
	HISLOG_ERROR(-2)

/*
	define packet type constants used on the hislog vbus channel
*/
#define PT_QueryArchivedData	0x00000001

/*
	hislog_key_t is the internal representation of
	time in hislog.
*/
typedef __i64 hislog_key_t; //跟RTK_TIME相同

typedef struct pmc_value_t hislog_item_t;

#include <pshpack1.h>

#define PT_QueryMode_OneData	0x00010000   //标签在某一时刻的历史值查询方式
#define PT_QueryMode_BYCOUNT	(0x00010000<<1)   //标签在某一时段根据得到指定个数的历史值查询方式
#define PT_QueryMode_BYINTERVAL	(0x00010000<<2)   //标签在某一时段根据得到间隔时间的历史值查询方式
#define PT_QueryMode_BYSUMMARY	(0x00010000<<3)   //标签在某一时段二次统计计算的历史值查询方式


#define PT_SUMMARY_MIN		(0x00010000>>1)   //统计计算的最小值
#define PT_SUMMARY_MAX		(0x00010000>>2)   //统计计算的最大值
#define PT_SUMMARY_AVG		(0x00010000>>3)   //统计计算的平均值
#define PT_SUMMARY_STDEV	(0x00010000>>4)   //统计计算的便准方差
#define PT_SUMMARY_TOTAL	(0x00010000>>5)   //统计计算的累计值

//2012-7-8  old hisd
//2012-7-9  增加成员--查询模式m_querymode
struct hisqry_pkt_t{
	RTK_GUID	m_gid;
	hislog_key_t	m_time;
	int 	m_step;
	int 	m_count;
	int 	m_tagcount;
	int     m_queryMode;
	// TAG_NAME m_names[0];
}PACKED;

//2012-7-8   new hisd
// struct hisqry_pkt_t{
// 	RTK_GUID	m_gid;
// 	int			m_qureymode;
// 	hislog_key_t	m_time;
// 	int m_step;
// 	int 	m_count;
// 	int 	m_tagcount;
// 	// TAG_NAME m_names[0];
// }PACKED;

struct hisack_pkt_t{
	RTK_GUID	m_gid;
	int			m_qureymode;
	TAG_NAME	m_tagName;
	__uint		m_count;
	// int m_ret;	//return 0 mean ok, it's seem we should never return error
	// int m_id;		//0 to MAX_QUERY_TAG-1
	// int m_count;
	// struct pmc_value_t value[0];
};

#define HISLOG_MAX_QUERY_COUNT 		600
#define HISLOG_MAX_QUERY_TAG_COUNT 	15

#include <poppack.h>

#endif
