#if !defined(AFX_DIAINPUTD_H__DD54EB18_E8C7_4A8A_AEA7_45FB54F6B3AE__INCLUDED_)
#define AFX_DIAINPUTD_H__DD54EB18_E8C7_4A8A_AEA7_45FB54F6B3AE__INCLUDED_

#include "Dnyattr.h"	// Added by ClassView
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
	bool m_bVisible;
	void RefreshShow();
	CDnyInputD * m_pInputD;
	CDiaInputD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaInputD)
	enum { IDD = IDD_INPUT_D };
		// NOTE: the ClassWizard will add data members here
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
	afx_msg void OnSet();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAINPUTD_H__DD54EB18_E8C7_4A8A_AEA7_45FB54F6B3AE__INCLUDED_)
