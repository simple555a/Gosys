// DiaGroupEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserGroupEdit.h"


// CDiaGroupEdit 对话框

IMPLEMENT_DYNAMIC(CDiaGroupEdit, CDialog)

CDiaGroupEdit::CDiaGroupEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaGroupEdit::IDD, pParent)
	, m_newgroupname(_T(""))
{
}

CDiaGroupEdit::~CDiaGroupEdit()
{
}

void CDiaGroupEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GROUP_NEWNAME, m_newgroupname);
	DDX_Control(pDX, IDC_GROUP_NEWNAME, m_newgroupnamectrl);
}


BEGIN_MESSAGE_MAP(CDiaGroupEdit, CDialog)
	ON_BN_CLICKED(IDOK, &CDiaGroupEdit::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaGroupEdit 消息处理程序

void CDiaGroupEdit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	groupName = m_newgroupname;
	OnOK();
}
