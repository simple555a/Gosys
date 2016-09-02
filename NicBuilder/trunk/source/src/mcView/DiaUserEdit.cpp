// DiaUserEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserEdit.h"

extern CString temppassword;

// CDiaUserEdit �Ի���

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


// CDiaUserEdit ��Ϣ�������

void CDiaUserEdit::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_passwordold != temppassword)
	{
		MessageBox("ԭʼ�������", NULL, MB_OK);
		return;
	}

	if (m_passwordnew == _T(""))
	{
		MessageBox("���벻��Ϊ�գ�", NULL, MB_OK);
		return;
	}


	if (m_passwordnew != m_passwordnew2)
	{
		MessageBox("������������벻һ�£�", NULL, MB_OK);
		return;
	}

	m_UserNameNew = m_userName;
	m_PassWordNew = m_passwordnew;
	m_PassWordNew2 = m_passwordnew2;
	OnOK();
}
