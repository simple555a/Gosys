// DiaButtonD.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaButtonD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonD dialog

CDiaButtonD::CDiaButtonD(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaButtonD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaButtonD)
	m_Action = -1;
	m_TagName = _T("");
	//}}AFX_DATA_INIT
}


void CDiaButtonD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaButtonD)
	DDX_Control(pDX, IDC_COMBO1, m_pulseWidth);
	DDX_Radio(pDX, IDC_SET, m_Action);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaButtonD, CDialog)
	//{{AFX_MSG_MAP(CDiaButtonD)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonD message handlers

void CDiaButtonD::OnSeltag() 
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
		
		m_TagName = name;

		UpdateData(false);
	}	
}

BOOL CDiaButtonD::OnInitDialog() 
{	
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_pulseWidth.AddString("自动");
	m_pulseWidth.AddString("100");
	m_pulseWidth.AddString("200");
	m_pulseWidth.AddString("500");
	m_pulseWidth.AddString("1000");
	m_pulseWidth.AddString("2000");
	m_pulseWidth.AddString("10000");

	if(-1 == m_iPulseWidth){
		m_pulseWidth.SetCurSel(0);
	}else{
		char buf[32];
		sprintf(buf, "%d", m_iPulseWidth);
		m_pulseWidth.SetWindowText(buf);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaButtonD::OnEditchangeCombo1() 
{
}

void CDiaButtonD::OnOK() 
{
	// TODO: Add extra validation here
	// TODO: Add your control notification handler code here
	CString s;
	int p;
	if(m_pulseWidth.GetCurSel() == 0){
		m_iPulseWidth = -1;
	}else{
		m_pulseWidth.GetWindowText(s);
		p = atoi(s);
		if(p > 0){
			m_iPulseWidth = p;
		}else{
			AfxMessageBox("请输入合法的脉冲时间间隔.");
			return;
		}
	}
	
	CDialog::OnOK();
}
