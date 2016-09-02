#pragma once
#include "afxwin.h"


// CDlgSelectKName dialog

class CDlgSelectKName : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectKName)

public:
	CDlgSelectKName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectKName();

// Dialog Data
	enum { IDD = IDD_SELECTKEYNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_KeyObjects;
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkListkeyname();
	virtual BOOL OnInitDialog();
	// //ËÑË÷Ô´×Ö·û´®
	
	CString m_selstr;
	afx_msg void OnBnClickedButton1();
};
