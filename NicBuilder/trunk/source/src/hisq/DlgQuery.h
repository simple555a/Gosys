#if !defined(AFX_DLGQUERY_H__2B7BF372_D14B_4AD4_BBBC_1DB8459D2CA5__INCLUDED_)
#define AFX_DLGQUERY_H__2B7BF372_D14B_4AD4_BBBC_1DB8459D2CA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery dialog

class CDlgQuery : public CDialog
{
// Construction
public:
	CDlgQuery(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgQuery)
	enum { IDD = IDD_QUERY };
	COleDateTime	m_date;
	COleDateTime	m_time;
	int		m_count;
	CString	m_tag1;
	CString	m_tag10;
	CString	m_tag2;
	CString	m_tag3;
	CString	m_tag4;
	CString	m_tag5;
	CString	m_tag6;
	CString	m_tag7;
	CString	m_tag8;
	CString	m_tag9;
	CString	m_dis1;
	CString	m_dis2;
	CString	m_dis3;
	CString	m_dis4;
	CString	m_dis5;
	CString	m_dis6;
	CString	m_dis7;
	CString	m_dis8;
	CString	m_dis9;
	CString	m_dis10;
	COleDateTime	m_dateEnd;
	COleDateTime	m_timeEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgQuery)
	virtual void OnOK();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnSeltag1();
	afx_msg void OnSeltag2();
	afx_msg void OnSeltag3();
	afx_msg void OnSeltag4();
	afx_msg void OnSeltag5();
	afx_msg void OnSeltag6();
	afx_msg void OnSeltag7();
	afx_msg void OnSeltag8();
	afx_msg void OnSeltag9();
	afx_msg void OnSeltag10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERY_H__2B7BF372_D14B_4AD4_BBBC_1DB8459D2CA5__INCLUDED_)
