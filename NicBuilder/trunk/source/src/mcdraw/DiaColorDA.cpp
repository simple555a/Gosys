// DiaColorDA.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaColorDA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaColorDA dialog


CDiaColorDA::CDiaColorDA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaColorDA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaColorDA)
	m_TagName = _T("");
	//}}AFX_DATA_INIT
	m_ColorNomal=0x0000ff00;
	m_ColorAlarm=0x000000ff;
}


void CDiaColorDA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaColorDA)
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaColorDA, CDialog)
	//{{AFX_MSG_MAP(CDiaColorDA)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_COLORALARM, OnColoralarm)
	ON_BN_CLICKED(IDC_COLORNOMAL, OnColornomal)
	ON_BN_CLICKED(IDC_SELTAG, OnSeltag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaColorDA message handlers

void CDiaColorDA::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CBrush brush;
	if(lpDrawItemStruct->CtlID==IDC_COLORNOMAL){
		brush.CreateSolidBrush(m_ColorNomal);
	}else{
		brush.CreateSolidBrush(m_ColorAlarm);
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

void CDiaColorDA::OnColoralarm() 
{
    CColorDialog dlg(m_ColorAlarm,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorAlarm=dlg.GetColor();
	}
	SetFocus();
	
}

void CDiaColorDA::OnColornomal() 
{
    CColorDialog dlg(m_ColorNomal,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_ColorNomal=dlg.GetColor();
	}
	SetFocus();	
}

void CDiaColorDA::OnSeltag() 
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
