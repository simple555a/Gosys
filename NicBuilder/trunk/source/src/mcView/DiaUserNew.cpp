// DiaUserNew.cpp : 实现文件
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserNew.h"


// CDiaUserNew 对话框

IMPLEMENT_DYNAMIC(CDiaUserNew, CDialog)

CDiaUserNew::CDiaUserNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaUserNew::IDD, pParent)
	, m_password(_T(""))
	, m_password2(_T(""))
	, m_userName(_T(""))
{
}

CDiaUserNew::~CDiaUserNew()
{
}

void CDiaUserNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER_PASSWORD, m_password);
	DDX_Text(pDX, IDC_USER_PASSWORD2, m_password2);
	DDX_Text(pDX, IDC_USER_NAME, m_userName);
}


BEGIN_MESSAGE_MAP(CDiaUserNew, CDialog)
	ON_BN_CLICKED(IDOK, &CDiaUserNew::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaUserNew 消息处理程序

void CDiaUserNew::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_password == _T(""))
	{
		MessageBox("密码不能为空！", NULL, MB_OK);
		return;
	}

	if (m_password != m_password2)
	{
		MessageBox("两次输入的密码不一致", NULL, MB_OK);
		return;
	}
	m_UserName = m_userName;
	m_PassWord = m_password;
	m_PassWord2 = m_password2;
	OnOK();
}
