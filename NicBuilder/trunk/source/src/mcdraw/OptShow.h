#if !defined(AFX_OPTSHOW_H__56471F1F_6494_4058_BDFA_D2F200A40CE9__INCLUDED_)
#define AFX_OPTSHOW_H__56471F1F_6494_4058_BDFA_D2F200A40CE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptShow dialog

class COptShow : public CPropertyPage
{
	DECLARE_DYNCREATE(COptShow)

// Construction
public:
	COptShow();
	~COptShow();

// Dialog Data
	//{{AFX_DATA(COptShow)
	enum { IDD = IDD_OPT_SHOW };
	long	m_PixHor;
	long	m_PixVer;
	int		m_ZoomMode;
	BOOL	m_ShowGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptShow)
	afx_msg void OnMmAnisotropic();
	afx_msg void OnMmText();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTSHOW_H__56471F1F_6494_4058_BDFA_D2F200A40CE9__INCLUDED_)
