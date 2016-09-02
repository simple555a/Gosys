// DiaInputD.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaInputD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaInputD dialog


CDiaInputD::CDiaInputD(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaInputD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaInputD)
	m_OffMsg = _T("");
	m_OnMsg = _T("");
	m_ResetPrm = _T("");
	m_SetPrm = _T("");
	m_TagName = _T("");
	m_WndPrm = _T("");
	//}}AFX_DATA_INIT
}


void CDiaInputD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaInputD)
	DDX_Text(pDX, IDC_OFFMSG, m_OffMsg);
	DDX_Text(pDX, IDC_ONMSG, m_OnMsg);
	DDX_Text(pDX, IDC_RESETPRM, m_ResetPrm);
	DDX_Text(pDX, IDC_SETPRM, m_SetPrm);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_WNDPRM, m_WndPrm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaInputD, CDialog)
	//{{AFX_MSG_MAP(CDiaInputD)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaInputD message handlers

void CDiaInputD::OnSeltag() 
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
