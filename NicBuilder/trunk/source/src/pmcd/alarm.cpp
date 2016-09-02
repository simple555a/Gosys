#include "precomp.h"

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
		"%d年%d月%d日 %02d:%02d:%02d",
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

static int on_off_line_broadcast(ALARM_CLASS ac)
{
	int res;	
	ALARM_TYPE	at=AT_WriteDevice;
	char __packet[MAX_PACKET_SIZE];
	RTK_ALARM_PACKET & ap=*((RTK_ALARM_PACKET*)__packet);

	memset(&ap,0,sizeof(ap));
	ap.Class=ac;

	/* since Jul 11,2001, alarm is formatted into a message string */
	/*
		报警信息格式：

		1 开关量：
		时间，名称（中文），类别，报警信息
		其中报警信息由组态确定（OnMsg, OffMsg）

		2 模拟量：
		时间，变量名，当前值，报警属性（高高、高、低、低低、速率），
		状态（报警、撤消）		  
	*/
	const char * strCls = ac==AC_Online?"上线广播":"离线广播";
	sprintf(ap.Msg,"%s;%s;站点:%s;%s", 
		texttime().data(), 
		_T(""),  
		(char*)CHostName(g_ThisNode->key),
		strCls 
		);
	
	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);

	if(debug_level >= dbg_level_trace){
		utils_debug("Raise alarm %s\n",
			ap.Msg
		);
	}

	return res;
}

int PMC_API online_broadcast()
{
	on_off_line_broadcast(AC_Online);	
	return 1;
}

int PMC_API offline_broadcast()
{
	on_off_line_broadcast(AC_Offline);
	return 1;
}

/*
  Raise the write-device alarm.
  This alarm will be multicasted on the network, thus enables the 
  logging system to log this event.
  Format of the write-device message:
  "时间;操作员站;变量;[值，单位 | OnMsg, OffMsg ];"
*/
int PMC_API write_device_alarm(
	const GROUP_KEY * gk, 
	const PRTK_TAG tte, 
	const TAG_VALUE * val, 
	const RTK_HOST * src
	)
{	 
	int res;
	ALARM_CLASS ac=AC_WriteDevice;
	char __packet[MAX_PACKET_SIZE];
	RTK_ALARM_PACKET & ap=*((RTK_ALARM_PACKET*)__packet);	 

	if(!gk){
		gk = &tte->group;
	}
	memset(&ap,0,sizeof(ap));
	/* format the packet*/

	ap.Tag.node.Data[0]=0;

	ap.Tag.sname.group=*gk;
	ap.Tag.sname.tag=tte->key;
	ap.Class=ac;
	ap.Rank=ALARM_WRITE_DEVICE_RANK;
	ap.Value=*val;

	/* since Jul 11,2001, alarm is formatted into a message string */
	/*
		报警信息格式：

		1 开关量：
		时间，名称（中文），报警信息，状态,操作员站：
		其中报警信息由组态确定（OnMsg, OffMsg）

		2 模拟量：
		时间，变量名，当前值，报警属性（高高、高、低、低低、速率），
		状态（报警、撤消、操作）
	*/
	//
	char * _name,name[33];
	if(0 && strlen(tte->s.Description) < 32){		
		memset(name,' ',sizeof(name));
		strncpy(name,tte->s.Description,strlen(tte->s.Description));
		name[sizeof(name) - 1] = 0;
		_name=name;
	}else{
		_name=tte->s.Description;
	}
	if(get_value_type(tte->s.Flags) == dt_bool)
	{
		//开关量
		sprintf(
			ap.Msg,
			"%s;%s;%s.%s.%s;%s;%s;%s;%s;",
			texttime().data(),
			_name,//描述
			(char*)CHostName(g_ThisNode->key),
			(char*)CGroupName(tte->group),
			(char*)CTagName(tte->key),
			"",//报警状态，
			val->iValue ? tte->s.SwitchMsg.OnMsg : tte->s.SwitchMsg.OffMsg,//操作信息
			"",//src->Data,//工作站
			""//strCls//操作员
			);
	}
	else
	{
		sprintf(
			ap.Msg,
			"%s;%s;%s.%s.%s;%s;%s->%s;%s;%s;",
			texttime().data(),
			_name,
			(char*)CHostName(g_ThisNode->key),
			(char*)CGroupName(tte->group),
			(char*)CTagName(tte->key),
			"",//
			"",//strAt1,
			"",//strAt2,//操作信息
			src->Data,//工作站
			""//strCls//操作员
			);
	}	 

	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		(RTK_PACKET*)&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);

	if(debug_level >= dbg_level_trace){
		utils_debug(
			"Raise alarm Tag:%s.%s,%s\n",
			(char *)CGroupName(*gk),
			(char *)CTagName(tte->key),
			ap.Msg
			);
	}

	return res;
}

string getIp()
{
	char name[256];
	gethostname(name, sizeof(name));
	name[255] = 0;
	return name;
}

int PMC_API power_state_change_alarm(int newState)
{
	int res;	
	int ac;
	ALARM_TYPE at;
	char __packet[MAX_PACKET_SIZE];
	RTK_ALARM_PACKET & ap=*((RTK_ALARM_PACKET*)__packet);
	char * msg;

	memset(&ap,0,sizeof(ap));
	ap.Class = ac = AC_PowerSwitch;
	at = (newState == PWR_RUNNING? AT_SwitchPrimary : AT_SwitchBackup);
	const char * strCls=at==AT_SwitchPrimary?"进入主站模式":"进入备站模式";
	msg = ap.Msg;
	msg += sprintf(msg,"%s;%s;站点:%s", strCls, texttime().data(), 
		(char*)CHostName(g_ThisNode->key)
		);
	VBUS_DATA d;
	d.size = sizeof(d);
	get_vbus_data(vbus_system, &d);
	msg += sprintf(msg, "(host=%s, guid=", getIp().c_str());
	msg += d.serverGuid.to_string(msg);
	msg += sprintf(msg, ")");
	
	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);
	
	if(debug_level >= dbg_level_trace){
		utils_debug("Raise alarm %s\n",
			ap.Msg
		);
	}
	
	return res;
}

int PMC_API write_device_alarm_y(
	const GROUP_KEY * gk, 
	const TAG_VALUE * val, 
	const RTK_HOST * src,
	const TAG_KEY * operName
	)
{	 
	int res;
	ALARM_CLASS ac=AC_WriteDevice;
	char __packet[MAX_PACKET_SIZE];
	RTK_ALARM_PACKET & ap=*((RTK_ALARM_PACKET*)__packet);	 
	ap.Class=ac;

	//iValue的值2为登陆，3为注销，4为系统启动，5为系统关闭，
	int temp = val->iValue; 
	sprintf(
		ap.Msg,
		"%s;%s;%s%s%s;%s;%s;%s;%s;",
		texttime().data(),
		temp%2 ? (temp%5 ? _T("用户注销") : _T("系统关闭")) : (temp%4 ? _T("用户登陆") : _T("系统启动")),//描述
		"",//点名第一节，
		"",//点名第二节，
		"",//点名第三节，
		"",//报警状态
		"",//操作信息（开消息/关消息）
		src->Data,//工作站
		operName->Data//操作员
		);

	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		(RTK_PACKET*)&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);

	return res;
}

int PMC_API write_device_alarm_z(
	const GROUP_KEY * gk, 
	const PRTK_TAG tte, 
	const TAG_VALUE * val, 
	const RTK_HOST * src,
	char * operName
	)
{	 
	int res;
	ALARM_CLASS ac=AC_WriteDevice;
	char __packet[MAX_PACKET_SIZE];
	RTK_ALARM_PACKET & ap=*((RTK_ALARM_PACKET*)__packet);	 

	if(!gk){
		gk = &tte->group;
	}
	memset(&ap,0,sizeof(ap));
	/* format the packet*/

	ap.Tag.node.Data[0]=0;

	TAG_VALUE pp =tte->d.Value.Value;
	ap.Tag.sname.group=*gk;
	ap.Tag.sname.tag=tte->key;
	ap.Class=ac;
	ap.Rank=ALARM_WRITE_DEVICE_RANK;
	ap.Value=*val;

	/* since Jul 11,2001, alarm is formatted into a message string */
	/*
		报警信息格式：

		1 开关量：
		时间，名称（中文），报警信息，状态,操作员站：
		其中报警信息由组态确定（OnMsg, OffMsg）

		2 模拟量：
		时间，变量名，当前值，报警属性（高高、高、低、低低、速率），
		状态（报警、撤消、操作）*/
	
	char * _name,name[33];
	if(0 && strlen(tte->s.Description) < 32){		
		memset(name,' ',sizeof(name));
		strncpy(name,tte->s.Description,strlen(tte->s.Description));
		name[sizeof(name) - 1] = 0;
		_name=name;
	}else{
		_name=tte->s.Description;
	}


	//
	if(get_value_type(tte->s.Flags) == dt_bool)
	{
		//开关量
		sprintf(
			ap.Msg,
			"%s;%s;%s.%s.%s;%s;%s;%s;%s;",
			texttime().data(),
			_name,//描述
			(char*)CHostName(g_ThisNode->key),
			(char*)CGroupName(tte->group),
			(char*)CTagName(tte->key),
			"",//报警状态，
			val->iValue ? tte->s.SwitchMsg.OnMsg : tte->s.SwitchMsg.OffMsg,//操作信息
			src->Data,//工作站
			operName//操作员
			);
	}
	else
	{
		//模拟量
		sprintf(
			ap.Msg,
			"%s;%s;%s.%s.%s;%s;%d->%d;%s;%s;",
			texttime().data(),
			_name,
			(char*)CHostName(g_ThisNode->key),
			(char*)CGroupName(tte->group),
			(char*)CTagName(tte->key),
			"",//
			tte->d.Value.Value.iValue,//实时值
			ap.Value.iValue,//操作员置值//操作信息
			src->Data,//工作站
			operName//操作员
			);
	}	 

	res = broadcast_rtk_data(
		vbus_alarm, 
		PORT_ALL_CLIENT, 
		PT_Alarm,
		(RTK_PACKET*)&ap, 
		sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg)+1
		);

	if(debug_level >= dbg_level_trace){
		utils_debug(
			"Raise alarm Tag:%s.%s,%s\n",
			(char *)CGroupName(*gk),
			(char *)CTagName(tte->key),
			ap.Msg
			);
	}

	return res;
}