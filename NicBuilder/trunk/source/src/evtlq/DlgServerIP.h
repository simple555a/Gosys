#if !defined(AFX_DLGSERVERIP_H__07D4B73E_1B8E_45CA_8DDC_764E688B27BA__INCLUDED_)
#define AFX_DLGSERVERIP_H__07D4B73E_1B8E_45CA_8DDC_764E688B27BA__INCLUDED_

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
	DWORD m_ip;
	CDlgServerIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgServerIP)
	enum { IDD = IDD_IP };
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

#endif // !defined(AFX_DLGSERVERIP_H__07D4B73E_1B8E_45CA_8DDC_764E688B27BA__INCLUDED_)
