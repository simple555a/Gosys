//#pragma warning(disable:4244)
#include "precomp.h"

inline bool isInteger(__u32 flags)
{
	__u8 dt = get_value_type(flags);
	return dt==dt_int8 || dt==dt_int16 || dt==dt_int32	|| dt==dt_int64 ||		
		dt==dt_uint8 || dt==dt_uint16 || dt==dt_uint32 || dt==dt_uint64;
}

inline bool isReal(__u32 flags)
{
	__u8 dt = get_value_type(flags);
	return dt==dt_real4 || dt==dt_real8;
}

inline bool isBool(__u32 flags)
{
	__u8 dt = get_value_type(flags);
	return dt == dt_bool;
}

static void _trim(string & s, string & d = string(" "))
{
	int a,b;

	a = s.find_first_not_of(d);
	b = s.find_last_not_of(d);

	if(a == string::npos && b == string::npos) {
		return;
	}

	if(a==string::npos){
		a = 0;
	}
	
	if (b==string::npos){
		b = s.length();
	}

	s = s.substr(a, b-a+1);
}

RTDB_API int PMC_API string_by_mask(__uint att, char * nn)
{
	*nn=0;
#define __check_field__(field,name) \
	if(att & TF_##field) {\
		if(*nn) {\
			strcat(nn,"+");\
			strcat(nn,#name);\
		}else{\
			strcpy(nn,#name);\
		}\
	}
	__check_field__(HiHi,h2)
	__check_field__(Hi,h)
	__check_field__(LoLo,l2)
	__check_field__(Lo,l)
	__check_field__(Rate,r)
	__check_field__(On2Off,off)
	__check_field__(Off2On,on)
	__check_field__(SaveToHistory,his)
	__check_field__(Step, step)
#undef __check_field__
	return strlen(nn);
}

RTDB_API __uint PMC_API mask_by_string(char * nn)
{
	parse_arg_1(nn,"|+ ");
/*
	bool bHiHi:1;
	bool bHi:1;
	bool bLo:1;
	bool bLoLo:1;
	bool bRate:1;
	bool bOn2Off:1;
	bool bOff2On:1;
*/
	__u32 att;
	int i;
	memset(&att, 0, sizeof(att));
	for(i=0;i<utils_argc;i++){
#define __check_field__(field,name) \
		if(!strcmpi(utils_argv[i],#name)) {\
			att |= TF_##field;\
		}
		__check_field__(HiHi,h2)
		else __check_field__(Hi,h)
		else __check_field__(LoLo,l2)
		else __check_field__(Lo,l)
		else __check_field__(Rate,r)
		else __check_field__(On2Off,off)
		else __check_field__(Off2On,on)
		else __check_field__(SaveToHistory, his)
		else __check_field__(Step, step)
	}
#undef __check_field__
	return att;
}

RTDB_API void PMC_API tag_trans_error(int err)
{
	switch(err){
	case -3:
		utils_error("Syntax error(not enough fields)");
		break;
	case -4:
		utils_error("Syntax error(void key).");
		break;
	case -5:
		utils_error("Syntax error(invalid cutoff tagname).");
		break;
	case -6:
		utils_error(
			"Syntax error(key too long, %d max)",
			rtkm_tag_key_length
			);
		break;
	case -7:
		utils_error("Syntax error(invalid TagType)");
		break;
	case -8:
		utils_error("Syntax error(AlarmDeadBand error)");
		break;
	}
}

RTDB_API int PMC_API tag_by_textW(
	PRTK_TAG _tte, 
	const wchar_t * strLineBuf, 
	PCGROUP_KEY gk
	)
{
	USES_CONVERSION;
	int retVal;
	retVal = tag_by_textA(_tte, W2A(strLineBuf), gk);
	return retVal;
}

RTDB_API int PMC_API tag_by_text2W(
	PRTK_TAG _tte, 
	const wchar_t * strLineBuf, 
	PCGROUP_KEY gk
	)
{
	USES_CONVERSION;
	int retVal;
	retVal = tag_by_text2A(_tte, W2A(strLineBuf), gk);
	return retVal;
}

/*
	Check if a tag definition line is valid;
	returns:
	0	--- ok	
	-1	--- void line
	-2	--- comment line
	-3	--- insufficient fields
	-4	--- void key
	-5	--- invalid cutoff name
	-6	--- invalid tagkey
	-7	--- invalid tag type
	-8	--- invalid dead band 
*/
RTDB_API int PMC_API tag_by_textA(
	PRTK_TAG _tte, 
	const char * strLineBuf, 
	PCGROUP_KEY gk
	)
{
	string strLine,element,key;
	CArgs args;
	int idx;
	TAG_TYPE type;
	RTK_TAG &tte = *_tte;

	// strupr(strLineBuf);
	strLine=strLineBuf;
	
	// skip blanks and Tabs
	idx=strLine.find_first_not_of(" \t\n");
	if(idx == string::npos) {
		return -1;
	}else if(idx != 0){
		strLine=strLine.substr(idx);
	}
	// skip void line
	if(strLine.length() == 0)  {
		return -1;
	}
	
	// skip comments line
	if(strLine.data()[0] == '#') {
		return -2;
	}
	
	/* 
		ok, this looks like a valid tag definition line, process it
		the line is expected to be :
		
		Key,Type,s.Flags,AlarmRank,fltAlarmDeadband,
		fltHiHi,fltHi,fltLo,fltLoLo,fltRate,
		CutOffTagName,CutOffMask,fltMinValue,fltMaxValue,
		OnMsg,OffMsg,EU,Description,Device,Address

		if the line has more than 19 fields, then we expect
		it to be a version 3.5 entry, with the following fields:

		ExcDev; ExcDevPercent; ExcMax; 
		Access; Owner;
		DisplayDigits;
		Compressing; CompMax; CompDev; CompDevPercent;
	*/
	
	// here parse_arg_ey instead of parse_arg_ex is used
	// because some optional fields might be missing, which cannot 
	// be detected by parse_arg_ex
	parse_arg_ey(strLineBuf, &args, ",\t\r\n");
	
	if(args.argc < 20){
		return -3;
	}else if(args.argc > 22 && args.argc < 27){
		return -3;
	}
	
	idx=0;
	memset(&tte, 0, sizeof(RTK_TAG));

	// extract key
	key=args.argv[idx++];
	_trim(key);
	if(key.length()==0){		
		return -4;
	}
	if(key.length()>rtkm_tag_key_length){
		return -6;
	}		
	tte.key = CTagName(key.data());
	
	// extract Type
	element=args.argv[idx++];
	_trim(element);
	if(!strcmpi(element.data(),"b")){
		type = dt_bool;
	}else if(!strcmpi(element.data(),"f")){
		type = dt_real4;
	}

#define __t(tpname, t)\
	else if(!strcmpi(element.data(), #tpname)){\
		type = dt_##t;\
	}

#define __st(t) __t(t, t)

	__st(bool)
	__st(real4)
	__st(real8)
	__st(int8)
	__st(int16)
	__st(int32)
	__st(int64)
	__st(uint8)
	__st(uint16)
	__st(uint32)
	__st(uint64)
	__st(date)
	__t(int, int32)
	__t(float, real4)
	__t(double, real8)

#undef __st
#undef __t

	// extract s.Flagss
	tte.s.Flags = mask_by_string(args.argv[idx++]);

	// set type, note this must be done after the Flags is set 
	// because the type field and flags are sharing a 32-bit integer
	set_value_type(tte.s.Flags, type);

	// extract AlarmRanks
	tte.s.AlarmRank=atoi(args.argv[idx++]);

	switch(get_value_type(tte.s.Flags)){
	case dt_real4:
	case dt_real8:
	case dt_int64:
	case dt_uint64:
		// extract fltAlarmDeadBand
		element=args.argv[idx++];
		tte.s.fltAlarmDeadband=(__r4)atof(element.data());
		if(tte.s.fltAlarmDeadband<0.001){
			// return -8;
		}

		// extrace fltHiHi,fltHi,fltLo,fltLoLo,
		tte.s.fltHiHi=(__r4)atof(args.argv[idx++]);
		tte.s.fltHi=(__r4)atof(args.argv[idx++]);
		tte.s.fltLo=(__r4)atof(args.argv[idx++]);
		tte.s.fltLoLo=(__r4)atof(args.argv[idx++]);
		tte.s.fltRate=(__r4)atof(args.argv[idx++]);
		break;
	case dt_int8:
	case dt_int16:
	case dt_int32:
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		// extract fltAlarmDeadBand
		element=args.argv[idx++];
		tte.s.u_deadband.i = atoi(element.data());
		// extrace fltHiHi,fltHi,fltLo,fltLoLo,
		tte.s.u_hihi.i = atoi(args.argv[idx++]);
		tte.s.u_hi.i = atoi(args.argv[idx++]);
		tte.s.u_lo.i = atoi(args.argv[idx++]);
		tte.s.u_lolo.i = atoi(args.argv[idx++]);
		tte.s.fltRate = (__r4)atoi(args.argv[idx++]);
		break;
	case dt_bool:
	case dt_date:
		idx+=6;
	}

	// extract CutOffTagName
	// this field should be written as : " GroupName.TagName "
	// GroupName can be defaulted to the same group
	{
		element=args.argv[idx++];
		_trim(element);
		bool bValid;
		if(element.length() != 0){
			CArgs _args;
			parse_arg_ex((char *)element.data(),&_args," .");
			char _g[rtkm_group_key_length+1],*t,*g;
			switch(_args.argc){
			case 1:
				// tag in the same group assumed
				strncpy(_g,gk->Data,rtkm_group_key_length);
				_g[sizeof(_g) - 1] = 0;
				g=_g;
				t=_args.argv[0];
				bValid=true;
				break;
			case 2:
				g=_args.argv[0];
				t=_args.argv[1];
				bValid=true;
				break;
			case 0:
				g=t="";
				bValid = true;
				break;
			default:
				bValid=false;
				break;
			}; // switch(_args.argc)
			if(bValid){
				if(strlen(g) > rtkm_group_key_length 
					|| strlen(t) > rtkm_tag_key_length 
				){
					return -5;
				}
				tte.s.CutOffTagName.group = CGroupName(g);
				tte.s.CutOffTagName.tag = CTagName(t);
			}else{
				utils_error("Warning : invalid cutoff : %s", element.data());
			}
		};// if(element.length() != 0){
		// extract CutOffMask
		tte.s.CutOffMask = mask_by_string(args.argv[idx++]);
	}

	switch(get_value_type(tte.s.Flags)){
		case dt_bool:
		case dt_date:
			// skip fields
			idx += 2;
			break;
			
		case dt_int8:
		case dt_int16:
		case dt_int32:
		case dt_uint8:
		case dt_uint16:
		case dt_uint32:
			tte.s.u_min.i = atoi(args.argv[idx++]);
			tte.s.u_max.i = atoi(args.argv[idx++]);
			break;
			
		case dt_real4:
		case dt_real8:
		case dt_int64:
		case dt_uint64:
			// fltMinValue,fltMaxValue
			tte.s.fltMinValue = (__r4)atof(args.argv[idx++]);
			tte.s.fltMaxValue = (__r4)atof(args.argv[idx++]);
			break;
	}

	switch(get_value_type(tte.s.Flags)){
	case dt_int8:
	case dt_int16:
	case dt_int32:
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
	case dt_real4:
	case dt_real8:
	case dt_int64:
	case dt_uint64:
		// skip onMsg and OffMsg
		idx+=2;
		strncpy(tte.s.AnalogMsg.EU,args.argv[idx++],rtkm_eu_length);
		tte.s.AnalogMsg.EU[rtkm_eu_length - 1] = 0;
		break;
	case dt_bool:
		strncpy(tte.s.SwitchMsg.OnMsg,args.argv[idx++],rtkm_onmsg_length);
		tte.s.SwitchMsg.OnMsg[rtkm_onmsg_length - 1] = 0;
		strncpy(tte.s.SwitchMsg.OffMsg,args.argv[idx++],rtkm_onmsg_length);
		tte.s.SwitchMsg.OffMsg[rtkm_onmsg_length - 1] = 0;
		// skip EU
		idx++;
		break;
	default:
		// skip onMsg, offMsg, analogMsg
		idx += 3;
	}

	strncpy(tte.s.Description,args.argv[idx++], rtkm_description_length);
	tte.s.Description[rtkm_description_length - 1] = 0;
	
	tte.s.Device = CDeviceName(args.argv[idx++]);
	{
		char * p;
		p = args.argv[idx++];
		if(*p == '"'){
			p++;
		}
		strncpy(tte.s.Address, p, rtkm_tag_address_length);
		tte.s.Address[sizeof(tte.s.Address) - 1] = 0;
		p = tte.s.Address + strlen(tte.s.Address) - 1;
		if(*p == '"'){
			*p = 0;
		}
	}

	if(args.argc >= 27){
		tte.s.ExcDev = (__r4)atof(args.argv[idx++]);
		tte.s.ExcMax = atoi(args.argv[idx++]);
		__u32 access;
		sscanf(args.argv[idx++], "%x", &access);
		tte.s.Access = access;
		tte.s.Owner = 0;
		idx++;
		tte.s.DisplayDigits = atoi(args.argv[idx++]);
		
		if(!stricmp(args.argv[idx], "none")){
			tte.s.Compressing = Compress_None;
		}else if(!stricmp(args.argv[idx], "const")){
			tte.s.Compressing = Compress_Const;
		}else if(!stricmp(args.argv[idx], "linear")){
			tte.s.Compressing = Compress_Linear;
		}else{
			tte.s.Compressing = Compress_Const;
		}
		idx++;

		tte.s.CompMax = atoi(args.argv[idx++]);
		tte.s.CompDev = (__r4)atof(args.argv[idx++]);
	}

	return validate_tag(_tte);
}

/*
	new format tag-text parser
	the tag declaration is now xml-style
	e.g.
	
	TAG_DECLARATION : '<' 'tag' ATTRIBUTE_LIST '>'
	
	ATTRIBUTE_LIST : ATTRIBUTE SEPERATOR ATTRIBUTE_LIST | 
	
	ATTRIBUTE : ATTRIBUTE_NAME '=' ATTRIBUTE_VALUE
	
	ATTRIBUTE_NAME : 
		'Key' | 
		'Type' |
		'StaticFlags' | 
		'AlarmRank' | 'AlarmDeadBand' |
		'HiHi' | 'Hi' | 'Lo' | 'LoLo' | 'Rate' |
		'CutoffTag' | 'CurtoffMask' |
		'Min' | 'Max' |
		'OnMsg' | 'OffMsg' | 'EU' | 'Description' |
		'Device' |	'Address' |
		'ExcDev' | 'ExcDevPercent' | 'ExcMax' |
		'DisplayDigits' |
		'Owner' | 'Access' |
		'Archiving' | 'Compressing' | 'CompMax' | 'CompDev' | 'CompDevPercent'
		
	ATTRIBUTE_VALUE : '"' STRING '"'

	SEPERATOR : ';' | ' ' | '\t' | '\n' | '\r'
*/
// BUGBUG: Source incomplete
RTDB_API int PMC_API tag_by_text2A(
	PRTK_TAG _tte, 
	const char * strLineBuf, 
	PCGROUP_KEY gk
	)
{
	CArgs args;
	int i;
	char * attrName, * attrValue;
	string a;
	string equalSign = string("\"");
	
	parse_arg_ex(strLineBuf, &args, "; \t\n\r");

	if(args.argc < 2){
		return -1;
	}
	
	if(stricmp(args.argv[0], "tag")){
		return -2;
	}

	memset(_tte, 0, sizeof(RTK_TAG));
	for(i = 1; i<args.argc; i++){
		attrName = strtok(args.argv[i], "=");
		if(!attrName){
			continue;
		}
		attrValue = attrName + strlen(attrName) + 1;
		// remove leading and trailing '"'
		a = attrValue;
		_trim(a, equalSign);
		
		// assign fields
		
		if(!stricmp("Key", attrName)){
			_tte->key = CTagName(a.c_str());
		}else if(!stricmp("Type", attrName)){
		}else if(!stricmp("StaticFlags", attrName)){
		}else if(!stricmp("AlarmRank", attrName)){
		}else if(!stricmp("AlarmDeadBand", attrName)){
		}else if(!stricmp("HiHi", attrName)){
		}else if(!stricmp("Hi", attrName)){
		}else if(!stricmp("Lo", attrName)){
		}else if(!stricmp("LoLo", attrName)){
		}else if(!stricmp("CutoffTag", attrName)){
		}else if(!stricmp("CutoffMask", attrName)){
		}else if(!stricmp("Min", attrName)){
		}else if(!stricmp("Max", attrName)){
		}else if(!stricmp("OnMsg", attrName)){
		}else if(!stricmp("OffMsg", attrName)){
		}else if(!stricmp("EU", attrName)){
		}else if(!stricmp("Description", attrName)){
		}else if(!stricmp("Device", attrName)){
		}else if(!stricmp("Address", attrName)){
		}else if(!stricmp("ExDev", attrName)){
		}else if(!stricmp("ExcDevPercent", attrName)){
		}else if(!stricmp("ExcMax", attrName)){
		}else if(!stricmp("DisplayDigits", attrName)){
		}else if(!stricmp("Owner", attrName)){
		}else if(!stricmp("Access", attrName)){
		}else if(!stricmp("Compressing", attrName)){
		}else if(!stricmp("CompMax", attrName)){
		}else if(!stricmp("CompDev", attrName)){
		}else if(!stricmp("CompDevPercent", attrName)){
		}else{
		}
	}
	
	return 0;
}

RTDB_API int PMC_API tag_to_text2A(
	char *output, 
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	)
{
	char * p = output;
	__u8 dt;
	
	dt = get_value_type(s->Flags);
	output += sprintf(output, "Key=%s,", (char *)CTagName(*key));
	output += sprintf(output, "Type=%s,", 
		dt==dt_bool? "bool" :
		dt==dt_int16? "int16" :
		dt==dt_int32? "int32" :
		dt==dt_real4? "real4" :
		dt==dt_real8? "real8" :
		dt==dt_date? "date" :
		dt==dt_uint16? "uint16" :
		dt==dt_uint32? "uint32" :
		dt==dt_int8? "int8" :
		dt==dt_uint8? "uint8":
		dt==dt_string? "string" :
		dt==dt_bstr? "bstr":
		"N.A.");
	output += sprintf(output, "Flags=");
	output += string_by_mask(s->Flags, output);
	output += sprintf(output, ",AlarmRank=%d,", s->AlarmRank);
	if(isReal(s->Flags)){
		output += sprintf(output, "HiHi=%f,", s->u_hihi.flt);
		output += sprintf(output, "Hi=%f,", s->u_hi.flt);
		output += sprintf(output, "Lo=%f,", s->u_lo.flt);
		output += sprintf(output, "LoLo=%f,", s->u_lolo.flt);
		output += sprintf(output, "Min=%f,", s->u_min.flt);
		output += sprintf(output, "Max=%f,", s->u_max.flt);
	}else if(isInteger(s->Flags)){
		output += sprintf(output, "HiHi=%f,", s->u_hihi.i);
		output += sprintf(output, "Hi=%f,", s->u_hi.i);
		output += sprintf(output, "Lo=%f,", s->u_lo.i);
		output += sprintf(output, "LoLo=%f,", s->u_lolo.i);
		output += sprintf(output, "Min=%f,", s->u_min.i);
		output += sprintf(output, "Max=%f,", s->u_max.i);
	}
	if(isBool(s->Flags)){
		output += sprintf(
			output, 
			"OnMsg='%s',OffMsg='%s',", 
			s->SwitchMsg.OnMsg,
			s->SwitchMsg.OffMsg
			);
	}else{
		output += sprintf(output, "EU='%s',", s->AnalogMsg.EU);
	}
	output += sprintf(output, "Desc='%s',", s->Description);
	output += sprintf(output, "Device='%s',", 
		(char *)CAlphaName<DEVICE_KEY>(s->Device)
		);
	output += sprintf(output, "Address='%s',", s->Address);

	return output - p;
}

RTDB_API int PMC_API tag_to_text2W(
	wchar_t *output, 
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	)
{
	char buf[2048];
	int retVal;
	USES_CONVERSION;
	retVal = tag_to_text2A(buf, key, s);
	wcscpy(output, A2W(buf));
	return retVal;
}

RTDB_API __bool PMC_API tagname_by_textW(
	TAG_NAME * tn, 
	const wchar_t * name
	)
{
	__bool retVal;
	USES_CONVERSION;
	retVal = tagname_by_textA(tn, W2A(name));
	return retVal;
}

RTDB_API __bool PMC_API tagname_by_textA(
	TAG_NAME * tn,
	const char * name
	)
{
	CArgs args;
	parse_arg_ey((char*)name,&args,".");
	memset(tn,0,sizeof(*tn));
	if(args.argc == 3) {
		if(strlen(args.argv[0]) > rtkm_node_key_length ||
		   strlen(args.argv[1]) > rtkm_group_key_length	  ||
		   strlen(args.argv[2]) > rtkm_tag_key_length	
		){
			return __false;
		};
		// strupr(args.argv[0]);
		// strupr(args.argv[1]);
		// strupr(args.argv[2]);
		tn->node = CNodeName(args.argv[0]);
		tn->sname.group = CGroupName(args.argv[1]);
		tn->sname.tag = CTagName(args.argv[2]);
	}else if(args.argc == 2){
		if(strlen(args.argv[0]) > rtkm_group_key_length	  ||
		   strlen(args.argv[1]) > rtkm_tag_key_length	
		){
			return __false;
		}
		tn->node = CNodeName(RTK_DEFAULT_NODE_KEY);
		tn->sname.group = CGroupName(args.argv[0]);
		tn->sname.tag = CTagName(args.argv[1]);
	}else if(args.argc == 1){
		if(strlen(args.argv[0]) > rtkm_tag_key_length){
			return __false;
		};
		tn->node = CNodeName(RTK_DEFAULT_NODE_KEY);
		tn->sname.group = CGroupName(RTK_DEFAULT_GROUP_KEY);
		tn->sname.tag = CTagName(args.argv[0]);
	}else{
		return __false;
	}
	return __true;
}

RTDB_API int PMC_API dbg_random_tag(
	PTAG_NAME tn, 
	PSTATIC_TAG_DATA s
	)
{
	/* 
		Key,Type,s.Flags,AlarmRank,fltAlarmDeadband,
		fltHiHi,fltHi,fltLo,fltLoLo,fltRate,
		CutOffTagName,CutOffMask,fltMinValue,fltMaxValue,
		OnMsg,OffMsg,EU,Description
	*/
	static int key=0;
	static int pno=1,dno=1,cno=1;
	char type;
	
	memset(s,0,sizeof(*s));

	sprintf(tn->node.Data,"s0");
	sprintf(tn->sname.group.Data,"g1");
	sprintf(tn->sname.tag.Data,"t%04d",key);

	if(rng_rand(0,4)>1){
		set_value_type(s->Flags, dt_bool);
		type='b';
	}else{	  
		set_value_type(s->Flags, dt_real4);
		type='f';
	}	 

	// s.Flags
	if(type=='b'){
		if(rng_rand(0,2))	s->Flags |= TF_On2Off;
		if(rng_rand(0,2))	s->Flags |= TF_Off2On;
	}else{
		if(rng_rand(0,2))	s->Flags |= TF_HiHi;
		if(rng_rand(0,2))	s->Flags |= TF_Hi;
		if(rng_rand(0,2))	s->Flags |= TF_Lo;
		if(rng_rand(0,2))	s->Flags |= TF_LoLo;
		if(rng_rand(0,2))	s->Flags |= TF_Rate;
	}
	
	// AlarmRanks
	s->AlarmRank=rng_rand(0,100);
	if(type=='f'){
		// fltAlarmDeadBand
		s->fltAlarmDeadband=(__r4)(rng_rand(1,5)*0.01);
		// fltHiHi,fltHi,fltLo,fltLoLo,
		s->fltHiHi=90;
		s->fltHi=80;
		s->fltLo=20;
		s->fltLoLo=10;		  
		s->fltRate=(float)0.1;
	};
	// extract CutOffTagName
	// extract CutOffMask
	// fltMinValue,fltMaxValue
	if(type=='f'){
		s->fltMinValue=0;
		s->fltMaxValue=100;
	}
	if(type=='b'){		  
		strcpy(s->SwitchMsg.OnMsg,"开关量开");
		strcpy(s->SwitchMsg.OffMsg,"开关量关");
		sprintf(s->Description,"开关量 No.%d",key);
	}else{
		char * eu[]={"千克","兆帕","KV"};
		strcpy(s->AnalogMsg.EU,eu[rng_rand(0,sizeof(eu)/sizeof(eu[0]))]);
		sprintf(s->Description,"模拟量 No.%d",key);
	}
	
#if 0
	char *adds[]={
	"上地南里8号院",
	"东王庄",
	"清华15＃426",
	"万人大食堂",
	"北大36＃130",
	"万人大水坑",
	"白石桥"
	};
	strcpy(s->Address,adds[rng_rand(0,sizeof(adds)/sizeof(adds[0]))]);
#endif
	s->Device = CDeviceName("$DEVICE");
	sprintf(s->Address,"%d.%d.%d",pno,dno,cno);
	if(++cno >= 31){
		cno=1;
		if(++dno >= 99){
			dno=1;
			pno+=2;
			if(pno >= 16){
				pno=1;
			}
		}
	}

	key++;
	return 1;
}

RTDB_API int PMC_API tag_to_textA(
	char *output, 
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	)
{
	char *p;
	/*
		Key,Type,s.Flags,AlarmRank,fltAlarmDeadband,
		fltHiHi,fltHi,fltLo,fltLoLo,fltRate,
		CutOffTagName,CutOffMask,fltMinValue,fltMaxValue,
		OnMsg,OffMsg,EU,Description
	*/
	char buf[128];
	p = output;
	// write key
	buf[rtkm_tag_key_length]=0;
	memcpy(buf,key->Data,rtkm_tag_key_length);
	p += sprintf(p, "%s,",buf);

	// write type
	switch(get_value_type(s->Flags)){

#define __t(t, tname)\
	case t:\
		p += sprintf(p, "%s,", #tname);\
		break;
	
#define __st(t) __t(dt_##t, t)

	__st(bool)
	__st(real4)
	__st(real8)
	__st(int8)
	__st(int16)
	__st(int32)
	__st(int64)
	__st(uint8)
	__st(uint16)
	__st(uint32)
	__st(uint64)
	__st(date)
	
	}
	
	// extract s.Flags	
	string_by_mask(s->Flags, buf);
	p += sprintf(p, "%s,",buf);
	
	// extract AlarmRanks
	p += sprintf(p, "%d,",s->AlarmRank);
	
	switch(get_value_type(s->Flags)){
	case dt_real4:
	case dt_real8:
	case dt_int64:
	case dt_uint64:
		// extract fltAlarmDeadBand
		p += sprintf(p, "%g,",s->fltAlarmDeadband);
		// extrace fltHiHi,fltHi,fltLo,fltLoLo,
		p += sprintf(
			p, 
			"%g,%g,%g,%g,",
			s->fltHiHi,
			s->fltHi,
			s->fltLo,
			s->fltLoLo
			);
		//tte.fltRate=atof(args.argv[idx++]);
		p += sprintf(p, "%g,",s->fltRate);
		break;
		
	case dt_int8:
	case dt_int16:
	case dt_int32:
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		// extract fltAlarmDeadBand
		p += sprintf(p, "%d,", s->u_deadband.i);
		// extrace fltHiHi,fltHi,fltLo,fltLoLo,
		p += sprintf(
			p, 
			"%d,%d,%d,%d,", 
			s->u_hihi.i, 
			s->u_hi.i, 
			s->u_lo.i, 
			s->u_lolo.i
			);
		//tte.fltRate=atof(args.argv[idx++]);
		p += sprintf(p, "%g,",s->fltRate);
		break;
	default:
	/*
	case dt_bool:
	case dt_date:
	*/
		p += sprintf(p, ",,,,,,");
		break;
	}
	
	// extract CutOffTagName
	// extract CutOffMask
	p += sprintf(p, "%s.%s,",
		(char*)CGroupName(s->CutOffTagName.group),
		(char*)CTagName(s->CutOffTagName.tag)
		);
	
	string_by_mask(s->CutOffMask,buf);
	p += sprintf(p, "%s,",buf);
	
	// fltMinValue,fltMaxValue
	switch(get_value_type(s->Flags)){
	case dt_real4:
	case dt_real8:
	case dt_int64:
	case dt_uint64:
		p += sprintf(p, "%g,%g,",s->fltMinValue,s->fltMaxValue);
		p += sprintf(p, ",,%s,%s,",s->AnalogMsg.EU,s->Description);
		break;
	case dt_date:
		p += sprintf(p, ",,");
		p += sprintf(p, ",,,%s,", s->Description);
		break;
	case dt_int8:
	case dt_int16:
	case dt_int32:
	case dt_uint8:
	case dt_uint16:
	case dt_uint32:
		p += sprintf(p, "%d,%d,", s->u_min.i, s->u_max.i);
		p += sprintf(p, ",,%s,%s,", s->AnalogMsg.EU, s->Description);
		break;
	case dt_bool:
		p += sprintf(p, ",,");
		p += sprintf(
			p, 
			"%s,%s,,%s,",
			s->SwitchMsg.OnMsg,
			s->SwitchMsg.OffMsg,
			s->Description
			);
		break;
	}
	p += sprintf(
		p, 
		"%s,\"%s\"",
		(char*)CDeviceName(s->Device), 
		s->Address
		);

	// version 3.5 components	
	p += sprintf(
		p, 
		",%g,%d,%d,0x%04x,%d,%s,%d,%g", 
		s->ExcDev,
		s->ExcMax,
		s->Access,
		s->Owner,		
		s->DisplayDigits,
		s->Compressing==Compress_None? "none" : 
			s->Compressing == Compress_Const? "const" :
			s->Compressing == Compress_Linear? "linear" :
			"none",
		s->CompMax,
		s->CompDev
		);

	return p - output;
}

RTDB_API int PMC_API tag_to_textW(
	wchar_t *output, 
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	)
{
	char buf[1024];
	int retVal;
	USES_CONVERSION;
	retVal = tag_to_textA(buf, key, s);
	wcscpy(output, A2W(buf));
	return retVal;
}

RTDB_API int PMC_API dbg_dump_tag(
	PCTAG_KEY key, 
	PCSTATIC_TAG_DATA s
	)
{
	char buf[4096];
	tag_to_text(buf, key, s);
	return utils_debug("%s\n", buf);
}

RTDB_API int PMC_API tagname_to_textA(
	PCTAG_NAME tn, 
	char * name, 
	__uint len
	)
{
	string s;
	s  = (char*)CNodeName(tn->node);
	s += ".";
	s += (char *)CGroupName(tn->sname.group);
	s += ".";
	s += (char *)CTagName(tn->sname.tag);
	strncpy(name, s.data(), len);
	name[len - 1] = 0;
	return strlen(name);
}

RTDB_API int PMC_API tagname_to_textW(
	PCTAG_NAME tn, 
	wchar_t * name, 
	__uint len
	)
{
	char nameA[TAGNAME_TEXT_LENGTH + 1];
	int retVal;
	USES_CONVERSION;
	retVal = tagname_to_textA(tn, nameA, len);
	wcsncpy(name, A2W(nameA), len);
	name[len - 1] = 0;
	return retVal;
}

/*
	check if a tag's configuration is valid
*/
/*
函数功能：判断标签配置信息是否有效，          
参数说明：
          tte  指向待判断的标签的数据结构(RTK_TAG)的指针
返 回 值：0，没有错误值的返回
问    题：
          1、都没有错误值的返回，怎么判断。。？
		  2、只能判断两种数据类型的标签点（int32 和 date）
*/
RTDB_API int PMC_API validate_tag(PRTK_TAG tte)
{
	r4i32_t foo;
	switch(get_value_type(tte->s.Flags)){
		case dt_int8:
		case dt_int16:
		case dt_int32:
		case dt_uint8:
		case dt_uint16:
		case dt_uint32:
			if(tte->s.u_max.i < tte->s.u_min.i){
				foo = tte->s.u_max;
				tte->s.u_max = tte->s.u_min;
				tte->s.u_min = foo;
			}
			break;
		case dt_real4:
		case dt_real8:
		case dt_int64:
		case dt_uint64:
		case dt_date:
			if(tte->s.u_max.flt < tte->s.u_min.flt){
				foo = tte->s.u_max;
				tte->s.u_max = tte->s.u_min;
				tte->s.u_min = foo;
			}
			break;
	}
	tte->d.Value.Flags |= TF_ConfigOk;
	return 0;
}


