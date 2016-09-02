// DiaValueD.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaValueD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaValueD dialog


CDiaValueD::CDiaValueD(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaValueD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaValueD)
	m_OffMsg = _T("");
	m_OnMsg = _T("");
	m_TagName = _T("");
	//}}AFX_DATA_INIT
}


void CDiaValueD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaValueD)
	DDX_Text(pDX, IDC_OFFMSG, m_OffMsg);
	DDX_Text(pDX, IDC_ONMSG, m_OnMsg);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaValueD, CDialog)
	//{{AFX_MSG_MAP(CDiaValueD)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaValueD message handlers

void CDiaValueD::OnSeltag() 
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
