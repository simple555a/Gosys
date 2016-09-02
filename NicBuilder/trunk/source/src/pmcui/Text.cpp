// Text.cpp: implementation of the CText class.
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
IMPLEMENT_DYNAMIC(CText,CElement)

CText::CText()
{
	m_bHavePen=false;
	m_bHaveBrush=false;
	m_bHaveFont=true;
	/*
	m_font.lfCharSet=DEFAULT_CHARSET;
	m_font.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	m_font.lfEscapement=0;
	//m_font.lfFaceName="宋体";
	strcpy(m_font.lfFaceName,"宋体");
	m_font.lfHeight=360;
	m_font.lfItalic=false;
	m_font.lfOrientation=0;
	m_font.lfOutPrecision=OUT_DEFAULT_PRECIS;
	m_font.lfPitchAndFamily=DEFAULT_PITCH;
	m_font.lfQuality=DEFAULT_QUALITY;
	m_font.lfStrikeOut=false;
	m_font.lfUnderline=false;
	m_font.lfWeight=400;
	m_font.lfWidth=150;
	*/
}

CText::~CText()
{

}

void CText::Draw(CDC *pDC)
{
	if(m_lStatus!=ST_NOMAL) return;

	long x1,y1;
	LOGFONT tFont;
	tFont=m_font;
	UPtoLP(tFont.lfHeight,tFont.lfWidth,&tFont.lfHeight,&tFont.lfWidth);
	CFont font;
	font.CreateFontIndirect(&tFont);
	CFont *pOldFont=pDC->SelectObject(&font);
	
	int i,iWidth;
	long lTemp,lUWidth=0;
	UINT nTemp;
	i=0;
	while(i<m_text.GetLength()){
		nTemp=(UCHAR)m_text[i];
		if(IsDBCSLeadByte(nTemp)){
			nTemp=nTemp*256+(UCHAR)m_text[i];
			i++;
		}
		::GetCharWidth(pDC->GetSafeHdc(),nTemp,nTemp,&iWidth);
		//CMeatFileDC没有属性DC，下面这句话就不灵了。
		//pDC->GetCharWidth(nTemp,nTemp,&iWidth);
		CElement::LPtoUP(iWidth,iWidth,&lTemp,0);
		lUWidth+=lTemp;
		i++;
	}

    if(m_font.lfEscapement==0)
	    m_lx2=m_lx1+lUWidth;
    else if(m_font.lfEscapement==900)
        m_ly2=m_ly1-lUWidth;
    else if(m_font.lfEscapement==1800)
        m_lx2=m_lx1-lUWidth;
    else if(m_font.lfEscapement==2700)
        m_ly2=m_ly1+lUWidth;


	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_FaceColor); 

	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	pDC->TextOut(x1,y1,m_text);
	
	//pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);
}

void CText::DrawTracker(CDC *pDC)
{
	LOGPEN tLogPen;
	tLogPen.lopnColor=RGB(0,0,0);
	tLogPen.lopnStyle=PS_DOT;
	tLogPen.lopnWidth.x=1;
	CPen tPen;
	tPen.CreatePenIndirect(&tLogPen);
	CPen *pOldPen=(CPen *)pDC->SelectObject(&tPen);
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	LOGFONT tFont;
	tFont=m_font;
	UPtoLP(tFont.lfHeight,tFont.lfWidth,&tFont.lfHeight,&tFont.lfWidth);
	CFont font;
	font.CreateFontIndirect(&tFont);
	CFont *pOldFont=pDC->SelectObject(&font);
	pDC->SetROP2(R2_NOTXORPEN);

	long x1,y1,x2,y2;
//	UPtoLP(m_lx1,m_ly1,&x1,&y1);
//	UPtoLP(m_lx2,m_ly2,&x2,&y2);

//	long ly1,ly2;
	switch(m_lHitTarget){
	case TK_CT:
		//pDC->SetTextColor(m_FaceColor); 
		//pDC->TextOut(x1,y1,m_text);
		break;
/*
	case TK_TM:
		ly1=m_potTracker.y;
		if((m_ly2-ly1)>=2*DELTA){
			UPtoLP(m_lx1-DELTA,m_ly1-DELTA,&x1,&y1);
			UPtoLP(m_lx2+DELTA,m_ly2+DELTA,&x2,&y2);
			pDC->Rectangle(x1,y1,x2,y2);
			m_ly1=ly1+DELTA;
			m_font.lfHeight=m_ly2-m_ly1;
			UPtoLP(m_lx1-DELTA,m_ly1-DELTA,&x1,&y1);
			pDC->Rectangle(x1,y1,x2,y2);
		}
		break;
	case TK_BM:
		ly2=m_potTracker.y;
		if((ly2-m_ly1)>=2*DELTA){
			UPtoLP(m_lx1-DELTA,m_ly1-DELTA,&x1,&y1);
			UPtoLP(m_lx2+DELTA,m_ly2+DELTA,&x2,&y2);
			pDC->Rectangle(x1,y1,x2,y2);
			m_ly2=ly2-DELTA;
			m_font.lfHeight=m_ly2-m_ly1;
			UPtoLP(m_lx2+DELTA,m_ly2+DELTA,&x2,&y2);
			pDC->Rectangle(x1,y1,x2,y2);
		}
		break;
*/
	case TK_MOV:
		UPtoLP(Left()-DELTA,Top()-DELTA,&x1,&y1);
		UPtoLP(Right()+DELTA,Bottom()+DELTA,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		m_lx1+=m_potTracker.x;
		m_ly1+=m_potTracker.y;
		m_lx2+=m_potTracker.x;
		m_ly2+=m_potTracker.y;
		UPtoLP(Left()-DELTA,Top()-DELTA,&x1,&y1);
		UPtoLP(Right()+DELTA,Bottom()+DELTA,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	}
	
	//pDC->SetBkMode(OPAQUE);
	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

long CText::HitTest(long x, long y)
{
	if(m_locked){
			return TK_NONE;
	}
	if(m_lStatus!=ST_NOMAL) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;
/*
	rtTemp.SetRect((m_lx1+m_lx2)/2-DELTA,m_ly1-2*DELTA,(m_lx1+m_lx2)/2+DELTA,m_ly1);
	if(rtTemp.PtInRect(pt)) return TK_TM;

	rtTemp.SetRect((m_lx1+m_lx2)/2-DELTA,m_ly2,(m_lx1+m_lx2)/2+DELTA,m_ly2+2*DELTA);
	if(rtTemp.PtInRect(pt)) return TK_BM;
*/
	rtTemp.SetRect(Left()-DELTA,Top()-DELTA,Right()+DELTA,Bottom()+DELTA);
	if(rtTemp.PtInRect(pt)) return TK_MOV;

	return TK_NONE;
}

void CText::DrawHotPoints(CDC *pDC)
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
/*
	pDC->SelectStockObject(GRAY_BRUSH);
	pDC->SelectStockObject(NULL_PEN);
	pDC->SetROP2(R2_NOT);

	long x5,y5,x6,y6;
	UPtoLP((m_lx1+m_lx2)/2,m_ly1-DELTA,&x5,&y5);
	UPtoLP((m_lx1+m_lx2)/2,m_ly2+DELTA,&x6,&y6);
	DrawHotPoint(pDC,x5,y5);
	DrawHotPoint(pDC,x6,y6);
*/
	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);
}

bool CText::RInR(LPCRECT lpRect)
{
	if(m_locked){
			return false;
	}
	if(m_lStatus!=ST_NOMAL) return false;

    /*
	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(m_lx1-DELTA,m_ly1-DELTA,m_lx2+DELTA,m_ly2+DELTA))
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

long CText::Left()
{
	if(m_lx1<m_lx2) return m_lx1;
	else return m_lx2;
}

long CText::Right()
{
	if(m_lx1<m_lx2) return m_lx2;
	else return m_lx1;
}

long CText::Top()
{
	if(m_ly1<m_ly2) return m_ly1;
	else return m_ly2;
}

long CText::Bottom()
{
	if(m_ly1<m_ly2) return m_ly2;
	else return m_ly1;
}

void CText::Format(long left, long top, long right, long bottom, long flag)
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
        m_font.lfEscapement+=2700;
        m_font.lfEscapement%=3600;
		break;
	case FM_R2U:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2U(ldx,ldy,&m_lx1,&m_ly1);
		R2U(ldx,ldy,&m_lx2,&m_ly2);
        m_font.lfEscapement+=900;
        m_font.lfEscapement%=3600;
		break;
	case FM_F2Z:
		ldx=(left+right)/2;
		F2Z(ldx,&m_lx1);
		F2Z(ldx,&m_lx2);
		if(m_lx1>m_lx2){
			ldx=m_lx1;
			m_lx1=m_lx2;
			m_lx2=m_lx1;
		}
		break;
	case FM_F2I:
		ldy=(top+bottom)/2;
		F2I(ldy,&m_ly1);
		F2I(ldy,&m_ly2);
		if(m_ly1>m_ly2){
			ldy=m_ly1;
			m_ly1=m_ly2;
			m_ly2=ldy;
		}
		break;
	case FM_S2G:
		ldx=m_lx2-m_lx1;
		ldy=m_ly2-m_ly1;
		m_lx1=(m_lx1+right/2)-(m_lx1+right/2)%right;
		m_lx2=m_lx1+ldx;
		m_ly1=(m_ly1+bottom/2)-(m_ly1+bottom/2)%bottom;
		m_ly2=m_ly1+ldy;
		break;
	}
}

CElement * CText::Clone(long lCmdId, long lStatus)
{
	CText * pNew=new CText();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	pNew->m_FaceColor=m_FaceColor;
	pNew->m_font=m_font;
	pNew->m_text=m_text;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CText::Save(MSXML::IXMLDOMElementPtr pRoot)
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
    val.bstrVal=::SysAllocString(L"text");
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

    //face color
    pAttr=pDoc->createElement("color");
    pStatic->appendChild(pAttr);
    //color
    val.vt=VT_UI4;
    val.ulVal=m_FaceColor;
    pAttr->setAttribute(L"c",val);

    //font
    pAttr=pDoc->createElement("font");
    pStatic->appendChild(pAttr);
    //lfHeight
    val.vt=VT_I4;
    val.lVal=m_font.lfHeight;
    pAttr->setAttribute(L"h",val);
    //lfWidth
    val.vt=VT_I4;
    val.lVal=m_font.lfWidth;
    pAttr->setAttribute(L"wd",val);
    //lfEscapement
    val.vt=VT_I4;
    val.lVal=m_font.lfEscapement;
    pAttr->setAttribute(L"e",val);
    //lfOrientation
    val.vt=VT_I4;
    val.lVal=m_font.lfOrientation;
    pAttr->setAttribute(L"o",val);
    //lfWeight
    val.vt=VT_I4;
    val.lVal=m_font.lfWeight;
    pAttr->setAttribute(L"wg",val);
    //lfItalic
    val.vt=VT_UI1;
    val.bVal=m_font.lfItalic;
    pAttr->setAttribute(L"i",val);
    //lfUnderline
    val.vt=VT_UI1;
    val.bVal=m_font.lfUnderline;
    pAttr->setAttribute(L"u",val);
    //lfStrikeOut
    val.vt=VT_UI1;
    val.bVal=m_font.lfStrikeOut;
    pAttr->setAttribute(L"so",val);
    //lfCharSet
    val.vt=VT_UI1;
    val.bVal=m_font.lfCharSet;
    pAttr->setAttribute(L"cs",val);
    //lfOutPrecision
    val.vt=VT_UI1;
    val.bVal=m_font.lfOutPrecision;
    pAttr->setAttribute(L"op",val);
    //lfClipPrecision
    val.vt=VT_UI1;
    val.bVal=m_font.lfClipPrecision;
    pAttr->setAttribute(L"cp",val);
    //lfQuality
    val.vt=VT_UI1;
    val.bVal=m_font.lfQuality;
    pAttr->setAttribute(L"q",val);
    //lfPitchAndFamily
    val.vt=VT_UI1;
    val.bVal=m_font.lfPitchAndFamily;
    pAttr->setAttribute(L"paf",val);
    //lfFaceName
    CString strFaceName=m_font.lfFaceName;
    //strFaceName.Format("%s",m_font.lfFaceName);
    val.vt=VT_BSTR;
    val.bstrVal=strFaceName.AllocSysString();
    pAttr->setAttribute(L"fn",val);

    //text
    pAttr=pDoc->createElement("text");
    pStatic->appendChild(pAttr);
    val.vt=VT_BSTR;
    val.bstrVal=m_text.AllocSysString();
    pAttr->setAttribute(L"t",val);

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

void CText::Load(MSXML::IXMLDOMElementPtr pGO)
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

    //face color
    pAttr=pStatic->selectSingleNode(L"./color");
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_FaceColor=val.ulVal;

    //font
    pAttr=pStatic->selectSingleNode(L"./font");
    //lfHeight
    val=pAttr->getAttribute(L"h");
    val.ChangeType(VT_I4);
    m_font.lfHeight=val.ulVal;
    //lfWidth
    val=pAttr->getAttribute(L"wd");
    val.ChangeType(VT_I4);
    m_font.lfWidth=val.ulVal;
    //lfEscapement
    val=pAttr->getAttribute(L"e");
    val.ChangeType(VT_I4);
    m_font.lfEscapement=val.ulVal;
    //lfOrientation
    val=pAttr->getAttribute(L"o");
    val.ChangeType(VT_I4);
    m_font.lfOrientation=val.ulVal;
    //lfWeight
    val=pAttr->getAttribute(L"wg");
    val.ChangeType(VT_I4);
    m_font.lfWeight=val.ulVal;
    //lfItalic
    val=pAttr->getAttribute(L"i");
    val.ChangeType(VT_UI1);
    m_font.lfItalic=val.ulVal;
    //lfUnderline
    val=pAttr->getAttribute(L"u");
    val.ChangeType(VT_UI1);
    m_font.lfUnderline=val.ulVal;
    //lfStrikeOut
    val=pAttr->getAttribute(L"so");
    val.ChangeType(VT_UI1);
    m_font.lfStrikeOut=val.ulVal;
    //lfCharSet
    val=pAttr->getAttribute(L"cs");
    val.ChangeType(VT_UI1);
    m_font.lfCharSet=val.ulVal;
    //lfOutPrecision
    val=pAttr->getAttribute(L"op");
    val.ChangeType(VT_UI1);
    m_font.lfOutPrecision=val.ulVal;
    //lfClipPrecision
    val=pAttr->getAttribute(L"cp");
    val.ChangeType(VT_UI1);
    m_font.lfClipPrecision=val.ulVal;
    //lfQuality
    val=pAttr->getAttribute(L"q");
    val.ChangeType(VT_UI1);
    m_font.lfQuality=val.ulVal;
    //lfPitchAndFamily
    val=pAttr->getAttribute(L"paf");
    val.ChangeType(VT_UI1);
    m_font.lfPitchAndFamily=val.ulVal;
    //lfFaceName
    val=pAttr->getAttribute(L"fn");
    CString strFaceName=val.bstrVal;
    sprintf(m_font.lfFaceName,"%s",(LPCTSTR)strFaceName);

    //text
    pAttr=pStatic->selectSingleNode(L"./text");
    val=pAttr->getAttribute(L"t");
    m_text=val.bstrVal;

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

void CText::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
}
