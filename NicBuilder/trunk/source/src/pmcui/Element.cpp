// Element.cpp: implementation of the CElement class.
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
long PMCUI_API lZoomMode,lPixHor,lPixVer;
static long lid;

IMPLEMENT_DYNAMIC(CElement,CObject)

CElement::CElement()
{
	m_lStatus=ST_NOMAL;
	m_lHitTarget=0;
	m_pFather=NULL;
	m_pChild=NULL;
	m_locked=false;
}

CElement::~CElement()
{

}

void CElement::Draw(CDC *pDC)
{

}

void CElement::DrawTracker(CDC *pDC)
{

}

void CElement::LPtoUP(long xl, long yl, long *xu, long *yu)
{
	switch(lZoomMode){
	case 0:
		if(xu) *xu=xl;
		if(yu) *yu=yl;
		break;
	case 1:
		if(xu) *xu=xl*TOTAL_WIDTH/lPixHor;
		if(yu) *yu=yl*TOTAL_HEIGHT/lPixVer;
		break;
    case 2:
		if(xu) *xu=xl*2.4;
		if(yu) *yu=-yl/2.4;
        break;
	}
}

void CElement::UPtoLP(long xu, long yu, long *xl, long *yl)
{
	switch(lZoomMode){
	case 0:
		if(xl) *xl=xu;
		if(yl) *yl=yu;
		break;
	case 1:
		if(xl) *xl=xu*lPixHor/TOTAL_WIDTH;
		if(yl) *yl=yu*lPixVer/TOTAL_HEIGHT;
		break;
    case 2:
		if(xl) *xl=xu*2.4;
		if(yl) *yl=-yu*2.4;
        break;
	}
}


void CElement::DrawHotPoint(CDC *pDC, long x, long y)
{
	long lDeltaX,lDeltaY;
	UPtoLP(DELTA,DELTA,&lDeltaX,&lDeltaY);
	pDC->Rectangle(x-lDeltaX,y-lDeltaY,x+lDeltaX,y+lDeltaY); 
}

void CElement::DrawHotPoints(CDC *pDC)
{

}

long CElement::HitTest(long x, long y)
{
	return TK_NONE;
}


bool CElement::RInR(LPCRECT lpRect)
{
	return false;
}

void CElement::Format(long left, long top, long right, long bottom, long flag)
{

}

long CElement::Left()
{
	return 0;
}

long CElement::Right()
{
	return 0;
}

long CElement::Top()
{
	return 0;
}

long CElement::Bottom()
{
	return 0;
}

void CElement::R2W(long xo, long yo, long *pxt, long *pyt)
{
	long xt,yt;
	xt=xo+yo-*pyt;
	yt=yo+*pxt-xo;
	*pxt=xt;
	*pyt=yt;
}

void CElement::R2U(long xo, long yo, long *pxt, long *pyt)
{
	long xt,yt;
	xt=xo+*pyt-yo;
	yt=yo+xo-*pxt;
	*pxt=xt;
	*pyt=yt;
}

void CElement::F2Z(long xo, long *pxt)
{
	*pxt=2*xo-*pxt;
}

void CElement::F2I(long yo, long *pyt)
{
	*pyt=2*yo-*pyt;
}

CElement * CElement::Clone(long lCmdId,long lStatus)
{	
	return NULL;
}


void CElement::Save(MSXML::IXMLDOMElementPtr pRoot)
{

}

void CElement::MoveInCell(POINT potTracker)
{

}

long CElement::GetLevel()
{
	if(m_DnyAttr.m_Linked)
		return 1;
	else
		return 0;
}
