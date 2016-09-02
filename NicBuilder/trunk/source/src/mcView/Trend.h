// Trend.h: interface for the CTrend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_)
#define AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "element.h"
#include <hisproxy.h>

#define TREND_COUNT 10

const long clTrendMinWidth=1600;
const long clTrendMinHeight=600;

class CTrend : public CElement	
{
	DECLARE_DYNAMIC(CTrend)

	RTK_HANDLE m_cache;	// this object holds the archive data
	pmc_value_t m_vals[5][180];
	unsigned int i_buf;

public:
	void config();
	void ResetTime(FILETIME ftm,int mode);
	void ScrlRight();
	void ScrlLeft();
	void autoScroll();
	void initCache();
	void DrawCurve(CDC *pDC);
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
	
	virtual long Bottom();
	virtual long Top();
	virtual long Right();
	virtual long Left();
	
	CString m_tagnames[TREND_COUNT];
	float m_fMin[TREND_COUNT];
	float m_fMax[TREND_COUNT];
	bool m_bAnalog[TREND_COUNT];
	bool m_autoScale;
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;

protected:
	COLORREF m_BkColor;
	COLORREF m_GdColor;
	LOGPEN m_pens[TREND_COUNT];
	long m_lMode;

	archive_cache_info_t m_cacheInfo;
public:
	long m_posX;
	long m_posY;
	FILETIME m_startTime;
public:
	CTrend();
	virtual ~CTrend();

private:
	void bufValues();
	void ShowValue(CDC *pDC);
};

#endif // !defined(AFX_TREND_H__5F997ED8_7BE1_471D_92CB_5DA5CCAD7E06__INCLUDED_)
