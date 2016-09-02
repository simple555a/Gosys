#if !defined(AFX_DLGQUERY_H__3FED2F20_B526_40E7_914E_91E3EF56BADD__INCLUDED_)
#define AFX_DLGQUERY_H__3FED2F20_B526_40E7_914E_91E3EF56BADD__INCLUDED_

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
	int m_cur_sel;

// Dialog Data
	//{{AFX_DATA(CDlgQuery)
	enum { IDD = IDD_QUERY };
	CComboBox	m_event_class;
	COleDateTime	m_date_end;
	COleDateTime	m_date_sta;
	COleDateTime	m_time_end;
	COleDateTime	m_time_sta;
	CString	m_filter;
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
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERY_H__3FED2F20_B526_40E7_914E_91E3EF56BADD__INCLUDED_)
