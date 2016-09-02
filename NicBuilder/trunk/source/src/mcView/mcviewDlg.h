// mcviewDlg.h : header file
//
#if !defined(AFX_MCVIEWDLG_H__2CDC827D_8E4B_4B28_84D8_E21647B7DB42__INCLUDED_)
#define AFX_MCVIEWDLG_H__2CDC827D_8E4B_4B28_84D8_E21647B7DB42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "alarmrecord.h"
#include "afxwin.h"

#define WM_REFRESH_MESSAGE WM_USER+105

class CFView;
/////////////////////////////////////////////////////////////////////////////
// CMcviewDlg dialog

class CMcviewDlg : public CDialog
{
// Construction
public:
	void RemovePopView(POSITION pos);
	CFView * FindView(CString sFileName);
	void PopupView(CString sFileName,int x=0,int y=0);
	CMcviewDlg(UINT nID=IDD_MCVIEW_DIALOG, CWnd* pParent = NULL);	// standard constructor
	~CMcviewDlg();

	void doAutoPopup();
// Dialog Data
	//{{AFX_DATA(CMcviewDlg)
	enum { IDD = IDD_MCVIEW_DIALOG };
	BOOL	m_alarmonoff;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcviewDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMcviewDlg)
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSystem();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAckalarm();
	afx_msg void OnAlarmlist();
	afx_msg void OnIndex();
	virtual void OnOK();
	afx_msg void OnNow();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAlarm();
	afx_msg void OnKey();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnLoop();
	afx_msg void OnDestroy();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnAlarmOnOff();//Zander 2012-10-8
	afx_msg LRESULT OnRefreshMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CloseOldPopView();
	SYSTEMTIME m_current;
	UINT m_nTimerRefresh;
	CBrush m_brushWhite;
public:
	afx_msg void OnPaint();
	void PopupTrend(CString tag1, CString tag2, CString tag3, CString tag4, CString tag5);
	CObList m_PopViews;
	CString m_viewList[10];
	int m_curView;
	CString m_viewLoop[32];
	int m_loopCount;
	int m_loopCurrent;
	int m_loopInterval;
	bool m_bShowLoop;
	CFView *m_pRemoveView;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE handlealarm;
	HANDLE handlepmcsvr;

	bool SendFakeAlarm(char* ackmsg);

#if 1  //2011-03-22 guooujie
	bool m_notListUpdate;
#endif
	afx_msg void OnBnClickedClosealarm();
	CButton m_alarmList;
	CButton m_alarmClose;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};

#define MCVIEW_MAIN_DLG_NAME "mcview main dialog"

struct mcview_ext_cmd_t{
	char projDir[MAX_PATH];
	char fileName[MAX_PATH];
	char reserved[128];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCVIEWDLG_H__2CDC827D_8E4B_4B28_84D8_E21647B7DB42__INCLUDED_)

