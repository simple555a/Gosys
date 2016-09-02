// Ellipse.cpp: implementation of the CEllipse class.
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
IMPLEMENT_DYNAMIC(CEllipse,CElement)

CEllipse::CEllipse()
{
	m_bHavePen=true;
	m_bHaveBrush=true;
	m_bHaveFont=false;
}

CEllipse::~CEllipse()
{

}

void CEllipse::Draw(CDC *pDC)
{
	if(m_lStatus!=ST_NOMAL) return;

	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateBrushIndirect(&m_brush);
	CBrush *pOldBrush=pDC->SelectObject(&brush);
	pDC->SetBkMode(TRANSPARENT);

	long x1,y1,x2,y2;
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
	pDC->Ellipse(x1,y1,x2,y2);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CEllipse::DrawTracker(CDC *pDC)
{
	LOGPEN tLogPen;
	tLogPen.lopnColor=RGB(0,0,0);
	tLogPen.lopnStyle=PS_DASH;
	tLogPen.lopnWidth.x=1;
	CPen tPen;
	tPen.CreatePenIndirect(&tLogPen);
	CPen *pOldPen=(CPen *)pDC->SelectObject(&tPen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);
	long x1,y1,x2,y2;

	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	pDC->Rectangle(x1,y1,x2,y2);
	switch(m_lHitTarget){
	case TK_LT:
		m_lx1=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LB:
		m_lx1=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx1,m_ly2,&x1,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RT:
		m_lx2=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx2,m_ly1,&x2,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RB:
		m_lx2=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LM:
		m_lx1=m_potTracker.x;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_TM:
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RM:
		m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_BM:
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_MOV:
		m_lx1+=m_potTracker.x;
		m_ly1+=m_potTracker.y;
		m_lx2+=m_potTracker.x;
		m_ly2+=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CEllipse::DrawHotPoints(CDC *pDC)
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

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);
}

long CEllipse::HitTest(long x, long y)
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

	CRgn rgnTmp;
	if(rgnTmp.CreateEllipticRgn(m_lx1,m_ly1,m_lx2,m_ly2)){
		if(rgnTmp.PtInRegion(pt)) return TK_MOV;
	}

	return TK_NONE;
}

bool CEllipse::RInR(LPCRECT lpRect)
{
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;

    /*
	CRgn rgnTmp;
	if(!rgnTmp.CreateEllipticRgn(m_lx1,m_ly1,m_lx2,m_ly2))
        return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
    */

    // 原来的相交选取改为现在的包含选取
	
    POINT pt1, pt2, pt3, pt4;
    pt1.x = m_lx1;
    pt1.y = m_ly1;
    pt2.x = m_lx1;
    pt2.y = m_ly2;
    pt3.x = m_lx2;
    pt3.y = m_ly1;
    pt4.x = m_lx2;
    pt4.y = m_ly2;

    CRect rectTmp(lpRect->left-1,lpRect->top-1,lpRect->right+1,lpRect->bottom+1);
    rectTmp.NormalizeRect();
    
    if( rectTmp.PtInRect(pt1) && rectTmp.PtInRect(pt2) && rectTmp.PtInRect(pt3) && rectTmp.PtInRect(pt4) ){
        return true;
    }else{
        return false;
    }

}

long CEllipse::Left()
{
	return m_lx1;
}

long CEllipse::Right()
{
	return m_lx2;
}

long CEllipse::Top()
{
	return m_ly1;
}

long CEllipse::Bottom()
{
	return m_ly2;
}

void CEllipse::Format(long left, long top, long right, long bottom, long flag)
{
	long ldx,ldy;
	switch(flag){
	case FM_A2L:
		ldx=left-Left();
		m_lx1+=ldx;
		m_lx2+=ldx;
		break;
	case FM_A2C:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;	
		break;
	case FM_A2R:
		ldx=right-Right();
		m_lx1+=ldx;
		m_lx2+=ldx;
		break;
	case FM_A2T:
		ldy=top-Top();
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2M:
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2B:
		ldy=bottom-Bottom();
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2CM:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_R2W:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2W(ldx,ldy,&m_lx1,&m_ly1);
		R2W(ldx,ldy,&m_lx2,&m_ly2);
		break;
	case FM_R2U:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2U(ldx,ldy,&m_lx1,&m_ly1);
		R2U(ldx,ldy,&m_lx2,&m_ly2);
		break;
	case FM_F2Z:
		ldx=(left+right)/2;
		F2Z(ldx,&m_lx1);
		F2Z(ldx,&m_lx2);
		break;
	case FM_F2I:
		ldy=(top+bottom)/2;
		F2I(ldy,&m_ly1);
		F2I(ldy,&m_ly2);
		break;
	case FM_S2G:
		m_lx1=(m_lx1+right/2)-(m_lx1+right/2)%right;
		m_lx2=(m_lx2+right/2)-(m_lx2+right/2)%right;
		m_ly1=(m_ly1+bottom/2)-(m_ly1+bottom/2)%bottom;
		m_ly2=(m_ly2+bottom/2)-(m_ly2+bottom/2)%bottom;
		break;
	}
}

CElement * CEllipse::Clone(long lCmdId, long lStatus)
{
	CEllipse * pNew=new CEllipse();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	pNew->m_pen=m_pen;
	pNew->m_brush=m_brush;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CEllipse::Save(MSXML::IXMLDOMElementPtr pRoot)
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
    val.bstrVal=::SysAllocString(L"ellipse");
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

    //brush
    pAttr=pDoc->createElement("brush");
    pStatic->appendChild(pAttr);
    //style
    val.vt=VT_UI4;
    val.ulVal=m_brush.lbStyle;
    pAttr->setAttribute(L"s",val);
    //color
    val.vt=VT_UI4;
    val.ulVal=m_brush.lbColor;
    pAttr->setAttribute(L"c",val);
    //hatch
    val.vt=VT_I4;
    val.lVal=m_brush.lbStyle;
    pAttr->setAttribute(L"w",val);

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

void CEllipse::Load(MSXML::IXMLDOMElementPtr pGO)
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

    //brush
    pAttr=pStatic->selectSingleNode(L"./brush");
    //style
    val=pAttr->getAttribute(L"s");
    val.ChangeType(VT_UI4);
    m_brush.lbStyle=val.ulVal;
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_brush.lbColor=val.ulVal;
    //hatch
    val=pAttr->getAttribute(L"w");
    val.ChangeType(VT_I4);
    m_brush.lbHatch=val.lVal;

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

void CEllipse::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
}
