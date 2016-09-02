// Cell.cpp: implementation of the CCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line.h"
#include "Arc.h"
#include "PolyLine.h"
#include "Rectangle.h"
#include "RoundRect.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "Text.h"
#include "Button.h"
#include "Meta.h"
#include "Cell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CCell,CElement)

CCell::CCell()
{

}

CCell::~CCell()
{
	POSITION pos;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		delete m_Elements.GetNext(pos);
	}
}

CElement * CCell::Clone(long lCmdId, long lStatus)
{
	CCell * pNew=new CCell();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_Elements.GetNext(pos));
        pNew->m_Elements.AddTail(pElement->Clone(0,ST_NOMAL));
	}

	return (CElement*) pNew;
}

void CCell::Draw(CDC *pDC)
{
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_Elements.GetNext(pos));
		pElement->Draw(pDC);
	}
}

void CCell::DrawHotPoints(CDC *pDC)
{
	LOGPEN tLogPen;
	tLogPen.lopnColor=RGB(0,0,0);
	tLogPen.lopnStyle=PS_DOT;
	tLogPen.lopnWidth.x=1;
	CPen tPen;
	tPen.CreatePenIndirect(&tLogPen);
	CPen *pOldPen=(CPen *)pDC->SelectObject(&tPen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2;
	UPtoLP(Left()-DELTA,Top()-DELTA,&x1,&y1);
	UPtoLP(Right()+DELTA,Bottom()+DELTA,&x2,&y2);
	pDC->Rectangle(x1,y1,x2,y2);

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);
}

void CCell::DrawTracker(CDC *pDC)
{
	LOGPEN tLogPen;
	tLogPen.lopnColor=RGB(0,0,0);
	tLogPen.lopnStyle=PS_DOT;
	tLogPen.lopnWidth.x=1;
	CPen tPen;
	tPen.CreatePenIndirect(&tLogPen);
	CPen *pOldPen=(CPen *)pDC->SelectObject(&tPen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2;

	switch(m_lHitTarget){
	case TK_CT:
		break;
	case TK_MOV:
		UPtoLP(Left(),Top(),&x1,&y1);
		UPtoLP(Right(),Bottom(),&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		MoveInCell(m_potTracker);
		UPtoLP(Left(),Top(),&x1,&y1);
		UPtoLP(Right(),Bottom(),&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	}
	
	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CCell::Format(long left, long top, long right, long bottom, long flag)
{
	POINT pot;
	POSITION pos;
	CElement *pElement;

	switch(flag){
	case FM_A2L:
		pot.x=left-Left();
		pot.y=0;
		MoveInCell(pot);
		break;
	case FM_A2C:
		pot.x=(left-Left()+right-Right())/2;
		pot.y=0;
		MoveInCell(pot);
		break;
	case FM_A2R:
		pot.x=right-Right();
		pot.y=0;
		MoveInCell(pot);
		break;
	case FM_A2T:
		pot.x=0;
		pot.y=top-Top();
		MoveInCell(pot);
		break;
	case FM_A2M:
		pot.x=0;
		pot.y=(top-Top()+bottom-Bottom())/2;
		MoveInCell(pot);
		break;
	case FM_A2B:
		pot.x=0;
		pot.y=bottom-Bottom();
		MoveInCell(pot);
		break;
	case FM_A2CM:
		pot.x=(left-Left()+right-Right())/2;
		pot.y=(top-Top()+bottom-Bottom())/2;
		MoveInCell(pot);
		break;
	case FM_R2W:
		for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
			pElement=(CElement *)(m_Elements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_R2W);
		}
		break;
	case FM_R2U:
		for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
			pElement=(CElement *)(m_Elements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_R2U);
		}
		break;
	case FM_F2Z:
		pot.x=left+right-Left()-Right();
		pot.y=0;
		MoveInCell(pot);
		break;
	case FM_F2I:
		pot.x=0;
		pot.y=top+bottom-Top()-Bottom();
		MoveInCell(pot);
		break;
	case FM_S2G:
		break;
	}

}

long CCell::HitTest(long x, long y)
{
	CElement *pElement,*pSelElement=NULL;
	POSITION pos;
	for(pos=m_Elements.GetTailPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetPrev(pos)));
		if(pElement->HitTest(x,y)==TK_PUSH){
			pSelElement=pElement;
			break;
		}
	}
	
	m_pSelElement=pSelElement;
	if(m_pSelElement==NULL){
		return TK_NONE;
	}else{
		return TK_PUSH;
	}
}

long CCell::Left()
{
	long lTemp=0x7fffffff;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
		pElement=(CElement *)(m_Elements.GetNext(pos));
		if(lTemp>pElement->Left()) lTemp=pElement->Left();
	}
	
	return lTemp;
}

void CCell::Load(MSXML::IXMLDOMElementPtr pGO)
{
    //×°ÔØ×ÓÔªËØ
    _bstr_t strGO=L"go";
    _variant_t strArc=L"arc";
    _variant_t strCell=L"cell";
    _variant_t strEllipse=L"ellipse";
    _variant_t strLine=L"line";
    _variant_t strMCButton=L"mcbutton";
    _variant_t strMeta=L"meta";
    _variant_t strPolygon=L"polygon";
    _variant_t strPolyLine="polyline";
    _variant_t strRectangle=L"rectangle";
    _variant_t strRoundRect=L"roundrect";
    _variant_t strText=L"text";

    _variant_t val;

    MSXML::IXMLDOMElementPtr pItem;
    CElement * pElement;
    for(pItem=pGO->firstChild;pItem!=NULL;pItem=pItem->nextSibling){
        if(pItem->baseName==strGO){
            val=pItem->getAttribute(L"type");
            if(val==strRectangle){
			    pElement=new CRectangle();
                ((CRectangle*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strArc){
			    pElement=new CArc();
                ((CArc*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strEllipse){
			    pElement=new CEllipse();
                ((CEllipse*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strLine){
			    pElement=new CLine();
                ((CLine*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strMCButton){
			    pElement=new CMCButton();
                ((CMCButton*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strMeta){
			    pElement=new CMeta();
                ((CMeta*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strRoundRect){
			    pElement=new CRoundRect();
                ((CRoundRect*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strText){
			    pElement=new CText();
                ((CText*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strPolygon){
			    pElement=new CPolygon();
                ((CPolygon*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
           }else if(val==strPolyLine){
			    pElement=new CPolyLine();
                ((CPolyLine*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }else if(val==strCell){
			    pElement=new CCell();
                ((CCell*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
				pElement->m_pFather = this;
            }
        }	
    }
}

long CCell::Right()
{
	long lTemp=0;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
		pElement=(CElement *)(m_Elements.GetNext(pos));
		if(lTemp<pElement->Right()) lTemp=pElement->Right();
	}
	
	return lTemp;
}

bool CCell::RInR(LPCRECT lpRect)
{
	if(m_lStatus!=ST_NOMAL) return false;

	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(Left()-DELTA,Top()-DELTA,Right()+DELTA,Bottom()+DELTA))
        return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
}

void CCell::Save(MSXML::IXMLDOMElementPtr pRoot)
{
	MSXML::IXMLDOMDocumentPtr pDoc=pRoot->ownerDocument;

	//add a graphy cell
	MSXML::IXMLDOMElementPtr pCell=pDoc->createElement(L"go");;
    _variant_t val;
	val.vt=VT_BSTR;
	val.bstrVal=::SysAllocString(L"cell");
	pCell->setAttribute(L"type",val);
	pRoot->appendChild(pCell);

	//save all elements
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		pElement->Save(pCell);
	}	
}

long CCell::Top()
{
	long lTemp=0x7fffffff;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
		pElement=(CElement *)(m_Elements.GetNext(pos));
		if(lTemp>pElement->Top()) lTemp=pElement->Top();
	}
	
	return lTemp;
}

long CCell::Bottom()
{
	long lTemp=0;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
		pElement=(CElement *)(m_Elements.GetNext(pos));
		if(lTemp<pElement->Bottom()) lTemp=pElement->Bottom();
	}
	
	return lTemp;
}

void CCell::MoveInCell(POINT potTracker)
{
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){		
		pElement=(CElement *)(m_Elements.GetNext(pos));
		pElement->MoveInCell(potTracker);
	}
}

long CCell::GetLableTag(CListBox *pLabList,CListBox *pTagList)
{
	long lCount=0;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount+=((CCell*)pElement)->GetLableTag(pLabList,pTagList);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//button wnd
			/*
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pLabList->SetItemData(lCount, 1);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			*/
			// color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				pLabList->AddString(pDny->m_TagLable);			
				pTagList->AddString(pDny->m_TagName);
				lCount++;
			}
		}
	}

	return lCount;
}


long CCell::SetTag(CListBox *pTagList)
{
	long lCount=0;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount+=((CCell*)pElement)->SetTag(pTagList);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//button wnd
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				pTagList->GetText(lCount,pDny->m_TagName);
				lCount++;
			}
		}
	}

	return lCount;
}


long CCell::GetDynTags(TAG_NAME *pFatherTags, bool bFake,long lFatherCount)
{
    TAG_NAME *pTags;
	long lCount=lFatherCount;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->GetDynTags(pFatherTags,bFake,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button wnd
			//pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			//if(pDny->m_Used){
			//	if(!bFake){
			//		tagname_by_text(pTags,pDny->m_TagName.GetBuffer(TAGNAME_TEXT_LENGTH));
			//		pDny->m_TagName.ReleaseBuffer();
			//		pTags++;
			//	}
			//	lCount++;
			//}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				if(!bFake){
                    pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
		}
	}

	return lCount;
}

CElement * CCell::GetSelElement()
{
	if(m_pSelElement==NULL) return NULL;

	if(m_pSelElement->IsKindOf(RUNTIME_CLASS(CCell))){
		return ((CCell*)m_pSelElement)->GetSelElement();
	}else{
		return m_pSelElement;
	}
}

long CCell::SetDynValues(pmc_value_t *pFatherValues,long lFatherCount)
{
    pmc_value_t *pValues;
	long lCount=lFatherCount;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->SetDynValues(pFatherValues,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
		}
	}

	return lCount;
}

void CCell::UpdateView()
{
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_Elements.GetNext(pos));
		pElement->UpdateView();
	}
}
