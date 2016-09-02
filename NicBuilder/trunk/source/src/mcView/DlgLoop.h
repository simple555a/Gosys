#if !defined(AFX_DLGLOOP_H__83D57061_5B87_47EB_9C44_CF9E8F8F6F5C__INCLUDED_)
#define AFX_DLGLOOP_H__83D57061_5B87_47EB_9C44_CF9E8F8F6F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoop dialog

class CDlgLoop : public CDialog
{
// Construction
public:
	CDlgLoop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLoop)
	enum { IDD = IDD_LOOP };
	CListBox	m_loopList;
	int		m_interval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoop)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOOP_H__83D57061_5B87_47EB_9C44_CF9E8F8F6F5C__INCLUDED_)
