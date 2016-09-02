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
 *  关于对话框定义
 

/* 关于对话框结构体 */
#pragma pack(push, 1)
typedef struct tagRTKGUIABOUTBOXSTRUCT
{
    DWORD 	    dwSize;	        /* 此结构体大小 */
	DWORD 	    dwMask;	        /* 说明此结构体中哪个域被填写了 */
	DWORD	    dwStyle;        /* 此对话框的风格 */
	LPSTR      lpszLogo;		/* 标题图象 */
	LPSTR		lpszProductLogo;/* 产品的图标路径 */
	LPSTR		lpszCaption;	/* 关于对话框的标题，默认"关于" */
	LPSTR		lpszProduct;	/* 产品名称，默认"" */
	LPSTR		lpszVersion;	/* 产品版本号，默认 */ 
	LPSTR		lpszCopyright;	/* 版权所有，默认"交大铭泰。。。。" */
	LPSTR		lpszOtherInfo;	/* 其他信息，比如用户信息，产品序列号等等，全部填充到Edit控件中 */
	LPSTR		lpszHomePage;	/* 主页 */
	LPVOID	    lpReserved;		/* 未来扩充版本指向新的结构体 */
} RTKGUIABOUTBOXSTRUCT, FAR * LPRTKGUIABOUTBOXSTRUCT;
#pragma pack(pop)

/* 关于对话框结构体中使用的掩码 RtkGuiAboutBoxStruct->RGAB */
#define RGAB_CAPTION        0x00000001
#define RGAB_PRODUCT        0x00000002
#define RGAB_VERSION        0x00000004
#define RGAB_COPYRIGHT      0x00000008
#define RGAB_OTHERINFO      0x00000010
#define RGAB_PRODUCTLOGO    0x00000020   
#define RGAB_HOMEPAGE		0x00000040
#define RGAB_LOGO			0x00000080

/* 关于对话框风格 RtkGuiAboutBoxStyle->RGABS */
#define RGABS_OWNERCENTER   0x00000001

/* 关于对话框Ansi函数 */
RTKGUIAPI LRESULT WINAPI RtkGuiAboutBox(HWND hWndOwner, LPRTKGUIABOUTBOXSTRUCT lpRGAB );
///RTKGUIAPI LRESULT WINAPI RtkGuiAboutBoxEx( HWND hWndOwner, LPCSTR lpszIniFile);
/* 
	等待对话框风格标识
	显示什么元素是可以自行设置的
*/
#define WDS_CANCEL	    0x00000001
#define WDS_PAUSE		0x00000002
#define WDS_STATE       0x00000004
#define WDS_ANIMATE     0x00000008
#define WDS_TIMELEFT    0x00000010
#define WDS_TIMEREMAIN  0x00000020
#define WDS_PROGRESS    0x00000040


/* 等待对话框按钮标识 WaitDialogControl->WDC */
#define WDC_NONE        0x00000000	// 无按钮按下
#define WDC_CANCEL      0x00000002  // 取消按钮
#define WDC_PAUSE       0x00000004  // 暂停按钮
#define WDC_RESUME		0x00000008	// 继续按钮

/* 等待对话框返回的通知 WaitDialogNotify->WDN */
#define WDN_BTNCLICK    (WM_USER + 0x00000001)    // 按钮被按下

/* 等待对话框回调函数定义 */
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

/* 消息框返回值定义 */
#define IDALWAYSOFFSET     0x0000000a
#define IDOKALWAYS         (IDOK     + IDALWAYSOFFSET)
#define IDCANCELALWAYS     (IDCANCEL + IDALWAYSOFFSET)
#define IDYESALWAYS        (IDYES    + IDALWAYSOFFSET)
#define IDNOALWAYS		   (IDNO     + IDALWAYSOFFSET)


/* 消息框做得不大好,555~,还需要很多改进和设计 */
#define MB_OKCANCELALWAYS	(MB_OKCANCEL + IDALWAYSOFFSET)
#define MB_YESNOALWAYS      (MB_YESNO    + IDALWAYSOFFSET)

/* 消息框函数 */
RTKGUIAPI int WINAPI RtkGuiMsgBox(HWND hWndOwner, LPSTR lpText, LPSTR lpCaption, UINT uType);
RTKGUIAPI int WINAPI RtkGuiMsgBoxEx(HWND hWndOwner, LPCSTR lpText, LPCSTR lpCaption, LPCSTR lpCheckBox, UINT uType);

/* 文件信息结构体 */
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
