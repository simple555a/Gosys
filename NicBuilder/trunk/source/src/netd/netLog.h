/*
�ļ�˵������־��¼
������
    Date        version   Author        Content
    2012-7-17   1.0.0.7   Zander Sun    ��־��¼ͷ�ļ�,���ڲ���-s�����µ�net.exe����
�Ķ���¼
    Date        version   Modified by   Modify
	
*/
#ifndef _NETLOG_H_
#define _NETLOG_H_

#include "netComm.h"
#define HAVE_PROFILE_SUPPORT
extern __bool logIsEnabled;
/*
���ܣ�
*/
std::string _texttime();
/*
���ܣ�
*/
static std::string getLogFileName();
/*
���ܣ�
*/
static void commit_Log();
/*
���ܣ�
*/
static void emitLog(MSG_LEVEL level, char *s);
/*
���ܣ�
*/
static void emitTimestamp(MSG_LEVEL level);
/*
���ܣ�
*/
static void timedStampedLog(MSG_LEVEL level, char *s);
/*
���ܣ�
*/
static int PMC_API utils_puts_with_log(MSG_LEVEL level,char *s);
/*
���ܣ�
*/
void close_log();
/*
���ܣ�
*/
void open_log();

#endif