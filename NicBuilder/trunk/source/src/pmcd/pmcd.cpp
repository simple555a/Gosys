#include "precomp.h"
#pragma hdrstop

#include <crtdbg.h>

char *projDir = NULL;

#ifdef _WIN32
#define _MKDIR(name) mkdir(name)
#else
#define _MKDIR(name) mkdir(name, 0777)
#endif

#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtknet.lib")
#pragma comment(lib, "librtkproxy.lib")
#pragma comment(lib, "libpowermgr.lib")
#pragma comment(lib, "libpmckey.lib")
#pragma comment(lib, "librtkshell.lib")
#pragma comment(lib, "libioss.lib")
#pragma comment(lib, "libntservice.lib")
#pragma comment(lib, "libspac.lib")

static void _load_proj()
{
	string fakedir;

	if(projDir){
		set_working_dir(projDir);
	}

	fakedir = get_working_dir();

	_MKDIR( (fakedir + "/file").data() );
	_MKDIR( (fakedir + "/pic").data() );
	_MKDIR( (fakedir + "/var").data() );
	_MKDIR( (fakedir + "/var/evtd").data() );
	_MKDIR( (fakedir + "/var/hisd").data() );
	_MKDIR( (fakedir + "/infs").data() );
	_MKDIR( (fakedir + "/config").data() );

	if(!g_bAllowBackup && GetPrivateProfileInt(
		"PMC", 
		"AllowBackup", 
		0, 
		get_config_file()
		))
	{
		// if g_bAllowBackup is not set in command line
		// then we look into config file
		g_bAllowBackup = __true;
	}
}

static __bool bExit;

int main(int argc,char **argv)
{
	int c, i;
	char *v[128];

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	logIsEnabled = __true;
	
	c = 1;
	v[0] = argv[0];
	for(i=1; i<argc; i++){
		if(!strnicmp(argv[i], "-p:", 3)){
			projDir = argv[i]+3;
		}else if(!strnicmp(argv[i], "-2", 2)){
			g_bAllowBackup = __true;
		}else if(!strnicmp(argv[i], "-leak:", 6)){
			long leak;
			leak = atoi(argv[i] + 6);
			utils_trace("Tracking memory leak on NO.%d\n", leak);
			_CrtSetBreakAlloc(leak);
		}else if(!strnicmp(argv[i], "-nolog", 6)){
			logIsEnabled = __false;
		}else{
			v[c++] = argv[i];
		}
	}
	
	debug_level=dbg_level_verbose;

	_load_proj();

	setlocale(LC_ALL,"chs");    
	up_time();

	open_log();
	
/*test code */
{
	// utils_trace("_NT_SYMBOL_PATH='%s'\n", getenv("_NT_SYMBOL_PATH"));
}

#ifdef _WIN32
	serv.RegisterService(c, v);
#else
	serv.Run(0, 0);
#endif
	
	close_log();

	return 1;
}

