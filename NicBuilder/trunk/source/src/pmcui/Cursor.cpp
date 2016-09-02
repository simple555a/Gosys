// Cursor.cpp: implementation of the CCursor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCursor::CCursor()
{
	m_bHavePen=false;
	m_bHaveBrush=false;
	m_bHaveFont=false;

	m_bActive=false;
	m_bShow=false;
}

CCursor::~CCursor()
{

}

void CCursor::Show(CDC *pDC)
{
	if(m_bShow) return;
	m_bShow=true;

	CPen *pOldPen=(CPen *)pDC->SelectStockObject(BLACK_PEN);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2;
	UPtoLP(m_lx,m_ly,&x1,&y1);
	UPtoLP(m_lx,m_ly+m_lHeight,&x2,&y2);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
}

void CCursor::Hide(CDC *pDC)
{
	if(!m_bShow) return;
	m_bShow=false;

	CPen *pOldPen=(CPen *)pDC->SelectStockObject(BLACK_PEN);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2;
	UPtoLP(m_lx,m_ly,&x1,&y1);
	UPtoLP(m_lx,m_ly+m_lHeight,&x2,&y2);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
}
