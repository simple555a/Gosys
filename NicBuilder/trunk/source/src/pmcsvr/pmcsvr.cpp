// pmcsvr.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f pmcsvrps.mk in the project directory.

#include "stdafx.h"
#pragma hdrstop
#include "resource.h"
#include <initguid.h>
#include "pmcsvr.h"

#include "pmcsvr_i.c"
#include "pmcdb_idl_i.c"

#include "opcserver.h"
#include "pmcdb.h"
#include "hisproxy.h"

#pragma comment(lib,"librtkproxy.lib")
#pragma comment(lib,"librtk.lib")
#pragma comment(lib,"librtdb.lib")
#pragma comment(lib,"libutils.lib")

const DWORD dwTimeOut = 1000*600; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

static void _setupProxy()
{
	char path[_MAX_PATH];
	GetModuleFileName(_Module.m_hInst, path, sizeof(path));
	extract_file_path(path);
	set_working_dir(path);
	init_proxy(
		0, 
		PROXY_SILENT | PROXY_ENABLE_BGLOADER | PROXY_ENABLE_DISPATCHER
		);
	initHisProxy();
	extern void hookEvents();
	hookEvents();
}

static void _closeProxy()
{
	uninitHisProxy();
	uninit_proxy();
}

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
	CExeModule* p = (CExeModule*)pv;
	p->MonitorShutdown();
	return 0;
}

LONG CExeModule::Unlock()
{
	LONG l = CComModule::Unlock();
	if (l == 0)
	{
		bActivity = true;
		SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
	}
	return l;
}

//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
	while (1)
	{
		WaitForSingleObject(hEventShutdown, INFINITE);
		DWORD dwWait=0;
		do
		{
			bActivity = false;
			dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
		} while (dwWait == WAIT_OBJECT_0);
		// timed out
		if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
		{
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
			CoSuspendClassObjects();
			if (!bActivity && m_nLockCnt == 0)
#endif
				break;
		}
	}
	CloseHandle(hEventShutdown);
	PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
	hEventShutdown = CreateEvent(NULL, false, false, NULL);
	if (hEventShutdown == NULL)
		return false;
	DWORD dwThreadID;
	HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
	return (h != NULL);
}

CExeModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_OpcServer, COpcServer)
OBJECT_ENTRY(CLSID_PmcDB, CPmcDB)
END_OBJECT_MAP()

LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (p1 != NULL && *p1 != NULL)
	{
		LPCTSTR p = p2;
		while (p != NULL && *p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1 = CharNext(p1);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, 
	HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
	lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_ASSERTE(SUCCEEDED(hRes));
	_Module.Init(ObjectMap, hInstance, &LIBID_PMCSVRLIB);
	_Module.dwThreadID = GetCurrentThreadId();
	TCHAR szTokens[] = _T("-/");

	int nRet = 0;
	BOOL bRun = TRUE;
	LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_PMCSVR, FALSE);
			nRet = _Module.UnregisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_PMCSVR, TRUE);
			nRet = _Module.RegisterServer(TRUE);
			bRun = FALSE;
			break;
		}
		lpszToken = FindOneOf(lpszToken, szTokens);
	}

	if (bRun)
	{
		_setupProxy();
		_Module.StartMonitor();
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
		hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
		_ASSERTE(SUCCEEDED(hRes));
		hRes = CoResumeClassObjects();
#else
		hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE);
#endif
		_ASSERTE(SUCCEEDED(hRes));

		MSG msg;
		while (GetMessage(&msg, 0, 0, 0))
			DispatchMessage(&msg);

		_Module.RevokeClassObjects();
		Sleep(dwPause); //wait for any threads to finish
		_closeProxy();
	}

	_Module.Term();
	CoUninitialize();
	return nRet;
}
