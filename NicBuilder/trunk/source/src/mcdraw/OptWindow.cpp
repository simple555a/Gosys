// OptWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "OptWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptWindow property page

IMPLEMENT_DYNCREATE(COptWindow, CPropertyPage)

COptWindow::COptWindow() : CPropertyPage(COptWindow::IDD)
{
	//{{AFX_DATA_INIT(COptWindow)
	m_WndHeight = 0;
	m_WndWidth = 0;
	m_WndCaption = _T("");
	//}}AFX_DATA_INIT
}

COptWindow::~COptWindow()
{
}

void COptWindow::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptWindow)
	DDX_Text(pDX, IDC_WNDHEIGHT, m_WndHeight);
	DDV_MinMaxLong(pDX, m_WndHeight, 200, 4600);
	DDX_Text(pDX, IDC_WNDWIDTH, m_WndWidth);
	DDV_MinMaxLong(pDX, m_WndWidth, 200, 6400);
	DDX_Text(pDX, IDC_WNDCAPTION, m_WndCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptWindow, CPropertyPage)
	//{{AFX_MSG_MAP(COptWindow)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptWindow message handlers
