#if !defined(AFX_DIACOLORA_H__CF791E62_5417_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIACOLORA_H__CF791E62_5417_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaColorA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaColorA dialog

class CDiaColorA : public CDialog
{
// Construction
public:
	COLORREF m_Color4;
	COLORREF m_Color3;
	COLORREF m_Color2;
	COLORREF m_Color1;
	COLORREF m_Color0;
	CDiaColorA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaColorA)
	enum { IDD = IDD_COLOR_A };
	float	m_Split1;
	float	m_Split2;
	float	m_Split3;
	float	m_Split4;
	CString	m_TagName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaColorA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaColorA)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnColor0();
	afx_msg void OnColor1();
	afx_msg void OnColor2();
	afx_msg void OnColor3();
	afx_msg void OnColor4();
	afx_msg void OnSeltag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACOLORA_H__CF791E62_5417_11D5_A636_00106075B53B__INCLUDED_)
