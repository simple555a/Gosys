// DlgServerIP.cpp : implementation file
//

#include "stdafx.h"
#include "hisq.h"
#include "DlgServerIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgServerIP dialog


CDlgServerIP::CDlgServerIP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgServerIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CDlgServerIP, CDialog)
	//{{AFX_MSG_MAP(CDlgServerIP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgServerIP message handlers

void CDlgServerIP::OnOK() 
{
	CIPAddressCtrl *p=(CIPAddressCtrl *)GetDlgItem(IDC_SERVERIP);
	p->GetAddress(m_ip);
	
	CDialog::OnOK();
}

BOOL CDlgServerIP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BYTE nf[4];
	memcpy(nf,&m_ip,4);
	char ip[20];
	sprintf(ip," %d . %d . %d . %d",nf[3],nf[2],nf[1],nf[0]);
	
	CWnd *p=GetDlgItem(IDC_CURIP);
	p->SetWindowText(ip);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
