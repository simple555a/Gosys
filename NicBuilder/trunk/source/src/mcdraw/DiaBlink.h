#if !defined(AFX_DIABLINK_H__CF791E64_5417_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIABLINK_H__CF791E64_5417_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaBlink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaBlink dialog

class CDiaBlink : public CDialog
{
// Construction
public:
	COLORREF m_ColorBrush;
	COLORREF m_ColorPen;
	COLORREF m_ColorText;
	CDiaBlink(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaBlink)
	enum { IDD = IDD_BLINK };
	CString	m_TagName;
	int		m_Visible;
	int		m_Speed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaBlink)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaBlink)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnColorbrush();
	afx_msg void OnColorhtext();
	afx_msg void OnColorpen();
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIABLINK_H__CF791E64_5417_11D5_A636_00106075B53B__INCLUDED_)
