// DiaTrendAttr.cpp : implementation file
//

#include "stdafx.h"
#include "mcdraw.h"
#include "DiaTrendAttr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaTrendAttr dialog


CDiaTrendAttr::CDiaTrendAttr(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaTrendAttr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaTrendAttr)
	m_tag1 = _T("");
	m_tag2 = _T("");
	m_tag3 = _T("");
	m_tag4 = _T("");
	m_tag5 = _T("");
	m_mode = 0;
	m_ca1 = FALSE;
	m_ca2 = FALSE;
	m_ca3 = FALSE;
	m_ca4 = FALSE;
	m_ca5 = FALSE;
	m_max1 = 0.0f;
	m_max2 = 0.0f;
	m_max3 = 0.0f;
	m_max4 = 0.0f;
	m_max5 = 0.0f;
	m_min1 = 0.0f;
	m_min2 = 0.0f;
	m_min3 = 0.0f;
	m_min4 = 0.0f;
	m_min5 = 0.0f;
	m_ca10 = FALSE;
	m_ca6 = FALSE;
	m_ca7 = FALSE;
	m_ca8 = FALSE;
	m_ca9 = FALSE;
	m_max10 = 0.0f;
	m_max6 = 0.0f;
	m_max7 = 0.0f;
	m_max8 = 0.0f;
	m_max9 = 0.0f;
	m_min10 = 0.0f;
	m_min6 = 0.0f;
	m_min7 = 0.0f;
	m_min8 = 0.0f;
	m_min9 = 0.0f;
	m_tag10 = _T("");
	m_tag6 = _T("");
	m_tag7 = _T("");
	m_tag8 = _T("");
	m_tag9 = _T("");
	//}}AFX_DATA_INIT
}


void CDiaTrendAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaTrendAttr)
	DDX_Text(pDX, IDC_TAG1, m_tag1);
	DDV_MaxChars(pDX, m_tag1, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG2, m_tag2);
	DDV_MaxChars(pDX, m_tag2, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG3, m_tag3);
	DDV_MaxChars(pDX, m_tag3, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG4, m_tag4);
	DDV_MaxChars(pDX, m_tag4, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG5, m_tag5);
	DDV_MaxChars(pDX, m_tag5, TAGNAME_TEXT_LENGTH);
	DDX_CBIndex(pDX, IDC_LENGTH, m_mode);
	DDX_Check(pDX, IDC_CA1, m_ca1);
	DDX_Check(pDX, IDC_CA2, m_ca2);
	DDX_Check(pDX, IDC_CA3, m_ca3);
	DDX_Check(pDX, IDC_CA4, m_ca4);
	DDX_Check(pDX, IDC_CA5, m_ca5);
	DDX_Text(pDX, IDC_MAX1, m_max1);
	DDX_Text(pDX, IDC_MAX2, m_max2);
	DDX_Text(pDX, IDC_MAX3, m_max3);
	DDX_Text(pDX, IDC_MAX4, m_max4);
	DDX_Text(pDX, IDC_MAX5, m_max5);
	DDX_Text(pDX, IDC_MIN1, m_min1);
	DDX_Text(pDX, IDC_MIN2, m_min2);
	DDX_Text(pDX, IDC_MIN3, m_min3);
	DDX_Text(pDX, IDC_MIN4, m_min4);
	DDX_Text(pDX, IDC_MIN5, m_min5);
	DDX_Check(pDX, IDC_CA10, m_ca10);
	DDX_Check(pDX, IDC_CA6, m_ca6);
	DDX_Check(pDX, IDC_CA7, m_ca7);
	DDX_Check(pDX, IDC_CA8, m_ca8);
	DDX_Check(pDX, IDC_CA9, m_ca9);
	DDX_Text(pDX, IDC_MAX10, m_max10);
	DDX_Text(pDX, IDC_MAX6, m_max6);
	DDX_Text(pDX, IDC_MAX7, m_max7);
	DDX_Text(pDX, IDC_MAX8, m_max8);
	DDX_Text(pDX, IDC_MAX9, m_max9);
	DDX_Text(pDX, IDC_MIN10, m_min10);
	DDX_Text(pDX, IDC_MIN6, m_min6);
	DDX_Text(pDX, IDC_MIN7, m_min7);
	DDX_Text(pDX, IDC_MIN8, m_min8);
	DDX_Text(pDX, IDC_MIN9, m_min9);
	DDX_Text(pDX, IDC_TAG10, m_tag10);
	DDV_MaxChars(pDX, m_tag10, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG6, m_tag6);
	DDV_MaxChars(pDX, m_tag6, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG7, m_tag7);
	DDV_MaxChars(pDX, m_tag7, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG8, m_tag8);
	DDV_MaxChars(pDX, m_tag8, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_TAG9, m_tag9);
	DDV_MaxChars(pDX, m_tag9, TAGNAME_TEXT_LENGTH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaTrendAttr, CDialog)
	//{{AFX_MSG_MAP(CDiaTrendAttr)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_PEN5, OnPen5)
	ON_BN_CLICKED(IDC_PEN4, OnPen4)
	ON_BN_CLICKED(IDC_PEN3, OnPen3)
	ON_BN_CLICKED(IDC_PEN2, OnPen2)
	ON_BN_CLICKED(IDC_PEN1, OnPen1)
	ON_BN_CLICKED(IDC_BKCOLOR, OnBkcolor)
	ON_BN_CLICKED(IDC_GDCOLOR, OnGdcolor)
	ON_BN_CLICKED(IDC_PEN10, OnPen10)
	ON_BN_CLICKED(IDC_PEN6, OnPen6)
	ON_BN_CLICKED(IDC_PEN7, OnPen7)
	ON_BN_CLICKED(IDC_PEN8, OnPen8)
	ON_BN_CLICKED(IDC_PEN9, OnPen9)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SELTAG11, &CDiaTrendAttr::OnBnClickedSeltag11)
	ON_BN_CLICKED(IDC_SELTAG12, &CDiaTrendAttr::OnBnClickedSeltag12)
	ON_BN_CLICKED(IDC_SELTAG13, &CDiaTrendAttr::OnBnClickedSeltag13)
	ON_BN_CLICKED(IDC_SELTAG14, &CDiaTrendAttr::OnBnClickedSeltag14)
	ON_BN_CLICKED(IDC_SELTAG15, &CDiaTrendAttr::OnBnClickedSeltag15)
	ON_BN_CLICKED(IDC_SELTAG16, &CDiaTrendAttr::OnBnClickedSeltag16)
	ON_BN_CLICKED(IDC_SELTAG17, &CDiaTrendAttr::OnBnClickedSeltag17)
	ON_BN_CLICKED(IDC_SELTAG18, &CDiaTrendAttr::OnBnClickedSeltag18)
	ON_BN_CLICKED(IDC_SELTAG19, &CDiaTrendAttr::OnBnClickedSeltag19)
	ON_BN_CLICKED(IDC_SELTAG20, &CDiaTrendAttr::OnBnClickedSeltag20)
	ON_BN_CLICKED(IDOK, &CDiaTrendAttr::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaTrendAttr message handlers

void CDiaTrendAttr::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CBrush brush;
	if(lpDrawItemStruct->CtlID==IDC_BKCOLOR){
		brush.CreateSolidBrush(m_BkColor);
	}else if(lpDrawItemStruct->CtlID==IDC_GDCOLOR){
		brush.CreateSolidBrush(m_GdColor);
	}else{
		brush.CreateSolidBrush(RGB(255,255,255));
	}

	CPen pen;
	pen.CreateStockObject(NULL_PEN);
	HPEN hOldPen=(HPEN)SelectObject(lpDrawItemStruct->hDC,pen.GetSafeHandle());
	HBRUSH hOldBrush=(HBRUSH)SelectObject(lpDrawItemStruct->hDC,brush.GetSafeHandle());
	Rectangle(lpDrawItemStruct->hDC,0,0,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom);
	
	POINT op;
	CPen pen1;

	if(lpDrawItemStruct->CtlID==IDC_PEN1){
		pen1.CreatePenIndirect(&(m_pens[0]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN2){
		pen1.CreatePenIndirect(&(m_pens[1]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN3){
		pen1.CreatePenIndirect(&(m_pens[2]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN4){
		pen1.CreatePenIndirect(&(m_pens[3]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN5){
		pen1.CreatePenIndirect(&(m_pens[4]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN6){
		pen1.CreatePenIndirect(&(m_pens[5]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN7){
		pen1.CreatePenIndirect(&(m_pens[6]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN8){
		pen1.CreatePenIndirect(&(m_pens[7]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN9){
		pen1.CreatePenIndirect(&(m_pens[8]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}else if(lpDrawItemStruct->CtlID==IDC_PEN10){
		pen1.CreatePenIndirect(&(m_pens[9]));
		SelectObject(lpDrawItemStruct->hDC,pen1.GetSafeHandle());
		MoveToEx(lpDrawItemStruct->hDC,0,lpDrawItemStruct->rcItem.bottom/2,&op);
		LineTo(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom/2);
	}

	SelectObject(lpDrawItemStruct->hDC,hOldBrush);
	SelectObject(lpDrawItemStruct->hDC,hOldPen);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDiaTrendAttr::OnPen5() 
{
    CColorDialog dlg(m_pens[4].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[4].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen4() 
{
    CColorDialog dlg(m_pens[3].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[3].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen3() 
{
    CColorDialog dlg(m_pens[2].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[2].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen2() 
{
    CColorDialog dlg(m_pens[1].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[1].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen1() 
{
    CColorDialog dlg(m_pens[0].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[0].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnBkcolor() 
{
    CColorDialog dlg(m_BkColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_BkColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnGdcolor() 
{
    CColorDialog dlg(m_GdColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_GdColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen10() 
{
    CColorDialog dlg(m_pens[9].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[9].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen6() 
{
    CColorDialog dlg(m_pens[5].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[5].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen7() 
{
    CColorDialog dlg(m_pens[6].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[6].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen8() 
{
    CColorDialog dlg(m_pens[7].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[7].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnPen9() 
{
    CColorDialog dlg(m_pens[8].lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_pens[8].lopnColor=dlg.GetColor();
	}
	SetFocus();
}

void CDiaTrendAttr::OnBnClickedSeltag11()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag1 = this->seltagName();

	UpdateData(false);

}

CString CDiaTrendAttr::seltagName()
{
	CString tmpseltagname;

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);

		tmpseltagname = name;

		//UpdateData(false);
	}
	return tmpseltagname;
}

void CDiaTrendAttr::OnBnClickedSeltag12()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag2 = this->seltagName();

	UpdateData(false);
}


void CDiaTrendAttr::OnBnClickedSeltag13()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag3 = this->seltagName();

	UpdateData(false);
}


void CDiaTrendAttr::OnBnClickedSeltag14()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag4 = this->seltagName();

	UpdateData(false);
}


void CDiaTrendAttr::OnBnClickedSeltag15()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag5 = this->seltagName();

	UpdateData(false);
}

void CDiaTrendAttr::OnBnClickedSeltag16()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag6 = this->seltagName();

	UpdateData(false);
}


void CDiaTrendAttr::OnBnClickedSeltag17()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag7 = this->seltagName();

	UpdateData(false);

}

void CDiaTrendAttr::OnBnClickedSeltag18()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag8 = this->seltagName();

	UpdateData(false);
}

void CDiaTrendAttr::OnBnClickedSeltag19()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag9 = this->seltagName();

	UpdateData(false);
}

void CDiaTrendAttr::OnBnClickedSeltag20()
{
	// TODO: Add your control notification handler code here
	if( !Discover() )
	{
		return;
	}

	m_tag10 = this->seltagName();

	UpdateData(false);
}

void CDiaTrendAttr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
