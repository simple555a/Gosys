// DiaReplace.cpp : implementation file
//

#include "stdafx.h"
#include "mcdraw.h"
#include "DiaReplace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaReplace dialog


CDiaReplace::CDiaReplace(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaReplace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaReplace)
	m_original = _T("");
	m_replaced = _T("");
	//}}AFX_DATA_INIT
}


void CDiaReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaReplace)
	DDX_Text(pDX, IDC_ORIGINAL, m_original);
	DDX_Text(pDX, IDC_REPLACED, m_replaced);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaReplace, CDialog)
	//{{AFX_MSG_MAP(CDiaReplace)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaReplace message handlers
