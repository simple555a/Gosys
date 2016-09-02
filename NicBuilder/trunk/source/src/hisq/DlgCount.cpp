// DlgCount.cpp : implementation file
//

#include "stdafx.h"
#include "hisq.h"
#include "DlgCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCount dialog


CDlgCount::CDlgCount(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCount)
	m_count = 0;
	//}}AFX_DATA_INIT
}


void CDlgCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCount)
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDV_MinMaxInt(pDX, m_count, 1, 600);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCount, CDialog)
	//{{AFX_MSG_MAP(CDlgCount)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCount message handlers
