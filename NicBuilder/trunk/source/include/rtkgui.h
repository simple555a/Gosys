/*
	Real time kernel OS-independant GUI layer

	- created
	by Steve, xxxx-xx-xx

	- modified
	by soycola, 2004/3

*/

#ifndef _RTK_GUI_
#define _RTK_GUI_

#ifdef LIBRTKGUI_EXPORTS
#define RTKGUIAPI __declspec(dllexport)
#else
#define RTKGUIAPI __declspec(dllimport)
#endif

#ifndef LIBRTKGUI_EXPORTS
#pragma comment(lib, "libRTKGUI.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ���ڶԻ�����
 

/* ���ڶԻ���ṹ�� */
#pragma pack(push, 1)
typedef struct tagRTKGUIABOUTBOXSTRUCT
{
    DWORD 	    dwSize;	        /* �˽ṹ���С */
	DWORD 	    dwMask;	        /* ˵���˽ṹ�����ĸ�����д�� */
	DWORD	    dwStyle;        /* �˶Ի���ķ�� */
	LPSTR      lpszLogo;		/* ����ͼ�� */
	LPSTR		lpszProductLogo;/* ��Ʒ��ͼ��·�� */
	LPSTR		lpszCaption;	/* ���ڶԻ���ı��⣬Ĭ��"����" */
	LPSTR		lpszProduct;	/* ��Ʒ���ƣ�Ĭ��"" */
	LPSTR		lpszVersion;	/* ��Ʒ�汾�ţ�Ĭ�� */ 
	LPSTR		lpszCopyright;	/* ��Ȩ���У�Ĭ��"������̩��������" */
	LPSTR		lpszOtherInfo;	/* ������Ϣ�������û���Ϣ����Ʒ���кŵȵȣ�ȫ����䵽Edit�ؼ��� */
	LPSTR		lpszHomePage;	/* ��ҳ */
	LPVOID	    lpReserved;		/* δ������汾ָ���µĽṹ�� */
} RTKGUIABOUTBOXSTRUCT, FAR * LPRTKGUIABOUTBOXSTRUCT;
#pragma pack(pop)

/* ���ڶԻ���ṹ����ʹ�õ����� RtkGuiAboutBoxStruct->RGAB */
#define RGAB_CAPTION        0x00000001
#define RGAB_PRODUCT        0x00000002
#define RGAB_VERSION        0x00000004
#define RGAB_COPYRIGHT      0x00000008
#define RGAB_OTHERINFO      0x00000010
#define RGAB_PRODUCTLOGO    0x00000020   
#define RGAB_HOMEPAGE		0x00000040
#define RGAB_LOGO			0x00000080

/* ���ڶԻ����� RtkGuiAboutBoxStyle->RGABS */
#define RGABS_OWNERCENTER   0x00000001

/* ���ڶԻ���Ansi���� */
RTKGUIAPI LRESULT WINAPI RtkGuiAboutBox(HWND hWndOwner, LPRTKGUIABOUTBOXSTRUCT lpRGAB );
///RTKGUIAPI LRESULT WINAPI RtkGuiAboutBoxEx( HWND hWndOwner, LPCSTR lpszIniFile);
/* 
	�ȴ��Ի������ʶ
	��ʾʲôԪ���ǿ����������õ�
*/
#define WDS_CANCEL	    0x00000001
#define WDS_PAUSE		0x00000002
#define WDS_STATE       0x00000004
#define WDS_ANIMATE     0x00000008
#define WDS_TIMELEFT    0x00000010
#define WDS_TIMEREMAIN  0x00000020
#define WDS_PROGRESS    0x00000040


/* �ȴ��Ի���ť��ʶ WaitDialogControl->WDC */
#define WDC_NONE        0x00000000	// �ް�ť����
#define WDC_CANCEL      0x00000002  // ȡ����ť
#define WDC_PAUSE       0x00000004  // ��ͣ��ť
#define WDC_RESUME		0x00000008	// ������ť

/* �ȴ��Ի��򷵻ص�֪ͨ WaitDialogNotify->WDN */
#define WDN_BTNCLICK    (WM_USER + 0x00000001)    // ��ť������

/* �ȴ��Ի���ص��������� */
typedef INT (WINAPI *LPFNWAITDLGPROC)(HANDLE hWD, UINT uMsg, LPARAM lParam);

RTKGUIAPI HWND WINAPI RtkGuiWDGetHwnd(HANDLE hWaitDlg);
RTKGUIAPI HANDLE WINAPI RtkGuiWDStart(UINT uType, LPFNWAITDLGPROC fnWaitDlg);
RTKGUIAPI BOOL WINAPI RtkGuiWDSetPos(HANDLE hWaitDlg, UINT nPos);
RTKGUIAPI BOOL WINAPI RtkGuiWDSetState(HANDLE hWaitDlg, LPCSTR lpszStateText);
RTKGUIAPI BOOL WINAPI RtkGuiWDDestroy(HANDLE hWaitDlg);
RTKGUIAPI BOOL WINAPI RtkGuiWDSetCaption(HANDLE hWaitDlg, LPCSTR lpszCaption);
RTKGUIAPI UINT WINAPI RtkGuiWDGetBtnState(HANDLE hWaitDlg);
RTKGUIAPI BOOL WINAPI RtkGuiWDSetBtnText(HANDLE hWaitDlg, UINT uBtnID, LPCSTR lpszBtnText);
RTKGUIAPI void WINAPI RtkGuiWDEnableSkin(BOOL bEnable);

/* ��Ϣ�򷵻�ֵ���� */
#define IDALWAYSOFFSET     0x0000000a
#define IDOKALWAYS         (IDOK     + IDALWAYSOFFSET)
#define IDCANCELALWAYS     (IDCANCEL + IDALWAYSOFFSET)
#define IDYESALWAYS        (IDYES    + IDALWAYSOFFSET)
#define IDNOALWAYS		   (IDNO     + IDALWAYSOFFSET)


/* ��Ϣ�����ò����,555~,����Ҫ�ܶ�Ľ������ */
#define MB_OKCANCELALWAYS	(MB_OKCANCEL + IDALWAYSOFFSET)
#define MB_YESNOALWAYS      (MB_YESNO    + IDALWAYSOFFSET)

/* ��Ϣ���� */
RTKGUIAPI int WINAPI RtkGuiMsgBox(HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType);
RTKGUIAPI int WINAPI RtkGuiMsgBoxEx(HWND hWndOwner, LPCSTR lpText, LPCSTR lpCaption, LPCSTR lpCheckBox, UINT uType);

/* �ļ���Ϣ�ṹ�� */
#pragma pack(push, 1)
typedef struct tagRTKGUIFILEVERSION
{
	DWORD	dwMajorVersion;
	DWORD	dwMinorVersion;
	DWORD   dwReleaseVersion;
	DWORD	dwBuildNumber;
} RTKGUIFILEVERSION, FAR * LPRTKGUIFILEVERSION;
#pragma pack(pop)

RTKGUIAPI BOOL WINAPI RtkGuiGetFileVersion(RTKGUIFILEVERSION *lpvInfo, LPCSTR lpszFileName);

RTKGUIAPI BOOL WINAPI  RtkGuiLoadBMP(LPCTSTR sBMPFile, HGLOBAL *phDIB, LOGPALETTE * pLP);
RTKGUIAPI void WINAPI RtkGuiDrawDIB(HDC hdc, HGLOBAL hDIB, int x, int y);

// 2004-7-22, added by J.Hunter
RTKGUIAPI void WINAPI pmcAbout(HWND hwnd, const char * partName);

#ifdef __cplusplus
}
#endif

#endif //_RTK_GUI_
