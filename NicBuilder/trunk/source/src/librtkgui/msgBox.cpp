/********************************************************************
	created:	2002.07.03   
	filename: 	RtkGuiMsgBox.cpp
	file path:	d:\dfvs2.0\src\vc\RtkGui\RtkGuimsgbox.cpp
	file base:	RtkGuimsgbox
	file ext:	.cpp
	author:		Steven
	
	purpose:	ר����Ϣ������һ����ѡ��ť
				��...�Լ���������δ����й�����...ms�ĺܶණ�����ǹ�˼���
				���һ�������ṹ��������Ʒȷʵ��Ҫ����ʱ��ú����һ�� 
*********************************************************************/
#include "precomp.h"
#pragma hdrstop
#include "string"
#include "resource.h"

using namespace std;

#define BTN_OK_TEXT "ȷ��"
#define BTN_CANCEL_TEXT "ȡ��"
#define BTN_YES_TEXT "��"
#define BTN_NO_TEXT "��"

#define MIN_DLG_WIDTH  180  
#define MAX_TEXT_WIDTH 200 // �ַ���

#define MIN_DLG_HEIGHT 130


extern HINSTANCE g_hInst; // RtkGui.cpp
static CHAR      g_szText[MAX_PATH];  // ��Ϣ����Ϣ����
static CHAR      g_szCaption[MAX_PATH]; // ��Ϣ�����
static CHAR		 g_szAlways[MAX_PATH]; // ��ѡ������
static INT		 g_iBtnID1, g_iBtnID2;
static CHAR      g_szBtnText1[MAX_PATH], g_szBtnText2[MAX_PATH];
static HFONT	 g_hFont;
/******************************************************************

** ������: _SetupMsgBox

** �� ��: HWND hMsgDlg, UINT uType

		** hMsgDlg--- �Ի�����

		** uType--- �Ի���ʹ�õ�����

** ����ֵ������ɹ�����TURE,��֮����FALSE

** ������������ʼ����Ϣ��ؼ�λ�úʹ�С

** �� �ߣ�steven

** �� �ڣ�2002.7.03

** �汾��1.0

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
				VARIABLE_PITCH |FF_SWISS, "����");
	
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
	
	
	SetWindowText(hMsgDlg, g_szCaption); // ���ñ��� 
	
	Button_SetCheck(hWndAlways, TRUE); // �Զ�ѡ�к�ѡ��ť
	Button_SetText(hWndAlways, g_szAlways);
    // �����Զ����еĹ���
   

    Static_SetText(hWndText, g_szText); // ������Ϣ����

    hDCText = GetDC(hWndText); 
    SelectObject(hDCText, g_hFont);
     
    RectText.right = MAX_TEXT_WIDTH;    
  
       
    DrawText( hDCText, g_szText, strlen(g_szText), 
			  &RectText, DT_CALCRECT | DT_WORDBREAK | DT_LEFT);
    
    ReleaseDC(hWndText, hDCText);
	switch(uType & MB_ICONMASK) // ����ͼ�� 
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

	/* �������ֿ��С���Ƿ���ʾͼ����������Ϣ�����յĿ�Ⱥ͸߶� */
	uMsgDlgWidth = RectText.right - RectText.left + uLeftPos + 24;
	if (uMsgDlgWidth < MIN_DLG_WIDTH) 
		uMsgDlgWidth = MIN_DLG_WIDTH;
	
	uMsgDlgHeight = RectText.bottom - RectText.top + 110;
	if (uMsgDlgHeight < MIN_DLG_HEIGHT)
		uMsgDlgHeight = MIN_DLG_HEIGHT;
	
	/* ������Ϣ���С */
	SetWindowPos( hMsgDlg, NULL, 0, 0, 
				  uMsgDlgWidth, uMsgDlgHeight, 
				  SWP_NOMOVE ); 

	/* ������Ϣ�ı�λ�úʹ�С */
	MoveWindow( hWndText,  uLeftPos, 10, 
			    RectText.right - RectText.left, 
			    RectText.bottom - RectText.top, TRUE ); 
		
	/* ���ĺ�ѡ��ťλ�� */
	SetWindowPos( hWndAlways, NULL, uLeftPos, 
				  uMsgDlgHeight - 90, 0, 0, 
				  SWP_NOSIZE ); 
	
	/* ���ĵ�һ����ťλ�� */
	SetWindowPos( hWndBtn1, NULL, uMsgDlgWidth / 2 - 76, 
				  uMsgDlgHeight - 62, 0, 0, SWP_NOSIZE ); 

	/* ���ĵڶ�����ťλ�� */
	SetWindowPos( hWndBtn2, NULL, uMsgDlgWidth / 2 + 4, 
				  uMsgDlgHeight - 62, 0, 0, SWP_NOSIZE ); 

    return TRUE;
}

/******************************************************************

** ������: _RtkGuiMsgBoxProc

** �� ��: HWND hMsgDlg, UINT uMsg, WPARAM wParam, LPARAM lParam

		** hMsgDlg--- �Ի�����

		** uMsg--- ��Ϣ����
		
		** wParam--- ����

		** lParam--- ����

** ����ֵ��BOOL

** ������������Ϣ��ص�����

** �� �ߣ�steven

** �� �ڣ�2002.7.03

** �汾��1.0

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

** ������: RtkGuiMsgBox

** �� ��: HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType

		** hWndOwner--- ��Ϣ���������ڣ����ΪNULL����Ϣ��Ϊ��ģ̬����

		** lpText--- ��Ϣ�ı�
		
		** lpCaption--- ��Ϣ�Ի������
		
		** uType--- ��Ϣ������

** ����ֵ����ȷ����1

** ����������Ssi��Ϣ�Ի��������˺�ѡ����

** �� �ߣ�steven

** �� �ڣ�2002.7.03

** �汾��1.0

*****************************************************************/
RTKGUIAPI int WINAPI RtkGuiMsgBox(HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType)
{
	
    return RtkGuiMsgBoxEx(hWndOwner, lpText, lpCaption, "������˴���", uType);
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

