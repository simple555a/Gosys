// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__44904D6D_DA57_4E91_9FE1_7DA19B3F2F23__INCLUDED_)
#define AFX_MAINFRM_H__44904D6D_DA57_4E91_9FE1_7DA19B3F2F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_tbDrawTools;
	CToolBar	m_tbColors;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewColor();
	afx_msg void OnUpdateViewColor(CCmdUI* pCmdUI);
	afx_msg void OnViewDraw();
	afx_msg void OnUpdateViewDraw(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
public:
	CBitmap m_bmpDiagcrossd;
	CBitmap m_bmpDiagcrossa;
	CBitmap m_bmpCrossd;
	CBitmap m_bmpCrossa;
	CBitmap m_bmpBdiagonald;
	CBitmap m_bmpBdiagonala;
	CBitmap m_bmpFdiagonald;
	CBitmap m_bmpFdiagonala;
	CBitmap m_bmpVerticald;
	CBitmap m_bmpVerticala;
	CBitmap m_bmpHorizontald;
	CBitmap m_bmpHorizontala;
	CBitmap m_bmpBsolidd;
	CBitmap m_bmpBsolida;
	CBitmap m_bmpBnulld;
	CBitmap m_bmpBnulla;
	CBitmap m_bmpDashdotdotd;
	CBitmap m_bmpDashdotdota;
	CBitmap m_bmpDashdotd;
	CBitmap m_bmpDashdota;
	CBitmap m_bmpDashd;
	CBitmap m_bmpDasha;
	CBitmap m_bmpDotd;
	CBitmap m_bmpDota;
	CBitmap m_bmpSolidd;
	CBitmap m_bmpSolida;
	CBitmap m_bmpNulld;
	CBitmap m_bmpNulla;
	CBitmap m_bmpLine6d;
	CBitmap m_bmpLine6a;
	CBitmap m_bmpLine5d;
	CBitmap m_bmpLine5a;
	CBitmap m_bmpLine4d;
	CBitmap m_bmpLine4a;
	CBitmap m_bmpLine3d;
	CBitmap m_bmpLine3a;
	CBitmap m_bmpLine2a;
	CBitmap m_bmpLine2d;
	CBitmap m_bmpLine1d;
	CBitmap m_bmpLine1a;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__44904D6D_DA57_4E91_9FE1_7DA19B3F2F23__INCLUDED_)
