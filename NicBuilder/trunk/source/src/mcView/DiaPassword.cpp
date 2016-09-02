// DiaPassword.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaPassword dialog


CDiaPassword::CDiaPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaPassword)
	m_rank = 1;
	m_pwd = _T("");
	m_pwd2 = _T("");
	//}}AFX_DATA_INIT
}


void CDiaPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaPassword)
	DDX_Radio(pDX, IDC_RANK0, m_rank);
	DDX_Text(pDX, IDC_NEW_PWD, m_pwd);
	DDV_MaxChars(pDX, m_pwd, 16);
	DDX_Text(pDX, IDC_CFM_PWD, m_pwd2);
	DDV_MaxChars(pDX, m_pwd2, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaPassword, CDialog)
	//{{AFX_MSG_MAP(CDiaPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaPassword message handlers

void CDiaPassword::OnOK() 
{
	UpdateData(TRUE);
	if(0!=m_pwd.Compare(m_pwd2)){
		MessageBox("两次输入的口令不一致,请重新输入",NULL,MB_OK|MB_ICONSTOP);
		m_pwd.Empty();
		m_pwd2.Empty();
		UpdateData(FALSE);
		return;
	}

	CDialog::OnOK();
}
