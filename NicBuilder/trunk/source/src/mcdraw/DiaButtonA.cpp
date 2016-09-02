// DiaButtonA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaButtonA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonA dialog


CDiaButtonA::CDiaButtonA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaButtonA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaButtonA)
	m_DownLimit = 0.0f;
	m_Fuction = -1;
	m_Number = 0.0f;
	m_TagName = _T("");
	m_UpLimit = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaButtonA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaButtonA)
	DDX_Text(pDX, IDC_DOWN_LIMIT, m_DownLimit);
	DDX_Radio(pDX, IDC_INC, m_Fuction);
	DDX_Text(pDX, IDC_NUM, m_Number);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_UP_LIMIT, m_UpLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaButtonA, CDialog)
	//{{AFX_MSG_MAP(CDiaButtonA)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaButtonA message handlers

void CDiaButtonA::OnSeltag() 
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
