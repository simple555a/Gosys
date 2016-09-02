// Arc.h: interface for the CArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARC_H__41A3D2E1_A497_48DD_9A0B_CDB0218138F8__INCLUDED_)
#define AFX_ARC_H__41A3D2E1_A497_48DD_9A0B_CDB0218138F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CArc : public CElement  
{
	DECLARE_DYNAMIC(CArc)
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
	LOGPEN m_open;
	long m_ly4;
	long m_ly3;
	long m_ly2;
	long m_ly1;
	long m_lx4;
	long m_lx3;
	long m_lx2;
	long m_lx1;
	long m_loy4;
	long m_loy3;
	long m_loy2;
	long m_loy1;
	long m_lox4;
	long m_lox3;
	long m_lox2;
	long m_lox1;
	CArc();
	virtual ~CArc();
private:
	bool SPE(double sa, double pa, double ea);
	double GetAngle(double dx, double dy);
};

#endif // !defined(AFX_ARC_H__41A3D2E1_A497_48DD_9A0B_CDB0218138F8__INCLUDED_)
