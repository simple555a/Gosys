// OptShow.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "OptShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptShow property page

IMPLEMENT_DYNCREATE(COptShow, CPropertyPage)

COptShow::COptShow() : CPropertyPage(COptShow::IDD)
{
	//{{AFX_DATA_INIT(COptShow)
	m_PixHor = 0;
	m_PixVer = 0;
	m_ZoomMode = -1;
	m_ShowGrid = FALSE;
	//}}AFX_DATA_INIT
}

COptShow::~COptShow()
{
}

void COptShow::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptShow)
	DDX_Text(pDX, IDC_PIX_HOR, m_PixHor);
	DDV_MinMaxLong(pDX, m_PixHor,16,9999);
	DDX_Text(pDX, IDC_PIX_VER, m_PixVer);
	DDV_MinMaxLong(pDX, m_PixVer,16,9999);
	DDX_Check(pDX, IDC_SHOWGRID, m_ShowGrid);
	DDX_Radio(pDX, IDC_MM_ANISOTROPIC, m_ZoomMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptShow, CPropertyPage)
	//{{AFX_MSG_MAP(COptShow)
	ON_BN_CLICKED(IDC_MM_ANISOTROPIC, OnMmAnisotropic)
	ON_BN_CLICKED(IDC_MM_TEXT, OnMmText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptShow message handlers

void COptShow::OnMmAnisotropic() 
{
	((CWnd*)GetDlgItem(IDC_PIX_HOR))->EnableWindow(false);
	((CWnd*)GetDlgItem(IDC_PIX_VER))->EnableWindow(false);
	m_ZoomMode=0;
}

void COptShow::OnMmText() 
{
	((CWnd*)GetDlgItem(IDC_PIX_HOR))->EnableWindow(true);
	((CWnd*)GetDlgItem(IDC_PIX_VER))->EnableWindow(true);
	m_ZoomMode=1;
}

BOOL COptShow::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if(m_ZoomMode==0){
		((CWnd*)GetDlgItem(IDC_PIX_HOR))->EnableWindow(false);
		((CWnd*)GetDlgItem(IDC_PIX_VER))->EnableWindow(false);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
