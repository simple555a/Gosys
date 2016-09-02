// DlgLoop.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgLoop.h"
#include "DlgSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoop dialog


CDlgLoop::CDlgLoop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoop)
	m_interval = 3;
	//}}AFX_DATA_INIT
}


void CDlgLoop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoop)
	DDX_Control(pDX, IDC_LOOPLIST, m_loopList);
	DDX_Text(pDX, IDC_INTERVAL, m_interval);
	DDV_MinMaxInt(pDX, m_interval, 1, 60);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoop, CDialog)
	//{{AFX_MSG_MAP(CDlgLoop)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoop message handlers

void CDlgLoop::OnOK() 
{
	CString sLoopFile = ((CMcviewApp*)::AfxGetApp())->m_path+"loops";
	FILE *pLoops = fopen((LPCTSTR)sLoopFile, "w+");
	ASSERT(pLoops != NULL);

	CString sItem;
	for(int i = 0; i < m_loopList.GetCount(); i++){
		m_loopList.GetText(i, sItem);
		fprintf(pLoops, sItem);
	}
	
	fclose(pLoops);

	CDialog::OnOK();
}

BOOL CDlgLoop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString sLoopFile = ((CMcviewApp*)::AfxGetApp())->m_path+"loops";
	FILE *pLoops = fopen((LPCTSTR)sLoopFile, "r");
	if(pLoops == NULL){
		return TRUE;
	}

	CString sItem;
	char buf[1024];
	while(fgets(buf, 1024, pLoops)){
		sItem = buf;
		m_loopList.AddString(sItem);
	}

	fclose(pLoops);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoop::OnAdd() 
{
	CDlgSelect dlgSelect;
	dlgSelect.m_dir = ((CMcviewApp*)::AfxGetApp())->m_path+"file\\*.gdf";

	if(dlgSelect.DoModal() != IDOK){
		return;
	}

	m_loopList.AddString(dlgSelect.m_select + "\n");
}

void CDlgLoop::OnDelete() 
{
	int nSel = m_loopList.GetCurSel();
	if(LB_ERR == nSel){
		return;
	}

	m_loopList.DeleteString(nSel);
}
