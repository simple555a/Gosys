// MCDrawView.h : interface of the CMCDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCDRAWVIEW_H__9FBCDE79_8573_472A_8283_54A572B78C44__INCLUDED_)
#define AFX_MCDRAWVIEW_H__9FBCDE79_8573_472A_8283_54A572B78C44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ED_NONE 0
#define ED_NEW 1
#define ED_MOVE 2
#define ED_RESIZE 3
#define ED_APPEND 4
#define ED_ADJUST 5
#define ED_BEFADJ 6
#define ED_EDIT 7

class CMCDrawView : public CScrollView
{
protected: // create from serialization only
	CMCDrawView();
	DECLARE_DYNCREATE(CMCDrawView)

// Attributes
public:
	CMCDrawDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCDrawView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMCDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMCDrawView)
	afx_msg void OnDrawArc();
	afx_msg void OnDrawButton();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawLibcomp();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawPicure();
	afx_msg void OnDrawPointer();
	afx_msg void OnDrawPolygon();
	afx_msg void OnDrawPolyline();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawRoundrect();
	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawArc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLibcomp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPicure(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPointer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPolyline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRoundrect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnColorBrush();
	afx_msg void OnColorFont();
	afx_msg void OnColorPen();
	afx_msg void OnColorWindow();
	afx_msg void OnLine1();
	afx_msg void OnLine2();
	afx_msg void OnLine3();
	afx_msg void OnLine4();
	afx_msg void OnLine5();
	afx_msg void OnLine6();
	afx_msg void OnLineDash();
	afx_msg void OnLineDashdot();
	afx_msg void OnLineDashdotdot();
	afx_msg void OnLineDot();
	afx_msg void OnLineNull();
	afx_msg void OnLineSolid();
	afx_msg void OnBrushBdiagonal();
	afx_msg void OnBrushCross();
	afx_msg void OnBrushDiagcross();
	afx_msg void OnBrushFdiagonal();
	afx_msg void OnBrushHorizontal();
	afx_msg void OnBrushNull();
	afx_msg void OnBrushSolid();
	afx_msg void OnBrushVertical();
	afx_msg void OnTextSet();
	afx_msg void OnTextBold();
	afx_msg void OnUpdateTextBold(CCmdUI* pCmdUI);
	afx_msg void OnTextItalic();
	afx_msg void OnUpdateTextItalic(CCmdUI* pCmdUI);
	afx_msg void OnTextUnderline();
	afx_msg void OnUpdateTextUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushBdiagonal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushCross(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushDiagcross(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushFdiagonal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushNull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushSolid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushVertical(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineDash(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineDashdot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineDashdotdot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineNull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineSolid(CCmdUI* pCmdUI);
	afx_msg void OnFormatA2b();
	afx_msg void OnFormatA2c();
	afx_msg void OnFormatA2cm();
	afx_msg void OnFormatA2l();
	afx_msg void OnFormatA2m();
	afx_msg void OnFormatA2t();
	afx_msg void OnFormatB2b();
	afx_msg void OnFormatB2f();
	afx_msg void OnFormatE2h();
	afx_msg void OnFormatE2v();
	afx_msg void OnFormatF2i();
	afx_msg void OnFormatF2z();
	afx_msg void OnFormatR2u();
	afx_msg void OnFormatR2w();
	afx_msg void OnFormatS2g();
	afx_msg void OnUpdateFormatB2b(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatB2f(CCmdUI* pCmdUI);
	afx_msg void OnFormatA2r();
	afx_msg void OnUpdateFormatA2b(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2c(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2cm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2l(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2m(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2r(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatA2t(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatE2h(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatE2v(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatF2i(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatF2z(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatR2u(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatR2w(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatS2g(CCmdUI* pCmdUI);
	afx_msg void OnToolsGrid();
	afx_msg void OnToolsMeta();
	afx_msg void OnToolsMkcell();
	afx_msg void OnToolsUpcell();
	afx_msg void OnUpdateToolsMeta(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsMkcell(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsUpcell(CCmdUI* pCmdUI);
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateColorBrush(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorPen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorWindow(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnToolsSavecell();
	afx_msg void OnUpdateToolsSavecell(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditText();
	afx_msg void OnSelectAll();
	afx_msg void OnUpdateEditText(CCmdUI* pCmdUI);
	afx_msg void OnDrawTrend();
	afx_msg void OnUpdateDrawTrend(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnToolsLock();
	afx_msg void OnToolsUnlock();
	afx_msg void OnShowAllAttribs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void CMCDrawView::PrintIt(CDC *pDC,CMCDrawDoc* pDoc);
	CString m_paste;
	CDnyAttr m_CurDnyAttr;
	CString m_sPicFile;
	CString m_sCellFile;
	long m_lCurCmdId;
	bool m_bUnderline;
	bool m_bItalic;
	long m_lBold;
	COLORREF m_FontColor;
	CCursor m_cursor;
	long m_lEditMode;
	BOOL m_bCreateNew;
	CRectTracker m_tracker;
	LOGBRUSH m_CurBrush;
	LOGFONT m_CurFont;
	LOGPEN m_CurPen;
	CPoint m_potPrv;
	CPoint m_potPrvMov;
	long m_lDrawMode;
	UINT m_cf;

public:
	CElement *m_pCurElement;
	CObList m_SelElements;
};

#ifndef _DEBUG  // debug version in MCDrawView.cpp
inline CMCDrawDoc* CMCDrawView::GetDocument()
   { return (CMCDrawDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCDRAWVIEW_H__9FBCDE79_8573_472A_8283_54A572B78C44__INCLUDED_)
