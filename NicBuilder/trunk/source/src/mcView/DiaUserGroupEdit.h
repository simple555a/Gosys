#pragma once
#include "afxwin.h"


// CDiaGroupEdit �Ի���

class CDiaGroupEdit : public CDialog
{
	DECLARE_DYNAMIC(CDiaGroupEdit)

public:
	CDiaGroupEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaGroupEdit();

// �Ի�������
	enum { IDD = IDD_USERGROUP_EDIT };

	CString groupName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_newgroupname;
	CEdit m_newgroupnamectrl;
	afx_msg void OnBnClickedOk();
};
