#pragma once


// CDiaUserNew �Ի���

class CDiaUserNew : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserNew)

public:
	CDiaUserNew(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaUserNew();

// �Ի�������
	enum { IDD = IDD_USERNEW };
	CString m_UserName;
	CString m_PassWord;
	CString m_PassWord2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_password;
	CString m_password2;
	CString m_userName;
	afx_msg void OnBnClickedOk();
};
