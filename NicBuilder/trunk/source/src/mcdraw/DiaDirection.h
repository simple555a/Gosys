#if !defined(AFX_DIADIRECTION_H__BF25B565_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIADIRECTION_H__BF25B565_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaDirection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaDirection dialog

class CDiaDirection : public CDialog
{
// Construction
public:
	CDiaDirection(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaDirection)
	enum { IDD = IDD_DIRECTION };
	float	m_MaxClockWiseO;
	float	m_MaxClockwiseT;
	float	m_MinClockwiseO;
	float	m_MinClockwiseT;
	CString	m_TagName;
	float	m_XBias;
	float	m_YBias;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaDirection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaDirection)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIADIRECTION_H__BF25B565_537B_11D5_A636_00106075B53B__INCLUDED_)
