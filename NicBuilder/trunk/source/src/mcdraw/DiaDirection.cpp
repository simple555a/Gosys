// DiaDirection.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaDirection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaDirection dialog


CDiaDirection::CDiaDirection(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaDirection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaDirection)
	m_MaxClockWiseO = 0.0f;
	m_MaxClockwiseT = 0.0f;
	m_MinClockwiseO = 0.0f;
	m_MinClockwiseT = 0.0f;
	m_TagName = _T("");
	m_XBias = 0.0f;
	m_YBias = 0.0f;
	//}}AFX_DATA_INIT
}


void CDiaDirection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaDirection)
	DDX_Text(pDX, IDC_MAXCLOCKWISE_O, m_MaxClockWiseO);
	DDX_Text(pDX, IDC_MAXCLOCKWISE_T, m_MaxClockwiseT);
	DDX_Text(pDX, IDC_MINCLOCKWISE_O, m_MinClockwiseO);
	DDX_Text(pDX, IDC_MINCLOCKWISE_T, m_MinClockwiseT);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDV_MaxChars(pDX, m_TagName, TAGNAME_TEXT_LENGTH);
	DDX_Text(pDX, IDC_XBIAS, m_XBias);
	DDX_Text(pDX, IDC_YBIAS, m_YBias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaDirection, CDialog)
	//{{AFX_MSG_MAP(CDiaDirection)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaDirection message handlers
