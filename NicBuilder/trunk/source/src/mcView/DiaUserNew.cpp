// DiaUserNew.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserNew.h"


// CDiaUserNew �Ի���

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


// CDiaUserNew ��Ϣ�������

void CDiaUserNew::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_password == _T(""))
	{
		MessageBox("���벻��Ϊ�գ�", NULL, MB_OK);
		return;
	}

	if (m_password != m_password2)
	{
		MessageBox("������������벻һ��", NULL, MB_OK);
		return;
	}
	m_UserName = m_userName;
	m_PassWord = m_password;
	m_PassWord2 = m_password2;
	OnOK();
}
