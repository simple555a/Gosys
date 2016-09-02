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
	//���λͼ��Դ
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

//ȫ����ͼ�����������m_pBackBitmap��,flagΪtrueʱ��������������ΪfalseʱΪ��ɫ����
CScreenShutter* CScreenShutter::getScreenBitmap(bool flag)
{
    //ȡ��Ļ�ֱ���
	int width = ::GetSystemMetrics(SM_CXSCREEN);
	int heigh = ::GetSystemMetrics(SM_CYSCREEN);
    m_pBitmap = new CBitmap();

	CDC ScrDC, MemDC;//MemDC���ڴ�DC
	ScrDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	MemDC.CreateCompatibleDC(&ScrDC);
 
	//����λͼ
	m_pBitmap->CreateCompatibleBitmap(&ScrDC, width, heigh);
	//������Ļ������λͼ��ȥ
    MemDC.SelectObject(m_pBitmap);//��m_pBitmapѡ�뵽�ڴ�DC��ȥ
	if (true == flag)
	{
		MemDC.BitBlt(0, 0, width, heigh, &ScrDC, 0, 0, SRCCOPY);//����ͼ��MemDC�ڴ�DC��SRCCOPY�ɸ�Ϊ��ɫ�ȶ��ֲ���
	}
	else
	{
		MemDC.BitBlt(0, 0, width, heigh, &ScrDC, 0, 0, NOTSRCCOPY);//��ɫ
	}

	ScrDC.DeleteDC();
	MemDC.DeleteDC();
	return this;
}


//��ͼ��Ļָ������,��������ڱ���m_pBackBitmap()��
CScreenShutter* CScreenShutter::getScreenBitmap(POINT pt1, POINT pt2)
{
	return this;
}


//��Cbitmap���󱣴浽�ļ��С�lpFileNameΪλͼ�ļ���
BOOL CScreenShutter::SaveBitmapToFile(CString FileName)
{
	//�豸������
	HDC hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���
    int iBits;
	//λͼ��ÿ��������ռ�ֽ���
	WORD wBitCount;
	//�����ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//λͼ���Խṹ
	BITMAP Bitmap;
	//λͼ�ļ�ͷ�ṹ
	BITMAPFILEHEADER bmfiHdr;
	//λͼ��Ϣͷ�ṹ
	BITMAPINFOHEADER bi;
	//ָ��λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;
	//�����ļ��������ڴ�������ɫ����
	HANDLE fh, hDib, hPal, hOldPal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//��ȡ��ʾ���ľ��
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

	//λͼ�������ֽڴ�С
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
    
	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	if(NULL == hDib)
		return false;
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;
    
	//�����ɫ��
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, *m_pBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ��
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}


	//��������ת����CString���͵�lpFileName��ͼƬ�����·�����ļ�����ת��Ϊ����LPCWSTR
	USES_CONVERSION;
	LPCWSTR lpFileName = A2CW((LPCSTR)FileName);

    //����λͼ�ļ�
	CW2A lpFileNameA(lpFileName);//����ת��
    fh = CreateFile(lpFileNameA, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
	{
		DWORD ret = GetLastError();
		return FALSE;
	}
		

	//����λͼͷ�ļ�
    bmfiHdr.bfType = 0x4D42; //"BM"
	//λͼ�ļ���С
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfiHdr.bfSize = dwDIBSize;
	bmfiHdr.bfReserved1 = 0;
	bmfiHdr.bfReserved2 = 0;
	bmfiHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	
	//д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfiHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);//д�뵽�ļ�fh��
	//д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//����ڴ����

	GlobalUnlock(hDib);
    GlobalFree(hDib);
	CloseHandle(fh);

    return TRUE;
}
