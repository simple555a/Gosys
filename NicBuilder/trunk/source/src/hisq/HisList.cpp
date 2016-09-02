// HisList.cpp : implementation file
//

#include "stdafx.h"
#include "mainfrm.h"
#include "hisq.h"
#include "HisList.h"
#include "config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHisList

CHisList::CHisList()
{
}

CHisList::~CHisList()
{
}


BEGIN_MESSAGE_MAP(CHisList, CListCtrl)
	//{{AFX_MSG_MAP(CHisList)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisList message handlers

void CHisList::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	long pt=pDispInfo->item.lParam;
	int id;
	FILETIME ftm;
	char str[64];
	PCpmc_value_t value;

	if(pDispInfo->item.mask & LVIF_TEXT)
	{
		cacheGetInfo(archiveCache, &cacheInfo);
		LPTSTR& pszText = pDispInfo->item.pszText;
		if(0==pDispInfo->item.iSubItem){
			SYSTEMTIME stm;
			ftm = hislogTimeByKey(cacheInfo.startTime + cacheInfo.step * pt);
			FileTimeToSystemTime(&ftm,&stm);
			sprintf(str,"%4dÄê%2dÔÂ%2dÈÕ %2d:%2d:%2d",
				stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
			pszText=str;
		}else if(pDispInfo->item.iSubItem<=cacheInfo.tagCount){
			id=pDispInfo->item.iSubItem-1;
			value = cacheGetValue(archiveCache, id, pt);
			
			if(pmf->m_names[id].IsEmpty()){
				sprintf(str,"");		
			}else if(!(value->Flags & TF_Valid) || !cacheGetFlag(archiveCache, id)){
				sprintf(str,".");		
			}else{
				switch(get_value_type(value->Flags)){
				case dt_real4:
					sprintf(str,pmf->m_format,value->Value.fltValue);		
					break;
				case dt_bool:
					if(0==value->Value.b){
						sprintf(str,"0");		
					}else{
						sprintf(str,"1");		
					}
					break;
				case dt_int32:
					sprintf(str,"%d",value->Value.iValue);		
					break;
				case dt_real8:
					sprintf(str,pmf->m_format,value->Value.dblValue);		
					break;
				default:
					sprintf(str,".");		
				}
			}
			pszText=str;
		}else if(pDispInfo->item.iSubItem<=HISLOG_MAX_QUERY_TAG_COUNT){
		}else{
			//ASSERT(0);
		}
	}

	*pResult = 0;
}
