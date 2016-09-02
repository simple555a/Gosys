// DiaSlideH.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaSlideH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideH dialog


CDiaSlideH::CDiaSlideH(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSlideH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSlideH)
	m_TagName = _T("");
	m_LeftSlideO = 0.0f;
	m_LeftSlideT = 0.0f;
	m_RightSlideO = 0.0f;
	m_RightSlideT = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaSlideH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSlideH)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_LEFTSLIDE_O, m_LeftSlideO);
	DDX_Text(pDX, IDC_LEFTSLIDE_T, m_LeftSlideT);
	DDX_Text(pDX, IDC_RIGHTSLIDE_O, m_RightSlideO);
	DDX_Text(pDX, IDC_RIGHTSLIDE_T, m_RightSlideT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSlideH, CDialog)
	//{{AFX_MSG_MAP(CDiaSlideH)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSlideH message handlers

void CDiaSlideH::OnSeltag() 
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
