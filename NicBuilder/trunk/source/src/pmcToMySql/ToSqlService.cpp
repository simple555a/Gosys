
#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>
#include "ToSqlService.h"
#include "LimitSingleInstance.h"
#include "Pmc2MysqlThread.h"

static CLimitSingleInstance g_SingleInstanceObj(TEXT("PmcToMySQL")); 

CPmcToSqlService::CPmcToSqlService():CNTService(TEXT("PmcToMySQL"), TEXT("PmcToMySQL service"))
{
}

CPmcToSqlService::~CPmcToSqlService()
{
}

void CPmcToSqlService::Run(DWORD, LPTSTR *)
{
	ReportStatus(SERVICE_START_PENDING);

	if ( g_SingleInstanceObj.IsAnotherInstanceRunning() ){
		printf("Another krt_filemaintenance instance is already running.\n");
		return;
	}

	__init();

	ReportStatus(SERVICE_RUNNING);

	hStop = ::CreateEvent(0, TRUE, FALSE, 0);
	WaitForSingleObject(hStop, INFINITE);

	if (hStop)
	{
		::CloseHandle(hStop);
	}
}

void CPmcToSqlService::Stop()
{
	ReportStatus(SERVICE_STOP_PENDING, 11000);

	if(hStop) 
		::SetEvent(hStop);
}

bool CPmcToSqlService::__init()
{
	//CKrtBackupThread::init();
	//CKrtStateMonitorThread::init();
	CPmc2MysqlThread::init();

	return __true;
}

bool CPmcToSqlService::__uninit()
{
	//CKrtBackupThread::uninit();
	//CKrtStateMonitorThread::uninit();
	CPmc2MysqlThread::uninit();
	return __true;
}
