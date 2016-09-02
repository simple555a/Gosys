#if !defined(AFX_DLGHOTKEY_H__ED93EAF1_A3C7_46CA_9F6F_5C75D891C902__INCLUDED_)
#define AFX_DLGHOTKEY_H__ED93EAF1_A3C7_46CA_9F6F_5C75D891C902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHotkey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHotkey dialog

class CDlgHotkey : public CDialog
{
// Construction
public:
	CDlgHotkey(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHotkey)
	enum { IDD = IDD_HOTKEY };
	CString	m_key;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHotkey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHotkey)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHOTKEY_H__ED93EAF1_A3C7_46CA_9F6F_5C75D891C902__INCLUDED_)
