// DlgFmtCurve.cpp : implementation file
//

#include "stdafx.h"
#include "hisq.h"
#include "DlgFmtCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFmtCurve dialog


CDlgFmtCurve::CDlgFmtCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFmtCurve::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFmtCurve)
	m_bShowGrid = FALSE;
	m_max1 = 0.0f;
	m_max10 = 0.0f;
	m_max2 = 0.0f;
	m_max3 = 0.0f;
	m_max4 = 0.0f;
	m_max5 = 0.0f;
	m_max6 = 0.0f;
	m_max7 = 0.0f;
	m_max8 = 0.0f;
	m_max9 = 0.0f;
	m_min1 = 0.0f;
	m_min10 = 0.0f;
	m_min2 = 0.0f;
	m_min3 = 0.0f;
	m_min4 = 0.0f;
	m_min5 = 0.0f;
	m_min6 = 0.0f;
	m_min7 = 0.0f;
	m_min8 = 0.0f;
	m_min9 = 0.0f;
	m_w1 = -1;
	m_w2 = -1;
	m_w3 = -1;
	m_w4 = -1;
	m_w5 = -1;
	m_w6 = -1;
	m_w7 = -1;
	m_w8 = -1;
	m_w9 = -1;
	m_w10 = -1;
	//}}AFX_DATA_INIT
}


void CDlgFmtCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFmtCurve)
	DDX_Check(pDX, IDC_SHOW_GRID, m_bShowGrid);
	DDX_Text(pDX, IDC_MAX1, m_max1);
	DDX_Text(pDX, IDC_MAX10, m_max10);
	DDX_Text(pDX, IDC_MAX2, m_max2);
	DDX_Text(pDX, IDC_MAX3, m_max3);
	DDX_Text(pDX, IDC_MAX4, m_max4);
	DDX_Text(pDX, IDC_MAX5, m_max5);
	DDX_Text(pDX, IDC_MAX6, m_max6);
	DDX_Text(pDX, IDC_MAX7, m_max7);
	DDX_Text(pDX, IDC_MAX8, m_max8);
	DDX_Text(pDX, IDC_MAX9, m_max9);
	DDX_Text(pDX, IDC_MIN1, m_min1);
	DDX_Text(pDX, IDC_MIN10, m_min10);
	DDX_Text(pDX, IDC_MIN2, m_min2);
	DDX_Text(pDX, IDC_MIN3, m_min3);
	DDX_Text(pDX, IDC_MIN4, m_min4);
	DDX_Text(pDX, IDC_MIN5, m_min5);
	DDX_Text(pDX, IDC_MIN6, m_min6);
	DDX_Text(pDX, IDC_MIN7, m_min7);
	DDX_Text(pDX, IDC_MIN8, m_min8);
	DDX_Text(pDX, IDC_MIN9, m_min9);
	DDX_CBIndex(pDX, IDC_VER_WT1, m_w1);
	DDX_CBIndex(pDX, IDC_VER_WT2, m_w2);
	DDX_CBIndex(pDX, IDC_VER_WT3, m_w3);
	DDX_CBIndex(pDX, IDC_VER_WT4, m_w4);
	DDX_CBIndex(pDX, IDC_VER_WT5, m_w5);
	DDX_CBIndex(pDX, IDC_VER_WT6, m_w6);
	DDX_CBIndex(pDX, IDC_VER_WT7, m_w7);
	DDX_CBIndex(pDX, IDC_VER_WT8, m_w8);
	DDX_CBIndex(pDX, IDC_VER_WT9, m_w9);
	DDX_CBIndex(pDX, IDC_VER_WT10, m_w10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFmtCurve, CDialog)
	//{{AFX_MSG_MAP(CDlgFmtCurve)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_PEN1, OnPen1)
	ON_BN_CLICKED(IDC_PEN10, OnPen10)
	ON_BN_CLICKED(IDC_PEN2, OnPen2)
	ON_BN_CLICKED(IDC_PEN3, OnPen3)
	ON_BN_CLICKED(IDC_PEN4, OnPen4)
	ON_BN_CLICKED(IDC_PEN5, OnPen5)
	ON_BN_CLICKED(IDC_PEN6, OnPen6)
	ON_BN_CLICKED(IDC_PEN7, OnPen7)
	ON_BN_CLICKED(IDC_PEN8, OnPen8)
	ON_BN_CLICKED(IDC_PEN9, OnPen9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFmtCurve message handlers

BOOL CDlgFmtCurve::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CComboBox *p;
	p=(CComboBox*)GetDlgItem(IDC_HOR_GRID);
	p->SetCurSel(m_iHorGrid-2);
	p=(CComboBox*)GetDlgItem(IDC_VER_GRID);
	p->SetCurSel(m_iVerGrid-2);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFmtCurve::OnOK() 
{
	CComboBox *p;
	p=(CComboBox*)GetDlgItem(IDC_HOR_GRID);
	m_iHorGrid=2+p->GetCurSel();
	p=(CComboBox*)GetDlgItem(IDC_VER_GRID);
	m_iVerGrid=2+p->GetCurSel();
		
	CDialog::OnOK();
}

void CDlgFmtCurve::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));

	CPen pen;
	pen.CreateStockObject(NULL_PEN);
	HPEN hOldPen=(HPEN)SelectObject(lpDrawItemStruct->hDC,pen.GetSafeHandle());
	HBRUSH hOldBrush=(HBRUSH)SelectObject(lpDrawItemStruct->hDC,brush.GetSafeHandle());
	Rectangle(lpDrawItemStruct->hDC,0,0,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom);
	
	POINT op;
	CPen pen1;

	if(lpDrawItemStruct->CtlID==IDC_PEN1){
		pen1.CreatePen(PS_SOLID,0,m_color[0]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN2){
		pen1.CreatePen(PS_SOLID,0,m_color[1]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN3){
		pen1.CreatePen(PS_SOLID,0,m_color[2]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN4){
		pen1.CreatePen(PS_SOLID,0,m_color[3]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN5){
		pen1.CreatePen(PS_SOLID,0,m_color[4]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN6){
		pen1.CreatePen(PS_SOLID,0,m_color[5]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN7){
		pen1.CreatePen(PS_SOLID,0,m_color[6]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN8){
		pen1.CreatePen(PS_SOLID,0,m_color[7]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN9){
		pen1.CreatePen(PS_SOLID,0,m_color[8]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN10){
		pen1.CreatePen(PS_SOLID,0,m_color[9]);
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}

	SelectObject(lpDrawItemStruct->hDC,hOldBrush);
	SelectObject(lpDrawItemStruct->hDC,hOldPen);
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDlgFmtCurve::OnPen1() 
{
    CColorDialog dlg(m_color[0],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[0]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen10() 
{
    CColorDialog dlg(m_color[9],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[9]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen2() 
{
    CColorDialog dlg(m_color[1],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[1]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen3() 
{
    CColorDialog dlg(m_color[2],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[2]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen4() 
{
    CColorDialog dlg(m_color[3],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[3]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen5() 
{
    CColorDialog dlg(m_color[4],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[4]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen6() 
{
    CColorDialog dlg(m_color[5],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[5]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen7() 
{
    CColorDialog dlg(m_color[6],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[6]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen8() 
{
    CColorDialog dlg(m_color[7],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[7]=dlg.GetColor();
	}
	SetFocus();	
}

void CDlgFmtCurve::OnPen9() 
{
    CColorDialog dlg(m_color[8],0,NULL);
	if(dlg.DoModal()==IDOK){
		m_color[8]=dlg.GetColor();
	}
	SetFocus();	
}
