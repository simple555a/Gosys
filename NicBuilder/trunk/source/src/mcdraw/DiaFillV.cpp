// DiaFillV.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaFillV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaFillV dialog


CDiaFillV::CDiaFillV(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaFillV::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaFillV)
	m_TagName = _T("");
	m_VerRef = -1;
	m_MinVFillO = 0.0f;
	m_MaxVFillT = 0.0f;
	m_MaxVFillO = 0.0f;
	m_MinVFillT = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaFillV::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaFillV)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Radio(pDX, IDC_TOP, m_VerRef);
	DDX_Text(pDX, IDC_MINVFILL_O, m_MinVFillO);
	DDX_Text(pDX, IDC_MAXVFILL_T, m_MaxVFillT);
	DDX_Text(pDX, IDC_MAXVFILL_O, m_MaxVFillO);
	DDX_Text(pDX, IDC_MINVFILL_T, m_MinVFillT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaFillV, CDialog)
	//{{AFX_MSG_MAP(CDiaFillV)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaFillV message handlers

void CDiaFillV::OnSeltag() 
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
