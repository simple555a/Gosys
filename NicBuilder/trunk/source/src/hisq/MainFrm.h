// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F45B5089_C660_4B2A_958F_2E339BB2FBA8__INCLUDED_)
#define AFX_MAINFRM_H__F45B5089_C660_4B2A_958F_2E339BB2FBA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "config.h"
#include "ChildView.h"
#include "HisList.h"

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

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
	bool m_bViewCurve;
public:
	CWnd  *m_pView;
	CString m_format;

	bool m_bShowGrid;
	int m_iHorGrid;
	int m_iVerGrid;
	bool m_bAutoZoom;
	
	float m_fMin[HISLOG_MAX_QUERY_TAG_COUNT];
	float m_fMax[HISLOG_MAX_QUERY_TAG_COUNT];
	CString m_names[HISLOG_MAX_QUERY_TAG_COUNT];
	CString m_discription[HISLOG_MAX_QUERY_TAG_COUNT];
	COLORREF m_color[HISLOG_MAX_QUERY_TAG_COUNT];
	int m_width[HISLOG_MAX_QUERY_TAG_COUNT];

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCurve();
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewCurve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnOptQuery();
	afx_msg void OnOptPrint();
	afx_msg void OnViewFormat();
	afx_msg void OnFormatCurve();
	afx_msg void OnOptOutfile();
	afx_msg void OnOptScrprv();
	afx_msg void OnOptSrcltr();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F45B5089_C660_4B2A_958F_2E339BB2FBA8__INCLUDED_)
