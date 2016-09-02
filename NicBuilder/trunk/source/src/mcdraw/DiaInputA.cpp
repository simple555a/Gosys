// DiaInputA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaInputA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaInputA dialog


CDiaInputA::CDiaInputA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaInputA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaInputA)
	m_MaxValue = 0.0f;
	m_MinValue = 0.0f;
	m_TagName = _T("");
	m_WndPrm = _T("");
	//}}AFX_DATA_INIT
}


void CDiaInputA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaInputA)
	DDX_Text(pDX, IDC_MAXVALUE, m_MaxValue);
	DDX_Text(pDX, IDC_MINVALUE, m_MinValue);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_WNDPRM, m_WndPrm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaInputA, CDialog)
	//{{AFX_MSG_MAP(CDiaInputA)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaInputA message handlers

void CDiaInputA::OnSeltag() 
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
