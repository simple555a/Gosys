#if !defined(AFX_DIABUTTONA_H__BF25B567_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIABUTTONA_H__BF25B567_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaButtonA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonA dialog

class CDiaButtonA : public CDialog
{
// Construction
public:
	CDiaButtonA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaButtonA)
	enum { IDD = IDD_BUTTON_A };
	float	m_DownLimit;
	int		m_Fuction;
	float	m_Number;
	CString	m_TagName;
	float	m_UpLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaButtonA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaButtonA)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIABUTTONA_H__BF25B567_537B_11D5_A636_00106075B53B__INCLUDED_)
