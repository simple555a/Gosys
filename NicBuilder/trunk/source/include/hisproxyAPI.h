#ifndef _HISPROXYAPI_H_
#define _HISPROXYAPI_H_

#include "precomp.h"
#include "hisproxy.h"

/***************************************/
/*
���ܣ���ѯһ����ǩ�㡢ָ��ʱ����ڡ�ָ����������ʷ����
*/
LIBHISPROXY_API __u32 PMC_API readHisdataByCount(
			    PCTAG_NAME tag,             //����ѯ��ǩ��ָ�� 
			    hislog_key_t* ptimes,       //ptimes[0]Ϊ��ѯ��ʼʱ�䣬ptimes[1]Ϊ��ѯ����ʱ��
			    long valueCount,            //ָ��ÿ����ǩ�����ѯ���ٸ�����
				hislog_item_t *value       //��ѯ���
			    );


/***************************************/
/*
���ܣ���ѯһ����ǩ�㡢ָ��ʱ����ڡ�ָ����ѯ�������ʷ����
*/
LIBHISPROXY_API __u32 PMC_API readHisdataByInterval(
			    PCTAG_NAME tag,             //����ѯ��ǩ��ָ�� 
			    hislog_key_t* ptimes,       //ptimes[0]Ϊ��ѯ��ʼʱ�䣬ptimes[1]Ϊ��ѯ����ʱ��, ptimes[2]Ϊ��ѯ���
				hislog_item_t *value       //��ѯ���
			    );





#endif



