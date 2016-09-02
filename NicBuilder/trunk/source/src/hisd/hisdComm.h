/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename: 	E:\vss\pmc2\src\pmcd\precomp.h
	file path:	E:\vss\pmc2\src\pmcd
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose:	Precompile header file for PMCD.EXE	
	history:
	created:	10:10:2002   16:10
*********************************************************************/
/*
创建：
Date        Version   Author       Content
2012-8-15   1.0.0.5   Zander Sun   1.照搬pmcd中的precomp.h，但去掉了不必要的头文件和函数声明，用于日志记录的include
*/
#ifndef _HISDCOMM_H_
#define _HISDCOMM_H_

#define RTK_SERVER

#pragma warning(disable : 4786)

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <signal.h>

#include <locale.h>
#include <string.h>
#include <string>

using namespace std;

#include <pmc.h>
#include <ioss.h>

#include <shell.h>
#include <transaction.h>
//#include <rtk_packet.h>
#include <object.h>
//#include <vbus.h>

#include <powermgr.h>

#endif