/*
�ļ����ƣ�sendtime.cpp

������ 
Date       Version   Author       Content
2012-7-13  1.0.0.4   Zander Sun   ����ϵͳʱ���ʵʱ��

�Ķ���¼
Date       Version   Modified By   Modify
2012-7-16  1.0.0.5   Zander Sun   ��ϵͳʱ���ǩ����Ϊ�궨��ָ��
2012-7-17  1.0.0.6   Zander Sun   ���ӷ���ϵͳʱ���ʵʱ��ĺ��������б�ǩ�����βθ���--sendTime2pmcdEx()
2012-11-26 1.0.0.7   Zander Sun   ϵͳʱ��ı�ǩ����Ϊ�ɵ�ǰʵʱ��ڵ㡢�������궨��һ��ָ������ǿͨ���ͣ���ֻ����EPP
*/

#include "sendtime.h"

void sendTime2pmcd()
{
	
	string tn_str = SYSTEM_TIME_TAGNAME;//2012-7-16
	SYSTEMTIME st;
	FILETIME ft;
	TAG_NAME tn;
	pmc_value_t pt;
	RTK_TIME* rt;

	// 2012-11-26 ȡ���ؽڵ����������SYSTEM_TIME_TAGNAMEһ�𣬹�������ʽ��ǩ��
	open_node(&tn.node);
	open_group_f(&tn.node, &tn.sname.group);
	memcpy(tn.sname.tag.Data, tn_str.c_str(), tn_str.length()+1);
	tn.sname.tag.Data[tn_str.length()+1] = '\0';
	
	//ȡϵͳʱ�䣬����ϵͳʱ���ǩ
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	rt = (RTK_TIME*)&ft;
	pt.Value.date = (__r8)((__i64)(rt->Data)*(1e-7));
	pt.Flags = dt_date;
	tagname_by_text(&tn, tn_str.data());
    
	//����ϵͳʱ���ǩ��ʵʱ��
	proxy_write_tags_ex(1, &tn, &pt);
	
	//��ձ�ǩ�ڴ�
	pmc_clear(&pt);	
}

//2012-7-17
void sendTime2pmcdEx(char* tn_char)
{
	SYSTEMTIME st;
	FILETIME ft;
	TAG_NAME tn;
	pmc_value_t pt;
	RTK_TIME* rt;
	
	//ȡϵͳʱ�䣬����ϵͳʱ���ǩ
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	rt = (RTK_TIME*)&ft;
	pt.Value.date = (__r8)((__i64)(rt->Data)*(1e-7));
	pt.Flags = dt_date;
	tagname_by_text(&tn, tn_char);
    
	//����ϵͳʱ���ǩ��ʵʱ��
	proxy_write_tags_ex(1, &tn, &pt);
	
	//��ձ�ǩ�ڴ�
	pmc_clear(&pt);	
}
