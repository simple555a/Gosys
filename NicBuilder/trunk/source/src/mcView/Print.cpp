// Print.cpp : implementation file
//

#include "stdafx.h"
#include "Print.h"


using namespace std;

CPrint::CPrint()
{
}

CPrint::~CPrint()
{
}

// 文本打印
//docName表示要打印文件的名字。打印array_a中的内容，array_b中是日志字段的名称，不同的日志字段不同。
//style是打印的方式（打印或反色打印）,Count是array_b中元素的个数，用来限定循环的次数
bool CPrint::printText(CString docName, CPMCEvent array_a[], CString array_b[], bool style, int Count)
{

	CPrintDialog dlg(FALSE);//如果设为TRUE，则不能实现打印功能，而是建立打印机
	if (IDOK != dlg.DoModal())
	{
		return false;
	}

	HDC hdcPtrinter = dlg.GetPrinterDC();//获取打印机的DC
	if (NULL == hdcPtrinter)
	{
		AfxMessageBox("无法找到打印机", NULL, MB_OK | MB_ICONSTOP);
		return false;
	}
	else
	{
		CDC dcPrinter;
		dcPrinter.Attach(hdcPtrinter);
		//通知doc开始打印
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = docName;

		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			AfxMessageBox("无法初始化打印机", NULL, MB_OK | MB_ICONSTOP);
			return false;
		}
		else
		{
			dcPrinter.SetMapMode(MM_TWIPS);//Each logical unit is converted to 1/20 of a point.a twip is 1/1440 inch
			CFont *pOldFont;
			CFont fnt;
			if (true == style)//ture为硬拷贝，false为反色拷贝
			{
				//设置字体
				fnt.CreatePointFont(70, "宋体", &dcPrinter);
				//设置字体颜色
				pOldFont = (CFont*)dcPrinter.SelectObject(&fnt);

			}
			else
			{   
				//打印机默认设置
				pOldFont = (CFont*)dcPrinter.SelectStockObject(DEVICE_DEFAULT_FONT);
			}

			CPoint pt(dcPrinter.GetDeviceCaps(HORZRES), dcPrinter.GetDeviceCaps(VERTRES));
			dcPrinter.DPtoLP(&pt);//Converts device units into logical units.
			pt.y = -pt.y;

			//给每一列的宽度赋值
			vector<CSize> width;
			for(int i = 0; i < Count; i++)
			{
				width.push_back(dcPrinter.GetTextExtent(array_b[i]));    
			}

			//
			double lineHeight = width[0].cy * 1.5;//设置每一行的高度
			double lineCount = (pt.y - width[0].cy * 12) / lineHeight;//每页打印的行数
			if(lineCount < 0)
				return false;
			long id = 0;
			CString str;
			int k = 0, iPage = 0;//页数
			double leftWidth, cLeftGaP = 800;//cLeftGaP左边留空的距离，是不变的。leftWidth是要打印的字段离纸张最左端的距离是变化的
			while(array_a[id].valid && id < 1024)//循环一次是打印一页
			{
				//start a page，打印一页
				if (dcPrinter.StartPage() < 0)
				{
					AfxMessageBox("无法初始化页", NULL, MB_OK | MB_ICONSTOP);
					dcPrinter.AbortDoc();
					return false;
				}
				else
				{
					iPage++;
					str.Format("第%2d页", iPage);
					dcPrinter.TextOut((int)cLeftGaP, (int)-lineHeight * 3, str);
					//打印第一行的表头
					leftWidth = cLeftGaP;//第一列的内容到最左边的距离，就是左边留空的大小
					for (int j = 0; j < Count; j++)
					{
						dcPrinter.TextOut((int)leftWidth, (int)-lineHeight * 5, array_b[j]);
						leftWidth += width[j].cx;
					}
					//打印数据
					leftWidth = cLeftGaP;
					for(int m = 0; m < lineCount; m++)//一次循环为一行
					{
						if (array_a[id].valid)
						{
							CArgs args;
							parse_arg_ey(array_a[id].data, &args, ";"); 
							str.Format("%3d", id);
							dcPrinter.TextOut((int)leftWidth, (int)-lineHeight * (6 + m), str);
							for (int k = 0; k < Count-1; k++)
							{
								leftWidth += width[k].cx;//从第二列的宽度开始
								str.Format("%s", args.argv[k]);
								dcPrinter.TextOut((int)leftWidth, (int)-lineHeight * (6 + m), str);	
							}
							id++;
							leftWidth = cLeftGaP;
						}
						else
						{
							break;
						}
					}	
				}
				dcPrinter.EndPage();//此页结束
			}
			fnt.DeleteObject();
			if (pOldFont != NULL)
				dcPrinter.SelectObject(pOldFont);
			dcPrinter.EndDoc();
		}
		dcPrinter.DeleteDC();
		return true;
	}	
}   
//位图打印,FileName是要打印的文件的路径及文件名
bool CPrint::printBitmap(CString FileName)
{
	CDC memDC, dc;
	//CClientDC dc(this);
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//获取屏幕的句柄

	//取屏幕分辨率,即位图的长和宽
	int bmpWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int bmpHeight = ::GetSystemMetrics(SM_CYSCREEN);
	memDC.CreateCompatibleDC(&dc);
	CBitmap* bitmap = new CBitmap();

	//转换FileName的类型，由CString变为LPCTSTR
	USES_CONVERSION;
	LPCTSTR lpszBmpFile = A2CT((LPCSTR)FileName);

	//加载要打印的位图
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, lpszBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	bitmap->Attach(hBitmap);

	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(bitmap);
	if (NULL == pOldBitmap)//if the bitmap is very big, better to check it,
	{
		memDC.DeleteDC();
		delete bitmap;
		return false;    
	}
	//打印
	CDC prtDC;
	CPrintInfo printInfo;
	CSize size;
	DOCINFO di;
	CString szPortName, szAppName, szPrintError;
	szAppName.LoadString(AFX_IDS_APP_TITLE);
	szPrintError = "";

	CSize paper_size;
	int xLongPPI = 0;//
	int yLongPPI = 0;//

	if (AfxGetApp()->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd))
	{
		HDC hDC = printInfo.m_pPD->m_pd.hDC;
		if (hDC == NULL)
		{
			hDC = printInfo.m_pPD->CreatePrinterDC();//创建打印机
		}
		if (hDC != NULL)//创建成功
		{
			prtDC.Attach(hDC);
			paper_size.cx = prtDC.GetDeviceCaps(HORZSIZE);//210，纸张的宽度
			paper_size.cy = prtDC.GetDeviceCaps(VERTSIZE);//297，纸张的高度
			xLongPPI = prtDC.GetDeviceCaps(LOGPIXELSX);//600,纸张宽度每逻辑英寸的像素数
			yLongPPI = prtDC.GetDeviceCaps(LOGPIXELSY);//600，纸张宽度每逻辑英寸的像素数
		}
		else
		{
			AfxMessageBox("无法找到打印机");
			return false;
		}
	}
	int scr_xLongPPI = dc.GetDeviceCaps(LOGPIXELSX);//96
	int scr_yLongPPI = dc.GetDeviceCaps(LOGPIXELSY);//96
	int paper_width = (int)((double)paper_size.cx * (double)xLongPPI / 25.4);
	int paper_height = (int)((double)paper_size.cy * (double)yLongPPI / 25.4);
	double ratio_x = (double)xLongPPI / (double)scr_xLongPPI;
	double ratio_y = (double)yLongPPI / (double)scr_yLongPPI;

	CString strPageNumber = "";

	int page_info_left = (int)((double)paper_width * 0.9);
	int page_info_right = paper_width; 
	int page_info_top = (int)((double)paper_height * 0.99);
	int page_info_bottom = paper_height;

	CRect page_info_rect = CRect(page_info_left, page_info_top, page_info_right, page_info_bottom);
	int printed_pages = 0;
	//int total_print_pages =0;
	BOOL bAbort_print = FALSE;

	//calculate pages
	int total_pages = (int)(bmpWidth * ratio_x + paper_width - 1) / paper_width;
	//pop up printer dialog
	CPrintDialog prtDlg(FALSE, PD_PAGENUMS);

	prtDlg.m_pd.nMinPage = 1;
	prtDlg.m_pd.nMaxPage = total_pages;
	prtDlg.m_pd.nFromPage = 1;
	prtDlg.m_pd.nToPage = total_pages;

	if (prtDlg.DoModal() == IDOK)
	{
		memset(&di, 0, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = szAppName;
		szPortName = prtDlg.GetPortName();
		di.lpszOutput = szPortName;
		prtDC.m_bPrinting = TRUE;
	}
	else
	{
		return false;
	}

	if (prtDC.StartDoc(&di) == -1)
	{
		AfxMessageBox("无法找到打印机");
		prtDC.Detach();
		prtDC.DeleteDC();
		return false;
	}

	prtDC.SetMapMode(MM_TEXT);//Sets the mapping mode.

	for (int i = 0; i < total_pages; i++)
	{
		prtDC.StartPage();
		strPageNumber.Format("第%d页，共%d页", ++printed_pages, total_pages);

		if ( i == (total_pages - 1) && total_pages > 1)//最后一页
		{
			int last_bmpWidth = (int)(bmpWidth - paper_width / ratio_x * i);
			prtDC.StretchBlt(0, 0, (int)(last_bmpWidth * ratio_x), (int)(bmpHeight* ratio_y), &memDC, (int)(paper_width * i / ratio_x), 0, (int)last_bmpWidth, (int)bmpHeight, SRCCOPY);
		}
		else
		{
			prtDC.StretchBlt(0, 0, (int)paper_width, (int)(bmpHeight * ratio_y), &memDC, (int)(paper_width * i / ratio_x), 0, (int)(paper_width / ratio_x), (int)bmpHeight, SRCCOPY);
		}
		prtDC.TextOut(page_info_rect.left, page_info_rect.top, strPageNumber);
		prtDC.EndPage(); 
	}
	memDC.SelectObject(pOldBitmap);
	delete bitmap;
	memDC.DeleteDC();

	prtDC.EndDoc();
	prtDC.Detach();
	prtDC.DeleteDC();

	return true;
}

//输出到excel表格。str表示excel中每列的字段名，array_c是传递记录中的详细内容
bool CPrint::outFile(CString str, CPMCEvent array_c[])
{
	//计算count的值，count为除序号外的列数，此处计算str中逗号的个数恰好等于count的值，作为循环的边界
	int count = 0;
	for (int s = 0; s < (int)strlen(str) ; s++)
	{
		if(str[s] == ',')
			count++;
	}
	//
	CFileDialog dlg(FALSE, "csv", "", OFN_OVERWRITEPROMPT, "csv Files(*.csv)|");
	if (dlg.DoModal() == IDOK)
	{     
		CString strFile = dlg.GetPathName();
		FILE *pFile = fopen(strFile, "w+");
		if(pFile)
		{
			fprintf(pFile, "%s", str);
			long id = 0;
			while(array_c[id].valid && id < 1024)
			{
				CArgs args;
				parse_arg_ey(array_c[id].data, &args, ";");
				//输出序号id
				id++;
				str.Format("%3d,", id);
				fprintf(pFile, "%s", str);
				//输出字段每列的内容
				for (int i = 0; i < count; i++)
				{				
					str.Format("%s,", args.argv[i]);
					fprintf(pFile, "%s", str);
				}
				//用来输出一个\n换行
				fprintf(pFile, "\n", str);
			}
			fclose(pFile);
		}
	}
	return true;
}
void CPrint::printView(CString strFileName)
{
	CDC memDC, dc;
	//CClientDC dc(this);
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//获取屏幕的句柄

	//取屏幕分辨率,即位图的长和宽
	int bmpWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int bmpHeight = ::GetSystemMetrics(SM_CYSCREEN);
	memDC.CreateCompatibleDC(&dc);
	CBitmap* bitmap = new CBitmap();

	//转换FileName的类型，由CString变为LPCTSTR
	USES_CONVERSION;
	LPCTSTR lpszBmpFile = A2CT((LPCSTR)strFileName);

	//加载要打印的位图
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, lpszBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	bitmap->Attach(hBitmap);
	BITMAP bm;
    bitmap->GetBitmap(&bm);
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(bitmap);

    /////
	CPrintDialog dlg(FALSE);
	if(IDOK!=dlg.DoModal()) return;;
	//dlg.GetDefaults();

	// is a default printer set up?
	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL)
	{
		AfxMessageBox("无法找到缺省打印机", NULL,MB_OK | MB_ICONSTOP);
	}
	else
	{
		// create a CDC and attach it to the default printer
		CDC dcPrinter;
		dcPrinter.Attach(hdcPrinter);

		// call StartDoc() to begin printing
		DOCINFO docinfo;
	    memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("操作员画面");

		//mox+++

		CPrintInfo Info;
		Info.m_rectDraw.SetRect(0,0,dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES));
		Info.SetMaxPage (1);
		///

		// if it fails, complain and exit gracefully
		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			AfxMessageBox("无法初始化打印机",NULL,MB_OK|MB_ICONSTOP);
		}
		else
		{
			// start a page
			if (dcPrinter.StartPage() < 0)
			{
				AfxMessageBox("无法初始化页",NULL,MB_OK|MB_ICONSTOP);
				dcPrinter.AbortDoc();
			}
			else
			{
				// actually do some printing
				dcPrinter.SetMapMode(MM_TEXT);
				dcPrinter.StretchBlt(0, 0, dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				dcPrinter.EndPage();
				memDC.SelectObject(pOldBitmap);
				delete bitmap;
				memDC.DeleteDC();
				dcPrinter.EndDoc(); 
			}
		}
		dcPrinter.Detach();
		dcPrinter.DeleteDC();
	}
}

//void CPrint::OnPrint(CDC* pdc, CPrintInfo* lParam, CString strFileName1)
//{
//	CDC* pDC   = pdc;
//	CPrintInfo* pInfo = (CPrintInfo *)lParam;
//
//	CFont DataFont;
//	DataFont.CreatePointFont(120,"宋体",pDC);
//
//	CString strFileName=strFileName1;
//
//	BSTR   bstr = strFileName.AllocSysString();
//	Bitmap* pPicture = Bitmap(bstr);
//	pPicture->RotateFlip(Gdiplus::Rotate90FlipNone);
//	HBITMAP hBitmap;//创建bmp的句柄
//	pPicture->GetHBITMAP(NULL,&hBitmap);//获取句柄
//	BITMAP bitmap;
//	::GetObject(hBitmap,sizeof(BITMAP),&bitmap);
//	double dScale=(double)pInfo->m_rectDraw.Width()/bitmap.bmWidth;
//	int nScaledWidth=m_cxWidth;
//	int nScaledHeight=(int)(bitmap.bmHeight*dScale);
//
//	HDC dcMem;
//	dcMem=::CreateCompatibleDC(pDC->m_hDC);
//	HBITMAP hOldBmp=(HBITMAP)::SelectObject(dcMem,hBitmap);
//
//	CRect r = pInfo->m_rectDraw;
//	//SizeToPlace(STP_FIX, r, CRect(0,0,bitmap.bmWidth, bitmap.bmHeight));//这个函数没有给出,可以自己写一下,注释掉也行,功能就是调整尺寸的
//	int nVertCenterPos = pDC->GetDeviceCaps (VERTRES) / 2;
//	::StretchBlt(pDC->m_hDC, r.left, r.top, r.Width(), r.Height(),
//		dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
//
//
//	::SelectObject(dcMem,hOldBmp);
//	::DeleteDC(dcMem);
//	::DeleteObject(hBitmap);
//	delete pPicture;
//	pPicture=NULL;
//}

/*
HDC hDC,                  打印机dc
HBITMAP hBmp,         待打印位图
int iX,                          位图的左上角点x坐标
int iY,                          位图的左上角点y坐标
double dScaleX,         打印机分辨率和屏幕分辨率x方向的比值，在没有指定位图显示高度和宽度的时候用的上
double dScaleY,         打印机分辨率和屏幕分辨率y方向的比值，在没有指定位图显示高度和宽度的时候用的上
int iWidth=0,         位图的显示宽度
int iLength=0         位图的显示高度
*/
//void CPrint::OnPrint(HDC hDC,HBITMAP hBmp,int iX,int iY,double dScaleX=1.0,double dScaleY=1.0,int iWidth=0,int iLength=0)
//{
//	HPALETTE hPal;
//	BITMAP                     bm;
//	BITMAPINFOHEADER         bi;
//	LPBITMAPINFOHEADER          lpbi;
//	DWORD                      dwLen;
//	HANDLE                     hDIB;
//	HANDLE                     handle;
//	HDC                            hDC1;
//	if(GetDeviceCaps(hDC,RASTERCAPS) & RC_PALETTE )
//	{
//		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
//		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
//		pLP->palVersion = 0x300;
//		pLP->palNumEntries =GetSystemPaletteEntries( hDC, 0, 255, pLP->palPalEntry );
//		hPal=CreatePalette(pLP );
//		delete[] pLP;
//	}
//	if (hPal==NULL)         hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
//	::GetObject(hBmp,sizeof(bm),(LPSTR)&bm);
//	bi.biSize             = sizeof(BITMAPINFOHEADER);
//	bi.biWidth                   = bm.bmWidth;
//	bi.biHeight                    = bm.bmHeight;
//	bi.biPlanes                    = 1;
//	bi.biBitCount                 = bm.bmPlanes * bm.bmBitsPixel;
//	bi.biCompression         = BI_RGB;
//	bi.biSizeImage              = 0;
//	bi.biXPelsPerMeter       = 0;
//	bi.biYPelsPerMeter       = 0;
//	bi.biClrUsed                   = 0;
//	bi.biClrImportant = 0;
//	int nColors = (1 << bi.biBitCount);
//	if( nColors > 256 )
//		nColors = 0;
//	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);
//	hDC1 = ::GetDC(NULL);
//	hPal = SelectPalette(hDC1,hPal,FALSE);
//	RealizePalette(hDC1);
//	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);
//	if (!hDIB)
//	{
//		SelectPalette(hDC1,hPal,FALSE);
//		::ReleaseDC(NULL,hDC1);
//		DeleteObject(hPal);
//		return ;
//	}
//	lpbi = (LPBITMAPINFOHEADER)hDIB;
//	*lpbi = bi;
//	::GetDIBits(hDC1, hBmp, 0L, (DWORD)bi.biHeight,
//		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);
//	bi = *lpbi;
//	if (bi.biSizeImage == 0)
//		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)* bi.biHeight;
//	dwLen += bi.biSizeImage;
//	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
//		hDIB = handle;
//	else
//	{
//		GlobalFree(hDIB);
//		SelectPalette(hDC1,hPal,FALSE);
//		::ReleaseDC(NULL,hDC1);
//		DeleteObject(hPal);
//		return ;
//	}
//	lpbi = (LPBITMAPINFOHEADER)hDIB;
//	BOOL bGotBits = GetDIBits( hDC1, hBmp,0L,(DWORD)bi.biHeight,(LPBYTE)lpbi+ (bi.biSize + nColors * sizeof(RGBQUAD)),
//		(LPBITMAPINFO)lpbi,(DWORD)DIB_RGB_COLORS);
//	if( !bGotBits )
//	{
//		GlobalFree(hDIB);
//		SelectPalette(hDC1,hPal,FALSE);
//		::ReleaseDC(NULL,hDC1);
//		DeleteObject(hPal);
//		return;
//	}
//	if(iWidth==0||iLength==0)
//	{
//		iWidth=lpbi->biWidth;
//		iLength=lpbi->biHeight;
//		iWidth=(int)(dScaleX*iWidth);
//		iLength=(int)(iLength*dScaleY);
//	}
//	StretchDIBits(hDC,iX,iY,iWidth,iLength,0,0,lpbi->biWidth,lpbi->biHeight,(LPBYTE)lpbi                     // address for bitmap bits
//		+ (bi.biSize + nColors * sizeof(RGBQUAD)),(LPBITMAPINFO)lpbi,DIB_RGB_COLORS,SRCCOPY);
//	SelectPalette(hDC1,hPal,FALSE);
//	::ReleaseDC(NULL,hDC1);
//	DeleteObject(hDIB);
//	DeleteObject(hPal);
//}