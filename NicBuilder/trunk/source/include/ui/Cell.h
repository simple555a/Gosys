// Cell.h: interface for the CCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELL_H__E717C0C7_E93D_4DB0_8FA4_048F9C6CBC73__INCLUDED_)
#define AFX_CELL_H__E717C0C7_E93D_4DB0_8FA4_048F9C6CBC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class PMCUI_API CCell : public CElement  
{
	DECLARE_DYNAMIC(CCell)
public:
	int GetDynCount();
	virtual long GetLevel();
	long SetTag(CListBox *pTagList,long lCount=0);
	long GetLableTag(CListBox *pLabList,long lFatherCount=0);
	virtual void MoveInCell(POINT potTracker);
	virtual long Bottom();
	virtual long Top();
	virtual void Save(MSXML::IXMLDOMElementPtr pRoot);
	virtual bool RInR(LPCRECT lpRect);
	virtual long Right();
	void Load(MSXML::IXMLDOMElementPtr pGO);
	virtual long Left();
	virtual long HitTest(long x, long y);
	virtual void Format(long left, long top, long right, long bottom, long flag);
	virtual void DrawTracker(CDC *pDC);
	virtual void DrawHotPoints(CDC *pDC);
	virtual void Draw(CDC *pDC);
	virtual CElement * Clone(long lCmdId, long lStatus);
	CObList m_Elements;
	//long m_ly2;
	//long m_ly1;
	//long m_lx2;
	//long m_lx1;
	CCell();
	virtual ~CCell();

};

#endif // !defined(AFX_CELL_H__E717C0C7_E93D_4DB0_8FA4_048F9C6CBC73__INCLUDED_)
