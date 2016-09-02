#pragma once
#include "afxwin.h"


// CDiaGroupEdit 对话框

class CDiaGroupEdit : public CDialog
{
	DECLARE_DYNAMIC(CDiaGroupEdit)

public:
	CDiaGroupEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaGroupEdit();

// 对话框数据
	enum { IDD = IDD_USERGROUP_EDIT };

	CString groupName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_newgroupname;
	CEdit m_newgroupnamectrl;
	afx_msg void OnBnClickedOk();
};
