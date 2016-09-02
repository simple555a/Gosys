// DlgQueryById.cpp : implementation file
//

#include "stdafx.h"
#include "evtlq.h"
#include "DlgQueryById.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQueryById dialog


CDlgQueryById::CDlgQueryById(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQueryById::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQueryById)
	m_count = 10;
	m_pt = 0;
	//}}AFX_DATA_INIT
}


void CDlgQueryById::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQueryById)
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Text(pDX, IDC_FIRSTPT, m_pt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQueryById, CDialog)
	//{{AFX_MSG_MAP(CDlgQueryById)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQueryById message handlers
