#pragma once


// CDiaUserEdit �Ի���

class CDiaUserEdit : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserEdit)

public:
	CDiaUserEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaUserEdit();

// �Ի�������
	enum { IDD = IDD_USEREDIT };

	CString m_UserNameNew;
	CString m_PassWordNew;
	CString m_PassWordNew2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_userName;
	CString m_passwordold;
	CString m_passwordnew;
	CString m_passwordnew2;
	afx_msg void OnBnClickedOk();
};
