// Trend.h: interface for the CTrend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_)
#define AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

const long clTrendMinWidth=1600;
const long clTrendMinHeight=1000;

class PMCUI_API CTrend : public CElement  
{
	DECLARE_DYNAMIC(CTrend)
public:
	void drawH(CDC *pDC);
	void drawV(CDC *pDC);

	virtual long GetLevel();
	virtual void Save(MSXML::IXMLDOMElementPtr pRoot);
	virtual void Load(MSXML::IXMLDOMElementPtr pGO);
	virtual CElement * Clone(long lCmdId, long lStatus);
	virtual bool RInR(LPCRECT lpRect);
	virtual void MoveInCell(POINT potTracker);
	virtual long HitTest(long x, long y);
	virtual void Format(long left, long top, long right, long bottom, long flag);
	virtual void DrawTracker(CDC *pDC);
	virtual void Draw(CDC *pDC);
	virtual void DrawHotPoints(CDC *pDC);
	COLORREF m_BkColor;
	COLORREF m_GdColor;
	LOGPEN m_pens[10];
	long m_lMode;
	//int m_direction;
	CString m_tagnames[10];
	float m_fMin[10];
	float m_fMax[10];
	bool m_bAnalog[10];
	virtual long Bottom();
	virtual long Top();
	virtual long Right();
	virtual long Left();
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;
	CTrend();
	virtual ~CTrend();

};

#endif // !defined(AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_)
