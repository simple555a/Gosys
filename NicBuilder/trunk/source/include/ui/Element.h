// Element.h: interface for the CElement class.
//
//////////////////////////////////////////////////////////////////////
#include "math.h"
#include "Dnyattr.h"	// Added by ClassView

#if !defined(AFX_ELEMENT_H__85A2554F_0154_4B85_9583_349C326E3DA2__INCLUDED_)
#define AFX_ELEMENT_H__85A2554F_0154_4B85_9583_349C326E3DA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GD_HZ TOTAL_WIDTH/100
#define GD_VT TOTAL_WIDTH/100//TOTAL_WIDTH/60
#define DELTA TOTAL_WIDTH/160

#define DE_POINTER 1
#define DE_LINE 2
#define DE_ARC 3
#define DE_POLYLINE 4
#define DE_BENDLINE 5
#define DE_RECTANGLE 6
#define DE_ROUNDRECT 7
#define DE_ELLIPSE 8
#define DE_POLYGON 9
#define DE_TEXT 10
#define DE_BUTTON 11
#define DE_PICTURE 12
#define DE_LIBCOMP 13
#define DE_TREND 14
#define DE_PASTE 15

#define TK_CLS 100
#define TK_MOV 99
#define TK_NONE 0
#define TK_LT 1
#define TK_LM 2
#define TK_LB 3
#define TK_BM 4
#define TK_RB 5
#define TK_RM 6
#define TK_RT 7
#define TK_TM 8
#define TK_SP 9
#define TK_EP 10
#define TK_CT 11

#define FM_A2L 3
#define FM_A2C 4
#define FM_A2R 5
#define FM_A2T 6
#define FM_A2M 7
#define FM_A2B 8
#define FM_A2CM 9
#define FM_R2W 12
#define FM_R2U 13
#define FM_F2Z 14
#define FM_F2I 15
#define FM_S2G 16

#define ST_NOMAL 0
#define ST_EDIT 1
#define ST_DELETE 2
#define ST_DISCARD 3
#define ST_ZORDER 4

#define MAX_DEMO_DYN_COUNT 16

class PMCUI_API CElement : public CObject  
{
	DECLARE_DYNAMIC(CElement)
public:
	virtual long GetLevel();
	virtual void MoveInCell(POINT potTracker);
	virtual void Save(MSXML::IXMLDOMElementPtr pRoot);
	CDnyAttr m_DnyAttr;
	bool m_locked;
	bool m_bHaveFont;
	bool m_bHavePen;
	bool m_bHaveBrush;
	CElement * m_pChild;
	CElement * m_pFather;
	virtual CElement * Clone(long lCmdId,long lStatus);
	long m_lCmdId;
	long m_lStatus;
	virtual long Bottom();
	virtual long Top();
	virtual long Right();
	virtual long Left();
	virtual void Format(long left,long top,long right,long bottom,long flag);
	virtual bool RInR(LPCRECT lpRect);
	virtual long HitTest(long x,long y);
	virtual void DrawHotPoints(CDC *pDC);
	void DrawHotPoint(CDC *pDC,long x,long y);
	CPoint m_potTracker;
	long m_lHitTarget;
	static void UPtoLP(long xu,long yu,long *xl,long *yl);
	static void LPtoUP(long xl,long yl,long *xu,long *yu);
	virtual void Draw(CDC *pDC);
	virtual void DrawTracker(CDC *pDC);
	CString m_sName;
	CElement();
	virtual ~CElement();

protected:
	virtual void F2I(long yo,long *pyt);
	virtual void F2Z(long xo,long *pxt);
	virtual void R2U(long xo,long yo,long * pxt,long * pyt);
	virtual void R2W(long xo,long yo,long * pxt,long * pyt);
};
#endif // !defined(AFX_ELEMENT_H__85A2554F_0154_4B85_9583_349C326E3DA2__INCLUDED_)



