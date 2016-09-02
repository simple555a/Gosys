#if !defined(AFX_DIASIZEH_H__04D652E7_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIASIZEH_H__04D652E7_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSizeH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeH dialog

class CDiaSizeH : public CDialog
{
// Construction
public:
	CDiaSizeH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSizeH)
	enum { IDD = IDD_SIZE_H };
	int		m_HorRef;
	CString	m_TagName;
	float	m_MaxWidthO;
	float	m_MaxWidthT;
	float	m_MinWidthO;
	float	m_MinWidthT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSizeH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSizeH)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASIZEH_H__04D652E7_530E_11D5_A636_00106075B53B__INCLUDED_)
