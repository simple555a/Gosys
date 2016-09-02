#if !defined(AFX_DIAPASSWORD_H__5CC6C3A8_9E3A_4E2E_BCAE_D3B5A93B508C__INCLUDED_)
#define AFX_DIAPASSWORD_H__5CC6C3A8_9E3A_4E2E_BCAE_D3B5A93B508C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaPassword dialog

class CDiaPassword : public CDialog
{
// Construction
public:
	CDiaPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaPassword)
	enum { IDD = IDD_PASSWORD };
	int		m_rank;
	CString	m_pwd;
	CString	m_pwd2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaPassword)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAPASSWORD_H__5CC6C3A8_9E3A_4E2E_BCAE_D3B5A93B508C__INCLUDED_)
