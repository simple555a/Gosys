// DlgDnyTip.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgDnyTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDnyTip dialog


CDlgDnyTip::CDlgDnyTip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDnyTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDnyTip)
	//}}AFX_DATA_INIT
	sTips0 = "";
	sTips1 = "";
	sTips2 = "";
}


void CDlgDnyTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDnyTip)
	DDX_Control(pDX, IDC_LINKS, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDnyTip, CDialog)
	//{{AFX_MSG_MAP(CDlgDnyTip)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LINKS, OnGetdispinfoLinks)
	ON_WM_NCLBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_TREND, OnTrend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDnyTip message handlers

void CDlgDnyTip::UpdateView()
{
	char line[1024 * 8];

	m_list.DeleteAllItems();
	// arg0
	memset(line, 0, sizeof(line));
	strncpy(line, (LPCTSTR)sTips0, sizeof(line) - 1);
	parse_arg_ey(line, &arg0, "\n");
	// arg1
	memset(line, 0, sizeof(line));
	strncpy(line, (LPCTSTR)sTips1, sizeof(line) - 1);
	parse_arg_ey(line, &arg1, "\n");
	tagname_by_textA(&tn, (LPCTSTR)sTips1);
	
	if(lock_rtdb(__false, 100)){
		PRTK_TAG p;
		p = query_tag_f(&tn);
		if(p){
			strncpy(address, p->s.Address, sizeof(address));
			address[sizeof(address) - 1] = 0;
		}else{
			strcpy(address, "N.A.");
		}
		unlock_rtdb();
	}

	memset(line, 0, sizeof(line));
	strncpy(line, (LPCTSTR)sTips2, sizeof(line) - 1);
	parse_arg_ey(line, &arg2, "\n");

	LV_ITEM listItem;
	listItem.mask = LVIF_TEXT|LVIF_PARAM;
	for(int i = 0; i < arg0.argc - 1; i++){
		listItem.iSubItem = 0;
		listItem.iItem = m_list.GetItemCount();
		listItem.pszText = LPSTR_TEXTCALLBACK;
		listItem.cchTextMax = 50;
		listItem.lParam = (LPARAM)i;
	
		m_list.InsertItem(&listItem);
		m_list.SetItemText(listItem.iItem,1, LPSTR_TEXTCALLBACK);
	}
}

BOOL CDlgDnyTip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LV_COLUMN listColumn;
	TCHAR*    rgszColumns[] = { _T("链接"),
								_T("标记名"),
								_T("描述")};
	listColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_LEFT;
	listColumn.cx = 100;
	listColumn.iSubItem = 0;
	listColumn.pszText = rgszColumns[0];
	m_list.InsertColumn(0, &listColumn);
	
	listColumn.iSubItem = 1;
	listColumn.pszText = rgszColumns[1];
	listColumn.cx = 200;
	m_list.InsertColumn(1, &listColumn);

	listColumn.iSubItem = 2;
	listColumn.pszText = rgszColumns[2];
	listColumn.cx = 200;
	m_list.InsertColumn(2, &listColumn);

#if 0
	listColumn.iSubItem = 3;
	listColumn.pszText = _T("值");
	listColumn.cx = 200;
	m_list.InsertColumn(3, &listColumn);
	
	listColumn.iSubItem = 4;
	listColumn.pszText = _T("地址");
	listColumn.cx = 100;
	m_list.InsertColumn(4, &listColumn);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgDnyTip::OnGetdispinfoLinks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	int pt=pDispInfo->item.lParam;
	
	if(pDispInfo->item.mask & LVIF_TEXT)
	{
		LPTSTR& pszText = pDispInfo->item.pszText;
		switch(pDispInfo->item.iSubItem)
		{
		case 0:
			pszText = arg0.argv[pt];
			//MessageBox(pszText);
			break;
		case 1:
			pszText = arg1.argv[pt];
			break;
		case 2:
			pszText = arg2.argv[pt];
			break;
		case 3:
			if(lock_rtdb(__false, 100)){
				PRTK_TAG p;
				pmc_value_t val, val2;
				p = query_tag_f(&tn);
				if(p){
					memset(&val, 0, sizeof(val));
					proxy_read_tags(1, &tn, &val, 0);
					set_value_type(val2.Flags, dt_string);
					pmc_type_cast(&val, &val2);
					strncpy(curValue, val2.Value.str, sizeof(curValue));
					curValue[sizeof(curValue) - 1] = 0;
					pmc_clear(&val2);
					pszText = curValue;
				}else{
					pszText = "N.A.";
				}
				unlock_rtdb();
			}
			break;
		case 4:
			pszText = address;
			break;
		default:
			ASSERT(0);
		}
	}
	
	*pResult = 0;
}

void CDlgDnyTip::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CDialog::OnNcLButtonDblClk(nHitTest, point);
	OnCancel();
}


void CDlgDnyTip::OnTrend() 
{
	CString tag1, tag2, tag3, tag4, tag5;
	tag1 = arg1.argv[0];
	tag2 = arg1.argv[1];
	tag3 = arg1.argv[2];
	tag4 = arg1.argv[3];
	tag5 = arg1.argv[4];

	//新余专用
#if 0
	CArgs args;
	
	parse_arg_ex(tag1, &args, ".");
	tag1 = args.argv[2];
	if(!tag1.IsEmpty()){
		tag1 = "xyhis.db." + tag1;
	}

	parse_arg_ex(tag2, &args, ".");
	tag2 = args.argv[2];
	if(!tag2.IsEmpty()){
		tag2 = "xyhis.db." + tag1;
	}

	parse_arg_ex(tag3, &args, ".");
	tag3 = args.argv[2];
	if(!tag3.IsEmpty()){
		tag3 = "xyhis.db." + tag3;
	}

	parse_arg_ex(tag4, &args, ".");
	tag4 = args.argv[2];
	if(!tag4.IsEmpty()){
		tag4 = "xyhis.db." + tag4;
	}

	parse_arg_ex(tag5, &args, ".");
	tag5 = args.argv[2];
	if(!tag5.IsEmpty()){
		tag5 = "xyhis.db." + tag5;
	}
#endif
	m_pCtrDlg->PopupTrend(tag1, tag2, tag3, tag4, tag5);
}
