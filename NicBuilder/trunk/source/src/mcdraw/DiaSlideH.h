#if !defined(AFX_DIASLIDEH_H__04D652E5_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIASLIDEH_H__04D652E5_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSlideH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideH dialog

class CDiaSlideH : public CDialog
{
// Construction
public:
	CDiaSlideH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSlideH)
	enum { IDD = IDD_SLIDE_H };
	CString	m_TagName;
	float	m_LeftSlideO;
	float	m_LeftSlideT;
	float	m_RightSlideO;
	float	m_RightSlideT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSlideH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSlideH)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASLIDEH_H__04D652E5_530E_11D5_A636_00106075B53B__INCLUDED_)
