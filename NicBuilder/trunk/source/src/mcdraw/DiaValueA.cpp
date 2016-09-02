// DiaValueA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaValueA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaValueA dialog


CDiaValueA::CDiaValueA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaValueA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaValueA)
	m_TagName = _T("");
	m_Format = _T("");
	//}}AFX_DATA_INIT
}


void CDiaValueA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaValueA)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_FORMAT, m_Format);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaValueA, CDialog)
	//{{AFX_MSG_MAP(CDiaValueA)
	ON_LBN_SELCHANGE(IDC_LIST_FORMAT, OnSelchangeListFormat)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaValueA message handlers

BOOL CDiaValueA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CListBox *p=(CListBox*)GetDlgItem(IDC_LIST_FORMAT);
    p->AddString("%g    自动");
    p->AddString("%.0f  不保留小数");
    p->AddString("%.1f  保留1位小数");
    p->AddString("%.2f  保留2位小数");
    p->AddString("%.3f  保留3位小数");
    p->AddString("%.e   指数");
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDiaValueA::OnSelchangeListFormat() 
{
    CListBox *p=(CListBox*)GetDlgItem(IDC_LIST_FORMAT);
    switch(p->GetCurSel()){
    case 0:
        m_Format="%g";
        break;
    case 1:
        m_Format="%.0f";
        break;
    case 2:
        m_Format="%.1f";
        break;
    case 3:
        m_Format="%.2f";
        break;
    case 4:
        m_Format="%.3f";
        break;
    case 5:
        m_Format="%e";
        break;
    default:
        return;
    }
    UpdateData(FALSE);
}

void CDiaValueA::OnSeltag() 
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
