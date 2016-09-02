/********************************************************************
	created:	2002/07/25
	created:	25:7:2002   17:54
	filename: 	D:\dfvs2.0\src\vc\SsiShl\SsiShlWaitDlg.h
	file path:	D:\dfvs2.0\src\vc\SsiShl
	file base:	SsiShlWaitDlg
	file ext:	h
	author:		steven
	
	purpose:	
*********************************************************************/

#ifndef _SSI_WAITDLG_SHL_
#define _SSI_WAITDLG_SHL_

/* 等待对话框环境上下文 */
#pragma pack(push, 1)
typedef struct tag_WAITDLG_CONTEXT
{
	HWND			hWaitDlg;
	UINT			uType;
	HANDLE			hEvent;
	UINT			uLastPushBtn;
	UINT			nPos;
	DWORD			dwTimeRemain, dwTimeLeft, dwLastTime;
	BOOL			bPause;
	HANDLE			hThread;
	LPFNWAITDLGPROC fnWaitDlg;
}WAITDLG_CONTEXT, *LPWAITDLG_CONTEXT;
#pragma pack(pop)

#endif // _SSI_WAITDLG_SHL_
