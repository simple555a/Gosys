#if !defined(AFX_HISLIST_H__4E528ED0_9211_46A6_86ED_EEEB39959FFB__INCLUDED_)
#define AFX_HISLIST_H__4E528ED0_9211_46A6_86ED_EEEB39959FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHisList window

class CHisList : public CListCtrl
{
// Construction
public:
	CHisList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHisList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHisList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHisList)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISLIST_H__4E528ED0_9211_46A6_86ED_EEEB39959FFB__INCLUDED_)
