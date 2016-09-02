#if !defined(AFX_DIACELLATTR_H__62A654A1_610B_11D5_82FA_0080C8F63C8D__INCLUDED_)
#define AFX_DIACELLATTR_H__62A654A1_610B_11D5_82FA_0080C8F63C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaCellAttr.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDiaCellAttr dialog

class CDiaCellAttr : public CDialog
{
// Construction
public:
	CCell * m_pCell;
	CDiaCellAttr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaCellAttr)
	enum { IDD = IDD_CELLATTR };
	CEdit	m_TagName;
	CListBox	m_TagList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaCellAttr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaCellAttr)
	afx_msg void OnSelchangeTags();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnDblclkTags();
	afx_msg void OnChangeTagName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iSel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACELLATTR_H__62A654A1_610B_11D5_82FA_0080C8F63C8D__INCLUDED_)
