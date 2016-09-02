// Text.h: interface for the CText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__5390A298_C63E_454C_B0B1_B37551ECA23E__INCLUDED_)
#define AFX_TEXT_H__5390A298_C63E_454C_B0B1_B37551ECA23E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"
#define PI 3.1415926

class CText : public CElement  
{
	DECLARE_DYNAMIC(CText)
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
	virtual void DrawHotPoints(CDC *pDC);
	virtual long HitTest(long x,long y);
	LOGFONT m_font;
	virtual void DrawTracker(CDC *pDC);
	virtual void Draw(CDC *pDC);
	CString m_text;
	COLORREF m_FaceColor;
	COLORREF m_oFaceColor;
	long m_ly2;
	long m_lx2;
	long m_ly1;
	long m_lx1;
	long m_loy2;
	long m_lox2;
	long m_loy1;
	long m_lox1;
	CText();
	virtual ~CText();

};

#endif // !defined(AFX_TEXT_H__5390A298_C63E_454C_B0B1_B37551ECA23E__INCLUDED_)
