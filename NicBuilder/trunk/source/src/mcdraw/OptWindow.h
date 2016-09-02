#if !defined(AFX_OPTWINDOW_H__6EA1CD1B_98E8_40DC_BE54_4E46BBF1F4F3__INCLUDED_)
#define AFX_OPTWINDOW_H__6EA1CD1B_98E8_40DC_BE54_4E46BBF1F4F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptWindow dialog

class COptWindow : public CPropertyPage
{
	DECLARE_DYNCREATE(COptWindow)

// Construction
public:
	COptWindow();
	~COptWindow();

// Dialog Data
	//{{AFX_DATA(COptWindow)
	enum { IDD = IDD_OPT_SIZE };
	long	m_WndHeight;
	long	m_WndWidth;
	CString	m_WndCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptWindow)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTWINDOW_H__6EA1CD1B_98E8_40DC_BE54_4E46BBF1F4F3__INCLUDED_)
