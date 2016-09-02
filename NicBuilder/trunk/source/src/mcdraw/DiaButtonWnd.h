#if !defined(AFX_DIABUTTONWND_H__BF25B568_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIABUTTONWND_H__BF25B568_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaButtonWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonWnd dialog

class CDiaButtonWnd : public CDialog
{
// Construction
public:
	CDiaButtonWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaButtonWnd)
	enum { IDD = IDD_BUTTON_WND };
	CString	m_WndFileName;
	int		m_ShowMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaButtonWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaButtonWnd)
	afx_msg void OnBrowse();
	afx_msg void OnOverlap();
	afx_msg void OnPopup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIABUTTONWND_H__BF25B568_537B_11D5_A636_00106075B53B__INCLUDED_)
