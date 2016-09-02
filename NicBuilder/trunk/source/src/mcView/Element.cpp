// Element.cpp: implementation of the CElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Element.h"
#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern bool gbIsPrint;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CElement,CObject)

CElement::CElement()
{
	m_lStatus=ST_NOMAL;
	m_lHitTarget=0;
	m_pFather=NULL;
	m_pChild=NULL;
	m_potTracker.x=0;
	m_potTracker.y=0;
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
    if(gbIsPrint){
        if(xu) *xu=xl/2.4;
        if(yu) *yu=-yl/2.4;
    }else{
	    if(xu) *xu=xl*TOTAL_WIDTH/glViewWidth;
	    if(yu) *yu=yl*TOTAL_HEIGHT/glViewHeight;
    }
}

void CElement::UPtoLP(long xu, long yu, long *xl, long *yl)
{
    if(gbIsPrint){
        if(xl) *xl=xu*2.4;
        if(yl) *yl=-yu*2.4;
    }else{
	    if(xl) *xl=xu*glViewWidth/TOTAL_WIDTH;
	    if(yl) *yl=yu*glViewHeight/TOTAL_HEIGHT;
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

void CElement::UpdateView()
{

}

void CElement::Slide(POINT potTracker)
{
	if(!m_DnyAttr.m_en) return;

	if(m_DnyAttr.m_SlideH.m_Used || m_DnyAttr.m_SlideV.m_Used){
		potTracker.x+=m_potTracker.x;
		potTracker.y+=m_potTracker.y;
		if(!m_DnyAttr.m_SlideH.m_Used){
			potTracker.x=0;
		}else{
			if(potTracker.x<m_DnyAttr.m_SlideH.m_LTT)
				potTracker.x=m_DnyAttr.m_SlideH.m_LTT;
			if(potTracker.x>m_DnyAttr.m_SlideH.m_RBT)
				potTracker.x=m_DnyAttr.m_SlideH.m_RBT;
			writeAnalog(
				(LPCTSTR)m_DnyAttr.m_SlideH.m_TagName,
				m_DnyAttr.m_SlideH.m_LTO+\
				(m_DnyAttr.m_SlideH.m_RBO-m_DnyAttr.m_SlideH.m_LTO)*\
				(potTracker.x-m_DnyAttr.m_SlideH.m_LTT)/\
				(m_DnyAttr.m_SlideH.m_RBT-m_DnyAttr.m_SlideH.m_LTT)
				);
			TRACE("MOVE ON\n");
		}
		if(!m_DnyAttr.m_SlideV.m_Used){
			potTracker.y=0;
		}else{
			if(potTracker.y<m_DnyAttr.m_SlideV.m_LTT)
				potTracker.y=m_DnyAttr.m_SlideV.m_LTT;
			if(potTracker.y>m_DnyAttr.m_SlideV.m_RBT)
				potTracker.y=m_DnyAttr.m_SlideV.m_RBT;
			writeAnalog(
				m_DnyAttr.m_SlideV.m_TagName,
				m_DnyAttr.m_SlideV.m_LTO+\
				(m_DnyAttr.m_SlideV.m_RBO-m_DnyAttr.m_SlideV.m_LTO)*\
				(potTracker.y-m_DnyAttr.m_SlideV.m_LTT)/\
				(m_DnyAttr.m_SlideV.m_RBT-m_DnyAttr.m_SlideV.m_LTT)
			);
		}

		MoveElement(potTracker);
		m_potTracker=potTracker;
	}
}


void CElement::MoveElement(POINT potOffset)
{
	//
}
