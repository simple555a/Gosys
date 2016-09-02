#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AlarmRecord.h"
#include "mcviewDlg.h"

#include "TestBtn.h"
#include "BtnST.h"

extern long g_AlarmQueueTail;
extern long g_AckAlarmQueueTail;
extern CAlarmRecord g_AlarmRecords[RCT_ALARM_COUNT];
extern CAlarmRecord g_AckAlarmRecords[RCT_ALARM_COUNT];
extern CAlarmRecord g_AlarmRecords_x[RCT_ALARM_COUNT];
extern long g_TopAlarmPt;


// CDiaAlarmDlg 对话框

class CDiaAlarmDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiaAlarmDlg)

public:
	CDiaAlarmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaAlarmDlg();

// 对话框数据
	enum { IDD = IDD_ALARM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAlarm();
	afx_msg void OnBnClickedAlarmAcked();
	afx_msg void OnBnClickedAlarmHis();
	afx_msg void OnBnClickedAlarmQuery();
	afx_msg void OnBnClickedPrintBtn();
	afx_msg void OnBnClickedOutfileBtn();
	CButton m_alarmquery;
	CButton m_print;
	CButton m_outfile;
	CListCtrl m_acklistalarm;
	CListCtrl m_HisList;
	CListCtrl m_ackedlistalarm;
	CListCtrl m_reallistalarm;

	CButton m_ackline;
	CButton m_ackall;

	CButtonST m_alarm_R;

	//CTestBtn m_alarm_R;
	CButtonST m_alarm_Y;
	CButtonST m_alarm_W;
	CButtonST m_alarm_G;

	CButtonST m_alarm_CR;
	CButtonST m_alarm_CY;
	CButtonST m_alarm_CW;
	CButtonST m_alarm_CG;
	
	int m_rank_R1;
	int m_rank_R2;
	int m_rank_Y1;
	int m_rank_Y2;
	int m_rank_W1;
	int m_rank_W2;
	int m_rank_G1;
	int m_rank_G2;

	CStatic m_alarmackgroupbox;

	virtual BOOL OnInitDialog();
	void ListAlarmInit();
	void UpdateAckedList();
	void UpdateAckList();
	void UpdateRealAckList();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool m_ListUpdate;
	long selected;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAck();
	afx_msg void OnBnClickedAckall();
	afx_msg void OnLvnGetdispinfoAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListHis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListHis(NMHDR *pNMHDR, LRESULT *pResult);

	CMcviewDlg *sfo;

private:
    bool m_qIniTime;
	COleDateTime m_dtSta;
	COleDateTime m_dtEnd;
	int m_cur_sel;
	CString m_filter;
	CAlarmRecord g_alarmcopy[RCT_ALARM_COUNT];

	PROCESS_INFORMATION pi;
	STARTUPINFO si;

public:
	afx_msg void OnBnClickedSoundOn();
	afx_msg void OnBnClickedSoundOff();
	// 开启报警声音
	CButton m_soundOn;
	// 关闭报警声音
	CButton m_soundOff;
	afx_msg void OnBnClickedAlarmBtnR();
	afx_msg void OnBnClickedAlarmBtnY();
	afx_msg void OnBnClickedAlarmBtnW();
	afx_msg void OnBnClickedAlarmBtnG();
	afx_msg void OnNMCustomdrawListReal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListReal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckalarmR();
	afx_msg void OnBnClickedCheckalarmY();
	afx_msg void OnBnClickedCheckalarmW();
	afx_msg void OnBnClickedCheckalarmG();
};
