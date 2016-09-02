#include <windows.h>	// includes basic windows functionality
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <winioctl.h>
#include "utils.h"

char * sys_file;
char * sys_name;
char * action;
char * depend_groups;
char * depend_services;
char * group_name;
char * display_name;
bool b_no_copy;
int service_type;
int	start_mode=SERVICE_DEMAND_START;
bool need_help;
char * service_args;

#pragma comment (lib,"advapi32.lib")
#pragma comment (lib,"user32.lib")



#pragma comment (lib,"libutils.lib")

/****************************************************************************
*
*	 FUNCTION: RemoveDriver( IN SC_HANDLE, IN LPCTSTR)
*
*	 PURPOSE: Deletes the driver service.
*
****************************************************************************/
BOOL RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
	SC_HANDLE  schService;
	BOOL	   ret;
	SERVICE_STATUS status;

	schService = OpenService( SchSCManager,
							  DriverName,
							  SERVICE_ALL_ACCESS
							  );

	if ( schService == NULL )
		return FALSE;

	ControlService(schService,SERVICE_STOP,&status);

	ret = DeleteService( schService );

	CloseServiceHandle( schService );

	return ret;
}

/****************************************************************************
*
*	 FUNCTION: StopDriver( IN SC_HANDLE, IN LPCTSTR)
*
*	 PURPOSE: Has the configuration manager stop the driver (unload it)
*
****************************************************************************/
BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
	SC_HANDLE		schService;
	BOOL			ret;
	SERVICE_STATUS	serviceStatus;

	schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS );
	if ( schService == NULL )
		return FALSE;

	ret = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );

	CloseServiceHandle( schService );

	return ret;
}

/***************************************************************

	FUNCTION : make_dependence_string
	PURPOSE  : format the dependance string according to 
	depend_groups and depend_services

***************************************************************/
static const char * make_dependence_string()
{
	static char dep[2048];
	CArgs args;
	char * p;
	int i;

	p=dep;
	*p=0;
	if(depend_groups){
		parse_arg_ex(depend_groups,&args,",");
		for(i=0;i<args.argc;i++){
			p+=_snprintf(
				p,
				sizeof(dep)-(p-dep)-2,
				"%c%s",
				SC_GROUP_IDENTIFIER,
				args.argv[i]
				) + 1;
		}
	}
	if(depend_services){
		parse_arg_ex(depend_services,&args,",");
		for(i=0;i<args.argc;i++){
			p+=_snprintf(p,sizeof(dep)-(p-dep)-2,"%s",args.argv[i])+1;
		}
	}

	*p++=0;
	*p++=0;
	return dep;
}

/****************************************************************************
*
*	 FUNCTION: InstallDriver( IN SC_HANDLE, IN LPCTSTR, IN LPCTSTR)
*
*	 PURPOSE: Creates a driver service.
*
****************************************************************************/
BOOL InstallDriver(
	const TCHAR * Name, 
	const TCHAR * Path,
	int startup_mdoe,
	PDWORD Error
	)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	const char * dep;

	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	// Remove previous instance
	RemoveDriver( schSCManager, Name );

	// Ignore success of installation: it may already be installed.    

	//
	// NOTE: This creates an entry for a standalone driver. If this
	//		 is modified for use with a driver that requires a Tag,
	//		 Group, and/or Dependencies, it may be necessary to
	//		 query the registry for existing driver information
	//		 (in order to determine a unique Tag, etc.).
	//

	dep=make_dependence_string();

	schService = CreateService( schSCManager,		   // SCManager database
		Name,			// name of service
		display_name,			// name to display
		SERVICE_ALL_ACCESS,    // desired access
		service_type,	// service type
		startup_mdoe,
		SERVICE_ERROR_NORMAL,  // error control type
		Path,			 // service's binary
		group_name,
		NULL,				   // no tag identifier
		dep,
		NULL,				   // LocalSystem account
		NULL				   // no password
		);
	if ( schService == NULL )
		return FALSE;

	CloseServiceHandle( schService );

	*Error=GetLastError();
	CloseServiceHandle( schSCManager );

	return TRUE;
}

BOOL StartDriver( const TCHAR * Name,PDWORD Error)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	BOOL		okay;

	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	*Error=GetLastError();
	if(!schSCManager) return 0;
	schService = OpenService( schSCManager,
		Name,
		SERVICE_ALL_ACCESS
		);
	// Ignore success of start: it may already be started.
	okay = StartService( schService, 0, NULL )
	   || GetLastError() == ERROR_SERVICE_ALREADY_RUNNING 
	   || GetLastError() == ERROR_SERVICE_DISABLED;
	*Error=GetLastError();
	CloseServiceHandle(schService);
	CloseServiceHandle( schSCManager );
	return okay;
}

BOOL StopDriver( const TCHAR * Name,PDWORD Error)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	BOOL		okay;
	SERVICE_STATUS st;

	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	*Error=GetLastError();
	if(!schSCManager) return 0;
	schService = OpenService( schSCManager,
		Name,
		SERVICE_ALL_ACCESS
		);
	// Ignore success of start: it may already be started.
	okay = ControlService(schService, SERVICE_CONTROL_STOP,&st );
	*Error=GetLastError();
	CloseServiceHandle(schService);
	CloseServiceHandle( schSCManager );
	return okay;
}

static DWORD Abort( HWND hWnd, TCHAR * Msg, DWORD Error );
/****************************************************************************
*
*	 FUNCTION: UnloadDeviceDriver( const TCHAR *)
*
*	 PURPOSE: Stops the driver and has the configuration manager unload it.
*
****************************************************************************/
BOOL UnloadDeviceDriver( const TCHAR * Name )
{
	SC_HANDLE	schSCManager;

	schSCManager = OpenSCManager(	NULL,				  // machine (NULL == local)
		NULL,				  // database (NULL == default)
		SC_MANAGER_ALL_ACCESS // access required
		);

	StopDriver( schSCManager, Name );
	RemoveDriver( schSCManager, Name );
	 
	CloseServiceHandle( schSCManager );

	return TRUE;
}

BOOL SameFile(char * f1,char * f2)
{
	char ff1[MAX_PATH],ff2[MAX_PATH];
	char *fn1,*fn2;
	GetFullPathName(f1,sizeof(ff1),ff1,&fn1);
	GetFullPathName(f2,sizeof(ff2),ff2,&fn2);
	return !strcmpi(ff1,ff2);
}

/*********************************************************
	  动态加载指定的驱动程序
	  输入：
	  SYS_FILE : 驱动程序文件(*.sys)
	  SYS_NAME : 驱动程序的名称(将在服务控制面板中可以看到的名字)
	  返回：1：成功，－1：失败
*********************************************************/
int LoadDevice(TCHAR * SYS_FILE,TCHAR * SYS_NAME)
{
	HWND hWnd=0;
	TCHAR	systemRoot[ MAX_PATH ];
	TCHAR	Path[ MAX_PATH ];
	TCHAR	driverPath[ MAX_PATH ];
	DWORD error;
	TCHAR * p;
	BOOL bShouldDelete=FALSE;
	TCHAR msgbuf[MAX_PATH];

	if(b_no_copy){
		GetFullPathName(SYS_FILE,sizeof(driverPath),driverPath,&p);
	}else{
		strncpy(Path,SYS_FILE,sizeof(Path));
		p=Path+strlen(Path);
		// Merge them into one single line so that it can be stepped over when debugging
		while(p != Path){if(*p=='\\' || *p=='/' || *p==':') {p++;break;};p--;};
		// copy the driver to <winnt>\system32\drivers so that we
		// can run off of a CD or network drive
		if( !GetEnvironmentVariable( "SYSTEMROOT", systemRoot, sizeof(systemRoot))) {		
			strcpy(msgbuf, _T("Could not resolve SYSTEMROOT environment variable") );
			return Abort( hWnd, msgbuf, GetLastError() );
		}
		sprintf( driverPath, _T("%s\\system32\\drivers\\%s"), 
									systemRoot, p );
		if(!SameFile(Path,driverPath)){
			if( !CopyFile( Path, driverPath, FALSE )) {			
				sprintf( msgbuf, _T("Unable to copy %s to %s\n\n")
					_T("Make sure that %s is in the current directory."), 
					SYS_NAME, driverPath, SYS_FILE );
				return Abort( hWnd, msgbuf, GetLastError() );
			}
			bShouldDelete = TRUE;
		}
	}

	if(service_args){
		strcat(driverPath, " ");
		strcat(driverPath, service_args);
	}

	utils_debug("Ok, installing %s as %s, mode=%d\n", driverPath, SYS_NAME, start_mode);

	if( !InstallDriver( SYS_NAME, driverPath,start_mode,&error ) )	{		
		SetFileAttributes( driverPath, FILE_ATTRIBUTE_NORMAL );
		if(bShouldDelete)	DeleteFile( driverPath );
	}
	return 1;
}

static int QueryService(char * Name)
{
	SC_HANDLE	schSCManager;
	SC_HANDLE	schService;
	SERVICE_STATUS st;
	char reg_path[MAX_PATH];
	char path[MAX_PATH];

	utils_trace("Querying service %s : ",Name);
	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );	
	if(!schSCManager) {
		wperror("OpenSCManager Failed.");
		return 0;
	}
	schService = OpenService( schSCManager,
							  Name,
							  SERVICE_ALL_ACCESS
							  );
	if(!schService){
		utils_trace("Not installed.\n");
		CloseServiceHandle(schSCManager);
		return 0;
	}
	utils_trace("Installed,");
	QueryServiceStatus(schService,&st);
	switch(st.dwCurrentState){
#define ssss(a) case a:utils_trace(#a"\n");break;
	ssss(SERVICE_STOPPED)
	ssss(SERVICE_START_PENDING)
	ssss(SERVICE_STOP_PENDING)
	ssss(SERVICE_RUNNING)
	ssss(SERVICE_CONTINUE_PENDING)
	ssss(SERVICE_PAUSE_PENDING)
	ssss(SERVICE_PAUSED)
	default:utils_trace("Unknown state\n");break;
	}
	_snprintf(
		reg_path,
		sizeof(reg_path),
		"local_machine\\system\\currentcontrolset\\services\\%s",
		Name
		);
	get_registry_string(reg_path,"ImagePath",path,sizeof(path));
	utils_trace("Driver Path=%s\n",path);
	CloseServiceHandle(schService);
	CloseServiceHandle( schSCManager );
	return st.dwCurrentState;
}

/******************************************************************************
*
*	FUNCTION:	Abort:
*
*	PURPOSE:	Handles emergency exit conditions.
*
*****************************************************************************/
DWORD Abort( HWND hWnd, TCHAR * Msg, DWORD Error )
{
	LPVOID	lpMsgBuf;
	TCHAR	errmsg[MAX_PATH];
	DWORD	error = GetLastError();
 
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, Error, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf, 0, NULL );
	_stprintf(errmsg, _T("%s: %s"), Msg, lpMsgBuf );
	if( Error == ERROR_INVALID_HANDLE || Error == ERROR_ACCESS_DENIED ||
		 Error == ERROR_FILE_NOT_FOUND ) 
		_stprintf(errmsg, _T("%s\nMake sure that you are an administrator, and ")
			_T("that driver is not already running.\n"), errmsg  );
	// MessageBox( hWnd, errmsg, _T("Filemon"), MB_OK|MB_ICONERROR );
	// PostQuitMessage( 1 );
	utils_error(errmsg);
	LocalFree( lpMsgBuf );
	return -1;
}

int main(int argc,char ** argv)
{
	int res;
	DWORD Error;
		
	BEGIN_ARGS
		ARG("-f:",sys_file,"char *","sys file of the driver")
		ARG("-s:",sys_name,"char *","name of the driver")
		ARG("-m:",start_mode,"int","Startup mode")
		ARG("-dg:",depend_groups,"char *","Dependence group(comma seperated)")
		ARG("-ds:",depend_services,"char *","Dependence services(comma seperated)")
		ARG("-g:",group_name,"char *","Group name")
		ARG("-a:",action,"char *","action : i=install,s=start,u=uninstall,S=Stop,q=query") 
		ARG("-nc",b_no_copy,"tbool","\tdon't copy .sys file to system32/drivers")
		ARG("--help",need_help,"tbool","\tshow this screen")
		ARG("-tp:", service_type, "int", "service type")
		ARG("-dn:", display_name, "char *", "display name")
		ARG("-sa:", service_args, "char *", "start parameter")
	END_ARGS
	
	// reset parameters
	sys_file=0;
	sys_name=0;
	action=0;
	depend_groups=0;
	depend_services=0;
	group_name=0;
	b_no_copy=0;
	start_mode=SERVICE_DEMAND_START;
	need_help=0;
	service_type = 1;
	display_name = 0;
	service_args = 0;

	if( process_args(argc,argv,__CmdLineArgValues) < 0 ){
		// print_usage_1();
		return 0;
	}

	if(need_help){
		print_usage_1();
		return 0;
	}

	if(!display_name){
		display_name = sys_name;
	}

	/*
		Oops, guess what, GetFullPathName("\"d:\\test.dll\"",...) will yield
		"d:\\test\\\"d:\\test.dll\"", shit!!
		Umh..., Something must be done.
		by soycola, 2002/5/28
	*/
	if( !sys_file )	{
		goto __end_of_weird_codes;
	}
	if (strlen(sys_file) <= 2) {
		goto __end_of_weird_codes;
	}
	if ( ('"' == *sys_file) && ('"' == sys_file[strlen(sys_file) -1 ]) ){
		// remove the leading and trailing '"' in sys_file which might confuse
		// GetFullPathName
		sys_file[strlen(sys_file)-1] = 0;
		sys_file++;
	}
	
	res = 0;

__end_of_weird_codes:

	if(need_help){
		print_usage_1();
		return 0;
	}
	if(!action){
		//print_usage(__CmdLineArgValues);
		return 0;
	}
	while(*action){
		switch(*action){
		case 'u':
			if(!sys_name){
				//print_usage(__CmdLineArgValues);
				return 0;
			}
			res = UnloadDeviceDriver(sys_name);
			break;
		case 'i':
			if(!sys_name || !sys_file){
				//print_usage(__CmdLineArgValues);
				return 0;
			}
			res = LoadDevice(sys_file,sys_name);
			break;
		case 's':
			res = StartDriver(sys_name,&Error);
			if(Error){
				Abort(0,"Error starting service.",Error);
			}
			break;
		case 'S':
			res = StopDriver(sys_name,&Error);
			if(Error){
				Abort(0,"Error stopping service.",Error);
			}
			break;
		case 'q':
			if(!sys_name){
				//print_usage(__CmdLineArgValues);
				return 0;
			}
			res = QueryService(sys_name);
			break;
		default:
			utils_error("Invalid action code : '%c'\n",*action);
			break;
		};
		action++;
	};
	return res;
}

extern "C" int _stdcall insmod_main(char * szCmdLine)
{
	CArgs args;
	parse_arg_ex(szCmdLine,&args," ");
	return main(args.argc,args.argv);
}

extern "C" void _stdcall dll_main(int,int,char * szCmdLine,int)
{
	setup_console_for_log();
	insmod_main(szCmdLine);
	utils_getc();
}