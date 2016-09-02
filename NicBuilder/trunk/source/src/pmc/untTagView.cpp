#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PERFGRAP"
#pragma resource "*.dfm"
#define err(s) lblHint->Caption = s

TframTag *framTag;

static __r8 _getSpan(PCRTK_TAG t)
{
	switch(get_value_type(t->s.Flags)){
	case dt_real4:
	case dt_real8:
        case dt_int64:
        case dt_uint64:
		return t->s.u_max.flt - t->s.u_min.flt;

	case dt_date:
		return 0;

	case dt_int8:
    case dt_int16:
	case dt_int32:
    case dt_uint8:
	case dt_uint16:
    case dt_uint32:
		return t->s.u_max.i - t->s.u_min.i;
	}

	return 0;
}

//---------------------------------------------------------------------------
__fastcall TframTag::TframTag(TComponent* Owner)
	: TFrame(Owner)
{
	tagname_by_text(&m_TagName, "..");
	lblStatus->Caption = "";
	grpSwitchProperty->Left = grpAnalogProperty_1->Left;
	grpSwitchProperty->Top = grpAnalogProperty_1->Top;
	ZeroMemory(&m_Tag, sizeof(m_Tag));
	ZeroMemory(&m_TagName, sizeof(m_TagName));
}
//---------------------------------------------------------------------------

/*
	Load data from storage to UI
*/
void __fastcall TframTag::UpdateView()
{
	//TODO: Add your source code here
	TComponent * comp;
	int i;
	PTAG_NAME name = &m_TagName;
	// update static information
	PRTK_TAG pTag = &m_Tag;
	char textname[TAGNAME_TEXT_LENGTH + 1];
	char buf[64];

	// temporarily disable unapplicable components
	tsException->TabVisible = false;
    
    btnWriteDevice->Enabled = (pTag->d.Value.Flags & TF_ReadOnly)? false : true;

	// now we update the UI to reflect the tag configuration
    /*
    0 开关量
    1 模拟量
    2 高精度模拟量
    3 16位整数
    4 32位整数
    5 64位整数
    6 64位整数
    7 无符号8位整数
    8 无符号16位整数
    9 无符号32位整数
    10 无符号64位整数
    11 日期
    */
    /*Zander 20120202
    0 开关量
    1 模拟量
    2 高精度模拟量
    3 16位整数
    4 32位整数
    5 64位整数
    6 日期
    */
	cmbType->OnChange = NULL;
	switch(get_value_type(pTag->s.Flags)){
	case dt_bool:
        cmbType->ItemIndex = 0;
        break;
	case dt_real4:
    	cmbType->ItemIndex = 1;
        break;
	case dt_real8:
        cmbType->ItemIndex = 2;
        break;
        case dt_int8:
        cmbType->ItemIndex = 3;
        break;
        case dt_int16:
        cmbType->ItemIndex = 4;
        break;
        case dt_int32:
        cmbType->ItemIndex = 5;
        break;
        case dt_int64:
        cmbType->ItemIndex = 6;
        break;
        case dt_uint8:
        cmbType->ItemIndex = 7;
        break;
        case dt_uint16:
        cmbType->ItemIndex = 8;
        break;
        case dt_uint32:
        cmbType->ItemIndex = 9;
        break;
        case dt_uint64:
        cmbType->ItemIndex = 10;
        break;
        case dt_date:
    	cmbType->ItemIndex = 11;
    	break;

        default:
    	cmbType->ItemIndex = 12;
    	break;
	}
	cmbType->OnChange = cmbTypeChange;

	{
		cmbDevice->Ctl3D = false;
		CDeviceItems * ditems;
		AnsiString Url;
		Url = (char*)CNodeName(name->node);
		Url += ".DEVICES";
		ditems = dynamic_cast<CDeviceItems*>(g_DCS.Search(Url));
		if(ditems){
			if(!ditems->m_iRefreshCount){
				ditems->Refresh();
			}
		}
		cmbDevice->Items->Clear();
		cmbDevice->Text = "";
		if(ditems){
			CDeviceItem * ditem;
			ditem = dynamic_cast<CDeviceItem*>(ditems->GetFirstChild());
			while(ditem){
				cmbDevice->Items->Add((char*)CDeviceName(ditem->info.k));
				ditem = dynamic_cast<CDeviceItem*>(ditems->GetNextChild(ditem));
			}
		}
	}

	{
		PRTK_GROUP pgrp;
		RTK_CURSOR hgrp;
		hgrp = open_group_f(&name->node, &name->sname.group);
		pgrp = (PRTK_GROUP)cursor_get_item(hgrp);
		if(pgrp){
			if(pgrp->period > 50){
				tmrUpdater->Interval = pgrp->period;
			}
		}
		close_handle(hgrp);
	}

	edtTagName->Text = (char *)CTagName(name->sname.tag);
	edtCName->Text = pTag->s.Description;
	edtAddr->Text  = pTag->s.Address;
	cmbDevice->Text = (char*)CDeviceName(pTag->s.Device);
#define cc(name) \
	do{ if(pTag->s.Flags & TF_##name){\
			chk##name->Checked =  true;\
		}else{\
			chk##name->Checked = false;\
		}\
	}while(0)
	cc(HiHi);
	cc(Lo);
	cc(Hi);
	cc(LoLo);
	cc(On2Off);
	cc(Off2On);
	cc(Rate);
	cc(SaveToHistory);
    cc(Step);
/*
	// cutoff is obsolete
#define dd(name) chk##name->Checked = pTag->s.CutOffMask & TF_##name ? true : false;
	dd(HiHi);
	dd(Lo);
	dd(Hi);
	dd(LoLo);
	dd(On2Off);
	dd(Off2On);
*/
	if(pTag->s.Flags & TF_System){
		cmbDevice->Enabled = false;
		edtAddr->ReadOnly = true;
		cmbType->Enabled = false;
		cmbDevice->Color = clScrollBar;
		edtAddr->Color = clScrollBar;
		cmbType->Color = clScrollBar;
	}else{
		cmbDevice->Enabled = true;
		edtAddr->ReadOnly = false;
		cmbType->Enabled = true;
		cmbType->Color = clWindow;

		cmbDevice->Color = clWindow;
		edtAddr->Color = clWindow;
	}
	
	switch(get_value_type(pTag->s.Flags)){
	case dt_real4:
	case dt_real8:
    case dt_int64:
    case dt_uint64:
		#define aa(f) sprintf(buf, "%g", pTag->s.flt##f);edt##f->Text = buf;
		aa(HiHi);
		aa(Hi);
		aa(Lo);
		aa(LoLo);
		aa(AlarmDeadband);
		aa(MaxValue);
		aa(MinValue);
		aa(Rate);
		edtEU->Text = pTag->s.AnalogMsg.EU;
		tsAnalog->TabVisible = true;
		tsLogical->TabVisible = false;
		break;
	case dt_int8:
    case dt_int16:
	case dt_int32:
    case dt_uint8:
	case dt_uint16:
    case dt_uint32:
		aa(AlarmDeadband);
		aa(Rate);
		#undef aa
		#define aa(v, name) \
			sprintf(buf, "%d", pTag->s.v);\
			edt##name->Text = buf;
		aa(u_hihi.i, HiHi);
		aa(u_hi.i, Hi);
		aa(u_lo.i, Lo);
		aa(u_lolo.i, LoLo);
		aa(u_max.i, MaxValue);
		aa(u_min.i, MinValue);
		edtEU->Text   = pTag->s.AnalogMsg.EU;
		tsAnalog->TabVisible = true;
		tsLogical->TabVisible = false;
		break;
	case dt_date:
		tsArchive->TabVisible = true;
		tsAnalog->TabVisible = false;
		tsLogical->TabVisible = false;
		break;
	case dt_bool:
		edtOnMessage->Text = pTag->s.SwitchMsg.OnMsg;
		edtOffMessage->Text = pTag->s.SwitchMsg.OffMsg;
		tsLogical->TabVisible = true;
		tsAnalog->TabVisible = false;		
	default:
		assert(0);
		break;
	}

	tsAnalog->Visible = tsAnalog->TabVisible;
	tsLogical->Visible = tsLogical->TabVisible;
	tsException->Visible = tsException->TabVisible;
	tsArchive->Visible = tsArchive->TabVisible;
	
	edtAlarmRank->Text = pTag->s.AlarmRank;
    edtAlarmRank2->Text = pTag->s.AlarmRank;

	chkCompressing->Checked = true;
	edtCompMax->Enabled = chkCompressing->Checked;
	edtCompDev->Enabled = chkCompressing->Checked;
	edtCompDevPercent->Enabled = chkCompressing->Checked;
	edtCompMax->Text = IntToStr(pTag->s.CompMax);
	edtCompDev->Text = FloatToStr(pTag->s.CompDev);
    __r8 span = _getSpan(pTag);
    if(span > 1e-5){
		edtCompDevPercent->Text = FloatToStr(pTag->s.CompDev / span * 100);
    }

	TAG_NAME ctn;
	ZeroMemory(&ctn, sizeof(ctn));
	ctn.sname = pTag->s.CutOffTagName;
	tagname_to_text(&ctn, textname, sizeof(textname));
	
	// debugging output
	{
		char nn[128];
		*nn=0;
	#define __check_field__(field,name) \
		if(pTag->d.Value.Flags & TF_##field) {\
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
		__check_field__(Alarm,alarm)
		__check_field__(Expired,expired)
		__check_field__(SharpChange,sharp)
		__check_field__(ReadOnly,read)
		__check_field__(Translated,trans)
		__check_field__(ConfigOk,ok)
		__check_field__(Valid,valid)
		utils_debug("%s flags=0x%08x,%s\n", textname, pTag->d.Value.Flags, nn);
	#undef __check_field__
	}

	// pg->Clear();
}

void __fastcall TframTag::tmrUpdaterTimer(TObject *Sender)
{
	pmc_value_t value;
	AnsiString txtValue;
	char buf[64];
	FILETIME ft;
	SYSTEMTIME st;

	proxy_read_tags(1, &m_TagName, &value, 0);
	if(value.Flags & TF_Valid){
    	pmc_value_t strVal;
        set_value_type(strVal.Flags, dt_string);
        pmc_type_cast(&value, &strVal);
        txtValue = strVal.Value.str;
        pmc_clear(&strVal);
	}else{
		txtValue = "N.A.";
	}
	edtValue->Text = txtValue;

	RTK_TAG * p = query_tag_f(&m_TagName);
	if(p){
		AnsiString s;
		FILETIME   lft;
		SYSTEMTIME st;
		FileTimeToLocalFileTime((LPFILETIME)&p->d.CachedTime, &lft);
		FileTimeToSystemTime(&lft, &st);
		s = "";
		s.cat_printf(
			"%d年%d月%d日  %02d:%02d:%02d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond
			);
		lblStatus->Caption = s;
	}else{
		lblStatus->Caption ="";
	}
}

AnsiString __fastcall TframTag::UpdateData()
{
	return "";
}

//---------------------------------------------------------------------------
void __fastcall TframTag::btnWriteDeviceClick(TObject *Sender)
{
	AnsiString value;
	pmc_value_t strVal, val;
	value = InputBox(
		"输入要写的数值",
		(char*)CTagName(m_TagName.sname.tag),
		""
		);
	if(value.Length() == 0){
		return;
	}
    set_value_type(strVal.Flags, dt_string);
    strVal.Value.str = value.c_str();
    val.Flags = m_Tag.s.Flags;
    pmc_type_cast(&strVal, &val);
	if(proxy_write_tags(1, &m_TagName, &val.Value) ){
		m_Hint = "写入成功";
	}else{
		m_Hint = "写入失败";
	}
}

//---------------------------------------------------------------------------

/*
	gather data from UI to internal representation
*/
bool TframTag::ValidateView()
{
	PTAG_NAME name;
	PRTK_TAG tag;	
	name = &m_TagName;
	tag = &m_Tag;
	
	try{
	#undef cc

	#define cc(name) tag->s.Flags |= (chk##name->Checked ? TF_##name : 0);

		if( edtTagName->Text.Length() >sizeof(TAG_KEY) ||
			!edtTagName->Text.Length()
		){
			m_Hint = "变量名不正确";
			throw(-1);
		}
		if( !is_valid_name(edtTagName->Text.c_str()) ){
			m_Hint = "变量名只能包含字母、数字和下划线";
			throw(-3);
		}
		if(edtTagName->Text.Length() > sizeof(TAG_KEY)){		
			m_Hint = AnsiString("错误: 标签名不能超过 ") + IntToStr(sizeof(TAG_KEY)) + " 个字节";		 
			throw(-3);
		}
		name->sname.tag = CTagName(edtTagName->Text.c_str());
		tag->s.Device = CDeviceName(cmbDevice->Text.c_str());
		tag->key = name->sname.tag;
		if(edtCName->Text.Length() > sizeof(tag->s.Description)){
			m_Hint = "中文名太长(最长 " + AnsiString(sizeof(tag->s.Description)/2) + " 字)";
			throw(-4);
		}
		strncpy(tag->s.Description, edtCName->Text.c_str(), sizeof(tag->s.Description));
		tag->s.Description[sizeof(tag->s.Description) - 1] = 0;
		if(edtAddr->Text.Length() > sizeof(tag->s.Address)){
			m_Hint = "地址太长(最长 " + AnsiString(sizeof(tag->s.Address)/2) + " 字)";
			throw(-5);
		}
		strncpy(tag->s.Address, edtAddr->Text.c_str(), sizeof(tag->s.Address));
		tag->s.Address[sizeof(tag->s.Address) - 1] = 0;

        tag->s.Flags = 0;
        
		switch(cmbType->ItemIndex){
		case 0:
			set_value_type(tag->s.Flags, dt_bool);
			break;
		case 1:
			set_value_type(tag->s.Flags, dt_real4);
			break;
		case 2:
			set_value_type(tag->s.Flags, dt_real8);
			break;
		case 3:
			set_value_type(tag->s.Flags, dt_int8);
			break;
		case 4:
			set_value_type(tag->s.Flags, dt_int16);
			break;
		case 5:
			set_value_type(tag->s.Flags, dt_int32);
			break;
		case 6:
			set_value_type(tag->s.Flags, dt_int64);
			break;
		case 7:
			set_value_type(tag->s.Flags, dt_uint8);
			break;
		case 8:
			set_value_type(tag->s.Flags, dt_uint16);
			break;
		case 9:
			set_value_type(tag->s.Flags, dt_uint32);
			break;
		case 10:
			set_value_type(tag->s.Flags, dt_uint64);
			break;
		case 11:
			set_value_type(tag->s.Flags, dt_date);
			break;
                case 12:
                        m_Hint = "不支持这个数据类型.";
                        throw(-41);
                      //  break;
		}

		cc(SaveToHistory);
        cc(Step);

		// collecting common analog attributes
		switch(get_value_type(tag->s.Flags)){
        case dt_int8:
		case dt_int16:
		case dt_int32:
        case dt_int64:
        case dt_uint8:
		case dt_uint16:
		case dt_uint32:
        case dt_uint64:
        case dt_real4:
		case dt_real8:
        case dt_date:
			cc(HiHi);
			cc(Lo);
			cc(Hi);
			cc(LoLo);
			cc(Rate);
			strncpy(
				tag->s.AnalogMsg.EU,
				edtEU->Text.c_str(),
				sizeof(tag->s.AnalogMsg.EU)
				);
			tag->s.AnalogMsg.EU[sizeof(tag->s.AnalogMsg.EU) - 1] = 0;
			break;
		case dt_bool:
			cc(On2Off);
			cc(Off2On);
			
			strncpy(
				tag->s.SwitchMsg.OnMsg,
				edtOnMessage->Text.c_str(),
				sizeof(tag->s.SwitchMsg.OnMsg)
				);
			tag->s.SwitchMsg.OnMsg[sizeof(tag->s.SwitchMsg.OnMsg) - 1] = 0;
			strncpy(
				tag->s.SwitchMsg.OffMsg,
				edtOffMessage->Text.c_str(),
				sizeof(tag->s.SwitchMsg.OffMsg)
				);
			tag->s.SwitchMsg.OffMsg[sizeof(tag->s.SwitchMsg.OffMsg) - 1] = 0;
			break;
		}

#define get(msg, name, ignore_on_condition, _default_value)\
	m_Hint = msg;\
	if( !edt##name->Text.Length() ){\
		if(ignore_on_condition){\
			tag->s.flt##name = _default_value;\
		}else{\
			throw(-1);\
		}\
	}else{\
		tag->s.flt##name = edt##name->Text.ToDouble();\
	}
#pragma warn -ccc
#pragma warn -rch
		switch(get_value_type(tag->s.Flags)){
		case dt_real4:
		case dt_real8:
			get("高高值不正确", HiHi, !chkHiHi->Checked, 0);
			get("低低值不正确", LoLo, !chkLoLo->Checked, 0);
			get("低值不正确", Lo, !chkLo->Checked, 0);
			get("高值不正确", Hi, !chkHi->Checked, 0);
			get("报警死区值不正确", AlarmDeadband, !(tag->s.Flags & 0xffffff00), 0);
			get("最大值不正确", MaxValue, false, 0);
			get("最小值不正确", MinValue, false, 0);
			get("速率报警阈值不正确", Rate, !chkRate->Checked, 0.1);
			break;

		case dt_int8:
		case dt_int16:
		case dt_int32:
        case dt_int64:
		case dt_uint8:
		case dt_uint16:
		case dt_uint32:
        case dt_uint64:
			get("速率报警阈值不正确", Rate, !chkRate->Checked, 0.1);
#undef get
#define get(msg, attrName, uiName, ignore_on_condition, _default_value)\
	m_Hint = msg;\
	if( !edt##uiName->Text.Length() ){\
		if(ignore_on_condition){\
			tag->s.##attrName = _default_value;\
		}else{\
			throw(-1);\
		}\
	}else{\
		tag->s.attrName = edt##uiName->Text.ToDouble();\
	}
			get("高高值不正确", u_hihi.i, HiHi, !chkHiHi->Checked, 0);
			get("低低值不正确", u_lolo.i, LoLo, !chkLoLo->Checked, 0);
			get("低值不正确", u_lo.i, Lo, !chkLo->Checked, 0);
			get("高值不正确", u_hi.i, Hi, !chkHi->Checked, 0);
			get("报警死区值不正确", u_deadband.i, AlarmDeadband, !(tag->s.Flags & 0xffffff00), 0);
			get("最大值不正确", u_max.i, MaxValue, false, 0);
			get("最小值不正确", u_min.i, MinValue, false, 0);
			break;
		}

#pragma warn .ccc
#pragma warn .rch

        if(get_value_type(tag->s.Flags) == dt_bool){
            tag->s.AlarmRank = edtAlarmRank2->Text.ToInt();
        }else{
    		tag->s.AlarmRank = edtAlarmRank->Text.ToInt();
        }

		tag->s.Compressing = Compress_Const;
		tag->s.CompDev = StrToFloat(edtCompDev->Text);
		tag->s.CompMax = StrToInt(edtCompMax->Text);
	}catch(...){
		return false;
	}
	m_Hint = "更新成功";
	return true;
}

void __fastcall TframTag::btnSaveClick(TObject *Sender)
{
	InvokeAction(ca_Save);	  
}
//---------------------------------------------------------------------------

void __fastcall TframTag::btnReadClick(TObject *Sender)
{
	//	  
}

void __fastcall TframTag::cmbTypeChange(TObject *Sender)
{
	//
	ValidateView();
	switch(cmbType->ItemIndex){
	case 0:
		set_value_type(m_Tag.s.Flags, dt_bool);
		break;
	case 1:
		set_value_type(m_Tag.s.Flags, dt_real4);
		break;
	case 2:
		set_value_type(m_Tag.s.Flags, dt_real8);
		break;
	case 3:
		set_value_type(m_Tag.s.Flags, dt_int8);
		break;
	case 4:
		set_value_type(m_Tag.s.Flags, dt_int16);
		break;
	case 5:
		set_value_type(m_Tag.s.Flags, dt_int32);
		break;
	case 6:
		set_value_type(m_Tag.s.Flags, dt_int64);
		break;

	case 7:
		set_value_type(m_Tag.s.Flags, dt_uint8);
		break;
	case 8:
		set_value_type(m_Tag.s.Flags, dt_uint16);
		break;
	case 9:
		set_value_type(m_Tag.s.Flags, dt_uint32);
		break;
	case 10:
		set_value_type(m_Tag.s.Flags, dt_uint64);
		break;

	case 11:
		set_value_type(m_Tag.s.Flags, dt_date);
		break;
	case 12:
		set_value_type(m_Tag.s.Flags, dt_null);
		break;
        }
	UpdateView();
	PageControl1->ActivePageIndex = 0;
	// PageControl1->Pages[0]->Visible = true;
}

void __fastcall TframTag::setTag(PCTAG_NAME name)
{
	PCRTK_TAG pTag;
	m_TagName = *name;
	pTag = query_tag_f(&m_TagName);
	if(pTag){
		// we are editing an existing tag, make a copy of the original tag configuration
		m_Tag = *pTag;
	}else{
		// no, we'are creating a new tag
		// pTag = &m_Tag;
	}
}
//---------------------------------------------------------------------------

void __fastcall TframTag::edtCompDevChange(TObject *Sender)
{
	//
	try{
		m_Tag.s.CompDev = StrToFloat(edtCompDev->Text);
        __r8 span = _getSpan(&m_Tag);
        if(span > 1e-5){
        	edtCompDevPercent->Text = FloatToStr(m_Tag.s.CompDev / span * 100);
        }
	}catch(...){
	}
}
//---------------------------------------------------------------------------

void __fastcall TframTag::edtCompDevPercentChange(TObject *Sender)
{
	//
	try{
		__r4 percent;
		percent = StrToFloat(edtCompDevPercent->Text);
		if(percent < 0 || percent > 100){
			return;
		}
		__r4 span = _getSpan(&m_Tag);
		m_Tag.s.CompDev = percent * span / 100;
        edtCompDev->Text = FloatToStr(m_Tag.s.CompDev);
	}catch(...){
	}
}
//---------------------------------------------------------------------------


