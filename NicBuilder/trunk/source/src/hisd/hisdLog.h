/*
文件说明：日志记录
创建：
    Date        version   Author        Content
    2012-8-15   1.0.0.5   Zander Sun    日志记录头文件,用于测试-s命令下的net.exe运行
改动记录
    Date        version   Modified by   Modify
	
*/
#ifndef _HISDLOG_H_
#define _HISDLOG_H_

#include "hisdComm.h"
#define HAVE_PROFILE_SUPPORT
extern __bool logIsEnabled;
/*
功能：
*/
std::string _texttime();
/*
功能：
*/
static std::string getLogFileName();
/*
功能：
*/
static void commit_Log();
/*
功能：
*/
static void emitLog(MSG_LEVEL level, char *s);
/*
功能：
*/
static void emitTimestamp(MSG_LEVEL level);
/*
功能：
*/
static void timedStampedLog(MSG_LEVEL level, char *s);
/*
功能：
*/
static int PMC_API utils_puts_with_log(MSG_LEVEL level,char *s);
/*
功能：
*/
void close_log();
/*
功能：
*/
void open_log();

#endif