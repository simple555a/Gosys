#if !defined(AFX_DIAREPLACE_H__38FB3265_EDED_4E63_96E0_C58236617A55__INCLUDED_)
#define AFX_DIAREPLACE_H__38FB3265_EDED_4E63_96E0_C58236617A55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaReplace.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaReplace dialog

class CDiaReplace : public CDialog
{
// Construction
public:
	CDiaReplace(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaReplace)
	enum { IDD = IDD_REPLACE };
	CString	m_original;
	CString	m_replaced;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaReplace)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaReplace)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAREPLACE_H__38FB3265_EDED_4E63_96E0_C58236617A55__INCLUDED_)
