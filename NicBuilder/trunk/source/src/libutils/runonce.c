 #include "precomp.h"
#pragma hdrstop 

#ifdef _WIN32
static HANDLE g_hMutex;

/*********************************************************
	����Ƿ��Ѿ���һ�������Ѿ������ˡ�
	����Ѿ�����һ��������ĸ����������У�����1��
	���û�У�����0
	��������ж������������أ�1
*********************************************************/
LIBUTILS_API int UTILS_API check_running_instanceA(const char * szProgramName)
{	
	if(NULL != g_hMutex){
		MessageBox(0,"OoPs, how come there is already a call to check_runing instance, r u crazy?",
			"libutils",MB_OK+MB_ICONEXCLAMATION);
		ExitProcess(0);
	}

	g_hMutex=CreateMutex(0,TRUE,szProgramName);
	if(!g_hMutex){
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError()){
		CloseHandle(g_hMutex);
		g_hMutex = 0;
		return 1;
	}
	return 0;
}

/*********************************************************
	����Ƿ��Ѿ���һ�������Ѿ������ˡ�
	����Ѿ�����һ��ָ���ĳ�����������У�����1��
	���û�У�����0
	��������ж������������أ�1
*********************************************************/
LIBUTILS_API int UTILS_API check_runningA(const char * szProgramName)
{	
	HANDLE hMutex;
	hMutex=CreateMutex(0,TRUE,szProgramName);
	if(!hMutex)
	{
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		return 1;
	}
	CloseHandle(hMutex);
	return 0;
}

/*********************************************************
	�趨���б�־
*********************************************************/
LIBUTILS_API int UTILS_API set_runningA(const char * szProgramName)
{	
	HANDLE hMutex;
	hMutex=CreateMutex(0,TRUE,szProgramName);
	if(!hMutex)
	{
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		return 1;
	}
	return 0;
}

LIBUTILS_API int UTILS_API check_running_instanceW(const wchar_t * szProgramName)
{	
	if(NULL != g_hMutex){
		MessageBox(0,"OoPs, how come there is already a call to check_runing instance, r u crazy?",
			"libutils",MB_OK+MB_ICONEXCLAMATION);
		ExitProcess(0);
	}

	g_hMutex=CreateMutexW(0,TRUE,szProgramName);
	if(!g_hMutex){
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError()){
		CloseHandle(g_hMutex);
		g_hMutex = 0;
		return 1;
	}
	return 0;
}

/*********************************************************
����Ƿ��Ѿ���һ�������Ѿ������ˡ�
����Ѿ�����һ��ָ���ĳ�����������У�����1��
���û�У�����0
��������ж������������أ�1
*********************************************************/
LIBUTILS_API int UTILS_API check_runningW(const wchar_t * szProgramName)
{	
	HANDLE hMutex;
	hMutex=CreateMutexW(0,TRUE,szProgramName);
	if(!hMutex)
	{
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		return 1;
	}
	CloseHandle(hMutex);
	return 0;
}

/*********************************************************
�趨���б�־
*********************************************************/
LIBUTILS_API int UTILS_API set_runningW(const wchar_t * szProgramName)
{	
	HANDLE hMutex;
	hMutex=CreateMutexW(0,TRUE,szProgramName);
	if(!hMutex)
	{
		return -1;
	}
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		return 1;
	}
	return 0;
}
#endif
