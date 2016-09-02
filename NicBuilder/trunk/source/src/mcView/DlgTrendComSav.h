#if !defined(AFX_DLGTRENDCOMSAV_H__E5A25A85_13B3_4F04_83A7_8AE9D40F295C__INCLUDED_)
#define AFX_DLGTRENDCOMSAV_H__E5A25A85_13B3_4F04_83A7_8AE9D40F295C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTrendComSav.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTrendComSav dialog

class CDlgTrendComSav : public CDialog
{
// Construction
public:
	CString m_select;
	CString m_dir;
	CDlgTrendComSav(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTrendComSav)
	enum { IDD = IDD_TRENDCOMPS };
	CListBox	m_objects;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTrendComSav)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTrendComSav)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkObjects();
	virtual void OnOK();
	afx_msg void OnSelchangeObjects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRENDCOMSAV_H__E5A25A85_13B3_4F04_83A7_8AE9D40F295C__INCLUDED_)
