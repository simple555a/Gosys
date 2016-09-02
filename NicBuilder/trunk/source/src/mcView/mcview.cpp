// mcview.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mcview.h"
#include "mcviewDlg.h"
#include "keyhook.h"
#include <direct.h>
#include <rtk_object.h>

#pragma comment(lib, "librtkproxy.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HHOOK hKeyboardLL=NULL;
BOOL gbNoSwitch=true;
BOOL g_alarmonoff=false;//Zander

CMcviewApp theApp;
char * g_startFile = "default.gdf";
bool g_bEnableNoSwitch;
HINSTANCE g_hScriptDlll;
bool bEnableSoundAlarm;
bool bRaiseSoundAlarm;
int g_maxPopup;

/////////////////////////////////////////////////////////////////////////////
// CMcviewApp											
BEGIN_MESSAGE_MAP(CMcviewApp, CWinApp)
	//{{AFX_MSG_MAP(CMcviewApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//	  DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

static void installHook()
{
	char sysdir[_MAX_PATH];
	::GetSystemDirectory(sysdir,_MAX_PATH);
	CString sGina;
	sGina.Format("%s\\pmcgina.cf",sysdir);
	FILE *fGina=fopen((LPCTSTR)sGina,"wb");
	if(NULL!=fGina){
		int b=1;
		fwrite(&b,sizeof(int),1,fGina);
		fclose(fGina);
	}
	hKeyboardLL=SetWindowsHookEx(
		WH_KEYBOARD_LL,
		(HOOKPROC)LowLevelKeyboardProc,
		::AfxGetApp()->m_hInstance,
		0
		);
}

static void removeHook()
{
	if(NULL!=hKeyboardLL) {
		UnhookWindowsHookEx(hKeyboardLL);
	}
	
	char sysdir[_MAX_PATH];
	::GetSystemDirectory(sysdir,_MAX_PATH);
	CString sGina;
	sGina.Format("%s\\pmcgina.cf",sysdir);
	FILE *fGina=fopen((LPCTSTR)sGina,"wb");
	if(NULL!=fGina){
		int b=0;
		fwrite(&b,sizeof(int),1,fGina);
		fclose(fGina);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMcviewApp construction

CMcviewApp::CMcviewApp()
{	
}

CMcviewApp::~CMcviewApp()
{	
}

class CSoundAlarmThread : public CRtkThread
{
public:
	virtual __uint run()
	{
		//read config file
		__uint method, freq, dura;
		method = GetPrivateProfileInt("ALARM", "method", 0, get_config_file());
		freq = GetPrivateProfileInt("ALARM", "freq", 1500, get_config_file());
		dura = GetPrivateProfileInt("ALARM", "dura", 1000, get_config_file());
		while(!m_bStopRequestPending){
			if(bRaiseSoundAlarm){
				if(method == 0){
					MessageBeep(MB_ICONEXCLAMATION);
					rtk_sleep(50);
					MessageBeep(MB_ICONEXCLAMATION);
				}else if(method == 1){
					Beep(freq, dura);
					rtk_sleep(50);
					Beep(freq, dura);
				}else{
					Beep(freq, dura);
					rtk_sleep(50);
					Beep(freq, dura);
				}
			}
			rtk_sleep(5000);
		}
		return 0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMcviewApp initialization

BOOL CMcviewApp::InitInstance()
{
	char * projDir = NULL;
	__bool debugMode = FALSE;
	__bool bSwitchable = FALSE;
	__bool bNoSound = FALSE;
	CSoundAlarmThread alerter;
	
	BEGIN_ARGS
		ARG("-p:", projDir, "char *", "specify the project directory")
		ARG("-f:", g_startFile, "char *", "specify the initial file to edit")
		ARG("--debug", debugMode, "tbool", "debug mode")
		ARG("-switchable", bSwitchable, "tbool", "disable no-switch mode")
		ARG("-nosound", bNoSound, "tbool", "disable sound alarm")
	END_ARGS

	// 2004/5/13, use commandline to disable no-switch
	{
		CArgs args;
		char * cmdLine;
		cmdLine = GetCommandLine();
		parse_arg_ex(cmdLine, &args);
		process_args_1(args.argc, args.argv);
		if(projDir){
			set_working_dir(projDir);
		}
		g_bEnableNoSwitch = !debugMode && !bSwitchable;
	}

	bEnableSoundAlarm = !bNoSound;
	
	m_path = get_working_dir();
	SetCurrentDirectory(m_path);

	if(check_running_instance("mcview.exe") == 1){
		// activate the existing window
		HWND h;
		COPYDATASTRUCT cds;
		mcview_ext_cmd_t t;
		h = FindWindow("#32770", MCVIEW_MAIN_DLG_NAME);
		if(!h){
			return FALSE;
		}
		if(!projDir){
			BringWindowToTop(h);
			return FALSE;
		}
		memset(&t, 0, sizeof(t));
		memset(&cds, 0, sizeof(cds));
		strncpy(t.fileName, g_startFile, sizeof(t.fileName));
		t.fileName[sizeof(t.fileName) - 1] = 0;
		strncpy(t.projDir, projDir, sizeof(t.projDir));
		t.projDir[sizeof(t.projDir) - 1] = 0;
		cds.cbData = sizeof(t);
		cds.lpData = &t;
		SendMessage(h, WM_COPYDATA, NULL, (LPARAM)&cds);
		BringWindowToTop(h);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//	of your final executable, you should remove from the following
	//	the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	::CoInitialize(NULL);
		
	if(g_bEnableNoSwitch){
		installHook();
	}

	char scriptDll[256];
	GetPrivateProfileString(
		"PMC", 
		"MCVIEW-SCRIPT", 
		"mcview-script.dll", 
		scriptDll, 
		sizeof(scriptDll), 
		get_config_file()
		);
	g_hScriptDlll = LoadLibrary(scriptDll);
	g_maxPopup = GetPrivateProfileInt(
		"PMC",
		"MAXPOPUP",
		4,
		get_config_file()
		);
//Zander 2012-10-11 for BZHG
// 	if(bEnableSoundAlarm){
// 		alerter.start();
// 	}
	
	RECT rectDesktop;
	::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	if(rectDesktop.right>1000){
		CMcviewDlg dlg;
		m_pMainWnd = &dlg;	  
		int nResponse = dlg.DoModal();
	}else{
		CMcviewDlg dlg(IDD_MCVIEW_DIALOG_S);
		m_pMainWnd = &dlg;	  
		int nResponse = dlg.DoModal();
	}

	if(bEnableSoundAlarm){
		alerter.stop(INFINITE);
	}
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//	application, rather than start the application's message pump.
	removeHook();
	return FALSE;
}

void CMcviewApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	//do nothing	
	//CWinApp::WinHelp(dwData, nCmd);
}

