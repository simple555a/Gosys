// Dm301Config.cpp : implementation file
//

#include "stdafx.h"
#include "libf8dmcfgui.h"
#include "Dm301Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDm301Config dialog


CDm301Config::CDm301Config(CWnd* pParent /*=NULL*/)
	: CDialog(CDm301Config::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDm301Config)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDm301Config::DoDataExchange(CDataExchange* pDX)
{
	CComboBox cb;
	HWND h;

	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CDm301Config)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	if(pDX->m_bSaveAndValidate){

#define ss(n) \
	do{ \
		GetDlgItem(IDC_COMBO##n, &h);\
		cb.Attach(h);\
		d[n-1] = cb.GetCurSel();\
		cb.Detach();\
	}while(0)

		ss(1);
		ss(2);
		ss(3);
		ss(4);
		ss(5);
		ss(6);
		ss(7);
		ss(8);

#undef ss

	} // save and validate
}


BEGIN_MESSAGE_MAP(CDm301Config, CDialog)
	//{{AFX_MSG_MAP(CDm301Config)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDm301Config message handlers

BOOL CDm301Config::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString s;
	CComboBox cb;
	HWND h;

	SetWindowText("DM301通道参数配置");

#define ss(n) \
	{ \
		s.Format("通道%d", n);\
		GetDlgItem(IDC_STATIC##n)->SetWindowText(s);\
		GetDlgItem(IDC_COMBO##n, &h);\
		cb.Attach(h);\
		cb.AddString("1-5V(4-20 mA)");\
		cb.AddString("0-5V");\
		cb.AddString("-5-5V");\
		cb.AddString("0-10 V");\
		cb.AddString("-10-10 V");\
		if(d[n-1] < 5){\
			cb.SetCurSel(d[n-1]);\
		}else{\
			cb.SetCurSel(0);\
		}\
		cb.Detach();\
	}

	ss(1);
	ss(2);
	ss(3);
	ss(4);
	ss(5);
	ss(6);
	ss(7);
	ss(8);

#undef ss

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern "C" __declspec(dllexport) __bool dm301Config(HWND p, __u8 * data, __uint len, __uint reserved)
{
	CDm301Config * c;
	__bool r;
	CWnd w;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(len > sizeof(c->d)){
		return __false;
	}
	w.Attach(p);
	c=new CDm301Config(&w);
	memcpy(c->d, data, len);
	r=c->DoModal()==IDOK? __true : __false;
	if(r){
		memcpy(data, c->d, len);
	}
	delete c;
	w.Detach();
	return r;
}

extern "C" __declspec(dllexport) int test(int,int,int,int)
{
	__u8 a[32]={1,2,3,0,1,2,3,4,0};
	HWND h;
	
	h=CreateWindow(
		  "EDIT",
		  "",
		  WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  CW_USEDEFAULT,
		  NULL,
		  NULL,
		  NULL,
		  NULL);
	dm301Config(h, a, 10, 0);
	return 0;
}

