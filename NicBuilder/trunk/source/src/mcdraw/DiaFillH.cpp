// DiaFillH.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaFillH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaFillH dialog


CDiaFillH::CDiaFillH(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaFillH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaFillH)
	m_HorRef = -1;
	m_TagName = _T("");
	m_MinHFillT = 0.0f;
	m_MinHFillO = 0.0f;
	m_MaxHFillT = 0.0f;
	m_MaxHFillO = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaFillH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaFillH)
	DDX_Radio(pDX, IDC_LEFT, m_HorRef);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_MINHFILL_T, m_MinHFillT);
	DDX_Text(pDX, IDC_MINHFILL_O, m_MinHFillO);
	DDX_Text(pDX, IDC_MAXHFILL_T, m_MaxHFillT);
	DDX_Text(pDX, IDC_MAXHFILL_O, m_MaxHFillO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaFillH, CDialog)
	//{{AFX_MSG_MAP(CDiaFillH)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaFillH message handlers

void CDiaFillH::OnSeltag() 
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
