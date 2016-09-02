// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__91E5DF4C_367A_4954_9861_4F8DCCE175EE__INCLUDED_)
#define AFX_STDAFX_H__91E5DF4C_367A_4954_9861_4F8DCCE175EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#define _ATL_APARTMENT_THREADED

#define _ATL_DEBUG_QI
#define _ATL_DEBUG_INTERFACES

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
};
extern CExeModule _Module;

#include <atlcom.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>
#include <assert.h>

#include <pmc.h>

using namespace std;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__91E5DF4C_367A_4954_9861_4F8DCCE175EE__INCLUDED)
