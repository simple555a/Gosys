//#pragma once
//#include "event.h"

// CPrint

class CPrint  
{

public:
	CPrint();
	~CPrint();

public:
	// ��ӡ�ı�
	bool printText(CString docName, CPMCEvent array_a[], CString array_b[], bool style, int Count);
	// ��ӡλͼ(ԭͼ��С)
    bool printBitmap(CString FileName);
	// �ļ������excel���
	bool outFile(CString str, CPMCEvent array_c[]);
	// ��ӡλͼ�·���
	void printView(CString strFileName);//
	//void OnPrint(CDC* pdc, CPrintInfo* lParam, CString strFileName1);
	void OnPrint(HDC hDC,HBITMAP hBmp,int iX,int iY,double dScaleX,double dScaleY,int iWidth,int iLength);
};