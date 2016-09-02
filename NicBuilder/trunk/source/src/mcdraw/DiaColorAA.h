#if !defined(AFX_DIACOLORAA_H__CF791E63_5417_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIACOLORAA_H__CF791E63_5417_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaColorAA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaColorAA dialog

class CDiaColorAA : public CDialog
{
// Construction
public:
	COLORREF m_ColorR;
	COLORREF m_ColorL2;
	COLORREF m_ColorL1;
	COLORREF m_ColorN;
	COLORREF m_ColorH1;
	COLORREF m_ColorH2;
	CDiaColorAA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaColorAA)
	enum { IDD = IDD_COLOR_AA };
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaColorAA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaColorAA)
	afx_msg void OnColorh1();
	afx_msg void OnColorh2();
	afx_msg void OnColorl1();
	afx_msg void OnColorl2();
	afx_msg void OnColorn();
	afx_msg void OnColorr();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACOLORAA_H__CF791E63_5417_11D5_A636_00106075B53B__INCLUDED_)
