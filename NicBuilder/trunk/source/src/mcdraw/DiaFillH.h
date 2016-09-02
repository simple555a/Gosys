#if !defined(AFX_DIAFILLH_H__BF25B561_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIAFILLH_H__BF25B561_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaFillH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaFillH dialog

class CDiaFillH : public CDialog
{
// Construction
public:
	CDiaFillH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaFillH)
	enum { IDD = IDD_FILL_H };
	int		m_HorRef;
	CString	m_TagName;
	float	m_MinHFillT;
	float	m_MinHFillO;
	float	m_MaxHFillT;
	float	m_MaxHFillO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaFillH)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaFillH)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAFILLH_H__BF25B561_537B_11D5_A636_00106075B53B__INCLUDED_)
