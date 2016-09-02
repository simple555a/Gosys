// Meta.cpp: implementation of the CMeta class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4018)
#include "Meta.h"
#include "Mcview.h"

#include <utils.h>
#pragma comment(lib, "libutils.lib")
#include <list>
#include <functional>
#include <librtk.h>
using namespace std;

extern COLORREF g_WindowColor;
static BOOL bTransparentMeta=FALSE;
static BOOL bFastMetaDraw=TRUE;

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
			utils_debug("Cache miss on %s(%d,%d)\n", file,r->right-r->left,r->bottom-r->top);
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
		debug();
		// utils_getc();
		Destroy();
	}
	void debug()
	{
		utils_debug(
			"Metafile cache : %d hits(%.3f), %d misses,cache size %d\n",
			m_iHits,
			(double)m_iHits / (m_iHits + m_iMiss),
			m_iMiss,
			c.size()
			);
	}
	void Destroy()
	{
		CCache::iterator it;
		for(it = c.begin(); it != c.end(); it++){
			delete (*it);
		}
		c.clear();
	}
public:
	struct CCacheEntry{
		string			file;
		HENHMETAFILE	handle;
		FILETIME		birth;
		CBitmap bm,*oldBm;
		int x, y;
		COLORREF transparentColor;
		CCacheEntry()
		{
			oldBm = 0;
			handle = 0;
		}
		
		CCacheEntry(CDC * theDc, const char * f, int x, int y);
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
				if(!bTransparentMeta){
					dc->BitBlt(r->left,r->top,abs(r->right-r->left),abs(r->bottom-r->top),&dc2,0,0,SRCCOPY);
				}else{
					#pragma comment(lib, "Msimg32.lib")
					BOOL ret;
					ret=TransparentBlt(
						dc->m_hDC,
						r->left,
						r->top,
						abs(r->right-r->left),
						abs(r->bottom-r->top),
						dc2.m_hDC,
						0,
						0,
						x,
						y,
						transparentColor
						);
					if(!ret){
						wperror("TransparentBlt");
						//DebugBreak();
					}
				}
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

CMetaCache::CCacheEntry::CCacheEntry(CDC * theDc, const char * f, int x, int y)
{
	CDC dc;
	extern CDC * currentStoredDc;

	handle = _open_metafile(f);
	this->x = x;
	this->y = y;
	file = f;
	if(!currentStoredDc){
		return;
	}
	if(!dc.CreateCompatibleDC(currentStoredDc)){
		wperror("CreateCompatibleDC");
		return;
	}
	bm.CreateCompatibleBitmap(currentStoredDc, x+8, y+8);
	oldBm=dc.SelectObject(&bm);
	dc.SetBkMode(TRANSPARENT);
	transparentColor=g_WindowColor;
	
	if(handle){
		CPen *pOldPen;
		pOldPen=(CPen *)dc.SelectStockObject(NULL_PEN);
		CBrush brushBack(g_WindowColor);
		CBrush *pOldBrush;
		
		pOldBrush=(CBrush *)dc.SelectObject(&brushBack); 
		dc.Rectangle(0,0,x+8,y+8);
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

//CMetaCache g_Cache;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

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
	long x1,y1,x2,y2;
	if(m_DnyAttr.m_Linked && m_DnyAttr.m_na){
		CPen *pop=(CPen*)(pDC->SelectStockObject(BLACK_PEN));
		CBrush *pob=(CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
		UPtoLP(m_lox1,m_loy1,&x1,&y1);
		UPtoLP(m_lox2,m_loy2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		pDC->SelectObject(pop);
		pDC->SelectObject(pob);
		pDC->DrawIcon(x1,y1,ghNA);
		return;
	}

	if(m_DnyAttr.m_Linked && !m_DnyAttr.m_vi) return;

	x1=m_lx1;
	y1=m_ly1;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);

	RECT rectBound;
	rectBound.left=x1;
	rectBound.top=y1;
	rectBound.right=x2;
	rectBound.bottom=y2;

	static int count;
	if(count++ == 500){
		bFastMetaDraw=GetPrivateProfileInt("pmc","FastMetaDraw",bFastMetaDraw,get_config_file());
		bTransparentMeta=GetPrivateProfileInt("pmc","TransparentMeta",bTransparentMeta,get_config_file());
		count=0;
		static int count2;
		if(count2++ == 10){
//			g_Cache.debug();
			count2=0;
		}
	}
	//if(!bFastMetaDraw){
		CString sPicFile;
		sPicFile=((CMcviewApp*)::AfxGetApp())->m_path+"pic\\"+m_sPicFile;
		m_hMeta=::GetEnhMetaFile(sPicFile);
		pDC->PlayMetaFile(m_hMeta,&rectBound);
		::DeleteEnhMetaFile(m_hMeta);
	//}else{
	//	g_Cache.Play(pDC, m_sPicFile, &rectBound);
	//}
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
		UPtoLP(m_lx1,m_lx1,&x1,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_TM:
		m_ly1=m_potTracker.y;
		UPtoLP(m_ly1,m_ly1,0,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RM:
		m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_lx2,&x2,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_BM:
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

long CMeta::HitTest(long x, long y)
{
    if(!m_DnyAttr.m_Linked) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	rtTemp.SetRect(m_lx1,m_ly1,m_lx2,m_ly2);
	if(rtTemp.PtInRect(pt)) return TK_PUSH;

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
		break;
	}
}

bool CMeta::RInR(LPCRECT lpRect)
{
	if(m_lStatus!=ST_NOMAL) return false;

	CRgn rgnTmp;
	if(!rgnTmp.CreateRectRgn(m_lx1,m_ly1,m_lx2,m_ly2)) return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
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

void CMeta::MoveElement(POINT potOffset)
{
	m_lx1=m_lox1+potOffset.x;
	m_lx2=m_lox2+potOffset.x;
	m_ly1=m_loy1+potOffset.y;
	m_ly2=m_loy2+potOffset.y;
}

void CMeta::UpdateView()
{
	m_DnyAttr.m_na=false;

	float fp;
	long lp;

	//blink
	//color brush a
	//color brush aa
	//color brush d
	//color brush da
	//color pen a
	//color pen aa
	//color pen d
	//color pen da
	//color text a
	//color text aa
	//color text d
	//color text da
	//direction

	//fill h
	//fill v
	long ltx1=m_lox1;
	long ltx2=m_lox2;
	long lty1=m_loy1;
	long lty2=m_loy2;
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
			ltx1=m_lx1;
			ltx2=m_lx2;
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
			lty1=m_ly1;
			lty2=m_ly2;
		}else{
			m_DnyAttr.m_na=true;
		}
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
				m_lx2=m_lx1+(m_lox2-m_lox1)*fp;
			}else if(m_DnyAttr.m_SizeH.m_Ref==1){
				m_lx1=(ltx2+ltx1)/2-(m_lox2-m_lox1)*fp/2.;
				m_lx2=(ltx2+ltx1)/2+(m_lox2-m_lox1)*fp/2.;
			}else if(m_DnyAttr.m_SizeH.m_Ref==2){
				m_lx1=m_lx2-(m_lox2-m_lox1)*fp;
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
				m_ly2=m_ly1+(m_loy2-m_loy1)*fp;
			}else if(m_DnyAttr.m_SizeV.m_Ref==1){
				m_ly1=(lty1+lty2)/2-(m_loy2-m_loy1)*fp/2.;
				m_ly2=(lty1+lty2)/2+(m_loy2-m_loy1)*fp/2.;
			}else if(m_DnyAttr.m_SizeV.m_Ref==2){
				m_ly1=m_ly2-(m_loy2-m_loy1)*fp;
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

