// DiaDnyAttr.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaDnyAttr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaDnyAttr dialog


CDiaDnyAttr::CDiaDnyAttr(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaDnyAttr::IDD, pParent)
	, m_rank0(FALSE)
	, m_rank1(FALSE)
	, m_rank2(FALSE)
	, m_rank3(FALSE)
	, m_rank4(FALSE)
	, m_rank5(FALSE)
	, m_rank6(FALSE)
	, m_rank7(FALSE)
	, m_rank8(FALSE)
	, m_rank9(FALSE)
{
	//{{AFX_DATA_INIT(CDiaDnyAttr)
	m_Blink = FALSE;
	m_BrushColorA = FALSE;
	m_BrushColorAA = FALSE;
	m_BrushColorD = FALSE;
	m_BrushColorDA = FALSE;
	m_ButtonA = FALSE;
	m_ButtonD = FALSE;
	m_ButtonWnd = FALSE;
	m_Direction = FALSE;
	m_Disable = FALSE;
	m_FillH = FALSE;
	m_FillV = FALSE;
	m_InputA = FALSE;
	m_InputD = FALSE;
	m_PenColorA = FALSE;
	m_PenColorAA = FALSE;
	m_PenColorD = FALSE;
	m_PenColorDA = FALSE;
	m_PosH = FALSE;
	m_PosV = FALSE;
	m_SizeH = FALSE;
	m_SizeV = FALSE;
	m_SlideH = FALSE;
	m_SlideV = FALSE;
	m_TextColorA = FALSE;
	m_TextColorAA = FALSE;
	m_TextColorD = FALSE;
	m_TextColorDA = FALSE;
	m_ValueA = FALSE;
	m_ValueD = FALSE;
	m_Visible = FALSE;
	//}}AFX_DATA_INIT
}


void CDiaDnyAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaDnyAttr)
	DDX_Check(pDX, IDC_BLINK, m_Blink);
	DDX_Check(pDX, IDC_BRUSHCOLOR_A, m_BrushColorA);
	DDX_Check(pDX, IDC_BRUSHCOLOR_AA, m_BrushColorAA);
	DDX_Check(pDX, IDC_BRUSHCOLOR_D, m_BrushColorD);
	DDX_Check(pDX, IDC_BRUSHCOLOR_DA, m_BrushColorDA);
	DDX_Check(pDX, IDC_BUTTON_A, m_ButtonA);
	DDX_Check(pDX, IDC_BUTTON_D, m_ButtonD);
	DDX_Check(pDX, IDC_BUTTON_WND, m_ButtonWnd);
	DDX_Check(pDX, IDC_DIRECTION, m_Direction);
	DDX_Check(pDX, IDC_DISABLE, m_Disable);
	DDX_Check(pDX, IDC_FILL_H, m_FillH);
	DDX_Check(pDX, IDC_FILL_V, m_FillV);
	DDX_Check(pDX, IDC_INPUT_A, m_InputA);
	DDX_Check(pDX, IDC_INPUT_D, m_InputD);
	DDX_Check(pDX, IDC_PENCOLOR_A, m_PenColorA);
	DDX_Check(pDX, IDC_PENCOLOR_AA, m_PenColorAA);
	DDX_Check(pDX, IDC_PENCOLOR_D, m_PenColorD);
	DDX_Check(pDX, IDC_PENCOLOR_DA, m_PenColorDA);
	DDX_Check(pDX, IDC_POS_H, m_PosH);
	DDX_Check(pDX, IDC_POS_V, m_PosV);
	DDX_Check(pDX, IDC_SIZE_H, m_SizeH);
	DDX_Check(pDX, IDC_SIZE_V, m_SizeV);
	DDX_Check(pDX, IDC_SLIDE_H, m_SlideH);
	DDX_Check(pDX, IDC_SLIDE_V, m_SlideV);
	DDX_Check(pDX, IDC_TEXTCOLOR_A, m_TextColorA);
	DDX_Check(pDX, IDC_TEXTCOLOR_AA, m_TextColorAA);
	DDX_Check(pDX, IDC_TEXTCOLOR_D, m_TextColorD);
	DDX_Check(pDX, IDC_TEXTCOLOR_DA, m_TextColorDA);
	DDX_Check(pDX, IDC_VALUE_A, m_ValueA);
	DDX_Check(pDX, IDC_VALUE_D, m_ValueD);
	DDX_Check(pDX, IDC_VISIBLE, m_Visible);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_RANK0, m_rank0);
	DDX_Check(pDX, IDC_RANK1, m_rank1);
	DDX_Check(pDX, IDC_RANK2, m_rank2);
	DDX_Check(pDX, IDC_RANK3, m_rank3);
	DDX_Check(pDX, IDC_RANK4, m_rank4);
	DDX_Check(pDX, IDC_RANK5, m_rank5);
	DDX_Check(pDX, IDC_RANK6, m_rank6);
	DDX_Check(pDX, IDC_RANK7, m_rank7);
	DDX_Check(pDX, IDC_RANK8, m_rank8);
	DDX_Check(pDX, IDC_RANK9, m_rank9);
}


BEGIN_MESSAGE_MAP(CDiaDnyAttr, CDialog)
	//{{AFX_MSG_MAP(CDiaDnyAttr)
	ON_BN_CLICKED(IDC_BLINK, OnBlink)
	ON_BN_CLICKED(IDC_BRUSHCOLOR_A, OnBrushcolorA)
	ON_BN_CLICKED(IDC_BRUSHCOLOR_AA, OnBrushcolorAa)
	ON_BN_CLICKED(IDC_BRUSHCOLOR_D, OnBrushcolorD)
	ON_BN_CLICKED(IDC_BRUSHCOLOR_DA, OnBrushcolorDa)
	ON_BN_CLICKED(IDC_BUTTON_A, OnButtonA)
	ON_BN_CLICKED(IDC_BUTTON_D, OnButtonD)
	ON_BN_CLICKED(IDC_BUTTON_WND, OnButtonWnd)
	ON_BN_CLICKED(IDC_DIRECTION, OnDirection)
	ON_BN_CLICKED(IDC_DISABLE, OnDisable)
	ON_BN_CLICKED(IDC_FILL_H, OnFillH)
	ON_BN_CLICKED(IDC_FILL_V, OnFillV)
	ON_BN_CLICKED(IDC_INPUT_A, OnInputA)
	ON_BN_CLICKED(IDC_PENCOLOR_A, OnPencolorA)
	ON_BN_CLICKED(IDC_PENCOLOR_AA, OnPencolorAa)
	ON_BN_CLICKED(IDC_PENCOLOR_D, OnPencolorD)
	ON_BN_CLICKED(IDC_PENCOLOR_DA, OnPencolorDa)
	ON_BN_CLICKED(IDC_POS_H, OnPosH)
	ON_BN_CLICKED(IDC_POS_V, OnPosV)
	ON_BN_CLICKED(IDC_SIZE_H, OnSizeH)
	ON_BN_CLICKED(IDC_SIZE_V, OnSizeV)
	ON_BN_CLICKED(IDC_SLIDE_H, OnSlideH)
	ON_BN_CLICKED(IDC_SLIDE_V, OnSlideV)
	ON_BN_CLICKED(IDC_TEXTCOLOR_A, OnTextcolorA)
	ON_BN_CLICKED(IDC_TEXTCOLOR_AA, OnTextcolorAa)
	ON_BN_CLICKED(IDC_TEXTCOLOR_D, OnTextcolorD)
	ON_BN_CLICKED(IDC_TEXTCOLOR_DA, OnTextcolorDa)
	ON_BN_CLICKED(IDC_VALUE_A, OnValueA)
	ON_BN_CLICKED(IDC_VALUE_D, OnValueD)
	ON_BN_CLICKED(IDC_VISIBLE, OnVisible)
	ON_BN_CLICKED(IDC_INPUT_D, OnInputD)
	ON_BN_CLICKED(IDC_CINPUT_D, OnCinputD)
	ON_BN_CLICKED(IDC_CINPUT_A, OnCinputA)
	ON_BN_CLICKED(IDC_CSLIDE_H, OnCslideH)
	ON_BN_CLICKED(IDC_CSLIDE_V, OnCslideV)
	ON_BN_CLICKED(IDC_CBUTTON_D, OnCbuttonD)
	ON_BN_CLICKED(IDC_CBUTTON_A, OnCbuttonA)
	ON_BN_CLICKED(IDC_CBUTTON_WND, OnCbuttonWnd)
	ON_BN_CLICKED(IDC_CVALUE_D, OnCvalueD)
	ON_BN_CLICKED(IDC_CVALUE_A, OnCvalueA)
	ON_BN_CLICKED(IDC_CPENCOLOR_D, OnCpencolorD)
	ON_BN_CLICKED(IDC_CPENCOLOR_A, OnCpencolorA)
	ON_BN_CLICKED(IDC_CPENCOLOR_DA, OnCpencolorDa)
	ON_BN_CLICKED(IDC_CPENCOLOR_AA, OnCpencolorAa)
	ON_BN_CLICKED(IDC_CBRUSHCOLOR_D, OnCbrushcolorD)
	ON_BN_CLICKED(IDC_CBRUSHCOLOR_A, OnCbrushcolorA)
	ON_BN_CLICKED(IDC_CBRUSHCOLOR_DA, OnCbrushcolorDa)
	ON_BN_CLICKED(IDC_CBRUSHCOLOR_AA, OnCbrushcolorAa)
	ON_BN_CLICKED(IDC_CTEXTCOLOR_D, OnCtextcolorD)
	ON_BN_CLICKED(IDC_CTEXTCOLOR_A, OnCtextcolorA)
	ON_BN_CLICKED(IDC_CTEXTCOLOR_DA, OnCtextcolorDa)
	ON_BN_CLICKED(IDC_CTEXTCOLOR_AA, OnCtextcolorAa)
	ON_BN_CLICKED(IDC_CSIZE_H, OnCsizeH)
	ON_BN_CLICKED(IDC_CPOS_H, OnCposH)
	ON_BN_CLICKED(IDC_CFILL_H, OnCfillH)
	ON_BN_CLICKED(IDC_CSIZE_V, OnCsizeV)
	ON_BN_CLICKED(IDC_CPOS_V, OnCposV)
	ON_BN_CLICKED(IDC_CFILL_V, OnCfillV)
	ON_BN_CLICKED(IDC_CVISIBLE, OnCvisible)
	ON_BN_CLICKED(IDC_CENABLE, OnCenable)
	ON_BN_CLICKED(IDC_CBLINK, OnCblink)
	ON_BN_CLICKED(IDC_CDIRECTION, OnCdirection)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RANK0, &CDiaDnyAttr::OnBnClickedRank0)
	ON_BN_CLICKED(IDC_RANK1, &CDiaDnyAttr::OnBnClickedRank1)
	ON_BN_CLICKED(IDC_RANK2, &CDiaDnyAttr::OnBnClickedRank2)
	ON_BN_CLICKED(IDC_RANK3, &CDiaDnyAttr::OnBnClickedRank3)
	ON_BN_CLICKED(IDC_RANK4, &CDiaDnyAttr::OnBnClickedRank4)
	ON_BN_CLICKED(IDC_RANK5, &CDiaDnyAttr::OnBnClickedRank5)
	ON_BN_CLICKED(IDC_RANK6, &CDiaDnyAttr::OnBnClickedRank6)
	ON_BN_CLICKED(IDC_RANK7, &CDiaDnyAttr::OnBnClickedRank7)
	ON_BN_CLICKED(IDC_RANK8, &CDiaDnyAttr::OnBnClickedRank8)
	ON_BN_CLICKED(IDC_RANK9, &CDiaDnyAttr::OnBnClickedRank9)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaDnyAttr message handlers

void CDiaDnyAttr::OnBlink() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnBrushcolorA() 
{
	UpdateData(true);

	if(m_BrushColorA){
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}	
}

void CDiaDnyAttr::OnBrushcolorAa() 
{
	UpdateData(true);

	if(m_BrushColorAA){
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnBrushcolorD() 
{
	UpdateData(true);

	if(m_BrushColorD){
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnBrushcolorDa() 
{
	UpdateData(true);

	if(m_BrushColorDA){
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnButtonA() 
{
	UpdateData(true);

	if(m_ButtonA){
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnButtonD() 
{
	UpdateData(true);

	if(m_ButtonD){
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnButtonWnd() 
{
	UpdateData(true);

	if(m_ButtonWnd){
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnDirection() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnDisable() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnFillH() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnFillV() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnInputA() 
{
	UpdateData(true);

	if(m_InputA){
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnInputD() 
{
	UpdateData(true);

	if(m_InputD){
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnPencolorA() 
{
	UpdateData(true);

	if(m_PenColorA){
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnPencolorAa() 
{
	UpdateData(true);

	if(m_PenColorAA){
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}	
}

void CDiaDnyAttr::OnPencolorD() 
{
	UpdateData(true);

	if(m_PenColorD){
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnPencolorDa() 
{
	UpdateData(true);

	if(m_PenColorDA){
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnPosH() 
{
}

void CDiaDnyAttr::OnPosV() 
{
}

void CDiaDnyAttr::OnSizeH() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnSizeV() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnSlideH() 
{
	// TODO: Add your control notification handler code here

}

void CDiaDnyAttr::OnSlideV() 
{
	// TODO: Add your control notification handler code here

}

void CDiaDnyAttr::OnTextcolorA() 
{
	UpdateData(true);

	if(m_TextColorD){
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnTextcolorAa() 
{
	UpdateData(true);

	if(m_TextColorAA){
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnTextcolorD() 
{
	UpdateData(true);

	if(m_TextColorD){
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnTextcolorDa() 
{
	UpdateData(true);

	if(m_TextColorDA){
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnValueA() 
{
	UpdateData(true);

	if(m_ValueA){
		SendDlgItemMessage(IDC_VALUE_D,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnValueD() 
{
	UpdateData(true);

	if(m_ValueD){
		SendDlgItemMessage(IDC_VALUE_A,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnVisible() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiaDnyAttr::OnCinputD() 
{
	CDiaInputD dlg;
	dlg.m_TagName=m_DnyAttr.m_InputD.m_TagName;
	dlg.m_WndPrm=m_DnyAttr.m_InputD.m_WndPrm;
	dlg.m_SetPrm=m_DnyAttr.m_InputD.m_SetPrm;
	dlg.m_ResetPrm=m_DnyAttr.m_InputD.m_ResetPrm;
	dlg.m_OnMsg=m_DnyAttr.m_InputD.m_OnMsg;
	dlg.m_OffMsg=m_DnyAttr.m_InputD.m_OffMsg;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_InputD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_InputD.m_WndPrm=dlg.m_WndPrm;
		m_DnyAttr.m_InputD.m_SetPrm=dlg.m_SetPrm;
		m_DnyAttr.m_InputD.m_ResetPrm=dlg.m_ResetPrm;
		m_DnyAttr.m_InputD.m_OnMsg=dlg.m_OnMsg;
		m_DnyAttr.m_InputD.m_OffMsg=dlg.m_OffMsg;
		m_DnyAttr.m_InputD.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnCinputA() 
{
	CDiaInputA dlg;
	dlg.m_TagName=m_DnyAttr.m_InputA.m_TagName;
	dlg.m_WndPrm=m_DnyAttr.m_InputA.m_WndPrm;
	dlg.m_MinValue=m_DnyAttr.m_InputA.m_MinVal;
	dlg.m_MaxValue=m_DnyAttr.m_InputA.m_MaxVal;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_InputA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_InputA.m_WndPrm=dlg.m_WndPrm;
		m_DnyAttr.m_InputA.m_MinVal=dlg.m_MinValue;
		m_DnyAttr.m_InputA.m_MaxVal=dlg.m_MaxValue;
		m_DnyAttr.m_InputA.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnCslideH() 
{
	CDiaSlideH dlg;
	dlg.m_TagName=m_DnyAttr.m_SlideH.m_TagName;
	dlg.m_LeftSlideO=m_DnyAttr.m_SlideH.m_LTO;
	dlg.m_LeftSlideT=m_DnyAttr.m_SlideH.m_LTT;
	dlg.m_RightSlideO=m_DnyAttr.m_SlideH.m_RBO;
	dlg.m_RightSlideT=m_DnyAttr.m_SlideH.m_RBT;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_SlideH.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_SlideH.m_LTO=dlg.m_LeftSlideO;
		m_DnyAttr.m_SlideH.m_LTT=dlg.m_LeftSlideT;
		m_DnyAttr.m_SlideH.m_RBO=dlg.m_RightSlideO;
		m_DnyAttr.m_SlideH.m_RBT=dlg.m_RightSlideT;
		m_DnyAttr.m_SlideH.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_POS_V,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnCslideV() 
{
	CDiaSlideV dlg;
	dlg.m_TagName=m_DnyAttr.m_SlideV.m_TagName;
	dlg.m_TopSlideO=m_DnyAttr.m_SlideV.m_LTO;
	dlg.m_TopSlideT=m_DnyAttr.m_SlideV.m_LTT;
	dlg.m_BottomSlideO=m_DnyAttr.m_SlideV.m_RBO;
	dlg.m_BottomSlideT=m_DnyAttr.m_SlideV.m_RBT;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_SlideV.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_SlideV.m_LTO=dlg.m_TopSlideO;
		m_DnyAttr.m_SlideV.m_LTT=dlg.m_TopSlideT;
		m_DnyAttr.m_SlideV.m_RBO=dlg.m_BottomSlideO;
		m_DnyAttr.m_SlideV.m_RBT=dlg.m_BottomSlideT;
		m_DnyAttr.m_SlideV.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_POS_H,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

void CDiaDnyAttr::OnCbuttonD() 
{
	CDiaButtonD dlg;
	dlg.m_TagName = m_DnyAttr.m_ButtonD.m_TagName;
	dlg.m_Action = m_DnyAttr.m_ButtonD.m_Action;
	dlg.m_iPulseWidth = m_DnyAttr.m_ButtonD.m_pulseWidth;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ButtonD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ButtonD.m_Action=dlg.m_Action;
		m_DnyAttr.m_ButtonD.m_pulseWidth = dlg.m_iPulseWidth;
		m_DnyAttr.m_ButtonD.m_Used = true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}	
}

void CDiaDnyAttr::OnCbuttonA() 
{
	CDiaButtonA dlg;
	dlg.m_TagName=m_DnyAttr.m_ButtonA.m_TagName;
	dlg.m_Fuction=m_DnyAttr.m_ButtonA.m_Fuction;
	dlg.m_DownLimit=m_DnyAttr.m_ButtonA.m_DownLimit;
	dlg.m_UpLimit=m_DnyAttr.m_ButtonA.m_UpLimit;
	dlg.m_Number=m_DnyAttr.m_ButtonA.m_Delta;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ButtonA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ButtonA.m_Fuction=dlg.m_Fuction;
		m_DnyAttr.m_ButtonA.m_DownLimit=dlg.m_DownLimit;
		m_DnyAttr.m_ButtonA.m_UpLimit=dlg.m_UpLimit;
		m_DnyAttr.m_ButtonA.m_Delta=dlg.m_Number;		
		m_DnyAttr.m_ButtonA.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_UNCHECKED,0);
	}	
}

void CDiaDnyAttr::OnCbuttonWnd() 
{
	CDiaButtonWnd dlg;
	dlg.m_WndFileName=m_DnyAttr.m_ButtonWnd.m_TagName;
	dlg.m_ShowMode=m_DnyAttr.m_ButtonWnd.m_Mode;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ButtonWnd.m_TagName=dlg.m_WndFileName;
		m_DnyAttr.m_ButtonWnd.m_Mode=dlg.m_ShowMode;
		m_DnyAttr.m_ButtonWnd.m_Used=true;
		SendDlgItemMessage(IDC_INPUT_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_INPUT_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_H,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_SLIDE_V,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BUTTON_WND,BM_SETCHECK,BST_CHECKED,0);
	}	
}

void CDiaDnyAttr::OnCvalueD() 
{
	CDiaValueD dlg;
	dlg.m_TagName=m_DnyAttr.m_ValueD.m_TagName;
	dlg.m_OffMsg=m_DnyAttr.m_ValueD.m_OffMsg;
	dlg.m_OnMsg=m_DnyAttr.m_ValueD.m_OnMsg;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ValueD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ValueD.m_OffMsg=dlg.m_OffMsg;
		m_DnyAttr.m_ValueD.m_OnMsg=dlg.m_OnMsg;
		m_DnyAttr.m_ValueD.m_Used=true;
		SendDlgItemMessage(IDC_VALUE_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_VALUE_A,BM_SETCHECK,BST_UNCHECKED,0);
	}	
}

void CDiaDnyAttr::OnCvalueA() 
{
	CDiaValueA dlg;
	dlg.m_TagName=m_DnyAttr.m_ValueA.m_TagName;
	dlg.m_Format=m_DnyAttr.m_ValueA.m_Format;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ValueA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ValueA.m_Format=dlg.m_Format;
		m_DnyAttr.m_ValueA.m_Used=true;
		SendDlgItemMessage(IDC_VALUE_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_VALUE_D,BM_SETCHECK,BST_UNCHECKED,0);
	}		
}

void CDiaDnyAttr::OnCpencolorD() 
{
	CDiaColorD dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorPenD.m_TagName;
	dlg.m_ColorOn=m_DnyAttr.m_ColorPenD.m_ColorOn;
	dlg.m_ColorOff=m_DnyAttr.m_ColorPenD.m_ColorOff;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorPenD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorPenD.m_ColorOn=dlg.m_ColorOn;
		m_DnyAttr.m_ColorPenD.m_ColorOff=dlg.m_ColorOff;
		m_DnyAttr.m_ColorPenD.m_Used=true;
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}		
}

void CDiaDnyAttr::OnCpencolorA() 
{
	CDiaColorA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorPenA.m_TagName;
	dlg.m_Color0=m_DnyAttr.m_ColorPenA.m_Color0;
	dlg.m_Color1=m_DnyAttr.m_ColorPenA.m_Color1;
	dlg.m_Color2=m_DnyAttr.m_ColorPenA.m_Color2;
	dlg.m_Color3=m_DnyAttr.m_ColorPenA.m_Color3;
	dlg.m_Color4=m_DnyAttr.m_ColorPenA.m_Color4;
	dlg.m_Split1=m_DnyAttr.m_ColorPenA.m_Split1;
	dlg.m_Split2=m_DnyAttr.m_ColorPenA.m_Split2;
	dlg.m_Split3=m_DnyAttr.m_ColorPenA.m_Split3;
	dlg.m_Split4=m_DnyAttr.m_ColorPenA.m_Split4;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorPenA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorPenA.m_Color0=dlg.m_Color0;
		m_DnyAttr.m_ColorPenA.m_Color1=dlg.m_Color1;
		m_DnyAttr.m_ColorPenA.m_Color2=dlg.m_Color2;
		m_DnyAttr.m_ColorPenA.m_Color3=dlg.m_Color3;
		m_DnyAttr.m_ColorPenA.m_Color4=dlg.m_Color4;
		m_DnyAttr.m_ColorPenA.m_Split1=dlg.m_Split1; 
		m_DnyAttr.m_ColorPenA.m_Split2=dlg.m_Split2;
		m_DnyAttr.m_ColorPenA.m_Split3=dlg.m_Split3;
		m_DnyAttr.m_ColorPenA.m_Split4=dlg.m_Split4;
		m_DnyAttr.m_ColorPenA.m_Used=true;
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCpencolorDa() 
{
	CDiaColorDA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorPenDA.m_TagName;
	dlg.m_ColorAlarm=m_DnyAttr.m_ColorPenDA.m_ColorAlarm;
	dlg.m_ColorNomal=m_DnyAttr.m_ColorPenDA.m_ColorNomal;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorPenDA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorPenDA.m_ColorAlarm=dlg.m_ColorAlarm;
		m_DnyAttr.m_ColorPenDA.m_ColorNomal=dlg.m_ColorNomal;
		m_DnyAttr.m_ColorPenDA.m_Used=true;
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCpencolorAa() 
{
	CDiaColorAA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorPenAA.m_TagName;
	dlg.m_ColorH1=m_DnyAttr.m_ColorPenAA.m_ColorH1;
	dlg.m_ColorH2=m_DnyAttr.m_ColorPenAA.m_ColorH2;
	dlg.m_ColorL1=m_DnyAttr.m_ColorPenAA.m_ColorL1;
	dlg.m_ColorL2=m_DnyAttr.m_ColorPenAA.m_ColorL2;
	dlg.m_ColorN=m_DnyAttr.m_ColorPenAA.m_ColorN;
	dlg.m_ColorR=m_DnyAttr.m_ColorPenAA.m_ColorR;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorPenAA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorPenAA.m_ColorH1=dlg.m_ColorH1;
		m_DnyAttr.m_ColorPenAA.m_ColorH2=dlg.m_ColorH2;
		m_DnyAttr.m_ColorPenAA.m_ColorL1=dlg.m_ColorL1;
		m_DnyAttr.m_ColorPenAA.m_ColorL2=dlg.m_ColorL2;
		m_DnyAttr.m_ColorPenAA.m_ColorN=dlg.m_ColorN;
		m_DnyAttr.m_ColorPenAA.m_ColorR=dlg.m_ColorR;
		m_DnyAttr.m_ColorPenAA.m_Used=true;
		SendDlgItemMessage(IDC_PENCOLOR_AA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_PENCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}				
}

void CDiaDnyAttr::OnCbrushcolorD() 
{
	CDiaColorD dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorBrushD.m_TagName;
	dlg.m_ColorOn=m_DnyAttr.m_ColorBrushD.m_ColorOn;
	dlg.m_ColorOff=m_DnyAttr.m_ColorBrushD.m_ColorOff;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorBrushD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorBrushD.m_ColorOn=dlg.m_ColorOn;
		m_DnyAttr.m_ColorBrushD.m_ColorOff=dlg.m_ColorOff;
		m_DnyAttr.m_ColorBrushD.m_Used=true;
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}		
}

void CDiaDnyAttr::OnCbrushcolorA() 
{
	CDiaColorA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorBrushA.m_TagName;
	dlg.m_Color0=m_DnyAttr.m_ColorBrushA.m_Color0;
	dlg.m_Color1=m_DnyAttr.m_ColorBrushA.m_Color1;
	dlg.m_Color2=m_DnyAttr.m_ColorBrushA.m_Color2;
	dlg.m_Color3=m_DnyAttr.m_ColorBrushA.m_Color3;
	dlg.m_Color4=m_DnyAttr.m_ColorBrushA.m_Color4;
	dlg.m_Split1=m_DnyAttr.m_ColorBrushA.m_Split1;
	dlg.m_Split2=m_DnyAttr.m_ColorBrushA.m_Split2;
	dlg.m_Split3=m_DnyAttr.m_ColorBrushA.m_Split3;
	dlg.m_Split4=m_DnyAttr.m_ColorBrushA.m_Split4;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorBrushA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorBrushA.m_Color0=dlg.m_Color0;
		m_DnyAttr.m_ColorBrushA.m_Color1=dlg.m_Color1;
		m_DnyAttr.m_ColorBrushA.m_Color2=dlg.m_Color2;
		m_DnyAttr.m_ColorBrushA.m_Color3=dlg.m_Color3;
		m_DnyAttr.m_ColorBrushA.m_Color4=dlg.m_Color4;
		m_DnyAttr.m_ColorBrushA.m_Split1=dlg.m_Split1; 
		m_DnyAttr.m_ColorBrushA.m_Split2=dlg.m_Split2;
		m_DnyAttr.m_ColorBrushA.m_Split3=dlg.m_Split3;
		m_DnyAttr.m_ColorBrushA.m_Split4=dlg.m_Split4;
		m_DnyAttr.m_ColorBrushA.m_Used=true;
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCbrushcolorDa() 
{
	CDiaColorDA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorBrushDA.m_TagName;
	dlg.m_ColorAlarm=m_DnyAttr.m_ColorBrushDA.m_ColorAlarm;
	dlg.m_ColorNomal=m_DnyAttr.m_ColorBrushDA.m_ColorNomal;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorBrushDA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorBrushDA.m_ColorAlarm=dlg.m_ColorAlarm;
		m_DnyAttr.m_ColorBrushDA.m_ColorNomal=dlg.m_ColorNomal;
		m_DnyAttr.m_ColorBrushDA.m_Used=true;
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCbrushcolorAa() 
{
	CDiaColorAA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorBrushAA.m_TagName;
	dlg.m_ColorH1=m_DnyAttr.m_ColorBrushAA.m_ColorH1;
	dlg.m_ColorH2=m_DnyAttr.m_ColorBrushAA.m_ColorH2;
	dlg.m_ColorL1=m_DnyAttr.m_ColorBrushAA.m_ColorL1;
	dlg.m_ColorL2=m_DnyAttr.m_ColorBrushAA.m_ColorL2;
	dlg.m_ColorN=m_DnyAttr.m_ColorBrushAA.m_ColorN;
	dlg.m_ColorR=m_DnyAttr.m_ColorBrushAA.m_ColorR;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorBrushAA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorBrushAA.m_ColorH1=dlg.m_ColorH1;
		m_DnyAttr.m_ColorBrushAA.m_ColorH2=dlg.m_ColorH2;
		m_DnyAttr.m_ColorBrushAA.m_ColorL1=dlg.m_ColorL1;
		m_DnyAttr.m_ColorBrushAA.m_ColorL2=dlg.m_ColorL2;
		m_DnyAttr.m_ColorBrushAA.m_ColorN=dlg.m_ColorN;
		m_DnyAttr.m_ColorBrushAA.m_ColorR=dlg.m_ColorR;
		m_DnyAttr.m_ColorBrushAA.m_Used=true;
		SendDlgItemMessage(IDC_BRUSHCOLOR_AA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_BRUSHCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}				
}

void CDiaDnyAttr::OnCtextcolorD() 
{
	CDiaColorD dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorTextD.m_TagName;
	dlg.m_ColorOn=m_DnyAttr.m_ColorTextD.m_ColorOn;
	dlg.m_ColorOff=m_DnyAttr.m_ColorTextD.m_ColorOff;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorTextD.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorTextD.m_ColorOn=dlg.m_ColorOn;
		m_DnyAttr.m_ColorTextD.m_ColorOff=dlg.m_ColorOff;
		m_DnyAttr.m_ColorTextD.m_Used=true;
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}		
}

void CDiaDnyAttr::OnCtextcolorA() 
{
	CDiaColorA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorTextA.m_TagName;
	dlg.m_Color0=m_DnyAttr.m_ColorTextA.m_Color0;
	dlg.m_Color1=m_DnyAttr.m_ColorTextA.m_Color1;
	dlg.m_Color2=m_DnyAttr.m_ColorTextA.m_Color2;
	dlg.m_Color3=m_DnyAttr.m_ColorTextA.m_Color3;
	dlg.m_Color4=m_DnyAttr.m_ColorTextA.m_Color4;
	dlg.m_Split1=m_DnyAttr.m_ColorTextA.m_Split1;
	dlg.m_Split2=m_DnyAttr.m_ColorTextA.m_Split2;
	dlg.m_Split3=m_DnyAttr.m_ColorTextA.m_Split3;
	dlg.m_Split4=m_DnyAttr.m_ColorTextA.m_Split4;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorTextA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorTextA.m_Color0=dlg.m_Color0;
		m_DnyAttr.m_ColorTextA.m_Color1=dlg.m_Color1;
		m_DnyAttr.m_ColorTextA.m_Color2=dlg.m_Color2;
		m_DnyAttr.m_ColorTextA.m_Color3=dlg.m_Color3;
		m_DnyAttr.m_ColorTextA.m_Color4=dlg.m_Color4;
		m_DnyAttr.m_ColorTextA.m_Split1=dlg.m_Split1; 
		m_DnyAttr.m_ColorTextA.m_Split2=dlg.m_Split2;
		m_DnyAttr.m_ColorTextA.m_Split3=dlg.m_Split3;
		m_DnyAttr.m_ColorTextA.m_Split4=dlg.m_Split4;
		m_DnyAttr.m_ColorTextA.m_Used=true;
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCtextcolorDa() 
{
	CDiaColorDA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorTextDA.m_TagName;
	dlg.m_ColorAlarm=m_DnyAttr.m_ColorTextDA.m_ColorAlarm;
	dlg.m_ColorNomal=m_DnyAttr.m_ColorTextDA.m_ColorNomal;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorTextDA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorTextDA.m_ColorAlarm=dlg.m_ColorAlarm;
		m_DnyAttr.m_ColorTextDA.m_ColorNomal=dlg.m_ColorNomal;
		m_DnyAttr.m_ColorTextDA.m_Used=true;
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_UNCHECKED,0);
	}			
}

void CDiaDnyAttr::OnCtextcolorAa() 
{
	CDiaColorAA dlg;
	dlg.m_TagName=m_DnyAttr.m_ColorTextAA.m_TagName;
	dlg.m_ColorH1=m_DnyAttr.m_ColorTextAA.m_ColorH1;
	dlg.m_ColorH2=m_DnyAttr.m_ColorTextAA.m_ColorH2;
	dlg.m_ColorL1=m_DnyAttr.m_ColorTextAA.m_ColorL1;
	dlg.m_ColorL2=m_DnyAttr.m_ColorTextAA.m_ColorL2;
	dlg.m_ColorN=m_DnyAttr.m_ColorTextAA.m_ColorN;
	dlg.m_ColorR=m_DnyAttr.m_ColorTextAA.m_ColorR;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_ColorTextAA.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_ColorTextAA.m_ColorH1=dlg.m_ColorH1;
		m_DnyAttr.m_ColorTextAA.m_ColorH2=dlg.m_ColorH2;
		m_DnyAttr.m_ColorTextAA.m_ColorL1=dlg.m_ColorL1;
		m_DnyAttr.m_ColorTextAA.m_ColorL2=dlg.m_ColorL2;
		m_DnyAttr.m_ColorTextAA.m_ColorN=dlg.m_ColorN;
		m_DnyAttr.m_ColorTextAA.m_ColorR=dlg.m_ColorR;
		m_DnyAttr.m_ColorTextAA.m_Used=true;
		SendDlgItemMessage(IDC_TEXTCOLOR_AA,BM_SETCHECK,BST_CHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_DA,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_D,BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgItemMessage(IDC_TEXTCOLOR_A,BM_SETCHECK,BST_UNCHECKED,0);
	}				
}

void CDiaDnyAttr::OnCsizeH() 
{
	CDiaSizeH dlg;
	dlg.m_TagName=m_DnyAttr.m_SizeH.m_TagName;
	dlg.m_MinWidthO=m_DnyAttr.m_SizeH.m_LTO;
	dlg.m_MinWidthT=m_DnyAttr.m_SizeH.m_LTT;
	dlg.m_MaxWidthO=m_DnyAttr.m_SizeH.m_RBO;
	dlg.m_MaxWidthT=m_DnyAttr.m_SizeH.m_RBT;
	dlg.m_HorRef=m_DnyAttr.m_SizeH.m_Ref;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_SizeH.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_SizeH.m_LTO=dlg.m_MinWidthO;
		m_DnyAttr.m_SizeH.m_LTT=dlg.m_MinWidthT;
		m_DnyAttr.m_SizeH.m_RBO=dlg.m_MaxWidthO;
		m_DnyAttr.m_SizeH.m_RBT=dlg.m_MaxWidthT;
		m_DnyAttr.m_SizeH.m_Ref=dlg.m_HorRef;
		m_DnyAttr.m_SizeH.m_Used=true;
		SendDlgItemMessage(IDC_SIZE_H,BM_SETCHECK,BST_CHECKED,0);
	}
}

void CDiaDnyAttr::OnCposH() 
{
	CDiaPosH dlg;
	dlg.m_TagName=m_DnyAttr.m_PosH.m_TagName;
	dlg.m_LeftPosO=m_DnyAttr.m_PosH.m_LTO;
	dlg.m_LeftPosT=m_DnyAttr.m_PosH.m_LTT;
	dlg.m_RightPosO=m_DnyAttr.m_PosH.m_RBO;
	dlg.m_RightPosT=m_DnyAttr.m_PosH.m_RBT;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_PosH.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_PosH.m_LTO=dlg.m_LeftPosO;
		m_DnyAttr.m_PosH.m_LTT=dlg.m_LeftPosT;
		m_DnyAttr.m_PosH.m_RBO=dlg.m_RightPosO;
		m_DnyAttr.m_PosH.m_RBT=dlg.m_RightPosT;
		m_DnyAttr.m_PosH.m_Used=true;
		SendDlgItemMessage(IDC_POS_H,BM_SETCHECK,BST_CHECKED,0);
	}
}

void CDiaDnyAttr::OnCfillH() 
{
	CDiaFillH dlg;
	dlg.m_TagName=m_DnyAttr.m_FillH.m_TagName;
	dlg.m_MinHFillO=m_DnyAttr.m_FillH.m_LTO;
	dlg.m_MinHFillT=m_DnyAttr.m_FillH.m_LTT;
	dlg.m_MaxHFillO=m_DnyAttr.m_FillH.m_RBO;
	dlg.m_MaxHFillT=m_DnyAttr.m_FillH.m_RBT;
	dlg.m_HorRef=m_DnyAttr.m_FillH.m_Ref;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_FillH.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_FillH.m_LTO=dlg.m_MinHFillO;
		m_DnyAttr.m_FillH.m_LTT=dlg.m_MinHFillT;
		m_DnyAttr.m_FillH.m_RBO=dlg.m_MaxHFillO;
		m_DnyAttr.m_FillH.m_RBT=dlg.m_MaxHFillT;
		m_DnyAttr.m_FillH.m_Ref=dlg.m_HorRef;
		m_DnyAttr.m_FillH.m_Used=true;
		SendDlgItemMessage(IDC_FILL_H,BM_SETCHECK,BST_CHECKED,0);
	}
}

void CDiaDnyAttr::OnCsizeV() 
{
	CDiaSizeV dlg;
	dlg.m_TagName=m_DnyAttr.m_SizeV.m_TagName;
	dlg.m_MinHeightO=m_DnyAttr.m_SizeV.m_LTO;
	dlg.m_MinHeightT=m_DnyAttr.m_SizeV.m_LTT;
	dlg.m_MaxHeightO=m_DnyAttr.m_SizeV.m_RBO;
	dlg.m_MaxHeightT=m_DnyAttr.m_SizeV.m_RBT;
	dlg.m_VerRef=m_DnyAttr.m_SizeV.m_Ref;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_SizeV.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_SizeV.m_LTO=dlg.m_MinHeightO;
		m_DnyAttr.m_SizeV.m_LTT=dlg.m_MinHeightT;
		m_DnyAttr.m_SizeV.m_RBO=dlg.m_MaxHeightO;
		m_DnyAttr.m_SizeV.m_RBT=dlg.m_MaxHeightT;
		m_DnyAttr.m_SizeV.m_Ref=dlg.m_VerRef;
		m_DnyAttr.m_SizeV.m_Used=true;
		SendDlgItemMessage(IDC_SIZE_V,BM_SETCHECK,BST_CHECKED,0);
	}

}

void CDiaDnyAttr::OnCposV() 
{
	CDiaPosV dlg;
	dlg.m_TagName=m_DnyAttr.m_PosV.m_TagName;
	dlg.m_TopPosO=m_DnyAttr.m_PosV.m_LTO;
	dlg.m_TopPosT=m_DnyAttr.m_PosV.m_LTT;
	dlg.m_BottomPosO=m_DnyAttr.m_PosV.m_RBO;
	dlg.m_BottomPosT=m_DnyAttr.m_PosV.m_RBT;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_PosV.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_PosV.m_LTO=dlg.m_TopPosO;
		m_DnyAttr.m_PosV.m_LTT=dlg.m_TopPosT;
		m_DnyAttr.m_PosV.m_RBO=dlg.m_BottomPosO;
		m_DnyAttr.m_PosV.m_RBT=dlg.m_BottomPosT;
		m_DnyAttr.m_PosV.m_Used=true;
		SendDlgItemMessage(IDC_POS_V,BM_SETCHECK,BST_CHECKED,0);
	}
}

void CDiaDnyAttr::OnCfillV() 
{
	CDiaFillV dlg;
	dlg.m_TagName=m_DnyAttr.m_FillV.m_TagName;
	dlg.m_MinVFillO=m_DnyAttr.m_FillV.m_LTO;
	dlg.m_MinVFillT=m_DnyAttr.m_FillV.m_LTT;
	dlg.m_MaxVFillO=m_DnyAttr.m_FillV.m_RBO;
	dlg.m_MaxVFillT=m_DnyAttr.m_FillV.m_RBT;
	dlg.m_VerRef=m_DnyAttr.m_FillV.m_Ref;
	if(dlg.DoModal()==IDOK){		
		m_DnyAttr.m_FillV.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_FillV.m_LTO=dlg.m_MinVFillO;
		m_DnyAttr.m_FillV.m_LTT=dlg.m_MinVFillT;
		m_DnyAttr.m_FillV.m_RBO=dlg.m_MaxVFillO;
		m_DnyAttr.m_FillV.m_RBT=dlg.m_MaxVFillT;
		m_DnyAttr.m_FillV.m_Ref=dlg.m_VerRef;
		m_DnyAttr.m_FillV.m_Used=true;
		SendDlgItemMessage(IDC_FILL_V,BM_SETCHECK,BST_CHECKED,0);
	}
}

void CDiaDnyAttr::OnCvisible() 
{
	CDiaVisibility dlg;
	dlg.m_TagName=m_DnyAttr.m_Visible.m_TagName;
	if(m_DnyAttr.m_Visible.m_Visble) dlg.m_Visible=0;
	else dlg.m_Visible=1;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_Visible.m_TagName=dlg.m_TagName;
		if(dlg.m_Visible==0) m_DnyAttr.m_Visible.m_Visble=true;
		else m_DnyAttr.m_Visible.m_Visble=false;
		m_DnyAttr.m_Visible.m_Used=true;
		SendDlgItemMessage(IDC_VISIBLE,BM_SETCHECK,BST_CHECKED,0);
	}	
}

void CDiaDnyAttr::OnCenable() 
{
	CDiaEnable dlg;
	dlg.m_TagName=m_DnyAttr.m_Enable.m_TagName;
	if(m_DnyAttr.m_Enable.m_Enable) dlg.m_Enable=0;
	else dlg.m_Enable=1;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_Enable.m_TagName=dlg.m_TagName;
		if(dlg.m_Enable==0) m_DnyAttr.m_Enable.m_Enable =true;
		else m_DnyAttr.m_Enable.m_Enable =false;
		m_DnyAttr.m_Enable.m_Used=true;
		SendDlgItemMessage(IDC_DISABLE,BM_SETCHECK,BST_CHECKED,0);
	}	
}

void CDiaDnyAttr::OnCblink() 
{
	CDiaBlink dlg;
	dlg.m_TagName=m_DnyAttr.m_Blink.m_TagName;
	if(m_DnyAttr.m_Blink.m_VisibleAtOn)	dlg.m_Visible=0;
	else dlg.m_Visible=1;
	dlg.m_Speed=m_DnyAttr.m_Blink.m_BlinkSpeed;
	dlg.m_ColorBrush=m_DnyAttr.m_Blink.m_ColorBrush;
	dlg.m_ColorPen=m_DnyAttr.m_Blink.m_ColorPen;
	dlg.m_ColorText=m_DnyAttr.m_Blink.m_ColorText;	
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_Blink.m_TagName=dlg.m_TagName;
		if(dlg.m_Visible==0) m_DnyAttr.m_Blink.m_VisibleAtOn=true;
		else m_DnyAttr.m_Blink.m_VisibleAtOn=false;
		m_DnyAttr.m_Blink.m_BlinkSpeed=dlg.m_Speed;
		m_DnyAttr.m_Blink.m_ColorBrush=dlg.m_ColorBrush;
		m_DnyAttr.m_Blink.m_ColorPen=dlg.m_ColorPen;
		m_DnyAttr.m_Blink.m_ColorText=dlg.m_ColorText;
		m_DnyAttr.m_Blink.m_Used=true;
		SendDlgItemMessage(IDC_BLINK,BM_SETCHECK,BST_CHECKED,0);
	}	
}

void CDiaDnyAttr::OnCdirection() 
{
	CDiaDirection dlg;
	dlg.m_TagName=m_DnyAttr.m_Direction.m_TagName;
	dlg.m_MaxClockWiseO=m_DnyAttr.m_Direction.m_MaxClockO;
	dlg.m_MaxClockwiseT=m_DnyAttr.m_Direction.m_MaxClockT;
	dlg.m_MinClockwiseO=m_DnyAttr.m_Direction.m_MinClockO;
	dlg.m_MinClockwiseT=m_DnyAttr.m_Direction.m_MinClockT;
	dlg.m_XBias=m_DnyAttr.m_Direction.m_XBias;
	dlg.m_YBias=m_DnyAttr.m_Direction.m_YBias;
	if(dlg.DoModal()==IDOK){
		m_DnyAttr.m_Direction.m_TagName=dlg.m_TagName;
		m_DnyAttr.m_Direction.m_MaxClockO=dlg.m_MaxClockWiseO;
		m_DnyAttr.m_Direction.m_MaxClockT=dlg.m_MaxClockwiseT;
		m_DnyAttr.m_Direction.m_MinClockO=dlg.m_MinClockwiseO;
		m_DnyAttr.m_Direction.m_MinClockT=dlg.m_MinClockwiseT;
		m_DnyAttr.m_Direction.m_XBias=dlg.m_XBias;
		m_DnyAttr.m_Direction.m_YBias=dlg.m_YBias;
		m_DnyAttr.m_Direction.m_Used=true;
		SendDlgItemMessage(IDC_DIRECTION,BM_SETCHECK,BST_CHECKED,0);
	}		
}

void CDiaDnyAttr::OnOK() 
{
	UpdateData(true);

    /*CComboBox *pCombo=(CComboBox*)GetDlgItem(IDC_OPTRANK);
    m_DnyAttr.m_Rank=pCombo->GetCurSel();*/

    m_DnyAttr.m_Linked=false;
	m_DnyAttr.m_Blink.m_Used=m_DnyAttr.m_Blink.m_Used && m_Blink;
	if(m_DnyAttr.m_Blink.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorBrushA.m_Used=m_DnyAttr.m_ColorBrushA.m_Used && m_BrushColorA;
	if(m_DnyAttr.m_ColorBrushA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorBrushAA.m_Used=m_DnyAttr.m_ColorBrushAA.m_Used && m_BrushColorAA;
	if(m_DnyAttr.m_ColorBrushAA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorBrushD.m_Used=m_DnyAttr.m_ColorBrushD.m_Used && m_BrushColorD;
	if(m_DnyAttr.m_ColorBrushD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorBrushDA.m_Used=m_DnyAttr.m_ColorBrushDA.m_Used && m_BrushColorDA;
	if(m_DnyAttr.m_ColorBrushDA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ButtonA.m_Used=m_DnyAttr.m_ButtonA.m_Used && m_ButtonA;
	if(m_DnyAttr.m_ButtonA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ButtonD.m_Used=m_DnyAttr.m_ButtonD.m_Used && m_ButtonD;
	if(m_DnyAttr.m_ButtonD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ButtonWnd.m_Used=m_DnyAttr.m_ButtonWnd.m_Used && m_ButtonWnd;
	if(m_DnyAttr.m_ButtonWnd.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_Direction.m_Used=m_DnyAttr.m_Direction.m_Used && m_Direction;
	if(m_DnyAttr.m_Direction.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_Enable.m_Used=m_DnyAttr.m_Enable.m_Used && m_Disable;
	if(m_DnyAttr.m_Enable.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_FillH.m_Used=m_DnyAttr.m_FillH.m_Used && m_FillH;
	if(m_DnyAttr.m_FillH.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_FillV.m_Used=m_DnyAttr.m_FillV.m_Used && m_FillV;
	if(m_DnyAttr.m_FillV.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_InputA.m_Used=m_DnyAttr.m_InputA.m_Used && m_InputA;
	if(m_DnyAttr.m_InputA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_InputD.m_Used=m_DnyAttr.m_InputD.m_Used && m_InputD;
	if(m_DnyAttr.m_InputD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorPenA.m_Used=m_DnyAttr.m_ColorPenA.m_Used && m_PenColorA;
	if(m_DnyAttr.m_ColorPenA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorPenAA.m_Used=m_DnyAttr.m_ColorPenAA.m_Used && m_PenColorAA;
	if(m_DnyAttr.m_ColorPenAA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorPenD.m_Used=m_DnyAttr.m_ColorPenD.m_Used && m_PenColorD;
	if(m_DnyAttr.m_ColorPenD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorPenDA.m_Used=m_DnyAttr.m_ColorPenDA.m_Used && m_PenColorDA;
	if(m_DnyAttr.m_ColorPenDA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_PosH.m_Used=m_DnyAttr.m_PosH.m_Used && m_PosH;
	if(m_DnyAttr.m_PosH.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_PosV.m_Used=m_DnyAttr.m_PosV.m_Used && m_PosV;
	if(m_DnyAttr.m_PosV.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_SizeH.m_Used=m_DnyAttr.m_SizeH.m_Used && m_SizeH;
	if(m_DnyAttr.m_SizeH.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_SizeV.m_Used=m_DnyAttr.m_SizeV.m_Used && m_SizeV;
	if(m_DnyAttr.m_SizeV.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_SlideH.m_Used=m_DnyAttr.m_SlideH.m_Used && m_SlideH;
	if(m_DnyAttr.m_SlideH.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_SlideV.m_Used=m_DnyAttr.m_SlideV.m_Used && m_SlideV;
	if(m_DnyAttr.m_SlideV.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorTextA.m_Used=m_DnyAttr.m_ColorTextA.m_Used && m_TextColorA;
	if(m_DnyAttr.m_ColorTextA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorTextAA.m_Used=m_DnyAttr.m_ColorTextAA.m_Used && m_TextColorAA;
	if(m_DnyAttr.m_ColorTextAA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorTextD.m_Used=m_DnyAttr.m_ColorTextD.m_Used && m_TextColorD;
	if(m_DnyAttr.m_ColorTextD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ColorTextDA.m_Used=m_DnyAttr.m_ColorTextDA.m_Used && m_TextColorDA;
	if(m_DnyAttr.m_ColorTextDA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ValueA.m_Used=m_DnyAttr.m_ValueA.m_Used && m_ValueA;
	if(m_DnyAttr.m_ValueA.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_ValueD.m_Used=m_DnyAttr.m_ValueD.m_Used && m_ValueD;
	if(m_DnyAttr.m_ValueD.m_Used) m_DnyAttr.m_Linked=true;
	m_DnyAttr.m_Visible.m_Used=m_DnyAttr.m_Visible.m_Used && m_Visible;
	if(m_DnyAttr.m_Visible.m_Used) m_DnyAttr.m_Linked=true;
	
	CDialog::OnOK();
}

void CDiaDnyAttr::IniShow()
{
	m_Blink=m_DnyAttr.m_Blink.m_Used;
	m_BrushColorA=m_DnyAttr.m_ColorBrushA.m_Used;
	m_BrushColorAA=m_DnyAttr.m_ColorBrushAA.m_Used;
	m_BrushColorD=m_DnyAttr.m_ColorBrushD.m_Used;
	m_BrushColorDA=m_DnyAttr.m_ColorBrushDA.m_Used;
	m_ButtonA=m_DnyAttr.m_ButtonA.m_Used;
	m_ButtonD=m_DnyAttr.m_ButtonD.m_Used;
	m_ButtonWnd=m_DnyAttr.m_ButtonWnd.m_Used;
	m_Direction=m_DnyAttr.m_Direction.m_Used;
	m_Disable=m_DnyAttr.m_Enable.m_Used;
	m_FillH=m_DnyAttr.m_FillH.m_Used;
	m_FillV=m_DnyAttr.m_FillV.m_Used;
	m_InputA=m_DnyAttr.m_InputA.m_Used;
	m_InputD=m_DnyAttr.m_InputD.m_Used;
	m_PenColorA=m_DnyAttr.m_ColorPenA.m_Used;
	m_PenColorAA=m_DnyAttr.m_ColorPenAA.m_Used;
	m_PenColorD=m_DnyAttr.m_ColorPenD.m_Used;
	m_PenColorDA=m_DnyAttr.m_ColorPenDA.m_Used;
	m_PosH=m_DnyAttr.m_PosH.m_Used;
	m_PosV=m_DnyAttr.m_PosV.m_Used;
	m_SizeH=m_DnyAttr.m_SizeH.m_Used;
	m_SizeV=m_DnyAttr.m_SizeV.m_Used;
	m_SlideH=m_DnyAttr.m_SlideH.m_Used;
	m_SlideV=m_DnyAttr.m_SlideV.m_Used;
	m_TextColorA=m_DnyAttr.m_ColorTextA.m_Used;
	m_TextColorAA=m_DnyAttr.m_ColorTextAA.m_Used;
	m_TextColorD=m_DnyAttr.m_ColorTextD.m_Used;
	m_TextColorDA=m_DnyAttr.m_ColorTextDA.m_Used;
	m_ValueA=m_DnyAttr.m_ValueA.m_Used;
	m_ValueD=m_DnyAttr.m_ValueD.m_Used;
	m_Visible=m_DnyAttr.m_Visible.m_Used;
}

BOOL CDiaDnyAttr::OnInitDialog() 
{
	CDialog::OnInitDialog();

    /*CComboBox *pCombo=(CComboBox*)GetDlgItem(IDC_OPTRANK);
    pCombo->SetCurSel(m_DnyAttr.m_Rank);*/
	CButton* pBtn0 = (CButton*)GetDlgItem(IDC_RANK0);
	pBtn0->SetCheck(m_DnyAttr.m_Rank & 0x01);

	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_RANK1);
	pBtn1->SetCheck(m_DnyAttr.m_Rank & 0x01<<1);

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_RANK2);
	pBtn2->SetCheck(m_DnyAttr.m_Rank & 0x01<<2);

	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_RANK3);
	pBtn3->SetCheck(m_DnyAttr.m_Rank & 0x01<<3);

	CButton* pBtn4 = (CButton*)GetDlgItem(IDC_RANK4);
	pBtn4->SetCheck(m_DnyAttr.m_Rank & 0x01<<4);

	CButton* pBtn5 = (CButton*)GetDlgItem(IDC_RANK5);
	pBtn5->SetCheck(m_DnyAttr.m_Rank & 0x01<<5);

	CButton* pBtn6 = (CButton*)GetDlgItem(IDC_RANK6);
	pBtn6->SetCheck(m_DnyAttr.m_Rank & 0x01<<6);

	CButton* pBtn7 = (CButton*)GetDlgItem(IDC_RANK7);
	pBtn7->SetCheck(m_DnyAttr.m_Rank & 0x01<<7);

	CButton* pBtn8 = (CButton*)GetDlgItem(IDC_RANK8);
	pBtn8->SetCheck(m_DnyAttr.m_Rank & 0x01<<8);

	CButton* pBtn9 = (CButton*)GetDlgItem(IDC_RANK9);
	pBtn9->SetCheck(m_DnyAttr.m_Rank & 0x01<<9);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaDnyAttr::OnBnClickedRank0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank0){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | 0x01;
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~0x01);
	}
}

void CDiaDnyAttr::OnBnClickedRank1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank1){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<1);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<1));
	}
}

void CDiaDnyAttr::OnBnClickedRank2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank2){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<2);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<2));
	}
}

void CDiaDnyAttr::OnBnClickedRank3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank3){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<3);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<3));
	}
}

void CDiaDnyAttr::OnBnClickedRank4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank4){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<4);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<4));
	}
}

void CDiaDnyAttr::OnBnClickedRank5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank5){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<5);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<5));
	}
}

void CDiaDnyAttr::OnBnClickedRank6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank6){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<6);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<6));
	}
}

void CDiaDnyAttr::OnBnClickedRank7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank7){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<7);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<7));
	}
}

void CDiaDnyAttr::OnBnClickedRank8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank8){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<8);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<8));
	}
}

void CDiaDnyAttr::OnBnClickedRank9()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_rank9){
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank | (0x01<<9);
	}else{
		m_DnyAttr.m_Rank = m_DnyAttr.m_Rank & (~(0x01<<9));
	}
}
