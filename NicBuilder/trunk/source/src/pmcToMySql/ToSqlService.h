#pragma once
#include <windows.h>
#include "ntservice.h"

class CPmcToSqlService :
	public CNTService
{
public:
	CPmcToSqlService();
	~CPmcToSqlService();

	virtual void Stop();
	virtual void Run(DWORD, LPTSTR * );

private:
	bool __init();
	bool __uninit();
	HANDLE hStop;
};
