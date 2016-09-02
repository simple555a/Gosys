// DiaUserGroupNew.cpp : 实现文件
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserGroupNew.h"


// CDiaUserGroupNew 对话框

IMPLEMENT_DYNAMIC(CDiaUserGroupNew, CDialog)

CDiaUserGroupNew::CDiaUserGroupNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaUserGroupNew::IDD, pParent)
	, m_groupNameValue(_T(""))
{
}

CDiaUserGroupNew::~CDiaUserGroupNew()
{
}

void CDiaUserGroupNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GROUP_NAME, m_groupNameValue);
}


BEGIN_MESSAGE_MAP(CDiaUserGroupNew, CDialog)
	ON_BN_CLICKED(IDOK, &CDiaUserGroupNew::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaUserGroupNew 消息处理程序

void CDiaUserGroupNew::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	groupName = m_groupNameValue;
	OnOK();
}
