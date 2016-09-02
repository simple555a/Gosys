// Arc.cpp: implementation of the CArc class.
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
	if(m_lStatus!=ST_NOMAL) return;

	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);

	long x1,y1,x2,y2,x3,y3,x4,y4;
	x1=m_lx1;
	y1=m_ly1;
	if(m_lx1>m_lx2){
		m_lx1=m_lx2;
		m_lx2=x1;
	}
	if(m_ly1>m_ly2){
		m_ly1=m_ly2;
		m_ly2=y1;
	}
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	if(m_lx3==0 && m_ly3==0 && m_lx4==0 && m_ly4==0){
		m_lx4=(m_lx2+m_lx1)/2;
		m_ly4=m_ly2+DELTA*2;
		m_lx3=m_lx2+DELTA*2;
		m_ly3=(m_ly1+m_ly2)/2;
	}
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
		UPtoLP(m_lx1,m_lx1,&x1,0);
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
	if(m_locked){
			return TK_NONE;
	}
	if(m_lStatus!=ST_NOMAL) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	rtTemp.SetRect(m_lx1-DELTA,m_ly1-DELTA,m_lx1+DELTA,m_ly1+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_LT;

	rtTemp.SetRect(m_lx1-DELTA,m_ly2-DELTA,m_lx1+DELTA,m_ly2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_LB;
	
	rtTemp.SetRect(m_lx2-DELTA,m_ly1-DELTA,m_lx2+DELTA,m_ly1+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_RT;

	rtTemp.SetRect(m_lx2-DELTA,m_ly2-DELTA,m_lx2+DELTA,m_ly2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_RB;

	rtTemp.SetRect((m_lx1+m_lx2)/2-DELTA,m_ly1-DELTA,(m_lx1+m_lx2)/2+DELTA,m_ly1+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_TM;

	rtTemp.SetRect((m_lx1+m_lx2)/2-DELTA,m_ly2-DELTA,(m_lx1+m_lx2)/2+DELTA,m_ly2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_BM;

	rtTemp.SetRect(m_lx1-DELTA,(m_ly1+m_ly2)/2-DELTA,m_lx1+DELTA,(m_ly1+m_ly2)/2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_LM;

	rtTemp.SetRect(m_lx2-DELTA,(m_ly1+m_ly2)/2-DELTA,m_lx2+DELTA,(m_ly1+m_ly2)/2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_RM;

	rtTemp.SetRect(m_lx3-DELTA,m_ly3-DELTA,m_lx3+DELTA,m_ly3+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_SP;
	
	rtTemp.SetRect(m_lx4-DELTA,m_ly4-DELTA,m_lx4+DELTA,m_ly4+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_EP;
	
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

	    if(SPE(sa,pa,ea)) return TK_MOV;
    }
	return TK_NONE;
}


bool CArc::RInR(LPCRECT lpRect)
{
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;
    /*
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
    */
    // 原来的相交选取改为现在的包含选取，对于圆弧曲线，
    // 如果包含曲线两头的两个点，选取

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

	CRect rectTmp(lpRect->left-1,lpRect->top-1,lpRect->right+1,lpRect->bottom+1);
    rectTmp.NormalizeRect();
	POINT pt1, pt2;
	pt1.x=ca+ra*cos(sa);
	pt1.y=cb-rb*sin(sa);
	pt2.x=ca+ra*cos(ea);
	pt2.y=cb-rb*sin(ea);
    if(rectTmp.PtInRect(pt1) && rectTmp.PtInRect(pt2)){
        return true;
    }else{
        return false;
    }
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
		m_lx1=(m_lx1+right/2)-(m_lx1+right/2)%right;
		m_lx2=(m_lx2+right/2)-(m_lx2+right/2)%right;
		m_lx3=(m_lx3+right/2)-(m_lx3+right/2)%right;
		m_lx4=(m_lx4+right/2)-(m_lx4+right/2)%right;
		m_ly1=(m_ly1+bottom/2)-(m_ly1+bottom/2)%bottom;
		m_ly2=(m_ly2+bottom/2)-(m_ly2+bottom/2)%bottom;
		m_ly3=(m_ly3+bottom/2)-(m_ly3+bottom/2)%bottom;
		m_ly4=(m_ly4+bottom/2)-(m_ly4+bottom/2)%bottom;
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
    val.lVal=m_ly1;
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
    //y1
    val=pAttr->getAttribute(L"y1");
    val.ChangeType(VT_I4);
    m_ly1=val.lVal;
    //x2
    val=pAttr->getAttribute(L"x2");
    val.ChangeType(VT_I4);
    m_lx2=val.lVal;
    //y2
    val=pAttr->getAttribute(L"y2");
    val.ChangeType(VT_I4);
    m_ly2=val.lVal;
    //x3
    val=pAttr->getAttribute(L"x3");
    val.ChangeType(VT_I4);
    m_lx3=val.lVal;
    //y3
    val=pAttr->getAttribute(L"y3");
    val.ChangeType(VT_I4);
    m_ly3=val.lVal;
    //x4
    val=pAttr->getAttribute(L"x4");
    val.ChangeType(VT_I4);
    m_lx4=val.lVal;
    //y4
    val=pAttr->getAttribute(L"y4");
    val.ChangeType(VT_I4);
    m_ly4=val.lVal;

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

