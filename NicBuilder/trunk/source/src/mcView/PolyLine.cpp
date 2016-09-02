//= UPDATE RECORD
//= Update for    Version     Update by      Date         Update                           
//= (1-1)         1.0.0.1        mox       2013-9-29      1.CPolyLine::Load()中新增m_open=m_pen
// PolyLine.cpp: implementation of the CPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolyLine.h"

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
	if(m_DnyAttr.m_Linked && m_DnyAttr.m_na){
		long x1,y1,x2,y2;
		UPtoLP(Left(),Top(),&x1,&y1);
		UPtoLP(Right(),Bottom(),&x2,&y2);
		CPen *pop=(CPen*)(pDC->SelectStockObject(BLACK_PEN));
		CBrush *pob=(CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
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
    if(!m_DnyAttr.m_Linked) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	long x1,y1,x2,y2;
	POINT ptTmp[4];
	int i;
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
		    if(rgnTmp.PtInRegion(pt)) return TK_PUSH;
        }
	}
	return TK_NONE;
}

bool CPolyLine::RInR(LPCRECT lpRect)
{
	if(m_lStatus!=ST_NOMAL) return false;

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
}

long CPolyLine::Left()
{
	long i,lTemp=5120;
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
	long i,lTemp=3200;
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
	m_ptoArray.Copy(m_ptArray);

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
    val=pAttr->getAttribute(L"c");//线条填充颜色
    val.ChangeType(VT_UI4);
    m_pen.lopnColor=val.ulVal;
	m_open=m_pen;//mox+++20130929

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

void CPolyLine::MoveElement(POINT potOffset)
{
	POINT *pPt,*poPt;

	pPt=m_ptArray.GetData();
	poPt=m_ptoArray.GetData();

	for(int i=0;i<m_ptArray.GetSize();i++){
		pPt->x=poPt->x+potOffset.x;
		pPt->y=poPt->y+potOffset.y;
		pPt++;
		poPt++;
	}
}

void CPolyLine::UpdateView()
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
				if((glTimer%8)>3) bBlink =false;
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
			if(r4>m_DnyAttr.m_ColorPenA.m_Split1){
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
	int i;
	POINT *pPt,*poPt;
	CArray<POINT,POINT> ptArray;
	ptArray.Copy(m_ptoArray);
	//pos h
	if(m_DnyAttr.m_PosH.m_Used){
		if(m_DnyAttr.m_PosH.m_TagValue.Flags & TF_Valid){			
			lp=(getFloat(m_DnyAttr.m_PosH.m_TagValue)-m_DnyAttr.m_PosH.m_LTO)*\
				(m_DnyAttr.m_PosH.m_RBT-m_DnyAttr.m_PosH.m_LTT)/\
				(m_DnyAttr.m_PosH.m_RBO-m_DnyAttr.m_PosH.m_LTO);
			if(lp<m_DnyAttr.m_PosH.m_LTT) lp=m_DnyAttr.m_PosH.m_LTT;
			if(lp>m_DnyAttr.m_PosH.m_RBT) lp=m_DnyAttr.m_PosH.m_RBT;
			pPt=m_ptArray.GetData();
			poPt=m_ptoArray.GetData();
			for(i=0;i<m_ptArray.GetSize();i++){
				pPt->x=poPt->x+lp;
				pPt++;
				poPt++;
			}
			ptArray.Copy(m_ptArray);			
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
			pPt=m_ptArray.GetData();
			poPt=m_ptoArray.GetData();
			for(i=0;i<m_ptArray.GetSize();i++){
				pPt->y=poPt->y+lp;
				pPt++;
				poPt++;
			}
			ptArray.Copy(m_ptArray);			
		}else{
			m_DnyAttr.m_na=true;
		}
	}

	long lLeft;
	long lRight;
	long lTop;
	long lBottom;
	POINT pt;
	lLeft=TOTAL_WIDTH;
	lTop=TOTAL_HEIGHT;
	lRight=0;
	lBottom=0;
	for(i=0;i<ptArray.GetSize();i++){
		pt=(POINT)ptArray.GetAt(i);
		if(lLeft>pt.x) lLeft=pt.x;
		if(lRight<pt.x) lRight=pt.x;
		if(lTop>pt.y) lTop=pt.y;
		if(lBottom<pt.y) lBottom=pt.y;
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
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->x=lLeft+(poPt->x-lLeft)*fp;
					pPt++;
					poPt++;
				}
			}else if(m_DnyAttr.m_SizeH.m_Ref==1){
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->x=(lLeft+lRight)/2-(poPt->x-lLeft/2-lRight/2)*fp;
					pPt++;
					poPt++;
				}
			}else if(m_DnyAttr.m_SizeH.m_Ref==2){
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->x=lRight-(lRight-poPt->x)*fp;
					pPt++;
					poPt++;
				}
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
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->y=lTop+(poPt->y-lTop)*fp;
					pPt++;
					poPt++;
				}
			}else if(m_DnyAttr.m_SizeV.m_Ref==1){
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->y=(lTop+lBottom)/2-(poPt->y-lTop/2-lBottom/2)*fp;
					pPt++;
					poPt++;
				}
			}else if(m_DnyAttr.m_SizeV.m_Ref==2){
				pPt=m_ptArray.GetData();
				poPt=ptArray.GetData();
				for(i=0;i<m_ptArray.GetSize();i++){
					pPt->y=lBottom-(lBottom-poPt->y)*fp;
					pPt++;
					poPt++;
				}
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
		if(!(m_DnyAttr.m_ButtonA.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//button d
	if(m_DnyAttr.m_ButtonD.m_Used){
		if(!(m_DnyAttr.m_ButtonD.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//input a
	if(m_DnyAttr.m_InputA.m_Used){
		if(!(m_DnyAttr.m_InputA.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//input d
	if(m_DnyAttr.m_InputD.m_Used){
		if(!(m_DnyAttr.m_InputD.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//slide h
	if(m_DnyAttr.m_SlideH.m_Used){
		if(!(m_DnyAttr.m_SlideH.m_TagValue.Flags & TF_Valid)){
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
