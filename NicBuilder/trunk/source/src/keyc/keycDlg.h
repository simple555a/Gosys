// keycDlg.h : header file
//

#if !defined(AFX_KEYCDLG_H__968F8374_400E_4B20_BBD4_55E5710703AB__INCLUDED_)
#define AFX_KEYCDLG_H__968F8374_400E_4B20_BBD4_55E5710703AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeycDlg dialog
struct SKey2View{
	char key[8];
	char view[_MAX_FNAME];
};

const int MAX_COUNT=1024;

class CKeycDlg : public CDialog
{
	SKey2View key2view[MAX_COUNT];
	int nv;
	bool bModified;
// Construction
public:
	CKeycDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeycDlg)
	enum { IDD = IDD_KEYC_DIALOG };
	CListCtrl	m_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeycDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeycDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetdispinfoKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkKey(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool CheckDual();
	CString m_path;
	void SortByKey(bool asc=true);
	void SortByView(bool asc=true);
	void InitList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCDLG_H__968F8374_400E_4B20_BBD4_55E5710703AB__INCLUDED_)
