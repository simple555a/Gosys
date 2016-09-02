/********************************************************************
	PC-Based monitoring & controlling system source
	prodoct version : 2.03
	filename:	E:\vss\pmc2\src\pmcd\alarm.cpp
	file path:	E:\vss\pmc2\src\pmcd
	author:		Jackie Pan, jhunter@tsinghua.org.cn
	
	purpose :	ThiSs module provide support alarm checking/raising
	history :
	created :	by J.Hunter,jhunter@263.net,May 2001
	revised :	10:10:2002	 16:11
	2003/5/22	Jackie, master/slave mode support added
*********************************************************************/
//
// �޸ļ�¼
// 2012-10-16   ���������߼��޸�    Zander      v1.0.0.3
// 2014-05-12   ������¼��������      mox       v1.0.0.3
#include "precomp.h"

#include <math.h>
#include <string>
#include <time.h>

#ifdef _WIN32
#pragma comment(linker, "/base:0x76530000")
#endif

#define UTILS_NODEBUG

/*
�������ܣ�ȡ�������ǰʱ����ַ���
����˵������
����ֵ��  �������ǰʱ����ַ�������ʽ���������� ʱ:��:�롱
*/
static std::string texttime()
{
#ifdef _WIN32
	#define localtime(a,b) *(b)=*localtime(a)
#else	
	#define localtime(a,b) localtime_r(a,b)
#endif
	time_t tmt;
	struct tm ltm;
	time(&tmt);
	localtime(&tmt, &ltm);
	char buf[128];	  
	sprintf(
		buf,
		"%d��%d��%d�� %02d:%02d:%02d",
		ltm.tm_year+1900,
		ltm.tm_mon+1,
		ltm.tm_mday,
		ltm.tm_hour,
		ltm.tm_min,
		ltm.tm_sec
		);
#ifndef _WIN32
	#undef localtime
#endif	  
	return std::string(buf);
}

/*
	given an alarm condition, and a alarm mask
	check if the mask if effective.
	in previous version (GD99 project)
	the alarm mask is an integer, so only a bitwise and
	will be ok, but now the mask is a structure, we have
	to use a switch..case... block to do the save work.
	returns:
		true : if the alarm should be _masked(not to raise alarm)
		false: if the alarm should not be _masked(raise alarm)
*/
/*
�������ܣ��ж��Ƿ���Ҫmask����
����˵����
          a_mask
		  at        ��������
����ֵ��
          true: ��Ҫmask����
		  false: ����Ҫmask����
*/
static bool _masked(__uint a_mask, ALARM_TYPE at)
{
	bool m=false;
	switch(at){
	case AT_HiHi:
		m=a_mask & TF_HiHi? true : false;
		break;
	case AT_Hi:
		m=a_mask & TF_Hi? true : false;
		break;
	case AT_Lo:
		m=a_mask & TF_Lo? true : false;
		break;
	case AT_LoLo:
		m=a_mask & TF_LoLo? true : false;
		break;
	case AT_On2Off:
		m=a_mask & TF_On2Off? true : false;
		break;
	case AT_Off2On:
		m=a_mask & TF_Off2On? true : false;
		break;
	case AT_SharpChange:
		m=a_mask & TF_Rate? true : false;
		break;
	}
	return m;
}

/*
	check alarm for analog tags
*/
/*
�������ܣ���鲢ȷ��ģ������ǩ�ı���״̬
����˵����
          tte   ģ������ǩ�ɵ�ʵʱֵ�ṹ��
		  val   �ӿ������õ����µ�ʵʱֵ
		  now   ��ǰʱ��

  ����ֵ��ģ������ǩ�ı���״̬
*/
static int _get_exception_analog(
	RTK_TAG * tte, 
	PCpmc_value_t val, 
	PCRTK_TIME now
	)
{
	bool alarm_masked=false;
	RTK_TAG * cutoff;
	TAG_FLAGS af;
	TAG_FLAGS gf;
	int  at,at_old;

	pmc_value_t Value, OldValue;	
	__r8 span;
	__r8 hihi, hi, lo, lolo;
	__r8 deadband;
    
	//1 ��ʼ���ֲ�����
	af = tte->s.Flags;
	gf = tte->d.Value.Flags;
	at_old = tte->d.AlarmStatus;//��ǩ�Ѿ����ڵı���״̬
	
	at = AT_NoAlarm;
	
	Value.Flags = OldValue.Flags = dt_real4;
	pmc_type_cast(val, &Value);//��val����Value
	pmc_type_cast(&tte->d.Value, &OldValue);//��tte�Ķ�ֵ̬����OldValue
	
	//2 ��tte�ľ�̬���Եõ������ľ��������������ֵ�ȣ�
	switch(get_value_type(tte->s.Flags)){
		//float��double���з��ź��޷��ŵ�64λ����
	case dt_real4:
	case dt_real8:
	case dt_int64:
	case dt_uint64:
		span = fabs(tte->s.fltMaxValue-tte->s.fltMinValue); //�����޵���ֵ
		hihi = tte->s.fltHiHi;//�߸߱���ֵ
		hi = tte->s.fltHi;//�߱���ֵ
		lo = tte->s.fltLo;//�ͱ���ֵ
		lolo = tte->s.fltLoLo;//�͵ͱ���ֵ
		deadband = tte->s.fltAlarmDeadband;//�����������ٷ���
		break;

	case dt_int32:
	case dt_int16:
	case dt_int8:
	case dt_uint32:
	case dt_uint16:
	case dt_uint8:
		span = abs(tte->s.u_max.i - tte->s.u_min.i);
		hihi = tte->s.u_hihi.i;
		hi = tte->s.u_hi.i;
		lo = tte->s.u_lo.i;
		lolo = tte->s.u_lolo.i;
		deadband = tte->s.u_deadband.i;
		break;
		
	default:
		assert(0);
	}
    
	//3 �ų���������ֵ̫С�����
	if(span < 1e-5){
		span = 10;     //��������ֵȱʡֵΪ10
	}
	
	/* alarm dead-band */
	//4 ���������Ĵ��� 
	__r8 foo;
	foo = deadband * span /100;//�õ�������������ֵ

	//4 ���������������
	if( (af & TF_HiHi) && Value.Value.flt > hihi){
		tte->d.Value.Flags |= TF_HiHi;
		at=AT_HiHi;
	}else if( (af & TF_Hi) && Value.Value.flt > hi){
		tte->d.Value.Flags |= TF_Hi;
		//�������Ѿ��ڸ߸߱�����ǰֵ>hihi-������ֵ������Ϊ�߸߱�������߱�
		if(at_old==AT_HiHi && Value.Value.flt > hihi - foo){
			at=AT_HiHi;
		}else{
			at=AT_Hi;
		}
	}else if((af & TF_LoLo) && Value.Value.flt < lolo){
		tte->d.Value.Flags |= TF_LoLo;
		at=AT_LoLo;
	}else if((af & TF_Lo) && Value.Value.flt < lo){
		tte->d.Value.Flags |= TF_Lo;
		if(at_old==AT_LoLo && Value.Value.flt < lolo + foo){
			at=AT_LoLo;
		}else{
			at=AT_Lo;
		}
	}else{
		if(at_old==AT_Lo && Value.Value.flt < lo+ foo){
			at=AT_Lo;
		}else if(at_old==AT_Hi && Value.Value.flt > hi - foo){
			at=AT_Hi;
		}
	}
	
	//5 ���ʱ����Ĵ������
	__r8 seconds = rtk_time_diff(now, &tte->d.Time);
	//��ֵ̬��Ч����Χ>1e-5��ʱ��1e-5��
	if( (gf & TF_Valid) && span > 1e-5 && seconds > 1e-5){
		__r8 rate;
		__r8 dev;
		dev = fabs (Value.Value.flt -OldValue.Value.flt);//�¾�ֵ�Ĳ�
		rate = dev /span*100;//�¾�ֵ�ı仯�ٷֱ�
		rate=(float)fabs(rate/seconds);//��λʱ���¾�ֵ�ı仯�ٷֱȡ�������ֵ�仯�İٷֱ�����
		//ʵ�ʱ仯����>��̬�仯���ʣ� �Ҿ�̬����ʱ�����������˹�����Ա�ǩ�Ķ�̬��ʶ���и�ֵ��
		if((rate > tte->s.fltRate) && (af & TF_SharpChange)){
			tte->d.Value.Flags |= TF_SharpChange;
			at = AT_SharpChange;
		}
		// TODO: Uncomment this when we provide HMI for
		// TODO: the ExcDev and ExcMax, for now, ExcDev and
		// TODO: ExcMax are specified globally for all tags
#if 0
		if(dev > tte->s.ExcDev){
			tte->d.Value.Flags |= TF_Rate;
		}
#else
		//6 TF_Rate
		if(tte->s.Flags & TF_Step){
			if(dev > span){
				tte->d.Value.Flags |= TF_Rate;
			}
		}else {
			if(dev*100 > span * g_fltMaxChangeRate){
				tte->d.Value.Flags |= TF_Rate;
			}
		}
#endif
	}

#if 0
	if(seconds > tte->s.ExcMax){
			tte->d.Value.Flags |= TF_Expired;
	}
#else
	//7 ��ʱ�ж�
	if((seconds > g_fltTagLife/2.) || (seconds < -g_fltTagLife/2.)){
		tte->d.Value.Flags |= TF_Expired;
	}
#endif

	/* get locking tag's address */
	/**/
	//8 �����Ƿ���Ҫ��mask
	if(tte->s.CutOffTagName.group.Data[0]){
		cutoff = query_tag(HNODE_LOCAL_MACHINE, &tte->s.CutOffTagName);
	}else{
		cutoff = 0;
	}
	
	//
	if(cutoff && (at!=AT_NoAlarm) ){
		/* check cut off condition */
		if(_masked(tte->s.CutOffMask, cutoff->d.AlarmStatus)){
			/*
			when cutoff is in any alarm state indicated by
			tte->AlarmMask, the alarm will be _masked out
			*/
			alarm_masked=true;
		}
	}
	
	/*
		ok, we reach here
		now, the new flags of the tag have been set
		new tagtime NOT set(i feel it would be better to set it out somewhere,
		say,in the WriteTag(...))
		ready? then we return with the new alarm status
	*/
	//���ص�ǰ����״̬
	if(alarm_masked) {
		return AT_NoAlarm;
	}

	return at;
}

/*
	check alarm for logical tags
*/
/*
�������ܣ��õ���������ǩ�ı���״̬
����˵����
          tte  ʵʱ���иñ�ǩ�ɵ�ʵʱֵ�ṹ��
		  val  �ӿ������õ����µ�ʵʱֵ
����ֵ�� ��������ǩ�ı���״̬
*/
static int _get_exception_bool(RTK_TAG * tte, const TAG_VALUE * val)
{
	bool				alarm_masked=false;
	RTK_TAG				*cutoff;
	TAG_FLAGS			af, gf;
	ALARM_TYPE			at;

	gf = tte->d.Value.Flags;
	
	/* get locking tag's address */
	if(tte->s.CutOffTagName.group.Data[0]){
		cutoff=query_tag(HNODE_LOCAL_MACHINE, &tte->s.CutOffTagName);
	}else{
		cutoff=0;
	}
	af=tte->s.Flags;

	at=AT_NoAlarm;	  

	/* check general flags */
	// ����val->b��af��ֵ���õ�at��tte��̬��ʶ��ֵ
	if(val->b){
		if(af & TF_Off2On){
			at=AT_Off2On;
			tte->d.Value.Flags |= TF_Off2On;
		}
	}else{
		if(af & TF_On2Off){
			at=AT_On2Off;
			tte->d.Value.Flags |= TF_On2Off;
		}
	}
    
	// ��ǩ�Ķ�ֵ̬�Ƿ�ʱ����
	if(gf & TF_Valid){
		if(val->b ^ tte->d.Value.Value.b){
			tte->d.Value.Flags |= TF_Expired;  //���ڡ���ʱ?
		}
	}
	
	// ��ǩ�ı������Ե��жϣ��Ƿ�mask��ǩ����
	if(cutoff && at!=AT_NoAlarm){
		/* check cut off condition */
		if(_masked(tte->s.CutOffMask,cutoff->d.AlarmStatus)){
			/*
			when cutoff is in any alarm state indicated by
			tte->AlarmMask, the alarm will be _masked out
			*/
			alarm_masked=true;
		}
	}
    
	//
	if(alarm_masked) {
		return AT_NoAlarm;
	}

	return at;
}

/*
�������ܣ�this function is to generate an alarm packet on the network
          ���������ͱ���packet
����˵����
		gk -- group key of the tag
		tk -- rtk tag of the tag
		ac -- alarm class
		at -- alarm type
		val -- new value of the alarm, will be used as alarm parameter
����ֵ��0               ʧ��
        ���͵��ֽ���    �ɹ�
*/
static int _send_alarm(
	const GROUP_KEY * gk, 
	RTK_TAG * tte, 
	ALARM_CLASS ac,         //��������״̬���ࣺ������ȡ����ȷ��
	ALARM_TYPE at,          //��������״̬���߸ߡ��ߡ��͡��͵͡�����
	PCpmc_value_t val
	)
{
	int			res;
	char		buf[2048];
	RTK_ALARM_PACKET &ap = *((RTK_ALARM_PACKET *)buf);

	/*
		the line below indicates that this packet is from a PCMS server.
		in this version of PCMS daemon, the servers are unaware
		of what NodeKeys they are assigned, only clients know
		this information.
		So, setting the nodekey to a null string helps the codes who
		receive the alarm packets to identify if the incoming alarm
		is an Alarm(sent by server) or an alarm confirmation (by
		some clients)
	*/
	//1 �������ݱ�
	ap.Tag.node.Data[0] = 0;
	ap.Tag.sname.group = *gk;
	ap.Tag.sname.tag = tte->key;
	ap.Class = ac;                //��������״̬
	ap.Rank = tte->s.AlarmRank;   //��������
	ap.Value = val->Value;        //ʵʱֵ

	/* since Jul 11,2001, alarm is formatted into a message string */
	/*
		������Ϣ��ʽ��

		1 ��������
		ʱ�䣬���ƣ����ģ���������Ϣ��״̬,(����Ավ��)
		���б�����Ϣ����̬ȷ����OnMsg, OffMsg��

		2 ģ������
		ʱ�䣬����������ǰֵ���������ԣ��߸ߡ��ߡ��͡��͵͡����ʣ���
		����״̬��������������������,(����Ավ��)
	*/
	//2 ��������״̬
	const char * strCls= ac==AC_Alarm? "��������" : 
		ac==AC_CancelAlarm? "������ʧ" :
		ac==AC_AckAlarm? "ȷ��" : "";	 
	
	//3 ��ǩ����
	char * _name,name[128];
	if(0 && strlen(tte->s.Description) < 32){		
		memset(name,' ',sizeof(name));
		strncpy(name,tte->s.Description,strlen(tte->s.Description));
		name[sizeof(name) - 1] = 0;
		_name=name;
	}else{
		_name=tte->s.Description;
	}

	//ȡ�û���
	char * loginName = "";

	/*4 ����״̬��Ϣ��mox
	*/
	//����������״̬��Ϣ  
	if(get_value_type(tte->s.Flags) == dt_bool){
		const char *strAt;
		strAt = at == AT_DFault? "���ݲ���ʧ��" :
			at == AT_On2Off? tte->s.SwitchMsg.OffMsg : tte->s.SwitchMsg.OnMsg,
		sprintf(
		    ap.Msg,
		    "%s;%s;%s.%s.%s;%s;%s;%s;",
			texttime().data(),                           //��ǰʱ��
			_name,                                       //��ǩ����
			(char*)CHostName(g_ThisNode->key),           //�ڵ���
			(char*)CGroupName(tte->group),               //����
			(char*)CTagName(tte->key),                   //��ǩ��
			strCls,                                      //����״̬
			strAt,                                       //��/����Ϣ������
			""                                           //����վ
			//loginName                                  //����Ա
			);                                           
	//�������ͱ����ı���״̬��Ϣ		
	}else{
		const char * strAt;
		strAt=at==AT_HiHi? "�߸߱���":
		at==AT_Hi? "�߱���":
		at==AT_Lo? "�ͱ���":
		at==AT_LoLo? "�͵ͱ���":
		at==AT_SharpChange? "���ʱ���":
		at==AT_DFault? "���ݲ���ʧ��" :
		"";
		pmc_value_t valueStr;
		set_value_type(valueStr.Flags, dt_string);
		pmc_type_cast(val, &valueStr);
		sprintf(
			ap.Msg,
			"%s;%s;%s.%s.%s;%s;%s;%s;",
			texttime().data(),
			_name,
			(char*)CHostName(g_ThisNode->key),
			(char*)CGroupName(tte->group),
			(char*)CTagName(tte->key),
			strCls,                                                          //����״̬
		    strAt,                                                           //������Ϣ
			valueStr.Value.str? valueStr.Value.str : "(<Unknown>)"           //��ǰֵ
			//loginName                                                      //����Ա
			//tte->s.AnalogMsg.EU,//������
			);
		pmc_clear(&valueStr);
	}	 
    
	//���ͱ������ݱ�
	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);
#if 0
	//if(debug_level >= dbg_level_trace){
		utils_debug(
			"Raise alarm Tag:%s.%s,%s\n",
			(char *)CGroupName(*gk),
			(char *)CTagName(tte->key),
			ap.Msg
		);
	//}
#endif

	return res;
}

#define ALARM_FLAGS \
	(TF_SharpChange | TF_HiHi | TF_Hi | TF_Lo |TF_LoLo | TF_On2Off | TF_Off2On)

/*
	This function is called whenver a new value is
	written to a tag in the system database.
	
	It checks various conditions to see if an alarm
	is to be generated, if yes, the alarm is sent over
	the network, using the alarming channel, which is
	is multicast channel, otherwise nothing is done.

	This function will also determine if an exception
	is to be generated, and set the various flags,
	these flags will guide the IOSS to keep the
	distributed database synchronized.

	returns : 1 -- if alarm is raised
			  0 -- if no alarm is to be raised
	inputs	:
*/
/*
�������ܣ��õ�����״̬�����ͱ�����Ϣ�����ݱ�
����˵����
          gk    ����ָ��
		  tte   ��ǩʵʱֵ�ṹ��
		  val   �ӿ������õ����µ�ʵʱֵ
		  now   ʱ��
�� �� ֵ����������״̬���߸ߡ��ߡ��͡��͵͡����ʣ�
*/
int PMC_API get_exception(
	const GROUP_KEY * gk, 
	PRTK_TAG tte, 
	PCpmc_value_t val, 
	PCRTK_TIME now
	)
{
	int at,at_old;
	at_old = tte->d.AlarmStatus;
    
	//1 ��ն�ֵ̬�ı�ʶ
	// clear some flags, then we'll re-generate them if necessary
	tte->d.Value.Flags &= ~(ALARM_FLAGS | TF_Rate | TF_Expired);
	
	//2 �õ���ʱ״̬
	/* case 1, valid flag changes --> broadcast it */
	//val��tte->d��һ����Ч�����ʾtte->d��ʱ����
	if( (val->Flags & TF_Valid) ^ (tte->d.Value.Flags & TF_Valid)){
		tte->d.Value.Flags |= TF_Expired;
	}
    
	//����ʱ����ǰʱ�����ǩ��̬�����еĹ㲥ʱ��Ĳ�
	__r8 seconds = rtk_time_diff(now, &tte->d.BroadcastTime);
	//ʱ������g_fltTagLife/2��С��-g_fltTagLife/2�����ǩҲ�ǳ�ʱ�ġ�
	if((seconds > g_fltTagLife/2.) || (seconds < -g_fltTagLife/2.)){
		tte->d.Value.Flags |= TF_Expired;
	}
	
	//2 �õ�����״̬
	if(val->Flags & TF_Valid){
		switch(get_value_type(tte->s.Flags)){
		
	    //��������ǩ�ı�������
		case dt_bool:
			// most likely branch
			at=_get_exception_bool(tte, &val->Value);
			break;
        
		//�����ͱ�ǩ�ޱ���
		case dt_date:
			at = 0;
			break;
        
		//�������͵ı���
		default:		
			at=_get_exception_analog(tte, val, now);
			break;
		}
	}else{
		at = AT_DFault;
	}

	/* ok, set new alarm status */
	tte->d.AlarmStatus = at;

    //3 	
	if(!(tte->d.Value.Flags & TF_Shutdown)){		
			/* the same type of alarm will not be reported twice*/
	//	if(at != at_old){              //Zander,1.0.0.3
			if(at == AT_NoAlarm){
				/* cancel alarm */
				if((at_old != AT_DFault)&&(at!=at_old)){//Zander,1.0.0.3
					_send_alarm(gk, tte, AC_CancelAlarm, at_old, val);
				}
			}else{
                if(at != AT_DFault && at_old != at){  //�����ظ����� Zander2013-8-28
				//if(at != AT_DFault){
					_send_alarm(gk, tte, AC_Alarm, at, val);
				}else if(tte->s.Flags & TF_AlarmIfNA){
					_send_alarm(gk, tte, AC_Alarm, at, val);
				}
			}
	//	}   //Zander,1.0.0.3
	}else{
		if(tte->d.Value.Flags & TF_Valid){			
			tte->d.Value.Flags |= TF_Expired;		
		}
	}	

	return at;
}





