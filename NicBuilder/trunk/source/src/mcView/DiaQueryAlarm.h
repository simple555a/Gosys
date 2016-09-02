#pragma once
#include "afxwin.h"


// CDiaQueryAlarm �Ի���

class CDiaQueryAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDiaQueryAlarm)

public:
	CDiaQueryAlarm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaQueryAlarm();

	int m_cur_sel;

// �Ի�������
	enum { IDD = IDD_QUERYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_event_class;
	COleDateTime m_date_start;
	COleDateTime m_date_end;
	COleDateTime m_time_start;
	COleDateTime m_time_end;
	CString m_filter;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
