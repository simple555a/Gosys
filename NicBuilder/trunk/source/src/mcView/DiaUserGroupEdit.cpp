// DiaGroupEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserGroupEdit.h"


// CDiaGroupEdit �Ի���

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


// CDiaGroupEdit ��Ϣ�������

void CDiaGroupEdit::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	groupName = m_newgroupname;
	OnOK();
}
