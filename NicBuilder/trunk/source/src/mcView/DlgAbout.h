#if !defined(AFX_DLGABOUT_H__7B9BD20D_BC16_4E5F_8DE1_A41D0D8FFE88__INCLUDED_)
#define AFX_DLGABOUT_H__7B9BD20D_BC16_4E5F_8DE1_A41D0D8FFE88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAbout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog

class CDlgAbout : public CDialog
{
// Construction
public:
	CDlgAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_ABOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAbout)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGABOUT_H__7B9BD20D_BC16_4E5F_8DE1_A41D0D8FFE88__INCLUDED_)
