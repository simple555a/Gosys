// Meta.cpp: implementation of the CMeta class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4018)
#include "stdafx.h"
#pragma hdrstop
#include <utils.h>
#pragma comment(lib, "libutils.lib")
#include <list>
#include <functional>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <utils.h>
#pragma comment(lib, "libutils.lib")
#include <list>
#include <functional>
#include <librtk.h>
using namespace std;

__declspec(dllexport) COLORREF g_WindowColor;
__declspec(dllexport) BOOL g_bPmcUiIsPrinting;

static HENHMETAFILE _stdcall _open_metafile(const char * fileName)
{
	CString sPicFile;
	HENHMETAFILE h;
	
	sPicFile = CString(get_working_dir()) + "pic\\" + fileName;
	h = ::GetEnhMetaFile(sPicFile);
	if(!h){
		sPicFile = CString(get_exe_dir()) + "pic\\" + fileName;
		h = ::GetEnhMetaFile(sPicFile);
	}
	return h;
}

class CMetaCache{
public:
	void Play(CDC *dc, const char * file, const RECT * r)
	{
		CCache::iterator it;
		CCacheEntry * e;
		it = FindEntry(file, r);
		if(it == c.end()){
			m_iMiss++;
			it = CacheEntry(file, r);
			utils_debug("Cache miss on %s\n", file);
		}else{
			m_iHits++;
		}
		if(it != c.end()){
			// dc->PlayMetaFile(it->handle, r);
			e=*it;
			e->Play(dc, r);
		}
	}
	void InvalidateCache()
	{
		Destroy();
	}
	CMetaCache()
	{
		m_iMiss = m_iHits = 0;
		maxCacheItems = GetPrivateProfileInt(
			"MCVIEW",
			"MetaCacheSize",
			MAX_ITEMS,
			get_config_file()
			);
		if(maxCacheItems < 32 || maxCacheItems > 256){
			utils_error("MetaCache=%d invalid, set to %d\n", maxCacheItems, MAX_ITEMS);
			maxCacheItems = MAX_ITEMS;
		}
		c.clear();
	}
	~CMetaCache()
	{
		utils_trace(
			"Metafile cache : %d hits(%.3f), %d misses\n",
			m_iHits,
			(double)m_iHits / (m_iHits + m_iMiss),
			m_iMiss
			);
		// utils_getc();
		Destroy();
	}
	void Destroy()
	{
		CCache::iterator it;
		for(it = c.begin(); it != c.end(); it++){
			delete (*it);
		}
		c.clear();
	}
protected:
	struct CCacheEntry{
		string			file;
		HENHMETAFILE	handle;
		FILETIME		birth;
		CBitmap bm,*oldBm;
		int x, y;
		CCacheEntry()
		{
			oldBm = 0;
			handle = 0;
		}
		
		CCacheEntry(CDC * theDc, const char * f, int x, int y)
		{
			CDC dc;
			HDC tmpDc;

			handle = _open_metafile(f);
			this->x = x;
			this->y = y;
			file = f;
			tmpDc=GetDC(0);
			if(!dc.CreateCompatibleDC(CDC::FromHandle(tmpDc))){
				wperror("CreateCompatibleDC");
				ReleaseDC(0,tmpDc);
				return;
			}
			bm.CreateCompatibleBitmap(CDC::FromHandle(tmpDc), x, y);
			ReleaseDC(0,tmpDc);
			oldBm=dc.SelectObject(&bm);
			dc.SetBkMode(TRANSPARENT);
			
			if(handle){
				CPen *pOldPen;
				pOldPen=(CPen *)dc.SelectStockObject(NULL_PEN);
				CBrush brushBack(g_WindowColor);
				CBrush *pOldBrush;
				
				pOldBrush=(CBrush *)dc.SelectObject(&brushBack); 
				dc.Rectangle(0,0,x,y);
				dc.SelectStockObject(WHITE_BRUSH);
				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
				dc.SetROP2(R2_COPYPEN);
				
				RECT rr;
				memset(&rr, 0, sizeof(rr));	
				rr.right = x;
				rr.bottom = y;
				dc.PlayMetaFile(handle, &rr);
				dc.SelectObject(oldBm);
				oldBm=0;
				DeleteEnhMetaFile(handle);
				handle=0;
			}
		}
		~CCacheEntry()
		{
			if(handle){
				DeleteEnhMetaFile(handle);
			}
		}
		void Play(CDC * dc, const RECT * r)
		{
			if(handle){
				dc->PlayMetaFile(handle, r);
			}else{
				CDC dc2;
				dc2.CreateCompatibleDC(dc);
				CBitmap * bm2;
				bm2=dc2.SelectObject(&bm);
				dc->BitBlt(r->left,r->top,abs(r->right-r->left),abs(r->bottom-r->top),&dc2,0,0,SRCCOPY);
				dc2.SelectObject(bm2);

			}
		}
	};
	typedef list<CCacheEntry*> CCache;
	CCache c;
	int maxCacheItems;
	enum{
		MAX_ITEMS = 64,
	};
	CCache::iterator FindEntry(const char * f, const RECT * r)
	{
		CCache::iterator it;
		CCacheEntry * e;
		int x, y;
		x = abs(r->right-r->left);
		y = abs(r->bottom-r->top);
		for(it=c.begin(); it != c.end(); it++){
			e=*it;
			if(!stricmp(e->file.data(), f) && x==e->x && y==e->y){
				return it;
			}
		}
		return it;
	}
	CCache::iterator CacheEntry(const char * f, const RECT * r)
	{
		CCache::iterator it;
		CCacheEntry * e;
		int x, y;
		x = abs(r->right-r->left);
		y = abs(r->bottom-r->top);
		e = new CCacheEntry(0, f, x, y);
		if(!e->handle && !(HBITMAP)e->bm){
			delete e;
			return c.end();
		}
		if(c.size() >= maxCacheItems){
			CCacheEntry * ee;
			ee=c.back();
			delete ee;
			c.pop_back();
		}
		c.push_front(e);
		return c.begin();
	}
	int	m_iHits;
	int	m_iMiss;
};

CMetaCache g_Cache;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMeta,CElement)

CMeta::CMeta()
{
	m_bHavePen=false;
	m_bHaveBrush=false;
	m_bHaveFont=false;
}

CMeta::~CMeta()
{
	::DeleteEnhMetaFile(m_hMeta);
}

long CMeta::Left()
{
	return m_lx1;
}

long CMeta::Right()
{
	return m_lx2;
}

long CMeta::Top()
{
	return m_ly1;
}

long CMeta::Bottom()
{
	return m_ly2;
}

void CMeta::Draw(CDC *pDC)
{
	if(m_lStatus!=ST_NOMAL) return;

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

	RECT rectBound;
	rectBound.left=x1;
	rectBound.top=y1;
	rectBound.right=x2;
	rectBound.bottom=y2;

	/*
	*/
	if(g_bPmcUiIsPrinting){
		m_hMeta=_open_metafile(m_sPicFile);
		pDC->PlayMetaFile(m_hMeta,&rectBound);
		::DeleteEnhMetaFile(m_hMeta);
	}else{
		g_Cache.Play(pDC, m_sPicFile, &rectBound);
	}
}

void CMeta::DrawHotPoints(CDC *pDC)
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

void CMeta::DrawTracker(CDC *pDC)
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

long CMeta::HitTest(long x, long y)
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

void CMeta::Format(long left, long top, long right, long bottom, long flag)
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

bool CMeta::RInR(LPCRECT lpRect)
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

CElement * CMeta::Clone(long lCmdId, long lStatus)
{
	CMeta * pNew=new CMeta();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	//pNew->m_hMeta=m_hMeta;
	pNew->m_sPicFile=m_sPicFile;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CMeta::Save(MSXML::IXMLDOMElementPtr pRoot)
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
    val.bstrVal=::SysAllocString(L"meta");
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

    //picture file
    pAttr=pDoc->createElement("file");
    pStatic->appendChild(pAttr);
    val.vt=VT_BSTR;
    val.bstrVal=m_sPicFile.AllocSysString();
    pAttr->setAttribute(L"path",val);

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

void CMeta::Load(MSXML::IXMLDOMElementPtr pGO)
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
    //file
    pAttr=pStatic->selectSingleNode(L"./file");
    //path
    val=pAttr->getAttribute(L"path");
    m_sPicFile=val.bstrVal;

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

void CMeta::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
}
