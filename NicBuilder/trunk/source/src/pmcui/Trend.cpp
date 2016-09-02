// Trend.cpp: implementation of the CTrend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4800)
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTrend,CElement)

CTrend::CTrend()
{
	m_bHavePen=false;
	m_bHaveBrush=false;
	m_bHaveFont=false;

	for(int i=0;i<10;i++){
		m_pens[i].lopnStyle=PS_SOLID;
		m_pens[i].lopnWidth.x=0;
		m_bAnalog[i]=true;
		m_fMin[i]=0.;
		m_fMax[i]=100.;
	}
	m_pens[0].lopnColor=RGB(255,0,0);
	m_pens[1].lopnColor=RGB(0,255,0);
	m_pens[2].lopnColor=RGB(0,0,255);
	m_pens[3].lopnColor=RGB(128,0,128);
	m_pens[4].lopnColor=RGB(128,128,0);
	//m_pens[5].lopnColor=RGB(0,128,128);
	//m_pens[6].lopnColor=RGB(128,0,0);
	//m_pens[7].lopnColor=RGB(0,128,0);
	//m_pens[8].lopnColor=RGB(0,0,128);
	//m_pens[9].lopnColor=RGB(255,128,128);
	m_BkColor=RGB(255,255,255);
	m_GdColor=RGB(192,192,192);
	m_lMode=1;
}

CTrend::~CTrend()
{

}

long CTrend::Left()
{
	return m_lx1;
}

long CTrend::Right()
{
	return m_lx2;
}

long CTrend::Top()
{
	return m_ly1;
}

long CTrend::Bottom()
{
	return m_ly2;
}

void CTrend::DrawHotPoints(CDC *pDC)
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

inline void DrawTag(CString str, CRect rt, UINT nFormat, CDC *pDC,COLORREF clr=RGB(0,0,0))
{
	LOGFONT lft;
	lft.lfCharSet=DEFAULT_CHARSET;
	lft.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lft.lfEscapement=0;	
	strcpy(lft.lfFaceName,"宋体");
	long ih;
	CElement::UPtoLP(100,100,0,&ih);
	lft.lfHeight=ih;
	lft.lfItalic=false;
	lft.lfOrientation=0;
	lft.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lft.lfPitchAndFamily=DEFAULT_PITCH;
	lft.lfQuality=DEFAULT_QUALITY;
	lft.lfStrikeOut=false;
	lft.lfUnderline=false;
	lft.lfWeight=FW_NORMAL;
	lft.lfWidth=0;

	CFont ft;
	ft.CreateFontIndirect(&lft);
	CFont *pOldFont=pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(clr); 


	if(nFormat & DT_BOTTOM){
		rt.top=rt.bottom-ih*2;
	}
	pDC->DrawText(str,rt,nFormat);
	pDC->SelectObject(pOldFont);
}

void CTrend::drawH(CDC *pDC)
{
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

	//DrawTag("yyyy年mm月dd日\nhh:nn:ss",CRect(x1,y1,x2,y2),DT_LEFT,pDC);
	long lyTag,lxTag1,lxTag2;
	UPtoLP(m_ly2-200,m_ly2-200,0,&lyTag);
	UPtoLP(m_lx1+200,m_lx1+200,&lxTag1,0);
	UPtoLP(m_lx2-200,m_lx2-200,&lxTag2,0);
	CPen *pOldPen=(CPen*)pDC->SelectStockObject(BLACK_PEN);
	CBrush brushBk(m_BkColor);
	CBrush *pOldBrush=(CBrush*)pDC->SelectObject(&brushBk);
	pDC->Rectangle(x1,y1,x2,lyTag);
	CPen penGrid(PS_SOLID,0,m_GdColor);
	pDC->SelectObject(&penGrid);
	for(long i=1;i<10;i++){
		pDC->MoveTo(x1,y1+i*(lyTag-y1)/10);
		pDC->LineTo(x2,y1+i*(lyTag-y1)/10);
		pDC->MoveTo(x1+i*(x2-x1)/10,y1);
		pDC->LineTo(x1+i*(x2-x1)/10,lyTag);
	}

	DrawTag("yyyy-mm-dd\n hh:nn:ss",CRect(lxTag1,lyTag,lxTag2,y2),DT_LEFT,pDC);
	DrawTag("yyyy-mm-dd\nhh:nn:ss ",CRect(lxTag1,lyTag,lxTag2,y2),DT_RIGHT,pDC);

	pDC->SelectStockObject(NULL_PEN);
	CBrush brushBlue(RGB(0,0,255));
	pDC->SelectObject(&brushBlue);
	CPoint pts[3];
	long lGap;
	UPtoLP(38,38,&lGap,0);
	pts[0].x = x1+lGap;
	pts[0].y = (lyTag+y2)/2;
	pts[1].x = lxTag1-lGap;
	pts[1].y = lyTag+lGap;
	pts[2].x = lxTag1-lGap;
	pts[2].y = y2-lGap;
	pDC->Polygon(pts,3);
	pts[0].x = lxTag2+lGap;
	pts[0].y = lyTag+lGap;
	pts[1].x = lxTag2+lGap;
	pts[1].y = y2-lGap;
	pts[2].x = x2-lGap;
	pts[2].y = (lyTag+y2)/2;
	pDC->Polygon(pts,3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CTrend::drawV(CDC *pDC)
{
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

	long lxTag,lyTag1,lyTag2;
	UPtoLP(m_lx1+500,m_ly1,&lxTag,0);
	UPtoLP(m_lx1,m_ly1+200,0,&lyTag1);
	UPtoLP(m_lx1,m_ly2-200,0,&lyTag2);
	CPen *pOldPen=(CPen*)pDC->SelectStockObject(BLACK_PEN);
	CBrush brushBk(m_BkColor);
	CBrush *pOldBrush=(CBrush*)pDC->SelectObject(&brushBk);
	pDC->Rectangle(lxTag,y1,x2,y2);
	CPen penGrid(PS_SOLID,0,m_GdColor);
	pDC->SelectObject(&penGrid);
	for(long i=1;i<10;i++){
		pDC->MoveTo(lxTag,y1+i*(y2-y1)/10);
		pDC->LineTo(x2,y1+i*(y2-y1)/10);
		pDC->MoveTo(lxTag+i*(x2-lxTag)/10,y1);
		pDC->LineTo(lxTag+i*(x2-lxTag)/10,y2);
	}
	DrawTag("yyyy-mm-dd\n hh:nn:ss",CRect(x1,lyTag1,lxTag,(y1+y2)/2),DT_LEFT,pDC);
	DrawTag("yyyy-mm-dd\n hh:nn:ss",CRect(x1,(y1+y2)/2,lxTag,lyTag2),DT_LEFT|DT_BOTTOM,pDC);

	pDC->SelectStockObject(NULL_PEN);
	CBrush brushBlue(RGB(0,0,255));
	pDC->SelectObject(&brushBlue);
	CPoint pts[3];
	long lGap;
	UPtoLP(38,38,&lGap,0);
	pts[0].x = (x1+lxTag)/2;
	pts[0].y = y1+lGap;
	pts[1].x = x1+lGap;
	pts[1].y = lyTag1-lGap;
	pts[2].x = lxTag-lGap;
	pts[2].y = lyTag1-lGap;
	pDC->Polygon(pts,3);
	pts[0].x = (x1+lxTag)/2;
	pts[0].y = y2-lGap;
	pts[1].x = x1+lGap;
	pts[1].y = lyTag2+lGap;
	pts[2].x = lxTag-lGap;
	pts[2].y = lyTag2+lGap;
	pDC->Polygon(pts,3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CTrend::Draw(CDC *pDC)
{
	if(m_lStatus!=ST_NOMAL) return;
	
	//if(m_direction==0){
		drawH(pDC);
	//}else{
	//	drawV(pDC);
	//}
}

void CTrend::DrawTracker(CDC *pDC)
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
		if(m_lx2-m_potTracker.x>=clTrendMinWidth)
			m_lx1=m_potTracker.x;
		if(m_ly2-m_potTracker.y>=clTrendMinHeight)
			m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LB:
		if(m_lx2-m_potTracker.x>=clTrendMinWidth)
			m_lx1=m_potTracker.x;
		if(m_potTracker.y-m_ly1>=clTrendMinHeight)
			m_ly2=m_potTracker.y;
		UPtoLP(m_lx1,m_ly2,&x1,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RT:
		if(m_potTracker.x-m_lx1>=clTrendMinWidth)
			m_lx2=m_potTracker.x;
		if(m_ly2-m_potTracker.y>=clTrendMinHeight)
			m_ly1=m_potTracker.y;
		UPtoLP(m_lx2,m_ly1,&x2,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RB:
		if(m_potTracker.x-m_lx1>=clTrendMinWidth)
			m_lx2=m_potTracker.x;
		if(m_potTracker.y-m_ly1>=clTrendMinHeight)
			m_ly2=m_potTracker.y;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LM:
		if(m_lx2-m_potTracker.x>=clTrendMinWidth)
			m_lx1=m_potTracker.x;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_TM:
		if(m_ly2-m_potTracker.y>=clTrendMinHeight)
			m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RM:
		if(m_potTracker.x-m_lx1>=clTrendMinWidth)
			m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_BM:
		if(m_potTracker.y-m_ly1>=clTrendMinHeight)
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

void CTrend::Format(long left, long top, long right, long bottom, long flag)
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

long CTrend::HitTest(long x, long y)
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

	rtTemp.SetRect(m_lx1-DELTA,m_ly1-DELTA,m_lx2+DELTA,m_ly2+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_MOV;

	return TK_NONE;
}

void CTrend::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
}

bool CTrend::RInR(LPCRECT lpRect)
{
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;

    /*
    CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(m_lx1,m_ly1,m_lx2,m_ly2)) return false;

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

CElement * CTrend::Clone(long lCmdId, long lStatus)
{
	CTrend * pNew=new CTrend();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	pNew->m_lMode=m_lMode;
	pNew->m_BkColor=m_BkColor;
	pNew->m_GdColor=m_GdColor;
	for(int i=0;i<10;i++){
		pNew->m_tagnames[i]=m_tagnames[i];
		pNew->m_pens[i]=m_pens[i];
		pNew->m_bAnalog[i]=m_bAnalog[i];
		pNew->m_fMin[i]=m_fMin[i];
		pNew->m_fMax[i]=m_fMax[i];
	}

	return (CElement*) pNew;
}

void CTrend::Load(MSXML::IXMLDOMElementPtr pGO)
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
	//direction
	//val=pAttr->getAttribute(L"direction");
	//val.ChangeType(VT_I4);
	//m_direction=val.lVal;

	//face
    pAttr=pStatic->selectSingleNode(L"./face");
	//mode
	val=pAttr->getAttribute(L"m");
	val.ChangeType(VT_I4);
	m_lMode=val.lVal;
	//back color
	val=pAttr->getAttribute(L"bc");
	val.ChangeType(VT_UI4);
	m_BkColor=val.ulVal;
	//grid color
	val=pAttr->getAttribute(L"gc");
	val.ChangeType(VT_UI4);
	m_GdColor=val.ulVal;

	//curve1
	pAttr=pStatic->selectSingleNode(L"./curve1");
	//tagname
	val=pAttr->getAttribute(L"tag");
	m_tagnames[0]=val.bstrVal;
	//color
	val=pAttr->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_pens[0].lopnColor=val.ulVal;
	//analog
	val=pAttr->getAttribute(L"a");
	val.ChangeType(VT_BOOL);
	m_bAnalog[0]=val.boolVal;
	//min
	val=pAttr->getAttribute(L"nv");
	val.ChangeType(VT_R4);
	m_fMin[0]=val.fltVal;
	//max
	val=pAttr->getAttribute(L"xv");
	val.ChangeType(VT_R4);
	m_fMax[0]=val.fltVal;

	//curve2
	pAttr=pStatic->selectSingleNode(L"./curve2");
	//tagname
	val=pAttr->getAttribute(L"tag");
	m_tagnames[1]=val.bstrVal;
	//color
	val=pAttr->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_pens[1].lopnColor=val.ulVal;
	//analog
	val=pAttr->getAttribute(L"a");
	val.ChangeType(VT_BOOL);
	m_bAnalog[1]=val.boolVal;
	//min
	val=pAttr->getAttribute(L"nv");
	val.ChangeType(VT_R4);
	m_fMin[1]=val.fltVal;
	//max
	val=pAttr->getAttribute(L"xv");
	val.ChangeType(VT_R4);
	m_fMax[1]=val.fltVal;

	//curve3
	pAttr=pStatic->selectSingleNode(L"./curve3");
	//tagname
	val=pAttr->getAttribute(L"tag");
	m_tagnames[2]=val.bstrVal;
	//color
	val=pAttr->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_pens[2].lopnColor=val.ulVal;
	//analog
	val=pAttr->getAttribute(L"a");
	val.ChangeType(VT_BOOL);
	m_bAnalog[2]=val.boolVal;
	//min
	val=pAttr->getAttribute(L"nv");
	val.ChangeType(VT_R4);
	m_fMin[2]=val.fltVal;
	//max
	val=pAttr->getAttribute(L"xv");
	val.ChangeType(VT_R4);
	m_fMax[2]=val.fltVal;

	//curve4
	pAttr=pStatic->selectSingleNode(L"./curve4");
	//tagname
	val=pAttr->getAttribute(L"tag");
	m_tagnames[3]=val.bstrVal;
	//color
	val=pAttr->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_pens[3].lopnColor=val.ulVal;
	//analog
	val=pAttr->getAttribute(L"a");
	val.ChangeType(VT_BOOL);
	m_bAnalog[3]=val.boolVal;
	//min
	val=pAttr->getAttribute(L"nv");
	val.ChangeType(VT_R4);
	m_fMin[3]=val.fltVal;
	//max
	val=pAttr->getAttribute(L"xv");
	val.ChangeType(VT_R4);
	m_fMax[3]=val.fltVal;

	//curve5
	pAttr=pStatic->selectSingleNode(L"./curve5");
	//tagname
	val=pAttr->getAttribute(L"tag");
	m_tagnames[4]=val.bstrVal;
	//color
	val=pAttr->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_pens[4].lopnColor=val.ulVal;
	//analog
	val=pAttr->getAttribute(L"a");
	val.ChangeType(VT_BOOL);
	m_bAnalog[4]=val.boolVal;
	//min
	val=pAttr->getAttribute(L"nv");
	val.ChangeType(VT_R4);
	m_fMin[4]=val.fltVal;
	//max
	val=pAttr->getAttribute(L"xv");
	val.ChangeType(VT_R4);
	m_fMax[4]=val.fltVal;

	////curve6
	//pAttr=pStatic->selectSingleNode(L"./curve6");
	////tagname
	//val=pAttr->getAttribute(L"tag");
	//m_tagnames[5]=val.bstrVal;
	////color
	//val=pAttr->getAttribute(L"c");
	//val.ChangeType(VT_UI4);
	//m_pens[5].lopnColor=val.ulVal;
	////analog
	//val=pAttr->getAttribute(L"a");
	//val.ChangeType(VT_BOOL);
	//m_bAnalog[5]=val.boolVal;
	////min
	//val=pAttr->getAttribute(L"nv");
	//val.ChangeType(VT_R4);
	//m_fMin[5]=val.fltVal;
	////max
	//val=pAttr->getAttribute(L"xv");
	//val.ChangeType(VT_R4);
	//m_fMax[5]=val.fltVal;

	////curve7
	//pAttr=pStatic->selectSingleNode(L"./curve7");
	////tagname
	//val=pAttr->getAttribute(L"tag");
	//m_tagnames[6]=val.bstrVal;
	////color
	//val=pAttr->getAttribute(L"c");
	//val.ChangeType(VT_UI4);
	//m_pens[6].lopnColor=val.ulVal;
	////analog
	//val=pAttr->getAttribute(L"a");
	//val.ChangeType(VT_BOOL);
	//m_bAnalog[6]=val.boolVal;
	////min
	//val=pAttr->getAttribute(L"nv");
	//val.ChangeType(VT_R4);
	//m_fMin[6]=val.fltVal;
	////max
	//val=pAttr->getAttribute(L"xv");
	//val.ChangeType(VT_R4);
	//m_fMax[6]=val.fltVal;

	////curve8
	//pAttr=pStatic->selectSingleNode(L"./curve8");
	////tagname
	//val=pAttr->getAttribute(L"tag");
	//m_tagnames[7]=val.bstrVal;
	////color
	//val=pAttr->getAttribute(L"c");
	//val.ChangeType(VT_UI4);
	//m_pens[7].lopnColor=val.ulVal;
	////analog
	//val=pAttr->getAttribute(L"a");
	//val.ChangeType(VT_BOOL);
	//m_bAnalog[7]=val.boolVal;
	////min
	//val=pAttr->getAttribute(L"nv");
	//val.ChangeType(VT_R4);
	//m_fMin[7]=val.fltVal;
	////max
	//val=pAttr->getAttribute(L"xv");
	//val.ChangeType(VT_R4);
	//m_fMax[7]=val.fltVal;

	////curve9
	//pAttr=pStatic->selectSingleNode(L"./curve9");
	////tagname
	//val=pAttr->getAttribute(L"tag");
	//m_tagnames[8]=val.bstrVal;
	////color
	//val=pAttr->getAttribute(L"c");
	//val.ChangeType(VT_UI4);
	//m_pens[8].lopnColor=val.ulVal;
	////analog
	//val=pAttr->getAttribute(L"a");
	//val.ChangeType(VT_BOOL);
	//m_bAnalog[8]=val.boolVal;
	////min
	//val=pAttr->getAttribute(L"nv");
	//val.ChangeType(VT_R4);
	//m_fMin[8]=val.fltVal;
	////max
	//val=pAttr->getAttribute(L"xv");
	//val.ChangeType(VT_R4);
	//m_fMax[8]=val.fltVal;

	////curve10
	//pAttr=pStatic->selectSingleNode(L"./curve10");
	////tagname
	//val=pAttr->getAttribute(L"tag");
	//m_tagnames[9]=val.bstrVal;
	////color
	//val=pAttr->getAttribute(L"c");
	//val.ChangeType(VT_UI4);
	//m_pens[9].lopnColor=val.ulVal;
	////analog
	//val=pAttr->getAttribute(L"a");
	//val.ChangeType(VT_BOOL);
	//m_bAnalog[9]=val.boolVal;
	////min
	//val=pAttr->getAttribute(L"nv");
	//val.ChangeType(VT_R4);
	//m_fMin[9]=val.fltVal;
	////max
	//val=pAttr->getAttribute(L"xv");
	//val.ChangeType(VT_R4);
	//m_fMax[9]=val.fltVal;
}

void CTrend::Save(MSXML::IXMLDOMElementPtr pRoot)
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
    val.bstrVal=::SysAllocString(L"trend");
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

	//direction
	//val.vt=VT_I4;
	//val.lVal=m_direction;
	//pAttr->setAttribute(L"direction",val);

	//face
	pAttr=pDoc->createElement(L"face");
	pStatic->appendChild(pAttr);
	//mode
	val.vt=VT_I4;
	val.lVal=m_lMode;
	pAttr->setAttribute(L"m",val);
	//back color
	val.vt=VT_UI4;
	val.ulVal=m_BkColor;
	pAttr->setAttribute(L"bc",val);
	//grid color
	val.vt=VT_UI4;
	val.ulVal=m_GdColor;
	pAttr->setAttribute(L"gc",val);

	//curve1
	pAttr=pDoc->createElement(L"curve1");
	pStatic->appendChild(pAttr);
	//tagname
	val.vt=VT_BSTR;
	val.bstrVal=m_tagnames[0].AllocSysString();
	pAttr->setAttribute(L"tag",val);
	//color
	val.vt=VT_UI4;
	val.ulVal=m_pens[0].lopnColor;
	pAttr->setAttribute(L"c",val);
	//analog
	val.vt=VT_BOOL;
	val.boolVal=m_bAnalog[0];
	pAttr->setAttribute(L"a",val);
	//min
	val.vt=VT_R4;
	val.fltVal=m_fMin[0];
	pAttr->setAttribute(L"nv",val);
	//max
	val.vt=VT_R4;
	val.fltVal=m_fMax[0];
	pAttr->setAttribute(L"xv",val);

	//curve2
	pAttr=pDoc->createElement(L"curve2");
	pStatic->appendChild(pAttr);
	//tagname
	val.vt=VT_BSTR;
	val.bstrVal=m_tagnames[1].AllocSysString();
	pAttr->setAttribute(L"tag",val);
	//color
	val.vt=VT_UI4;
	val.ulVal=m_pens[1].lopnColor;
	pAttr->setAttribute(L"c",val);
	//analog
	val.vt=VT_BOOL;
	val.boolVal=m_bAnalog[1];
	pAttr->setAttribute(L"a",val);
	//min
	val.vt=VT_R4;
	val.fltVal=m_fMin[1];
	pAttr->setAttribute(L"nv",val);
	//max
	val.vt=VT_R4;
	val.fltVal=m_fMax[1];
	pAttr->setAttribute(L"xv",val);

	//curve3
	pAttr=pDoc->createElement(L"curve3");
	pStatic->appendChild(pAttr);
	//tagname
	val.vt=VT_BSTR;
	val.bstrVal=m_tagnames[2].AllocSysString();
	pAttr->setAttribute(L"tag",val);
	//color
	val.vt=VT_UI4;
	val.ulVal=m_pens[2].lopnColor;
	pAttr->setAttribute(L"c",val);
	//analog
	val.vt=VT_BOOL;
	val.boolVal=m_bAnalog[2];
	pAttr->setAttribute(L"a",val);
	//min
	val.vt=VT_R4;
	val.fltVal=m_fMin[2];
	pAttr->setAttribute(L"nv",val);
	//max
	val.vt=VT_R4;
	val.fltVal=m_fMax[2];
	pAttr->setAttribute(L"xv",val);

	//curve4
	pAttr=pDoc->createElement(L"curve4");
	pStatic->appendChild(pAttr);
	//tagname
	val.vt=VT_BSTR;
	val.bstrVal=m_tagnames[3].AllocSysString();
	pAttr->setAttribute(L"tag",val);
	//color
	val.vt=VT_UI4;
	val.ulVal=m_pens[3].lopnColor;
	pAttr->setAttribute(L"c",val);
	//analog
	val.vt=VT_BOOL;
	val.boolVal=m_bAnalog[3];
	pAttr->setAttribute(L"a",val);
	//min
	val.vt=VT_R4;
	val.fltVal=m_fMin[3];
	pAttr->setAttribute(L"nv",val);
	//max
	val.vt=VT_R4;
	val.fltVal=m_fMax[3];
	pAttr->setAttribute(L"xv",val);

	//curve5
	pAttr=pDoc->createElement(L"curve5");
	pStatic->appendChild(pAttr);
	//tagname
	val.vt=VT_BSTR;
	val.bstrVal=m_tagnames[4].AllocSysString();
	pAttr->setAttribute(L"tag",val);
	//color
	val.vt=VT_UI4;
	val.ulVal=m_pens[4].lopnColor;
	pAttr->setAttribute(L"c",val);
	//analog
	val.vt=VT_BOOL;
	val.boolVal=m_bAnalog[4];
	pAttr->setAttribute(L"a",val);
	//min
	val.vt=VT_R4;
	val.fltVal=m_fMin[4];
	pAttr->setAttribute(L"nv",val);
	//max
	val.vt=VT_R4;
	val.fltVal=m_fMax[4];
	pAttr->setAttribute(L"xv",val);

	////curve6
	//pAttr=pDoc->createElement(L"curve6");
	//pStatic->appendChild(pAttr);
	////tagname
	//val.vt=VT_BSTR;
	//val.bstrVal=m_tagnames[5].AllocSysString();
	//pAttr->setAttribute(L"tag",val);
	////color
	//val.vt=VT_UI4;
	//val.ulVal=m_pens[5].lopnColor;
	//pAttr->setAttribute(L"c",val);
	////analog
	//val.vt=VT_BOOL;
	//val.boolVal=m_bAnalog[5];
	//pAttr->setAttribute(L"a",val);
	////min
	//val.vt=VT_R4;
	//val.fltVal=m_fMin[5];
	//pAttr->setAttribute(L"nv",val);
	////max
	//val.vt=VT_R4;
	//val.fltVal=m_fMax[5];
	//pAttr->setAttribute(L"xv",val);

	////curve7
	//pAttr=pDoc->createElement(L"curve7");
	//pStatic->appendChild(pAttr);
	////tagname
	//val.vt=VT_BSTR;
	//val.bstrVal=m_tagnames[6].AllocSysString();
	//pAttr->setAttribute(L"tag",val);
	////color
	//val.vt=VT_UI4;
	//val.ulVal=m_pens[6].lopnColor;
	//pAttr->setAttribute(L"c",val);
	////analog
	//val.vt=VT_BOOL;
	//val.boolVal=m_bAnalog[6];
	//pAttr->setAttribute(L"a",val);
	////min
	//val.vt=VT_R4;
	//val.fltVal=m_fMin[6];
	//pAttr->setAttribute(L"nv",val);
	////max
	//val.vt=VT_R4;
	//val.fltVal=m_fMax[6];
	//pAttr->setAttribute(L"xv",val);

	////curve8
	//pAttr=pDoc->createElement(L"curve8");
	//pStatic->appendChild(pAttr);
	////tagname
	//val.vt=VT_BSTR;
	//val.bstrVal=m_tagnames[7].AllocSysString();
	//pAttr->setAttribute(L"tag",val);
	////color
	//val.vt=VT_UI4;
	//val.ulVal=m_pens[7].lopnColor;
	//pAttr->setAttribute(L"c",val);
	////analog
	//val.vt=VT_BOOL;
	//val.boolVal=m_bAnalog[7];
	//pAttr->setAttribute(L"a",val);
	////min
	//val.vt=VT_R4;
	//val.fltVal=m_fMin[7];
	//pAttr->setAttribute(L"nv",val);
	////max
	//val.vt=VT_R4;
	//val.fltVal=m_fMax[7];
	//pAttr->setAttribute(L"xv",val);

	////curve9
	//pAttr=pDoc->createElement(L"curve9");
	//pStatic->appendChild(pAttr);
	////tagname
	//val.vt=VT_BSTR;
	//val.bstrVal=m_tagnames[8].AllocSysString();
	//pAttr->setAttribute(L"tag",val);
	////color
	//val.vt=VT_UI4;
	//val.ulVal=m_pens[8].lopnColor;
	//pAttr->setAttribute(L"c",val);
	////analog
	//val.vt=VT_BOOL;
	//val.boolVal=m_bAnalog[8];
	//pAttr->setAttribute(L"a",val);
	////min
	//val.vt=VT_R4;
	//val.fltVal=m_fMin[8];
	//pAttr->setAttribute(L"nv",val);
	////max
	//val.vt=VT_R4;
	//val.fltVal=m_fMax[8];
	//pAttr->setAttribute(L"xv",val);

	////curve10
	//pAttr=pDoc->createElement(L"curve10");
	//pStatic->appendChild(pAttr);
	////tagname
	//val.vt=VT_BSTR;
	//val.bstrVal=m_tagnames[9].AllocSysString();
	//pAttr->setAttribute(L"tag",val);
	////color
	//val.vt=VT_UI4;
	//val.ulVal=m_pens[9].lopnColor;
	//pAttr->setAttribute(L"c",val);
	////analog
	//val.vt=VT_BOOL;
	//val.boolVal=m_bAnalog[9];
	//pAttr->setAttribute(L"a",val);
	////min
	//val.vt=VT_R4;
	//val.fltVal=m_fMin[9];
	//pAttr->setAttribute(L"nv",val);
	////max
	//val.vt=VT_R4;
	//val.fltVal=m_fMax[9];
	//pAttr->setAttribute(L"xv",val);

    //if linked then save dynamic attribution
    pDynamic=pDoc->createElement("dynamic");
    pGO->appendChild(pDynamic);
    //assert unlinked
    val.vt=VT_BOOL;
    val.boolVal=FALSE;
    pDynamic->setAttribute(L"linked",val);
}

long CTrend::GetLevel()
{
	return 2;
}
