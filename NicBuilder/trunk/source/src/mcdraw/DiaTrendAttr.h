#if !defined(AFX_DIATRENDATTR_H__C30E1F55_3722_4309_B89E_2B8EF63C21A7__INCLUDED_)
#define AFX_DIATRENDATTR_H__C30E1F55_3722_4309_B89E_2B8EF63C21A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaTrendAttr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaTrendAttr dialog

class CDiaTrendAttr : public CDialog
{
// Construction
public:
	COLORREF m_GdColor;
	COLORREF m_BkColor;
	LOGPEN m_pens[10];
	CDiaTrendAttr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaTrendAttr)
	enum { IDD = IDD_TRENDATTR };
	CString	m_tag1;
	CString	m_tag2;
	CString	m_tag3;
	CString	m_tag4;
	CString	m_tag5;
	int		m_mode;
	BOOL	m_ca1;
	BOOL	m_ca2;
	BOOL	m_ca3;
	BOOL	m_ca4;
	BOOL	m_ca5;
	float	m_max1;
	float	m_max2;
	float	m_max3;
	float	m_max4;
	float	m_max5;
	float	m_min1;
	float	m_min2;
	float	m_min3;
	float	m_min4;
	float	m_min5;
	BOOL	m_ca10;
	BOOL	m_ca6;
	BOOL	m_ca7;
	BOOL	m_ca8;
	BOOL	m_ca9;
	float	m_max10;
	float	m_max6;
	float	m_max7;
	float	m_max8;
	float	m_max9;
	float	m_min10;
	float	m_min6;
	float	m_min7;
	float	m_min8;
	float	m_min9;
	CString	m_tag10;
	CString	m_tag6;
	CString	m_tag7;
	CString	m_tag8;
	CString	m_tag9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaTrendAttr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaTrendAttr)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPen5();
	afx_msg void OnPen4();
	afx_msg void OnPen3();
	afx_msg void OnPen2();
	afx_msg void OnPen1();
	afx_msg void OnBkcolor();
	afx_msg void OnGdcolor();
	afx_msg void OnPen10();
	afx_msg void OnPen6();
	afx_msg void OnPen7();
	afx_msg void OnPen8();
	afx_msg void OnPen9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString seltagName();
	afx_msg void OnBnClickedSeltag11();

	afx_msg void OnBnClickedSeltag12();
	afx_msg void OnBnClickedSeltag13();
	afx_msg void OnBnClickedSeltag14();
	afx_msg void OnBnClickedSeltag15();
	afx_msg void OnBnClickedSeltag16();
	afx_msg void OnBnClickedSeltag17();
	afx_msg void OnBnClickedSeltag18();
	afx_msg void OnBnClickedSeltag19();
	afx_msg void OnBnClickedSeltag20();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIATRENDATTR_H__C30E1F55_3722_4309_B89E_2B8EF63C21A7__INCLUDED_)
