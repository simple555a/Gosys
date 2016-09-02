// Cursor.h: interface for the CCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURSOR_H__3779B8D3_8276_4D6B_AB68_A98D0812AD60__INCLUDED_)
#define AFX_CURSOR_H__3779B8D3_8276_4D6B_AB68_A98D0812AD60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Element.h"

class PMCUI_API CCursor : public CElement
{
public:
	long m_lHeight;
	BOOL m_bActive;
	void Hide(CDC *pDC);
	void Show(CDC *pDC);
	long m_ly;
	long m_lx;
	CCursor();
	virtual ~CCursor();
	BOOL m_bShow;
};

#endif // !defined(AFX_CURSOR_H__3779B8D3_8276_4D6B_AB68_A98D0812AD60__INCLUDED_)
