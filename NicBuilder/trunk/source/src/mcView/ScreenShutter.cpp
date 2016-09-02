// CScreenShutter.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenShutter.h"

using namespace std;


CScreenShutter::CScreenShutter()
:m_pBitmap(NULL)
{
}

CScreenShutter::~CScreenShutter()
{
	//清除位图资源
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

//全屏截图，结果保存在m_pBackBitmap中,flag为true时候是正常拷贝，为false时为反色拷贝
CScreenShutter* CScreenShutter::getScreenBitmap(bool flag)
{
    //取屏幕分辨率
	int width = ::GetSystemMetrics(SM_CXSCREEN);
	int heigh = ::GetSystemMetrics(SM_CYSCREEN);
    m_pBitmap = new CBitmap();

	CDC ScrDC, MemDC;//MemDC是内存DC
	ScrDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	MemDC.CreateCompatibleDC(&ScrDC);
 
	//创建位图
	m_pBitmap->CreateCompatibleBitmap(&ScrDC, width, heigh);
	//拷贝屏幕的区域到位图中去
    MemDC.SelectObject(m_pBitmap);//将m_pBitmap选入到内存DC中去
	if (true == flag)
	{
		MemDC.BitBlt(0, 0, width, heigh, &ScrDC, 0, 0, SRCCOPY);//复制图像到MemDC内存DC，SRCCOPY可改为反色等多种参数
	}
	else
	{
		MemDC.BitBlt(0, 0, width, heigh, &ScrDC, 0, 0, NOTSRCCOPY);//反色
	}

	ScrDC.DeleteDC();
	MemDC.DeleteDC();
	return this;
}


//截图屏幕指定区域,结果保存在变量m_pBackBitmap()中
CScreenShutter* CScreenShutter::getScreenBitmap(POINT pt1, POINT pt2)
{
	return this;
}


//把Cbitmap对象保存到文件中、lpFileName为位图文件名
BOOL CScreenShutter::SaveBitmapToFile(CString FileName)
{
	//设备描述表
	HDC hDC;
	//当前分辨率下每像素所占字节数
    int iBits;
	//位图中每个像素所占字节数
	WORD wBitCount;
	//定义调色板大小，位图中像素字节大小，位图文件大小，写入文件字节数
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构
	BITMAP Bitmap;
	//位图文件头结构
	BITMAPFILEHEADER bmfiHdr;
	//位图信息头结构
	BITMAPINFOHEADER bi;
	//指向位图信息头结构
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄
	HANDLE fh, hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//获取显示器的句柄
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1) wBitCount = 1;
	else if(iBits <= 4) wBitCount = 4;
	else if(iBits <= 8) wBitCount = 8;
	else wBitCount = 24;
  
	GetObject(*m_pBitmap, sizeof(Bitmap), (LPTSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant =0;
	bi.biClrUsed = 0;

	//位图中像素字节大小
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
    
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	if(NULL == hDib)
		return false;
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;
    
	//处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//获取该调色板下新的像素值
	GetDIBits(hDC, *m_pBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//恢复调色板
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}


	//参数类型转换将CString类型的lpFileName（图片保存的路径和文件名）转换为类型LPCWSTR
	USES_CONVERSION;
	LPCWSTR lpFileName = A2CW((LPCSTR)FileName);

    //创建位图文件
	CW2A lpFileNameA(lpFileName);//类型转换
    fh = CreateFile(lpFileNameA, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
	{
		DWORD ret = GetLastError();
		return FALSE;
	}
		

	//设置位图头文件
    bmfiHdr.bfType = 0x4D42; //"BM"
	//位图文件大小
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfiHdr.bfSize = dwDIBSize;
	bmfiHdr.bfReserved1 = 0;
	bmfiHdr.bfReserved2 = 0;
	bmfiHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	
	//写入位图文件头
	WriteFile(fh, (LPSTR)&bmfiHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);//写入到文件fh中
	//写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除内存分配

	GlobalUnlock(hDib);
    GlobalFree(hDib);
	CloseHandle(fh);

    return TRUE;
}
