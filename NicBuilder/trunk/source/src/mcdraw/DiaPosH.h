#if !defined(AFX_DIAPOSH_H__04D652E9_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAPOSH_H__04D652E9_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaPosH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaPosH dialog

class CDiaPosH : public CDialog
{
// Construction
public:
	CDiaPosH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaPosH)
	enum { IDD = IDD_POS_H };
	CString	m_TagName;
	float	m_RightPosT;
	float	m_RightPosO;
	float	m_LeftPosT;
	float	m_LeftPosO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaPosH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaPosH)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAPOSH_H__04D652E9_530E_11D5_A636_00106075B53B__INCLUDED_)
