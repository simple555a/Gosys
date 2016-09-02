// Polygon.h: interface for the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGON_H__FD04858B_D3CC_4C76_A31E_11EAD3286638__INCLUDED_)
#define AFX_POLYGON_H__FD04858B_D3CC_4C76_A31E_11EAD3286638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"
#include <afxtempl.h>

class PMCUI_API CPolygon : public CElement  
{
	DECLARE_DYNAMIC(CPolygon)
public:
	virtual void MoveInCell(POINT potTracker);
	void Load(MSXML::IXMLDOMElementPtr pGO);
	virtual void Save(MSXML::IXMLDOMElementPtr pRoot);
	virtual CElement * Clone(long lCmdId,long lStatus);
	virtual void Format(long left,long top,long right,long bottom,long flag);
	virtual long Bottom();
	virtual long Top();
	virtual long Right();
	virtual long Left();
	virtual bool RInR(LPCRECT lpRect);
	virtual long HitTest(long x,long y);
	virtual void DrawHotPoints(CDC *pDC);
	virtual void DrawTracker(CDC *pDC);
	virtual void Draw(CDC *pDC);
	LOGBRUSH m_brush;
	LOGPEN m_pen;
	CArray<POINT,POINT> m_ptArray;
	CPolygon();
	virtual ~CPolygon();

};

#endif // !defined(AFX_POLYGON_H__FD04858B_D3CC_4C76_A31E_11EAD3286638__INCLUDED_)
