#if !defined(AFX_DIAFILLV_H__BF25B562_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAFILLV_H__BF25B562_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaFillV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaFillV dialog

class CDiaFillV : public CDialog
{
// Construction
public:
	CDiaFillV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaFillV)
	enum { IDD = IDD_FILL_V };
	CString	m_TagName;
	int		m_VerRef;
	float	m_MinVFillO;
	float	m_MaxVFillT;
	float	m_MaxVFillO;
	float	m_MinVFillT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaFillV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaFillV)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAFILLV_H__BF25B562_537B_11D5_A636_00106075B53B__INCLUDED_)
