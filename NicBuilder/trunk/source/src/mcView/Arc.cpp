// Arc.cpp: implementation of the CArc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Arc.h"

#define PI 3.14159265358979

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CArc,CElement)

CArc::CArc()
{
	m_bHavePen=true;
	m_bHaveBrush=false;
	m_bHaveFont=false;
	m_lx3=0;
	m_ly3=0;
	m_lx4=0;
	m_ly4=0;
}

CArc::~CArc()
{

}

void CArc::Draw(CDC *pDC)
{
	long x1,y1,x2,y2,x3,y3,x4,y4;
	if(m_DnyAttr.m_Linked && m_DnyAttr.m_na){
		CPen *pop=(CPen*)(pDC->SelectStockObject(BLACK_PEN));
		CBrush *pob=(CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
		UPtoLP(m_lox1,m_loy1,&x1,&y1);
		UPtoLP(m_lox2,m_loy2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		pDC->SelectObject(pop);
		pDC->SelectObject(pob);
		pDC->DrawIcon(x1,y1,ghNA);
		return;
	}

	if(m_DnyAttr.m_Linked && !m_DnyAttr.m_vi) return;

	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);

	x1=m_lx1;
	y1=m_ly1;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	UPtoLP(m_lx3,m_ly3,&x3,&y3);
	UPtoLP(m_lx4,m_ly4,&x4,&y4);
	pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);

	pDC->SelectObject(pOldPen);
}

void CArc::DrawTracker(CDC *pDC)
{
	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2,x3,y3,x4,y4;

	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	UPtoLP(m_lx3,m_ly3,&x3,&y3);
	UPtoLP(m_lx4,m_ly4,&x4,&y4);
	//pDC->Chord(x1,y1,x2,y2,x3,y3,x4,y4);
	pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);

	switch(m_lHitTarget){
	case TK_LT:
		m_lx1=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_LB:
		m_lx1=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx1,m_ly2,&x1,&y2);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_RT:
		m_lx2=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx2,m_ly1,&x2,&y1);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_RB:
		m_lx2=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_LM:
		m_lx1=m_potTracker.x;
		UPtoLP(m_lx1,m_lx1,&x1,&x1);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_RM:
		m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_lx2,&x2,0);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_TM:
		m_ly1=m_potTracker.y;
		UPtoLP(m_ly1,m_ly1,0,&y1);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_BM:
		m_ly2=m_potTracker.y;
		UPtoLP(m_ly2,m_ly2,0,&y2);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;

	case TK_MOV:
		m_lx1+=m_potTracker.x;
		m_ly1+=m_potTracker.y;
		m_lx2+=m_potTracker.x;
		m_ly2+=m_potTracker.y;
		m_lx3+=m_potTracker.x;
		m_ly3+=m_potTracker.y;
		m_lx4+=m_potTracker.x;
		m_ly4+=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		UPtoLP(m_lx3,m_ly3,&x3,&y3);
		UPtoLP(m_lx4,m_ly4,&x4,&y4);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_SP:
		m_lx3=m_potTracker.x;
		m_ly3=m_potTracker.y;
		UPtoLP(m_lx3,m_ly3,&x3,&y3);
		//pDC->Chord(x1,y1,x2,y2,x3,y3,x4,y4);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	case TK_EP:
		m_lx4=m_potTracker.x;
		m_ly4=m_potTracker.y;
		UPtoLP(m_lx4,m_ly4,&x4,&y4);
		//pDC->Chord(x1,y1,x2,y2,x3,y3,x4,y4);
		pDC->Arc(x1,y1,x2,y2,x3,y3,x4,y4);
		break;
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CArc::DrawHotPoints(CDC *pDC)
{
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
	CPen *pOldPen=(CPen *)pDC->SelectStockObject(NULL_PEN);
	pDC->SetROP2(R2_NOT);

	long x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7,x8,y8;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx1,m_ly2,&x2,&y2);
	UPtoLP(m_lx2,m_ly1,&x3,&y3);
	UPtoLP(m_lx2,m_ly2,&x4,&y4);
	UPtoLP((m_lx1+m_lx2)/2,m_ly1,&x5,&y5);
	UPtoLP((m_lx1+m_lx2)/2,m_ly2,&x6,&y6);
	UPtoLP(m_lx1,(m_ly1+m_ly2)/2,&x7,&y7);
	UPtoLP(m_lx2,(m_ly1+m_ly2)/2,&x8,&y8);
	DrawHotPoint(pDC,x1,y1);
	DrawHotPoint(pDC,x2,y2);
	DrawHotPoint(pDC,x3,y3);
	DrawHotPoint(pDC,x4,y4);
	DrawHotPoint(pDC,x5,y5);
	DrawHotPoint(pDC,x6,y6);
	DrawHotPoint(pDC,x7,y7);
	DrawHotPoint(pDC,x8,y8);
	
	long x0,y0,x9,y9;
	UPtoLP(m_lx3,m_ly3,&x0,&y0);
	UPtoLP(m_lx4,m_ly4,&x9,&y9);
	long lDeltaX,lDeltaY;
	UPtoLP((long)(1.2*DELTA),(long)(1.2*DELTA),&lDeltaX,&lDeltaY);
	pDC->Ellipse(x0-lDeltaX,y0-lDeltaY,x0+lDeltaX,y0+lDeltaY); 
	pDC->Ellipse(x9-lDeltaX,y9-lDeltaY,x9+lDeltaX,y9+lDeltaY); 

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);
}

long CArc::HitTest(long x, long y)
{
    if(!m_DnyAttr.m_Linked) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;
	
	long x1,y1,x2,y2;
	if(m_lx1<=m_lx2){
		x1=m_lx1;
		x2=m_lx2;
	}else{
		x1=m_lx2;
		x2=m_lx1;
	}
	if(m_ly1<m_ly2){
		y1=m_ly1;
		y2=m_ly2;
	}else{
		y1=m_ly2;
		y2=m_ly1;
	}
	CRgn rgnTmp;
    if(rgnTmp.CreateEllipticRgn(x1-DELTA,y1-DELTA,x2+DELTA,y2+DELTA)){
	    if(!rgnTmp.PtInRegion(pt)) return TK_NONE;
	    x1=x1+DELTA;
	    y1=y1+DELTA;
	    x2=x2-DELTA;
	    y2=y2-DELTA;
	    if(x1<x2 && y1<y2){
		    CRgn rgnTmp;
            if(rgnTmp.CreateEllipticRgn(x1,y1,x2,y2)){
		        if(rgnTmp.PtInRegion(pt)) return TK_NONE;
            }else{
                return TK_NONE;
            }
	    }
	    double dx=(double)x-((double)m_lx1+(double)m_lx2)/2;
	    double dy=-(double)y+((double)m_ly1+(double)m_ly2)/2;
	    double dxs=(double)m_lx3-((double)m_lx1+(double)m_lx2)/2;
	    double dys=-(double)m_ly3+((double)m_ly1+(double)m_ly2)/2;
	    double dxe=(double)m_lx4-((double)m_lx1+(double)m_lx2)/2;
	    double dye=-(double)m_ly4+((double)m_ly1+(double)m_ly2)/2;
	    double sa=GetAngle(dxs,dys);
	    double ea=GetAngle(dxe,dye);
	    double pa=GetAngle(dx,dy);

	    if(SPE(sa,pa,ea)) return TK_PUSH;
    }
	return TK_NONE;
}


bool CArc::RInR(LPCRECT lpRect)
{
	if(m_lStatus!=ST_NOMAL) return false;

	double ca=((double)m_lx1+(double)m_lx2)/2.;
	double cb=((double)m_ly1+(double)m_ly2)/2.;
	double ra=((double)m_lx2-(double)m_lx1)/2.;
	double rb=((double)m_ly2-(double)m_ly1)/2.;
	double dxs=(double)m_lx3-ca;
	double dys=-(double)m_ly3+cb;
	double dxe=(double)m_lx4-ca;
	double dye=-(double)m_ly4+cb;
	double sa=GetAngle(dxs,dys);
	double ea=GetAngle(dxe,dye);
	double pa;

	CRect recTmp(lpRect->left-1,lpRect->top-1,lpRect->right+1,lpRect->bottom+1);
	POINT pt;
	pt.x=ca+ra*cos(sa);
	pt.y=cb-rb*sin(sa);
	if(recTmp.PtInRect(pt)) return true;
	pt.x=ca+ra*cos(ea);
	pt.y=cb-rb*sin(ea);
	if(recTmp.PtInRect(pt)) return true;

	double dxp,dyp;
	dxp=(double)(lpRect->left)-ca;
	if(fabs(dxp)<ra){
		dyp=rb*sqrt(1.-dxp*dxp/ra/ra);
		pa=GetAngle(dxp,dyp);
		pt.x=lpRect->left;
		if(SPE(sa,pa,ea)){
			pt.y=(long)cb-(long)dyp;
			if(recTmp.PtInRect(pt)) return true;
		}
		pa=GetAngle(dxp,-dyp);
		if(SPE(sa,pa,ea)){
			pt.y=(long)cb+(long)dyp;
			if(recTmp.PtInRect(pt)) return true;
		}
	}
	dxp=(double)(lpRect->right)-ca;
	if(fabs(dxp)<ra){
		dyp=rb*sqrt(1.-dxp*dxp/ra/ra);
		pa=GetAngle(dxp,dyp);
		pt.x=lpRect->right;
		if(SPE(sa,pa,ea)){
			pt.y=(long)cb-(long)dyp;
			if(recTmp.PtInRect(pt)) return true;
		}
		pa=GetAngle(dxp,-dyp);
		if(SPE(sa,pa,ea)){
			pt.y=(long)cb+(long)dyp;
			if(recTmp.PtInRect(pt)) return true;
		}
	}
	dyp=-(double)(lpRect->top)+cb;
	if(fabs(dyp)<rb){
		dxp=ra*sqrt(1.-dyp*dyp/rb/rb);
		pa=GetAngle(dxp,dyp);
		pt.y=lpRect->top;
		if(SPE(sa,pa,ea)){
			pt.x=(long)ca+(long)dxp;
			if(recTmp.PtInRect(pt)) return true;
		}
		pa=GetAngle(-dxp,dyp);
		if(SPE(sa,pa,ea)){
			pt.x=(long)ca-(long)dxp;
			if(recTmp.PtInRect(pt)) return true;
		}
	}
	dyp=-(double)(lpRect->bottom)+cb;
	if(fabs(dyp)<rb){
		dxp=ra*sqrt(1.-dyp*dyp/rb/rb);
		pa=GetAngle(dxp,dyp);
		pt.y=lpRect->bottom;
		if(SPE(sa,pa,ea)){
			pt.x=(long)ca+(long)dxp;
			if(recTmp.PtInRect(pt)) return true;
		}
		pa=GetAngle(-dxp,dyp);
		if(SPE(sa,pa,ea)){
			pt.x=(long)ca-(long)dxp;
			if(recTmp.PtInRect(pt)) return true;
		}
	}

	return false;
}


double CArc::GetAngle(double dx, double dy)
{
	double dd=sqrt(dx*dx+dy*dy);
	if(dd<1.) dd=1.;
	double pCos=dx/dd;
	double pSin=dy/dd;
	double pa=asin(fabs(pSin));
	if(pSin>0 && pCos>0){
		return pa;
	}else if(pSin>=0 && pCos<=0){
		return (PI-pa);
	}else if(pSin<=0 && pCos>=0){
		return (2.*PI-pa);
	}else if(pSin<=0 && pCos<=0){
		return (PI+pa);
	}else{
		return 0;
	}
}

bool CArc::SPE(double sa, double pa, double ea)
{
	if(pa<sa) pa=pa+2*PI;
	if(ea<sa) ea=ea+2*PI;
	if(pa<ea) return true;
	else return false;

}

long CArc::Left()
{
	return m_lx1;
}

long CArc::Right()
{
	return m_lx2;
}

long CArc::Top()
{
	return m_ly1;
}

long CArc::Bottom()
{
	return m_ly2;
}

void CArc::Format(long left, long top, long right, long bottom, long flag)
{
	long ldx,ldy;
	switch(flag){
	case FM_A2L:
		ldx=left-Left();
		m_lx1+=ldx;
		m_lx2+=ldx;
		m_lx3+=ldx;
		m_lx4+=ldx;
		break;
	case FM_A2C:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;
		m_lx3+=ldx;
		m_lx4+=ldx;
		break;
	case FM_A2R:
		ldx=right-Right();
		m_lx1+=ldx;
		m_lx2+=ldx;
		m_lx3+=ldx;
		m_lx4+=ldx;
		break;
	case FM_A2T:
		ldy=top-Top();
		m_ly1+=ldy;
		m_ly2+=ldy;
		m_ly3+=ldy;
		m_ly4+=ldy;
		break;
	case FM_A2M:
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		m_ly3+=ldy;
		m_ly4+=ldy;
		break;
	case FM_A2B:
		ldy=bottom-Bottom();
		m_ly1+=ldy;
		m_ly2+=ldy;
		m_ly3+=ldy;
		m_ly4+=ldy;
		break;
	case FM_A2CM:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;
		m_lx3+=ldx;
		m_lx4+=ldx;
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		m_ly3+=ldy;
		m_ly4+=ldy;
		break;
	case FM_R2W:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2W(ldx,ldy,&m_lx1,&m_ly1);
		R2W(ldx,ldy,&m_lx2,&m_ly2);
		R2W(ldx,ldy,&m_lx3,&m_ly3);
		R2W(ldx,ldy,&m_lx4,&m_ly4);
		break;
	case FM_R2U:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2U(ldx,ldy,&m_lx1,&m_ly1);
		R2U(ldx,ldy,&m_lx2,&m_ly2);
		R2U(ldx,ldy,&m_lx3,&m_ly3);
		R2U(ldx,ldy,&m_lx4,&m_ly4);
		break;
	case FM_F2Z:
		ldx=(left+right)/2;
		F2Z(ldx,&m_lx1);
		F2Z(ldx,&m_lx2);
		F2Z(ldx,&m_lx3);
		F2Z(ldx,&m_lx4);
		ldx=m_lx3;
		ldy=m_ly3;
		m_lx3=m_lx4;
		m_ly3=m_ly4;
		m_lx4=ldx;
		m_ly4=ldy;
		break;
	case FM_F2I:
		ldy=(top+bottom)/2;
		F2I(ldy,&m_ly1);
		F2I(ldy,&m_ly2);
		F2I(ldy,&m_ly3);
		F2I(ldy,&m_ly4);
		ldx=m_lx3;
		ldy=m_ly3;
		m_lx3=m_lx4;
		m_ly3=m_ly4;
		m_lx4=ldx;
		m_ly4=ldy;
		break;
	case FM_S2G:
		break;
	}
}

CElement * CArc::Clone(long lCmdId, long lStatus)
{
	CArc * pNew=new CArc();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_lx3=m_lx3;
	pNew->m_lx4=m_lx4;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	pNew->m_ly3=m_ly3;
	pNew->m_ly4=m_ly4;
	pNew->m_pen=m_pen;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CArc::Save(MSXML::IXMLDOMElementPtr pRoot)
{
	MSXML::IXMLDOMElementPtr pGO;
	MSXML::IXMLDOMElementPtr pStatic;
	MSXML::IXMLDOMElementPtr pDynamic;
	MSXML::IXMLDOMElementPtr pAttr;
    _variant_t val;

	MSXML::IXMLDOMDocumentPtr pDoc=pRoot->ownerDocument;
    
    //add a graphy object
    pGO=pDoc->createElement(L"go");    
    val.vt=VT_BSTR;
    val.bstrVal=::SysAllocString(L"arc");
    pGO->setAttribute(L"type",val);
    pRoot->appendChild(pGO);

    //add a group static attribution
    pStatic=pDoc->createElement(L"static");
    pGO->appendChild(pStatic);
    
    //position
    pAttr=pDoc->createElement(L"position");
    pStatic->appendChild(pAttr);
    //x1
    val.vt=VT_I4;
    val.lVal=m_lx1;
    pAttr->setAttribute(L"x1",val);
    //y1
    val.vt=VT_I4;
    val.lVal=m_ly1;
    pAttr->setAttribute(L"y1",val);
    //x2
    val.vt=VT_I4;
    val.lVal=m_lx2;
    pAttr->setAttribute(L"x2",val);
    //y2
    val.vt=VT_I4;
    val.lVal=m_ly2;
    pAttr->setAttribute(L"y2",val);
    //x3
    val.vt=VT_I4;
    val.lVal=m_lx3;
    pAttr->setAttribute(L"x3",val);
    //y3
    val.vt=VT_I4;
    val.lVal=m_ly3;
    pAttr->setAttribute(L"y3",val);
    //x4
    val.vt=VT_I4;
    val.lVal=m_lx4;
    pAttr->setAttribute(L"x4",val);
    //y4
    val.vt=VT_I4;
    val.lVal=m_ly4;
    pAttr->setAttribute(L"y4",val);

    //pen
    pAttr=pDoc->createElement("pen");
    pStatic->appendChild(pAttr);
    //style
    val.vt=VT_UI4;
    val.ulVal=m_pen.lopnStyle;
    pAttr->setAttribute(L"s",val);
    //width
    val.vt=VT_I4;
    val.lVal=m_pen.lopnWidth.x;
    pAttr->setAttribute(L"w",val);
    //color
    val.vt=VT_UI4;
    val.ulVal=m_pen.lopnColor;
    pAttr->setAttribute(L"c",val);

    //if linked then save dynamic attribution
    pDynamic=pDoc->createElement("dynamic");
    pGO->appendChild(pDynamic);
    if(m_DnyAttr.m_Linked){
        //linked
        val.vt=VT_BOOL;
        val.boolVal=-1;
        pDynamic->setAttribute(L"linked",val);
        //save dynamic attribution
        m_DnyAttr.Save(pDoc,pDynamic);
    }else{
        //unlinked
        val.vt=VT_BOOL;
        val.boolVal=FALSE;
        pDynamic->setAttribute(L"linked",val);
    }
}

void CArc::Load(MSXML::IXMLDOMElementPtr pGO)
{
    MSXML::IXMLDOMElementPtr pStatic;
    MSXML::IXMLDOMElementPtr pDynamic;
	MSXML::IXMLDOMElementPtr pAttr;
    _variant_t val;

    pStatic=pGO->selectSingleNode(L"./static");
    //position
    pAttr=pStatic->selectSingleNode(L"./position");
    //x1
    val=pAttr->getAttribute(L"x1");
    val.ChangeType(VT_I4);
    m_lx1=val.lVal;
	m_lox1=m_lx1;
    //y1
    val=pAttr->getAttribute(L"y1");
    val.ChangeType(VT_I4);
    m_ly1=val.lVal;
	m_loy1=m_ly1;
    //x2
    val=pAttr->getAttribute(L"x2");
    val.ChangeType(VT_I4);
    m_lx2=val.lVal;
	m_lox2=m_lx2;
    //y2
    val=pAttr->getAttribute(L"y2");
    val.ChangeType(VT_I4);
    m_ly2=val.lVal;
	m_loy2=m_ly2;
    //x3
    val=pAttr->getAttribute(L"x3");
    val.ChangeType(VT_I4);
    m_lx3=val.lVal;
	m_lox3=m_lx3;
    //y3
    val=pAttr->getAttribute(L"y3");
    val.ChangeType(VT_I4);
    m_ly3=val.lVal;
	m_loy3=m_ly3;
    //x4
    val=pAttr->getAttribute(L"x4");
    val.ChangeType(VT_I4);
    m_lx4=val.lVal;
	m_lox4=m_lx4;
    //y4
    val=pAttr->getAttribute(L"y4");
    val.ChangeType(VT_I4);
    m_ly4=val.lVal;
	m_loy4=m_ly4;

    //pen
    pAttr=pStatic->selectSingleNode(L"./pen");
    //style
    val=pAttr->getAttribute(L"s");
    val.ChangeType(VT_UI4);
    m_pen.lopnStyle=val.ulVal;
    //width
    val=pAttr->getAttribute(L"w");
    val.ChangeType(VT_I4);
    m_pen.lopnWidth.x=val.lVal;
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_pen.lopnColor=val.ulVal;
	m_open=m_pen;

    //load dynamic attribution if have
    pDynamic=pGO->selectSingleNode(L"./dynamic");
    val=pDynamic->getAttribute(L"linked");
    val.ChangeType(VT_BOOL);
    if(val.boolVal){
        m_DnyAttr.Load(pDynamic);
        m_DnyAttr.m_Linked=true;
    }else{
        m_DnyAttr.m_Linked=false;
    }
}

void CArc::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
	m_lx3+=potTracker.x;
	m_ly3+=potTracker.y;
	m_lx4+=potTracker.x;
	m_ly4+=potTracker.y;
}


void CArc::MoveElement(POINT potOffset)
{
	m_lx1=m_lox1+potOffset.x;
	m_lx2=m_lox2+potOffset.x;
	m_lx3=m_lox3+potOffset.x;
	m_lx4=m_lox4+potOffset.x;
	m_ly1=m_loy1+potOffset.y;
	m_ly2=m_loy2+potOffset.y;
	m_ly3=m_loy3+potOffset.y;
	m_ly4=m_loy4+potOffset.y;
}

void CArc::UpdateView()
{
	m_DnyAttr.m_na=false;

	float fp;
	long lp;

	//blink
	if(m_DnyAttr.m_Blink.m_Used){
		if(m_DnyAttr.m_Blink.m_TagValue.Flags & TF_Valid){
			bool bBlink=false;
			if(m_DnyAttr.m_Blink.m_VisibleAtOn && m_DnyAttr.m_Blink.m_TagValue.Value.iValue){
				bBlink=true;
			}else if(!m_DnyAttr.m_Blink.m_VisibleAtOn && !m_DnyAttr.m_Blink.m_TagValue.Value.iValue){
				bBlink=true;
			}else{
				bBlink=false;
			}
			if(m_DnyAttr.m_Blink.m_BlinkSpeed==0){
				if((glTimer%4)>1) bBlink=false;
			}else if(m_DnyAttr.m_Blink.m_BlinkSpeed==1){
				if((glTimer%8)>3) bBlink=false;
			}else{
				if((glTimer%16)>7) bBlink=false;
			}
			if(bBlink){
				m_pen.lopnColor=m_DnyAttr.m_Blink.m_ColorPen;
			}else{
				m_pen=m_open;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}

	//color brush a
	//color brush aa
	//color brush d
	//color brush da
	//color pen a
	if(m_DnyAttr.m_ColorPenA.m_Used){
		if(m_DnyAttr.m_ColorPenA.m_TagValue.Flags & TF_Valid){
			__r4 r4 = getFloat(m_DnyAttr.m_ColorPenA.m_TagValue);
			if(r4 > m_DnyAttr.m_ColorPenA.m_Split1){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color0;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split2){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color1;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split3){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color2;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split4){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color3;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color4;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen aa
	if(m_DnyAttr.m_ColorPenAA.m_Used){
		if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_HiHi){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorH2;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Hi){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorH1;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_LoLo){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorL2;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Lo){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorL1;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Rate){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorR;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorN;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen d
	if(m_DnyAttr.m_ColorPenD.m_Used){
		if(m_DnyAttr.m_ColorPenD.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorPenD.m_TagValue.Value.iValue){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenD.m_ColorOn;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenD.m_ColorOff;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen da
	if(m_DnyAttr.m_ColorPenDA.m_Used){
		if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_Off2On){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorAlarm;
			}else if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_On2Off){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorAlarm;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorNomal;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color text a
	//color text aa
	//color text d
	//color text da
	//direction

	//fill h
	//fill v
	long ltx1=m_lox1;
	long ltx2=m_lox2;
	long ltx3=m_lox3;
	long ltx4=m_lox4;
	long lty1=m_loy1;
	long lty2=m_loy2;
	long lty3=m_loy3;
	long lty4=m_loy4;
	//pos h
	if(m_DnyAttr.m_PosH.m_Used){
		if(m_DnyAttr.m_PosH.m_TagValue.Flags & TF_Valid){			
			lp=(getFloat(m_DnyAttr.m_PosH.m_TagValue)-m_DnyAttr.m_PosH.m_LTO)*\
				(m_DnyAttr.m_PosH.m_RBT-m_DnyAttr.m_PosH.m_LTT)/\
				(m_DnyAttr.m_PosH.m_RBO-m_DnyAttr.m_PosH.m_LTO);
			if(lp<m_DnyAttr.m_PosH.m_LTT) lp=m_DnyAttr.m_PosH.m_LTT;
			if(lp>m_DnyAttr.m_PosH.m_RBT) lp=m_DnyAttr.m_PosH.m_RBT;
			m_lx1=m_lox1+lp;
			m_lx2=m_lox2+lp;
			m_lx3=m_lox3+lp;
			m_lx4=m_lox4+lp;
			ltx1=m_lx1;
			ltx2=m_lx2;
			ltx3=m_lx3;
			ltx4=m_lx4;
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//pos v
	if(m_DnyAttr.m_PosV.m_Used){
		if(m_DnyAttr.m_PosV.m_TagValue.Flags & TF_Valid){			
			lp=(getFloat(m_DnyAttr.m_PosV.m_TagValue)-m_DnyAttr.m_PosV.m_LTO)*\
				(m_DnyAttr.m_PosV.m_RBT-m_DnyAttr.m_PosV.m_LTT)/\
				(m_DnyAttr.m_PosV.m_RBO-m_DnyAttr.m_PosV.m_LTO);
			if(lp<m_DnyAttr.m_PosV.m_LTT) lp=m_DnyAttr.m_PosV.m_LTT;
			if(lp>m_DnyAttr.m_PosV.m_RBT) lp=m_DnyAttr.m_PosV.m_RBT;
			m_ly1=m_loy1+lp;
			m_ly2=m_loy2+lp;
			m_ly3=m_loy3+lp;
			m_ly4=m_loy4+lp;
			lty1=m_ly1;
			lty2=m_ly2;
			lty3=m_ly3;
			lty4=m_ly4;
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//size h
	if(m_DnyAttr.m_SizeH.m_Used){
		if(m_DnyAttr.m_SizeH.m_TagValue.Flags & TF_Valid){
			fp=(getFloat(m_DnyAttr.m_SizeH.m_TagValue)-m_DnyAttr.m_SizeH.m_LTO)*\
				(m_DnyAttr.m_SizeH.m_RBT-m_DnyAttr.m_SizeH.m_LTT)/\
				(m_DnyAttr.m_SizeH.m_RBO-m_DnyAttr.m_SizeH.m_LTO)/100;
            fp += m_DnyAttr.m_SizeH.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>1.) fp=1.;
			if(m_DnyAttr.m_SizeH.m_Ref==0){
				m_lx1=ltx1;
				m_lx2=ltx1+(ltx2-ltx1)*fp;
				m_lx3=ltx1+(ltx3-ltx1)*fp;
				m_lx4=ltx1+(ltx4-ltx1)*fp;
			}else if(m_DnyAttr.m_SizeH.m_Ref==1){
				m_lx1=(ltx1+ltx2)/2+(ltx1-ltx1/2-ltx2/2)*fp;
				m_lx2=(ltx1+ltx2)/2+(ltx2-ltx1/2-ltx2/2)*fp;
				m_lx3=(ltx1+ltx2)/2+(ltx3-ltx1/2-ltx2/2)*fp;
				m_lx4=(ltx1+ltx2)/2+(ltx4-ltx1/2-ltx2/2)*fp;
			}else if(m_DnyAttr.m_SizeH.m_Ref==2){
				m_lx1=ltx2+(ltx1-ltx2)*fp;
				m_lx2=ltx2;
				m_lx3=ltx2+(ltx3-ltx2)*fp;
				m_lx4=ltx2+(ltx4-ltx2)*fp;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//size v
	if(m_DnyAttr.m_SizeV.m_Used){
		if(m_DnyAttr.m_SizeV.m_TagValue.Flags & TF_Valid){
			fp=(getFloat(m_DnyAttr.m_SizeV.m_TagValue)-m_DnyAttr.m_SizeV.m_LTO)*\
				(m_DnyAttr.m_SizeV.m_RBT-m_DnyAttr.m_SizeV.m_LTT)/\
				(m_DnyAttr.m_SizeV.m_RBO-m_DnyAttr.m_SizeV.m_LTO)/100;
            fp += m_DnyAttr.m_SizeV.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>100.) fp=100.;
			if(m_DnyAttr.m_SizeV.m_Ref==0){
				m_ly1=lty1;
				m_ly2=lty1+(lty2-lty1)*fp;
				m_ly3=lty1+(lty3-lty1)*fp;
				m_ly4=lty1+(lty4-lty1)*fp;
			}else if(m_DnyAttr.m_SizeV.m_Ref==1){
				m_ly1=(lty1+lty2)/2+(lty1-lty1/2-lty2/2)*fp;
				m_ly2=(lty1+lty2)/2+(lty2-lty1/2-lty2/2)*fp;
				m_ly3=(lty1+lty2)/2+(lty3-lty1/2-lty2/2)*fp;
				m_ly4=(lty1+lty2)/2+(lty4-lty1/2-lty2/2)*fp;
			}else if(m_DnyAttr.m_SizeV.m_Ref==2){
				m_ly1=lty2+(lty1-lty2)*fp;
				m_ly2=lty2;
				m_ly3=lty2+(lty3-lty2)*fp;
				m_ly4=lty2+(lty4-lty2)*fp;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//value a
	//value d
	//visible
	if(m_DnyAttr.m_Visible.m_Used){
		if(m_DnyAttr.m_Visible.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_Visible.m_Visble && m_DnyAttr.m_Visible.m_TagValue.Value.iValue){
				m_DnyAttr.m_vi=true;
			}else if(!m_DnyAttr.m_Visible.m_Visble && !m_DnyAttr.m_Visible.m_TagValue.Value.iValue){
				m_DnyAttr.m_vi=true;
			}else{
				m_DnyAttr.m_vi=false;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}

	if (m_DnyAttr.m_rank == 0)
	{
		m_DnyAttr.m_en=true;
	}
	else
	{
		if((m_DnyAttr.m_rank & glRank) == 0){
			m_DnyAttr.m_en=false;
		}else{
			m_DnyAttr.m_en=true;
		}
	}
	//enable
	if(m_DnyAttr.m_Enable.m_Used){
		if(m_DnyAttr.m_Enable.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_Enable.m_TagValue.Value.iValue && m_DnyAttr.m_Enable.m_Enable){
			}else if(!m_DnyAttr.m_Enable.m_TagValue.Value.iValue && !m_DnyAttr.m_Enable.m_Enable){
			}else{
				m_DnyAttr.m_en=false;
			}
		}else{
			m_DnyAttr.m_na=true;
			m_DnyAttr.m_en=false;
		}
	}
	//button a
	if(m_DnyAttr.m_ButtonA.m_Used){
		if(! (m_DnyAttr.m_ButtonA.m_TagValue.Flags & TF_Valid) ){
			m_DnyAttr.m_en=false;
		}
	}
	//button d
	if(m_DnyAttr.m_ButtonD.m_Used){
		if(! (m_DnyAttr.m_ButtonD.m_TagValue.Flags & TF_Valid) ){
			m_DnyAttr.m_en=false;
		}
	}
	//input a
	if(m_DnyAttr.m_InputA.m_Used){
		if(! (m_DnyAttr.m_InputA.m_TagValue.Flags & TF_Valid) ){
			m_DnyAttr.m_en=false;
		}
	}
	//input d
	if(m_DnyAttr.m_InputD.m_Used){
		if(! (m_DnyAttr.m_InputD.m_TagValue.Flags & TF_Valid)) {
			m_DnyAttr.m_en=false;
		}
	}
	//slide h
	if(m_DnyAttr.m_SlideH.m_Used){
		if(! (m_DnyAttr.m_SlideH.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//slide v
	if(m_DnyAttr.m_SlideV.m_Used){
		if(!(m_DnyAttr.m_SlideV.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
}
