// DiaColorAA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaColorAA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaColorAA dialog


CDiaColorAA::CDiaColorAA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaColorAA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaColorAA)
	m_TagName = _T("");
	//}}AFX_DATA_INIT
	m_ColorN=0x00000000;
	m_ColorL1=0x00000000;
	m_ColorL2=0x00000000;
	m_ColorH1=0x00000000;
	m_ColorH2=0x00000000;
	m_ColorR=0x00000000;
}


void CDiaColorAA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaColorAA)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaColorAA, CDialog)
	//{{AFX_MSG_MAP(CDiaColorAA)
	ON_BN_CLICKED(IDC_COLORH1, OnColorh1)
	ON_BN_CLICKED(IDC_COLORH2, OnColorh2)
	ON_BN_CLICKED(IDC_COLORL1, OnColorl1)
	ON_BN_CLICKED(IDC_COLORL2, OnColorl2)
	ON_BN_CLICKED(IDC_COLORN, OnColorn)
	ON_BN_CLICKED(IDC_COLORR, OnColorr)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaColorAA message handlers

void CDiaColorAA::OnColorh1() 
{
    CColorDialog dlg(m_ColorH1,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorH1=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnColorh2() 
{
    CColorDialog dlg(m_ColorH2,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorH2=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnColorl1() 
{
    CColorDialog dlg(m_ColorL1,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorL1=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnColorl2() 
{
    CColorDialog dlg(m_ColorL2,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorL2=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnColorn() 
{
    CColorDialog dlg(m_ColorN,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorN=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnColorr() 
{
    CColorDialog dlg(m_ColorR,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorR=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorAA::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CBrush brush;
	if(lpDrawItemStruct->CtlID==IDC_COLORL2){
		brush.CreateSolidBrush(m_ColorL2);
	}else if(lpDrawItemStruct->CtlID==IDC_COLORL1){
		brush.CreateSolidBrush(m_ColorL1);
	}else if(lpDrawItemStruct->CtlID==IDC_COLORN){
		brush.CreateSolidBrush(m_ColorN);
	}else if(lpDrawItemStruct->CtlID==IDC_COLORH1){
		brush.CreateSolidBrush(m_ColorH1);
	}else if(lpDrawItemStruct->CtlID==IDC_COLORH2){
		brush.CreateSolidBrush(m_ColorH2);
	}else{
		brush.CreateSolidBrush(m_ColorR);
	}

	CPen pen;
	pen.CreateStockObject(NULL_PEN);
	HPEN hOldPen=(HPEN)SelectObject(lpDrawItemStruct->hDC,pen.GetSafeHandle());
	HBRUSH hOldBrush=(HBRUSH)SelectObject(lpDrawItemStruct->hDC,brush.GetSafeHandle());
	Rectangle(lpDrawItemStruct->hDC,0,0,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom);

	SelectObject(lpDrawItemStruct->hDC,hOldBrush);
	SelectObject(lpDrawItemStruct->hDC,hOldPen);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDiaColorAA::OnSeltag() 
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
