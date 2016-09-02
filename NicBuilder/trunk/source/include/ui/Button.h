// Button.h: interface for the CButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTON_H__9278FC1B_B093_47CD_9E0C_496689D4A103__INCLUDED_)
#define AFX_BUTTON_H__9278FC1B_B093_47CD_9E0C_496689D4A103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class PMCUI_API CMCButton : public CElement  
{
	DECLARE_DYNAMIC(CMCButton)
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
	LOGBRUSH m_brush;
	BOOL m_bPush;
	virtual void DrawTracker(CDC *pDC);
	virtual void Draw(CDC *pDC);
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;
	CMCButton();
	virtual ~CMCButton();

};

#endif // !defined(AFX_BUTTON_H__9278FC1B_B093_47CD_9E0C_496689D4A103__INCLUDED_)
