// mcEvtQuery.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "evtlq.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// mc command line parse, support /st

class CMCCommand : public CCommandLineInfo
{
public:
    CMCCommand();
    virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
    bool bShowAtTop;
};

CMCCommand::CMCCommand()
{
    bShowAtTop = false;           
}

void CMCCommand::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
    if(bFlag){
        CString sPara = lpszParam;
        if(0 == sPara.CompareNoCase("st")){
            bShowAtTop = true;
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp

BEGIN_MESSAGE_MAP(CMcEvtQueryApp, CWinApp)
	//{{AFX_MSG_MAP(CMcEvtQueryApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp construction

CMcEvtQueryApp::CMcEvtQueryApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMcEvtQueryApp object

CMcEvtQueryApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp initialization

BOOL CMcEvtQueryApp::InitInstance()
{
	//get app path
    char lpFilename[_MAX_PATH];
    memset(lpFilename,0,_MAX_PATH);
    GetModuleFileName( NULL,lpFilename,_MAX_PATH);
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath( lpFilename, drive, dir, fname, ext );
    CString sAppDrive=drive;
    CString sAppPath=dir;
    m_path=sAppDrive+sAppPath;

	GetPrivateProfileString("evtlq", "folder", (LPCSTR)m_path, lpFilename, _MAX_PATH, get_config_file());
	m_folder=lpFilename;

    AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	//	NULL);
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW,NULL,NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

    CMCCommand cmdInfo;
	ParseCommandLine(cmdInfo);
    if(cmdInfo.bShowAtTop){
        pFrame->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    }

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMcEvtQueryApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp message handlers

