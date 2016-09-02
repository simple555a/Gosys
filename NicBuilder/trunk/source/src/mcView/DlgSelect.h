#if !defined(AFX_DLGSELECT_H__955CC421_978C_4E03_BAB3_22660D014AB9__INCLUDED_)
#define AFX_DLGSELECT_H__955CC421_978C_4E03_BAB3_22660D014AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect dialog

class CDlgSelect : public CDialog
{
// Construction
public:
	CString m_select;
	CString m_dir;
	CDlgSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelect)
	enum { IDD = IDD_SELECT };
	CListBox	m_objects;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelect)
	virtual void OnOK();
	afx_msg void OnDblclkObjects();
	afx_msg void OnSelchangeObjects();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECT_H__955CC421_978C_4E03_BAB3_22660D014AB9__INCLUDED_)
