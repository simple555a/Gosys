/*
文件名称：sendtime.cpp

创建： 
Date       Version   Author       Content
2012-7-13  1.0.0.4   Zander Sun   发送系统时间给实时库

改动记录
Date       Version   Modified By   Modify
2012-7-16  1.0.0.5   Zander Sun   将系统时间标签名改为宏定义指定
2012-7-17  1.0.0.6   Zander Sun   增加发送系统时间给实时库的函数，其中标签名由形参给定--sendTime2pmcdEx()
2012-11-26 1.0.0.7   Zander Sun   系统时间的标签名改为由当前实时库节点、组名、宏定义一起指定，增强通用型，不只限于EPP
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

	// 2012-11-26 取本地节点和组名，和SYSTEM_TIME_TAGNAME一起，构造三段式标签名
	open_node(&tn.node);
	open_group_f(&tn.node, &tn.sname.group);
	memcpy(tn.sname.tag.Data, tn_str.c_str(), tn_str.length()+1);
	tn.sname.tag.Data[tn_str.length()+1] = '\0';
	
	//取系统时间，生成系统时间标签
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	rt = (RTK_TIME*)&ft;
	pt.Value.date = (__r8)((__i64)(rt->Data)*(1e-7));
	pt.Flags = dt_date;
	tagname_by_text(&tn, tn_str.data());
    
	//发送系统时间标签给实时库
	proxy_write_tags_ex(1, &tn, &pt);
	
	//清空标签内存
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
	
	//取系统时间，生成系统时间标签
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &ft);
	rt = (RTK_TIME*)&ft;
	pt.Value.date = (__r8)((__i64)(rt->Data)*(1e-7));
	pt.Flags = dt_date;
	tagname_by_text(&tn, tn_char);
    
	//发送系统时间标签给实时库
	proxy_write_tags_ex(1, &tn, &pt);
	
	//清空标签内存
	pmc_clear(&pt);	
}
