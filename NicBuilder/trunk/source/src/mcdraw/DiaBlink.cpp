// DiaBlink.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaBlink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaBlink dialog


CDiaBlink::CDiaBlink(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaBlink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaBlink)
	m_TagName = _T("");
	m_Visible = 0;
	m_Speed = 1;
	//}}AFX_DATA_INIT
	m_ColorBrush=0x00000000;
	m_ColorPen=0x00000000;
	m_ColorText=0x00000000;
}


void CDiaBlink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaBlink)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Radio(pDX, IDC_VISIBALE, m_Visible);
	DDX_Radio(pDX, IDC_FAST, m_Speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaBlink, CDialog)
	//{{AFX_MSG_MAP(CDiaBlink)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_COLORBRUSH, OnColorbrush)
	ON_BN_CLICKED(IDC_COLORHTEXT, OnColorhtext)
	ON_BN_CLICKED(IDC_COLORPEN, OnColorpen)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaBlink message handlers

void CDiaBlink::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CBrush brush;
     
	if(lpDrawItemStruct->CtlID==IDC_COLORBRUSH){
        brush.CreateSolidBrush(m_ColorBrush);
	}else if(lpDrawItemStruct->CtlID==IDC_COLORPEN){
        brush.CreateSolidBrush(m_ColorPen);
	}else{
        brush.CreateSolidBrush(m_ColorText);
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

void CDiaBlink::OnColorbrush() 
{
    CColorDialog dlg(m_ColorBrush,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorBrush=dlg.GetColor();
	}
	SetFocus();
}

void CDiaBlink::OnColorhtext() 
{
    CColorDialog dlg(m_ColorText,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorText=dlg.GetColor();
	}
	SetFocus();
}

void CDiaBlink::OnColorpen() 
{
    CColorDialog dlg(m_ColorPen,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorPen=dlg.GetColor();
	}
	SetFocus();
}

void CDiaBlink::OnSeltag() 
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
