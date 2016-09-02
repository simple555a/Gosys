#if !defined(AFX_DIASAVE_H__ADC86317_135F_4A9F_89EA_86649A3B2336__INCLUDED_)
#define AFX_DIASAVE_H__ADC86317_135F_4A9F_89EA_86649A3B2336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSave dialog

class CDiaSave : public CDialog
{
// Construction
public:
	CString m_dir;
	CDiaSave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSave)
	enum { IDD = IDD_SAVE };
	CListBox	m_objects;
	CString	m_select;
	/*
		2005-3-25, selectMode indicates what to browse
		(0=gdf, 1=emf, 2=gcf, ...)
	*/
	CString m_baseDir;
	CString m_wildCards;
	int	m_selectMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSave)
	afx_msg void OnSelchangeObjects();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkObjects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASAVE_H__ADC86317_135F_4A9F_89EA_86649A3B2336__INCLUDED_)
