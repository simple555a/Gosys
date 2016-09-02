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
	bool m_ack;       //���캯����ʼ��Ϊfalse������δȷ��Ϊfalse�������Ѿ�ȷ��Ϊtrue
	bool m_valid;     //���캯����ʼ��Ϊfalse���ص������и�ֵΪture������δ֪������
	char m_msg[256];
	ALARM_CLASS m_ac; //��������״̬���ࣺ������ȡ����ȷ��
	int m_rank;       //�����ĵȼ�
	CAlarmRecord();
	virtual ~CAlarmRecord();
};

#endif // !defined(AFX_ALARMRECORD_H__7971E257_588D_486C_BFFA_D1E507D23747__INCLUDED_)
