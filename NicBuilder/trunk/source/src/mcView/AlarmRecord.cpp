

// AlarmRecord.cpp: implementation of the CAlarmRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mcview.h"
#include "AlarmRecord.h"
#include<direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long FindTopAlarmNoOperate();//只查报警不查操作信息

bool g_bShowAllAlarms;
long g_TopAlarmPt;
long g_AlarmQueueTail;
CAlarmRecord g_AlarmRecords[RCT_ALARM_COUNT];
CAlarmRecord g_AlarmRecords_x[RCT_ALARM_COUNT];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmRecord::CAlarmRecord()
{
	m_valid=false;
	m_ack=false;
	*m_msg = 0;
}

CAlarmRecord::~CAlarmRecord()
{

}

/*
	GET index of the first un-acknowledged alarm entry.
	returns:
	-1 if not found.(either because no alarm is present, 
	or all alarms have been confirmed)
*/
long FindTopAlarm()
{
	if(g_AlarmQueueTail<0) {
		/* the queue is empty */
		return -1;
	}

	int ptTop = -1;
	int pt;
	int i;

	// first find alarm
	// note, the search starts from most recent alarm entry and 
	// extends backwards, this is because alarm entries are inserted
	// into the queue with an ascending order.
	pt = g_AlarmQueueTail;
	for(i = 0; 
	i<RCT_ALARM_COUNT;
	i++, pt=(pt+RCT_ALARM_COUNT-1)%RCT_ALARM_COUNT//此式为pt-1，当pt=0，pt-1=999
	){
		CAlarmRecord * r = &g_AlarmRecords[pt];
		if(r->m_valid && !(r->m_ac==AC_CancelAlarm) && !r->m_ack){
			if(ptTop<0){
				ptTop=pt;
			}else{
				if(r->m_rank < r->m_rank)//???
					ptTop=pt;
			}
		}
	}
	/* yes we got it */
	if(ptTop>=0) return ptTop;

#if 1
	//second find alarm-cancel
	for(i=0;i<RCT_ALARM_COUNT;i++){
		if(g_AlarmRecords[pt].m_valid && g_AlarmRecords[pt].m_ac==AC_CancelAlarm && !g_AlarmRecords[pt].m_ack){
			if(ptTop<0){
				ptTop=pt;
			}else{
				if(g_AlarmRecords[ptTop].m_rank<g_AlarmRecords[pt].m_rank)
					ptTop=pt;
			}
		}
		pt--;
		if(pt<0) {
			pt = RCT_ALARM_COUNT;
		}
	}
#endif

	return ptTop;
}

//mox+++
long FindTopAlarmNoOperate()//只查报警不查操作信息
{
	if(g_AlarmQueueTail<0) {
		/* the queue is empty */
		return -1;
	}

	int ptTop = -1;
	int pt;
	int i;

	// first find alarm
	// note, the search starts from most recent alarm entry and 
	// extends backwards, this is because alarm entries are inserted
	// into the queue with an ascending order.
	pt = g_AlarmQueueTail;
	for(i = 0; 
		i<RCT_ALARM_COUNT;
		i++, pt=(pt+RCT_ALARM_COUNT-1)%RCT_ALARM_COUNT//此式为pt-1，当pt=0，pt-1=999
		){
			CAlarmRecord * r = &g_AlarmRecords[pt];
			if(r->m_valid && !r->m_ack && r->m_ac == AC_Alarm){
				if(ptTop<0){
					ptTop=pt;
				}else{
					if(r->m_rank < r->m_rank)//???
						ptTop=pt;
				}
			}
	}
	/* yes we got it */
	if(ptTop>=0) return ptTop;

#if 1
	//second find alarm-cancel
	for(i=0;i<RCT_ALARM_COUNT;i++){
		if(g_AlarmRecords[pt].m_valid && !g_AlarmRecords[pt].m_ack && g_AlarmRecords[pt].m_ac == AC_Alarm){
			if(ptTop<0){
				ptTop=pt;
			}else{
				if(g_AlarmRecords[ptTop].m_rank<g_AlarmRecords[pt].m_rank)
					ptTop=pt;
			}
		}
		pt--;
		if(pt<0) {
			pt = RCT_ALARM_COUNT;
		}
	}
#endif

	return ptTop;
}
//mox+++

