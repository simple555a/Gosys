#pragma once


// CDiaUserGroupNew �Ի���

class CDiaUserGroupNew : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserGroupNew)

public:
	CDiaUserGroupNew(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaUserGroupNew();

// �Ի�������
	enum { IDD = IDD_USERGROUPNEW };

	CString groupName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_groupNameValue;
};
