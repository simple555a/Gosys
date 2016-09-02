// TestBtn.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "TestBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestBtn

CTestBtn::CTestBtn()
{
	m_BoundryPen.CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, RGB(0, 0, 0));
	m_InsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(250, 196, 88)); 
	m_InsideBoundryPenRight.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(251, 202, 106));
	m_InsideBoundryPenTop.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(252, 210, 121));
	m_InsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(229, 151, 0));

 	m_FillActive.CreateSolidBrush(RGB(223, 222, 236));
 	m_FillInactive.CreateSolidBrush(RGB(222, 223, 236));
	
	m_InsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(153, 198, 252)); 
	m_InsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
	m_InsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(162, 189, 252));
	m_InsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));

	m_RgbFill1 = m_RgbFill2 = m_RgbFill3 = 255;
	
	m_bOver = m_bSelected = m_bTracking = m_bFocus = FALSE;
}




CTestBtn::~CTestBtn()
{
	m_BoundryPen.DeleteObject();
	m_InsideBoundryPenLeft.DeleteObject();
	m_InsideBoundryPenRight.DeleteObject();
	m_InsideBoundryPenTop.DeleteObject();
	m_InsideBoundryPenBottom.DeleteObject();
	
	m_FillActive.DeleteObject();
	m_FillInactive.DeleteObject();
	
	m_InsideBoundryPenLeftSel.DeleteObject();
	m_InsideBoundryPenTopSel.DeleteObject();
	m_InsideBoundryPenRightSel.DeleteObject();
	m_InsideBoundryPenBottomSel.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTestBtn, CButton)
	//{{AFX_MSG_MAP(CTestBtn)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBtn message handlers
void CTestBtn::setColor(COLORREF color1,COLORREF color2,int m_Rgb1,int m_Rgb2,int m_Rgb3)
{
	ActiveColor = color1;
	InactiveColor = color2;
	m_RgbFill1 = m_Rgb1;
	m_RgbFill2 = m_Rgb2;
	m_RgbFill3 = m_Rgb3;

	m_FillActive.DeleteObject();
	m_FillInactive.DeleteObject();

 	m_FillActive.CreateSolidBrush(ActiveColor);
 	m_FillInactive.CreateSolidBrush(InactiveColor);

// 	m_BoundryPen.CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, RGB(0, 0, 0));
// 	m_InsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(250, 196, 88)); 
// 	m_InsideBoundryPenRight.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(251, 202, 106));
// 	m_InsideBoundryPenTop.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(252, 210, 121));
// 	m_InsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(229, 151, 0));	
// 	
// 	m_InsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(153, 198, 252)); 
// 	m_InsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
// 	m_InsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(162, 189, 252));
// 	m_InsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
}

// void CTestBtn::setColor(COLORREF color1,COLORREF color2)
// {
// 	ActiveColor = color1;
// 	InactiveColor = color2;
// 	
// 	
// 	m_FillActive.CreateSolidBrush(ActiveColor);
// 	m_FillInactive.CreateSolidBrush(InactiveColor);
// 	
// 	m_BoundryPen.CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, RGB(0, 0, 0));
// 	m_InsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(250, 196, 88)); 
// 	m_InsideBoundryPenRight.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(251, 202, 106));
// 	m_InsideBoundryPenTop.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(252, 210, 121));
// 	m_InsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(229, 151, 0));	
// 	
// 	m_InsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(153, 198, 252)); 
// 	m_InsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
// 	m_InsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(162, 189, 252));
// 	m_InsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
// }

void CTestBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}


void CTestBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)   
{

	//��lpDrawItemStruct��ȡ�ؼ��������Ϣ
	CRect rect =  lpDrawItemStruct->rcItem;				//�ռ���ռ��������
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC = pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;			//�ؼ�״̬
	POINT pt ;
	TCHAR strText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, strText, MAX_PATH);
	
	//����ť����߿�����һ���뾶Ϊ5��Բ�Ǿ���
	pt.x = 5;
	pt.y = 5;
	CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
	pDC->RoundRect(&rect, pt);
	
	//��ȡ��ť��״̬
	if (state & ODS_FOCUS)
	{
		m_bFocus = TRUE;
		m_bSelected = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
		m_bSelected = FALSE;
	}
	
	
	if (state & ODS_SELECTED || state & ODS_DEFAULT)
	{
		m_bFocus = TRUE;
	}
	
	pDC->SelectObject(hOldPen);
	
	rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
	
	//���ݰ�ť��״̬��䰴ť�ĵ�ɫ
	CBrush* pOldBrush;
	if (m_bOver)
	{
		pOldBrush = pDC->SelectObject(&m_FillActive);
		DoGradientFill(pDC, &rect);
	}
	else
	{
		pOldBrush = pDC->SelectObject(&m_FillInactive);
		DoGradientFill(pDC, &rect);
	}
	
	//���ݰ�ť��״̬�����ڱ߿�
	if (m_bOver || m_bSelected)
		DrawInsideBorder(pDC, &rect);
				
	pDC->SelectObject(pOldBrush);
	
	//��ʾ��ť���ı�
	if (strText!=NULL)
	{
		CFont* hFont = GetFont();
		CFont* hOldFont = pDC->SelectObject(hFont);
		CSize szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
		CPoint pt( rect.CenterPoint().x - szExtent.cx / 2, rect.CenterPoint().y - szExtent.cy / 2);
		if (state & ODS_SELECTED) 
			pt.Offset(1, 1);
		int nMode = pDC->SetBkMode(TRANSPARENT);
		if (state & ODS_DISABLED)
			pDC->DrawState(pt, szExtent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
	
	
	pDC->RestoreDC(nSaveDC);

}

void CTestBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking) 
	{ 
		TRACKMOUSEEVENT tme; tme.cbSize = sizeof(tme); 
		tme.hwndTrack = m_hWnd; tme.dwFlags = TME_LEAVE | TME_HOVER; 
		tme.dwHoverTime = 1; 
		m_bTracking = _TrackMouseEvent(&tme); 
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CTestBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}

LRESULT CTestBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return 0;
}


// void CTestBtn::DoGradientFill(CDC *pDC, CRect* rect)
// {
// 	CBrush brBk[64];
// 	int nWidth = rect->Width();	
// 	int nHeight = rect->Height();
// 	CRect rct;
// 	
// 	for (int i = 0; i < 64; i ++)
// 	{
// 		if (m_bOver)
// 		{
// 			if (m_bFocus)
// 				brBk[i].CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 3)));
// 			else
// 				brBk[i].CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 5)));
// 		}
// 		else
// 		{
// 			if (m_bFocus)
// 				brBk[i].CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 4)));
// 			else
// 				brBk[i].CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 5)));
// 		}
// 	}
// 	
// 	for (i = rect->top; i <= nHeight + 2; i ++) 
// 	{
// 		rct.SetRect(rect->left, i, nWidth + 2, i + 1);
// 		pDC->FillRect(&rct, &brBk[((i * 63) / nHeight)]);
// 	}
// 	
// 	for (i = 0; i < 64; i ++)
// 		brBk[i].DeleteObject();
// }

void CTestBtn::DoGradientFill(CDC *pDC, CRect* rect)
{
	CBrush brBk[64];
	int nWidth = rect->Width();	
	int nHeight = rect->Height();
	CRect rct;
	
	for (int i = 0; i < 64; i ++)
	{
		if (m_bOver)
		{
			if (m_bFocus)
				brBk[i].CreateSolidBrush(RGB(m_RgbFill1 - (i / 4), m_RgbFill2 - (i / 4), m_RgbFill3 - (i / 3)));
			else
				brBk[i].CreateSolidBrush(RGB(m_RgbFill1 - (i / 4), m_RgbFill2 - (i / 4), m_RgbFill3 - (i / 5)));
		}
		else
		{
			if (m_bFocus)
				brBk[i].CreateSolidBrush(RGB(m_RgbFill1 - (i / 3), m_RgbFill2 - (i / 3), m_RgbFill3 - (i / 4)));
			else
				brBk[i].CreateSolidBrush(RGB(m_RgbFill1 - (i / 3), m_RgbFill2 - (i / 3), m_RgbFill3 - (i / 5)));
		}
	}
	
	for (int i = rect->top; i <= nHeight + 2; i ++) 
	{
		rct.SetRect(rect->left, i, nWidth + 2, i + 1);
		pDC->FillRect(&rct, &brBk[(((i-2) * 63) / nHeight)]);
	}
	
	for (int i = 0; i < 64; i ++)
		brBk[i].DeleteObject();
}

// void CTestBtn::DoGradientFill(CDC *pDC, CRect* rect)
// {
// 	CBrush brBk;
// 	int nWidth = rect->Width();	
// 	int nHeight = rect->Height();
// 	CRect rct;
// 	
// 	
// 		if (m_bOver)
// 		{
// 			if (m_bFocus)
// 				brBk.CreateSolidBrush(ActiveColor);
// 			else
// 				brBk.CreateSolidBrush(ActiveColor);
// 		}
// 		else
// 		{
// 			if (m_bFocus)
// 				brBk.CreateSolidBrush(ActiveColor);
// 			else
// 				brBk.CreateSolidBrush(ActiveColor);
// 		}
// 	
// 	
// 
// 		rct.SetRect(rect->left,rect->top, nWidth + 2,nHeight+3);
// 		pDC->FillRect(&rct, &brBk);
// 
// 	
// 
// 		brBk.DeleteObject();
// }


//���ư�ť���ڱ߿�
void CTestBtn::DrawInsideBorder(CDC *pDC, CRect* rect)
{
	CPen *pLeft, *pRight, *pTop, *pBottom;
	
	if (m_bSelected && !m_bOver)
	{
		pLeft = & m_InsideBoundryPenLeftSel;
		pRight = &m_InsideBoundryPenRightSel;
		pTop = &m_InsideBoundryPenTopSel;
		pBottom = &m_InsideBoundryPenBottomSel;
	}
	else
	{
		pLeft = &m_InsideBoundryPenLeft;
		pRight = &m_InsideBoundryPenRight;
		pTop = &m_InsideBoundryPenTop;
		pBottom = &m_InsideBoundryPenBottom;
	}
	
	CPoint oldPoint = pDC->MoveTo(rect->left, rect->bottom - 1);
	CPen* pOldPen = pDC->SelectObject(pLeft);
	pDC->LineTo(rect->left, rect->top + 1);
	pDC->SelectObject(pRight);
	pDC->MoveTo(rect->right - 1, rect->bottom - 1);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pTop);
	pDC->MoveTo(rect->left - 1, rect->top);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pBottom);
	pDC->MoveTo(rect->left, rect->bottom);
	pDC->LineTo(rect->right - 1, rect->bottom);
	pDC->SelectObject(pOldPen);
	pDC->MoveTo(oldPoint);
	
	if (m_bSelected && !m_bOver)
		DrawFocusRect(pDC->m_hDC,rect);
}
