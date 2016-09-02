#pragma once


// CDiaUserGroupNew 对话框

class CDiaUserGroupNew : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserGroupNew)

public:
	CDiaUserGroupNew(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaUserGroupNew();

// 对话框数据
	enum { IDD = IDD_USERGROUPNEW };

	CString groupName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_groupNameValue;
};
