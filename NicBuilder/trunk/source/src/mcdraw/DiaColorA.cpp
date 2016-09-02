// DiaColorA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaColorA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaColorA dialog


CDiaColorA::CDiaColorA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaColorA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaColorA)
	m_Split1 = 0.0f;
	m_Split2 = 0.0f;
	m_Split3 = 0.0f;
	m_Split4 = 0.0f;
	m_TagName = _T("");
	//}}AFX_DATA_INIT
	m_Color0=0x00000000;
	m_Color1=0x00000000;
	m_Color2=0x00000000;
	m_Color3=0x00000000;
	m_Color4=0x00000000;
}


void CDiaColorA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaColorA)
	DDX_Text(pDX, IDC_SPLIT1, m_Split1);
	DDX_Text(pDX, IDC_SPLIT2, m_Split2);
	DDX_Text(pDX, IDC_SPLIT3, m_Split3);
	DDX_Text(pDX, IDC_SPLIT4, m_Split4);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaColorA, CDialog)
	//{{AFX_MSG_MAP(CDiaColorA)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_COLOR0, OnColor0)
	ON_BN_CLICKED(IDC_COLOR1, OnColor1)
	ON_BN_CLICKED(IDC_COLOR2, OnColor2)
	ON_BN_CLICKED(IDC_COLOR3, OnColor3)
	ON_BN_CLICKED(IDC_COLOR4, OnColor4)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaColorA message handlers

void CDiaColorA::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CBrush brush;
	if(lpDrawItemStruct->CtlID==IDC_COLOR0){
		brush.CreateSolidBrush(m_Color0);
	}else if(lpDrawItemStruct->CtlID==IDC_COLOR1){
		brush.CreateSolidBrush(m_Color1);
	}else if(lpDrawItemStruct->CtlID==IDC_COLOR2){
		brush.CreateSolidBrush(m_Color2);
	}else if(lpDrawItemStruct->CtlID==IDC_COLOR3){
		brush.CreateSolidBrush(m_Color3);
	}else{
		brush.CreateSolidBrush(m_Color4);
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

void CDiaColorA::OnColor0() 
{
    CColorDialog dlg(m_Color0,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_Color0=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorA::OnColor1() 
{
    CColorDialog dlg(m_Color1,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_Color1=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorA::OnColor2() 
{
    CColorDialog dlg(m_Color2,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_Color2=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorA::OnColor3() 
{
    CColorDialog dlg(m_Color3,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_Color3=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorA::OnColor4() 
{
    CColorDialog dlg(m_Color4,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_Color4=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorA::OnSeltag() 
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
