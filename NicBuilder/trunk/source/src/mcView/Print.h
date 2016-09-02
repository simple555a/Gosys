//#pragma once
//#include "event.h"

// CPrint

class CPrint  
{

public:
	CPrint();
	~CPrint();

public:
	// 打印文本
	bool printText(CString docName, CPMCEvent array_a[], CString array_b[], bool style, int Count);
	// 打印位图(原图大小)
    bool printBitmap(CString FileName);
	// 文件输出到excel表格
	bool outFile(CString str, CPMCEvent array_c[]);
	// 打印位图新方法
	void printView(CString strFileName);//
	//void OnPrint(CDC* pdc, CPrintInfo* lParam, CString strFileName1);
	void OnPrint(HDC hDC,HBITMAP hBmp,int iX,int iY,double dScaleX,double dScaleY,int iWidth,int iLength);
};