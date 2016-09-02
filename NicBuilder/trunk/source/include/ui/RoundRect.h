// RoundRect.h: interface for the CRoundRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDRECT_H__1268F87B_D5F3_4D91_AC3F_FE056D885748__INCLUDED_)
#define AFX_ROUNDRECT_H__1268F87B_D5F3_4D91_AC3F_FE056D885748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class PMCUI_API CRoundRect : public CElement  
{
	DECLARE_DYNAMIC(CRoundRect)
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
	float m_fh;
	float m_fw;
	virtual void DrawTracker(CDC *pDC);
	virtual void Draw(CDC *pDC);
	LOGBRUSH m_brush;
	LOGPEN m_pen;
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;
	CRoundRect();
	virtual ~CRoundRect();

};

#endif // !defined(AFX_ROUNDRECT_H__1268F87B_D5F3_4D91_AC3F_FE056D885748__INCLUDED_)
