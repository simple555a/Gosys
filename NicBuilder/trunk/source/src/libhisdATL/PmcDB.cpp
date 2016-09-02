// PmcDB.cpp : Implementation of CPmcDB
#include "stdafx.h"
#include "LibhisdATL.h"
#include "PmcDB.h"
//#include<IMessage.h>

#include "libhis.h"
#include "event.h"
#include"../mcView/AlarmRecord.h"
#include <atlstr.h>
#include <direct.h>

/////////////////////////////////////////////////////////////////////////////

//Functions


static void formatEvent(int rank, ALARM_CLASS ac, const char *msg, PCRTK_ADDR src, CPMCEvent *evt)
{
	evt->valid = true;
	switch (ac){
	case AC_Alarm:
	case AC_AckAlarm:
	case AC_CancelAlarm:
	case AC_Online:
	case AC_Offline:
	case AC_WriteDevice:
		evt->eClass = ac;
		strncpy(evt->data, msg, sizeof(evt->data));
		evt->data[sizeof(evt->data) - 1] = 0;
		evt->src = src->host;
		evt->rTime = getCurrentTime();
		evt->eTime = evt->rTime;
		break;
	}
}
//2015.03.19 GM ��Ӵ˻ص�����,�������ձ���ȷ����Ϣ
//ALARM_CLASS�а���������Ϣ������ȷ�ϣ�������ʧ������

static void _cdecl HisdATLCallBack(int rank, ALARM_CLASS ac, const char *msg, PCRTK_ADDR src)
{
	
	//2015.11.09   GM   ԭ�еı���ȷ�ϳ���

	//**************************************************************************************
	//���ȴ�msg�н�������ǩ�����ɲο�evtCallBack()����
	//CPMCEvent evt;
	//formatEvent(rank, ac, msg, src, &evt);

	//CArgs args;

	//parse_arg_ex(evt.data, &args, ";");

	////���ݱ�ǩ���ҵ�ʵʱ���ж�Ӧ��rtktag ��Ϣ���ɲο����´��룺

	//TAG_NAME tn;
	//PRTK_TAG p=NULL;

	//tagname_by_text(&tn, args.argv[1]);
	////PRTK_TAG p;
	//if (!lock_rtdb(__false, 500))
	//{
	//	return;
	//}
	//p = query_tag_f(&tn);

	//if (!p)
	//{
	//	unlock_rtdb();
	//	return;
	//}

	//unlock_rtdb();

	//
	////����ac��ֵ����������p�еĶ�Ӧ����
	//
	////if (ac==AC_AckAlarm)
	////{
	////	p->d.AlarmClass=AC_AckAlarm;
	////}
	//

	//switch (ac){
	//case AC_AckAlarm:
	//	{
	//		p->d.AlarmClass=AC_AckAlarm;
	//		break;
	//	}

	//	//moxe+++
	//case AC_Alarm:
	//		break;
	//case AC_CancelAlarm:
	//	{
	//		
	//			p->d.AlarmClass=AC_CancelAlarm;
	//			break;
	//	}
	//case AC_Online:
	//case AC_Offline:
	//	//alarms->addNewRecord(&evt);
	//	break;
	//case AC_WriteDevice:
	//	//operates->addNewRecord(&evt);
	//	break;
	//}

	//**************************************************************************************
	
	//MessageBox.show("haha");
	//formatEvent(rank, ac, msg, src, &evt);


    //2015.11.10   GM   �µı���ȷ�ϴ�������ԭ�еı��������ǵ�������ȷ�ϱ����������������ȫ��ȷ�ϰ�ťʱ
	//�����������������ָ���Ϊ���ȫ��ȷ�ϰ�ťʱ����������Ҳ����
	FILE   * fp;
	if(ac == AC_AckAlarm)//receive the fake alarm  
	{   
		CString str3, str4, str6;
		
		str3.Format("%s",msg); 
		//str4 = str3.Right(8);
		str6 = str3.Right(3);
		if (!strcmp(str6, "ALL"))
		{
			    string str, s1, s2, s3, s4;
				char   strLineBuf[1024];

				CArgs args;
				TAG_NAME tn;
				PRTK_TAG p=NULL;

				str = string(get_working_dir()) + string("BZAlarm\\alarms.cfg");
				mkdir(str.c_str());
				
				fp = fopen(str.data(),"rt");

				if(!fp){
					utils_error("Cannot open alarms.cfg file:%s\n", str.data()); 	   
					return ;
				}
 
				//iLine = 0;
				while(!feof(fp)){
					if( !fgets(strLineBuf,sizeof(strLineBuf),fp) ) break;
					if(strLineBuf[strlen(strLineBuf)-1]=='\n'){
						strLineBuf[strlen(strLineBuf)-1]=0;
					}
					if(strLineBuf[0] == '#'){
						continue;
					}

					parse_arg_ey(strLineBuf, &args, ";");

					tagname_by_text(&tn, args.argv[0]);
					//PRTK_TAG p;
					if (!lock_rtdb(__false, 500))
					{
						return;
					}
					p = query_tag_f(&tn);

					if (!p)
					{
						unlock_rtdb();
						return;
					}

					unlock_rtdb();
					if (p->d.Value.Flags & ((TF_SharpChange | TF_HiHi | TF_Hi | TF_Lo |TF_LoLo | TF_On2Off | TF_Off2On)))
					{
						p->d.AlarmClass=AC_AckAlarm;
					}
				}
			}
			else//ack just one alarm
			{
				CPMCEvent evt;
				formatEvent(rank, ac, msg, src, &evt);

				CArgs args;

				parse_arg_ex(evt.data, &args, ";");

				//���ݱ�ǩ���ҵ�ʵʱ���ж�Ӧ��rtktag ��Ϣ���ɲο����´��룺

				TAG_NAME tn;
				PRTK_TAG p=NULL;

				tagname_by_text(&tn, args.argv[1]);
				//PRTK_TAG p;
				if (!lock_rtdb(__false, 500))
				{
					return;
				}
				p = query_tag_f(&tn);

				if (!p)
				{
					unlock_rtdb();
					return;
				}

				unlock_rtdb();
				if (p->d.Value.Flags & ((TF_SharpChange | TF_HiHi | TF_Hi | TF_Lo |TF_LoLo | TF_On2Off | TF_Off2On)))
				{
					p->d.AlarmClass=AC_AckAlarm;
				}
			}

	}
	else if (ac == AC_CancelAlarm)
	{
		CPMCEvent evt;
		formatEvent(rank, ac, msg, src, &evt);

		CArgs args;

		parse_arg_ex(evt.data, &args, ";");

		//���ݱ�ǩ���ҵ�ʵʱ���ж�Ӧ��rtktag ��Ϣ���ɲο����´��룺

		TAG_NAME tn;
		PRTK_TAG p=NULL;

		tagname_by_text(&tn, args.argv[1]);
		//PRTK_TAG p;
		if (!lock_rtdb(__false, 500))
		{
			return;
		}
		p = query_tag_f(&tn);

		if (!p)
		{
			unlock_rtdb();
			return;
		}

		unlock_rtdb();
		p->d.AlarmClass=AC_CancelAlarm;
	} 

//

//

//

//
//	//��isAlarmTag�������������Ӷ���һ���Ե��жϣ������Ӧ����Ϊ����ȷ�ϣ���value_i.Value.b��0
//		
}

void _setupProxy()
{
	char path[_MAX_PATH];
	GetModuleFileName(_Module.m_hInst, path, sizeof(path));
	extract_file_path(path);
	set_working_dir(path);
	init_proxy(
		0,
		//PROXY_SILENT | PROXY_ENABLE_BGLOADER    
		PROXY_ENABLE_DISPATCHER | PROXY_SILENT | PROXY_ENABLE_BGLOADER
		);
	initHisProxy();

	//2015.03.19 GM ���Ӵ���Ϣ�������
	g_ProxyDispatchTable.OnAlarm=HisdATLCallBack;
}



// CPmcDB
CPmcDB::CPmcDB()
{
	if (!count)
	{
		count++;
		_setupProxy();
	}
}

CPmcDB::~CPmcDB()
{
	count--;
	if (!count)
	{
		uninitHisProxy();
		uninit_proxy();
	}
}

STDMETHODIMP CPmcDB::lock(BOOL exclusive, LONG timeOut)
{
	// TODO: Add your implementation code here
	lock_rtdb(exclusive, timeOut);

	return S_OK;
}

STDMETHODIMP CPmcDB::unlock()
{ 
	// TODO: Add your implementation code here
	unlock_rtdb();

	return S_OK;
}


static SAFEARRAY * getArrayParam(const VARIANT * v)
{
	SAFEARRAY * a;
	switch(v->vt){
	case VT_VARIANT | VT_BYREF:
		a=getArrayParam(v->pvarVal);
		break;
	case VT_ARRAY | VT_VARIANT:
		a=v->parray;
		break;
	case VT_ARRAY | VT_VARIANT | VT_BYREF:
		a=*v->pparray;
		break;
	default:
		a=0;
	}
	return a;
}

STDMETHODIMP CPmcDB::readSnapshot(BSTR tagName, VARIANT *tagValue)
{
	// TODO: Add your implementation code here
	TAG_NAME tn;
	PRTK_TAG p;

	tagname_by_textW(&tn, tagName);

	if (!lock_rtdb(__false, 500))
	{
		return S_FALSE;
	}

	p = query_tag_f(&tn);

	if (!p)
	{
		tagValue->bstrVal = SysAllocString(L"N.A.");
		tagValue->vt = VT_BSTR;
		unlock_rtdb();
		return S_OK;
	}

	pmc_value_to_variant(&p->d.Value, tagValue);

	unlock_rtdb();

	return S_OK;
}



STDMETHODIMP CPmcDB::writeDevice(BSTR tagName, VARIANT tagValue)
{
	// TODO: Add your implementation code here
	TAG_NAME tn;
	pmc_value_t pt;

	tagname_by_textW(&tn, tagName);
	if (!pmc_variant_to_value(&tagValue, &pt))
	{
		return E_INVALIDARG;
	}
	proxy_write_tags_ex(1, &tn, &pt);
	pmc_clear(&pt);

	return S_OK;
}

//����ʷ��ȡĳ��ʱ�̵�ֵ
STDMETHODIMP CPmcDB::readArchiveValue(BSTR tagName, VARIANT date, VARIANT *tagValue)
{
	// TODO: Add your implementation code here
	SYSTEMTIME st;
	FILETIME ft;
	TAG_NAME tn;
	hislog_item_t value;
	VARIANT dd;
	HRESULT hr;
	
	if(date.vt != VT_DATE){
		// BUGBUG: not tested
		hr = VariantChangeType(&dd, &date, 0, VT_DATE);
		if(FAILED(hr)){
			return hr;
		}
		date = dd;
	}
	VariantTimeToSystemTime(date.date, &st);
	SystemTimeToFileTime(&st, &ft);
	tagname_by_textW(&tn, tagName);
	queryHisData(PT_QueryMode_OneData, &tn, &ft, 1, 1);
	__bool ret = __false;
    ret = getHisData(&value);
	if (ret == __false)
	{
		return S_FALSE;
	}
	pmc_value_to_variant(&value, tagValue);	

	return S_OK;
}

//����ʷ��ȡʱ��ε�ֵ
STDMETHODIMP CPmcDB::readArchiveValuesByCount(BSTR tag, VARIANT sTime, VARIANT eTime, int valueCount, VARIANT *tagValue)
{
	// TODO: Add your implementation code here
	SYSTEMTIME st;
	FILETIME ft[2];
	VARIANT pdate[2];
	TAG_NAME tn;
	hislog_item_t* pvalue;
	VARIANT dd;
	HRESULT hr;
	VARIANT* pv;
    //
	SAFEARRAY * _tagValue=getArrayParam(tagValue);
	if(!_tagValue){
		return E_INVALIDARG;
	}
	if(_tagValue->rgsabound[0].cElements < count){
		return E_INVALIDARG;
	}
	
	hr = SafeArrayAccessData(_tagValue, (void **)&pv);
	if(FAILED(hr)){
		return hr;
	}

	//
	pdate[0] = sTime;
	pdate[1] = eTime;

	for (int i=0; i!=2; i++)
	{
		
		if(pdate[i].vt != VT_DATE){
			// BUGBUG: not tested
			hr = VariantChangeType(&dd, &pdate[i], 0, VT_DATE);
			if(FAILED(hr)){
				return hr;
			}
			pdate[i] = dd;
		}
		VariantTimeToSystemTime(pdate[i].date, &st);
		SystemTimeToFileTime(&st, &ft[i]);
	}

	tagname_by_textW(&tn, tag);
	
	queryHisData(PT_QueryMode_BYCOUNT, &tn, ft, 1, long(valueCount));

	//
	pvalue = (hislog_item_t*)malloc(valueCount*sizeof(hislog_item_t));

	__bool ret = __false;
    ret = getHisData(pvalue);
	if (ret == __false)
	{
		return S_FALSE;
	}
	for (int j=0; j!=valueCount; j++, pv++)
	{
		pmc_value_to_variant(&pvalue[j], pv);
	}

	SafeArrayUnaccessData(_tagValue);

	free(pvalue);
		
	return S_OK;
}

//дֵ��ʵʱ��
STDMETHODIMP CPmcDB::writeTag(BSTR tagName, VARIANT tagValue)
{
	// TODO: Add your implementation code here
	TAG_NAME tn;
	pmc_value_t pt;
	
	tagname_by_textW(&tn, tagName);
	if(!pmc_variant_to_value(&tagValue, &pt)){
		return E_INVALIDARG;
	}
	proxy_write_tags_ex(1, &tn, &pt);
	pmc_clear(&pt);

	return S_OK;
}

// is larm
STDMETHODIMP CPmcDB::isAlarmTag(BSTR tagName, VARIANT *alarm_sign)
{
	// TODO: Add your implementation code here
	TAG_NAME tn;
	PRTK_TAG p;

	tagname_by_textW(&tn, tagName);

	if (!lock_rtdb(__false, 500))
	{
		return S_FALSE;
	}

	p = query_tag_f(&tn);

	if (!p)
	{
		alarm_sign->bstrVal = SysAllocString(L"N.A.");
		alarm_sign->vt = VT_BSTR;
		unlock_rtdb();
		return S_OK;
	}

	// 
	pmc_value_t value_i;
	memset(&value_i, 0, sizeof(pmc_value_t));
	set_value_type(value_i.Flags, dt_bool);

	if (p->d.Value.Flags & ((TF_SharpChange | TF_HiHi | TF_Hi | TF_Lo |TF_LoLo | TF_On2Off | TF_Off2On)))
	{
		//UINT32 u32tmp;
		//u32tmp=p->d.Value.Flags & TF_ACKAlarm;
		if(p->d.AlarmClass==AC_AckAlarm)
		{
			value_i.Value.b = 0;
		}
		else
		{
			value_i.Value.b = 1;
		}		
		
	}else{
		value_i.Value.b = 0;
	}

	value_i.Flags |= TF_Valid;

	pmc_value_to_variant(&value_i, alarm_sign);

	unlock_rtdb();

	return S_OK;
}
