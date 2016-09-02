#if !defined(AFX_DIAINPUTD_H__BF25B563_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAINPUTD_H__BF25B563_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaInputD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaInputD dialog

class CDiaInputD : public CDialog
{
// Construction
public:
	CDiaInputD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaInputD)
	enum { IDD = IDD_INPUT_D };
	CString	m_OffMsg;
	CString	m_OnMsg;
	CString	m_ResetPrm;
	CString	m_SetPrm;
	CString	m_TagName;
	CString	m_WndPrm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaInputD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaInputD)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAINPUTD_H__BF25B563_537B_11D5_A636_00106075B53B__INCLUDED_)
