/*
文件名称：sendtime.h

创建： 
Date       Version   Author       Content
2012-7-13  1.0.0.4   Zander Sun   发送系统时间给实时库


改动记录
Date       Version   Modified By   Modify
2012-7-16  1.0.0.5   Zander Sun   1. 将系统时间标签名由宏定义指定
2012-7-17  1.0.0.6   Zander Sun   1. 将系统时间标签名由形参指定，增加对应的发送系统时间函数--sendTime2pmcdEx()
                                  2. 增加系统时间标签名是否可配置的宏定义--STIME_TAGNAME_CONFIG_OK，并置0
								  3. 系统时间标签名指定为EWS2.Epp.SYSTEMTIME
2012-7-17  1.0.0.7   Zander Sun   1. STIME_TAGNAME_CONFIG_OK，并置1
*/
#ifndef _SENDTIME_H_
#define _SENDTIME_H_

#include "pmc.h"

#define SYSTEM_TIME_TAGNAME "SYSTEMTIME"  //上位机系统时间的标签名。2012-7-16 2012-11-26
#define STIME_TAGNAME_CONFIG_OK 1 //系统时间标签名是否可配置，是：则通过*.ini配置,由形参指定;否：则不可配置，由宏定义指定。2012-7-17

/*
功能：发送上位机系统时间给实时数据库，系统时间标签名为宏定义指定
*/
void sendTime2pmcd();
/*
2012-7-17
功能： 发送上位机系统时间给实时数据库，系统时间标签名为形参指定
参数： 
        tn_char    系统时间的标签名
*/
void sendTime2pmcdEx(char* tn_char);

#endif