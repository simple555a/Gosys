#ifndef _HISPROXYAPI_H_
#define _HISPROXYAPI_H_

#include "precomp.h"
#include "hisproxy.h"

/***************************************/
/*
功能：查询一个标签点、指定时间段内、指定个数的历史数据
*/
LIBHISPROXY_API __u32 PMC_API readHisdataByCount(
			    PCTAG_NAME tag,             //待查询标签名指针 
			    hislog_key_t* ptimes,       //ptimes[0]为查询起始时间，ptimes[1]为查询结束时间
			    long valueCount,            //指定每个标签名需查询多少个数据
				hislog_item_t *value       //查询结果
			    );


/***************************************/
/*
功能：查询一个标签点、指定时间段内、指定查询间隔的历史数据
*/
LIBHISPROXY_API __u32 PMC_API readHisdataByInterval(
			    PCTAG_NAME tag,             //待查询标签名指针 
			    hislog_key_t* ptimes,       //ptimes[0]为查询起始时间，ptimes[1]为查询结束时间, ptimes[2]为查询间隔
				hislog_item_t *value       //查询结果
			    );





#endif



