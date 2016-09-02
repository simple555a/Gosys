#if !defined(AFX_DIAINPUTA_H__AD01393F_E4A3_41B9_AFF2_9C68EDDFA982__INCLUDED_)
#define AFX_DIAINPUTA_H__AD01393F_E4A3_41B9_AFF2_9C68EDDFA982__INCLUDED_

#include "Dnyattr.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaInputA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaInputA dialog

class CDiaInputA : public CDialog
{
// Construction
public:
	void RefreshShow();
	CDnyInputA *m_pInputA;
	bool	m_bVisible;
	bool	m_bValueInited;
	CDiaInputA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaInputA)
	enum { IDD = IDD_INPUT_A };
	float	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaInputA)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaInputA)
	afx_msg void OnOutput();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void On0();
	virtual BOOL OnInitDialog();
	afx_msg void On1();
	afx_msg void On2();
	afx_msg void On3();
	afx_msg void On4();
	afx_msg void On5();
	afx_msg void On6();
	afx_msg void On7();
	afx_msg void On8();
	afx_msg void On9();
	afx_msg void OnSub();
	afx_msg void OnDot();
	afx_msg void OnClr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEdit* pBoxOne;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAINPUTA_H__AD01393F_E4A3_41B9_AFF2_9C68EDDFA982__INCLUDED_)
