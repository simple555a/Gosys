// MCDrawView.cpp : implementation of the CMCDrawView class
//

#include "stdafx.h"
#include "MCDraw.h"
#include "MainFrm.h"
#include "OptWindow.h"
#include "OptShow.h"
#include "OptGrid.h"

#include "DiaDnyAttr.h"
#include "DiaCellAttr.h"

#include "MCDrawDoc.h"
#include "MCDrawView.h"

#include "diaselect.h"
#include "diasave.h"
#include "diareplace.h"
#include "diatrendattr.h"

#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) COLORREF g_WindowColor;
extern __declspec(dllimport) BOOL g_bPmcUiIsPrinting;

extern CMCDrawApp theApp;
#pragma warning(disable:4800)
/////////////////////////////////////////////////////////////////////////////
// CMCDrawView

IMPLEMENT_DYNCREATE(CMCDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CMCDrawView, CScrollView)
	//{{AFX_MSG_MAP(CMCDrawView)
	ON_COMMAND(ID_DRAW_ARC, OnDrawArc)
	ON_COMMAND(ID_DRAW_BUTTON, OnDrawButton)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_COMMAND(ID_DRAW_LIBCOMP, OnDrawLibcomp)
	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
	ON_COMMAND(ID_DRAW_PICURE, OnDrawPicure)
	ON_COMMAND(ID_DRAW_POINTER, OnDrawPointer)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_COMMAND(ID_DRAW_POLYLINE, OnDrawPolyline)
	ON_COMMAND(ID_DRAW_RECTANGLE, OnDrawRectangle)
	ON_COMMAND(ID_DRAW_ROUNDRECT, OnDrawRoundrect)
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARC, OnUpdateDrawArc)
	ON_UPDATE_COMMAND_UI(ID_DRAW_BUTTON, OnUpdateDrawButton)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LIBCOMP, OnUpdateDrawLibcomp)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_PICURE, OnUpdateDrawPicure)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POINTER, OnUpdateDrawPointer)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYLINE, OnUpdateDrawPolyline)

	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, OnUpdateDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDRECT, OnUpdateDrawRoundrect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, OnUpdateDrawText)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_COLOR_BRUSH, OnColorBrush)
	ON_COMMAND(ID_COLOR_FONT, OnColorFont)
	ON_COMMAND(ID_COLOR_PEN, OnColorPen)
	ON_COMMAND(ID_COLOR_WINDOW, OnColorWindow)
	ON_COMMAND(ID_LINE_1, OnLine1)
	ON_COMMAND(ID_LINE_2, OnLine2)
	ON_COMMAND(ID_LINE_3, OnLine3)
	ON_COMMAND(ID_LINE_4, OnLine4)
	ON_COMMAND(ID_LINE_5, OnLine5)
	ON_COMMAND(ID_LINE_6, OnLine6)
	ON_COMMAND(ID_LINE_DASH, OnLineDash)
	ON_COMMAND(ID_LINE_DASHDOT, OnLineDashdot)
	ON_COMMAND(ID_LINE_DASHDOTDOT, OnLineDashdotdot)
	ON_COMMAND(ID_LINE_DOT, OnLineDot)
	ON_COMMAND(ID_LINE_NULL, OnLineNull)
	ON_COMMAND(ID_LINE_SOLID, OnLineSolid)
	ON_COMMAND(ID_BRUSH_BDIAGONAL, OnBrushBdiagonal)
	ON_COMMAND(ID_BRUSH_CROSS, OnBrushCross)
	ON_COMMAND(ID_BRUSH_DIAGCROSS, OnBrushDiagcross)
	ON_COMMAND(ID_BRUSH_FDIAGONAL, OnBrushFdiagonal)
	ON_COMMAND(ID_BRUSH_HORIZONTAL, OnBrushHorizontal)
	ON_COMMAND(ID_BRUSH_NULL, OnBrushNull)
	ON_COMMAND(ID_BRUSH_SOLID, OnBrushSolid)
	ON_COMMAND(ID_BRUSH_VERTICAL, OnBrushVertical)
	ON_COMMAND(IDR_TEXT_SET, OnTextSet)
	ON_COMMAND(IDR_TEXT_BOLD, OnTextBold)
	ON_UPDATE_COMMAND_UI(IDR_TEXT_BOLD, OnUpdateTextBold)
	ON_COMMAND(IDR_TEXT_ITALIC, OnTextItalic)
	ON_UPDATE_COMMAND_UI(IDR_TEXT_ITALIC, OnUpdateTextItalic)
	ON_COMMAND(IDR_TEXT_UNDERLINE, OnTextUnderline)
	ON_UPDATE_COMMAND_UI(IDR_TEXT_UNDERLINE, OnUpdateTextUnderline)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_BDIAGONAL, OnUpdateBrushBdiagonal)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_CROSS, OnUpdateBrushCross)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_DIAGCROSS, OnUpdateBrushDiagcross)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_FDIAGONAL, OnUpdateBrushFdiagonal)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_HORIZONTAL, OnUpdateBrushHorizontal)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_NULL, OnUpdateBrushNull)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_SOLID, OnUpdateBrushSolid)
	ON_UPDATE_COMMAND_UI(ID_BRUSH_VERTICAL, OnUpdateBrushVertical)
	ON_UPDATE_COMMAND_UI(ID_LINE_1, OnUpdateLine1)
	ON_UPDATE_COMMAND_UI(ID_LINE_2, OnUpdateLine2)
	ON_UPDATE_COMMAND_UI(ID_LINE_3, OnUpdateLine3)
	ON_UPDATE_COMMAND_UI(ID_LINE_4, OnUpdateLine4)
	ON_UPDATE_COMMAND_UI(ID_LINE_5, OnUpdateLine5)
	ON_UPDATE_COMMAND_UI(ID_LINE_6, OnUpdateLine6)
	ON_UPDATE_COMMAND_UI(ID_LINE_DASH, OnUpdateLineDash)
	ON_UPDATE_COMMAND_UI(ID_LINE_DASHDOT, OnUpdateLineDashdot)
	ON_UPDATE_COMMAND_UI(ID_LINE_DASHDOTDOT, OnUpdateLineDashdotdot)
	ON_UPDATE_COMMAND_UI(ID_LINE_DOT, OnUpdateLineDot)
	ON_UPDATE_COMMAND_UI(ID_LINE_NULL, OnUpdateLineNull)
	ON_UPDATE_COMMAND_UI(ID_LINE_SOLID, OnUpdateLineSolid)
	ON_COMMAND(ID_FORMAT_A2B, OnFormatA2b)
	ON_COMMAND(ID_FORMAT_A2C, OnFormatA2c)
	ON_COMMAND(ID_FORMAT_A2CM, OnFormatA2cm)
	ON_COMMAND(ID_FORMAT_A2L, OnFormatA2l)
	ON_COMMAND(ID_FORMAT_A2M, OnFormatA2m)
	ON_COMMAND(ID_FORMAT_A2T, OnFormatA2t)
	ON_COMMAND(ID_FORMAT_B2B, OnFormatB2b)
	ON_COMMAND(ID_FORMAT_B2F, OnFormatB2f)
	ON_COMMAND(ID_FORMAT_E2H, OnFormatE2h)
	ON_COMMAND(ID_FORMAT_E2V, OnFormatE2v)
	ON_COMMAND(ID_FORMAT_F2I, OnFormatF2i)
	ON_COMMAND(ID_FORMAT_F2Z, OnFormatF2z)
	ON_COMMAND(ID_FORMAT_R2U, OnFormatR2u)
	ON_COMMAND(ID_FORMAT_R2W, OnFormatR2w)
	ON_COMMAND(ID_FORMAT_S2G, OnFormatS2g)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_B2B, OnUpdateFormatB2b)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_B2F, OnUpdateFormatB2f)
	ON_COMMAND(ID_FORMAT_A2R, OnFormatA2r)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2B, OnUpdateFormatA2b)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2C, OnUpdateFormatA2c)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2CM, OnUpdateFormatA2cm)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2L, OnUpdateFormatA2l)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2M, OnUpdateFormatA2m)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2R, OnUpdateFormatA2r)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_A2T, OnUpdateFormatA2t)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_E2H, OnUpdateFormatE2h)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_E2V, OnUpdateFormatE2v)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_F2I, OnUpdateFormatF2i)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_F2Z, OnUpdateFormatF2z)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_R2U, OnUpdateFormatR2u)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_R2W, OnUpdateFormatR2w)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_S2G, OnUpdateFormatS2g)
	ON_COMMAND(ID_TOOLS_GRID, OnToolsGrid)
	ON_COMMAND(ID_TOOLS_META, OnToolsMeta)
	ON_COMMAND(ID_TOOLS_MKCELL, OnToolsMkcell)
	ON_COMMAND(ID_TOOLS_UPCELL, OnToolsUpcell)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_META, OnUpdateToolsMeta)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MKCELL, OnUpdateToolsMkcell)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_UPCELL, OnUpdateToolsUpcell)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BRUSH, OnUpdateColorBrush)
	ON_UPDATE_COMMAND_UI(ID_COLOR_FONT, OnUpdateColorFont)
	ON_UPDATE_COMMAND_UI(ID_COLOR_PEN, OnUpdateColorPen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_WINDOW, OnUpdateColorWindow)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_TOOLS_SAVECELL, OnToolsSavecell)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SAVECELL, OnUpdateToolsSavecell)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_TEXT, OnEditText)
	ON_COMMAND(IDD_SELECTALL, OnSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TEXT, OnUpdateEditText)
	ON_COMMAND(ID_DRAW_TREND, OnDrawTrend)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TREND, OnUpdateDrawTrend)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_TOOLS_LOCK, OnToolsLock)
	ON_COMMAND(ID_TOOLS_UNLOCK, OnToolsUnlock)
	ON_COMMAND(IDM_SHOW_ALL_ATTRIBS, OnShowAllAttribs)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCDrawView construction/destruction
extern long lOldZoom;
extern BOOL bShowGrid;
extern long lVtGrid,lHzGrid;
extern BOOL bLockOnGrid;
extern long lWndWidth,lWndHeight;
extern CString sWndCaption;

CMCDrawView::CMCDrawView()
{
	bLockOnGrid=FALSE;
	bShowGrid=FALSE;
	lHzGrid=GD_HZ;
	lVtGrid=GD_VT;
	lZoomMode=0;
	lPixHor=1024;
	lPixVer=732;
	m_lDrawMode=DE_POINTER;
	m_lEditMode=ED_NONE;
	m_pCurElement=NULL;

	m_CurPen.lopnColor=RGB(64,64,192);
	//m_CurPen.lopnColor=RGB(0,0,0);
	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=0;
	m_CurPen.lopnWidth.y=0;

	m_CurBrush.lbColor=RGB(64,192,64);
	m_CurBrush.lbStyle=BS_SOLID;
	m_CurBrush.lbHatch=HS_BDIAGONAL;

	m_lBold=400;
	m_bItalic=false;
	m_bUnderline=false;
	m_CurFont.lfCharSet=DEFAULT_CHARSET;
	m_CurFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	m_CurFont.lfEscapement=0;	
	//wcscpy(m_CurFont.lfFaceName,L"宋体");	
	strcpy(m_CurFont.lfFaceName,"宋体");

	m_CurFont.lfHeight=120;
	m_CurFont.lfItalic=false;
	m_CurFont.lfOrientation=0;
	m_CurFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
	m_CurFont.lfPitchAndFamily=DEFAULT_PITCH;
	m_CurFont.lfQuality=DEFAULT_QUALITY;
	m_CurFont.lfStrikeOut=false;
	m_CurFont.lfUnderline=false;
	m_CurFont.lfWeight=FW_NORMAL;
	m_CurFont.lfWidth=0;
	
	m_FontColor=RGB(0,0,0);

	m_cursor.m_lHeight=m_CurFont.lfHeight;

	m_lCurCmdId=0;

	m_cf=RegisterClipboardFormat("gos");
}

CMCDrawView::~CMCDrawView()
{
}

BOOL CMCDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//	the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMCDrawView drawing
void CMCDrawView::PrintIt(CDC *pDC,CMCDrawDoc* pDoc)
{
	//画所有元件
	POSITION pos;
	CElement *pElement;
	
	//画背景色
	g_bPmcUiIsPrinting=TRUE;
	g_WindowColor=pDoc->m_WindowColor;

	long lWidth,lHeight,lTotalWidth,lTotalHeight;
	CElement::UPtoLP(lWndWidth,lWndHeight,&lWidth,&lHeight);
	CElement::UPtoLP(TOTAL_WIDTH,TOTAL_HEIGHT,&lTotalWidth,&lTotalHeight);
	CPen *pOldPen;
	pOldPen=(CPen *)pDC->SelectStockObject(NULL_PEN);
	CBrush brushBack(pDoc->m_WindowColor);
	CBrush *pOldBrush;
	pOldBrush=(CBrush *)pDC->SelectObject(&brushBack); 
	pDC->Rectangle(0,0,lWidth+DELTA,lHeight+DELTA);
	pDC->SelectStockObject(WHITE_BRUSH);
	if(lWidth<lTotalWidth)
		pDC->Rectangle(lWidth,0,lTotalWidth,lTotalHeight);
	if(lHeight<lTotalHeight)
		pDC->Rectangle(0,lHeight,lTotalWidth,lTotalHeight);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	//先画没有动态连接的非单元
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(!pElement->IsKindOf(RUNTIME_CLASS(CTrend)) && !pElement->IsKindOf(RUNTIME_CLASS(CCell)) && !pElement->m_DnyAttr.m_Linked)
				pElement->Draw(pDC);
		}
	}
	//后画趋势图
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CTrend)))
				pElement->Draw(pDC);
		}
	}
	//最后画有动态连接的或单元
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CCell)) || pElement->m_DnyAttr.m_Linked)
				pElement->Draw(pDC);
		}
	}
	g_bPmcUiIsPrinting=FALSE;
}

void CMCDrawView::OnDraw(CDC* pDC)
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDC->IsPrinting()){
		PrintIt(pDC,pDoc);
		return;
	}

	//先画在内存里
	CDC dcMem;
	if(!dcMem.CreateCompatibleDC(pDC)) return;
	CScrollView::OnPrepareDC(&dcMem);

	long lWidth,lHeight,lTotalWidth,lTotalHeight;
	CElement::UPtoLP(lWndWidth,lWndHeight,&lWidth,&lHeight);
	CElement::UPtoLP(TOTAL_WIDTH,TOTAL_HEIGHT,&lTotalWidth,&lTotalHeight);
	CBitmap memBitmap;
	POINT ptSize;
	ptSize.x=lTotalWidth+DELTA;
	ptSize.y=lTotalHeight+DELTA;
	pDC->LPtoDP(&ptSize);
	if(!memBitmap.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y)) return;

	dcMem.SelectObject(&memBitmap);
	dcMem.SetBkMode(TRANSPARENT);

	/* 2005-10-28 jht*/
	g_WindowColor = pDoc->m_WindowColor;
	//画背景色
	CPen *pOldPen;
	pOldPen=(CPen *)dcMem.SelectStockObject(NULL_PEN);
	CBrush brushBack(pDoc->m_WindowColor);
	CBrush *pOldBrush;
	pOldBrush=(CBrush *)dcMem.SelectObject(&brushBack); 
	dcMem.Rectangle(0,0,lWidth+DELTA,lHeight+DELTA);
	dcMem.SelectStockObject(WHITE_BRUSH);
	if(lWidth<lTotalWidth)
		dcMem.Rectangle(lWidth,0,lTotalWidth,lTotalHeight);
	if(lHeight<lTotalHeight)
		dcMem.Rectangle(0,lHeight,lTotalWidth,lTotalHeight);
	//画网格
	dcMem.SetROP2(R2_NOT);
	if(bShowGrid){
		long xu=0,yu=0;
		long xl,yl;
		while(yu<=TOTAL_HEIGHT){
			POINT pTemp;
			CElement::UPtoLP(xu,yu,&xl,&yl);
			pTemp.x=xl;
			pTemp.y=yl;
			dcMem.SetPixelV(pTemp,RGB(0,0,0));
			xu=xu+lHzGrid;
			if(xu>TOTAL_WIDTH){
				xu=0;
				yu=yu+lVtGrid;
			}
		}
	}
	dcMem.SetROP2(R2_COPYPEN);

	//画所有元件
	POSITION pos;
	CElement *pElement;
	//先画没有动态连接的非单元
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(!pElement->IsKindOf(RUNTIME_CLASS(CTrend)) && !pElement->IsKindOf(RUNTIME_CLASS(CCell)) && !pElement->m_DnyAttr.m_Linked)
				pElement->Draw(&dcMem);
		}
	}
	//后画趋势图
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CTrend)))
				pElement->Draw(&dcMem);
		}
	}
	//最后画有动态连接的或单元
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CCell)) || pElement->m_DnyAttr.m_Linked)
				pElement->Draw(&dcMem);
		}
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		((CElement*)(m_SelElements.GetNext(pos)))->DrawHotPoints(&dcMem);
	}
	if(m_SelElements.IsEmpty() && m_pCurElement!=NULL){
		m_pCurElement->DrawHotPoints(&dcMem);
	}

	pDC->BitBlt(0,0,lTotalWidth,lTotalHeight,&dcMem,0,0,SRCCOPY);
	dcMem.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CMCDrawView printing

BOOL CMCDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMCDrawView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	lOldZoom=lZoomMode;
	lZoomMode=2;
	OnInitialUpdate();

	CScrollView::OnPrepareDC(pDC);
}

void CMCDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	lZoomMode=lOldZoom;
	OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CMCDrawView diagnostics

#ifdef _DEBUG
void CMCDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMCDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMCDrawDoc* CMCDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCDrawDoc)));
	return (CMCDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCDrawView message handlers

void CMCDrawView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	SIZE sizeTotal;
	switch(lZoomMode){
	case 0:
		sizeTotal.cx=TOTAL_WIDTH;
		sizeTotal.cy=TOTAL_HEIGHT;
		GetParentFrame()->RecalcLayout();
		SetScaleToFitSize(sizeTotal);
		break;
	case 1:
		sizeTotal.cx=lPixHor;
		sizeTotal.cy=lPixVer;
		SetScrollSizes(MM_TEXT,sizeTotal);
		break;
	case 2:
		sizeTotal.cx=TOTAL_WIDTH;
		sizeTotal.cy=TOTAL_HEIGHT;
		SetScrollSizes(MM_TWIPS,sizeTotal);
	}	
}

void CMCDrawView::OnDrawArc() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_ARC;
}

void CMCDrawView::OnDrawButton() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_BUTTON;	
}

void CMCDrawView::OnDrawEllipse() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_ELLIPSE;	

}

void CMCDrawView::OnDrawLibcomp() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_LIBCOMP;	

	CDiaSelect dlg;
	dlg.m_dir = CString(get_working_dir())+"lib";
	dlg.m_wildcards = "*.gcf";
	if(dlg.DoModal()==IDOK){
		m_sCellFile=dlg.m_select+".gcf";
	}else{
		OnDrawPointer();
	}
	//CFileDialog dlg(true,"gcf",NULL,OFN_HIDEREADONLY,"Cell Files(*.gcf)|*.gcf|");
	//dlg.m_ofn.lpstrInitialDir=".\\lib";
	//if(dlg.DoModal()==IDOK){
	//	m_sCellFile=dlg.GetPathName();
	//}else{
	//	OnDrawPointer();
	//}
}

void CMCDrawView::OnDrawLine() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_LINE;	
}

void CMCDrawView::OnDrawPicure() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_PICTURE;	

	CDiaSelect dlg;
	dlg.m_dir = CString(get_working_dir())+"pic";
	dlg.m_wildcards = "*.emf";
	if(dlg.DoModal()==IDOK){
		m_sPicFile=dlg.m_select+".emf";
	}else{
		OnDrawPointer();
	}
}

void CMCDrawView::OnDrawPointer() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_POINTER;	
}

void CMCDrawView::OnDrawPolygon() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_POLYGON;	
}

void CMCDrawView::OnDrawPolyline() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_POLYLINE;	
}

void CMCDrawView::OnDrawRectangle() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_RECTANGLE;	
}

void CMCDrawView::OnDrawRoundrect() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_ROUNDRECT;	
}

void CMCDrawView::OnDrawText() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_TEXT;	
}

void CMCDrawView::OnUpdateDrawArc(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_ARC){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawButton(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_BUTTON){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawEllipse(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_ELLIPSE){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawLibcomp(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_LIBCOMP){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawLine(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_LINE){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawPicure(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_PICTURE){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawPointer(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_POINTER){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawPolygon(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_POLYGON){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawPolyline(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_POLYLINE){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawRectangle(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_RECTANGLE){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawRoundrect(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_ROUNDRECT){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnUpdateDrawText(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_TEXT){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
		if(m_cursor.m_bActive){
			m_cursor.m_bActive=false;
		}
	}
}

void CMCDrawView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	if(m_pCurElement==NULL){
		//ReleaseCapture();
		//OnDrawPointer();
		if(!m_SelElements.IsEmpty()){
			CDiaDnyAttr dlg;
			dlg.m_DnyAttr=m_CurDnyAttr;
			if(dlg.DoModal()){
				POSITION pos;
				CElement * pElement;
				for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
					pElement=(CElement*)(m_SelElements.GetNext(pos));
					pElement->m_DnyAttr=dlg.m_DnyAttr;
				}
			}
		}
	}else{
		if(m_lDrawMode==DE_POLYLINE || m_lDrawMode==DE_POLYGON){
			m_lEditMode=ED_NONE;
			m_pCurElement->m_lHitTarget=TK_CLS;
			m_pCurElement->DrawTracker(pDC);
			m_pCurElement->Draw(pDC);
			//m_pCurElement=NULL;
			OnDrawPointer();
			ReleaseCapture();
		}else{
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CCell))){
				CDiaCellAttr dlgCell;
				dlgCell.m_pCell=(CCell*)m_pCurElement;
				dlgCell.DoModal();
			}else if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CTrend))){
				CDiaTrendAttr dlgTrend;
				dlgTrend.m_mode=((CTrend*)m_pCurElement)->m_lMode;
				dlgTrend.m_BkColor=((CTrend*)m_pCurElement)->m_BkColor;
				dlgTrend.m_GdColor=((CTrend*)m_pCurElement)->m_GdColor;
				dlgTrend.m_tag1=((CTrend*)m_pCurElement)->m_tagnames[0];
				dlgTrend.m_tag2=((CTrend*)m_pCurElement)->m_tagnames[1];
				dlgTrend.m_tag3=((CTrend*)m_pCurElement)->m_tagnames[2];
				dlgTrend.m_tag4=((CTrend*)m_pCurElement)->m_tagnames[3];
				dlgTrend.m_tag5=((CTrend*)m_pCurElement)->m_tagnames[4];
				dlgTrend.m_tag6=((CTrend*)m_pCurElement)->m_tagnames[5];
				dlgTrend.m_tag7=((CTrend*)m_pCurElement)->m_tagnames[6];
				dlgTrend.m_tag8=((CTrend*)m_pCurElement)->m_tagnames[7];
				dlgTrend.m_tag9=((CTrend*)m_pCurElement)->m_tagnames[8];
				dlgTrend.m_tag10=((CTrend*)m_pCurElement)->m_tagnames[9];
				dlgTrend.m_ca1=((CTrend*)m_pCurElement)->m_bAnalog[0];
				dlgTrend.m_ca2=((CTrend*)m_pCurElement)->m_bAnalog[1];
				dlgTrend.m_ca3=((CTrend*)m_pCurElement)->m_bAnalog[2];
				dlgTrend.m_ca4=((CTrend*)m_pCurElement)->m_bAnalog[3];
				dlgTrend.m_ca5=((CTrend*)m_pCurElement)->m_bAnalog[4];
				dlgTrend.m_ca6=((CTrend*)m_pCurElement)->m_bAnalog[5];
				dlgTrend.m_ca7=((CTrend*)m_pCurElement)->m_bAnalog[6];
				dlgTrend.m_ca8=((CTrend*)m_pCurElement)->m_bAnalog[7];
				dlgTrend.m_ca9=((CTrend*)m_pCurElement)->m_bAnalog[8];
				dlgTrend.m_ca10=((CTrend*)m_pCurElement)->m_bAnalog[9];
				dlgTrend.m_min1=((CTrend*)m_pCurElement)->m_fMin[0];
				dlgTrend.m_min2=((CTrend*)m_pCurElement)->m_fMin[1];
				dlgTrend.m_min3=((CTrend*)m_pCurElement)->m_fMin[2];
				dlgTrend.m_min4=((CTrend*)m_pCurElement)->m_fMin[3];
				dlgTrend.m_min5=((CTrend*)m_pCurElement)->m_fMin[4];
				dlgTrend.m_min6=((CTrend*)m_pCurElement)->m_fMin[5];
				dlgTrend.m_min7=((CTrend*)m_pCurElement)->m_fMin[6];
				dlgTrend.m_min8=((CTrend*)m_pCurElement)->m_fMin[7];
				dlgTrend.m_min9=((CTrend*)m_pCurElement)->m_fMin[8];
				dlgTrend.m_min10=((CTrend*)m_pCurElement)->m_fMin[9];
				dlgTrend.m_max1=((CTrend*)m_pCurElement)->m_fMax[0];
				dlgTrend.m_max2=((CTrend*)m_pCurElement)->m_fMax[1];
				dlgTrend.m_max3=((CTrend*)m_pCurElement)->m_fMax[2];
				dlgTrend.m_max4=((CTrend*)m_pCurElement)->m_fMax[3];
				dlgTrend.m_max5=((CTrend*)m_pCurElement)->m_fMax[4];
				dlgTrend.m_max6=((CTrend*)m_pCurElement)->m_fMax[5];
				dlgTrend.m_max7=((CTrend*)m_pCurElement)->m_fMax[6];
				dlgTrend.m_max8=((CTrend*)m_pCurElement)->m_fMax[7];
				dlgTrend.m_max9=((CTrend*)m_pCurElement)->m_fMax[8];
				dlgTrend.m_max10=((CTrend*)m_pCurElement)->m_fMax[9];
				for(int i=0;i<10;i++){
					dlgTrend.m_pens[i]=((CTrend*)m_pCurElement)->m_pens[i];
				}
				if(dlgTrend.DoModal()==IDOK){

					CString az;
					CString bz = "111"; 
					
					((CTrend*)m_pCurElement)->m_lMode=dlgTrend.m_mode;
					((CTrend*)m_pCurElement)->m_BkColor=dlgTrend.m_BkColor;
					((CTrend*)m_pCurElement)->m_GdColor=dlgTrend.m_GdColor;

					az = bz;

					((CTrend*)m_pCurElement)->m_tagnames[0]=dlgTrend.m_tag1;
					((CTrend*)m_pCurElement)->m_tagnames[1]=dlgTrend.m_tag2;
					((CTrend*)m_pCurElement)->m_tagnames[2]=dlgTrend.m_tag3;
					((CTrend*)m_pCurElement)->m_tagnames[3]=dlgTrend.m_tag4;
					((CTrend*)m_pCurElement)->m_tagnames[4]=dlgTrend.m_tag5;
					((CTrend*)m_pCurElement)->m_tagnames[5]=dlgTrend.m_tag6;
					((CTrend*)m_pCurElement)->m_tagnames[6]=dlgTrend.m_tag7;
					((CTrend*)m_pCurElement)->m_tagnames[7]=dlgTrend.m_tag8;
					((CTrend*)m_pCurElement)->m_tagnames[8]=dlgTrend.m_tag9;
					((CTrend*)m_pCurElement)->m_tagnames[9]=dlgTrend.m_tag10;
					((CTrend*)m_pCurElement)->m_bAnalog[0]=dlgTrend.m_ca1;
					((CTrend*)m_pCurElement)->m_bAnalog[1]=dlgTrend.m_ca2;
					((CTrend*)m_pCurElement)->m_bAnalog[2]=dlgTrend.m_ca3;
					((CTrend*)m_pCurElement)->m_bAnalog[3]=dlgTrend.m_ca4;
					((CTrend*)m_pCurElement)->m_bAnalog[4]=dlgTrend.m_ca5;
					((CTrend*)m_pCurElement)->m_bAnalog[5]=dlgTrend.m_ca6;
					((CTrend*)m_pCurElement)->m_bAnalog[6]=dlgTrend.m_ca7;
					((CTrend*)m_pCurElement)->m_bAnalog[7]=dlgTrend.m_ca8;
					((CTrend*)m_pCurElement)->m_bAnalog[8]=dlgTrend.m_ca9;
					((CTrend*)m_pCurElement)->m_bAnalog[9]=dlgTrend.m_ca10;
					((CTrend*)m_pCurElement)->m_fMin[0]=dlgTrend.m_min1;
					((CTrend*)m_pCurElement)->m_fMin[1]=dlgTrend.m_min2;
					((CTrend*)m_pCurElement)->m_fMin[2]=dlgTrend.m_min3;
					((CTrend*)m_pCurElement)->m_fMin[3]=dlgTrend.m_min4;
					((CTrend*)m_pCurElement)->m_fMin[4]=dlgTrend.m_min5;
					((CTrend*)m_pCurElement)->m_fMin[5]=dlgTrend.m_min6;
					((CTrend*)m_pCurElement)->m_fMin[6]=dlgTrend.m_min7;
					((CTrend*)m_pCurElement)->m_fMin[7]=dlgTrend.m_min8;
					((CTrend*)m_pCurElement)->m_fMin[8]=dlgTrend.m_min9;
					((CTrend*)m_pCurElement)->m_fMin[9]=dlgTrend.m_min10;
					((CTrend*)m_pCurElement)->m_fMax[0]=dlgTrend.m_max1;
					((CTrend*)m_pCurElement)->m_fMax[1]=dlgTrend.m_max2;
					((CTrend*)m_pCurElement)->m_fMax[2]=dlgTrend.m_max3;
					((CTrend*)m_pCurElement)->m_fMax[3]=dlgTrend.m_max4;
					((CTrend*)m_pCurElement)->m_fMax[4]=dlgTrend.m_max5;
					((CTrend*)m_pCurElement)->m_fMax[5]=dlgTrend.m_max6;
					((CTrend*)m_pCurElement)->m_fMax[6]=dlgTrend.m_max7;
					((CTrend*)m_pCurElement)->m_fMax[7]=dlgTrend.m_max8;
					((CTrend*)m_pCurElement)->m_fMax[8]=dlgTrend.m_max9;
					((CTrend*)m_pCurElement)->m_fMax[9]=dlgTrend.m_max10;
					((CTrend*)m_pCurElement)->m_pens[0]=dlgTrend.m_pens[0];
					((CTrend*)m_pCurElement)->m_pens[1]=dlgTrend.m_pens[1];
					((CTrend*)m_pCurElement)->m_pens[2]=dlgTrend.m_pens[2];
					((CTrend*)m_pCurElement)->m_pens[3]=dlgTrend.m_pens[3];
					((CTrend*)m_pCurElement)->m_pens[4]=dlgTrend.m_pens[4];
					((CTrend*)m_pCurElement)->m_pens[5]=dlgTrend.m_pens[5];
					((CTrend*)m_pCurElement)->m_pens[6]=dlgTrend.m_pens[6];
					((CTrend*)m_pCurElement)->m_pens[7]=dlgTrend.m_pens[7];
					((CTrend*)m_pCurElement)->m_pens[8]=dlgTrend.m_pens[8];
					((CTrend*)m_pCurElement)->m_pens[9]=dlgTrend.m_pens[9];
				}
			}else{
				if(!pDoc->AllowNew() && !m_pCurElement->m_DnyAttr.m_Linked){
					MessageBox("动态元素已达上限",NULL,MB_OK|MB_ICONINFORMATION);
				}else{
					CDiaDnyAttr dlg;
					if(m_pCurElement->m_DnyAttr.m_Linked){
						dlg.m_DnyAttr=m_pCurElement->m_DnyAttr;
						dlg.IniShow();
					}else{
						dlg.m_DnyAttr=m_CurDnyAttr;
					}
					if(dlg.DoModal()==IDOK){
						m_pCurElement->m_DnyAttr=dlg.m_DnyAttr;
						m_CurDnyAttr=dlg.m_DnyAttr;
					}
				}
			}
		}
	}
	
	OnDraw(pDC);
	ReleaseDC(pDC);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CMCDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_potPrv=point;
	pDC->DPtoLP(&m_potPrv);
	if(bLockOnGrid){
		long lvg,lhg;
		CElement::UPtoLP(lVtGrid,lHzGrid,&lvg,&lhg);
		m_potPrv.x=(m_potPrv.x+lhg/2)-(m_potPrv.x+lhg/2)%lhg;
		m_potPrv.y=(m_potPrv.y+lvg/2)-(m_potPrv.y+lvg/2)%lvg;
	}
	m_potPrvMov=m_potPrv;
	long x,y;
	CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&x,&y);

	CRectTracker tracker;

	CElement *pElement, *pNew;
	POSITION pos, pos1;
	bool bSelGroup = false;

	switch(m_lDrawMode){
	case DE_POINTER:
		if(nFlags & MK_SHIFT || nFlags & MK_CONTROL){
			bSelGroup = false;
		}else{
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				if(((CElement*)(m_SelElements.GetNext(pos)))->HitTest(x,y)!=TK_NONE){
					bSelGroup=true;
					break;
				}
			}
			if(!bSelGroup){
				m_SelElements.RemoveAll();
			}else{
				for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
					pElement=((CElement*)(m_SelElements.GetNext(pos)));
					pElement->m_lHitTarget=TK_CLS;
					pElement->DrawTracker(pDC);
					pElement->m_lHitTarget=TK_MOV;
					m_lEditMode=ED_RESIZE;
				}
			}
		}

		if(!bSelGroup){
			if(m_pCurElement!=NULL){
				if(nFlags & MK_SHIFT || nFlags & MK_CONTROL){
					m_SelElements.AddTail(m_pCurElement);
					m_pCurElement->m_lHitTarget=TK_NONE;
					m_pCurElement = NULL;
				}else{
					if(m_pCurElement->HitTest(x,y)==TK_NONE){
						m_pCurElement=NULL;
					}
				}
			}

			//first find cell and dyna-linked
			if(m_pCurElement==NULL){
				for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
					pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMem);
					if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
						while(pElement->m_pChild!=NULL){
							if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
							pElement=pElement->m_pChild;
							if(pElement->m_lStatus==ST_ZORDER) break;
						}
						if(pElement->IsKindOf(RUNTIME_CLASS(CCell)) || pElement->m_DnyAttr.m_Linked){
							if(pElement->HitTest(x,y)!=TK_NONE){
								m_pCurElement=pElement;
							}
						}
					}
				}
			}
			//second find trend
			if(m_pCurElement==NULL){
				for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
					pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMeta);
					if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
						while(pElement->m_pChild!=NULL){
							if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
							pElement=pElement->m_pChild;
							if(pElement->m_lStatus==ST_ZORDER) break;
						}
						if(pElement->IsKindOf(RUNTIME_CLASS(CTrend))){
							if(pElement->HitTest(x,y)!=TK_NONE){
								m_pCurElement=pElement;
							}
						}
					}
				}
			}
			//third find static
			if(m_pCurElement==NULL){
				for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
					pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMeta);
					if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
						while(pElement->m_pChild!=NULL){
							if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
							pElement=pElement->m_pChild;
							if(pElement->m_lStatus==ST_ZORDER) break;
						}
						if(!pElement->IsKindOf(RUNTIME_CLASS(CTrend)) && !pElement->IsKindOf(RUNTIME_CLASS(CCell)) && !pElement->m_DnyAttr.m_Linked){
							if(pElement->HitTest(x,y)!=TK_NONE){
								m_pCurElement=pElement;
							}
						}
					}
				}
			}

			if(m_pCurElement==NULL){
				if(!tracker.TrackRubberBand(this, point, true)) break;
				RECT retTmp;
				tracker.GetTrueRect(&retTmp);
				pDC->DPtoLP(&retTmp);
				CElement::LPtoUP(retTmp.left,retTmp.top,&retTmp.left,&retTmp.top);
				CElement::LPtoUP(retTmp.right,retTmp.bottom,&retTmp.right,&retTmp.bottom);				
				if(retTmp.left!=retTmp.right && retTmp.top!=retTmp.bottom){
					for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
						pElement=(CElement*)(pDoc->m_listElements.GetNext(pos));
						if(pElement->RInR(&retTmp)){
							if( m_SelElements.Find(pElement) == NULL ){
								m_SelElements.AddTail(pElement);
								pElement->m_lHitTarget=TK_NONE;
							}
						}
					}
				}
			}

			if(m_pCurElement!=NULL){
				if(nFlags & MK_SHIFT || nFlags & MK_CONTROL){
					m_pCurElement->m_lHitTarget=TK_NONE;
					if((pos=m_SelElements.Find(m_pCurElement))==NULL){
						m_SelElements.AddTail(m_pCurElement);
					}else{
						m_SelElements.RemoveAt(pos);
						m_pCurElement = NULL;
					}
				}else{
					m_pCurElement->m_lHitTarget=TK_CLS;
					m_pCurElement->DrawTracker(pDC);
					m_pCurElement->m_lHitTarget=m_pCurElement->HitTest(x,y);
					if(m_pCurElement->m_lHitTarget==TK_SP || m_pCurElement->m_lHitTarget==TK_EP || m_pCurElement->m_lHitTarget<0){
						m_pCurElement->Draw(pDC);
					}
					if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)) && m_pCurElement->m_lHitTarget==TK_MOV){
						m_pCurElement->Draw(pDC);
					}
					m_lEditMode=ED_RESIZE;
					SetCapture();
				}
			}
		}

		if(m_SelElements.GetCount()==1){
			m_pCurElement=(CElement *)(m_SelElements.GetAt(m_SelElements.GetHeadPosition()));
			m_SelElements.RemoveAll();
		}
		if(m_SelElements.GetCount()>1){
			m_pCurElement=NULL;
		}
		
		if(!(nFlags & MK_SHIFT || nFlags & MK_CONTROL)){
			m_lCurCmdId++;
			if(m_pCurElement!=NULL){
				if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					m_pCurElement->m_lStatus=ST_EDIT;
					m_pCurElement=pNew;
				}
			}else if(!m_SelElements.IsEmpty()){
				for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
					pElement=(CElement*)(m_SelElements.GetNext(pos));
					if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
						pDoc->m_listElements.AddTail(pNew);
						pElement->m_lStatus=ST_EDIT;
						pos1=m_SelElements.Find(pElement);
						m_SelElements.SetAt(pos1,pNew);
					}
				}
			}
		}

		break;

	case DE_PICTURE:		
	case DE_LIBCOMP:
	case DE_TREND:
	case DE_PASTE:
		break;
	case DE_TEXT:
		break;
	case DE_POLYLINE:
	case DE_POLYGON:
		if(m_lEditMode==ED_NONE){
			m_lEditMode=ED_NEW;
			SetCapture();
		}else if(m_lEditMode==ED_RESIZE){
			m_lEditMode=ED_APPEND;
		}
		break;
	default:
		if(m_lEditMode==ED_NONE){
			m_lEditMode=ED_NEW;
			SetCapture();
		}
	}

	if(m_lDrawMode==DE_PICTURE){
		m_lCurCmdId++;
		CMeta *pMeta=new CMeta();
		pMeta->m_lStatus=ST_NOMAL;
		pMeta->m_lCmdId=m_lCurCmdId;
		pMeta->m_sPicFile=m_sPicFile;

		pMeta->m_hMeta = (HENHMETAFILE)pmcui_open_metafile(m_sPicFile);

		ENHMETAHEADER emh;
		::GetEnhMetaFileHeader(pMeta->m_hMeta,sizeof(ENHMETAHEADER),&emh);
		pMeta->m_lx1=x;
		pMeta->m_ly1=y;
		RECTL rtMeta=emh.rclBounds;
		pMeta->m_lx2=x+rtMeta.right-rtMeta.left;
		pMeta->m_ly2=y+rtMeta.bottom-rtMeta.top;
		::DeleteEnhMetaFile(pMeta->m_hMeta);
		m_pCurElement=pMeta;
		m_pCurElement->m_lHitTarget=TK_CLS;
		pDoc->m_listElements.AddTail(m_pCurElement); 
	}

	if(m_lDrawMode==DE_LIBCOMP){
		MSXML::IXMLDOMDocumentPtr pCellFile;
		HRESULT hr = pCellFile.CreateInstance(MSXML::CLSID_DOMDocument);
		if (FAILED(hr)){
			 _com_error er(hr);
			 AfxMessageBox(er.ErrorMessage());
		}else{
			CString sCellFile=theApp.m_path+"lib\\"+m_sCellFile;
			if(pCellFile->load(sCellFile.AllocSysString())==FALSE){
				sCellFile="无法打开文件"+sCellFile;
				 AfxMessageBox(sCellFile);
			}else{
				//load from cell file
				_bstr_t strFile=L"celf";
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
				MSXML::IXMLDOMElementPtr pRoot=pCellFile->documentElement;
				if(pRoot->baseName==strFile){
					m_lCurCmdId++;
					CCell *pCell=new CCell();
					pCell->m_lStatus=ST_NOMAL;
					pCell->m_lCmdId=m_lCurCmdId;
					pCell->Load(pRoot);
					//move to correct position
					_variant_t val;
					POINT ptOffSet;
					val=pRoot->getAttribute(L"x");
					val.ChangeType(VT_I4);
					ptOffSet.x=-val.lVal+x;
					val=pRoot->getAttribute(L"y");
					val.ChangeType(VT_I4);
					ptOffSet.y=-val.lVal+y;
					pCell->MoveInCell(ptOffSet);
					m_pCurElement=pCell;
					m_pCurElement->m_lHitTarget=TK_CLS;
					pDoc->m_listElements.AddTail(m_pCurElement);
				}else{
					AfxMessageBox("文件类型错误");
				}
			}
		}
	}

	if(m_lDrawMode==DE_TREND){
		m_lCurCmdId++;
		CTrend *pTrend =new CTrend();
		pTrend->m_lStatus=ST_NOMAL;
		pTrend->m_lCmdId=m_lCurCmdId;
		pTrend->m_lx1=x;
		pTrend->m_ly1=y;
		pTrend->m_lx2=x+clTrendMinWidth;
		pTrend->m_ly2=y+clTrendMinHeight;
		/*
		CString sPicFile=theApp.m_path+"trend.emf";
		HENHMETAFILE hMeta=::GetEnhMetaFile(sPicFile);
		ENHMETAHEADER emh;
		::GetEnhMetaFileHeader(hMeta,sizeof(ENHMETAHEADER),&emh);
		pTrend->m_lx1=x;
		pTrend->m_ly1=y;
		RECTL rtMeta=emh.rclBounds;
		pTrend->m_lx2=x+rtMeta.right-rtMeta.left;
		pTrend->m_ly2=y+rtMeta.bottom-rtMeta.top;
		::DeleteEnhMetaFile(hMeta);
		*/
		m_pCurElement=pTrend;
		
		m_pCurElement->m_lHitTarget=TK_CLS;
		pDoc->m_listElements.AddTail(m_pCurElement); 
	}

	if(m_lDrawMode==DE_PASTE){
		MSXML::IXMLDOMDocumentPtr pPaste;	  
		HRESULT hr = pPaste.CreateInstance(MSXML::CLSID_DOMDocument);
		if (FAILED(hr)){
			_com_error er(hr);
			AfxMessageBox(er.ErrorMessage());
		}else{
			pPaste->loadXML((LPCTSTR)m_paste);
			MSXML::IXMLDOMElementPtr pRoot=pPaste->documentElement;
			//装载数据
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
			_variant_t strTrend=L"trend";

			_variant_t val;
			//get offset
			POINT ptOffSet;
			val=pRoot->getAttribute(L"x");
			val.ChangeType(VT_I4);
			ptOffSet.x=-val.lVal+x;
			val=pRoot->getAttribute(L"y");
			val.ChangeType(VT_I4);
			ptOffSet.y=-val.lVal+y;

			MSXML::IXMLDOMElementPtr pItem;
			CElement * pElement;
			m_lCurCmdId++;
			m_SelElements.RemoveAll();
			m_pCurElement=NULL;
			for(pItem=pRoot->firstChild;pItem!=NULL;pItem=pItem->nextSibling){
				if(pItem->baseName==strGO){
					pElement=NULL;
					val=pItem->getAttribute(L"type");
					if(val==strRectangle){
						pElement=new CRectangle();
						((CRectangle*)pElement)->Load(pItem);
					}else if(val==strArc){
						pElement=new CArc();
						((CArc*)pElement)->Load(pItem);
					}else if(val==strEllipse){
						pElement=new CEllipse();
						((CEllipse*)pElement)->Load(pItem);
					}else if(val==strLine){
						pElement=new CLine();
						((CLine*)pElement)->Load(pItem);
					}else if(val==strMCButton){
						pElement=new CMCButton();
						((CMCButton*)pElement)->Load(pItem);
					}else if(val==strMeta){
						pElement=new CMeta();
						((CMeta*)pElement)->Load(pItem);
					}else if(val==strRoundRect){
						pElement=new CRoundRect();
						((CRoundRect*)pElement)->Load(pItem);
					}else if(val==strText){
						pElement=new CText();
						((CText*)pElement)->Load(pItem);
					}else if(val==strPolygon){
						pElement=new CPolygon();
						((CPolygon*)pElement)->Load(pItem);
					}else if(val==strPolyLine){
						pElement=new CPolyLine();
						((CPolyLine*)pElement)->Load(pItem);
					}else if(val==strTrend){
						pElement=new CTrend();
						((CTrend*)pElement)->Load(pItem);
					}else if(val==strCell){
						pElement=new CCell();
						((CCell*)pElement)->Load(pItem);
						if(((CCell*)pElement)->GetDynCount()>0) pElement->m_DnyAttr.m_Linked=true;
					}
					if(pElement){
						if(pDoc->AllowNew() || !pElement->m_DnyAttr.m_Linked){
							pElement->m_lCmdId=m_lCurCmdId;
							pElement->m_lStatus=ST_NOMAL;
							pElement->MoveInCell(ptOffSet);
							m_SelElements.AddTail(pElement);
							pDoc->m_listElements.AddTail(pElement);
						}else{
							MessageBox("动态元素已达上限",NULL,MB_OK|MB_ICONINFORMATION);
							break;
						}
					}
				}
			}

			pPaste.Release();
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CMCDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	
	POSITION pos;
	CElement * pElement;
	switch(m_lDrawMode){
	case DE_POLYLINE:
	case DE_POLYGON:
		break;
	case DE_TEXT:
		if(m_lEditMode==ED_NONE){
			ReleaseCapture();
			m_lEditMode=ED_NEW;
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&m_cursor.m_lx,&m_cursor.m_ly);
			m_cursor.m_bActive=true;
			SetTimer(1,500,NULL);
			OnTimer(1);
			break;
		}else{
			m_cursor.m_bActive=false;
			m_cursor.m_bShow=false;
		}
	case DE_PICTURE:
	case DE_LIBCOMP:
	case DE_TREND:
	case DE_PASTE:
		if(nFlags & MK_SHIFT) break;
	default:
		m_lEditMode=ED_NONE;
		if(m_SelElements.GetCount()==1){
			m_pCurElement=(CElement *)(m_SelElements.GetHead());
			m_SelElements.RemoveAll();
		}else if(m_SelElements.GetCount()>1){
			m_pCurElement=NULL;
		}
		if(m_pCurElement!=NULL){
			m_pCurElement->m_lHitTarget=TK_CLS;
		}
		if(!m_SelElements.IsEmpty()){
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=((CElement*)(m_SelElements.GetNext(pos)));
				pElement->m_lHitTarget=TK_CLS;
			}
		}
		ReleaseCapture();
		OnDrawPointer();
		break;
	}
	
	if(m_lDrawMode!=DE_POLYLINE && m_lDrawMode!=DE_POLYGON){
		OnDraw(pDC);
	}

	ReleaseDC(pDC);
	CScrollView::OnLButtonUp(nFlags, point);
}

void CMCDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);

	CScrollView::OnPrepareDC(pDC);
	
	POINT *pPotTemp;
	long lCount;
	long lx,ly;

	CMCDrawDoc* pDoc = GetDocument();
	if(m_lEditMode==ED_NEW){
		m_SelElements.RemoveAll();
		switch(m_lDrawMode){
		case DE_LINE:
			m_pCurElement=new CLine();
			((CLine *)m_pCurElement)->m_pen=m_CurPen;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CLine *)m_pCurElement)->m_lx1,&((CLine *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CLine *)m_pCurElement)->m_lx2,&((CLine *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_ARC:
			m_pCurElement=new CArc();
			((CArc *)m_pCurElement)->m_pen=m_CurPen;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CArc *)m_pCurElement)->m_lx1,&((CArc *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CArc *)m_pCurElement)->m_lx2,&((CArc *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_POLYLINE:
			m_pCurElement=new CPolyLine();
			((CPolyLine *)m_pCurElement)->m_pen=m_CurPen;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&lx,&ly);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&lx,&ly);
			((CPolyLine *)m_pCurElement)->m_ptArray.SetSize(2);
			pPotTemp=((CPolyLine *)m_pCurElement)->m_ptArray.GetData();
			pPotTemp->x=lx;
			pPotTemp->y=ly;
			pPotTemp++;
			pPotTemp->x=lx;
			pPotTemp->y=ly;
			m_pCurElement->m_lHitTarget=-1;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_RECTANGLE:
			m_pCurElement=new CRectangle();
			((CRectangle *)m_pCurElement)->m_pen=m_CurPen;
			((CRectangle *)m_pCurElement)->m_brush=m_CurBrush;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CRectangle *)m_pCurElement)->m_lx1,&((CRectangle *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CRectangle *)m_pCurElement)->m_lx2,&((CRectangle *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_ROUNDRECT:
			m_pCurElement=new CRoundRect();
			((CRoundRect *)m_pCurElement)->m_pen=m_CurPen;
			((CRoundRect *)m_pCurElement)->m_brush=m_CurBrush;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CRoundRect *)m_pCurElement)->m_lx1,&((CRoundRect *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CRoundRect *)m_pCurElement)->m_lx2,&((CRoundRect *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_ELLIPSE:
			m_pCurElement=new CEllipse();
			((CEllipse *)m_pCurElement)->m_pen=m_CurPen;
			((CEllipse *)m_pCurElement)->m_brush=m_CurBrush;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CEllipse *)m_pCurElement)->m_lx1,&((CEllipse *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CEllipse *)m_pCurElement)->m_lx2,&((CEllipse *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_POLYGON:
			m_pCurElement=new CPolygon();
			((CPolygon *)m_pCurElement)->m_pen=m_CurPen;
			((CPolygon *)m_pCurElement)->m_brush=m_CurBrush;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&lx,&ly);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&lx,&ly);
			((CPolygon *)m_pCurElement)->m_ptArray.SetSize(2);
			pPotTemp=((CPolygon *)m_pCurElement)->m_ptArray.GetData();
			pPotTemp->x=lx;
			pPotTemp->y=ly;
			pPotTemp++;
			pPotTemp->x=lx;
			pPotTemp->y=ly;
			m_pCurElement->m_lHitTarget=-1;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_BUTTON:
			m_pCurElement=new CMCButton();
			((CMCButton *)m_pCurElement)->m_brush=m_CurBrush;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CMCButton *)m_pCurElement)->m_lx1,&((CMCButton *)m_pCurElement)->m_ly1);
			CElement::LPtoUP(m_potPrv.x,m_potPrv.y,&((CMCButton *)m_pCurElement)->m_lx2,&((CMCButton *)m_pCurElement)->m_ly2);
			m_pCurElement->m_lHitTarget=TK_RB;
			m_lEditMode=ED_RESIZE;
			break;
		//default:
			//m_lEditMode=ED_NONE;
		}
		if(m_pCurElement!=NULL){
			m_lCurCmdId++;
			m_pCurElement->m_lCmdId=m_lCurCmdId;
		}
	}
	
	if(m_lEditMode==ED_APPEND){
		switch(m_lDrawMode){
		case DE_POLYLINE:
			lCount=((CPolyLine *)m_pCurElement)->m_ptArray.GetSize()+1;
			((CPolyLine *)m_pCurElement)->m_ptArray.SetSize(lCount);
			((CPolyLine *)m_pCurElement)->m_ptArray.SetAt(lCount-1,((CPolyLine *)m_pCurElement)->m_ptArray.GetAt(lCount-2));
			m_pCurElement->m_lHitTarget=-1;
			m_lEditMode=ED_RESIZE;
			break;
		case DE_POLYGON:
			lCount=((CPolygon *)m_pCurElement)->m_ptArray.GetSize()+1;
			if(lCount==3){
				m_pCurElement->m_lHitTarget=TK_CLS;
				m_pCurElement->DrawTracker(pDC);
			}
			((CPolygon *)m_pCurElement)->m_ptArray.SetSize(lCount);
			((CPolygon *)m_pCurElement)->m_ptArray.SetAt(lCount-1,((CPolygon *)m_pCurElement)->m_ptArray.GetAt(lCount-2));
			m_pCurElement->m_lHitTarget=-1;
			m_lEditMode=ED_RESIZE;
			break;
		}
	}

	pDC->DPtoLP(&point);
	POINT potMouse=point;
	if(m_lEditMode==ED_RESIZE){
		if(nFlags & MK_CONTROL){
			if(abs(point.x-m_potPrv.x)>abs(point.y-m_potPrv.y)){
				point.y=m_potPrv.y;
			}else{
				point.x=m_potPrv.x;
			}
		}
		if(bLockOnGrid){
			long lvg,lhg;
			CElement::UPtoLP(lVtGrid,lHzGrid,&lvg,&lhg);
			point.x=(point.x+lhg/2)-(point.x+lhg/2)%lhg;
			point.y=(point.y+lvg/2)-(point.y+lvg/2)%lvg;
		}
		potMouse=point;

		if(m_pCurElement!=NULL){
			if(m_pCurElement->m_lHitTarget==TK_MOV){
				CPoint ptTemp=point;
				point-=m_potPrvMov;
				m_potPrvMov=ptTemp;
			}
			
			CElement::LPtoUP(point.x,point.y,&m_pCurElement->m_potTracker.x,&m_pCurElement->m_potTracker.y);
			m_pCurElement->DrawTracker(pDC);
		}
		if(!m_SelElements.IsEmpty()){
			CPoint ptTemp=point;
			point-=m_potPrvMov;
			m_potPrvMov=ptTemp;
			POSITION pos;
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				CElement* pElement=((CElement*)(m_SelElements.GetNext(pos)));
				CElement::LPtoUP(point.x,point.y,&pElement->m_potTracker.x,&pElement->m_potTracker.y);
				pElement->DrawTracker(pDC);
			}
		}
	}
	
	ReleaseDC(pDC);

	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	if(pStatus){
		CElement::LPtoUP(potMouse.x,potMouse.y,&potMouse.x,&potMouse.y);
		CString sTemp;
		sTemp.Format("鼠标 %d,%d",potMouse.x,potMouse.y);
		pStatus->SetPaneText(0,sTemp);
		if(m_pCurElement!=NULL && m_SelElements.IsEmpty()){
			sTemp.Format("位置 %d,%d",m_pCurElement->Left(),m_pCurElement->Top());
			pStatus->SetPaneText(1,sTemp);
			sTemp.Format("大小 %dx%d",m_pCurElement->Right()-m_pCurElement->Left(),\
				m_pCurElement->Bottom()-m_pCurElement->Top());
			pStatus->SetPaneText(2,sTemp);
		}else{
			sTemp="";
			pStatus->SetPaneText(1,sTemp);
			pStatus->SetPaneText(2,sTemp);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CMCDrawView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	LOGFONT tFont;
	tFont=m_CurFont;
	CElement::UPtoLP(tFont.lfHeight,tFont.lfWeight,&tFont.lfHeight,&tFont.lfWidth);
	CFont font;
	font.CreateFontIndirect(&tFont);
	CFont *pOldFont=pDC->SelectObject(&font);

	m_cursor.Hide(pDC);

	if(m_lEditMode==ED_NEW){
		if( VK_ESCAPE == nChar ){
			OnDrawPointer();
			OnDraw(pDC);
		}
		if( !iscntrl(nChar) ){
			m_pCurElement=new CText();
			((CText *)m_pCurElement)->m_font=m_CurFont;
			((CText *)m_pCurElement)->m_FaceColor=m_FontColor;
			pDoc->m_listElements.AddTail(m_pCurElement); 
			((CText *)m_pCurElement)->m_lx1=m_cursor.m_lx;
			((CText *)m_pCurElement)->m_ly1=m_cursor.m_ly;
			((CText *)m_pCurElement)->m_lx2=m_cursor.m_lx;
			((CText *)m_pCurElement)->m_ly2=m_cursor.m_ly+m_CurFont.lfHeight;
			m_pCurElement->m_lHitTarget=TK_CT;
			m_lEditMode=ED_EDIT;
			m_lCurCmdId++;
			m_pCurElement->m_lCmdId=m_lCurCmdId;
		}
	}
	if(m_lEditMode==ED_EDIT){
		if(nChar==13){//如果输入为回车键
			m_cursor.m_lx=((CText *)m_pCurElement)->m_lx1;
			m_cursor.m_ly=((CText *)m_pCurElement)->m_ly1+m_CurFont.lfHeight+2*DELTA;
			m_pCurElement=new CText();
			((CText *)m_pCurElement)->m_font=m_CurFont;
			((CText *)m_pCurElement)->m_FaceColor=m_FontColor;
			pDoc->m_listElements.AddTail(m_pCurElement);			 
			((CText *)m_pCurElement)->m_lx1=m_cursor.m_lx;
			((CText *)m_pCurElement)->m_ly1=m_cursor.m_ly;
			((CText *)m_pCurElement)->m_lx2=m_cursor.m_lx;
			((CText *)m_pCurElement)->m_ly2=m_cursor.m_ly+m_CurFont.lfHeight;
			m_pCurElement->m_lHitTarget=TK_CT;
			m_lEditMode=ED_EDIT;
			m_lCurCmdId++;
			m_pCurElement->m_lCmdId=m_lCurCmdId;
		}else if(nChar==VK_BACK){
			CString tmpText=((CText *)m_pCurElement)->m_text;
			if(tmpText.GetLength()>1){
				if(IsDBCSLeadByte((UCHAR)tmpText.GetAt(tmpText.GetLength()-1))){
					((CText *)m_pCurElement)->m_text.Delete(((CText *)m_pCurElement)->m_text.GetLength()-1);
				}
				((CText *)m_pCurElement)->m_text.Delete(((CText *)m_pCurElement)->m_text.GetLength()-1);			
			}else{
				((CText *)m_pCurElement)->m_text.Delete(((CText *)m_pCurElement)->m_text.GetLength()-1);			
			}
		}else if( !iscntrl( nChar ) ){
			CString strTemp;
			strTemp.Format(_T("%c"), nChar);
			((CText *)m_pCurElement)->m_text+=strTemp;
		}else if( VK_ESCAPE == nChar ){
			OnDrawPointer();
		}

		if(m_pCurElement) m_pCurElement->DrawTracker(pDC);
		OnDraw(pDC);
		m_cursor.m_lx=((CText *)m_pCurElement)->m_lx2;
		m_cursor.Show(pDC);
	}
	

	pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CMCDrawView::OnTimer(UINT nIDEvent) 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	//LOGFONT tFont;
	//tFont=m_CurFont;
	//CElement::UPtoLP(tFont.lfHeight,tFont.lfWeight,&tFont.lfHeight,&tFont.lfWidth);
	//CFont font;
	//font.CreateFontIndirect(&tFont);
	//CFont *pOldFont=pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);

	switch(nIDEvent){
	case 1:
		if(!m_cursor.m_bActive && !m_cursor.m_bShow){
			KillTimer(1);
		}else{
			if(m_cursor.m_bShow) m_cursor.Hide(pDC);
			else m_cursor.Show(pDC);
		}
		break;
	}
	
	//pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
	CScrollView::OnTimer(nIDEvent);
}

BOOL CMCDrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_lEditMode==ED_NONE && m_lDrawMode==DE_TEXT){
		SetCursor(LoadCursor(NULL,IDC_IBEAM));
		return true;
	}else if(m_lDrawMode==DE_PICTURE || m_lDrawMode==DE_LIBCOMP || m_lDrawMode==DE_TREND || m_lDrawMode==DE_PASTE){
		SetCursor(LoadCursor(NULL,IDC_CROSS));
		return true;
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CMCDrawView::OnColorBrush() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CColorDialog dlg(m_CurBrush.lbColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_CurBrush.lbColor=dlg.GetColor();

		CElement *pElement,*pNew;
		POSITION pos,pos1;
		m_lCurCmdId++;

		if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
			if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
				pDoc->m_listElements.AddTail(pNew);
				m_pCurElement->m_lStatus=ST_EDIT;
				m_pCurElement=pNew;
			}
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
				((CRectangle *)m_pCurElement)->m_brush.lbColor=m_CurBrush.lbColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
				((CPolygon *)m_pCurElement)->m_brush.lbColor=m_CurBrush.lbColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
				((CRoundRect *)m_pCurElement)->m_brush.lbColor=m_CurBrush.lbColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
				((CEllipse *)m_pCurElement)->m_brush.lbColor=m_CurBrush.lbColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
				((CMCButton *)m_pCurElement)->m_brush.lbColor=m_CurBrush.lbColor;
		}

		if(!m_SelElements.IsEmpty()){
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->m_bHaveBrush){
					if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
						pDoc->m_listElements.AddTail(pNew);
						pElement->m_lStatus=ST_EDIT;
						pos1=m_SelElements.Find(pElement);
						m_SelElements.SetAt(pos1,pNew);
					}
				}
			}
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
					((CRectangle *)pElement)->m_brush.lbColor=m_CurBrush.lbColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
					((CPolygon *)pElement)->m_brush.lbColor=m_CurBrush.lbColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
					((CRoundRect *)pElement)->m_brush.lbColor=m_CurBrush.lbColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
					((CEllipse *)pElement)->m_brush.lbColor=m_CurBrush.lbColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
					((CMCButton *)pElement)->m_brush.lbColor=m_CurBrush.lbColor;
			}
	
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnColorFont() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CColorDialog dlg(0,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_FontColor=dlg.GetColor();

		CElement *pElement,*pNew;
		POSITION pos,pos1;
		m_lCurCmdId++;

		if(m_pCurElement!=NULL && m_pCurElement->m_bHaveFont){
			if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
				pDoc->m_listElements.AddTail(pNew);
				m_pCurElement->m_lStatus=ST_EDIT;
				m_pCurElement=pNew;
			}
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText)))
				((CText*)m_pCurElement)->m_FaceColor=m_FontColor;
		}

		if(!m_SelElements.IsEmpty()){
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->m_bHaveFont){
					if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
						pDoc->m_listElements.AddTail(pNew);
						pElement->m_lStatus=ST_EDIT;
						pos1=m_SelElements.Find(pElement);
						m_SelElements.SetAt(pos1,pNew);
					}
				}
			}
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->IsKindOf(RUNTIME_CLASS(CText)))
					((CText*)pElement)->m_FaceColor=m_FontColor;
			}
		}
	}	

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnColorPen() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CColorDialog dlg(m_CurPen.lopnColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		m_CurPen.lopnColor=dlg.GetColor();

		CElement *pElement,*pNew;
		POSITION pos,pos1;
		m_lCurCmdId++;

		if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
			if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
				pDoc->m_listElements.AddTail(pNew);
				m_pCurElement->m_lStatus=ST_EDIT;
				m_pCurElement=pNew;
			}
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
				((CRectangle *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
				((CPolygon *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
				((CRoundRect *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
				((CEllipse *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;

			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine)))
				((CLine *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc)))
				((CArc *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine)))
				((CPolyLine *)m_pCurElement)->m_pen.lopnColor=m_CurPen.lopnColor;
		}

		if(!m_SelElements.IsEmpty()){
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->m_bHavePen){
					if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
						pDoc->m_listElements.AddTail(pNew);
						pElement->m_lStatus=ST_EDIT;
						pos1=m_SelElements.Find(pElement);
						m_SelElements.SetAt(pos1,pNew);
					}
				}
			}
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
					((CRectangle *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
					((CPolygon *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
					((CRoundRect *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
					((CEllipse *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;

				if(pElement->IsKindOf(RUNTIME_CLASS(CLine)))
					((CLine *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CArc)))
					((CArc *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
				if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine)))
					((CPolyLine *)pElement)->m_pen.lopnColor=m_CurPen.lopnColor;
			}
		}
	}	

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnColorWindow() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	CColorDialog dlg(GetDocument()->m_WindowColor,0,NULL);
	if(dlg.DoModal()==IDOK){
		GetDocument()->m_WindowColor=dlg.GetColor();
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine1() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine2() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=12;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);		
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine3() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=24;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);		
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine4() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=36;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);		
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine5() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=48;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnLine6() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=60;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=m_CurPen.lopnWidth.x;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineDash() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_DASH;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineDashdot() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_DASHDOT;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineDashdotdot() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_DASHDOTDOT;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineDot() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	m_CurPen.lopnStyle=PS_DOT;
	m_CurPen.lopnWidth.x=0;

	if(m_pCurElement!=NULL){
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		CElement * pElement;
		POSITION pos;
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineNull() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_NULL;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine))){
			((CLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc))){
			((CArc *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
			((CPolyLine *)m_pCurElement)->m_pen.lopnWidth.x=0;
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnLineSolid() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurPen.lopnStyle=PS_SOLID;
	m_CurPen.lopnWidth.x=0;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHavePen){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
			((CRectangle *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
			((CPolygon *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
			((CRoundRect *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
			((CEllipse *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;

		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CLine)))
			((CLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CArc)))
			((CArc *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolyLine)))
			((CPolyLine *)m_pCurElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHavePen){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_pen.lopnWidth.x=0;
				((CRectangle *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_pen.lopnWidth.x=0;
				((CPolygon *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_pen.lopnWidth.x=0;
				((CRoundRect *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_pen.lopnWidth.x=0;
				((CEllipse *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}

			if(pElement->IsKindOf(RUNTIME_CLASS(CLine))){
				((CLine *)pElement)->m_pen.lopnWidth.x=0;
				((CLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CArc))){
				((CArc *)pElement)->m_pen.lopnWidth.x=0;
				((CArc *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolyLine))){
				((CPolyLine *)pElement)->m_pen.lopnWidth.x=0;
				((CPolyLine *)pElement)->m_pen.lopnStyle=m_CurPen.lopnStyle;
			}
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushBdiagonal() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_BDIAGONAL;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);		
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushCross() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_CROSS;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushDiagcross() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_DIAGCROSS;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushFdiagonal() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_FDIAGONAL;
	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushHorizontal() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_HORIZONTAL;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushNull() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_NULL;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushSolid() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_SOLID;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle)))
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon)))
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect)))
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse)))
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnBrushVertical() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	m_CurBrush.lbStyle=BS_HATCHED;
	m_CurBrush.lbHatch=HS_VERTICAL;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveBrush){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
			((CRectangle *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRectangle *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
			((CPolygon *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CPolygon *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
			((CRoundRect *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CRoundRect *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
			((CEllipse *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CEllipse *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
			((CMCButton *)m_pCurElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
			((CMCButton *)m_pCurElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
		}
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveBrush){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CRectangle))){
				((CRectangle *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRectangle *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CPolygon))){
				((CPolygon *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CPolygon *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CRoundRect))){
				((CRoundRect *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CRoundRect *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CEllipse))){
				((CEllipse *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CEllipse *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
			if(pElement->IsKindOf(RUNTIME_CLASS(CMCButton))){
				((CMCButton *)pElement)->m_brush.lbStyle=m_CurBrush.lbStyle;
				((CMCButton *)pElement)->m_brush.lbHatch=m_CurBrush.lbHatch;
			}
		}
	}

	OnDraw(pDC);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnTextSet() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	LOGFONT tmpFont=m_CurFont;
	tmpFont.lfHeight=tmpFont.lfHeight/8;
	CFontDialog dlg(&tmpFont,CF_SCREENFONTS);

	if(dlg.DoModal()==IDOK){
		dlg.GetCurrentFont(&tmpFont);
		tmpFont.lfHeight=abs(tmpFont.lfHeight*8);
		m_CurFont=tmpFont;

		CElement *pElement,*pNew;
		POSITION pos,pos1;
		m_lCurCmdId++;

		if(m_pCurElement!=NULL && m_pCurElement->m_bHaveFont){
			if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
				pDoc->m_listElements.AddTail(pNew);
				m_pCurElement->m_lStatus=ST_EDIT;
				m_pCurElement=pNew;
			}
			if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))){
				((CText *)m_pCurElement)->m_font.lfHeight=m_CurFont.lfHeight;
				((CText *)m_pCurElement)->m_ly2 =((CText *)m_pCurElement)->m_ly1+m_CurFont.lfHeight;
				((CText *)m_pCurElement)->m_font.lfWeight=m_CurFont.lfWeight;
				((CText *)m_pCurElement)->m_font.lfItalic=m_CurFont.lfItalic;
				((CText *)m_pCurElement)->m_font.lfUnderline=m_CurFont.lfUnderline;
				::memcpy(((CText *)m_pCurElement)->m_font.lfFaceName,m_CurFont.lfFaceName,32);
			}
		}

		if(!m_SelElements.IsEmpty()){
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->m_bHaveFont){
					if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
						pDoc->m_listElements.AddTail(pNew);
						pElement->m_lStatus=ST_EDIT;
						pos1=m_SelElements.Find(pElement);
						m_SelElements.SetAt(pos1,pNew);
					}
				}
			}
			for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
				pElement=(CElement*)(m_SelElements.GetNext(pos));
				if(pElement->IsKindOf(RUNTIME_CLASS(CText))){
					((CText *)pElement)->m_font.lfHeight=m_CurFont.lfHeight;
					((CText *)pElement)->m_ly2 =((CText *)pElement)->m_ly1+m_CurFont.lfHeight;
					((CText *)pElement)->m_font.lfWeight=m_CurFont.lfWeight;
					((CText *)pElement)->m_font.lfItalic=m_CurFont.lfItalic;
					((CText *)pElement)->m_font.lfUnderline=m_CurFont.lfUnderline;
					::memcpy(((CText *)pElement)->m_font.lfFaceName,m_CurFont.lfFaceName,32);
				}
			}
		}

		m_cursor.m_lHeight=m_CurFont.lfHeight;
		if(m_pCurElement!=NULL || !m_SelElements.IsEmpty()){
			OnDraw(pDC);
			if(m_lEditMode==ED_EDIT){
				m_cursor.m_lx=((CText *)m_pCurElement)->m_lx2;
				m_cursor.Show(pDC);
			}
		}
	}

	ReleaseDC(pDC);
}

void CMCDrawView::OnTextBold() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	long lBold;
	if(m_lBold==400) 
		lBold=700;
	else
		lBold=400;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveFont){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText)))
			((CText *)m_pCurElement)->m_font.lfWeight=lBold;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveFont){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText)))
				((CText *)pElement)->m_font.lfWeight=lBold;
		}
	}
	
	if(m_pCurElement!=NULL || !m_SelElements.IsEmpty()){
		OnDraw(pDC);
		if(m_lEditMode==ED_EDIT){
			m_cursor.m_lx=((CText *)m_pCurElement)->m_lx2;
			m_cursor.Show(pDC);
		}
	}

	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateTextBold(CCmdUI* pCmdUI) 
{
	if(m_pCurElement!=NULL){
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))){
			if(((CText *)m_pCurElement)->m_font.lfWeight==400){
				pCmdUI->SetCheck(0);
				m_lBold=400;
			}else{
				pCmdUI->SetCheck();
				m_lBold=700;
			}
		}
		return;
	}

	m_lBold=400;
	if(!m_SelElements.IsEmpty()){
		CElement * pElement;
		POSITION pos;
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText))){
				if(((CText *)pElement)->m_font.lfWeight==700){
					pCmdUI->SetCheck();
					m_lBold=700;
					break;
				}
			}
		}
	}

	if(m_lBold==700)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnTextItalic() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	bool bItalic=!m_bItalic;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveFont){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText)))
			((CText *)m_pCurElement)->m_font.lfItalic=bItalic;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveFont){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText)))
				((CText *)pElement)->m_font.lfItalic=bItalic;
		}
	}
	
	if(m_pCurElement!=NULL || !m_SelElements.IsEmpty()){
		OnDraw(pDC);
		if(m_lEditMode==ED_EDIT){
			m_cursor.m_lx=((CText *)m_pCurElement)->m_lx2;
			m_cursor.Show(pDC);
		}
	}

	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateTextItalic(CCmdUI* pCmdUI) 
{
	if(m_pCurElement!=NULL){
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))){
			if(((CText *)m_pCurElement)->m_font.lfItalic){
				pCmdUI->SetCheck();
				m_bItalic=true;
			}else{
				pCmdUI->SetCheck(0);
				m_bItalic=false;
			}
		}
		return;
	}

	m_bItalic=false;
	if(!m_SelElements.IsEmpty()){
		CElement * pElement;
		POSITION pos;
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText))){
				if(((CText *)pElement)->m_font.lfItalic){
					pCmdUI->SetCheck();
					m_bItalic=true;
					break;
				}
			}
		}
	}

	if(m_bItalic)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnTextUnderline() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	bool bUnderline=!m_bUnderline;

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	if(m_pCurElement!=NULL && m_pCurElement->m_bHaveFont){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText)))
			((CText *)m_pCurElement)->m_font.lfUnderline=bUnderline;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->m_bHaveFont){
				if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
					pDoc->m_listElements.AddTail(pNew);
					pElement->m_lStatus=ST_EDIT;
					pos1=m_SelElements.Find(pElement);
					m_SelElements.SetAt(pos1,pNew);
				}
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText)))
				((CText *)pElement)->m_font.lfUnderline=bUnderline;
		}
	}
	
	if(m_pCurElement!=NULL || !m_SelElements.IsEmpty()){
		OnDraw(pDC);
		if(m_lEditMode==ED_EDIT){
			m_cursor.m_lx=((CText *)m_pCurElement)->m_lx2;
			m_cursor.Show(pDC);
		}
	}

	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateTextUnderline(CCmdUI* pCmdUI) 
{
	if(m_pCurElement!=NULL){
		if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))){
			if(((CText *)m_pCurElement)->m_font.lfUnderline){
				pCmdUI->SetCheck();
				m_bUnderline=true;
			}else{
				pCmdUI->SetCheck(0);
				m_bUnderline=false;
			}
		}
		return;
	}

	m_bUnderline=false;
	if(!m_SelElements.IsEmpty()){
		CElement * pElement;
		POSITION pos;
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if(pElement->IsKindOf(RUNTIME_CLASS(CText))){
				if(((CText *)pElement)->m_font.lfUnderline){
					pCmdUI->SetCheck();
					m_bUnderline=true;
					break;
				}
			}
		}
	}

	if(m_bUnderline)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushBdiagonal(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_BDIAGONAL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushCross(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_CROSS)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushDiagcross(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_DIAGCROSS)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushFdiagonal(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_FDIAGONAL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushHorizontal(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_HORIZONTAL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushNull(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_NULL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushSolid(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_SOLID)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateBrushVertical(CCmdUI* pCmdUI) 
{
	if(m_CurBrush.lbStyle==BS_HATCHED && m_CurBrush.lbHatch==HS_VERTICAL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine1(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==0)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine2(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==60)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine3(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==120)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine4(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==180)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine5(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==240)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLine6(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnWidth.x==360)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineDash(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_DASH)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineDashdot(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_DASHDOT)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineDashdotdot(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_DASHDOTDOT)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineDot(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_DOT)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineNull(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_NULL)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnUpdateLineSolid(CCmdUI* pCmdUI) 
{
	if(m_CurPen.lopnStyle==PS_SOLID)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMCDrawView::OnFormatA2b() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2B);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2c() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2C);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2cm() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2CM);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2l() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2L);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2m() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2M);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2r() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2R);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatA2t() 
{
	if(m_SelElements.GetCount()<2) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement->Left()) left=pElement->Left();
		if(right<pElement->Right()) right=pElement->Right();
		if(top>pElement->Top()) top=pElement->Top();
		if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
	}

	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement *)(m_SelElements.GetNext(pos));
		pElement->Format(left,top,right,bottom,FM_A2T);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatB2b() 
{
	if(m_pCurElement==NULL) return;

	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	//find where to insert
	long lLevel=m_pCurElement->GetLevel();
	CElement *pFather=m_pCurElement,*pPrev,*pChild;
	while(pFather->m_pFather!=NULL && pFather->m_lStatus!=ST_ZORDER){
		pFather=pFather->m_pFather;
	}
	POSITION posCur=pDoc->m_listElements.Find(pFather);
	POSITION posPrev=posCur, posF=NULL, pos;
	(CElement*)pDoc->m_listElements.GetPrev(posPrev);
	if(posPrev!=NULL){
		do{
			pos = posPrev;
			pPrev=(CElement*)pDoc->m_listElements.GetPrev(posPrev);
			if(pPrev->GetLevel()==lLevel && (pPrev->m_pFather==NULL || pPrev->m_lStatus==ST_ZORDER)){
				pChild=pPrev->m_pChild;
				while(pChild!=NULL){
					if(pChild->m_lStatus==ST_ZORDER) break;
					pChild=pChild->m_pChild;
				}
				if(pChild==NULL){
					posF = pos;
				}
			}
		}while(posPrev!=NULL);
	}

	if(posF!=NULL){
		CElement *pNew,*pZOrder;
		m_lCurCmdId++;
		if((pZOrder=m_pCurElement->Clone(m_lCurCmdId,ST_ZORDER)) != NULL){
			m_pCurElement->m_lStatus=ST_EDIT;
			pNew=pZOrder->Clone(m_lCurCmdId,ST_NOMAL);
			m_pCurElement=pNew;
			pDoc->m_listElements.AddTail(pNew);
		}		

		pDoc->m_listElements.InsertBefore(posF,pZOrder);

		CDC *pDC=GetDC();
		ASSERT_VALID(pDC);
		CScrollView::OnPrepareDC(pDC);
		OnDraw(pDC);
		ReleaseDC(pDC);
	}
}

void CMCDrawView::OnFormatB2f() 
{
	if(m_pCurElement==NULL) return;

	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	//find where to insert
	long lLevel=m_pCurElement->GetLevel();
	CElement *pFather=m_pCurElement,*pNext,*pChild;
	while(pFather->m_pFather!=NULL && pFather->m_lStatus!=ST_ZORDER){
		pFather=pFather->m_pFather;
	}
	POSITION posCur=pDoc->m_listElements.Find(pFather);
	POSITION posNext = posCur, posF = NULL, pos;
	(CElement*)pDoc->m_listElements.GetNext(posNext);
	if(posNext!=NULL){
		do{
			pos = posNext;
			pNext=(CElement*)pDoc->m_listElements.GetNext(posNext);
			if(pNext->GetLevel()==lLevel && (pNext->m_pFather==NULL || pNext->m_lStatus==ST_ZORDER)){
				pChild=pNext->m_pChild;
				while(pChild!=NULL){
					if(pChild->m_lStatus==ST_ZORDER) break;
					pChild=pChild->m_pChild;
				}
				if(pChild==NULL){
					posF = pos;
				}
			}
		}while(posNext!=NULL);
	}

	if(posF!=NULL){
		CElement *pNew,*pZOrder;
		m_lCurCmdId++;
		if((pZOrder=m_pCurElement->Clone(m_lCurCmdId,ST_ZORDER)) != NULL){
			m_pCurElement->m_lStatus=ST_EDIT;
			pNew=pZOrder->Clone(m_lCurCmdId,ST_NOMAL);
			m_pCurElement=pNew;
			pDoc->m_listElements.AddTail(pNew);
		}		

		pDoc->m_listElements.InsertAfter(posF,pZOrder);

		CDC *pDC=GetDC();
		ASSERT_VALID(pDC);
		CScrollView::OnPrepareDC(pDC);
		OnDraw(pDC);
		ReleaseDC(pDC);
	}
}

void CMCDrawView::OnFormatE2h() 
{
	if(m_SelElements.GetCount()<3) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1,pos2;
	
	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	CObList OrderElements;
	CElement *pElement1,*pElement2;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(OrderElements.IsEmpty()){
			OrderElements.AddHead(pElement1);
		}else{
			pos2=NULL;
			for(pos1=OrderElements.GetHeadPosition();pos1!=NULL;){
				pElement2=(CElement *)(OrderElements.GetNext(pos1));
				if(pElement1->Left()<pElement2->Left()){
					pos2=OrderElements.Find(pElement2);
					OrderElements.InsertBefore(pos2,pElement1);
					break;
				}
			}
			if(pos2==NULL) OrderElements.AddTail(pElement1);
		}
	}

	long left=TOTAL_WIDTH,right=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(left>pElement1->Left()) left=pElement1->Left();
		if(right<pElement1->Right()) right=pElement1->Right();
	}
	
	long delta=right-left;
	pElement1=(CElement *)(m_SelElements.GetHead());
	left=left+(pElement1->Right()-pElement1->Left())/2;
	delta-=(pElement1->Right()-pElement1->Left())/2;
	pElement1=(CElement *)(OrderElements.GetTail());
	delta-=(pElement1->Right()-pElement1->Left())/2;
	delta=delta/(OrderElements.GetCount()-1);
	int i=0;
	for(pos=OrderElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(i!=0 && i!=OrderElements.GetCount()-1){			
			pElement1->Format(left+delta*i,0,left+delta*i,0,FM_A2C);
		}
		i++;
	}
	
	OrderElements.RemoveAll();

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatE2v() 
{
	if(m_SelElements.GetCount()<3) return;
	
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1,pos2;
	
	m_lCurCmdId++;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
			pDoc->m_listElements.AddTail(pNew);
			pElement->m_lStatus=ST_EDIT;
			pos1=m_SelElements.Find(pElement);
			m_SelElements.SetAt(pos1,pNew);
		}
	}

	CObList OrderElements;
	CElement *pElement1,*pElement2;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(OrderElements.IsEmpty()){
			OrderElements.AddHead(pElement1);
		}else{
			pos2=NULL;
			for(pos1=OrderElements.GetHeadPosition();pos1!=NULL;){
				pElement2=(CElement *)(OrderElements.GetNext(pos1));
				if(pElement1->Top()<pElement2->Top()){
					pos2=OrderElements.Find(pElement2);
					OrderElements.InsertBefore(pos2,pElement1);
					break;
				}
			}
			if(pos2==NULL) OrderElements.AddTail(pElement1);
		}
	}

	long top=TOTAL_HEIGHT,bottom=0;
	for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(top>pElement1->Top()) top=pElement1->Top();
		if(bottom<pElement1->Bottom()) bottom=pElement1->Bottom();
	}

	long delta=bottom-top;
	pElement1=(CElement *)(m_SelElements.GetHead());
	top=top+(pElement1->Bottom()-pElement1->Top())/2;
	delta-=(pElement1->Bottom()-pElement1->Top())/2;
	pElement1=(CElement *)(OrderElements.GetTail());
	delta-=(pElement1->Bottom()-pElement1->Top())/2;
	delta=delta/(OrderElements.GetCount()-1);

	int i=0;
	for(pos=OrderElements.GetHeadPosition();pos!=NULL;){
		pElement1=(CElement *)(m_SelElements.GetNext(pos));
		if(i!=0 && i!=OrderElements.GetCount()-1){			
			pElement1->Format(0,top+delta*i,0,top+delta*i,FM_A2M);
		}
		i++;
	}
	
	OrderElements.RemoveAll();

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatF2i() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	if(m_pCurElement!=NULL){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		left=m_pCurElement->Left();
		right=m_pCurElement->Right();
		top=m_pCurElement->Top();
		bottom=m_pCurElement->Bottom();
		m_pCurElement->Format(left,top,right,bottom,FM_F2I);
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
				pDoc->m_listElements.AddTail(pNew);
				pElement->m_lStatus=ST_EDIT;
				pos1=m_SelElements.Find(pElement);
				m_SelElements.SetAt(pos1,pNew);
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			if(left>pElement->Left()) left=pElement->Left();
			if(right<pElement->Right()) right=pElement->Right();
			if(top>pElement->Top()) top=pElement->Top();
			if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
		}

		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_F2I);
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatF2z() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	if(m_pCurElement!=NULL){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		left=m_pCurElement->Left();
		right=m_pCurElement->Right();
		top=m_pCurElement->Top();
		bottom=m_pCurElement->Bottom();
		m_pCurElement->Format(left,top,right,bottom,FM_F2Z);
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
				pDoc->m_listElements.AddTail(pNew);
				pElement->m_lStatus=ST_EDIT;
				pos1=m_SelElements.Find(pElement);
				m_SelElements.SetAt(pos1,pNew);
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			if(left>pElement->Left()) left=pElement->Left();
			if(right<pElement->Right()) right=pElement->Right();
			if(top>pElement->Top()) top=pElement->Top();
			if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
		}

		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_F2Z);
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatR2u() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_POINTER;	
	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	if(m_pCurElement!=NULL){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		left=m_pCurElement->Left();
		right=m_pCurElement->Right();
		top=m_pCurElement->Top();
		bottom=m_pCurElement->Bottom();
		m_pCurElement->Format(left,top,right,bottom,FM_R2U);
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
				pDoc->m_listElements.AddTail(pNew);
				pElement->m_lStatus=ST_EDIT;
				pos1=m_SelElements.Find(pElement);
				m_SelElements.SetAt(pos1,pNew);
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			if(left>pElement->Left()) left=pElement->Left();
			if(right<pElement->Right()) right=pElement->Right();
			if(top>pElement->Top()) top=pElement->Top();
			if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
		}

		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_R2U);
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatR2w() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos,pos1;
	m_lCurCmdId++;

	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_POINTER;	
	long left=TOTAL_WIDTH,right=0,top=TOTAL_HEIGHT,bottom=0;
	if(m_pCurElement!=NULL){
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}
		left=m_pCurElement->Left();
		right=m_pCurElement->Right();
		top=m_pCurElement->Top();
		bottom=m_pCurElement->Bottom();
		m_pCurElement->Format(left,top,right,bottom,FM_R2W);
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
				pDoc->m_listElements.AddTail(pNew);
				pElement->m_lStatus=ST_EDIT;
				pos1=m_SelElements.Find(pElement);
				m_SelElements.SetAt(pos1,pNew);
			}
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			if(left>pElement->Left()) left=pElement->Left();
			if(right<pElement->Right()) right=pElement->Right();
			if(top>pElement->Top()) top=pElement->Top();
			if(bottom<pElement->Bottom()) bottom=pElement->Bottom();
		}
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			CElement * pElement=(CElement *)(m_SelElements.GetNext(pos));
			pElement->Format(left,top,right,bottom,FM_R2W);
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnFormatS2g() 
{
	if(m_SelElements.IsEmpty() && m_pCurElement==NULL) return;

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	CElement *pElement,*pNew;
	POSITION pos,pos1;

	m_lCurCmdId++;
	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			if((pNew=pElement->Clone(m_lCurCmdId,ST_NOMAL)) != NULL){
				pDoc->m_listElements.AddTail(pNew);
				pElement->m_lStatus=ST_EDIT;
				pos1=m_SelElements.Find(pElement);
				m_SelElements.SetAt(pos1,pNew);
				pNew->Format(0,0,lHzGrid,lVtGrid,FM_S2G);
			}
		}
	}else{
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
			pNew->Format(0,0,lHzGrid,lVtGrid,FM_S2G);
		}
	}

	OnDraw(pDC);
	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateFormatB2b(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL){
		pCmdUI->Enable(false);
	}else{
		pCmdUI->Enable(true);
	}

	/*
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	//find where to insert
	long lLevel=m_pCurElement->GetLevel();
	CElement *pFather=m_pCurElement,*pPrev,*pChild;
	while(pFather->m_pFather!=NULL && pFather->m_lStatus!=ST_ZORDER){
		pFather=pFather->m_pFather;
	}
	POSITION posCur=pDoc->m_listElements.Find(pFather);
	POSITION posPrev=posCur,posPrev1;
	(CElement*)pDoc->m_listElements.GetPrev(posPrev);
	if(posPrev!=NULL){
		do{
			posPrev1=posPrev;
			pPrev=(CElement*)pDoc->m_listElements.GetPrev(posPrev);
			if(pPrev->GetLevel()==lLevel && (pPrev->m_pFather==NULL || pPrev->m_lStatus==ST_ZORDER)){
				pChild=pPrev->m_pChild;
				while(pChild!=NULL){
					if(pChild->m_lStatus==ST_ZORDER) break;
					pChild=pChild->m_pChild;
				}
				if(pChild==NULL){
					posPrev=posPrev1;
					break;
				}
			}
		}while(posPrev!=NULL);
	}

	if(posPrev!=NULL){
		pCmdUI->Enable(true);
	}else{
		pCmdUI->Enable(false);
	}
	*/
}

void CMCDrawView::OnUpdateFormatB2f(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL){
		pCmdUI->Enable(false);
	}else{
		pCmdUI->Enable(true);
	}

	/*
	CMCDrawDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	//find where to insert
	long lLevel=m_pCurElement->GetLevel();
	CElement *pFather=m_pCurElement,*pNext,*pChild;
	while(pFather->m_pFather!=NULL && pFather->m_lStatus!=ST_ZORDER){
		pFather=pFather->m_pFather;
	}
	POSITION posCur=pDoc->m_listElements.Find(pFather);
	POSITION posNext=posCur,posNext1;
	(CElement*)pDoc->m_listElements.GetNext(posNext);
	if(posNext!=NULL){
		do{
			posNext1=posNext;
			pNext=(CElement*)pDoc->m_listElements.GetNext(posNext);
			if(pNext->GetLevel()==lLevel && (pNext->m_pFather==NULL || pNext->m_lStatus==ST_ZORDER)){
				pChild=pNext->m_pChild;
				while(pChild!=NULL){
					if(pChild->m_lStatus==ST_ZORDER) break;
					pChild=pChild->m_pChild;
				}
				if(pChild==NULL){
					posNext=posNext1;
					break;
				}
			}
		}while(posNext!=NULL);
	}

	if(posNext!=NULL){
		pCmdUI->Enable(true);
	}else{
		pCmdUI->Enable(false);
	}
	*/
}


void CMCDrawView::OnUpdateFormatA2b(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2c(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2cm(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2l(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2m(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2r(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatA2t(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<2) pCmdUI->Enable(false);
	else pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatE2h(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<3)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatE2v(CCmdUI* pCmdUI) 
{
	if(m_SelElements.GetCount()<3)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatF2i(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) 
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatF2z(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) 
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatR2u(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) 
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatR2w(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) 
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateFormatS2g(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) 
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnToolsGrid() 
{
	// TODO: Add your command handler code here
	
}

void CMCDrawView::OnToolsMeta() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) return;

	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString sFileName;
	CDiaSave dlg;

	dlg.m_dir = "pic\\*.emf";
	dlg.m_baseDir = "pic";
	dlg.m_wildCards = "*.emf";
	dlg.m_selectMode = 1;
	if(dlg.DoModal()==IDOK){
		sFileName=dlg.m_select+".emf";
	}else{
		return;
	}	

	CMetaFileDC dcMeta;
	if(!dcMeta.CreateEnhanced(NULL,NULL,NULL,"MCDraw.exe\0\0\0")){
		MessageBox("指定文件已经被打开",NULL,MB_ICONERROR|MB_OK);
		return;
	}

	long lLeft=TOTAL_WIDTH,lRight=0,lTop=TOTAL_HEIGHT,lBottom=0;
	
	POSITION pos;//,pos1;
	CElement *pElement;
	g_bPmcUiIsPrinting = TRUE;
	if(!m_SelElements.IsEmpty()){
		//先画没有动态连接的非单元
		for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMeta);
			if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
				while(pElement->m_pChild!=NULL){
					if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
					pElement=pElement->m_pChild;
					if(pElement->m_lStatus==ST_ZORDER) break;
				}
				if(!pElement->IsKindOf(RUNTIME_CLASS(CTrend)) && !pElement->IsKindOf(RUNTIME_CLASS(CCell)) && !pElement->m_DnyAttr.m_Linked){
					if(m_SelElements.Find(pElement)!=NULL){
						if(pElement->Left()<lLeft) lLeft=pElement->Left();
						if(pElement->Right()>lRight) lRight=pElement->Right();
						if(pElement->Top()<lTop) lTop=pElement->Top();
						if(pElement->Bottom()<lBottom) lBottom=pElement->Bottom();
						pElement->Draw(&dcMeta);
					}
				}
			}
		}
		//后画趋势图
		for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMeta);
			if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
				while(pElement->m_pChild!=NULL){
					if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
					pElement=pElement->m_pChild;
					if(pElement->m_lStatus==ST_ZORDER) break;
				}
				if(pElement->IsKindOf(RUNTIME_CLASS(CTrend))){
					if(m_SelElements.Find(pElement)!=NULL){
						if(pElement->Left()<lLeft) lLeft=pElement->Left();
						if(pElement->Right()>lRight) lRight=pElement->Right();
						if(pElement->Top()<lTop) lTop=pElement->Top();
						if(pElement->Bottom()<lBottom) lBottom=pElement->Bottom();
						pElement->Draw(&dcMeta);
					}
				}
			}
		}
		//最后画有动态连接的或单元
		for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));//->Draw(&dcMeta);
			if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
				while(pElement->m_pChild!=NULL){
					if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
					pElement=pElement->m_pChild;
					if(pElement->m_lStatus==ST_ZORDER) break;
				}
				if(pElement->IsKindOf(RUNTIME_CLASS(CCell)) || pElement->m_DnyAttr.m_Linked){
					if(m_SelElements.Find(pElement)!=NULL){
						if(pElement->Left()<lLeft) lLeft=pElement->Left();
						if(pElement->Right()>lRight) lRight=pElement->Right();
						if(pElement->Top()<lTop) lTop=pElement->Top();
						if(pElement->Bottom()<lBottom) lBottom=pElement->Bottom();
						pElement->Draw(&dcMeta);
					}
				}
			}
		}

	}else{
		lLeft=m_pCurElement->Left();
		lRight=m_pCurElement->Right();
		lTop=m_pCurElement->Top();
		lBottom=m_pCurElement->Bottom();
		m_pCurElement->Draw(&dcMeta);
	}

	OnEditDelete();

	//m_lCurCmdId++;
	CMeta *pMeta=new CMeta();
	pMeta->m_lStatus=ST_NOMAL;
	pMeta->m_lCmdId=m_lCurCmdId;
	pMeta->m_sPicFile=sFileName;
	sFileName=theApp.m_path+"\\pic\\"+sFileName;
	HENHMETAFILE hMeta=dcMeta.CloseEnhanced();
	pMeta->m_hMeta=::CopyEnhMetaFile(hMeta,sFileName);
	ENHMETAHEADER emh;
	::GetEnhMetaFileHeader(pMeta->m_hMeta,sizeof(ENHMETAHEADER),&emh);
	RECTL rtMeta=emh.rclBounds;
	CElement::LPtoUP(rtMeta.left,rtMeta.top,&(rtMeta.left),&(rtMeta.top));
	CElement::LPtoUP(rtMeta.right,rtMeta.bottom,&(rtMeta.right),&(rtMeta.bottom));
	pMeta->m_lx1=rtMeta.left;
	pMeta->m_ly1=rtMeta.top;
	pMeta->m_lx2=rtMeta.right;
	pMeta->m_ly2=rtMeta.bottom;
	::DeleteEnhMetaFile(hMeta);
	::DeleteEnhMetaFile(pMeta->m_hMeta);
	m_pCurElement=pMeta;
	pDoc->m_listElements.AddTail(m_pCurElement); 

	OnDraw(pDC);
	ReleaseDC(pDC);

	g_bPmcUiIsPrinting = FALSE;
}

void CMCDrawView::OnToolsMkcell() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) return;

	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//make a cell
	m_lCurCmdId++;
	CCell *pCell=new CCell();

	POSITION pos,pos1;
	CElement *pElement;

	long lElementCount=0;
	if(!m_SelElements.IsEmpty()){
		for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));
			if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
				while(pElement->m_pChild!=NULL){
					if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
					pElement=pElement->m_pChild;
					if(pElement->m_lStatus==ST_ZORDER) break;
				}
				if((pos1=m_SelElements.Find(pElement))!=NULL){
					if(!pElement->IsKindOf(RUNTIME_CLASS(CTrend))){
						pElement=pElement->Clone(0,ST_NOMAL);
						pElement->m_pFather=NULL;
						//属性名就是标记名
						pElement->m_DnyAttr.TagToLab();
						pCell->m_Elements.AddTail(pElement);
						lElementCount++;
					}else{
						m_SelElements.RemoveAt(pos1);
					}
				}
			}
		}
	}else{
		if(!m_pCurElement->IsKindOf(RUNTIME_CLASS(CTrend))){
			pElement=m_pCurElement->Clone(0,ST_NOMAL);
			pElement->m_pFather=NULL;
			//属性名就是标记名
			pElement->m_DnyAttr.TagToLab();
			pCell->m_Elements.AddTail(pElement);
			lElementCount++;
		}else{
			OnDrawPointer();
		}
	}
	
	if(lElementCount==0){
		delete pCell;
		return;
	}

	//delete selected object
	OnEditDelete();

	//m_lCurCmdId++;
	pCell->m_lStatus=ST_NOMAL;
	pCell->m_lCmdId=m_lCurCmdId;
	m_pCurElement=pCell;
	pDoc->m_listElements.AddTail(m_pCurElement);

	OnDraw(pDC);
	ReleaseDC(pDC);	
}

void CMCDrawView::OnToolsUpcell() 
{
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);

	if(m_pCurElement==NULL) return;
	if(!m_pCurElement->IsKindOf(RUNTIME_CLASS(CCell))) return;
	CCell *pCell=(CCell*)m_pCurElement;
	OnEditDelete();

	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos;//,pos1;
	CElement *pElement;
	//m_lCurCmdId++;
	for(pos=pCell->m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pCell->m_Elements.GetNext(pos)));
		pElement=pElement->Clone(m_lCurCmdId,ST_NOMAL);
		pElement->m_pFather=NULL;
		//pElement->m_lHitTarget=TK_CLS;
		pDoc->m_listElements.AddTail(pElement);
		m_SelElements.AddTail(pElement);
	}

	OnDraw(pDC);
	ReleaseDC(pDC);		
}

void CMCDrawView::OnUpdateToolsMeta(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty())
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();	
}

void CMCDrawView::OnUpdateToolsMkcell(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty())
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnUpdateToolsUpcell(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL){
		pCmdUI->Enable(false);
		return;
	}
	
	if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CCell))){
		pCmdUI->Enable();
	}else{
		pCmdUI->Enable(false);
	}
}

void CMCDrawView::OnToolsOptions() 
{
	COptShow pageShow;
	pageShow.m_PixHor=lPixHor;
	pageShow.m_PixVer=lPixVer;
	pageShow.m_ShowGrid=bShowGrid;
	pageShow.m_ZoomMode=lZoomMode;

	COptGrid pageGrid;
	pageGrid.m_HorGrid=lHzGrid;
	pageGrid.m_VerGrid=lVtGrid;
	pageGrid.m_LockOnGrid=bLockOnGrid;

	COptWindow pageWindow;
	pageWindow.m_WndWidth=lWndWidth;
	pageWindow.m_WndHeight=lWndHeight;
	pageWindow.m_WndCaption=sWndCaption;

	CPropertySheet propSheet;
	propSheet.AddPage(&pageShow);
	propSheet.AddPage(&pageGrid);
	propSheet.AddPage(&pageWindow);
	if(propSheet.DoModal()==IDOK){
		lPixHor=pageShow.m_PixHor;
		lPixVer=pageShow.m_PixVer;
		bShowGrid=pageShow.m_ShowGrid;
		lZoomMode=pageShow.m_ZoomMode;

		lHzGrid=pageGrid.m_HorGrid;
		lVtGrid=pageGrid.m_VerGrid;
		bLockOnGrid=pageGrid.m_LockOnGrid;
		
		lWndWidth=pageWindow.m_WndWidth;
		lWndHeight=pageWindow.m_WndHeight;
		sWndCaption=pageWindow.m_WndCaption;

		OnInitialUpdate();
		Invalidate(); 
	}	
}

void CMCDrawView::OnUpdateColorBrush(CCmdUI* pCmdUI) 
{

	CDC *pDC=((CMainFrame *)AfxGetMainWnd())->m_tbColors.GetDC();
	ASSERT_VALID(pDC);

	CBrush *pOldBrush;

	CBrush brush2(m_CurBrush.lbColor);
	pOldBrush=(CBrush *)pDC->SelectObject(&brush2); 
	pDC->Rectangle(3+23,15,19+23,18);

	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateColorFont(CCmdUI* pCmdUI) 
{
	CDC *pDC=((CMainFrame *)AfxGetMainWnd())->m_tbColors.GetDC();
	ASSERT_VALID(pDC);

	CBrush *pOldBrush;
	
	CBrush brush3(m_FontColor);
	pOldBrush=(CBrush *)pDC->SelectObject(&brush3); 
	pDC->Rectangle(3+23*2,15,19+23*2,18);

	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateColorPen(CCmdUI* pCmdUI) 
{
	CDC *pDC=((CMainFrame *)AfxGetMainWnd())->m_tbColors.GetDC();
	ASSERT_VALID(pDC);

	CBrush *pOldBrush;
	
	CBrush brush1(m_CurPen.lopnColor);
	pOldBrush=(CBrush *)pDC->SelectObject(&brush1); 
	pDC->Rectangle(3,15,19,18);

	pDC->SelectObject(pOldBrush);	
	ReleaseDC(pDC);
}

void CMCDrawView::OnUpdateColorWindow(CCmdUI* pCmdUI) 
{
	CDC *pDC=((CMainFrame *)AfxGetMainWnd())->m_tbColors.GetDC();
	ASSERT_VALID(pDC);

	CBrush *pOldBrush;
	
	CMCDrawDoc* pDoc = GetDocument();
	CBrush brush4(pDoc->m_WindowColor);
	pOldBrush=(CBrush *)pDC->SelectObject(&brush4); 
	pDC->Rectangle(3+23*3,15,19+23*3,18);

	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

void CMCDrawView::OnEditDelete() 
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CElement *pElement,*pNew;
	POSITION pos;

	m_lCurCmdId++;

	if(m_pCurElement!=NULL){
		pNew=m_pCurElement->Clone(m_lCurCmdId,ST_DELETE);
		pDoc->m_listElements.AddTail(pNew);
		m_pCurElement->m_lStatus=ST_EDIT;
		m_pCurElement=NULL;
	}

	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement*)(m_SelElements.GetNext(pos));
			pElement->m_lStatus=ST_EDIT;
			pNew=pElement->Clone(m_lCurCmdId,ST_DELETE);
			pDoc->m_listElements.AddTail(pNew);
		}
		m_SelElements.RemoveAll();
	}
	//Invalidate();
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	OnDraw(pDC);
	ReleaseDC(pDC);	
}

void CMCDrawView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if(m_pCurElement!=NULL || !m_SelElements.IsEmpty()){
		pCmdUI->Enable();
	}else{
		pCmdUI->Enable(false);
	}
}

void CMCDrawView::OnEditUndo() 
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_pCurElement=NULL;
	m_SelElements.RemoveAll();

	long lCmdId=0;
	CElement *pElement, *pFather;
	POSITION pos;
	for(pos=pDoc->m_listElements.GetTailPosition();pos!=NULL;){
		pElement=(CElement*)(pDoc->m_listElements.GetPrev(pos));
		if(pElement->m_lStatus==ST_DISCARD) continue;

		if(lCmdId!=0 && lCmdId!=pElement->m_lCmdId)
			break;
		else
			lCmdId=pElement->m_lCmdId;
		
		if(pElement->m_lStatus != ST_EDIT){
			pElement->m_lStatus=ST_DISCARD;
			pFather=pElement->m_pFather;
			if(pFather!=NULL){
				if(pFather->m_lStatus==ST_ZORDER){
					pFather->m_lStatus=ST_DISCARD;
					pFather=pFather->m_pFather;
				}
				pFather->m_lStatus=ST_NOMAL;
				pFather->m_lHitTarget=TK_MOV;
				m_SelElements.AddTail(pFather);
			}
		}
	}

	if(m_SelElements.GetCount()==1){
		m_pCurElement=(CElement *)(m_SelElements.GetAt(m_SelElements.GetHeadPosition()));
		m_SelElements.RemoveAll();
	}
	
	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	OnDraw(pDC);
	ReleaseDC(pDC);	
}

void CMCDrawView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_listElements.IsEmpty())
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable();
}

void CMCDrawView::OnToolsSavecell() 
{
	if(m_pCurElement==NULL) return;
	if(!m_pCurElement->IsKindOf(RUNTIME_CLASS(CCell))) return;

	CString sFileName;
	CDiaSave dlg;

	dlg.m_dir = "lib\\*.gcf";
	dlg.m_baseDir = "lib";
	dlg.m_wildCards = "*.gcf";
	dlg.m_selectMode = 2;

	if(dlg.DoModal()==IDOK){
		sFileName=theApp.m_path+"lib\\"+dlg.m_select+".gcf";
	}else{
		return;
	}	
	/*
	CFileDialog dlg(false,"gcf",sFileName,OFN_OVERWRITEPROMPT,"Cell File(*.gcf)|*.gcf||");
	CString sDir=theApp.m_path+"lib";
	dlg.m_ofn.lpstrInitialDir=(LPCTSTR)sDir;
	if(dlg.DoModal()==IDOK){
		sFileName=dlg.GetPathName();
	}else{
		return;
	}
	*/
	MSXML::IXMLDOMDocumentPtr pDoc;
	
	::CoInitialize(NULL);

	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 AfxMessageBox(er.ErrorMessage());
		 return;
	}
		
	pDoc->loadXML(L"<?xml version=\"1.0\"?><celf></celf>");
	MSXML::IXMLDOMElementPtr pRoot=pDoc->documentElement;;

	CCell *pCell=(CCell*)m_pCurElement;
	//save offset
	_variant_t val;
	val.vt=VT_I4;
	val.lVal=pCell->Left();
	pRoot->setAttribute(L"x",val);
	val.lVal=pCell->Top();
	pRoot->setAttribute(L"y",val);
	//save all element
	POSITION pos;
	CElement *pElement;
	for(pos=pCell->m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pCell->m_Elements.GetNext(pos)));
		pElement->Save(pRoot);
	}	 

	pDoc->save(sFileName.AllocSysString());
	pDoc.Release();	
}

void CMCDrawView::OnUpdateToolsSavecell(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL){
		pCmdUI->Enable(false);
		return;
	}
	
	if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CCell))){
		pCmdUI->Enable();
	}else{
		pCmdUI->Enable(false);
	}
}

void CMCDrawView::OnEditCopy() 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) return;

	MSXML::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 AfxMessageBox(er.ErrorMessage());
		 return;
	}
		
	POSITION pos;
	CElement *pElement;
	long x=TOTAL_WIDTH,y=TOTAL_HEIGHT;
	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(m_SelElements.GetNext(pos)));
			if(x>pElement->Left()) x=pElement->Left();
			if(y>pElement->Top()) y=pElement->Top();
		}	 
	}else{
		x=m_pCurElement->Left();
		y=m_pCurElement->Top();
	}
	//save offset
	pDoc->loadXML(L"<?xml version=\"1.0\"?><gos></gos>");
	MSXML::IXMLDOMElementPtr pRoot=pDoc->documentElement;
	_variant_t val;
	val.vt=VT_I4;
	val.lVal=x;
	pRoot->setAttribute(L"x",val);
	val.lVal=y;
	pRoot->setAttribute(L"y",val);
	//save all element
	if(!m_SelElements.IsEmpty()){
		for(pos=m_SelElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(m_SelElements.GetNext(pos)));
			pElement->Save(pRoot);
		}
	}else if(m_pCurElement){
		m_pCurElement->Save(pRoot);
	}

	//place gos into clipbord
	_bstr_t sText=pDoc->Getxml();
	//为什么要乘 2 ? 为了应付汉字
	HGLOBAL hText=::GlobalAlloc(GMEM_SHARE,sText.length()*2+1);
	LPSTR pText=(LPSTR)::GlobalLock(hText);
	ASSERT(pText);
	strcpy(pText,sText);
	::GlobalUnlock(hText);
	//afxDump<<pText<<"\n";
	COleDataSource *pSource=new COleDataSource();
	pSource->CacheGlobalData(m_cf,hText);
	if(pSource) pSource->SetClipboard();

	pDoc.Release();		
}

void CMCDrawView::OnEditText() 
{
	if(m_pCurElement==NULL) return;
	if(!m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))) return;
	
	CDiaReplace dia;
	dia.m_original=((CText*)m_pCurElement)->m_text;
	dia.m_replaced=((CText*)m_pCurElement)->m_text;
	if(dia.DoModal()==IDOK){
		CMCDrawDoc *pDoc=GetDocument();
		ASSERT_VALID(pDoc);
		
		CElement *pNew;
		m_lCurCmdId++;
		if((pNew=m_pCurElement->Clone(m_lCurCmdId,ST_NOMAL))!=NULL){
			pDoc->m_listElements.AddTail(pNew);
			m_pCurElement->m_lStatus=ST_EDIT;
			m_pCurElement=pNew;
		}

		((CText*)m_pCurElement)->m_text=dia.m_replaced;

		CDC *pDC=GetDC();
		ASSERT_VALID(pDC);
		CScrollView::OnPrepareDC(pDC);
		OnDraw(pDC);	
		ReleaseDC(pDC);
	}
}

void CMCDrawView::OnUpdateEditText(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL){
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_pCurElement->IsKindOf(RUNTIME_CLASS(CText))){
		pCmdUI->Enable();
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCDrawView::OnDrawTrend() 
{
	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_TREND;
}

void CMCDrawView::OnUpdateDrawTrend(CCmdUI* pCmdUI) 
{
	if(m_lDrawMode==DE_TREND){
		pCmdUI->SetCheck(1);
	}else{
		pCmdUI->SetCheck(0);
	}
}

void CMCDrawView::OnEditPaste() 
{
	COleDataObject dataObject;
	VERIFY(dataObject.AttachClipboard());

#define SETFORMATETC(fe,cf,asp,td,med,li)\
	((fe).cfFormat=cf,\
	(fe).dwAspect=asp,\
	(fe).ptd=td,\
	(fe).tymed=med,\
	(fe).lindex=li)
	
	FORMATETC fmt;
	STGMEDIUM stg;
	SETFORMATETC(fmt,m_cf,DVASPECT_CONTENT,NULL,TYMED_HGLOBAL,-1);
	if(dataObject.GetData(m_cf,&stg,&fmt)){
		HGLOBAL hText=stg.hGlobal;
		CString sText=(LPSTR)::GlobalLock(hText);
		m_paste=sText;
		//afxDump<<sText<<"\n";
		::GlobalUnlock(hText);
	}

	m_lEditMode=ED_NONE;
	m_lDrawMode=DE_PASTE;
}

void CMCDrawView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	COleDataObject dataObject;
	VERIFY(dataObject.AttachClipboard());

	if(dataObject.IsDataAvailable(m_cf)){
		pCmdUI->Enable();
		if(m_lDrawMode==DE_PASTE){
			pCmdUI->SetCheck();
		}else{
			pCmdUI->SetCheck(0);
		}
	}else{
		pCmdUI->Enable(FALSE);
	}
	
}

void CMCDrawView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()){
		pCmdUI->Enable(FALSE);
	}else{
		pCmdUI->Enable();
	}
}

void CMCDrawView::OnSelectAll()
{
	CMCDrawDoc* pDoc = GetDocument();
	CElement * pElement;
	m_SelElements.RemoveAll();
	POSITION pos;
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=(CElement*)(m_SelElements.GetNext(pos));
		m_SelElements.AddTail(pElement);
	}
	InvalidateRect(0, FALSE);
}

void CMCDrawView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMainFrame *pMainWnd=(CMainFrame*)(theApp.GetMainWnd());
	CMenu mnuPop;
	mnuPop.LoadMenu(IDR_POPUP);
	CMenu * pSubMenu=NULL;
	//pen
	pSubMenu=mnuPop.GetSubMenu(0)->GetSubMenu(0);
	pSubMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&pMainWnd->m_bmpLine1d,&pMainWnd->m_bmpLine1a);
	pSubMenu->SetMenuItemBitmaps(1,MF_BYPOSITION,&pMainWnd->m_bmpLine2d,&pMainWnd->m_bmpLine2a);
	pSubMenu->SetMenuItemBitmaps(2,MF_BYPOSITION,&pMainWnd->m_bmpLine3d,&pMainWnd->m_bmpLine3a);
	pSubMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&pMainWnd->m_bmpLine4d,&pMainWnd->m_bmpLine4a);
	pSubMenu->SetMenuItemBitmaps(4,MF_BYPOSITION,&pMainWnd->m_bmpLine5d,&pMainWnd->m_bmpLine5a);
	pSubMenu->SetMenuItemBitmaps(5,MF_BYPOSITION,&pMainWnd->m_bmpLine6d,&pMainWnd->m_bmpLine6a);
	pSubMenu->SetMenuItemBitmaps(8,MF_BYPOSITION,&pMainWnd->m_bmpSolidd,&pMainWnd->m_bmpSolida);
	pSubMenu->SetMenuItemBitmaps(9,MF_BYPOSITION,&pMainWnd->m_bmpDotd,&pMainWnd->m_bmpDota);
	pSubMenu->SetMenuItemBitmaps(10,MF_BYPOSITION,&pMainWnd->m_bmpDashd,&pMainWnd->m_bmpDasha);
	pSubMenu->SetMenuItemBitmaps(11,MF_BYPOSITION,&pMainWnd->m_bmpDashdotd,&pMainWnd->m_bmpDashdota);
	pSubMenu->SetMenuItemBitmaps(12,MF_BYPOSITION,&pMainWnd->m_bmpDashdotdotd,&pMainWnd->m_bmpDashdotdota);
	//brush
	pSubMenu=mnuPop.GetSubMenu(0)->GetSubMenu(1);
	pSubMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&pMainWnd->m_bmpBsolidd,&pMainWnd->m_bmpBsolida);
	pSubMenu->SetMenuItemBitmaps(1,MF_BYPOSITION,&pMainWnd->m_bmpBnulld,&pMainWnd->m_bmpBnulla);
	pSubMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&pMainWnd->m_bmpHorizontald,&pMainWnd->m_bmpHorizontala);
	pSubMenu->SetMenuItemBitmaps(4,MF_BYPOSITION,&pMainWnd->m_bmpVerticald,&pMainWnd->m_bmpVerticala);
	pSubMenu->SetMenuItemBitmaps(5,MF_BYPOSITION,&pMainWnd->m_bmpFdiagonald,&pMainWnd->m_bmpFdiagonala);
	pSubMenu->SetMenuItemBitmaps(6,MF_BYPOSITION,&pMainWnd->m_bmpBdiagonald,&pMainWnd->m_bmpBdiagonala);
	pSubMenu->SetMenuItemBitmaps(7,MF_BYPOSITION,&pMainWnd->m_bmpCrossd,&pMainWnd->m_bmpCrossa);
	pSubMenu->SetMenuItemBitmaps(8,MF_BYPOSITION,&pMainWnd->m_bmpDiagcrossd,&pMainWnd->m_bmpDiagcrossa);
	CRect rectWnd;
	GetWindowRect(rectWnd);
	mnuPop.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		rectWnd.left+point.x,rectWnd.top+point.y,this);

	OnDrawPointer();
	CScrollView::OnRButtonDown(nFlags, point);
}

void CMCDrawView::OnEditCut() 
{
	OnEditCopy();
	OnEditDelete();
}

void CMCDrawView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()){
		pCmdUI->Enable(FALSE);
	}else{
		pCmdUI->Enable();
	}
}

void CMCDrawView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{	 
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CMCDrawView::OnToolsLock() 
{
	if(m_pCurElement==NULL && m_SelElements.IsEmpty()) return;

	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	POSITION pos,pos1;
	CElement *pElement;

	long lElementCount=0;
	if(!m_SelElements.IsEmpty()){
		for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));
			if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
				while(pElement->m_pChild!=NULL){
					if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
					pElement=pElement->m_pChild;
					if(pElement->m_lStatus==ST_ZORDER) break;
				}
				if((pos1=m_SelElements.Find(pElement))!=NULL){
					pElement->m_locked = true;
					pElement->m_lHitTarget = TK_NONE;
				}
			}
		}
	}else{
		m_pCurElement->m_locked = true;
		m_pCurElement->m_lHitTarget = TK_NONE;
	}
	
	m_SelElements.RemoveAll();
	m_pCurElement = NULL;
	OnDrawPointer();

	CDC *pDC=GetDC();
	ASSERT_VALID(pDC);
	CScrollView::OnPrepareDC(pDC);
	OnDraw(pDC);
	ReleaseDC(pDC);	
}

void CMCDrawView::OnToolsUnlock() 
{
	CMCDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos;
	CElement *pElement;
	for(pos=pDoc->m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(pDoc->m_listElements.GetNext(pos)));
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			pElement->m_locked = false;
		}
	}	
}

void CMCDrawView::OnShowAllAttribs() 
{
	// TODO: Add your command handler code here
	
}

