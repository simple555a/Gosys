// PolyLine.cpp: implementation of the CPolyLine class.
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
IMPLEMENT_DYNAMIC(CPolyLine,CElement)

CPolyLine::CPolyLine()
{
	m_bHavePen=true;
	m_bHaveBrush=false;
	m_bHaveFont=false;
}

CPolyLine::~CPolyLine()
{

}

void CPolyLine::Draw(CDC *pDC)
{
	if(m_lStatus!=ST_NOMAL) return;

	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);

	CArray<POINT,POINT> m_ptAryTmp;
	m_ptAryTmp.SetSize(m_ptArray.GetSize());

	POINT *pPtLP,*pPtUP;
	pPtLP=m_ptAryTmp.GetData();
	pPtUP=m_ptArray.GetData();

	long i;
	for(i=0;i<m_ptArray.GetSize();i++){
		UPtoLP(pPtUP->x,pPtUP->y,&(pPtLP->x),&(pPtLP->y));
		pPtLP++;
		pPtUP++;
	}
	pDC->Polyline(m_ptAryTmp.GetData(),m_ptArray.GetSize());
	pDC->SelectObject(pOldPen);
}

void CPolyLine::DrawTracker(CDC *pDC)
{
	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CPen *pOldPen=pDC->SelectObject(&pen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);

	CArray<POINT,POINT> m_ptAryTmp;
	m_ptAryTmp.SetSize(m_ptArray.GetSize());

	POINT *pPtLP,*pPtUP,*pPotTemp;
	long i;
	pPtLP=m_ptAryTmp.GetData();
	pPtUP=m_ptArray.GetData();
	for(i=0;i<m_ptArray.GetSize();i++){
		UPtoLP(pPtUP->x,pPtUP->y,&(pPtLP->x),&(pPtLP->y));
		pPtLP++;
		pPtUP++;
	}
	if(m_lHitTarget>0) pDC->Polyline(m_ptAryTmp.GetData(),m_ptAryTmp.GetSize());

	if(m_lHitTarget<0 && m_lHitTarget>=-m_ptAryTmp.GetSize()){
		pPtUP=pPtUP+m_lHitTarget;
		pPtLP=pPtLP+m_lHitTarget;
		if(m_lHitTarget+1<0){
			pPotTemp=pPtLP+1;
			pDC->MoveTo(pPtLP->x,pPtLP->y);
			pDC->LineTo(pPotTemp->x,pPotTemp->y);
		}
		if(m_lHitTarget-1>=-m_ptAryTmp.GetSize()){
			pPotTemp=pPtLP-1;
			pDC->MoveTo(pPtLP->x,pPtLP->y);
			pDC->LineTo(pPotTemp->x,pPotTemp->y);
		}
		pPtUP->x=m_potTracker.x;
		pPtUP->y=m_potTracker.y;
		UPtoLP(pPtUP->x,pPtUP->y,&(pPtLP->x),&(pPtLP->y));
		if(m_lHitTarget+1<0){
			pPotTemp=pPtLP+1;
			pDC->MoveTo(pPtLP->x,pPtLP->y);
			pDC->LineTo(pPotTemp->x,pPotTemp->y);
		}
		if(m_lHitTarget-1>=-m_ptAryTmp.GetSize()){
			pPotTemp=pPtLP-1;
			pDC->MoveTo(pPtLP->x,pPtLP->y);
			pDC->LineTo(pPotTemp->x,pPotTemp->y);
		}
	}

	switch(m_lHitTarget){
	case TK_MOV:
		pPtUP=m_ptArray.GetData();
		for(i=0;i<m_ptArray.GetSize();i++){
			pPtUP->x+=m_potTracker.x;
			pPtUP->y+=m_potTracker.y;
			pPtUP++;
		}
		break;
	}

	if(m_lHitTarget>0 && m_lHitTarget!=TK_CLS){
		pPtLP=m_ptAryTmp.GetData();
		pPtUP=m_ptArray.GetData();
		for(i=0;i<m_ptArray.GetSize();i++){
			UPtoLP(pPtUP->x,pPtUP->y,&(pPtLP->x),&(pPtLP->y));
			pPtLP++;
			pPtUP++;
		}
		pDC->Polyline(m_ptAryTmp.GetData(),m_ptAryTmp.GetSize());
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CPolyLine::DrawHotPoints(CDC *pDC)
{
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
	CPen *pOldPen=(CPen *)pDC->SelectStockObject(NULL_PEN);
	pDC->SetROP2(R2_NOT);

	POINT *pPtUP,PotTemp;
	long i;
	pPtUP=m_ptArray.GetData();
	for(i=0;i<m_ptArray.GetSize();i++){
		UPtoLP(pPtUP->x,pPtUP->y,&(PotTemp.x),&(PotTemp.y));
		DrawHotPoint(pDC,PotTemp.x,PotTemp.y);
		pPtUP++;
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);
}

long CPolyLine::HitTest(long x, long y)
{
	if(m_locked){
			return TK_NONE;
	}
	if(m_lStatus!=ST_NOMAL) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	POINT *pPtUP;
	long i;
	pPtUP=m_ptArray.GetData();
	for(i=0;i<m_ptArray.GetSize();i++){
		rtTemp.SetRect(pPtUP->x-DELTA,pPtUP->y-DELTA,pPtUP->x+DELTA,pPtUP->y+DELTA);
		if(rtTemp.PtInRect(pt)) return i-m_ptArray.GetSize();
		pPtUP++;
	}

	long x1,y1,x2,y2;
	POINT ptTmp[4];
	for(i=0;i<m_ptArray.GetSize()-1;i++){
		x1=m_ptArray[i].x;
		y1=m_ptArray[i].y;
		x2=m_ptArray[i+1].x;
		y2=m_ptArray[i+1].y;
		if((x1-x2)*(y1-y2)>=0){
			ptTmp[0].x=x1-DELTA;
			ptTmp[0].y=y1+DELTA;
			ptTmp[1].x=x1+DELTA;
			ptTmp[1].y=y1-DELTA;
			ptTmp[2].x=x2+DELTA;
			ptTmp[2].y=y2-DELTA;
			ptTmp[3].x=x2-DELTA;
			ptTmp[3].y=y2+DELTA;
		}else{
			ptTmp[0].x=x1+DELTA;
			ptTmp[0].y=y1+DELTA;
			ptTmp[1].x=x1-DELTA;
			ptTmp[1].y=y1-DELTA;
			ptTmp[2].x=x2-DELTA;
			ptTmp[2].y=y2-DELTA;
			ptTmp[3].x=x2+DELTA;
			ptTmp[3].y=y2+DELTA;
		}
		CRgn rgnTmp;
        if(rgnTmp.CreatePolygonRgn(ptTmp,4,ALTERNATE)){
		    if(rgnTmp.PtInRegion(pt)) return TK_MOV;
        }
	}

	return TK_NONE;
}

bool CPolyLine::RInR(LPCRECT lpRect)
{
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;

    /*
	POINT ptTmp[4],*pPtUP;
	long i,x1,y1,x2,y2;
	pPtUP=m_ptArray.GetData();
	for(i=0;i<m_ptArray.GetSize()-1;i++){
		x1=pPtUP->x;
		y1=pPtUP->y;
		pPtUP++;
		x2=pPtUP->x;
		y2=pPtUP->y;
		if((x1-x2)*(y1-y2)>=0){
			ptTmp[0].x=x1-1;
			ptTmp[0].y=y1+1;
			ptTmp[1].x=x1+1;
			ptTmp[1].y=y1-1;
			ptTmp[2].x=x2+1;
			ptTmp[2].y=y2-1;
			ptTmp[3].x=x2-1;
			ptTmp[3].y=y2+1;
		}else{
			ptTmp[0].x=x1+1;
			ptTmp[0].y=y1+1;
			ptTmp[1].x=x1-1;
			ptTmp[1].y=y1-1;
			ptTmp[2].x=x2-1;
			ptTmp[2].y=y2-1;
			ptTmp[3].x=x2+1;
			ptTmp[3].y=y2+1;
		}
		CRgn rgnTmp;
        if(rgnTmp.CreatePolygonRgn(ptTmp,4,ALTERNATE)){
		    if(rgnTmp.RectInRegion(lpRect)) return true;
        }

	}
	
	return false;
    */

    // 原来的相交选取改为现在的包含选取
	
    CRect rectTmp(lpRect->left-1,lpRect->top-1,lpRect->right+1,lpRect->bottom+1);
    rectTmp.NormalizeRect();

    POINT *pPtUP;
	pPtUP=m_ptArray.GetData();

	for(int i=0;i<m_ptArray.GetSize();i++){
        if( !rectTmp.PtInRect(*pPtUP) ){
            return false;
        }
        pPtUP++;
	}

    return true;
    
}

long CPolyLine::Left()
{
	long i,lTemp=TOTAL_WIDTH;
	POINT pt;
	for(i=0;i<m_ptArray.GetSize();i++){
		pt=(POINT)m_ptArray.GetAt(i);
		if(lTemp>pt.x) lTemp=pt.x;
	}

	return lTemp;
}

long CPolyLine::Right()
{
	long i,lTemp=0;
	POINT pt;
	for(i=0;i<m_ptArray.GetSize();i++){
		pt=(POINT)m_ptArray.GetAt(i);
		if(lTemp<pt.x) lTemp=pt.x;
	}

	return lTemp;
}

long CPolyLine::Top()
{
	long i,lTemp=TOTAL_HEIGHT;
	POINT pt;
	for(i=0;i<m_ptArray.GetSize();i++){
		pt=(POINT)m_ptArray.GetAt(i);
		if(lTemp>pt.y) lTemp=pt.y;
	}

	return lTemp;
}

long CPolyLine::Bottom()
{
	long i,lTemp=0;
	POINT pt;
	for(i=0;i<m_ptArray.GetSize();i++){
		pt=(POINT)m_ptArray.GetAt(i);
		if(lTemp<pt.y) lTemp=pt.y;
	}

	return lTemp;
}

void CPolyLine::Format(long left, long top, long right, long bottom, long flag)
{
	long ldx,ldy;
	int i;
	POINT *pPt;
	pPt=m_ptArray.GetData();

	switch(flag){
	case FM_A2L:
		ldx=left-Left();
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->x+=ldx;
			pPt++;
		}
		break;
	case FM_A2C:
		ldx=(left+right-Left()-Right())/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->x+=ldx;
			pPt++;
		}
		break;
	case FM_A2R:
		ldx=right-Right();
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->x+=ldx;
			pPt++;
		}
		break;
	case FM_A2T:
		ldy=top-Top();
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->y+=ldy;
			pPt++;
		}
		break;
	case FM_A2M:
		ldy=(top+bottom-Top()-Bottom())/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->y+=ldy;
			pPt++;
		}
		break;
	case FM_A2B:
		ldy=bottom-Bottom();
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->y+=ldy;
			pPt++;
		}
		break;
	case FM_A2CM:
		ldx=(left+right-Left()-Right())/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->x+=ldx;
			pPt++;
		}
		ldy=(top+bottom-Top()-Bottom())/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->y+=ldy;
			pPt++;
		}
		break;
	case FM_R2W:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		for(i=0;i<m_ptArray.GetSize();i++){ 
			R2W(ldx,ldy,&(pPt->x),&(pPt->y));
			pPt++;
		}
		break;
	case FM_R2U:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			R2U(ldx,ldy,&(pPt->x),&(pPt->y));
			pPt++;
		}
		break;
	case FM_F2Z:
		ldx=(left+right)/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			F2Z(ldx,&(pPt->x));
			pPt++;
		}
		break;
	case FM_F2I:
		ldy=(top+bottom)/2;
		for(i=0;i<m_ptArray.GetSize();i++){
			F2I(ldy,&(pPt->y));
			pPt++;
		}
		break;
	case FM_S2G:
		for(i=0;i<m_ptArray.GetSize();i++){
			pPt->x=(pPt->x+right/2)-(pPt->x+right/2)%right;
			pPt->y=(pPt->y+bottom/2)-(pPt->y+bottom/2)%bottom;
			pPt++;
		}
		break;
	}
}

CElement * CPolyLine::Clone(long lCmdId, long lStatus)
{
	CPolyLine * pNew=new CPolyLine();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_ptArray.Copy(m_ptArray);
	pNew->m_pen=m_pen;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CPolyLine::Save(MSXML::IXMLDOMElementPtr pRoot)
{
	MSXML::IXMLDOMElementPtr pGO;
	MSXML::IXMLDOMElementPtr pStatic;
	MSXML::IXMLDOMElementPtr pDynamic;
	MSXML::IXMLDOMElementPtr pAttr;
	MSXML::IXMLDOMElementPtr pPoint;
    _variant_t val;

	MSXML::IXMLDOMDocumentPtr pDoc=pRoot->ownerDocument;
    
    //add a graphy object
    pGO=pDoc->createElement(L"go");    
    val.vt=VT_BSTR;
    val.bstrVal=::SysAllocString(L"polyline");
    pGO->setAttribute(L"type",val);
    pRoot->appendChild(pGO);

    //add a group static attribution
    pStatic=pDoc->createElement(L"static");
    pGO->appendChild(pStatic);
    
    //position
    pAttr=pDoc->createElement(L"position");
    pStatic->appendChild(pAttr);
    //point array
	POINT *pPt;
    pPt=m_ptArray.GetData();
    for(int i=0;i<m_ptArray.GetSize();i++){
        pPoint=pDoc->createElement(L"pt");
        pAttr->appendChild(pPoint);
        val.vt=VT_I4;
        val.lVal=pPt->x;
        pPoint->setAttribute(L"x",val);
        val.lVal=pPt->y;
        pPoint->setAttribute(L"y",val);
        pPt++;
    }    

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

void CPolyLine::Load(MSXML::IXMLDOMElementPtr pGO)
{
    MSXML::IXMLDOMElementPtr pStatic;
    MSXML::IXMLDOMElementPtr pDynamic;
	MSXML::IXMLDOMElementPtr pAttr;
    MSXML::IXMLDOMElementPtr pPoint;
    _variant_t val;

    pStatic=pGO->selectSingleNode(L"./static");
    //position
    pAttr=pStatic->selectSingleNode(L"./position");
    //point array
    int i=0;
	POINT * pPt;
    for(pPoint=pAttr->firstChild;pPoint!=NULL;pPoint=pPoint->nextSibling){
        i++;
        m_ptArray.SetSize(i);
        pPt=m_ptArray.GetData();
        pPt=pPt+i-1;
        val=pPoint->getAttribute(L"x");
        val.ChangeType(VT_I4);
        pPt->x=val.lVal;
        val=pPoint->getAttribute(L"y");
        val.ChangeType(VT_I4);
        pPt->y=val.lVal;
    }

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

void CPolyLine::MoveInCell(POINT potTracker)
{
	POINT *pPtUP;

	pPtUP=m_ptArray.GetData();
	for(int i=0;i<m_ptArray.GetSize();i++){
		pPtUP->x+=potTracker.x;
		pPtUP->y+=potTracker.y;
		pPtUP++;
	}
}
