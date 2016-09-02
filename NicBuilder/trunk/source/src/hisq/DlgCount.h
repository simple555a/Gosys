#if !defined(AFX_DLGCOUNT_H__118F35E2_81A1_406F_8BB5_9F505955FBB9__INCLUDED_)
#define AFX_DLGCOUNT_H__118F35E2_81A1_406F_8BB5_9F505955FBB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCount dialog

class CDlgCount : public CDialog
{
// Construction
public:
	CDlgCount(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCount)
	enum { IDD = IDD_PRTCOUNT };
	int		m_count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCount)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOUNT_H__118F35E2_81A1_406F_8BB5_9F505955FBB9__INCLUDED_)
