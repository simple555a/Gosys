// DiaSizeV.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaSizeV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeV dialog


CDiaSizeV::CDiaSizeV(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSizeV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSizeV)
	m_VerRef = -1;
	m_TagName = _T("");
	m_MaxHeightO = 0.0f;
	m_MaxHeightT = 0.0f;
	m_MinHeightO = 0.0f;
	m_MinHeightT = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaSizeV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSizeV)
	DDX_Radio(pDX, IDC_TOP, m_VerRef);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_MAXHEIGHT_O, m_MaxHeightO);
	DDX_Text(pDX, IDC_MAXHEIGHT_T, m_MaxHeightT);
	DDX_Text(pDX, IDC_MINHEIGHT_O, m_MinHeightO);
	DDX_Text(pDX, IDC_MINHEIGHT_T, m_MinHeightT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSizeV, CDialog)
	//{{AFX_MSG_MAP(CDiaSizeV)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSizeV message handlers

void CDiaSizeV::OnSeltag() 
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
