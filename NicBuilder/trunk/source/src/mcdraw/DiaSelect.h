#if !defined(AFX_DIASELECT_H__773A0C77_FFD4_4D96_841A_388C1A2638AC__INCLUDED_)
#define AFX_DIASELECT_H__773A0C77_FFD4_4D96_841A_388C1A2638AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSelect dialog

class CDiaSelect : public CDialog
{
// Construction
public:
	CString m_select;
	CString m_wildcards;
	CString m_dir;
	CDiaSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSelect)
	enum { IDD = IDD_SELECT };
	CStatic	m_preview;
	CListBox	m_objects;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeObjects();
	virtual void OnOK();
	afx_msg void OnDblclkObjects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASELECT_H__773A0C77_FFD4_4D96_841A_388C1A2638AC__INCLUDED_)
