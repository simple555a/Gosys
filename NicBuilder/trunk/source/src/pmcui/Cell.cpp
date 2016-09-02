// Cell.cpp: implementation of the CCell class.
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
	if(m_lStatus!=ST_NOMAL) return;

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
	    UPtoLP(Left()-DELTA,Top()-DELTA,&x1,&y1);
	    UPtoLP(Right()+DELTA,Bottom()+DELTA,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		MoveInCell(m_potTracker);
	    UPtoLP(Left()-DELTA,Top()-DELTA,&x1,&y1);
	    UPtoLP(Right()+DELTA,Bottom()+DELTA,&x2,&y2);
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
		pot.x=-Left()%right;
		pot.y=-Top()%bottom;
		MoveInCell(pot);
		break;
	}

}

long CCell::HitTest(long x, long y)
{
	if(m_locked){
			return TK_NONE;
	}
	if(m_lStatus!=ST_NOMAL) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	rtTemp.SetRect(Left()-DELTA,Top()-DELTA,Right()+DELTA,Bottom()+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_MOV;

	return TK_NONE;
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
    //装载子元素
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
            }else if(val==strArc){
			    pElement=new CArc();
                ((CArc*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strEllipse){
			    pElement=new CEllipse();
                ((CEllipse*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strLine){
			    pElement=new CLine();
                ((CLine*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strMCButton){
			    pElement=new CMCButton();
                ((CMCButton*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strMeta){
			    pElement=new CMeta();
                ((CMeta*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strRoundRect){
			    pElement=new CRoundRect();
                ((CRoundRect*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strText){
			    pElement=new CText();
                ((CText*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strPolygon){
			    pElement=new CPolygon();
                ((CPolygon*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
           }else if(val==strPolyLine){
			    pElement=new CPolyLine();
                ((CPolyLine*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
            }else if(val==strCell){
			    pElement=new CCell();
                ((CCell*)pElement)->Load(pItem);
                m_Elements.AddTail(pElement);
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
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;

    /*
	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(Left()-DELTA,Top()-DELTA,Right()+DELTA,Bottom()+DELTA))
        return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
    */

    // 原来的相交选取改为现在的包含选取
	
    POINT pt1, pt2, pt3, pt4;
    pt1.x = Left();
    pt1.y = Top();
    pt2.x = Left();
    pt2.y = Bottom();
    pt3.x = Right();
    pt3.y = Top();
    pt4.x = Right();
    pt4.y = Bottom();

    CRect rectTmp(lpRect->left-1,lpRect->top-1,lpRect->right+1,lpRect->bottom+1);
    rectTmp.NormalizeRect();
    
    if( rectTmp.PtInRect(pt1) && rectTmp.PtInRect(pt2) && rectTmp.PtInRect(pt3) && rectTmp.PtInRect(pt4) ){
        return true;
    }else{
        return false;
    }

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

long CCell::GetLableTag(CListBox *pLabList,long lFatherCount)
{
    CString sTemp;
	long lCount=lFatherCount;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->GetLableTag(pLabList,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//button wnd
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				// inform this is not a tag
				pLabList->SetItemData(lCount, 1);
				lCount++;
			}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
                sTemp.Format("%-30s%s",pDny->m_TagLable,pDny->m_TagName);
                pLabList->AddString(sTemp);
				lCount++;
			}
		}
	}

	return lCount;
}


long CCell::SetTag(CListBox *pTagList,long lFatherCount)
{
    CString sTemp;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
    long lCount=lFatherCount;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->SetTag(pTagList,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//button wnd
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				pTagList->GetText(lCount,sTemp);
                pDny->m_TagName=sTemp.Mid(30);
				lCount++;
			}
		}
	}

	return lCount;
}

long CCell::GetLevel()
{
	return 1;
}

int CCell::GetDynCount()
{
    int nCount=0;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_Elements.GetNext(pos));
        if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
            nCount+=((CCell*)pElement)->GetDynCount();
        }else{
            nCount++;
        }
	}

    return nCount;
}
