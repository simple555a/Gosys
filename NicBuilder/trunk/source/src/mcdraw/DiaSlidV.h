#if !defined(AFX_DIASLIDV_H__04D652E4_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIASLIDV_H__04D652E4_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSlidV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideV dialog

class CDiaSlideV : public CDialog
{
// Construction
public:
	CDiaSlideV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSlideV)
	enum { IDD = IDD_SLIDE_V };
	CString	m_TagName;
	float	m_BottomSlideO;
	float	m_BottomSlideT;
	float	m_TopSlideO;
	float	m_TopSlideT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSlideV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSlideV)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASLIDV_H__04D652E4_530E_11D5_A636_00106075B53B__INCLUDED_)
