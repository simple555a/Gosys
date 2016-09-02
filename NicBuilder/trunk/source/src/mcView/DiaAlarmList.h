#if !defined(AFX_DIAALARMLIST_H__72AAAD3D_C365_4D2A_BBE8_A0A556BA164D__INCLUDED_)
#define AFX_DIAALARMLIST_H__72AAAD3D_C365_4D2A_BBE8_A0A556BA164D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaAlarmList.h : header file
//

#include "AlarmRecord.h"
#include "AlarmListBox.h"
#include "mcviewDlg.h"

#define WM_REDRAWWINDOW_MESSAGE WM_USER+102

/////////////////////////////////////////////////////////////////////////////
// CDiaAlarmList dialog

class CDiaAlarmList : public CDialog
{
// Construction
public:
	CDiaAlarmList(CWnd* pParent = NULL);   // standard constructor
	~CDiaAlarmList();

	void UpdateList();
// Dialog Data
	//{{AFX_DATA(CDiaAlarmList)
	enum { IDD = IDD_ALARMLIST };
	CListCtrl	m_list;
	BOOL	m_ShowOperations;
	//}}AFX_DATA
	CMcviewDlg *sfo;//mox+++20130929

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaAlarmList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaAlarmList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAck();
	afx_msg void OnAckall();
	afx_msg void OnGetdispinfoAlarmlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowOperations();
	afx_msg LRESULT AlarmListRedrawWindow(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnAlarmDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

extern bool g_bShowAllAlarms;
/* # of the un-ack..ed alarm record with the highest rank */
extern long g_TopAlarmPt;
/*
	incoming alarm records are entered in this
	circular fifo.
*/
/* alarm record queue tail index */
extern long g_AlarmQueueTail;
extern CAlarmRecord g_AlarmRecords[RCT_ALARM_COUNT];

extern long FindTopAlarm();
extern long FindTopAlarmNoOperate();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAALARMLIST_H__72AAAD3D_C365_4D2A_BBE8_A0A556BA164D__INCLUDED_)
