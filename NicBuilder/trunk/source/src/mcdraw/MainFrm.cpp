// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MCDraw.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _WIN32
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtkproxy.lib")
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_COLOR, OnViewColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR, OnUpdateViewColor)
	ON_COMMAND(ID_VIEW_DRAW, OnViewDraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateViewDraw)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS, OnUpdatePos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SIZE, OnUpdateSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_POS,
	ID_INDICATOR_SIZE,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bmpLine1a.LoadBitmap(IDB_LINE1A);
	m_bmpLine1d.LoadBitmap(IDB_LINE1D);	
	m_bmpLine2a.LoadBitmap(IDB_LINE2A);	
	m_bmpLine2d.LoadBitmap(IDB_LINE2D);	
	m_bmpLine3a.LoadBitmap(IDB_LINE3A);	
	m_bmpLine3d.LoadBitmap(IDB_LINE3D);	
	m_bmpLine4a.LoadBitmap(IDB_LINE4A);	
	m_bmpLine4d.LoadBitmap(IDB_LINE4D);	
	m_bmpLine5a.LoadBitmap(IDB_LINE5A);	
	m_bmpLine5d.LoadBitmap(IDB_LINE5D);	
	m_bmpLine6a.LoadBitmap(IDB_LINE6A);	
	m_bmpLine6d.LoadBitmap(IDB_LINE6D);	
	m_bmpNulla.LoadBitmap(IDB_NULLA);
	m_bmpNulld.LoadBitmap(IDB_NULLD);	
	m_bmpSolida.LoadBitmap(IDB_SOLIDA);	
	m_bmpSolidd.LoadBitmap(IDB_SOLIDD);	
	m_bmpDota.LoadBitmap(IDB_DOTA);	
	m_bmpDotd.LoadBitmap(IDB_DOTD);	
	m_bmpDasha.LoadBitmap(IDB_DASHA);	
	m_bmpDashd.LoadBitmap(IDB_DASHD);	
	m_bmpDashdota.LoadBitmap(IDB_DASHDOTA);	
	m_bmpDashdotd.LoadBitmap(IDB_DASHDOTD);	
	m_bmpDashdotdota.LoadBitmap(IDB_DASHDOTDOTA);	
	m_bmpDashdotdotd.LoadBitmap(IDB_DASHDOTDOTD);	
	m_bmpBnulla.LoadBitmap(IDB_BNULLA);	
	m_bmpBnulld.LoadBitmap(IDB_BNULLD);	
	m_bmpBsolida.LoadBitmap(IDB_BSOLIDA);	
	m_bmpBsolidd.LoadBitmap(IDB_BSOLIDD);	
	m_bmpHorizontala.LoadBitmap(IDB_HORIZONTALA);	
	m_bmpHorizontald.LoadBitmap(IDB_HORIZONTALD);	
	m_bmpVerticala.LoadBitmap(IDB_VERTICALA);	
	m_bmpVerticald.LoadBitmap(IDB_VERTICALD);	
	m_bmpFdiagonala.LoadBitmap(IDB_FDIAGONALA);	
	m_bmpFdiagonald.LoadBitmap(IDB_FDIAGONALD);	
	m_bmpBdiagonala.LoadBitmap(IDB_BDIAGONALA);	
	m_bmpBdiagonald.LoadBitmap(IDB_BDIAGONALD);	
	m_bmpCrossa.LoadBitmap(IDB_CROSSA);	
	m_bmpCrossd.LoadBitmap(IDB_CROSSD);	
	m_bmpDiagcrossa.LoadBitmap(IDB_DIAGCROSSA);	
	m_bmpDiagcrossd.LoadBitmap(IDB_DIAGCROSSD);	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;		// fail to create
	}
	if (!m_tbColors.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_tbColors.LoadToolBar(IDR_COLORS))
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;		// fail to create
	}
	if (!m_tbDrawTools.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_tbDrawTools.LoadToolBar(IDR_DRAWTOOLS))
	{
		TRACE0("Failed to create toolbar3\n");
		return -1;		// fail to create
	}


	if (!m_wndStatusBar.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,ID_MCDRAW_STATUS_BAR) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;		// fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//	be dockable
	
	EnableDocking(CBRS_ALIGN_ANY);

	m_tbDrawTools.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_tbDrawTools);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_tbColors.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf(&m_tbColors,&m_wndToolBar);

	//m_tbFont.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBarLeftOf(&m_tbFont,&m_tbColors);

	CMenu * pMainMenu=GetMenu();
	CMenu * pSubMenu=NULL;
	pSubMenu=pMainMenu->GetSubMenu(6);
	//PEN
	pSubMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&m_bmpLine1d,&m_bmpLine1a);
	pSubMenu->SetMenuItemBitmaps(1,MF_BYPOSITION,&m_bmpLine2d,&m_bmpLine2a);
	pSubMenu->SetMenuItemBitmaps(2,MF_BYPOSITION,&m_bmpLine3d,&m_bmpLine3a);
	pSubMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&m_bmpLine4d,&m_bmpLine4a);
	pSubMenu->SetMenuItemBitmaps(4,MF_BYPOSITION,&m_bmpLine5d,&m_bmpLine5a);
	pSubMenu->SetMenuItemBitmaps(5,MF_BYPOSITION,&m_bmpLine6d,&m_bmpLine6a);
	//pSubMenu->SetMenuItemBitmaps(6,MF_BYPOSITION,&m_bmpNulld,&m_bmpNulla);
	pSubMenu->SetMenuItemBitmaps(8,MF_BYPOSITION,&m_bmpSolidd,&m_bmpSolida);
	pSubMenu->SetMenuItemBitmaps(9,MF_BYPOSITION,&m_bmpDotd,&m_bmpDota);
	pSubMenu->SetMenuItemBitmaps(10,MF_BYPOSITION,&m_bmpDashd,&m_bmpDasha);
	pSubMenu->SetMenuItemBitmaps(11,MF_BYPOSITION,&m_bmpDashdotd,&m_bmpDashdota);
	pSubMenu->SetMenuItemBitmaps(12,MF_BYPOSITION,&m_bmpDashdotdotd,&m_bmpDashdotdota);
	//BRUSH
	pSubMenu=pMainMenu->GetSubMenu(7);
	pSubMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&m_bmpBsolidd,&m_bmpBsolida);
	pSubMenu->SetMenuItemBitmaps(1,MF_BYPOSITION,&m_bmpBnulld,&m_bmpBnulla);
	pSubMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&m_bmpHorizontald,&m_bmpHorizontala);
	pSubMenu->SetMenuItemBitmaps(4,MF_BYPOSITION,&m_bmpVerticald,&m_bmpVerticala);
	pSubMenu->SetMenuItemBitmaps(5,MF_BYPOSITION,&m_bmpFdiagonald,&m_bmpFdiagonala);
	pSubMenu->SetMenuItemBitmaps(6,MF_BYPOSITION,&m_bmpBdiagonald,&m_bmpBdiagonala);
	pSubMenu->SetMenuItemBitmaps(7,MF_BYPOSITION,&m_bmpCrossd,&m_bmpCrossa);
	pSubMenu->SetMenuItemBitmaps(8,MF_BYPOSITION,&m_bmpDiagcrossd,&m_bmpDiagcrossa);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//	the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewColor() 
{
	CControlBar* pBar = &m_tbColors;
	ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
}

void CMainFrame::OnUpdateViewColor(CCmdUI* pCmdUI) 
{
	CControlBar* pBar = &m_tbColors;
	pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnViewDraw() 
{
	CControlBar* pBar = &m_tbDrawTools;
	ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);	
}

void CMainFrame::OnUpdateViewDraw(CCmdUI* pCmdUI) 
{
	CControlBar* pBar = &m_tbDrawTools;
	pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnViewStatusBar() 
{
	m_wndStatusBar.ShowWindow((m_wndStatusBar.GetStyle() & WS_VISIBLE)==0);

	RecalcLayout();	
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_wndStatusBar.GetStyle() & WS_VISIBLE)!=0);	
}

void CMainFrame::OnUpdatePos(CCmdUI* pCmdUI)
{
}

void CMainFrame::OnUpdateSize(CCmdUI* pCmdUI)
{
}