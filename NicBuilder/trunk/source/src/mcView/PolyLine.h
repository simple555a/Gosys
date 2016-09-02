// PolyLine.h: interface for the CPolyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINE_H__1B4CBA0E_647A_4931_8A70_FD4586A0178F__INCLUDED_)
#define AFX_POLYLINE_H__1B4CBA0E_647A_4931_8A70_FD4586A0178F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"
#include <afxtempl.h>

class CPolyLine : public CElement  
{
	DECLARE_DYNAMIC(CPolyLine)
public:
	virtual void UpdateView();
	virtual void MoveElement(POINT potOffset);
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
	LOGPEN m_pen;
	CArray<POINT,POINT> m_ptArray;
	LOGPEN m_open;
	CArray<POINT,POINT> m_ptoArray;
	CPolyLine();
	virtual ~CPolyLine();
};

#endif // !defined(AFX_POLYLINE_H__1B4CBA0E_647A_4931_8A70_FD4586A0178F__INCLUDED_)
