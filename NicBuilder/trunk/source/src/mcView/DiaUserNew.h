#pragma once


// CDiaUserNew 对话框

class CDiaUserNew : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserNew)

public:
	CDiaUserNew(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaUserNew();

// 对话框数据
	enum { IDD = IDD_USERNEW };
	CString m_UserName;
	CString m_PassWord;
	CString m_PassWord2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_password;
	CString m_password2;
	CString m_userName;
	afx_msg void OnBnClickedOk();
};
