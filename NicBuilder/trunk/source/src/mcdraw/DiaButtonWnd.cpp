// DiaButtonWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaButtonWnd.h"
#include "DiaSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMCDrawApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDiaButtonWnd dialog

CDiaButtonWnd::CDiaButtonWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaButtonWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaButtonWnd)
	m_WndFileName = _T("");
	m_ShowMode = -1;
	//}}AFX_DATA_INIT
}


void CDiaButtonWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaButtonWnd)
	DDX_Text(pDX, IDC_WNDNAME, m_WndFileName);
	DDX_Radio(pDX, IDC_POPUP, m_ShowMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaButtonWnd, CDialog)
	//{{AFX_MSG_MAP(CDiaButtonWnd)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_OVERLAP, OnOverlap)
	ON_BN_CLICKED(IDC_POPUP, OnPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonWnd message handlers

void CDiaButtonWnd::OnBrowse() 
{
	CString sFilePath;
	CDiaSelect dlg;
    dlg.m_dir = "file";
	dlg.m_wildcards = "*.gdf";
	if(dlg.DoModal()==IDOK){
		m_WndFileName=dlg.m_select;
		UpdateData(FALSE);
	}else{
		return;
	}	
}

void CDiaButtonWnd::OnOverlap() 
{
    m_ShowMode = 1;	
}

void CDiaButtonWnd::OnPopup() 
{
    m_ShowMode = 0;	
}
