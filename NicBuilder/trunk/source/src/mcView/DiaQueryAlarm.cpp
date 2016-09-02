// DiaQueryAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaQueryAlarm.h"


// CDiaQueryAlarm 对话框

IMPLEMENT_DYNAMIC(CDiaQueryAlarm, CDialog)

CDiaQueryAlarm::CDiaQueryAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaQueryAlarm::IDD, pParent)
	, m_date_start(COleDateTime::GetCurrentTime())
	, m_date_end(COleDateTime::GetCurrentTime())
	, m_time_start(COleDateTime::GetCurrentTime())
	, m_time_end(COleDateTime::GetCurrentTime())
	, m_filter(_T(""))
{
	m_cur_sel = 0;
}

CDiaQueryAlarm::~CDiaQueryAlarm()
{
}

void CDiaQueryAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_TYPE, m_event_class);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_date_start);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_date_end);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START, m_time_start);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_time_end);
	DDX_Text(pDX, IDC_FILTER, m_filter);
}


BEGIN_MESSAGE_MAP(CDiaQueryAlarm, CDialog)
END_MESSAGE_MAP()


// CDiaQueryAlarm 消息处理程序

BOOL CDiaQueryAlarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_event_class.SetCurSel(m_cur_sel);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDiaQueryAlarm::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_cur_sel = m_event_class.GetCurSel();
	if(!UpdateData()) return;

	CDialog::OnOK();
}
