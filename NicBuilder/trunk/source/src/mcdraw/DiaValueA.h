#if !defined(AFX_DIAVALUEA_H__04D652E2_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAVALUEA_H__04D652E2_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaValueA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaValueA dialog

class CDiaValueA : public CDialog
{
// Construction
public:
	CDiaValueA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaValueA)
	enum { IDD = IDD_VALUE_A };
	CString	m_TagName;
	CString	m_Format;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaValueA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaValueA)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListFormat();
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAVALUEA_H__04D652E2_530E_11D5_A636_00106075B53B__INCLUDED_)
