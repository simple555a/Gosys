// DlgQuery.cpp : implementation file
//

#include "stdafx.h"
#include "evtlq.h"
#include "DlgQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery dialog


CDlgQuery::CDlgQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuery)
	m_date_end = COleDateTime::GetCurrentTime();
	m_date_sta = COleDateTime::GetCurrentTime();
	m_time_end = COleDateTime::GetCurrentTime();
	m_time_sta = COleDateTime::GetCurrentTime();
	m_filter = _T("");
	//}}AFX_DATA_INIT
	m_cur_sel = 0;
}


void CDlgQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQuery)
	DDX_Control(pDX, IDC_EVENT_CLASS, m_event_class);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_date_end);
	DDX_DateTimeCtrl(pDX, IDC_DATE_STA, m_date_sta);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_time_end);
	DDX_DateTimeCtrl(pDX, IDC_TIME_STA, m_time_sta);
	DDX_Text(pDX, IDC_FILTER, m_filter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery message handlers

void CDlgQuery::OnOK() 
{
	m_cur_sel = m_event_class.GetCurSel();
	if(!UpdateData()) return;
	
	CDialog::OnOK();
}

BOOL CDlgQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_event_class.SetCurSel(m_cur_sel);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
