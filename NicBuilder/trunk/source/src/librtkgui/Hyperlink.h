
// Hyperlink.h   -- Definition of CHyperlink class
// Ryan Lederman -- ryan@winprog.org

#ifndef _HYPERLINK_H
#define _HYPERLINK_H

#include "windows.h"

#define S_MOUSEOVER	   0
#define S_MOUSENOTOVER 1

#define HYPERLINK_CLASSNAME "Hyperlink"

/*
typedef struct {
	HWND hWndParent;		// Handle to parent window
	int  ctlID;				// Identifier used for control
	int  coordX;			// X axis coordinate of upper left corner
	int  coordY;			// Y axis coordinate of upper left corner
	int  width;				// Width of control, in pixels
	int  height;			// Height of control, in pixels
	char *szFontName;		// Face name of font used
	int  fontsize;			// Size of font
	bool bBold;				// Bold font?
	bool bUnderline;		// Underline font normally?
	COLORREF clrText;		// Foreground color (text color) of control
	COLORREF clrHilite;		// Highlighted color of text (mouse over, if enabled)
	COLORREF clrBack;		// Background color used (if applicable)
	bool bUseBg;			// Use background color, or just make control transparent?
	bool bUseCursor;		// Use custom curosr?
	HCURSOR hCursor;		// Handle to custom cursor, if used.
	PHYPERLINKFN pFn;		// Function pointer for function that is executed when user clicks
	char *szCaption;		// Caption
} HYPERLINKSTRUCT, *PHYPERLINKSTRUCT;
*/
class CHyperlink {

public:				// Public methods
	CHyperlink();
	~CHyperlink();
	//bool Create( PHYPERLINKSTRUCT phs );	
	BOOL Create(LPRECT prc, HWND hWndParent,LPCTSTR szCaption,LPCTSTR szLink,UINT nID,LPCTSTR lpCursorName);
	void Destroy();
private:			// Private methods
	static LRESULT CALLBACK HyperlinkProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void OnDraw();
	void OnMouseMove();
	void OnMouseLeave();
public:				// Public members
	HWND m_hWnd;

private:			// Private members
	HFONT		 m_hFont;
	HCURSOR		 m_hCursor;

	bool		 m_bMouseOver;
	char*		 m_szLink;
	char*		 m_szCaption;
	int			 m_iLastState;

protected:
	static BOOL RegisterHyperlink();
	static BOOL m_bRegistered;

};

#endif /* _HYPERLINK_H */