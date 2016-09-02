#if !defined(AFX_FVIEW_H__3B082501_CF41_490A_8BAE_6FEC04671580__INCLUDED_)
#define AFX_FVIEW_H__3B082501_CF41_490A_8BAE_6FEC04671580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FView.h : header file
//
#include "stdafx.h"
#include "element.h"
#include "mcviewdlg.h"

//if data not arrived in 0.5 second, we think it not valid
#define PRV_SHOW 5
//TAG_NAME * tag_names=new TAG_NAME[50];

/////////////////////////////////////////////////////////////////////////////
// CFView dialog
class CFView : public CDialog
{
// Construction
public:
	COLORREF m_WindowColor;
	void LoadFile(CString sFileName,int x=0,int y=0);
	CFView(CWnd* pParent = NULL);	// standard constructor
	~CFView();
	void cleanView();
// Dialog Data
	//{{AFX_DATA(CFView)
	enum { IDD = IDD_FVIEW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DealShortkey(unsigned int nKey);
	long m_prvShow;
	long m_lTimeOut;
	POINT m_potPrv;
	void UpdateElements();
	CElement * m_pSelElement;
	long GetDynTags(TAG_NAME *pTags,bool bFake,long lFahterCount=0);
	CDC dcMem;
	CDC dcBkMem;
	CBitmap memBitmap;
	CBitmap memBkBitmap;
	CString m_sCaption;
	long m_lWndHeight;
	long m_lWndWidth;
	bool m_HaveElements;
	CObList m_Elements;
	CObList m_BkElements;
	CObList m_TdElements;
	TAG_NAME * m_pTagNames;
	struct pmc_value_t  * m_pTagValues;
	long m_lTagCount;
	CBrush m_brushWhite;

	/*为了防止脉冲事间丢失所加*/
	bool m_bPulse;
	TAG_NAME tnPulse;
	pmc_value_t tgPulse;
	int wdPulse;

public:
	void setTrendTags(CString tag1, CString tag2, CString tag3, CString tag4, CString tag5);
	void ReSubscrib();
	void CFView::DrawOnPrinter(CDC *pDC);
	void PrintView();
	void PrintView(CString strFileName);
	long SetDynValues(struct pmc_value_t  *pValues);
	CString m_sFileName;
	POSITION m_pos;
	bool m_bPopup;
	CMcviewDlg * m_pCtrDlg;
	void Show();
	void PlaceWindowAt(CWnd * wnd, LONG x, LONG y);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FVIEW_H__3B082501_CF41_490A_8BAE_6FEC04671580__INCLUDED_)
