#define WIN32_LEAN_AND_MEAN
#pragma warning (disable : 4530 4786)

#include <windows.h>
#include "detours.h"
#include <map>
#include <string>
#include <utils.h>

using namespace std;

typedef std::map<DWORD,string> FILEMAP;
FILEMAP m;

HANDLE g_ComHandles[16];
static char *prefix = "\\\\.\\com";
static int len = strlen(prefix);

DETOUR_TRAMPOLINE(
	HANDLE WINAPI Real_CreateFile(
	LPCTSTR,
	DWORD,
	DWORD,
	LPSECURITY_ATTRIBUTES,
	DWORD,
	DWORD,
	HANDLE),
	CreateFileA
);

DETOUR_TRAMPOLINE(
	BOOL WINAPI Real_ReadFile(
	HANDLE hFile,                
	LPVOID lpBuffer,             
	DWORD nNumberOfBytesToRead,  
	LPDWORD lpNumberOfBytesRead, 
	LPOVERLAPPED lpOverlapped    
	),ReadFile
);

DETOUR_TRAMPOLINE(
	BOOL WINAPI Real_CloseHandle(HANDLE),  CloseHandle
	);

HANDLE WINAPI My_CreateFile(
	LPCTSTR lpFileName,                         
	DWORD dwDesiredAccess,                      
	DWORD dwShareMode,                          
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	DWORD dwCreationDisposition,                
	DWORD dwFlagsAndAttributes,                 
	HANDLE hTemplateFile                       
	)
{
	HANDLE h;
	h = Real_CreateFile(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile
		);
	utils_debug("%s created as %08x\n", lpFileName, h);
	if(h != INVALID_HANDLE_VALUE){
		m.insert(FILEMAP::value_type((DWORD)h, lpFileName));
	}
	if( !strnicmp(lpFileName, prefix, len) ){
		int comno = atoi(lpFileName + len);
		if(comno > 0 && comno < 16){
			utils_trace("Hooked open com port : %d, handle = %08x\n", comno, h);
			g_ComHandles[comno - 1] = h;
		}
	}
	return h;
}

BOOL WINAPI My_CloseHandle(HANDLE h)
{
	FILEMAP::iterator it;
	it = m.find((DWORD)h);
	if (it != m.end()){
		utils_debug("File %s closed.\n", it->second.data());
		for(int i=0; i<16; i++){
			if(g_ComHandles[i] == h){
				utils_trace("Com handle %d(0x%08x) closed.\n", i+1, h);
				break;
			}
		}
	}
	return Real_CloseHandle(h);
}

BOOL WINAPI My_ReadFile(
	HANDLE hFile,                // handle to file
	LPVOID lpBuffer,             // data buffer
	DWORD nNumberOfBytesToRead,  // number of bytes to read
	LPDWORD lpNumberOfBytesRead, // number of bytes read
	LPOVERLAPPED lpOverlapped    // overlapped buffer
	)
{
	FILEMAP::iterator it;
	it = m.find((DWORD)hFile);
	string fn;
	if(it == m.end()){
		fn = "<Unknown>";
	}else{
		fn = it->second;
	}
	utils_debug(
		"Read from %s, offset %08x, bytes %08x\n", 
		fn.data(), 
		SetFilePointer(hFile, 0, 0, FILE_CURRENT), 
		nNumberOfBytesToRead
		);
	return Real_ReadFile(
		hFile, 
		lpBuffer, 
		nNumberOfBytesToRead, 
		lpNumberOfBytesRead, 
		lpOverlapped
		);
}

void twith()
{
	DetourFunctionWithTrampoline(
		(PBYTE)Real_CreateFile,
		(PBYTE)My_CreateFile
		);
	/*
	DetourFunctionWithTrampoline(
		(PBYTE)Real_ReadFile,
		(PBYTE)My_ReadFile
		);
	*/
	DetourFunctionWithTrampoline(
		(PBYTE)Real_CloseHandle, 
		(PBYTE)My_CloseHandle
		);
}

void twithout()
{
	DetourRemove((PBYTE)Real_CreateFile,(PBYTE)My_CreateFile);
	// DetourRemove((PBYTE)Real_ReadFile, (PBYTE)My_ReadFile);
	DetourRemove((PBYTE)Real_CloseHandle,(PBYTE)My_CloseHandle);
}

/*
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to the DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved   // reserved
	)
{
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		twith();
		break;
	case DLL_PROCESS_DETACH:
		twithout();
		break;
	}
	return TRUE;
}
*/