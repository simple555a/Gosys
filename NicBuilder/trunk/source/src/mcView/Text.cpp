// Text.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Text.h"
#include "wingdi.h"

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
	long x1,y1,x2,y2;
	if(m_DnyAttr.m_Linked && m_DnyAttr.m_na){
		CPen *pop=(CPen*)(pDC->SelectStockObject(BLACK_PEN));
		CBrush *pob=(CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
		UPtoLP(m_lox1,m_loy1,&x1,&y1);
		UPtoLP(m_lox2,m_loy2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		/*
		pDC->MoveTo(x1, y1);
		pDC->LineTo(x1 + 10, y1 + 10);
		pDC->MoveTo(x1 + 10, y1);
		pDC->LineTo(x1, y1 + 10);
		*/
		pDC->SelectObject(pop);
		pDC->SelectObject(pob);
		pDC->DrawIcon(x1,y1,ghNA);
		return;
	}

	if(m_DnyAttr.m_Linked && !m_DnyAttr.m_vi) return;

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

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_FaceColor); 

	UPtoLP(m_lx1,m_ly1,&x1,&y1);

	CString sTemp;
	sTemp=m_text;
	if(m_DnyAttr.m_ValueA.m_Used){
		pmc_value_t d;
		set_value_type(d.Flags, dt_real4);
		pmc_type_cast(&m_DnyAttr.m_ValueA.m_TagValue, &d);
		sTemp.Format(m_DnyAttr.m_ValueA.m_Format, d.Value.flt);
	}else if(m_DnyAttr.m_ValueD.m_Used){
		if(m_DnyAttr.m_ValueD.action_proc && m_DnyAttr.m_ValueD.action_proc != (void*)-1){
			char * s;
			s = ((char * (WINAPI*)())m_DnyAttr.m_ValueD.action_proc)();
			sTemp = s;
			free_buffer(s);
		}else {
			if(m_DnyAttr.m_ValueD.m_TagValue.Value.iValue){
				sTemp=m_DnyAttr.m_ValueD.m_OnMsg;
			}else{
				sTemp=m_DnyAttr.m_ValueD.m_OffMsg;
			}
		}
	}
	
	pDC->TextOut(x1,y1,sTemp);
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
    if(!m_DnyAttr.m_Linked) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	rtTemp.SetRect(Left(),Top(),Right(),Bottom());
	if(rtTemp.PtInRect(pt)) return TK_PUSH;

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
	if(m_lStatus!=ST_NOMAL) return false;

	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(m_lx1-DELTA,m_ly1-DELTA,m_lx2+DELTA,m_ly2+DELTA))
        return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
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

    //face color
    pAttr=pStatic->selectSingleNode(L"./color");
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_FaceColor=val.ulVal;
	m_oFaceColor=m_FaceColor;

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

void CText::MoveElement(POINT potOffset)
{
	m_lx1=m_lox1+potOffset.x;
	m_lx2=m_lox2+potOffset.x;
	m_ly1=m_loy1+potOffset.y;
	m_ly2=m_loy2+potOffset.y;
}

void CText::UpdateView()
{
	m_DnyAttr.m_na=false;

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
				m_FaceColor=m_DnyAttr.m_Blink.m_ColorPen;
			}else{
				m_FaceColor=m_oFaceColor;
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
	//color pen aa
	//color pen d
	//color pen da
	//color text a
	if(m_DnyAttr.m_ColorTextA.m_Used){
		if(m_DnyAttr.m_ColorTextA.m_TagValue.Flags & TF_Valid){
			__r4 r4 = getFloat(m_DnyAttr.m_ColorTextA.m_TagValue);
			if(r4 >m_DnyAttr.m_ColorTextA.m_Split1){
				m_FaceColor=m_DnyAttr.m_ColorTextA.m_Color0;
			}else if(r4>m_DnyAttr.m_ColorTextA.m_Split2){
				m_FaceColor=m_DnyAttr.m_ColorTextA.m_Color1;
			}else if(r4>m_DnyAttr.m_ColorTextA.m_Split3){
				m_FaceColor=m_DnyAttr.m_ColorTextA.m_Color2;
			}else if(r4>m_DnyAttr.m_ColorTextA.m_Split4){
				m_FaceColor=m_DnyAttr.m_ColorTextA.m_Color3;
			}else{
				m_FaceColor=m_DnyAttr.m_ColorTextA.m_Color4;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color text aa
	if(m_DnyAttr.m_ColorTextAA.m_Used){
		if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_HiHi){
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorH2;
			}else if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_Hi){
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorH1;
			}else if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_LoLo){
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorL2;
			}else if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_Lo){
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorL1;
			}else if(m_DnyAttr.m_ColorTextAA.m_TagValue.Flags & TF_Rate){
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorR;
			}else{
				m_FaceColor=m_DnyAttr.m_ColorTextAA.m_ColorN;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color text d
	if(m_DnyAttr.m_ColorTextD.m_Used){
		if(m_DnyAttr.m_ColorTextD.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorTextD.m_TagValue.Value.iValue){
				m_FaceColor=m_DnyAttr.m_ColorTextD.m_ColorOn;
			}else{
				m_FaceColor=m_DnyAttr.m_ColorTextD.m_ColorOff;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color text da
	if(m_DnyAttr.m_ColorTextDA.m_Used){
		if(m_DnyAttr.m_ColorTextDA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorTextDA.m_TagValue.Flags & TF_Off2On){
				m_FaceColor=m_DnyAttr.m_ColorTextDA.m_ColorAlarm;
			}else if(m_DnyAttr.m_ColorTextDA.m_TagValue.Flags & TF_On2Off){
				m_FaceColor=m_DnyAttr.m_ColorTextDA.m_ColorAlarm;
			}else{
				m_FaceColor=m_DnyAttr.m_ColorTextDA.m_ColorNomal;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//direction

	//fill h
	//fill v
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
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//size h
	//size v
	//value a
	if(m_DnyAttr.m_ValueA.m_Used){
		if(!(m_DnyAttr.m_ValueA.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_na=true;
		}
	}
	//value d
	if(m_DnyAttr.m_ValueD.m_Used){
		if(m_DnyAttr.m_ValueD.action_proc && m_DnyAttr.m_ValueD.action_proc != (void*)-1){
			m_DnyAttr.m_na = false;
		}else if(!(m_DnyAttr.m_ValueD.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_na=true;
		}
	}
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
