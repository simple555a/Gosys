// DiaPosH.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaPosH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaPosH dialog


CDiaPosH::CDiaPosH(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaPosH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaPosH)
	m_TagName = _T("");
	m_RightPosT = 0.0f;
	m_RightPosO = 0.0f;
	m_LeftPosT = 0.0f;
	m_LeftPosO = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaPosH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaPosH)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDX_Text(pDX, IDC_RIGHTPOS_T, m_RightPosT);
	DDX_Text(pDX, IDC_RIGHTPOS_O, m_RightPosO);
	DDX_Text(pDX, IDC_LEFTPOS_T, m_LeftPosT);
	DDX_Text(pDX, IDC_LEFTPOS_O, m_LeftPosO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaPosH, CDialog)
	//{{AFX_MSG_MAP(CDiaPosH)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaPosH message handlers

void CDiaPosH::OnSeltag() 
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
