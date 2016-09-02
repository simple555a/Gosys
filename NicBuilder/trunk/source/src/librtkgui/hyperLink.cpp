
// Hyperlink.cpp -- Implementation of CHyperlink class
// Ryan Lederman -- ryan@winprog.org
#include "precomp.h"
#pragma hdrstop
#include "Hyperlink.h"

BOOL CHyperlink::m_bRegistered=FALSE;

extern HINSTANCE g_hInst;
CHyperlink::CHyperlink()
{
	m_hWnd=NULL;
	m_hFont=NULL;
	m_hCursor=NULL;

	m_szCaption=NULL;
	m_szLink=NULL;
}
CHyperlink::~CHyperlink()
{

}
BOOL CHyperlink::RegisterHyperlink()
{
	if(m_bRegistered)
		return TRUE;	
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_VREDRAW ;
	wcex.lpfnWndProc	= ( WNDPROC )HyperlinkProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= HYPERLINK_CLASSNAME;
	wcex.hIconSm		= NULL;
	if(RegisterClassEx(&wcex))
	{
		m_bRegistered=TRUE;
		return TRUE;
	}
	return FALSE;	
}


BOOL CHyperlink::Create(LPRECT prc, HWND hWndParent,LPCTSTR szCaption,LPCTSTR szLink,UINT nID,LPCTSTR lpCursorName)
{
	
	if( !IsWindow( hWndParent ) )
		return false;
	if(!RegisterHyperlink())
		return false;
	m_hWnd = CreateWindow( HYPERLINK_CLASSNAME, "", WS_CHILD | WS_VISIBLE, prc->left, prc->top, prc->right-prc->left, prc->bottom-prc->top,
						   hWndParent,(HMENU)nID, g_hInst, NULL );
	if( m_hWnd == NULL )
		return false;

	if( SetProp( m_hWnd, "CLASSPOINTER", (HANDLE)this ) == 0 )
		return false;	
	
	HFONT hDlgFont;
	LOGFONT lFont;
	if ((hDlgFont = (HFONT)::SendMessage(hWndParent, WM_GETFONT, 0, 0L))!= NULL)
	{
		if (GetObject(hDlgFont, sizeof(LOGFONT), (LPSTR)&lFont)!= NULL)
		{
			lFont.lfUnderline = TRUE;
			m_hFont = CreateFontIndirect(&lFont);
        }        
	}
	if( m_hFont == NULL )
		return false;	

	m_hCursor = LoadCursor(g_hInst,lpCursorName);
	m_szCaption = new char[ strlen(szCaption) + 1 ];
	if( m_szCaption == NULL )
		return false;	
	strcpy( m_szCaption, szCaption);


	m_szLink = new char[ strlen(szLink) + 1 ];
	if( m_szLink == NULL )
		return false;	
	strcpy( m_szLink, szLink);

	m_iLastState = -1;
	return true;
}		
LRESULT CALLBACK CHyperlink::HyperlinkProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	long pThisClass = (long)GetProp( hWnd, "CLASSPOINTER" );
	if(pThisClass)
	{		
		switch( msg )
		{
		case WM_PAINT: 
			((CHyperlink*)pThisClass)->OnDraw(); 
			break;
		case WM_MOUSEMOVE: 
			//((CHyperlink*)pThisClass)->OnMouseMove(); 
			break;
		case WM_MOUSELEAVE:
			//((CHyperlink*)pThisClass)->OnMouseLeave();
			break;
		case WM_LBUTTONUP: 		
		case WM_RBUTTONUP: 
			ShellExecute(NULL, "open", ((CHyperlink*)pThisClass)->m_szLink, "", "", SW_SHOW);			
			break;
		case WM_SETCURSOR:			
			SetCursor( ((CHyperlink*)pThisClass)->m_hCursor );
			return true;
		default:
			break;

		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void CHyperlink::OnDraw( void )
{
	PAINTSTRUCT ps = {0};
	HDC			dc;
	RECT		winRect = {0};

	dc = BeginPaint( m_hWnd, &ps );
	GetClientRect( m_hWnd, &winRect );

	SetBkMode( dc, TRANSPARENT );	
	
	SelectObject( dc, m_hFont );

	if( m_bMouseOver == S_MOUSENOTOVER )
	{
		SetTextColor( dc, RGB(0xFF,0,0) );
	}
	else
	{
		SetTextColor( dc,  RGB(0,0,0xFF));
	}

	DrawTextEx( dc, m_szCaption, strlen( m_szCaption ), &winRect, DT_VCENTER | DT_LEFT, NULL );
	EndPaint( m_hWnd, &ps );
}

void CHyperlink::OnMouseLeave()
{
	m_bMouseOver = false;
	
	if( m_iLastState != S_MOUSENOTOVER )
	{
		InvalidateRect( m_hWnd, NULL, TRUE );
		UpdateWindow( m_hWnd );
	}
	m_iLastState = S_MOUSENOTOVER;
}
void CHyperlink::OnMouseMove()
{
	m_bMouseOver = true;
	
	if( m_iLastState != S_MOUSEOVER )
	{
		InvalidateRect( m_hWnd, NULL, TRUE );
		UpdateWindow( m_hWnd );
	}
	m_iLastState = S_MOUSEOVER;	
}

void CHyperlink::Destroy( void )
{
	if(m_hWnd)
	{
		DestroyWindow( m_hWnd );
		m_hWnd=NULL;
	}	

	if(m_hFont)
	{
		DeleteObject( m_hFont );
		m_hFont=NULL;
	}

	if(m_hCursor)
	{
		DeleteObject( m_hCursor );
		m_hCursor=NULL;
	}

	if( m_szCaption )
	{
		delete m_szCaption;
		m_szCaption=NULL;
	}
	
	if( m_szLink )
	{
		delete m_szLink;
		m_szLink=NULL;
	}
}
	