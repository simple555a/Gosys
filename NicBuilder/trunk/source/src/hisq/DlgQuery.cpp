// DlgQuery.cpp : implementation file
//

#include "stdafx.h"
#include "hisq.h"
#include "DlgQuery.h"
#include "config.h"
#include "pmcseeker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery dialog


CDlgQuery::CDlgQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuery)
	m_date = COleDateTime::GetCurrentTime();
	m_time = COleDateTime::GetCurrentTime();
	m_count = 10;
	m_tag1 = _T("");
	m_tag10 = _T("");
	m_tag2 = _T("");
	m_tag3 = _T("");
	m_tag4 = _T("");
	m_tag5 = _T("");
	m_tag6 = _T("");
	m_tag7 = _T("");
	m_tag8 = _T("");
	m_tag9 = _T("");
	m_dis1 = _T("");
	m_dis2 = _T("");
	m_dis3 = _T("");
	m_dis4 = _T("");
	m_dis5 = _T("");
	m_dis6 = _T("");
	m_dis7 = _T("");
	m_dis8 = _T("");
	m_dis9 = _T("");
	m_dis10 = _T("");
	m_dateEnd = COleDateTime::GetCurrentTime();
	m_timeEnd = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDlgQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQuery)
	DDX_DateTimeCtrl(pDX, IDC_DATE_STA, m_date);
	DDX_DateTimeCtrl(pDX, IDC_TIME_STA, m_time);
	DDX_Text(pDX, IDC_TAG1, m_tag1);
	DDV_MaxChars(pDX, m_tag1, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG10, m_tag10);
	DDV_MaxChars(pDX, m_tag10, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG2, m_tag2);
	DDV_MaxChars(pDX, m_tag2, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG3, m_tag3);
	DDV_MaxChars(pDX, m_tag3, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG4, m_tag4);
	DDV_MaxChars(pDX, m_tag4, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG5, m_tag5);
	DDV_MaxChars(pDX, m_tag5, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG6, m_tag6);
	DDV_MaxChars(pDX, m_tag6, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG7, m_tag7);
	DDV_MaxChars(pDX, m_tag7, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG8, m_tag8);
	DDV_MaxChars(pDX, m_tag8, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG9, m_tag9);
	DDV_MaxChars(pDX, m_tag9, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_DIS1, m_dis1);
	DDX_Text(pDX, IDC_DIS2, m_dis2);
	DDX_Text(pDX, IDC_DIS3, m_dis3);
	DDX_Text(pDX, IDC_DIS4, m_dis4);
	DDX_Text(pDX, IDC_DIS5, m_dis5);
	DDX_Text(pDX, IDC_DIS6, m_dis6);
	DDX_Text(pDX, IDC_DIS7, m_dis7);
	DDX_Text(pDX, IDC_DIS8, m_dis8);
	DDX_Text(pDX, IDC_DIS9, m_dis9);
	DDX_Text(pDX, IDC_DIS10, m_dis10);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_timeEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgQuery)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_SELTAG1, OnSeltag1)
	ON_BN_CLICKED(IDC_SELTAG2, OnSeltag2)
	ON_BN_CLICKED(IDC_SELTAG3, OnSeltag3)
	ON_BN_CLICKED(IDC_SELTAG4, OnSeltag4)
	ON_BN_CLICKED(IDC_SELTAG5, OnSeltag5)
	ON_BN_CLICKED(IDC_SELTAG6, OnSeltag6)
	ON_BN_CLICKED(IDC_SELTAG7, OnSeltag7)
	ON_BN_CLICKED(IDC_SELTAG8, OnSeltag8)
	ON_BN_CLICKED(IDC_SELTAG9, OnSeltag9)
	ON_BN_CLICKED(IDC_SELTAG10, OnSeltag10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuery message handlers

inline void trim(CString *pstr){
	(*pstr).TrimLeft();
	(*pstr).TrimRight();
	/*
	CString stmp;
	stmp.TrimLeft((LPCTSTR)(*pstr));
	(*pstr).TrimRight((LPCTSTR)stmp);
	*/
}

void CDlgQuery::OnOK() 
{
	if(!UpdateData()) return;
	
	trim(&m_tag1);
	trim(&m_tag2);
	trim(&m_tag3);
	trim(&m_tag4);
	trim(&m_tag5);
	trim(&m_tag6);
	trim(&m_tag7);
	trim(&m_tag8);
	trim(&m_tag9);
	trim(&m_tag10);

	int icount=0;
	if(!m_tag1.IsEmpty()) icount++;
	if(!m_tag2.IsEmpty()) icount++;
	if(!m_tag3.IsEmpty()) icount++;
	if(!m_tag4.IsEmpty()) icount++;
	if(!m_tag5.IsEmpty()) icount++;
	if(!m_tag6.IsEmpty()) icount++;
	if(!m_tag7.IsEmpty()) icount++;
	if(!m_tag8.IsEmpty()) icount++;
	if(!m_tag9.IsEmpty()) icount++;
	if(!m_tag10.IsEmpty()) icount++;

	if(!icount){
		MessageBox("至少需要输入一个数据点",NULL,MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	CDialog::OnOK();
}

extern CMcHisQueryApp theApp;

void CDlgQuery::OnSave() 
{
	CFileDialog dlg(false, "hqc", "", OFN_OVERWRITEPROMPT, "hqc Files(*.hqc)|*.hqc||");
    CString sPath = theApp.m_path + "config\\hisq";
    dlg.m_ofn.lpstrInitialDir = sPath;

	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();

#define ss(i)\
    fprintf(pConf, ":%s ",m_tag##i);\
	fprintf(pConf, ":%s ",pmf->m_discription[i-1]);\
	fprintf(pConf, "%f ",pmf->m_fMin[i-1]);\
	fprintf(pConf, "%f ",pmf->m_fMax[i-1]);\

	if(dlg.DoModal()==IDOK){
        UpdateData();
        CString strFile = dlg.GetPathName();
        FILE *pConf=fopen(strFile, "w+");
        if(pConf){
			ss(1)
			ss(2)
			ss(3)
			ss(4)
			ss(5)
			ss(6)
			ss(7)
			ss(8)
			ss(9)
			ss(10)
            fclose(pConf);
        }

	}	
	
}

void CDlgQuery::OnLoad() 
{
	CFileDialog dlg(true, "hqc", "", OFN_OVERWRITEPROMPT, "hqc Files(*.hqc)|*.hqc||");
    CString sPath = theApp.m_path + "config\\hisq";
    dlg.m_ofn.lpstrInitialDir = sPath;

	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();

#define ll(i)\
	fscanf(pConf, "%s",line);\
	m_tag##i.Format("%s",&line[1]);\
	fscanf(pConf, "%s",line);\
	m_dis##i.Format("%s", &line[1]);\
	pmf->m_discription[i-1]=m_dis##i;\
	fscanf(pConf, "%f",&pmf->m_fMin[i-1]);\
	fscanf(pConf, "%f",&pmf->m_fMax[i-1]);\

	if(dlg.DoModal()==IDOK){
        CString strFile = dlg.GetPathName();
        FILE *pConf = fopen(strFile, "r");
		CString str;
        if(pConf){
		    char line[1024];
			ll(1)
			ll(2)
			ll(3)
			ll(4)
			ll(5)
			ll(6)
			ll(7)
			ll(8)
			ll(9)
			ll(10)
        }
        UpdateData(FALSE);
    }
}

void set_min_max(int i, RTK_TAG &tag)
{
	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();
	switch(get_value_type(tag.s.Flags)){
	case dt_real4:
	case dt_real8:
	case dt_int32:
		pmf->m_fMin[i] = tag.s.fltMinValue;
		pmf->m_fMax[i] = tag.s.fltMaxValue;
		if(pmf->m_fMax[i]-pmf->m_fMin[i] < 0.000001){
			pmf->m_fMax[i]=pmf->m_fMin[i]+0.000001;

		}
		break;
	default:
		pmf->m_fMin[i] = -0.5;
		pmf->m_fMax[i] = 1.5; 
	}
}

void CDlgQuery::OnSeltag1() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag1 = name;
		
		set_min_max(0, tag);
		m_dis1 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag2() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag2 = name;

		set_min_max(1, tag);
		m_dis2 = tag.s.Description;

		UpdateData(false);
	}		
}

void CDlgQuery::OnSeltag3() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag3 = name;

		set_min_max(2, tag);
		m_dis3 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag4() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag4 = name;

		set_min_max(3, tag);
		m_dis4 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag5() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag5 = name;

		set_min_max(4, tag);
		m_dis5 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag6() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag6 = name;

		set_min_max(5, tag);
		m_dis6 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag7() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag7 = name;

		set_min_max(6, tag);
		m_dis7 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag8() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag8 = name;

		set_min_max(7, tag);
		m_dis8 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag9() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag9 = name;

		set_min_max(8, tag);
		m_dis9 = tag.s.Description;

		UpdateData(false);
	}
}

void CDlgQuery::OnSeltag10() 
{
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		m_tag10 = name;

		set_min_max(9, tag);
		m_dis10 = tag.s.Description;

		UpdateData(false);
	}
}
