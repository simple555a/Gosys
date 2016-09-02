// AlarmRecord.h: interface for the CAlarmRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALARMRECORD_H__7971E257_588D_486C_BFFA_D1E507D23747__INCLUDED_)
#define AFX_ALARMRECORD_H__7971E257_588D_486C_BFFA_D1E507D23747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RCT_ALARM_COUNT 1000

class CAlarmRecord  
{
public:
	bool m_ack;       //构造函数初始化为false，报警未确认为false，报警已经确认为true
	bool m_valid;     //构造函数初始化为false，回调函数中赋值为ture，含义未知。。。
	char m_msg[256];
	ALARM_CLASS m_ac; //报警处理状态分类：报警、取消、确认
	int m_rank;       //报警的等级
	CAlarmRecord();
	virtual ~CAlarmRecord();
};

#endif // !defined(AFX_ALARMRECORD_H__7971E257_588D_486C_BFFA_D1E507D23747__INCLUDED_)
