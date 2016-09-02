// DiaSlidV.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaSlidV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideV dialog


CDiaSlideV::CDiaSlideV(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSlideV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSlideV)
	m_TagName = _T("");
	m_BottomSlideO = 0.0f;
	m_BottomSlideT = 0.0f;
	m_TopSlideO = 0.0f;
	m_TopSlideT = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaSlideV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSlideV)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_BOTTOMSLIDE_O, m_BottomSlideO);
	DDX_Text(pDX, IDC_BOTTOMSLIDE_T, m_BottomSlideT);
	DDX_Text(pDX, IDC_TOPSLIDE_O, m_TopSlideO);
	DDX_Text(pDX, IDC_TOPSLIDE_T, m_TopSlideT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSlideV, CDialog)
	//{{AFX_MSG_MAP(CDiaSlideV)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideV message handlers

void CDiaSlideV::OnSeltag() 
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
