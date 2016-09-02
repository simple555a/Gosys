// MCDraw.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <direct.h>

#include "MCDraw.h"
#include "MainFrm.h"
#include "MCDrawDoc.h"
#include "MCDrawView.h"
#include "diaselect.h"
#include "rtkgui.h"

#pragma comment(lib, "pmcseeker.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMCDrawApp

BEGIN_MESSAGE_MAP(CMCDrawApp, CWinApp)
	//{{AFX_MSG_MAP(CMCDrawApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//	  DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCDrawApp construction

/////////////////////////////////////////////////////////////////////////////
// The one and only CMCDrawApp object

CMCDrawApp theApp;
//CDog theDog(1,'d');

BOOL bShowGrid;
long lOldZoom;
long lVtGrid,lHzGrid;
BOOL bLockOnGrid;
long lWndWidth,lWndHeight;
CString sWndCaption;
char * docEncoding;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {7BC78925-F5A4-4DE5-B2F2-0303955FC672}
static const CLSID clsid =
{ 0x7bc78925, 0xf5a4, 0x4de5, { 0xb2, 0xf2, 0x3, 0x3, 0x95, 0x5f, 0xc6, 0x72 } };

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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
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


/////////////////////////////////////////////////////////////////////////////
// CMCDrawApp initialization

CMCDrawApp::CMCDrawApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CMCDrawApp::~CMCDrawApp()
{
	UnloadSeeker();
	uninit_proxy();           //librtkproxy.dll   rtkproxy.cpp
}

BOOL CMCDrawApp::InitInstance()
{
	char * projDir = NULL;
	char * file = NULL;
	__bool showOnTop = __false;
	__bool bSesame = __false;

	BEGIN_ARGS
		ARG("-p:", projDir, "char *", "specify the project directory")
		ARG("-f:", file, "char *", "specify the initial file to edit")
		ARG("-t", showOnTop, "tbool", "always on top")
		ARG("-encoding:", docEncoding, "char *", "document encoding")
		ARG("-sesame", bSesame, "tbool", "call sesame procedure")
	END_ARGS

#if defined(DEBUG) && 0
	{
		long allocReqNum;
		void * my_pointer = malloc(10);
		_CrtIsMemoryBlock(my_pointer, 10, &allocReqNum, NULL, NULL);
		free(my_pointer);
	}
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(213);
#endif

	if (!AfxOleInit()){
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}	

	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMCDrawDoc),
		RUNTIME_CLASS(CMainFrame),		 // main SDI frame window
		RUNTIME_CLASS(CMCDrawView));
	AddDocTemplate(pDocTemplate);

	{
		CArgs args;
		char * cmdLine;
		cmdLine = GetCommandLine();
		parse_arg_ex(cmdLine, &args);
		process_args_1(args.argc, args.argv);
		if(projDir){
			set_working_dir(projDir);
		}
		if(docEncoding){
			docEncoding = strdup(docEncoding);
		}
	}

	m_path = get_working_dir();
	SetCurrentDirectory(m_path);

	OnFileNew();
	
	if(file){
		theDoc->OnOpenDocument(file);
		if(bSesame){
			theDoc->OnSaveDocument(file);
		}
	}else{
		theDoc->OnFileNew();
	}

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	init_proxy(m_pMainWnd->m_hWnd, PROXY_ENABLE_BGLOADER);         //librtkproxy.dll   rtkproxy.cpp
	LoadSeeker(m_pMainWnd->m_hWnd);
	
	if(showOnTop){
		m_pMainWnd->SetWindowPos(
			&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE
			);
	}
	
	((CMCDrawDoc*)(((CMainFrame*)m_pMainWnd)->GetActiveDocument()))->m_bDemo = false;

	m_pMainWnd->DragAcceptFiles();

	CString str;
	str.Format("%s/file",m_path);
	_mkdir((LPCTSTR)str);
	str.Format("%s/lib", m_path);
	_mkdir((LPCTSTR)str);
	str.Format("%s/pic", m_path);
	_mkdir((LPCTSTR)str);

	return bSesame? FALSE : TRUE;
}


// App command to run the dialog
void CMCDrawApp::OnAppAbout()
{
#if 0
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
#else
	pmcAbout(GetMainWnd()->m_hWnd, "»­Ãæ¼à¿Ø³ÌÐò");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CMCDrawApp message handlers


BOOL CMCDrawApp::SaveAllModified() 
{
	return CWinApp::SaveAllModified();
}
