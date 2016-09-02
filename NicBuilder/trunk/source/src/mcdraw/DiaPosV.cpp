// DiaPosV.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaPosV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaPosV dialog


CDiaPosV::CDiaPosV(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaPosV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaPosV)
	m_TagName = _T("");
	m_TopPosT = 0.0f;
	m_TopPosO = 0.0f;
	m_BottomPosT = 0.0f;
	m_BottomPosO = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaPosV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaPosV)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDX_Text(pDX, IDC_TOPPOS_T, m_TopPosT);
	DDX_Text(pDX, IDC_TOPPOS_O, m_TopPosO);
	DDX_Text(pDX, IDC_BOTTOMPOS_T, m_BottomPosT);
	DDX_Text(pDX, IDC_BOTTOMPOS_O, m_BottomPosO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaPosV, CDialog)
	//{{AFX_MSG_MAP(CDiaPosV)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaPosV message handlers

void CDiaPosV::OnSeltag() 
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
