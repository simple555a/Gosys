#if !defined(AFX_DLGSERVERIP_H__04F1A183_1ADB_48B7_8D2F_8F1FE4ED6CFD__INCLUDED_)
#define AFX_DLGSERVERIP_H__04F1A183_1ADB_48B7_8D2F_8F1FE4ED6CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgServerIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgServerIP dialog

class CDlgServerIP : public CDialog
{
// Construction
public:
	CDlgServerIP(CWnd* pParent = NULL);   // standard constructor
	DWORD m_ip;

// Dialog Data
	//{{AFX_DATA(CDlgServerIP)
	enum { IDD = IDD_IP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgServerIP)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgServerIP)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERVERIP_H__04F1A183_1ADB_48B7_8D2F_8F1FE4ED6CFD__INCLUDED_)
