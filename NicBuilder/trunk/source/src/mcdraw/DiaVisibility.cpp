// DiaVisibility.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaVisibility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaVisibility dialog


CDiaVisibility::CDiaVisibility(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaVisibility::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaVisibility)
	m_TagName = _T("");
	m_Visible = -1;
	//}}AFX_DATA_INIT
}


void CDiaVisibility::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaVisibility)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Radio(pDX, IDC_VISIBALE, m_Visible);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaVisibility, CDialog)
	//{{AFX_MSG_MAP(CDiaVisibility)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaVisibility message handlers

void CDiaVisibility::OnSeltag() 
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
