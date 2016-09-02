#if !defined(AFX_DIASIZEV_H__04D652E6_530E_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIASIZEV_H__04D652E6_530E_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaSizeV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeV dialog

class CDiaSizeV : public CDialog
{
// Construction
public:
	CDiaSizeV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaSizeV)
	enum { IDD = IDD_SIZE_V };
	int		m_VerRef;
	CString	m_TagName;
	float	m_MaxHeightO;
	float	m_MaxHeightT;
	float	m_MinHeightO;
	float	m_MinHeightT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaSizeV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaSizeV)
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASIZEV_H__04D652E6_530E_11D5_A636_00106075B53B__INCLUDED_)
