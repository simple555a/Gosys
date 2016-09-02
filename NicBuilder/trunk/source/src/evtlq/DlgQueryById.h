#if !defined(AFX_DLGQUERYBYID_H__96E4EC8A_7C93_4E26_B80A_6EC125DAFEEF__INCLUDED_)
#define AFX_DLGQUERYBYID_H__96E4EC8A_7C93_4E26_B80A_6EC125DAFEEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQueryById.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQueryById dialog

class CDlgQueryById : public CDialog
{
// Construction
public:
	CDlgQueryById(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgQueryById)
	enum { IDD = IDD_QUERY_BY_ID };
	int		m_count;
	int		m_pt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQueryById)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgQueryById)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERYBYID_H__96E4EC8A_7C93_4E26_B80A_6EC125DAFEEF__INCLUDED_)
