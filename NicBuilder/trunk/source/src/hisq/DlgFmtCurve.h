#if !defined(AFX_DLGFMTCURVE_H__7B10E533_56DF_41FD_A737_7404CFF604CE__INCLUDED_)
#define AFX_DLGFMTCURVE_H__7B10E533_56DF_41FD_A737_7404CFF604CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFmtCurve.h : header file
//
#include "config.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFmtCurve dialog

class CDlgFmtCurve : public CDialog
{
// Construction
public:
	CDlgFmtCurve(CWnd* pParent = NULL);   // standard constructor
	int	m_iVerGrid;
	int m_iHorGrid;
	COLORREF m_color[HISLOG_MAX_QUERY_TAG_COUNT];

// Dialog Data
	//{{AFX_DATA(CDlgFmtCurve)
	enum { IDD = IDD_FORMAT_CURVE };
	BOOL	m_bShowGrid;
	float	m_max1;
	float	m_max10;
	float	m_max2;
	float	m_max3;
	float	m_max4;
	float	m_max5;
	float	m_max6;
	float	m_max7;
	float	m_max8;
	float	m_max9;
	float	m_min1;
	float	m_min10;
	float	m_min2;
	float	m_min3;
	float	m_min4;
	float	m_min5;
	float	m_min6;
	float	m_min7;
	float	m_min8;
	float	m_min9;
	int		m_w1;
	int		m_w2;
	int		m_w3;
	int		m_w4;
	int		m_w5;
	int		m_w6;
	int		m_w7;
	int		m_w8;
	int		m_w9;
	int		m_w10;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFmtCurve)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFmtCurve)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPen1();
	afx_msg void OnPen10();
	afx_msg void OnPen2();
	afx_msg void OnPen3();
	afx_msg void OnPen4();
	afx_msg void OnPen5();
	afx_msg void OnPen6();
	afx_msg void OnPen7();
	afx_msg void OnPen8();
	afx_msg void OnPen9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFMTCURVE_H__7B10E533_56DF_41FD_A737_7404CFF604CE__INCLUDED_)
