// DlgTrendTime.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgTrendTime.h"
#include "DlgTrendComSav.h"
#include "DlgSelectKName.h"

#pragma comment(lib, "pmcSeeker.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL gbNoSwitch;

/////////////////////////////////////////////////////////////////////////////
// CDlgTrendTime dialog


CDlgTrendTime::CDlgTrendTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrendTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTrendTime)
	m_date = COleDateTime::GetCurrentTime();
	m_time = COleDateTime::GetCurrentTime();
	m_mode = 1;
	m_ca1 = FALSE;
	m_ca2 = FALSE;
	m_ca3 = FALSE;
	m_ca4 = FALSE;
	m_ca5 = FALSE;
	m_max1 = 0.0f;
	m_max2 = 0.0f;
	m_max3 = 0.0f;
	m_max4 = 0.0f;
	m_max5 = 0.0f;
	m_min1 = 0.0f;
	m_min2 = 0.0f;
	m_min3 = 0.0f;
	m_min4 = 0.0f;
	m_min5 = 0.0f;
	m_tag1 = _T("");
	m_tag2 = _T("");
	m_tag3 = _T("");
	m_tag4 = _T("");
	m_tag5 = _T("");
	m_autoScale = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTrendTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTrendTime)
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_TIME, m_time);
	DDX_CBIndex(pDX, IDC_LENGTH, m_mode);
	DDX_Check(pDX, IDC_CA1, m_ca1);
	DDX_Check(pDX, IDC_CA2, m_ca2);
	DDX_Check(pDX, IDC_CA3, m_ca3);
	DDX_Check(pDX, IDC_CA4, m_ca4);
	DDX_Check(pDX, IDC_CA5, m_ca5);
	DDX_Text(pDX, IDC_MAX1, m_max1);
	DDX_Text(pDX, IDC_MAX2, m_max2);
	DDX_Text(pDX, IDC_MAX3, m_max3);
	DDX_Text(pDX, IDC_MAX4, m_max4);
	DDX_Text(pDX, IDC_MAX5, m_max5);
	DDX_Text(pDX, IDC_MIN1, m_min1);
	DDX_Text(pDX, IDC_MIN2, m_min2);
	DDX_Text(pDX, IDC_MIN3, m_min3);
	DDX_Text(pDX, IDC_MIN4, m_min4);
	DDX_Text(pDX, IDC_MIN5, m_min5);
	DDX_Text(pDX, IDC_TAG1, m_tag1);
	DDV_MaxChars(pDX, m_tag1, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG2, m_tag2);
	DDV_MaxChars(pDX, m_tag2, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG3, m_tag3);
	DDV_MaxChars(pDX, m_tag3, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG4, m_tag4);
	DDV_MaxChars(pDX, m_tag4, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG5, m_tag5);
	DDV_MaxChars(pDX, m_tag5, TAGNAME_TEXT_LENGTH);
	DDX_Check(pDX, IDC_AUTO_SCALE, m_autoScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTrendTime, CDialog)
	//{{AFX_MSG_MAP(CDlgTrendTime)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_1, On1)
	ON_BN_CLICKED(IDC_2, On2)
	ON_BN_CLICKED(IDC_3, On3)
	ON_BN_CLICKED(IDC_4, On4)
	ON_BN_CLICKED(IDC_5, On5)
	ON_BN_CLICKED(IDC_6, On6)
	ON_BN_CLICKED(IDC_7, On7)
	ON_BN_CLICKED(IDC_8, On8)
	ON_BN_CLICKED(IDC_9, On9)
	ON_BN_CLICKED(IDC_10, On10)
	ON_BN_CLICKED(IDC_DOT, OnDot)
	ON_BN_CLICKED(IDC_SUB, OnSub)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_EN_SETFOCUS(IDC_MAX1, OnSetfocusMax1)
	ON_EN_SETFOCUS(IDC_MAX2, OnSetfocusMax2)
	ON_EN_SETFOCUS(IDC_MAX3, OnSetfocusMax3)
	ON_EN_SETFOCUS(IDC_MAX4, OnSetfocusMax4)
	ON_EN_SETFOCUS(IDC_MAX5, OnSetfocusMax5)
	ON_EN_SETFOCUS(IDC_MIN1, OnSetfocusMin1)
	ON_EN_SETFOCUS(IDC_MIN2, OnSetfocusMin2)
	ON_EN_SETFOCUS(IDC_MIN3, OnSetfocusMin3)
	ON_EN_SETFOCUS(IDC_MIN4, OnSetfocusMin4)
	ON_EN_SETFOCUS(IDC_MIN5, OnSetfocusMin5)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHOOSE1, &CDlgTrendTime::OnBnClickedChoose1)
	ON_BN_CLICKED(IDC_CHOOSE2, &CDlgTrendTime::OnBnClickedChoose2)
	ON_BN_CLICKED(IDC_CHOOSE3, &CDlgTrendTime::OnBnClickedChoose3)
	ON_BN_CLICKED(IDC_CHOOSE4, &CDlgTrendTime::OnBnClickedChoose4)
	ON_BN_CLICKED(IDC_CHOOSE5, &CDlgTrendTime::OnBnClickedChoose5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTrendTime message handlers
extern CMcviewApp theApp;

void CDlgTrendTime::OnSave() 
{
	CFileDialog dlg(false, "tcr", "", OFN_OVERWRITEPROMPT, "tcr Files(*.tcr)|*.tcr||");
	CString sPath = theApp.m_path + "config";
	dlg.m_ofn.lpstrInitialDir = sPath;

	if(dlg.DoModal()==IDOK){
		UpdateData();
		CString strFile = dlg.GetPathName();
		FILE *pConf=fopen(strFile, "w+");
		if(pConf){
			// trend 1
			fprintf(pConf, "%d,", m_ca1);
			fprintf(pConf, "%s,", m_tag1);
			fprintf(pConf, "%g,", m_min1);
			fprintf(pConf, "%g\n", m_max1);
			// trend 2
			fprintf(pConf, "%d,",m_ca2);
			fprintf(pConf, "%s,",m_tag2);
			fprintf(pConf, "%g,", m_min2);
			fprintf(pConf, "%g\n", m_max2);
			// trend 3
			fprintf(pConf, "%d,",m_ca3);
			fprintf(pConf, "%s,",m_tag3);
			fprintf(pConf, "%g,", m_min3);
			fprintf(pConf, "%g\n", m_max3);
			// trend 4
			fprintf(pConf, "%d,",m_ca4);
			fprintf(pConf, "%s,",m_tag4);
			fprintf(pConf, "%g,", m_min4);
			fprintf(pConf, "%g\n", m_max4);
			// trend 5
			fprintf(pConf, "%d,",m_ca5);
			fprintf(pConf, "%s,",m_tag5);
			fprintf(pConf, "%g,", m_min5);
			fprintf(pConf, "%g\n", m_max5);

			fclose(pConf);
		}
	}		
}

void CDlgTrendTime::OnLoad()
{
	CDlgTrendComSav dlg;
	dlg.m_dir=theApp.m_path+"config\\*.tcr";

	if(dlg.DoModal()==IDOK){
		CString strFile=theApp.m_path+"config\\"+dlg.m_select+".tcr";
		FILE *pConf = fopen(strFile, "r");
		if(pConf){
			CArgs args;
			char line[1024];
			// 1
			if(NULL != fgets(line,1024,pConf)){
				parse_arg_ey(line,&args,",\n");
				if(0 == strcmp(args.argv[0],"1")){
					m_ca1 = TRUE;
				}else{
					m_ca1 = FALSE;
				}
				m_tag1 = args.argv[1];
				m_min1 = atof(args.argv[2]);
				m_max1 = atof(args.argv[3]);
			}
			// 2
			if(NULL != fgets(line,1024,pConf)){
				parse_arg_ey(line,&args,",\n");
				if(0 == strcmp(args.argv[0],"1")){
					m_ca2 = TRUE;
				}else{
					m_ca2 = FALSE;
				}
				m_tag2 = args.argv[1];
				m_min2 = atof(args.argv[2]);
				m_max2 = atof(args.argv[3]);
			}
			// 3
			if(NULL != fgets(line,1024,pConf)){
				parse_arg_ey(line,&args,",\n");
				if(0 == strcmp(args.argv[0],"1")){
					m_ca3 = TRUE;
				}else{
					m_ca3 = FALSE;
				}
				m_tag3 = args.argv[1];
				m_min3 = atof(args.argv[2]);
				m_max3 = atof(args.argv[3]);
			}
			// 4
			if(NULL != fgets(line,1024,pConf)){
				parse_arg_ey(line,&args,",\n");
				if(0 == strcmp(args.argv[0],"1")){
					m_ca4 = TRUE;
				}else{
					m_ca4 = FALSE;
				}
				m_tag4 = args.argv[1];
				m_min4 = atof(args.argv[2]);
				m_max4 = atof(args.argv[3]);
			}
			// 5
			if(NULL != fgets(line,1024,pConf)){
				parse_arg_ey(line,&args,",\n");
				if(0 == strcmp(args.argv[0],"1")){
					m_ca5 = TRUE;
				}else{
					m_ca5 = FALSE;
				}
				m_tag5 = args.argv[1];
				m_min5 = atof(args.argv[2]);
				m_max5 = atof(args.argv[3]);
			}
		}
		UpdateData(FALSE);
	}
}

extern long glRank;
BOOL CDlgTrendTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN1);
	pBoxOne->SetFocus();
	/*
	CWnd *pWnd = GetDlgItem(IDC_SAVE);;
	if(9 == glRank){
		pWnd->EnableWindow(TRUE);
	}else{
		pWnd->EnableWindow(FALSE); 
	}
	*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgTrendTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->GetDlgCtrlID()==IDC_TAG1){
		pDC->SetTextColor(cuvColor[0]);
	}else if(pWnd->GetDlgCtrlID()==IDC_TAG2){
		pDC->SetTextColor(cuvColor[1]);
	}else if(pWnd->GetDlgCtrlID()==IDC_TAG3){
		pDC->SetTextColor(cuvColor[2]);
	}else if(pWnd->GetDlgCtrlID()==IDC_TAG4){
		pDC->SetTextColor(cuvColor[3]);
	}else if(pWnd->GetDlgCtrlID()==IDC_TAG5){
		pDC->SetTextColor(cuvColor[4]);
	}
	
	return hbr;
}

void CDlgTrendTime::On1() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '1',0);		
}

void CDlgTrendTime::On2() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '2',0);	
}

void CDlgTrendTime::On3() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '3',0);	
}

void CDlgTrendTime::On4() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '4',0);	
}

void CDlgTrendTime::On5() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '5',0);	
}

void CDlgTrendTime::On6() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '6',0);		
}

void CDlgTrendTime::On7() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '7',0);	
}

void CDlgTrendTime::On8() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '8',0);		
}

void CDlgTrendTime::On9() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '9',0);	
}

void CDlgTrendTime::On10() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '0',0);	
}

void CDlgTrendTime::OnDot() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '.',0);	
}

void CDlgTrendTime::OnSub() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '-',0);	
}

void CDlgTrendTime::OnDel() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CLEAR , 0, 0);	
}

void CDlgTrendTime::OnSetfocusMax1() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MAX1);
}

void CDlgTrendTime::OnSetfocusMax2() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MAX2);
}

void CDlgTrendTime::OnSetfocusMax3() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MAX3);
}

void CDlgTrendTime::OnSetfocusMax4() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MAX4);
}

void CDlgTrendTime::OnSetfocusMax5() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MAX5);
}

void CDlgTrendTime::OnSetfocusMin1() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN1);
}

void CDlgTrendTime::OnSetfocusMin2() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN2);
}

void CDlgTrendTime::OnSetfocusMin3() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN3);
}

void CDlgTrendTime::OnSetfocusMin4() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN4);
}

void CDlgTrendTime::OnSetfocusMin5() 
{
	pBoxOne = (CEdit*) GetDlgItem(IDC_MIN5);
}

void CDlgTrendTime::OnBnClickedChoose1()
{
	// TODO: Add your control notification handler code here
	if(gbNoSwitch)
	{
        MessageBox("请选择可以切换到其它应用!", NULL, MB_OK);
		return;
	}

    //
	if( !Discover() )
	{
		return;
	}

	m_tag1 = this->seltagName();

	UpdateData(false);
}

void CDlgTrendTime::OnBnClickedChoose2()
{
	// TODO: Add your control notification handler code here
	if(gbNoSwitch)
	{
		MessageBox("请选择可以切换到其它应用!", NULL, MB_OK);
		return;
	}

	//
	if( !Discover() )
	{
		return;
	}

	m_tag2 = this->seltagName();

	UpdateData(false);
}

void CDlgTrendTime::OnBnClickedChoose3()
{
	// TODO: Add your control notification handler code here
	if(gbNoSwitch)
	{
		MessageBox("请选择可以切换到其它应用!", NULL, MB_OK);
		return;
	}

	//
	if( !Discover() )
	{
		return;
	}

	m_tag3 = this->seltagName();

	UpdateData(false);
}

void CDlgTrendTime::OnBnClickedChoose4()
{
	// TODO: Add your control notification handler code here
	if(gbNoSwitch)
	{
		MessageBox("请选择可以切换到其它应用!", NULL, MB_OK);
		return;
	}

	//
	if( !Discover() )
	{
		return;
	}

	m_tag4 = this->seltagName();

	UpdateData(false);
}

void CDlgTrendTime::OnBnClickedChoose5()
{
	// TODO: Add your control notification handler code here
	if(gbNoSwitch)
	{
		MessageBox("请选择可以切换到其它应用!", NULL, MB_OK);
		return;
	}

	//
	if( !Discover() )
	{
		return;
	}

	m_tag5 = this->seltagName();

	UpdateData(false);
}

CString CDlgTrendTime::seltagName()
{
	CString tmpseltagname;

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);

		tmpseltagname = name;

		//UpdateData(false);
	}
	return tmpseltagname;
}