// Rectangle.h: interface for the CRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTANGLE_H__2F198009_1877_450C_A08F_BA440A6DA7CD__INCLUDED_)
#define AFX_RECTANGLE_H__2F198009_1877_450C_A08F_BA440A6DA7CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CRectangle : public CElement  
{
	DECLARE_DYNAMIC(CRectangle)
public:
	virtual void MoveElement(POINT potOffset);
	virtual void UpdateView();
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
	LOGBRUSH m_obrush;
	LOGPEN m_pen;
	LOGPEN m_open;
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;
	long m_loy2;
	long m_lox2;
	long m_loy1;
	long m_lox1;
	CRectangle();
	virtual ~CRectangle();

};

#endif // !defined(AFX_RECTANGLE_H__2F198009_1877_450C_A08F_BA440A6DA7CD__INCLUDED_)
