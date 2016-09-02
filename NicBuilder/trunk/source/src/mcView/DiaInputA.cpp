// DiaInputA.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaInputA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString m_sCurUser;
/////////////////////////////////////////////////////////////////////////////
// CDiaInputA dialog
CDiaInputA::CDiaInputA(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaInputA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaInputA)
	m_value = 0.0f;
	//}}AFX_DATA_INIT
	m_bVisible=false;
	m_bValueInited = false;
}


void CDiaInputA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaInputA)
	DDX_Text(pDX, IDC_VALUE, m_value);
	//}}AFX_DATA_MAP
	if(m_pInputA->m_MinVal>m_pInputA->m_MaxVal){
		pDX->Fail();
	}else{
		DDV_MinMaxFloat(pDX, m_value, m_pInputA->m_MinVal, m_pInputA->m_MaxVal);
	}
}


BEGIN_MESSAGE_MAP(CDiaInputA, CDialog)
	//{{AFX_MSG_MAP(CDiaInputA)
	ON_BN_CLICKED(IDC_OUTPUT, OnOutput)
	ON_WM_NCLBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_0, On0)
	ON_BN_CLICKED(IDC_1, On1)
	ON_BN_CLICKED(IDC_2, On2)
	ON_BN_CLICKED(IDC_3, On3)
	ON_BN_CLICKED(IDC_4, On4)
	ON_BN_CLICKED(IDC_5, On5)
	ON_BN_CLICKED(IDC_6, On6)
	ON_BN_CLICKED(IDC_7, On7)
	ON_BN_CLICKED(IDC_8, On8)
	ON_BN_CLICKED(IDC_9, On9)
	ON_BN_CLICKED(IDC_SUB, OnSub)
	ON_BN_CLICKED(IDC_DOT, OnDot)
	ON_BN_CLICKED(IDC_CLR, OnClr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaInputA message handlers

void CDiaInputA::RefreshShow()
{
	SetWindowText((LPCTSTR)(m_pInputA->m_WndPrm));
	CString sTemp;
	sTemp.Format("%.2f",m_pInputA->m_MinVal);
	SetDlgItemText(IDC_LIMIT_MIN,(LPCTSTR)sTemp);
	sTemp.Format("%.2f",m_pInputA->m_MaxVal);
	SetDlgItemText(IDC_LIMIT_MAX,(LPCTSTR)sTemp);
	CWnd *pWnd;
	pWnd=GetDlgItem(IDC_OUTPUT);
	if(m_pInputA->m_TagValue.Flags & TF_Valid){		
		pWnd->EnableWindow();
		sTemp.Format("%.2f", getFloat(m_pInputA->m_TagValue));
		SetDlgItemText(IDC_STATUS,(LPCTSTR)sTemp);
		//将模拟量前一时刻的值，记录到ini文件中mox+++
		//WritePrivateProfileString("temp", "oldValue", "", get_config_file());
		//WritePrivateProfileString("temp", "oldValue", sTemp, get_config_file());
		if(!m_bValueInited){
			SetDlgItemText(IDC_VALUE, sTemp);
			GotoDlgCtrl(pBoxOne);
		}
	}else{
		pWnd->EnableWindow(FALSE);
		SetDlgItemText(IDC_STATUS,"N/A");
		//将模拟量前一时刻的值记录到ini文件中mox+++
        //WritePrivateProfileString("temp", "oldValue", "", get_config_file());
		//WritePrivateProfileString("temp", "oldValue", "N/A", get_config_file());
	}
	m_bValueInited = true;
}

void CDiaInputA::OnOutput() 
{
	if(!UpdateData()) return;
    //将模拟量手动置的值记录到ini文件中mox+++
	//CString temp;
	//temp.Format("%f",m_value);
	//WritePrivateProfileString("temp", "newValue", "", get_config_file());
	//WritePrivateProfileString("temp", "newValue", temp, get_config_file());
    ///mox+++

    //mox20141015
	//writeAnalog(m_pInputA->m_TagName, m_value);
    //取ini中前一时刻的值
	//CString old_value;
	//char buff1[8];
	//memset(buff1,  ' ', sizeof(buff1));
	//char strAt1[8];//模拟量置值前一时刻的值
	//memset(strAt1, ' ', sizeof(strAt1));

	//取过去值
	//GetPrivateProfileString(
	//	"temp",
	//	"oldValue",
	//	"N/A",
	//	buff1,
	//	_countof(buff1),
	//	get_config_file());
	//if (buff1)
	//{
	//	_tcsncpy_s(strAt1, buff1, sizeof(strAt1));
	//	memset(buff1, ' ', sizeof(buff1));
	//}
    
	//old_value.Format("%s",strAt1);
	writeAnalog_x(m_pInputA->m_TagName, m_value);
	OnCancel();
}



void CDiaInputA::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	OnCancel();
		
	CDialog::OnNcLButtonDblClk(nHitTest, point);
}

BOOL CDiaInputA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pBoxOne = (CEdit*) GetDlgItem(IDC_VALUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaInputA::On0() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '0',0);	
}

void CDiaInputA::On1() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '1',0);		
}

void CDiaInputA::On2() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '2',0);		
}

void CDiaInputA::On3() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '3',0);	
}

void CDiaInputA::On4() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '4',0);		
}

void CDiaInputA::On5() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '5',0);	
}

void CDiaInputA::On6() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '6',0);	
}

void CDiaInputA::On7() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '7',0);	
}

void CDiaInputA::On8() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '8',0);		
}

void CDiaInputA::On9() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '9',0);	
}

void CDiaInputA::OnSub() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '-',0);	
}

void CDiaInputA::OnDot() 
{
	::SendMessage(pBoxOne->m_hWnd, WM_CHAR, '.',0);	
}

void CDiaInputA::OnClr() 
{
	GotoDlgCtrl(pBoxOne);
	pBoxOne->Clear();
}
