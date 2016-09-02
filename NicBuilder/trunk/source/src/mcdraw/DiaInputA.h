#if !defined(AFX_DIAINPUTA_H__BF25B564_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAINPUTA_H__BF25B564_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaInputA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaInputA dialog

class CDiaInputA : public CDialog
{
// Construction
public:
	CDiaInputA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaInputA)
	enum { IDD = IDD_INPUT_A };
	float	m_MaxValue;
	float	m_MinValue;
	CString	m_TagName;
	CString	m_WndPrm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaInputA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaInputA)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAINPUTA_H__BF25B564_537B_11D5_A636_00106075B53B__INCLUDED_)
