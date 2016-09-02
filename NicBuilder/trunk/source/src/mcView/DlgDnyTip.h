#if !defined(AFX_DLGDNYTIP_H__0965C8FA_C5F2_47DE_A0E2_5A6D8853181C__INCLUDED_)
#define AFX_DLGDNYTIP_H__0965C8FA_C5F2_47DE_A0E2_5A6D8853181C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDnyTip.h : header file
//

#include "mcviewDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgDnyTip dialog

class CDlgDnyTip : public CDialog
{
private:
	CArgs arg0;
	CArgs arg1;
	CArgs arg2;
	TAG_NAME tn;
// Construction
public:

	CMcviewDlg * m_pCtrDlg;
    CString sTips0;
    CString sTips1;
    CString sTips2;
	char curValue[32];
	char address[128];
	CDlgDnyTip(CWnd* pParent = NULL);   // standard constructor
	void UpdateView();

// Dialog Data
	//{{AFX_DATA(CDlgDnyTip)
	enum { IDD = IDD_DNYTIP };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDnyTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDnyTip)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoLinks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnTrend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDNYTIP_H__0965C8FA_C5F2_47DE_A0E2_5A6D8853181C__INCLUDED_)
