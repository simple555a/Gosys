// DiaUserGroupNew.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserGroupNew.h"


// CDiaUserGroupNew �Ի���

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


// CDiaUserGroupNew ��Ϣ�������

void CDiaUserGroupNew::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	groupName = m_groupNameValue;
	OnOK();
}
