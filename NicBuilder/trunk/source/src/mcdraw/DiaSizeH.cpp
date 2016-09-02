// DiaSizeH.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaSizeH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeH dialog


CDiaSizeH::CDiaSizeH(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSizeH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSizeH)
	m_HorRef = -1;
	m_TagName = _T("");
	m_MaxWidthO = 0.0f;
	m_MaxWidthT = 0.0f;
	m_MinWidthO = 0.0f;
	m_MinWidthT = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaSizeH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSizeH)
	DDX_Radio(pDX, IDC_LEFT, m_HorRef);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_MAXWIDTH_O, m_MaxWidthO);
	DDX_Text(pDX, IDC_MAXWIDTH_T, m_MaxWidthT);
	DDX_Text(pDX, IDC_MINWIDTH_O, m_MinWidthO);
	DDX_Text(pDX, IDC_MINWIDTH_T, m_MinWidthT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSizeH, CDialog)
	//{{AFX_MSG_MAP(CDiaSizeH)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeH message handlers

void CDiaSizeH::OnSeltag() 
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
