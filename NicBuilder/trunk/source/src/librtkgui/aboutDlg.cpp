/********************************************************************
	created:	2002.07.02	 
	
	purpose:	公共关于对话框，所有gui应用程序通过调用注册
				ssishlaboutbox结构体ssishlaboutbox函数
				
*********************************************************************/
#include "precomp.h"
#pragma hdrstop
#include "dllversion.h"
#include "utils.h"
#include "resource.h"
#include <stdio.h>
#include <Lmcons.h>
#include "hyperlink.h"

char  *szCodeText="B6ABB7BDCEC0CABF323030330D0AD7DCB2DFBBAE0D0A20202020BACED5BDCCD20D0A202020200D0AB8DFB2E3BEADC0ED0D0A20202020B0FCCEB020BAABD5D7C7BF0D0A202020200D0AB2FAC6B7BEADC0EDA3A8CAD0B3A1A3A90D0A20202020CBEFC3F70D0A202020200D0ACFEEC4BFBEADC0ED0D0A20202020C5CBCFFEBBD40D0A202020200D0AC8EDBCFEBEADC0ED0D0A20202020CBEFC5CA0D0A202020200D0AB2FAC6B7BEADC0EDA3A8D1D0B7A2A3A90D0A20202020D1EEC8AA0D0A202020200D0AD3C3BBA7B4FAB1ED0D0A20202020CDF5D3EE0D0A202020200D0AB3CCD0F20D0A20202020CDF5D6BEC7BF20C1D6BAE3CEC420CDF2C4BEBAE320B0B2BAE9D4C320C0EECFE8F4B420CEBAC8E7BBD420D5C5CAA4B1F820D5C2CBB6BDDC20D6DCBEA2D3F020C3B7CEC0B7E520B3C2CEC4C7C920D6DCD4C6C5F420BFC2D4BEC7BF20B6C5D0F1B6AB20D5C5D0A1C6BD0D0A202020200D0AC3C0CAF50D0A20202020BBC6CFFEBBB620C0EED1DE20D6DCBEB220CDF5B7EFCEB00D0A202020200D0ACEC4B5B50D0A20202020C0EED1DEC4FE202020200D0A202020200D0AB2E2CAD40D0A20202020DDD1D4C6B7C920CDF5BAEC20CDF5BFA120B8B6D4C620C0EEC5F420BDAAD3F1B7D220D5C5C5F40D0A202020200D0AD6CAB1A30D0A20202020C0EEB1F9BDE020CEE2B3C9D1E00D0A202020200D0AD3D1C7E9D0ADD6FA0D0A20202020CCC6B8FD20C0EECFFED3C220D5C5BCF9F7A120B3C2B8D520CDF5D3C220CDF5C8D920C0EEBAF1C5F420CDF5D4C6D4F320B3C2D6BEBBAA20B3C2BEB8CBC920CFAFC1ACD3F1206C753020676A706C616E640D0A202020200D0AC8C8D0C4D3C3BBA70D0A20202020B2DCD4BD20C3CFD1F420D1EED0EDC3F720D0ECD2A220D1EEB2A820CEE2CEB020C2BDD1F30D0A0D0ACCD8B1F0B8D0D0BB0D0A20202020";
char  *szMember=NULL ;


#define Y_INC	-1

extern HINSTANCE g_hInst;

static RTKGUIABOUTBOXSTRUCT * g_lpRGAB = NULL;
HICON	g_hProductLogo;
HBITMAP g_hLogo;
CHyperlink g_link;


inline void _SetCtrlImage(DWORD dwMask, DWORD dwMaskInclude, LPCSTR lpszImageFile, UINT uType, HWND hCtrl)
{
	
	if (dwMask & dwMaskInclude)
	{ 
		switch(uType)
		{
			case IMAGE_BITMAP:
				{
					g_hLogo = (HBITMAP)LoadImage(NULL, lpszImageFile, uType, 0, 0, LR_LOADFROMFILE);
					SendMessage(hCtrl, STM_SETIMAGE, (WPARAM)uType, (LPARAM)g_hLogo);
				}
			break;

			case IMAGE_ICON:
				{
					g_hProductLogo = (HICON)LoadImage(NULL, lpszImageFile, uType, 32, 32, LR_LOADFROMFILE);
					SendMessage(hCtrl, STM_SETIMAGE, (WPARAM)uType, (LPARAM)g_hProductLogo);
				}
			break;
		}
		
		
		
	}
}

/* 设置控件文字的ANSI版本，调用SetCtrlTextW版本 */
inline BOOL _SetCtrlText(DWORD dwMask, DWORD dwMaskInclude, LPCSTR lpText, HWND hCtrl, BOOL bSetSize)
{
	BOOL bReturn = FALSE;
	RECT Rect;
	HDC  hDCCtrl;

	if (dwMask & dwMaskInclude) 
	{
		if (bSetSize) 
		{
			hDCCtrl = GetDC(hCtrl);
			DrawText(hDCCtrl, lpText, strlen(lpText), &Rect, DT_CALCRECT);
			SetWindowPos(hCtrl, HWND_TOP, 0, 0, 
						 Rect.right - Rect.left, 
						 Rect.bottom - Rect.top, 
						 SWP_NOMOVE);
			ReleaseDC(hCtrl, hDCCtrl);
		}
		
		bReturn = SetWindowText(hCtrl, lpText);
	}
	return bReturn;
}

/* 设置关于对话框的风格 */
inline BOOL WINAPI _SetupAboutBoxStyle(DWORD dwStyle, HWND hDlg)
{
	/*
	HWND hOwner;
	RECT rOwner, rDlg;
	int  nWidthOwner, nHeightOwner;
	int  nLeftDlg, nTopDlg, nWidthDlg, nHeightDlg;
	int  nScreenHeight, nScreenWidth;

	// 对话框位置在物主窗口中间 
	
	if (dwStyle & RGABS_OWNERCENTER) 
	{
		hOwner = GetWindow(hDlg, GW_OWNER);
		GetWindowRect(hOwner, &rOwner);
		GetWindowRect(hDlg,   &rDlg);
		
		nWidthOwner   = rOwner.right  - rOwner.left;
		nHeightOwner  = rOwner.bottom - rOwner.top;
		nWidthDlg	  = rDlg.right	  - rDlg.left;
		nHeightDlg	  = rDlg.bottom   - rDlg.top;
		nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);

		nLeftDlg	  = rOwner.left + (nWidthOwner	- nWidthDlg)  / 2;
		nTopDlg 	  = rOwner.top	+ (nHeightOwner - nHeightDlg) / 2;
 
		if (nLeftDlg < 0) 
		{
			nLeftDlg = 0;
		} 
		else if (nLeftDlg > (nScreenWidth - nWidthDlg)) 
		{
			nLeftDlg = nScreenWidth - nWidthDlg;
		}

		if (nTopDlg < 0) 
		{
			nTopDlg = 0;
		}
		else if (nTopDlg > (nScreenHeight - nHeightDlg))
		{
			nTopDlg = nScreenHeight - nHeightDlg;
		}

		MoveWindow( hDlg, nLeftDlg, nTopDlg, 
					nWidthDlg, nHeightDlg, TRUE );
	}
*/
	return TRUE;
}

VOID CALLBACK Show(
  HWND hDlg,		 // handle to window
  UINT uMsg,		 // WM_TIMER message
  UINT idEvent,  // timer identifier
  DWORD dwTime		 // current system time
)
{
	HWND hwnd = GetDlgItem(hDlg, IDC_OTHERINFO);
	if(hwnd)
	{	
		DWORD dwResult=SendMessage(hwnd,EM_SCROLL,SB_LINEDOWN, 0);
		if(dwResult==0)
		{
			SendMessage(hwnd,EM_LINESCROLL,0, -1000);
		}
		UpdateWindow(hwnd);
	}	
}

int _CodeText(LPCTSTR szText,LPTSTR szHex)
{
	int i;
	int len;
	LPTSTR szCur=NULL;
	len=strlen(szText);
	szCur=szHex;
	for (i=0;i<len;i++)
	{
		sprintf(szCur,"%02X",(unsigned char)szText[i]);		
	//	TRACE1("%02X\n",szText[i]);
		szCur+=2;
	}
	return i*2;
}	

int _UnCodeText(LPCTSTR szHex,LPTSTR szText)
{
	int i;
	int len;
	CHAR szBuf[3];
	LPTSTR szCur=NULL;
	len=strlen(szHex)/2;
	szCur=szText;
	for (i=0;i<len;i++)
	{
		szBuf[0]=szHex[i*2];
		szBuf[1]=szHex[i*2+1];
		szBuf[2]='\0';
		sscanf(szBuf,"%X",&szCur[i]);
	}	
	szCur[i]='\0';
	return i;
}

BOOL _SetUnderLine(HWND hDlg,int nDlgItem)
{
   
   HFONT hDlgFont;
   LOGFONT lFont;
   if ((hDlgFont = (HFONT)::SendMessage(hDlg, WM_GETFONT, 0, 0L))!= NULL)
   {
		if (GetObject(hDlgFont, sizeof(LOGFONT), (LPSTR)&lFont)!= NULL)
		{
			 lFont.lfUnderline = TRUE;
			 if ((hDlgFont = CreateFontIndirect(&lFont)) != NULL)
			 {
				::SendDlgItemMessage(hDlg, nDlgItem, WM_SETFONT, (DWORD)hDlgFont, 0L);
				return TRUE;
			 }
		}		 
   }
   return FALSE;

}
BOOL WINAPI _SsiShlAboutBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush;
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_CAPTION,	g_lpRGAB->lpszCaption,	 hDlg, FALSE);
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_PRODUCT,	g_lpRGAB->lpszProduct,	 GetDlgItem(hDlg, IDC_PRODUCT), TRUE);
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_VERSION,	g_lpRGAB->lpszVersion,	 GetDlgItem(hDlg, IDC_VERSION), TRUE);
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_COPYRIGHT,	g_lpRGAB->lpszCopyright, GetDlgItem(hDlg, IDC_COPYRIGHT), TRUE);
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_OTHERINFO,	g_lpRGAB->lpszOtherInfo, GetDlgItem(hDlg, IDC_OTHERINFO), FALSE);
				_SetCtrlText(g_lpRGAB->dwMask, RGAB_HOMEPAGE,	g_lpRGAB->lpszHomePage,	 GetDlgItem(hDlg, IDC_HOMEPAGE), TRUE);
				_SetCtrlImage(g_lpRGAB->dwMask, RGAB_PRODUCTLOGO,g_lpRGAB->lpszProductLogo, IMAGE_ICON, GetDlgItem(hDlg, IDC_PRODUCTLOGO));
				_SetCtrlImage(g_lpRGAB->dwMask, RGAB_LOGO, g_lpRGAB->lpszLogo, IMAGE_BITMAP, GetDlgItem(hDlg, IDC_LOGO));
			
				//EnableWindow(GetDlgItem(hDlg, IDC_OTHERINFO), FALSE);
				SendMessage(GetDlgItem(hDlg, IDC_HOMEPAGE), STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)LoadCursor(NULL, (LPCSTR)IDC_SIZE));
			
			
				_SetupAboutBoxStyle(g_lpRGAB->dwStyle, hDlg);								
				_SetUnderLine(hDlg,IDC_HOMEPAGE);
				RECT  RectHomePage;
				::GetWindowRect(::GetDlgItem(hDlg, IDC_HOMEPAGE), &RectHomePage);	
				::ScreenToClient(hDlg,(LPPOINT)(&RectHomePage));
				::ScreenToClient(hDlg,&((LPPOINT)(&RectHomePage))[1]);

				if(g_lpRGAB->dwMask&RGAB_HOMEPAGE&&g_lpRGAB->lpszHomePage)
				{	
					g_link.Create(&RectHomePage,hDlg,g_lpRGAB->lpszHomePage,g_lpRGAB->lpszHomePage,119,(LPCTSTR)IDC_CURSORHAND);					
				}
				else
				{
					g_link.Create(&RectHomePage,hDlg,"http://www.actech-mc.com","http://www.actech-mc.com",119,(LPCTSTR)IDC_CURSORHAND);					
				}						
				return TRUE;
			}
			break;

		case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
					case IDOK:
						EndDialog(hDlg, FALSE);
						if(szMember!=NULL)
						{
							delete szMember;
							szMember=NULL;
						}
						return FALSE;
				}
			}
			break;
		
		case WM_LBUTTONDBLCLK:
			{
				if (wParam == (MK_CONTROL | MK_LBUTTON ))
				{			
					if(szMember==NULL)
					{
						szMember=new char[strlen(szCodeText)/2+1+UNLEN];
						if(szMember!=NULL)
						{
							_UnCodeText(szCodeText,szMember);
							CHAR szUserName[UNLEN+1]={0};
							ULONG nSize=UNLEN;
							GetUserName(szUserName,&nSize);
							strcat(szMember,szUserName);
							strcat(szMember,"\r\n\r\n\r\n\r\n");
						}
					}
					
					Edit_SetText(GetDlgItem(hDlg, IDC_OTHERINFO), szMember);
					int nCount = Edit_GetLineCount(GetDlgItem(hDlg, IDC_OTHERINFO));
					SetTimer(hDlg, nCount, 800, Show);
				}

			}
			break;
		
				
		case WM_MOUSEMOVE:
			{
				RECT  RectHomePage;
				POINT pt;

				GetCursorPos(&pt);

				GetWindowRect(GetDlgItem(hDlg, IDC_HOMEPAGE), &RectHomePage);
			
				if (PtInRect(&RectHomePage, pt)) 
				{
					SetCursor(LoadCursor(g_hInst, (LPCTSTR)IDC_CURSORHAND));	
				} 
				else 
				{
					SetCursor(LoadCursor(NULL, IDC_ARROW));
				}	
			}
			break;

		case WM_LBUTTONDOWN:
			{
				HWND  hWndHomePage = GetDlgItem(hDlg, IDC_HOMEPAGE);
				RECT  RectHomePage;
				POINT pt;
				char  szText[MAX_PATH];

				GetCursorPos(&pt);
				GetWindowText(hWndHomePage, szText, sizeof(szText));
				GetWindowRect(hWndHomePage, &RectHomePage);
			
				if (PtInRect(&RectHomePage, pt)) 
				{
				   ShellExecute(NULL, "open", szText, "", "", SW_SHOW);
				}
			}
			break;
   
		case WM_DESTROY:
			if (g_lpRGAB)
			{
				g_link.Destroy();
				if (g_hProductLogo)
				{
					DestroyIcon(g_hProductLogo);
					g_hProductLogo = NULL;
				}
				if (g_hLogo)
				{
					DeleteObject(g_hLogo);
					g_hLogo = NULL;
				}
				free(g_lpRGAB);
			}			 
			break;
		default:
			break; 
	}
	return FALSE;
}



// 关于对话框函数ANSI版本
RTKGUIAPI LRESULT WINAPI RtkGuiAboutBox(HWND hWndOwner, LPRTKGUIABOUTBOXSTRUCT lpRGAB )
{
	if (!lpRGAB) 
	{
		return -1;
	}
   
	// 将传过来的结构体拷贝到本地
	g_lpRGAB = (RTKGUIABOUTBOXSTRUCT*)malloc(sizeof(RTKGUIABOUTBOXSTRUCT));
	memcpy(g_lpRGAB, lpRGAB, lpRGAB->dwSize);

   
	// 显示关于对话框
	int r=DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWndOwner, _SsiShlAboutBoxProc);	 
	if(r==-1){
		int err=GetLastError();
	}

	return 1;
}

/*
void _GetLibsInfo(CHAR *szOtherInfo, CHAR *szLibs)
{
	int   i = 0;
	char  szVersion[MAX_PATH], szProductName[MAX_PATH];

	CDLLVersion *Version;
	
	sprintf(szOtherInfo, "");
	do
	{		
		Version = new CDLLVersion(&szLibs[i], MOD_DIR);	
			
		sprintf(szVersion, "%d.%d", Version->GetMajorVersion(), Version->GetMinorVersion());
		sprintf(szOtherInfo, "%s\r\n%s\t版本: %s", szOtherInfo, szProductName, szVersion);
				
		delete Version;
		
				
		i += strlen(&szLibs[i]) + 1;
		
		
	} while (szLibs[i]);

}
*/

/* 从ini文件读取About配置,属于SsiShlAboutBox外壳 */
/*
RTKGUIAPI LRESULT WINAPI SsiShlAboutBoxEx(HWND hWndOwner, LPCSTR lpszIniFile)
{
	BOOL  bSoundLoop;
	char  szProduct[MAX_PATH], szProductLogo[MAX_PATH], szVersion[MAX_PATH], szHomePage[MAX_PATH], szLogo[MAX_PATH], szSound[MAX_PATH], szAppPath[MAX_PATH];
	char  szTemp[2 * MAX_PATH];
	DWORD fdwSound = SND_ASYNC | SND_FILENAME | SND_NOWAIT | SND_NODEFAULT;
	DWORD dwMask = 0;
	HMODULE hWinmmLib = NULL;
	BOOL  (WINAPI * lpfnPlaySoundA)(LPCSTR, HMODULE, DWORD) = NULL;
	

	RTKGUIABOUTBOXSTRUCT RGAB;

	ZeroMemory(&RGAB, sizeof(RGAB));
	RGAB.dwSize  = sizeof(RGAB);
	RGAB.dwStyle = RGABS_OWNERCENTER;

	FixModulePath(g_hInst, szAppPath);
	

	GetPrivateProfileString("MAIN", "product name", "", szProduct, MAX_PATH, lpszIniFile);
	if (*szProduct)
	{
		dwMask |= RGAB_PRODUCT;
		RGAB.lpszProduct = szProduct;
	}

	GetPrivateProfileString("MAIN", "product icon",   "", szTemp, MAX_PATH, lpszIniFile);
	if (*szTemp)
	{
		sprintf(szProductLogo, "%s\\%s", szAppPath, szTemp);
		dwMask |= RGAB_PRODUCTLOGO;
		RGAB.lpszProductLogo  = szProductLogo;
	}

	GetPrivateProfileString("MAIN", "version number", "", szVersion, MAX_PATH, lpszIniFile);
	if (*szVersion)
	{
		dwMask |= RGAB_VERSION;
		RGAB.lpszVersion = szVersion;
	}

	GetPrivateProfileString("MAIN", "web addr", "", szHomePage, MAX_PATH, lpszIniFile);
	if (*szHomePage)
	{
		dwMask |= RGAB_HOMEPAGE;
		RGAB.lpszHomePage = szHomePage;
	}

	GetPrivateProfileString("MEDIA", "Image", "", szTemp, MAX_PATH, lpszIniFile);
	if (*szTemp)
	{
		dwMask |= RGAB_LOGO;
		sprintf(szLogo, "%s\\%s", szAppPath, szTemp);
		RGAB.lpszLogo = szLogo;
	}
	

	GetPrivateProfileSection("LIB", szTemp, 2 * MAX_PATH, lpszIniFile);
	if (*szTemp)
	{
		_GetLibsInfo(szOtherInfo, szTemp);
		dwMask |= RGAB_OTHERINFO;
		RGAB.lpszOtherInfo = szOtherInfo;
	}

	bSoundLoop = GetPrivateProfileInt("MEDIA", "Sound Loop", 0, lpszIniFile);
	GetPrivateProfileString("MEDIA", "Sound", "", szTemp, MAX_PATH, lpszIniFile);
	if (*szTemp)
	{	
		sprintf(szSound, "%s\\%s", szAppPath, szTemp);
		if (bSoundLoop) 
		{
			fdwSound |= SND_LOOP;
		}

		hWinmmLib = LoadLibrary("winmm.dll");
		if (hWinmmLib)
		{
			(FARPROC&)lpfnPlaySoundA = GetProcAddress(hWinmmLib, "PlaySoundA");
			if (lpfnPlaySoundA)
			{
				lpfnPlaySoundA(szSound, NULL, fdwSound);
			}
			else
			{
				FreeLibrary(hWinmmLib);
			}
		}
	}
	
	
	RGAB.dwMask = dwMask;

	SsiShlAboutBox(hWndOwner, &RGAB);
	
	// 清除资源
	if (hWinmmLib)
	{
		lpfnPlaySoundA(NULL, NULL, 0);
		FreeLibrary(hWinmmLib);
	}

	

	return 1;
}
*/