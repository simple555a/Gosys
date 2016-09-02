// DlgTrendComSav.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgTrendComSav.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTrendComSav dialog


CDlgTrendComSav::CDlgTrendComSav(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrendComSav::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTrendComSav)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTrendComSav::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTrendComSav)
	DDX_Control(pDX, IDC_OBJECTS, m_objects);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTrendComSav, CDialog)
	//{{AFX_MSG_MAP(CDlgTrendComSav)
	ON_LBN_DBLCLK(IDC_OBJECTS, OnDblclkObjects)
	ON_LBN_SELCHANGE(IDC_OBJECTS, OnSelchangeObjects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTrendComSav message handlers

BOOL CDlgTrendComSav::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString sTemp;
	hFind = FindFirstFile((LPCTSTR)m_dir, &FindFileData);
	if(hFind!=INVALID_HANDLE_VALUE){
		sTemp=FindFileData.cFileName;
		sTemp=sTemp.Left(sTemp.GetLength()-4);
		m_objects.AddString(sTemp);
		while(FindNextFile(hFind,&FindFileData)){
			sTemp=FindFileData.cFileName;
			sTemp=sTemp.Left(sTemp.GetLength()-4);
			m_objects.AddString(sTemp);
		}
		FindClose(hFind);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE	
}

void CDlgTrendComSav::OnDblclkObjects() 
{
	OnOK();	
}

void CDlgTrendComSav::OnOK() 
{
	if(m_objects.GetCurSel()==LB_ERR){
		MessageBox("«Î—°‘Ò∂‘œÛ");
		return;
	}
	
	CDialog::OnOK();
}

void CDlgTrendComSav::OnSelchangeObjects() 
{
	m_objects.GetText(m_objects.GetCurSel(),m_select);	
}
