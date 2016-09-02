#include "precomp.h"
#pragma hdrstop 

#ifdef _WIN32 
BOOL UTILS_API is_winnt()
{
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&os);
	if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return TRUE;
	}
	return FALSE;
}

/*------------------------------------------------------------------
| Name: running_as_administrator
| Desc: checks if user has administrator privileges
| Notes: This function returns TRUE if the user identifier associated with 
|   this process is a member of the the Administrators group.
------------------------------------------------------------------*/
LIBUTILS_API int UTILS_API running_as_administrator ( VOID)
{
	BOOL   fAdmin;
	HANDLE  hThread;
	TOKEN_GROUPS *ptg = NULL;
	DWORD  cbTokenGroups;
	DWORD  dwGroup;
	PSID   psidAdmin;
	
	SID_IDENTIFIER_AUTHORITY SystemSidAuthority= SECURITY_NT_AUTHORITY;
	
	if(!is_winnt()){
		return TRUE;
	}
	// First we must open a handle to the access token for this thread.
	
	if ( !OpenThreadToken ( GetCurrentThread(), TOKEN_QUERY, FALSE, &hThread))
	{
		if ( GetLastError() == ERROR_NO_TOKEN)
		{
			// If the thread does not have an access token, we'll examine the
			// access token associated with the process.
			
			if (! OpenProcessToken ( GetCurrentProcess(), TOKEN_QUERY, 
				&hThread))
				return ( FALSE);
		}
		else 
			return ( FALSE);
	}
	
	// Then we must query the size of the group information associated with
	// the token. Note that we expect a FALSE result from GetTokenInformation
	// because we've given it a NULL buffer. On exit cbTokenGroups will tell
	// the size of the group information.
	
	if ( GetTokenInformation ( hThread, TokenGroups, NULL, 0, &cbTokenGroups))
		return ( FALSE);
	
	// Here we verify that GetTokenInformation failed for lack of a large
	// enough buffer.
	
	if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return ( FALSE);
	
	// Now we allocate a buffer for the group information.
	// Since _alloca allocates on the stack, we don't have
	// to explicitly deallocate it. That happens automatically
	// when we exit this function.
	
	if ( ! ( ptg= _alloca ( cbTokenGroups))) 
		return ( FALSE);
	
	// Now we ask for the group information again.
	// This may fail if an administrator has added this account
	// to an additional group between our first call to
	// GetTokenInformation and this one.
	
	if ( !GetTokenInformation ( hThread, TokenGroups, ptg, cbTokenGroups,
		&cbTokenGroups) )
		return ( FALSE);
	
	// Now we must create a System Identifier for the Admin group.
	
	if ( ! AllocateAndInitializeSid ( &SystemSidAuthority, 2, 
		SECURITY_BUILTIN_DOMAIN_RID, 
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &psidAdmin) )
		return ( FALSE);
	
	// Finally we'll iterate through the list of groups for this access
	// token looking for a match against the SID we created above.
	
	fAdmin= FALSE;
	
	for ( dwGroup= 0; dwGroup < ptg->GroupCount; dwGroup++)
	{
		if ( EqualSid ( ptg->Groups[dwGroup].Sid, psidAdmin))
		{
			fAdmin = TRUE;
			
			break;
		}
	}
	
	// Before we exit we must explicity deallocate the SID we created.
	
	FreeSid ( psidAdmin);
	
	return ( fAdmin);
}
/* eof - RunningAsAdministrator */

#define NOT_INITIALIZED 10
typedef BOOL (WINAPI * PROC_CREATEPROCESSWITHLOGONW)(
  LPCWSTR lpUsername,                 // user's name
  LPCWSTR lpDomain,                   // user's domain
  LPCWSTR lpPassword,                 // user's password
  DWORD dwLogonFlags,                 // logon option
  LPCWSTR lpApplicationName,          // executable module name
  LPWSTR lpCommandLine,               // command-line string
  DWORD dwCreationFlags,              // creation flags
  LPVOID lpEnvironment,               // new environment block
  LPCWSTR lpCurrentDirectory,         // current directory name
  LPSTARTUPINFOW lpStartupInfo,       // startup information
  LPPROCESS_INFORMATION lpProcessInfo // process information
);
static PROC_CREATEPROCESSWITHLOGONW proc_CreateProcessWithLogonW=
(PROC_CREATEPROCESSWITHLOGONW)NOT_INITIALIZED;

/*
	function run_as()
	this proc simplifies the call to CreateProcessWithLogonW
	it also checks if current OS is NT, if not, the user/pwd/domain 
	parameters are ignored
*/
static int 
run_as_w(
	LPCWSTR user,
	LPCWSTR pwd,
	LPCWSTR domain,
	LPCWSTR lpApplicationName,          // executable module name
	LPWSTR  lpCommandLine,               // command-line string
	DWORD   dwCreationFlags,              // creation flags
	LPVOID  lpEnvironment,               // new environment block
	LPCWSTR lpCurrentDirectory,         // current directory name
	LPSTARTUPINFOW lpStartupInfo,       // startup information
	LPPROCESS_INFORMATION lpProcessInfo // process information
	)
{
	HMODULE hModule;
	if(!is_winnt()){
		utils_errorA("Not running on winnt.\n");
		return 0;
	}

	if( NOT_INITIALIZED ==( DWORD)proc_CreateProcessWithLogonW){
		// this piece of code must be linked against advapi32.lib
		hModule=GetModuleHandle("advapi32.dll");
		if(!hModule){
			return 0;
		}
		proc_CreateProcessWithLogonW=(PROC_CREATEPROCESSWITHLOGONW)
			GetProcAddress(hModule,"CraeteProcessWithLogonW");
	}

	if(!proc_CreateProcessWithLogonW){
		utils_errorA("CreateProcessWithLogonW -- entry not found.\n");
		return 0;
	}
	return proc_CreateProcessWithLogonW(
		user,
		pwd,
		domain,
		0,
		lpApplicationName,
		lpCommandLine,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		lpProcessInfo
	);
}

/*********************************************************************
Create a console for GUI-based application.
So that you can use printf/utils_errorA/... etc to output messages.
*********************************************************************/
LIBUTILS_API int UTILS_API setup_console_for_log() 
{
	static char b_console_created;
	DWORD astds[3]={STD_OUTPUT_HANDLE,STD_ERROR_HANDLE,STD_INPUT_HANDLE};
	FILE *atrgs[3]={stdout,stderr,stdin};
	register int i;
	int osf;
	long hand;
	FILE * fp;

	if(b_console_created)	return 1;

	AllocConsole(); // you only get 1 console.
	// lovely hack to get the standard io (printf, getc, etc) to the new console. Pretty much does what the
	// C lib does for us, but when we want it, and inside of a Window'd app.
	// The ugly look of this is due to the error checking (bad return values. Remove the if xxx checks if you like it that way.
	for(i=0; i<3; i++ ) {
		hand=(long)GetStdHandle(astds[i]);
		if( hand!=(long)INVALID_HANDLE_VALUE ) {
#ifdef _MSC_VER
			osf=_open_osfhandle(hand,_O_TEXT);
#else
			int osf=_open_osfhandle(hand,O_TEXT);
#endif
			if( osf!=-1 ) {
				fp=_fdopen(osf,(astds[i]==STD_INPUT_HANDLE) ? "r" : "w");
				if( fp!=NULL ) {
					*(atrgs[i])=*fp;
					setvbuf(fp,NULL,_IONBF,0);
				}
			}
		}
	}
	b_console_created=TRUE;
	return 1;
}

////////////////////////////////////////////////////////////////////////
// perror, windows version
// 
////////////////////////////////////////////////////////////////////////
LIBUTILS_API int UTILS_API wperrorA(char * error_string)
{
	LPVOID lpMsgBuf;
	int e=GetLastError();
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		e,
		0,
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	utils_errorA("%s:%d,%s\n",error_string,e,(LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return e;
}

LIBUTILS_API int UTILS_API wperrorW(wchar_t * error_string)
{
	LPVOID lpMsgBuf;
	int e=GetLastError();
	FormatMessageW( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		e,
		0,
		(LPWSTR)&lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	utils_errorW(L"%s:%d,%s\n",error_string,e,(LPWSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return e;
}

typedef struct _tag_parameters
{
	void * param;
	DWORD param_type;
}CProxyParameter;

/***********************************************************
function name : delphi_trace
purpose:

This encapsulates the utils_trace function so that delphi program can 
take advantage of it.

The corresponding prototype in delphi is :

function trace(s : pchar;args : array of const):integer;
cdecl;external 'libutils.dll' name 'delphi_trace';

---------------!!!!!! Caution !!!!!!---------------
Don't use any incompatible types in the args array, such as ansistring,
and any object types.
***********************************************************/
LIBUTILS_API int UTILS_API delphi_trace(char * fmt,CProxyParameter * params,int count)
{
	int retval;
	if(!utils_traceA)
	{
		return 0;
	}
	__asm rdtsc
		__asm mov retval,eax
		utils_traceA("<%08x>:", retval);
	_asm{
		mov ecx,count
			inc  ecx
			lea ebx,[ecx*4]
			jecxz skip_parameters
				mov eax,params				
				add eax,ebx
				add eax,ebx
				sub eax,8
push_next:
			push [eax]
			sub eax,8
				loop push_next
skip_parameters:
			push fmt
				call utils_traceA
				add esp,ebx
				add esp,4
				mov retval,eax
	}
	return retval;
}

#endif
