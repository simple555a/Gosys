// DiaUserEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserEdit.h"

extern CString temppassword;

// CDiaUserEdit 对话框

IMPLEMENT_DYNAMIC(CDiaUserEdit, CDialog)

CDiaUserEdit::CDiaUserEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaUserEdit::IDD, pParent)
	, m_userName(_T(""))
	, m_passwordold(_T(""))
	, m_passwordnew(_T(""))
	, m_passwordnew2(_T(""))
{

}

CDiaUserEdit::~CDiaUserEdit()
{
}

void CDiaUserEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER_NAME, m_userName);
	DDX_Text(pDX, IDC_USER_PASSWORDOLD, m_passwordold);
	DDX_Text(pDX, IDC_USER_PASSWORDNEW, m_passwordnew);
	DDX_Text(pDX, IDC_USER_PASSWORDNEW2, m_passwordnew2);
}


BEGIN_MESSAGE_MAP(CDiaUserEdit, CDialog)
	ON_BN_CLICKED(IDOK, &CDiaUserEdit::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaUserEdit 消息处理程序

void CDiaUserEdit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_passwordold != temppassword)
	{
		MessageBox("原始密码错误！", NULL, MB_OK);
		return;
	}

	if (m_passwordnew == _T(""))
	{
		MessageBox("密码不能为空！", NULL, MB_OK);
		return;
	}


	if (m_passwordnew != m_passwordnew2)
	{
		MessageBox("两次输入的密码不一致！", NULL, MB_OK);
		return;
	}

	m_UserNameNew = m_userName;
	m_PassWordNew = m_passwordnew;
	m_PassWordNew2 = m_passwordnew2;
	OnOK();
}
