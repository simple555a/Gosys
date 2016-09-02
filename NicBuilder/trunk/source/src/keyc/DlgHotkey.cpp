// DlgHotkey.cpp : implementation file
//

#include "stdafx.h"
#include "keyc.h"
#include "DlgHotkey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHotkey dialog


CDlgHotkey::CDlgHotkey(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHotkey)
	m_key = _T("");
	//}}AFX_DATA_INIT
}


void CDlgHotkey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHotkey)
	DDX_CBString(pDX, IDC_HOTKEY, m_key);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHotkey, CDialog)
	//{{AFX_MSG_MAP(CDlgHotkey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHotkey message handlers

BOOL CDlgHotkey::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CComboBox *pKeys=(CComboBox*)GetDlgItem(IDC_HOTKEY);
	
	CString sTemp;
	//add NUL
	pKeys->AddString("");
	int i;
	//add F1 - F12
	for(i=1;i<=12;i++){
		sTemp.Format("F%d",i);
		pKeys->AddString(sTemp);
	}
	//add 0 - 9
	for(i=0;i<10;i++){
		sTemp.Format("%d",i);
		pKeys->AddString(sTemp);
	}
	//add A - Z
	for(char c=65;c<90;c++){
		sTemp.Format("%c",c);
		pKeys->AddString(sTemp);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
