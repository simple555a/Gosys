#include "precomp.h"
#include <librtk.h>

// App command to run the dialog
RTKGUIAPI void WINAPI pmcAbout(HWND hwnd, const char * partName)
{
	RTKGUIABOUTBOXSTRUCT s;
	RTKGUIFILEVERSION f;
	char product[MAX_PATH];
	char sTemp[MAX_PATH];
	char logo[MAX_PATH];
	char version[MAX_PATH];
	char productLogo[MAX_PATH];
	
	sprintf(sTemp,  "%slibrtdb.dll", get_exe_dir());
	RtkGuiGetFileVersion(&f, sTemp);
	
	ZeroMemory(&s, sizeof(s));
	s.dwSize       = sizeof(s);
	s.dwMask       =   RGAB_PRODUCT 
		| RGAB_CAPTION 
		| RGAB_PRODUCTLOGO 
		| RGAB_OTHERINFO 
		| RGAB_HOMEPAGE
		| RGAB_COPYRIGHT
		| RGAB_VERSION
		| RGAB_LOGO;
	s.dwStyle          = RGABS_OWNERCENTER;
	sprintf(product, "工业控制系统监控组态软件 - %s", partName);
	s.lpszProduct      = product;
	s.lpszCaption      = "关于MC Draw";
	sprintf(productLogo, "%sres/company.ico", get_exe_dir());
	s.lpszProductLogo = productLogo;
	
	s.lpszOtherInfo	  = "-+= 高起点、高标准、高水平 =+-";
	s.lpszHomePage     = "http://www.cncs.bj,cn";
	s.lpszCopyright    = "版权所有：北京中核东方控制系统工程有限公司";
	sprintf(logo, "%sres\\logo.bmp", get_exe_dir());
	s.lpszLogo = logo;
	sprintf(
		version, 
		"版本 %d.%d.%d.%d", 
		f.dwMajorVersion, 
		f.dwMinorVersion, 
		f.dwReleaseVersion, 
		f.dwBuildNumber
		);
	s.lpszVersion      = version;
	
	RtkGuiAboutBox(hwnd, &s);
}