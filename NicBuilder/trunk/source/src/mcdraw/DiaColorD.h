#if !defined(AFX_DIACOLORD_H__BF25B569_537B_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIACOLORD_H__BF25B569_537B_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaColorD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaColorD dialog

class CDiaColorD : public CDialog
{
// Construction
public:
	COLORREF m_ColorOff;
	COLORREF m_ColorOn;
	CDiaColorD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaColorD)
	enum { IDD = IDD_COLOR_D };
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaColorD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaColorD)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnColoroff();
	afx_msg void OnColoron();
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACOLORD_H__BF25B569_537B_11D5_A636_00106075B53B__INCLUDED_)
