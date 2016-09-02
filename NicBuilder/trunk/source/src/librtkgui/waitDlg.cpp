/********************************************************************
	created:	2002.07.02   
	filename: 	RtkGuiAboutBox.cpp
	file path:	d:\dfvs2.0\src\vc\RtkGui\RtkGuiwaitdlg.cpp
	file base:	RtkGuiwaitdlg
	file ext:	.cpp
	author:		Steven
	
	purpose:	�ȴ�����Ի������ܾ���������һ�����ָ����ʣ�
                processdlg???
                
*********************************************************************/
#include "precomp.h"
#pragma hdrstop
#include "waitdlg.h"
#include "resource.h"

#define  TIMEOUT    5000
#define  DFVS_BACK_COLOR 0xefefef

#ifndef NDEBUG
#define _trace OutputDebugString
#else
#define _trace(a) 
#endif

extern HINSTANCE g_hInst;

static BOOL g_bUseSkin = FALSE;

void _ResizeStatic(HWND hStatic, LPSTR szText)
{
	HDC  hDCStatic;
	RECT RectStatic;
	
	hDCStatic = GetDC(hStatic);
	DrawText(hDCStatic, szText, strlen(szText), &RectStatic, DT_CALCRECT);
	SetWindowPos(hStatic, 
				 NULL,
				 0,
				 0,
				 RectStatic.right - RectStatic.left,
				 RectStatic.bottom - RectStatic.top,
				 SWP_NOMOVE);
	ReleaseDC(hStatic, hDCStatic);
}

BOOL WINAPI _RtkGuiWaitDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static WAITDLG_CONTEXT *ctx;
//	static HBRUSH hBrush = NULL;
	
    switch(uMsg)
    {
		case WM_SHOWWINDOW:
		{
			HWND hState, hTimeLeft, hTimeRemain, 
				 hProgress, hCancel, hPause, hAnimate;
			
			hPause      = GetDlgItem(hDlg, IDPAUSE);
			hCancel     = GetDlgItem(hDlg, IDCANCEL);
			hState      = GetDlgItem(hDlg, IDC_STATE);
			hAnimate    = GetDlgItem(hDlg, IDC_ANIMATE);
			hProgress   = GetDlgItem(hDlg, IDC_PROGRESS);
			hTimeLeft   = GetDlgItem(hDlg, IDC_TIMELEFT);
			hTimeRemain = GetDlgItem(hDlg, IDC_TIMEREMAIN);

			ctx = (LPWAITDLG_CONTEXT)GetWindowLong(hDlg, GWL_USERDATA);
			ctx->dwLastTime = GetTickCount();
		
			if (ctx->uType & WDS_STATE) {
				ShowWindow(hState, SW_SHOW);
			}

			if (ctx->uType & WDS_ANIMATE) {
				ShowWindow(hAnimate, SW_SHOW);
				Animate_Open(hAnimate, MAKEINTRESOURCE(IDR_WAIT));
			}

			 if (ctx->uType & WDS_CANCEL) {
				ShowWindow(hCancel, SW_SHOW);
			}
			
			if (ctx->uType & WDS_PAUSE) {
				ShowWindow(hPause, SW_SHOW);
			}

			if (ctx->uType & WDS_TIMELEFT) {
				ShowWindow(hTimeLeft, SW_SHOW);
			}

			if (ctx->uType & WDS_TIMEREMAIN) {
				ShowWindow(hTimeRemain, SW_SHOW);
			}
			
			if (ctx->uType & WDS_PROGRESS) {
				ShowWindow(hProgress, SW_SHOW);
			}
			/* 
				���취,Ϊÿ����ϵ���һ��λ��,��û���뵽���õķ�����ʱ��,
				ֻ�������²�
				Ŀǰ֧�ּ��������
				1:��cancel��ť�ͽ�������״̬�ı�
				2:��cancel��ť�ͽ�������״̬�ı���ʱ��(ʣ��ʱ����ѹ�ȥʱ��)
				3:��cancel��ť�ͽ������Ͷ�����״̬�ı�
				4:��cancel��ť�ͽ������Ͷ�����״̬�ı���ʱ��
				
				5:��cancel,pause��ť�ͽ�������״̬�ı�
				6:��cancel,pause��ť�ͽ������Ͷ�����״̬�ı�
				7:������ʾ����ʾtimeleft��timeremain�Ͳ���Ҫ����λ����
			*/
			switch (ctx->uType)
			{
				case WDS_CANCEL|WDS_PROGRESS|WDS_STATE:
						SetWindowPos(hState, NULL, 6, 10, 0, 0, SWP_NOSIZE);
						SetWindowPos(hProgress, NULL, 6, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hCancel, NULL, 270, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hDlg, NULL, 0, 0, 360, 96, SWP_NOMOVE);
					break;
				
				case WDS_CANCEL|WDS_PROGRESS|WDS_STATE|WDS_TIMELEFT:
						SetWindowPos(hState, NULL, 6, 10, 0, 0, SWP_NOSIZE);
						SetWindowPos(hProgress, NULL, 6, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hCancel, NULL, 270, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hTimeLeft, NULL, 6, 60, 0, 0, SWP_NOSIZE);
						SetWindowPos(hDlg, NULL, 0, 0, 360, 110, SWP_NOMOVE);
					break;
				
				case WDS_CANCEL|WDS_PROGRESS|WDS_STATE|WDS_TIMEREMAIN:
						SetWindowPos(hState, NULL, 6, 10, 0, 0, SWP_NOSIZE);
						SetWindowPos(hProgress, NULL, 6, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hCancel, NULL, 270, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hTimeRemain, NULL, 6, 60, 0, 0, SWP_NOSIZE);
						SetWindowPos(hDlg, NULL, 0, 0, 360, 110, SWP_NOMOVE);
					break;

				case WDS_CANCEL|WDS_PROGRESS|WDS_STATE|WDS_TIMEREMAIN|WDS_TIMELEFT:
						SetWindowPos(hState, NULL, 6, 10, 0, 0, SWP_NOSIZE);
						SetWindowPos(hProgress, NULL, 6, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hCancel, NULL, 270, 30, 0, 0, SWP_NOSIZE);
						SetWindowPos(hTimeLeft, NULL, 6, 60, 0, 0, SWP_NOSIZE);
						SetWindowPos(hTimeRemain, NULL, 215, 60, 0, 0, SWP_NOSIZE);
						SetWindowPos(hDlg, NULL, 0, 0, 360, 110, SWP_NOMOVE);
					break;
			
				case WDS_CANCEL|WDS_PROGRESS|WDS_ANIMATE|WDS_STATE:
						SetWindowPos(hAnimate, NULL, 6, 0, 0, 0, SWP_NOSIZE);
						SetWindowPos(hState, NULL, 10, 70, 0, 0, SWP_NOSIZE);
						SetWindowPos(hProgress, NULL, 6, 90, 0, 0, SWP_NOSIZE);
						SetWindowPos(hCancel, NULL, 270, 90, 0, 0, SWP_NOSIZE);
						SetWindowPos(hDlg, NULL, 0, 0, 360, 156, SWP_NOMOVE);
					break;
			
				case WDS_CANCEL|WDS_PAUSE|WDS_PROGRESS:
					break;

				case WDS_CANCEL|WDS_PAUSE|WDS_PROGRESS|WDS_ANIMATE:
					break;

				default:
					break;
			}

		}	
		return TRUE;
        break;

	case WM_TIMER:
		{
			char szTimeRemain[MAX_PATH], szTimeLeft[MAX_PATH];
		
			if (!ctx->nPos)
			{
				return TRUE;
			}
			if (!ctx->bPause)
			{
				ctx->dwTimeLeft  += GetTickCount() - ctx->dwLastTime;
				ctx->dwLastTime   = GetTickCount();
				ctx->dwTimeRemain = ctx->dwTimeLeft / ctx->nPos * (100 - ctx->nPos);
	
				sprintf(szTimeRemain, 
						"%02d:%02d:%02d", 
						(ctx->dwTimeRemain / 1000) / 3600,
						(ctx->dwTimeRemain / 1000) % 3600 / 60,
						(ctx->dwTimeRemain / 1000) % 60);
				sprintf(szTimeLeft,   
						"%02d:%02d:%02d", 
						(ctx->dwTimeLeft / 1000) / 3600,
						(ctx->dwTimeLeft / 1000) % 3600 / 60,
						(ctx->dwTimeLeft / 1000) % 60);
				
				Static_SetText(GetDlgItem(ctx->hWaitDlg, IDC_TIMEREMAIN), szTimeRemain); 
				Static_SetText(GetDlgItem(ctx->hWaitDlg, IDC_TIMELEFT), szTimeLeft); 
				_ResizeStatic(GetDlgItem(ctx->hWaitDlg, IDC_TIMEREMAIN), szTimeRemain);
				_ResizeStatic(GetDlgItem(ctx->hWaitDlg, IDC_TIMELEFT), szTimeLeft); 
	
			} 
			else
			{
				ctx->dwLastTime = GetTickCount();
			}

		}
		break;


    case WM_COMMAND:
        {
			if (ctx->fnWaitDlg) 
			{
				int nResult;
				switch(LOWORD(wParam)) 
				{
					case IDCANCEL:
						nResult = ctx->fnWaitDlg(ctx, WDN_BTNCLICK, WDC_CANCEL);
						break;
					case IDPAUSE:
						ctx->bPause = !ctx->bPause;
						if (ctx->bPause)
						{
							Animate_Stop(GetDlgItem(hDlg, IDC_ANIMATE));
							RtkGuiWDSetBtnText(ctx, IDPAUSE, "����");
							nResult = ctx->fnWaitDlg(ctx, WDN_BTNCLICK, WDC_PAUSE);
						}
						else
						{
							Animate_Play(GetDlgItem(hDlg, IDC_ANIMATE), 0, -1, -1);
							RtkGuiWDSetBtnText(ctx, IDPAUSE, "��ͣ");
							nResult = ctx->fnWaitDlg(ctx, WDN_BTNCLICK, WDC_RESUME);
						}
						break;
				}
				// ���û��ص������������-1,��ɾ���˶Ի���
				if (nResult == -1) {
					SendMessage(ctx->hWaitDlg, WM_DESTROY, 0, 0);
				}
                
            }
			else 
			{
				switch(wParam)
				{
					case IDCANCEL:
						ctx->uLastPushBtn |= WDC_CANCEL;
						break;
					case IDPAUSE:
						ctx->bPause = !ctx->bPause;
						
						if (ctx->bPause)
						{
							Animate_Stop(GetDlgItem(hDlg, IDC_ANIMATE));
							RtkGuiWDSetBtnText(ctx, IDPAUSE, "����");
							ctx->uLastPushBtn |= WDC_PAUSE;
						}
						else
						{
							Animate_Play(GetDlgItem(hDlg, IDC_ANIMATE), 0, -1, -1);
							RtkGuiWDSetBtnText(ctx, IDPAUSE, "��ͣ");
							ctx->uLastPushBtn |= WDC_RESUME;
						}
				
						break;
				}
            }
		}
        break;
		/*
	case WM_CTLCOLORDLG:
		{
			if (hBrush) 
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}
			hBrush = CreateSolidBrush(DFVS_BACK_COLOR);
			SetBkColor((HDC)wParam, DFVS_BACK_COLOR);
			return (BOOL)hBrush;
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if (hBrush) 
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}
			hBrush = CreateSolidBrush(DFVS_BACK_COLOR);
			SetBkColor((HDC)wParam, DFVS_BACK_COLOR);
			return (BOOL)hBrush;
		}
		break;*/
    case WM_CLOSE:
        {
			DestroyWindow(hDlg);
			PostQuitMessage(0);
			return FALSE;
        }
        break;

    }
    return FALSE;
}

UINT WINAPI _WaitDlgThread(LPVOID a_ctx)
{
    MSG     Msg;
	// HMODULE hSkinLib = NULL;
	void    (WINAPI * lpfnSubClassWindow)(HWND) = NULL;
	void    (WINAPI * lpfnSubClassButton)(HWND) = NULL;
	WAITDLG_CONTEXT *ctx = (LPWAITDLG_CONTEXT)a_ctx;

	
    /* ��ʼ�������ؼ��� */
    InitCommonControls();
    
    /* ������ģʽ�Ի��� */
    ctx->hWaitDlg = CreateDialog(g_hInst, (LPCTSTR)IDD_WAITDLG, 
							   NULL, _RtkGuiWaitDlgProc );
    
    if (ctx->hWaitDlg == NULL) 
	{
		// �¼���ֹ,����RtkGuiWDStart��������
		if (ctx->hEvent) {
			SetEvent(ctx->hEvent);
		}
        return FALSE;
    }
    
	// ����skinengine��ǰֻ�������߳�ִ�У�����
	/*
	if (g_bUseSkin)
	{
		hSkinLib = LoadLibrary("dfvsskin.dll");
	
		(FARPROC&)lpfnSubClassWindow = GetProcAddress(hSkinLib, "2");
		//(FARPROC&)lpfnSubClassButton = GetProcAddress(hSkinLib, "8");
		
		if (lpfnSubClassWindow)
		{
			lpfnSubClassWindow(ctx->hWaitDlg);
		}
		if (lpfnSubClassButton)
		{
			lpfnSubClassButton(GetDlgItem(ctx->hWaitDlg, IDCANCEL));
		}
		
	}
	*/
	SetWindowLong(ctx->hWaitDlg, GWL_USERDATA, (LONG)ctx); 

	_trace("Shell thread init complete.\n");

	// ��ʼ����ϣ�֪ͨ���߳̿���ʹ�öԻ����ˣ��¼���ֹ,����RtkGuiWDStart��������
	if (ctx->hEvent) 
	{
		SetEvent(ctx->hEvent);
	}	
     
    ShowWindow(ctx->hWaitDlg, SW_SHOWNORMAL);
	UpdateWindow(ctx->hWaitDlg);
    
	if (   (ctx->uType & WDS_TIMELEFT) 
		|| (ctx->uType & WDS_TIMEREMAIN) )
	{
		SetTimer(ctx->hWaitDlg, 1, 500, NULL);
	}
	
    _trace("Shell thread enter message loop.\n");

    while (GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

	// free resource
	/*
	if (hSkinLib)
	{
		if (lpfnSubClassWindow)
		{
			lpfnSubClassWindow = NULL;
		}
		if (lpfnSubClassButton)
		{
	 		lpfnSubClassButton = NULL;
		}
		FreeLibrary(hSkinLib);
	}
	*/
    
	// 2002/8/16, Jackie, ˭���ʹ�ã�˭����ͷ�
	free(ctx);
	ctx = NULL;

	return TRUE;
}


/* �����ȴ��Ի��� */
RTKGUIAPI HANDLE WINAPI RtkGuiWDStart(UINT uType, LPFNWAITDLGPROC fnWaitDlg)
{
	WAITDLG_CONTEXT * ctx;
	UINT uThreadID;
	DWORD dwWait;
	
	ctx = (LPWAITDLG_CONTEXT)malloc(sizeof(WAITDLG_CONTEXT));
	if (!ctx)
	{
		return NULL;
	}
	
	ZeroMemory(ctx, sizeof(WAITDLG_CONTEXT));
    
	ctx->fnWaitDlg = fnWaitDlg;
	ctx->uType     = uType;
	
	ctx->hEvent    = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!ctx->hEvent)
	{
		free(ctx);
		return NULL;
	}
	
	ctx->hThread   = (HANDLE)_beginthreadex(NULL, 0, _WaitDlgThread, (LPVOID)ctx, 0, &uThreadID);
	if (!ctx->hThread)
	{
		CloseHandle(ctx->hEvent);
		free(ctx);
		return NULL;
	}	
	
	dwWait = WaitForSingleObject(ctx->hEvent, TIMEOUT);

	HANDLE hTemp;
	if( ((DWORD)(&ctx->hEvent) & 0x3) )
	{
		// not on dword boundary
		hTemp = ctx->hEvent;
		ctx->hEvent = NULL;
	}
	else
	{
		hTemp = NULL;
		InterlockedExchange((LONG*)&ctx->hEvent, (LONG)hTemp);
	}
	CloseHandle(hTemp);

	if(dwWait != WAIT_OBJECT_0)
	{
		_trace("Shell timeout.\n");
		PostThreadMessage(uThreadID, WM_QUIT, 0, 0);		
		return NULL;
	}

	return ctx;
}

/* ���õȴ��Ի����ڽ�������λ�� */
RTKGUIAPI BOOL WINAPI RtkGuiWDSetPos(HANDLE hWDContext, UINT nPos)
{
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;
	ctx->nPos = nPos;
    return SendMessage(GetDlgItem(ctx->hWaitDlg, IDC_PROGRESS), PBM_SETPOS, nPos, 0); 
}

/* ���õȴ��Ի���״̬�ı�,�����ؼ���С */
RTKGUIAPI BOOL WINAPI RtkGuiWDSetState(HANDLE hWDContext, LPCSTR lpszStateText)
{
	HDC  hDCState;
	RECT Rect;
	HWND hState;
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;

	hState   = GetDlgItem(ctx->hWaitDlg, IDC_STATE);
	hDCState = GetDC(hState);
	DrawText(hDCState, lpszStateText, strlen(lpszStateText), &Rect, DT_CALCRECT);
	SetWindowPos(hState, HWND_TOP, 0, 0, 
				 Rect.right - Rect.left, 
				 Rect.bottom - Rect.top, 
				 SWP_NOMOVE);
	ReleaseDC(hState, hDCState);
    return Static_SetText(hState, lpszStateText);
}

/* ���ð�ť�ı� */
RTKGUIAPI BOOL WINAPI RtkGuiWDSetBtnText(HANDLE hWDContext, UINT uBtnID, LPCSTR lpszBtnText)
{
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;

	return Button_SetText(GetDlgItem(ctx->hWaitDlg, uBtnID), lpszBtnText);
}

/* �رյȴ��Ի��� */
RTKGUIAPI BOOL WINAPI RtkGuiWDDestroy(HANDLE hWDContext)
{
	DWORD dwWait = WAIT_OBJECT_0;
	HANDLE hTemp = NULL;
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;

	if (!ctx)
	{
		return FALSE;
	}
	
	// // 2002/8/16, Jackie, �����￪ʼ�����߳̾���Ҳ������ctx��
	hTemp = ctx->hThread;
	PostMessage(ctx->hWaitDlg, WM_CLOSE, 0, 0);
	
	if (hTemp)
	{		
		dwWait = WaitForSingleObject(hTemp, TIMEOUT);
		CloseHandle(hTemp);
	}

	if(dwWait != WAIT_OBJECT_0)
	{
		// DEBUG_PRINTF(("OOPS, WaitDlg thread time out.\n"));
	}

	return TRUE;
}

/* ���õȴ��Ի���ı��� */
RTKGUIAPI BOOL WINAPI RtkGuiWDSetCaption(HANDLE hWDContext, LPCSTR lpszCaption)
{
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;

    return SetWindowText(ctx->hWaitDlg, lpszCaption);
}

/* ��ȡ�Ǹ��ĸ���ť�������� */
RTKGUIAPI UINT WINAPI RtkGuiWDGetBtnState(HANDLE hWDContext)
{
	UINT uTemp;
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;
	
	uTemp = ctx->uLastPushBtn;
	ctx->uLastPushBtn = WDC_NONE;
	return uTemp;
}

RTKGUIAPI HWND WINAPI RtkGuiWDGetHwnd(HANDLE hWDContext)
{
	LPWAITDLG_CONTEXT ctx = (LPWAITDLG_CONTEXT)hWDContext;
	return ctx->hWaitDlg;
}

RTKGUIAPI void WINAPI RtkGuiWDEnableSkin(BOOL bEnable)
{
	g_bUseSkin = bEnable;
}