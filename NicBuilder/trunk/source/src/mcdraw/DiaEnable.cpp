// DiaEnable.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaEnable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaEnable dialog


CDiaEnable::CDiaEnable(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaEnable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaEnable)
	m_Enable = -1;
	m_TagName = _T("");
	//}}AFX_DATA_INIT
}


void CDiaEnable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaEnable)
	DDX_Radio(pDX, IDC_ENABLE, m_Enable);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaEnable, CDialog)
	//{{AFX_MSG_MAP(CDiaEnable)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaEnable message handlers

void CDiaEnable::OnSeltag() 
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
