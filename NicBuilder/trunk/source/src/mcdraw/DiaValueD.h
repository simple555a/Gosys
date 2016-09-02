#if !defined(AFX_DIAVALUED_H__04D652E1_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAVALUED_H__04D652E1_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaValueD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaValueD dialog

class CDiaValueD : public CDialog
{
// Construction
public:
	CDiaValueD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaValueD)
	enum { IDD = IDD_VALUE_D };
	CString	m_OffMsg;
	CString	m_OnMsg;
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaValueD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaValueD)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAVALUED_H__04D652E1_530E_11D5_A636_00106075B53B__INCLUDED_)
