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
	sprintf(product, "��ҵ����ϵͳ�����̬��� - %s", partName);
	s.lpszProduct      = product;
	s.lpszCaption      = "����MC Draw";
	sprintf(productLogo, "%sres/company.ico", get_exe_dir());
	s.lpszProductLogo = productLogo;
	
	s.lpszOtherInfo	  = "-+= ����㡢�߱�׼����ˮƽ =+-";
	s.lpszHomePage     = "http://www.cncs.bj,cn";
	s.lpszCopyright    = "��Ȩ���У������к˶�������ϵͳ�������޹�˾";
	sprintf(logo, "%sres\\logo.bmp", get_exe_dir());
	s.lpszLogo = logo;
	sprintf(
		version, 
		"�汾 %d.%d.%d.%d", 
		f.dwMajorVersion, 
		f.dwMinorVersion, 
		f.dwReleaseVersion, 
		f.dwBuildNumber
		);
	s.lpszVersion      = version;
	
	RtkGuiAboutBox(hwnd, &s);
}