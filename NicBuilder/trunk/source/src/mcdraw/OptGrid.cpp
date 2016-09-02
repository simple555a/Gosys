// OptGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "OptGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptGrid property page

IMPLEMENT_DYNCREATE(COptGrid, CPropertyPage)

COptGrid::COptGrid() : CPropertyPage(COptGrid::IDD)
{
	//{{AFX_DATA_INIT(COptGrid)
	m_HorGrid = 0;
	m_LockOnGrid = FALSE;
	m_VerGrid = 0;
	//}}AFX_DATA_INIT
}

COptGrid::~COptGrid()
{
}

void COptGrid::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptGrid)
	DDX_Text(pDX, IDC_HORGRID, m_HorGrid);
	DDV_MinMaxLong(pDX, m_HorGrid, 16, 256);
	DDX_Check(pDX, IDC_LOCKONGRID, m_LockOnGrid);
	DDX_Text(pDX, IDC_VERGRID, m_VerGrid);
	DDV_MinMaxLong(pDX, m_VerGrid, 16, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptGrid, CPropertyPage)
	//{{AFX_MSG_MAP(COptGrid)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptGrid message handlers
