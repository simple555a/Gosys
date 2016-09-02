#if !defined(AFX_DIACOLORDA_H__CF791E61_5417_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIACOLORDA_H__CF791E61_5417_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaColorDA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaColorDA dialog

class CDiaColorDA : public CDialog
{
// Construction
public:
	COLORREF m_ColorAlarm;
	COLORREF m_ColorNomal;
	CDiaColorDA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaColorDA)
	enum { IDD = IDD_COLOR_DA };
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaColorDA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaColorDA)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnColoralarm();
	afx_msg void OnColornomal();
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACOLORDA_H__CF791E61_5417_11D5_A636_00106075B53B__INCLUDED_)
