#if !defined(AFX_DM301CONFIG_H__25DDB601_6E0F_423B_B61E_073F1482E974__INCLUDED_)
#define AFX_DM301CONFIG_H__25DDB601_6E0F_423B_B61E_073F1482E974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dm301Config.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDm301Config dialog

class CDm301Config : public CDialog
{
// Construction
public:
	CDm301Config(CWnd* pParent = NULL);   // standard constructor

	__u8 d[32];
// Dialog Data
	//{{AFX_DATA(CDm301Config)
	enum { IDD = IDD_DM301 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDm301Config)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDm301Config)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DM301CONFIG_H__25DDB601_6E0F_423B_B61E_073F1482E974__INCLUDED_)
