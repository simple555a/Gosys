// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C05F8484_3F51_4F17_B3F1_B5209B343A57__INCLUDED_)
#define AFX_MAINFRM_H__C05F8484_3F51_4F17_B3F1_B5209B343A57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "QuerySocket.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CToolBar    m_wndToolBar;
	CListCtrl    m_wndView;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnOptQuery();
	afx_msg void OnOptQueryById();
	afx_msg void OnOptIp();
	afx_msg void OnOptPrint();
	afx_msg void OnOptOutfile();
	//}}AFX_MSG
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	bool m_qIniTime;
	COleDateTime m_dtSta;
	COleDateTime m_dtEnd;
	int m_cur_sel;
	bool m_qIniId;
	int m_idSta;
	int m_idCount;
	CString m_filter;
	//DWORD m_ip;
	char m_ipFile[_MAX_PATH];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C05F8484_3F51_4F17_B3F1_B5209B343A57__INCLUDED_)
