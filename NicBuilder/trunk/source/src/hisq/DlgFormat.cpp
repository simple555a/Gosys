// DlgFormat.cpp : implementation file
//

#include "stdafx.h"
#include "hisq.h"
#include "DlgFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog


CDlgFormat::CDlgFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormat)
	m_format = "%.3f";
	//}}AFX_DATA_INIT
}


void CDlgFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormat)
	DDX_Control(pDX, IDC_LIST_FORMAT, m_list);
	DDX_Text(pDX, IDC_EDIT_FORMAT, m_format);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormat, CDialog)
	//{{AFX_MSG_MAP(CDlgFormat)
	ON_LBN_SELCHANGE(IDC_LIST_FORMAT, OnSelchangeListFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat message handlers

BOOL CDlgFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_list.AddString("自动");
    m_list.AddString("不显示小数");
	m_list.AddString("1位小数");
	m_list.AddString("2位小数");
	m_list.AddString("3位小数");
	m_list.AddString("4位小数");
	m_list.AddString("5位小数");
	m_list.AddString("6位小数");
	m_list.AddString("指数");
    if("%g"==m_format){
        m_list.SetCurSel(0);
    }else if("%.0f"==m_format){
		m_list.SetCurSel(1);
	}else if("%.1f"==m_format){
		m_list.SetCurSel(2);
	}else if("%.2f"==m_format){
		m_list.SetCurSel(3);
	}else if("%.3f"==m_format){
		m_list.SetCurSel(4);
	}else if("%.4f"==m_format){
		m_list.SetCurSel(5);
	}else if("%.5f"==m_format){
		m_list.SetCurSel(6);
	}else if("%.61f"==m_format){
		m_list.SetCurSel(7);
	}else if("%e"==m_format){
		m_list.SetCurSel(8);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFormat::OnSelchangeListFormat() 
{
	switch(m_list.GetCurSel()){
    case 0:
        m_format="%g";
	case 1:
		m_format="%.0f";
		break;
	case 2:
		m_format="%.1f";
		break;
	case 3:
		m_format="%.2f";
		break;
	case 4:
		m_format="%.3f";
		break;
	case 5:
		m_format="%.4f";
		break;
	case 6:
		m_format="%.5f";
		break;
	case 7:
		m_format="%.6f";
		break;
	case 8:
		m_format="%e";
		break;
	default:
		;
	}

	UpdateData(FALSE);
}
