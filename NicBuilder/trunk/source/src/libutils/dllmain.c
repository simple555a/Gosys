/********************************************************************
	created:	2002/08/26
	created:	26:8:2002   15:14
	filename: 	D:\dfvs2\SRC\VC\PUBLIC\SRC\LIBUTILS\dllmain.c
	file path:	D:\dfvs2\SRC\VC\PUBLIC\SRC\LIBUTILS
	file base:	dllmain
	file ext:	c
	author:		Jackie Hunter, jhunter@tsinghua.org.cn
	
	purpose:	Initialization code for utility project

	History :
	2002/8/26, Jackie, added dwObsfucator variable for exename_by_pid
*********************************************************************/
#include "precomp.h"
#pragma hdrstop

#pragma comment(linker, "/base:0x76800000")

#ifdef _WIN32
#pragma comment(lib, "advapi32.lib")
#endif

#ifdef __GNUC__
/* Note this procedure must be accopanied by at least one global class variable
 * seems to be gcc's bug, __attribute__((constructor)) will faile without 
 * a global class variable
 * so we've defined a foo variable here
 * */
static void init_lib(void) __attribute__((constructor));
#endif

static void init_lib(void)
{
	 fp_debug = stdout;
	 fp_error = stderr;
	 fp_trace = stdout;
	 cpu_calibrate();		
}

#ifdef _WIN32
BOOL	g_bNeedConsole;
HMODULE	g_hModule;

BOOL WINAPI DllMain(
	 HINSTANCE hinstDLL,  // handle to the DLL module
	 DWORD fdwReason,     // reason for calling function
	 LPVOID lpvReserved   // reserved
	 )
{
	 TCHAR * cmd;
	 switch(fdwReason)
	 {
	 case DLL_PROCESS_ATTACH:
		 
		  init_lib();
		  
		  g_hModule = hinstDLL;
		  cmd=GetCommandLine();
		  if(strstr(cmd," --withconsole") || strstr(cmd, " --debug") ){
			   setup_console_for_log();
			   g_bNeedConsole=TRUE;
		  }
		  
		  break;
	 }
	 return 1;
}
#endif
