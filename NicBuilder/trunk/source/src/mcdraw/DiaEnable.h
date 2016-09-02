#if !defined(AFX_DIAENABLE_H__81A5D001_54E1_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAENABLE_H__81A5D001_54E1_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaEnable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaEnable dialog

class CDiaEnable : public CDialog
{
// Construction
public:
	CDiaEnable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaEnable)
	enum { IDD = IDD_ENABLE };
	int		m_Enable;
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaEnable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaEnable)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAENABLE_H__81A5D001_54E1_11D5_A636_00106075B53B__INCLUDED_)
