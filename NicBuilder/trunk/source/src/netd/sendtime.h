/*
�ļ����ƣ�sendtime.h

������ 
Date       Version   Author       Content
2012-7-13  1.0.0.4   Zander Sun   ����ϵͳʱ���ʵʱ��


�Ķ���¼
Date       Version   Modified By   Modify
2012-7-16  1.0.0.5   Zander Sun   1. ��ϵͳʱ���ǩ���ɺ궨��ָ��
2012-7-17  1.0.0.6   Zander Sun   1. ��ϵͳʱ���ǩ�����β�ָ�������Ӷ�Ӧ�ķ���ϵͳʱ�亯��--sendTime2pmcdEx()
                                  2. ����ϵͳʱ���ǩ���Ƿ�����õĺ궨��--STIME_TAGNAME_CONFIG_OK������0
								  3. ϵͳʱ���ǩ��ָ��ΪEWS2.Epp.SYSTEMTIME
2012-7-17  1.0.0.7   Zander Sun   1. STIME_TAGNAME_CONFIG_OK������1
*/
#ifndef _SENDTIME_H_
#define _SENDTIME_H_

#include "pmc.h"

#define SYSTEM_TIME_TAGNAME "SYSTEMTIME"  //��λ��ϵͳʱ��ı�ǩ����2012-7-16 2012-11-26
#define STIME_TAGNAME_CONFIG_OK 1 //ϵͳʱ���ǩ���Ƿ�����ã��ǣ���ͨ��*.ini����,���β�ָ��;���򲻿����ã��ɺ궨��ָ����2012-7-17

/*
���ܣ�������λ��ϵͳʱ���ʵʱ���ݿ⣬ϵͳʱ���ǩ��Ϊ�궨��ָ��
*/
void sendTime2pmcd();
/*
2012-7-17
���ܣ� ������λ��ϵͳʱ���ʵʱ���ݿ⣬ϵͳʱ���ǩ��Ϊ�β�ָ��
������ 
        tn_char    ϵͳʱ��ı�ǩ��
*/
void sendTime2pmcdEx(char* tn_char);

#endif