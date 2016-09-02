#if !defined(AFX_DIABUTTOND_H__BF25B566_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIABUTTOND_H__BF25B566_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaButtonD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonD dialog

class CDiaButtonD : public CDialog
{
// Construction
public:
	CDiaButtonD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaButtonD)
	enum { IDD = IDD_BUTTON_D };
	CComboBox	m_pulseWidth;
	int		m_Action;
	int		m_iPulseWidth;
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaButtonD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaButtonD)
	afx_msg void OnSeltag();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeCombo1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIABUTTOND_H__BF25B566_537B_11D5_A636_00106075B53B__INCLUDED_)
