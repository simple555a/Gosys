// DiaColorD.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaColorD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaColorD dialog


CDiaColorD::CDiaColorD(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaColorD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaColorD)
	m_TagName = _T("");
	//}}AFX_DATA_INIT
	m_ColorOn=0x0000ff00;
	m_ColorOff=0x000000ff;
}


void CDiaColorD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaColorD)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaColorD, CDialog)
	//{{AFX_MSG_MAP(CDiaColorD)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_COLOROFF, OnColoroff)
	ON_BN_CLICKED(IDC_COLORON, OnColoron)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaColorD message handlers

void CDiaColorD::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CBrush brush;
	if(lpDrawItemStruct->CtlID==IDC_COLORON){
		brush.CreateSolidBrush(m_ColorOn);
	}else{
		brush.CreateSolidBrush(m_ColorOff);
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

void CDiaColorD::OnColoroff() 
{
    CColorDialog dlg(m_ColorOff,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorOff=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorD::OnColoron() 
{
    CColorDialog dlg(m_ColorOn,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorOn=dlg.GetColor();
	}
	SetFocus();
}

void CDiaColorD::OnSeltag() 
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
