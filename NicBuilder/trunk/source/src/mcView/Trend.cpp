// Trend.cpp: implementation of the CTrend class.
//
//////////////////////////////////////////////////////////////////////

/*
修改记录：
1、Zander,2012-5-28
   查询数据报等待时间由0改为200，以解决历史趋势控件显示不稳定

2、Zander Sun, 2012-7-12  1.2.0.4
   由于历史数据查询的数据报报头又改回了旧历史库的报头，所以查询接口函数的调用方法也修改了回来--CTrend::initCache()
*/

#include "stdafx.h"
#pragma hdrstop
#include "Trend.h"
#include "dlgTrendTime.h"
#include "common.h"

#pragma warning(disable:4800)
#pragma warning(disable:4305)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CString timeAsString(hislog_key_t t);

IMPLEMENT_DYNAMIC(CTrend,CElement)

/*
	display legent text on current select time stamp
*/
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


	pDC->DrawText(str,rt,nFormat);
	pDC->SelectObject(pOldFont);
}

CTrend::CTrend()
{
	m_bHavePen=false;
	m_bHaveBrush=false;
	m_bHaveFont=false;
	m_autoScale=false;

	unsigned char r, g, b;
	for(int i=0;i<TREND_COUNT;i++){
		m_pens[i].lopnStyle=PS_SOLID;
		m_pens[i].lopnWidth.x=0;
		m_bAnalog[i]=true;
		m_fMin[i]=0.;
		m_fMax[i]=100.;
		r = 255 - (255 / TREND_COUNT) * i;
		g = (255 / TREND_COUNT) * i;
		if(i % 2){
			b = 0;
		}else{
			b = 255;
		}
		m_pens[i].lopnColor = RGB(r, g, b);
	}

	m_BkColor=RGB(255,255,255);
	m_GdColor=RGB(192,192,192);

	m_DnyAttr.m_en=true;
	m_posX = -1;

	m_cache = createArchiveCache();
	memset(m_vals, 0, sizeof(m_vals));
	i_buf = 0;
}

CTrend::~CTrend()
{
	close_handle(m_cache);
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

void CTrend::Draw(CDC *pDC)
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

	long lyTag,lxTag1,lxTag2;
	UPtoLP(m_ly2-200,m_ly2-200,0,&lyTag);
	UPtoLP(m_lx1+200,m_lx1+200,&lxTag1,0);
	UPtoLP(m_lx2-200,m_lx2-200,&lxTag2,0);
	// utils_debug("m_lx2=%d,lxTag2=%d\n", m_lx2,lxTag2);
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

#if 0
	{
		static int count;
		if(++count==1){
			utils_trace("(%d,%d),(%d,%d),(%d,%d)\n",
				pts[0].x,pts[0].y,
				pts[1].x,pts[1].y,
				pts[2].x,pts[2].y
				);
			utils_debug("lxTag1=%d,lxTag2=%d,lGap=%d,x1=%d,x2=%d,m_lx1=%d,m_lx2=%d\n",
				lxTag1,lxTag2,lGap,x1,x2,m_lx1,m_lx2
				);
			count=0;
		}
	}
#endif

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

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
		UPtoLP(m_lx1,m_lx1,&x1,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_TM:
		if(m_ly2-m_potTracker.y>=clTrendMinHeight)
			m_ly1=m_potTracker.y;
		UPtoLP(m_ly1,m_ly1,0,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RM:
		if(m_potTracker.x-m_lx1>=clTrendMinWidth)
			m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_lx2,&x2,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_BM:
		if(m_potTracker.y-m_ly1>=clTrendMinHeight)
			m_ly2=m_potTracker.y;
		UPtoLP(m_ly2,m_ly2,0,&y2);
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
	CRect rtTemp0,rtTemp1,rtTemp2,rtTemp3;
	rtTemp0.SetRect(m_lx1, m_ly1, m_lx2, m_ly2-200);
	rtTemp1.SetRect(m_lx1,m_ly2-200,m_lx1+200,m_ly2);
	rtTemp2.SetRect(m_lx2-200,m_ly2-200,m_lx2,m_ly2);
	rtTemp3.SetRect(m_lx1+600,m_ly2-200,m_lx2-600,m_ly2);

	POINT pt;
	pt.x=x;
	pt.y=y;

	if(rtTemp0.PtInRect(pt)){
		m_DnyAttr.m_push = TREND_VALUE;
		return TK_PUSH;
	}
	if(rtTemp1.PtInRect(pt)){
		m_DnyAttr.m_push=TREND_LEFT;
		return TK_PUSH;
	}
	if(rtTemp2.PtInRect(pt)){
		m_DnyAttr.m_push=TREND_RIGHT;
		return TK_PUSH;
	}
	if(rtTemp3.PtInRect(pt)){
		m_DnyAttr.m_push=TREND_TIME;
		return TK_PUSH;
	}

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
	if(m_lStatus!=ST_NOMAL) return false;

	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(m_lx1,m_ly1,m_lx2,m_ly2)) return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
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
	for(int i=0;i<TREND_COUNT;i++){
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

	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &m_startTime);
		long step;
		switch(m_lMode){
		case 1:
			step = 15*60;	//15 min
			break;
		case 2:
			step = 3600;	//1   hour
			break;
		case 3:
			step = 4*3600;	//4 4 hour
			break;
		case 4:
			step = 16*3600; //16 hour
			break;
		default:
			step =3*60;//3 3 min
		}
		m_startTime = hislogTimeByKey(hislogKeyByTime(&m_startTime) - step * 3 / 4);
	}
	
	initCache();
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

void CTrend::initCache()
{
	__uint step;
	TAG_NAME names[HISLOG_MAX_QUERY_TAG_COUNT]; 
	__int tagCount;
	
	switch(m_lMode){
	case 1:
		step = 5;	//15 min
		break;
	case 2:
		step = 20;	//1   hour
		break;
	case 3:
		step = 80;	//4 4 hour
		break;
	case 4:
		step = 320; //16 hour
		break;
	default:
		step =1;	//3 3 min
	}

	tagCount = 0;
	for(int i=0; i<TREND_COUNT; i++){
		if(m_tagnames[i].IsEmpty()) {
			continue;
		}
		tagname_by_textA(&names[tagCount++], (LPCTSTR)m_tagnames[i]);
	}
    //2012-7-12	
	cacheSetMode(m_cache, PT_QueryMode_BYCOUNT);
	cacheSetNames(m_cache, names, tagCount);
	cacheSetTimeSpan(m_cache, hislogKeyByTime(&m_startTime), step, 180);
	cacheQuery(m_cache, 0, &m_cacheInfo);
	
/*Zander 20120202*/
// 	cacheSetNames(m_cache, names, tagCount);
// 	__u32 i_count = 180;
// 	hislog_key_t i_st = hislogKeyByTime(&m_startTime);
// 	hislog_key_t i_et = i_st + step*i_count;
// 	cacheSetTimeSpan(m_cache, i_st, i_et, i_count, step);
// 	cacheQuery(m_cache, 200, &m_cacheInfo);//2012-5-28
/**/
	i_buf = 100;
}

void CTrend::autoScroll()
{	
	hislog_key_t now, te;

	te = m_cacheInfo.startTime + m_cacheInfo.step * m_cacheInfo.valueCount;

	now = hislogGetCurrentTime();
	
	if(m_cacheInfo.startTime > now) {
		// we're totally in the future, no need to query for new data
		return;
	}

	// now check if we're getting new data as times passed by
	if(te > now){
		/*
			we advance the query start time by one
			tenth of the total query time span if the
			current time goes near enouth to the right
			margin(one tenth of the span)
		*/
		if(now > te - 18 * m_cacheInfo.step){
		//	cacheSetMode(m_cache, PT_QueryMode_BYCOUNT);//2012-7-10
			cacheScrollTimeSpan(m_cache, 18 * m_cacheInfo.step);
			cacheQuery(m_cache, 200, &m_cacheInfo);//2012-5-28
			i_buf = 0;
		}else{
			/*
				if 5 seconds or more passed since last query, we
				issue another query request
			*/
			int i, j;
			for(i = 0; i < 5; i++){
				for(j = 0; j < 180; j++){
					
				}
			}
			if(now >= m_cacheInfo.queryTime + 5){
				cacheQuery(m_cache, 200, &m_cacheInfo);//2012-5-28
				i_buf = 0;
			}
		}
	}
}

/*
	draw current values, move pointer.
*/
void CTrend::DrawCurve(CDC *pDC)
{
	i_buf++;
	if(i_buf > 5){
		bufValues();
		if(i_buf > 100){
			i_buf = 100;
		}
	}

	long x1,y1,x2,y2;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2-200,&x2,&y2);
	__uint i,j;
	long x,y,xOld,yOld;
	float fy;
	CPen *pOldPen;
	PCpmc_value_t value;
	__uint tc;

	tc = m_cacheInfo.tagCount;
	
	if(m_autoScale){
		for(i = 0; i < tc; i++){
			float fb, fs;
			fb = -1e10;
			fs = 1e10;

			for(j=0; j<180; j++){
				//value = cacheGetValue(m_cache, i, j);
				value=&m_vals[i][j];
				if((value->Flags & TF_Valid)){
					switch(get_value_type(value->Flags)){
					case dt_real4:
						if(fb < value->Value.fltValue){
							fb = value->Value.fltValue;
						}
						if(fs > value->Value.fltValue){
							fs = value->Value.fltValue;
						}
						
						if(fs < fb){
							m_fMax[i] = fb + (fb - fs) / 10.;
							m_fMin[i] = fs - (fb - fs) / 10.;
						}

						break;
					}
				}
			}
		}
	}

	for(i=0; i<tc; i++){
		CPen pen;
		pen.CreatePenIndirect(&(m_pens[i]));
		pOldPen=(CPen*)pDC->SelectObject(&pen);
		//if(!cacheGetFlag(m_cache, i)){
			//
		//}else{
			x=x1;
			xOld=-1;
			for(j=0; j<180; j++){
				value=&m_vals[i][j];
				//value = cacheGetValue(m_cache, i, j);
				if(!(value->Flags & TF_Valid)){
					xOld = -1;
				}else{
					x=x1+(j+0.5)*(x2-x1)/180;
					
					switch(get_value_type(value->Flags)){
					case dt_real4:
						//if(value->Value.fltValue > m_fMax[i]){
						//	m_fMax[i] += (value->Value.fltValue - m_fMin[i]) / 2.;
						//}
						//if(value->Value.fltValue < m_fMin[i]){
						//	m_fMin[i] -= (m_fMax[i] - value->Value.fltValue) / 2.;
						//}
						fy = m_fMax[i] - m_fMin[i];
						if(fy < 0.000001) fy = 0.000001;
						y = y2 - (y2 - y1) * (value->Value.fltValue-m_fMin[i]) / fy;
						break;
					case dt_bool:
						fy = m_fMax[i] - m_fMin[i];
						if(fy < 0.000001) fy = 0.000001;
						if(0==value->Value.b){
							y = y2 - (y2-y1)*(0-m_fMin[i]) / fy;
						}else{
							y = y2 - (y2-y1)*(1-m_fMin[i]) / fy;
						}
// 					

// 						if(0==value->Value.b){
// 							y=y2-(2*i+0.5)*(y2-y1)/10;
// 						}else{
// 							y=y2-(2*i+1.5)*(y2-y1)/10;
// 						}
						break;

// 					case dt_bool:
// 						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
// 						if(fy<0.000001) fy=0.000001;
// 						if(0==value->Value.b){
// 							y = (0-pmf->m_fMin[i]) / fy * 1000;
// 						}else{
// 							y = (1-pmf->m_fMin[i]) / fy * 1000;
// 						}
// 						y = fsy*(cVerGap+cLabelHeight+y);
// 						break;

					case dt_int32:
						fy=m_fMax[i]-m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y=y2-(y2-y1)*(value->Value.iValue-m_fMin[i])/fy;
						break;
					case dt_int16:
						fy=m_fMax[i]-m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y=y2-(y2-y1)*(value->Value.iValue-m_fMin[i])/fy;
						break;
					case dt_int64:
						fy=m_fMax[i]-m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y=y2-(y2-y1)*(value->Value.iValue-m_fMin[i])/fy;
						break;
					case dt_real8:
						fy=m_fMax[i]-m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y=y2-(y2-y1)*(value->Value.dblValue-m_fMin[i])/fy;
						break;
					case dt_date:
						fy=m_fMax[i]-m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y=y2-(y2-y1)*(value->Value.dblValue-m_fMin[i])/fy;
						break;
					}
					
					if(y>y1 && y<=y2){
						if(xOld>0){
							pDC->MoveTo(xOld,yOld);
							pDC->LineTo(x,y);
						}else{
							pDC->SetPixelV(x,y,m_pens[i].lopnColor);
						}
						xOld=x;
						yOld=y;
					}else{
						xOld=-1;
					}
				}
			//}
		}
			
		pDC->SelectObject(pOldPen);
	}

	ShowValue(pDC);

	// draw labels on the axis
	{
		long lyTag,lxTag1,lxTag2;
		
		UPtoLP(m_ly2-200,m_ly2-200,0,&lyTag);
		UPtoLP(m_lx1+200,m_lx1+200,&lxTag1,0);
		UPtoLP(m_lx2-200,m_lx2-200,&lxTag2,0);
		
		CRect rt(lxTag1,lyTag,lxTag2,y2);
		
		DrawTag(
			timeAsString(m_cacheInfo.startTime), 
			rt, 
			DT_LEFT|DT_NOCLIP,
			pDC
			);

		hislog_key_t te;
		te = m_cacheInfo.startTime + m_cacheInfo.step * m_cacheInfo.valueCount;
		DrawTag(
			timeAsString(te), 
			rt, 
			DT_RIGHT | DT_NOCLIP,
			pDC
			);
	}
	
	// handle auto-scrolling
	autoScroll();
}

void CTrend::ScrlLeft()
{
	cacheScrollTimeSpan(m_cache, -(__i64)m_cacheInfo.step * m_cacheInfo.valueCount);
	cacheQuery(m_cache, 200, &m_cacheInfo);//2012-5-28
	i_buf = 100;
}

void CTrend::ScrlRight()
{
	cacheScrollTimeSpan(m_cache, m_cacheInfo.step * m_cacheInfo.valueCount);
	cacheQuery(m_cache, 200, &m_cacheInfo);//2012-5-28
	i_buf = 100;
}

void CTrend::ResetTime(FILETIME ftm, int mode)
{
	m_startTime = ftm;
	m_lMode=mode;
	initCache();	
}

void CTrend::ShowValue(CDC * pDC)
{

	long x1,y1,x2,y2;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2-200,&x2,&y2);

	if(m_posX < x1 || m_posX > x2) return;

	long xPos = m_posX + 2;
	long j = (m_posX - x1) * 180 / (x2	- x1);
	if(j > 180){
		j = 180;
	}
	long yPos = m_posY;
	long dy;
	CElement::UPtoLP(100, 100, 0, &dy);
	CString str;

	str = timeAsString(m_cacheInfo.startTime + j * m_cacheInfo.step);

	COLORREF clr = 0xFFFFFF ^ m_BkColor;
	CRect rt(xPos,yPos,x2,y2);
	DrawTag(str, rt, DT_LEFT|DT_NOCLIP, pDC, clr);

	CPen pen;
	pen.CreatePen(PS_DASH, 1, clr);
	CPen *pOldPen=(CPen*)pDC->SelectObject(&pen);
	pDC->MoveTo(m_posX, y1);
	pDC->LineTo(m_posX, y2);
	pDC->SelectObject(pOldPen);

	__uint i;
	for(i = 0; i < m_cacheInfo.tagCount; i++){
		yPos = m_posY + (i + 2) * dy;
		CRect rt(xPos,yPos,x2,y2);
		PCpmc_value_t value;
		
		if(!cacheGetFlag(m_cache, i)){
			str = "N/A";
		}else{
			//value = cacheGetValue(m_cache, i, j);
			value = &m_vals[i][j];
			if(value->Flags & TF_Valid){
				char * p = pmc_value_string(value);
				str = p;
				free_buffer(p);
			}else{
				str = "N/A";
			}
		}
		DrawTag(str, rt, DT_LEFT|DT_NOCLIP, pDC, m_pens[i].lopnColor);
	}
}

void CTrend::config()
{
	FILETIME ftm;
	
	SYSTEMTIME stm;
	CDlgTrendTime dlg;

	ftm = hislogTimeByKey(m_cacheInfo.startTime);
	
	FileTimeToSystemTime(&ftm,&stm);
	
	dlg.m_date.SetDate(stm.wYear,stm.wMonth,stm.wDay);
	dlg.m_time.SetTime(stm.wHour,stm.wMinute,stm.wSecond);
	
	dlg.m_mode = this->m_lMode;
	dlg.m_autoScale = m_autoScale;
	// trend 1
	dlg.m_tag1 = this->m_tagnames[0];
	dlg.m_ca1 = this->m_bAnalog[0];
	dlg.m_min1 = this->m_fMin[0];
	dlg.m_max1 = this->m_fMax[0];
	dlg.cuvColor[0] = this->m_pens[0].lopnColor;
	// trend 2
	dlg.m_tag2 = this->m_tagnames[1];
	dlg.m_ca2 = this->m_bAnalog[1];
	dlg.m_min2 = this->m_fMin[1];
	dlg.m_max2 = this->m_fMax[1];
	dlg.cuvColor[1] = this->m_pens[1].lopnColor;
	// trend 3
	dlg.m_tag3 = this->m_tagnames[2];
	dlg.m_ca3 = this->m_bAnalog[2];
	dlg.m_min3 = this->m_fMin[2];
	dlg.m_max3 = this->m_fMax[2];
	dlg.cuvColor[2] = this->m_pens[2].lopnColor;
	// trend 4
	dlg.m_tag4 = this->m_tagnames[3];
	dlg.m_ca4 = this->m_bAnalog[3];
	dlg.m_min4 = this->m_fMin[3];
	dlg.m_max4 = this->m_fMax[3];
	dlg.cuvColor[3] = this->m_pens[3].lopnColor;
	//trend 5
	dlg.m_tag5 = this->m_tagnames[4];
	dlg.m_ca5 = this->m_bAnalog[4];
	dlg.m_min5 = this->m_fMin[4];
	dlg.m_max5 = this->m_fMax[4];
	dlg.cuvColor[4] = this->m_pens[4].lopnColor;
	
	if(dlg.DoModal()==IDOK){
		stm.wYear=dlg.m_date.GetYear();
		stm.wMonth=dlg.m_date.GetMonth();
		stm.wDay=dlg.m_date.GetDay();
		stm.wHour=dlg.m_time.GetHour();
		stm.wMinute=dlg.m_time.GetMinute();
		stm.wSecond=dlg.m_time.GetSecond();
		stm.wMilliseconds=0;
		
		SystemTimeToFileTime(&stm, &m_startTime);
		
		// trend 1
		this->m_tagnames[0] = dlg.m_tag1;
		this->m_bAnalog[0] = dlg.m_ca1;
		this->m_fMin[0] = dlg.m_min1;
		this->m_fMax[0] = dlg.m_max1;
		// trend 2
		this->m_tagnames[1] = dlg.m_tag2;
		this->m_bAnalog[1] = dlg.m_ca2;
		this->m_fMin[1] = dlg.m_min2;
		this->m_fMax[1] = dlg.m_max2;
		// trend 3
		this->m_tagnames[2] = dlg.m_tag3;
		this->m_bAnalog[2] = dlg.m_ca3;
		this->m_fMin[2] = dlg.m_min3;
		this->m_fMax[2] = dlg.m_max3;
		// trend 4
		this->m_tagnames[3] = dlg.m_tag4;
		this->m_bAnalog[3] = dlg.m_ca4;
		this->m_fMin[3] = dlg.m_min4;
		this->m_fMax[3] = dlg.m_max4;
		// trend 5
		this->m_tagnames[4] = dlg.m_tag5;
		this->m_bAnalog[4] = dlg.m_ca5;
		this->m_fMin[4] = dlg.m_min5;
		this->m_fMax[4] = dlg.m_max5;

		m_autoScale = dlg.m_autoScale;
		m_lMode = dlg.m_mode;
		
		initCache();
	}
}

//将hisproxy缓存中的数取出，赋给m_vals
void CTrend::bufValues()
{
	for(int i=0; i<5; i++){
		for(int j=0; j<180; j++){
			if(!cacheGetFlag(m_cache, i)){
				m_vals[i][j].Flags = 0;
			}else{
				m_vals[i][j] = *cacheGetValue(m_cache, i, j);
			}
		}
		continue;
	}
}
