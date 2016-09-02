/********************************************************************
	created:	2002.07.03   
	filename: 	RtkGuiMsgBox.cpp
	file path:	d:\dfvs2.0\src\vc\RtkGui\RtkGuimsgbox.cpp
	file base:	RtkGuimsgbox
	file ext:	.cpp
	author:		Steven
	
	purpose:	专用消息框，增加一个核选框按钮
				哎...自己都觉得这段代码有够垃圾...ms的很多东西真是构思巧妙，
				想出一个完美结构合理的替代品确实需要花点时间好好设计一下 
*********************************************************************/
#include "precomp.h"
#pragma hdrstop
#include "string"
#include "resource.h"

using namespace std;

#define BTN_OK_TEXT "确定"
#define BTN_CANCEL_TEXT "取消"
#define BTN_YES_TEXT "是"
#define BTN_NO_TEXT "否"

#define MIN_DLG_WIDTH  180  
#define MAX_TEXT_WIDTH 200 // 字符数

#define MIN_DLG_HEIGHT 130


extern HINSTANCE g_hInst; // RtkGui.cpp
static CHAR      g_szText[MAX_PATH];  // 消息框消息文字
static CHAR      g_szCaption[MAX_PATH]; // 消息框标题
static CHAR		 g_szAlways[MAX_PATH]; // 核选框文字
static INT		 g_iBtnID1, g_iBtnID2;
static CHAR      g_szBtnText1[MAX_PATH], g_szBtnText2[MAX_PATH];
static HFONT	 g_hFont;
/******************************************************************

** 函数名: _SetupMsgBox

** 输 入: HWND hMsgDlg, UINT uType

		** hMsgDlg--- 对话框句柄

		** uType--- 对话框使用的类型

** 返回值：如果成功返回TURE,反之返回FALSE

** 功能描述：初始化消息框控件位置和大小

** 作 者：steven

** 日 期：2002.7.03

** 版本：1.0

*****************************************************************/
inline BOOL _SetupMsgBox(HWND hMsgDlg, UINT uType)
{
	HWND   hWndIcon, hWndText, hWndAlways, hWndBtn1, hWndBtn2;
    RECT   RectText = {0};
	HDC    hDCText;
	UINT   uMsgDlgWidth, uMsgDlgHeight, uLeftPos = 50;
    DWORD  dwBtn1Style, dwBtn2Style;



	g_hFont = CreateFont(MulDiv(9, - GetDeviceCaps( GetDC(hMsgDlg), LOGPIXELSY) , 72),
				0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_STROKE_PRECIS,
				CLIP_STROKE_PRECIS , DRAFT_QUALITY,
				VARIABLE_PITCH |FF_SWISS, "宋体");
	
	dwBtn1Style = WS_CHILDWINDOW|WS_VISIBLE|BS_TEXT|BS_PUSHBUTTON|BS_DEFPUSHBUTTON;
	dwBtn2Style = WS_CHILDWINDOW|WS_VISIBLE|BS_TEXT|BS_PUSHBUTTON;

    /*
	if (uType & MB_DEFBUTTON2)
	{
		dwBtn2Style |= BS_DEFPUSHBUTTON;
	}
	else
	{
		dwBtn1Style |= BS_DEFPUSHBUTTON;
	}
    */
	hWndBtn2 = CreateWindow("BUTTON", g_szBtnText2, dwBtn2Style, 0, 0, 72, 21, hMsgDlg, (HMENU)g_iBtnID2, g_hInst, 0 );
    hWndBtn1 = CreateWindow("BUTTON", g_szBtnText1, dwBtn1Style, 0, 0, 72, 21, hMsgDlg, (HMENU)g_iBtnID1, g_hInst, 0 );
	

	SendMessage(hWndBtn1, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE,0));
	SendMessage(hWndBtn2, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE,0));
	
	hWndIcon    = GetDlgItem(hMsgDlg, IDC_ICO);
	hWndText    = GetDlgItem(hMsgDlg, IDC_TEXT);
	hWndAlways  = GetDlgItem(hMsgDlg,  IDC_ALWAYS);
	
	
	SetWindowText(hMsgDlg, g_szCaption); // 设置标题 
	
	Button_SetCheck(hWndAlways, TRUE); // 自动选中核选按钮
	Button_SetText(hWndAlways, g_szAlways);
    // 加上自动换行的功能
   

    Static_SetText(hWndText, g_szText); // 设置消息文字

    hDCText = GetDC(hWndText); 
    SelectObject(hDCText, g_hFont);
     
    RectText.right = MAX_TEXT_WIDTH;    
  
       
    DrawText( hDCText, g_szText, strlen(g_szText), 
			  &RectText, DT_CALCRECT | DT_WORDBREAK | DT_LEFT);
    
    ReleaseDC(hWndText, hDCText);
	switch(uType & MB_ICONMASK) // 设置图标 
    {
        case MB_ICONERROR:
            Static_SetIcon(hWndIcon, LoadIcon(NULL, IDI_ERROR));
	        break;
        case MB_ICONWARNING:
            Static_SetIcon(hWndIcon, LoadIcon(NULL, IDI_WARNING));
            break;
        case MB_ICONINFORMATION:
            Static_SetIcon(hWndIcon, LoadIcon(NULL, IDI_INFORMATION));
            break;
        case MB_ICONQUESTION:
            Static_SetIcon(hWndIcon, LoadIcon(NULL, IDI_QUESTION));
			break;
		default:
			uLeftPos = 12;
			break;
    }

	/* 根据文字框大小和是否显示图标来计算消息框最终的宽度和高度 */
	uMsgDlgWidth = RectText.right - RectText.left + uLeftPos + 24;
	if (uMsgDlgWidth < MIN_DLG_WIDTH) 
		uMsgDlgWidth = MIN_DLG_WIDTH;
	
	uMsgDlgHeight = RectText.bottom - RectText.top + 110;
	if (uMsgDlgHeight < MIN_DLG_HEIGHT)
		uMsgDlgHeight = MIN_DLG_HEIGHT;
	
	/* 更改消息框大小 */
	SetWindowPos( hMsgDlg, NULL, 0, 0, 
				  uMsgDlgWidth, uMsgDlgHeight, 
				  SWP_NOMOVE ); 

	/* 更改消息文本位置和大小 */
	MoveWindow( hWndText,  uLeftPos, 10, 
			    RectText.right - RectText.left, 
			    RectText.bottom - RectText.top, TRUE ); 
		
	/* 更改核选按钮位置 */
	SetWindowPos( hWndAlways, NULL, uLeftPos, 
				  uMsgDlgHeight - 90, 0, 0, 
				  SWP_NOSIZE ); 
	
	/* 更改第一个按钮位置 */
	SetWindowPos( hWndBtn1, NULL, uMsgDlgWidth / 2 - 76, 
				  uMsgDlgHeight - 62, 0, 0, SWP_NOSIZE ); 

	/* 更改第二个按钮位置 */
	SetWindowPos( hWndBtn2, NULL, uMsgDlgWidth / 2 + 4, 
				  uMsgDlgHeight - 62, 0, 0, SWP_NOSIZE ); 

    return TRUE;
}

/******************************************************************

** 函数名: _RtkGuiMsgBoxProc

** 输 入: HWND hMsgDlg, UINT uMsg, WPARAM wParam, LPARAM lParam

		** hMsgDlg--- 对话框句柄

		** uMsg--- 消息参数
		
		** wParam--- 参数

		** lParam--- 参数

** 返回值：BOOL

** 功能描述：消息框回调函数

** 作 者：steven

** 日 期：2002.7.03

** 版本：1.0

*****************************************************************/
BOOL WINAPI _RtkGuiMsgBoxProc(HWND hMsgDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush;

    switch(uMsg)
    {
        case WM_INITDIALOG:
			_SetupMsgBox(hMsgDlg, lParam);
            return TRUE;            
            break;

        case WM_COMMAND:
            {
				WORD wID = LOWORD(wParam);
                switch(wID)
                {
                    case IDOK:                    
                    case IDCANCEL:  
					case IDYES:
					case IDNO:
                    {
                        if (Button_GetCheck(GetDlgItem(hMsgDlg, IDC_ALWAYS))) {
                            wID = wID + IDALWAYSOFFSET;                            
                        }
                        return EndDialog(hMsgDlg, wID);
                        
                    }
                    break;                
                }
            }
            break;
        case WM_DESTROY:
			if (g_hFont)
			{
				DeleteObject(g_hFont);
			}
            break;
        default:
            break;
    }
    
    return FALSE;
}


/******************************************************************

** 函数名: RtkGuiMsgBox

** 输 入: HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType

		** hWndOwner--- 消息框物主窗口，如果为NULL，消息框为非模态窗口

		** lpText--- 消息文本
		
		** lpCaption--- 消息对话框标题
		
		** uType--- 消息框类型

** 返回值：正确返回1

** 功能描述：Ssi消息对话框，增加了核选功能

** 作 者：steven

** 日 期：2002.7.03

** 版本：1.0

*****************************************************************/
RTKGUIAPI int WINAPI RtkGuiMsgBox(HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType)
{
	
    return RtkGuiMsgBoxEx(hWndOwner, lpText, lpCaption, "总是如此处理", uType);
}

RTKGUIAPI int WINAPI RtkGuiMsgBoxEx(HWND hWndOwner, LPCSTR lpText, LPCSTR lpCaption, LPCSTR lpAlways, UINT uType)
{
	switch(uType & MB_TYPEMASK)
	{
		
		case MB_YESNOALWAYS:
			{
				g_iBtnID1 = IDYES;
				g_iBtnID2 = IDNO;
				
				strcpy(g_szBtnText1, BTN_YES_TEXT);
				strcpy(g_szBtnText2, BTN_NO_TEXT);
				strcpy(g_szText, lpText);
				strcpy(g_szCaption, lpCaption);
				strcpy(g_szAlways, lpAlways);

				return DialogBoxParam(g_hInst, (LPSTR)IDD_MSGBOX, hWndOwner, _RtkGuiMsgBoxProc, (LPARAM)uType);    
			}
			break;
		
		case MB_OKCANCELALWAYS:
			{
				g_iBtnID1 = IDOK;
				g_iBtnID2 = IDCANCEL;

				strcpy(g_szBtnText1, BTN_OK_TEXT);
				strcpy(g_szBtnText2, BTN_CANCEL_TEXT);
   				strcpy(g_szText, lpText);
				strcpy(g_szAlways, lpAlways);
				strcpy(g_szCaption, lpCaption);

				return DialogBoxParam(g_hInst, (LPSTR)IDD_MSGBOX, hWndOwner, _RtkGuiMsgBoxProc, (LPARAM)uType);    
			}
			break;
		
		default:
			return MessageBox(hWndOwner, lpText, lpCaption, uType);
			break;
	}	
}

