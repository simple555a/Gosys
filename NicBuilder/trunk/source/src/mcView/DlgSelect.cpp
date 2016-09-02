// DlgSelect.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgSelect.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect dialog


CDlgSelect::CDlgSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelect)
	DDX_Control(pDX, IDC_OBJECTS, m_objects);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgSelect)
	ON_LBN_DBLCLK(IDC_OBJECTS, OnDblclkObjects)
	ON_LBN_SELCHANGE(IDC_OBJECTS, OnSelchangeObjects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect message handlers

void CDlgSelect::OnOK() 
{
	if(m_objects.GetCurSel()==LB_ERR){
		MessageBox("«Î—°‘Ò∂‘œÛ");
		return;
	}
	
	CDialog::OnOK();
}

void CDlgSelect::OnDblclkObjects() 
{
	OnOK();	
}

void CDlgSelect::OnSelchangeObjects() 
{
	m_objects.GetText(m_objects.GetCurSel(),m_select);
}

BOOL CDlgSelect::OnInitDialog() 
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
