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

// �ı���ӡ
//docName��ʾҪ��ӡ�ļ������֡���ӡarray_a�е����ݣ�array_b������־�ֶε����ƣ���ͬ����־�ֶβ�ͬ��
//style�Ǵ�ӡ�ķ�ʽ����ӡ��ɫ��ӡ��,Count��array_b��Ԫ�صĸ����������޶�ѭ���Ĵ���
bool CPrint::printText(CString docName, CPMCEvent array_a[], CString array_b[], bool style, int Count)
{

	CPrintDialog dlg(FALSE);//�����ΪTRUE������ʵ�ִ�ӡ���ܣ����ǽ�����ӡ��
	if (IDOK != dlg.DoModal())
	{
		return false;
	}

	HDC hdcPtrinter = dlg.GetPrinterDC();//��ȡ��ӡ����DC
	if (NULL == hdcPtrinter)
	{
		AfxMessageBox("�޷��ҵ���ӡ��", NULL, MB_OK | MB_ICONSTOP);
		return false;
	}
	else
	{
		CDC dcPrinter;
		dcPrinter.Attach(hdcPtrinter);
		//֪ͨdoc��ʼ��ӡ
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = docName;

		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			AfxMessageBox("�޷���ʼ����ӡ��", NULL, MB_OK | MB_ICONSTOP);
			return false;
		}
		else
		{
			dcPrinter.SetMapMode(MM_TWIPS);//Each logical unit is converted to 1/20 of a point.a twip is 1/1440 inch
			CFont *pOldFont;
			CFont fnt;
			if (true == style)//tureΪӲ������falseΪ��ɫ����
			{
				//��������
				fnt.CreatePointFont(70, "����", &dcPrinter);
				//����������ɫ
				pOldFont = (CFont*)dcPrinter.SelectObject(&fnt);

			}
			else
			{   
				//��ӡ��Ĭ������
				pOldFont = (CFont*)dcPrinter.SelectStockObject(DEVICE_DEFAULT_FONT);
			}

			CPoint pt(dcPrinter.GetDeviceCaps(HORZRES), dcPrinter.GetDeviceCaps(VERTRES));
			dcPrinter.DPtoLP(&pt);//Converts device units into logical units.
			pt.y = -pt.y;

			//��ÿһ�еĿ�ȸ�ֵ
			vector<CSize> width;
			for(int i = 0; i < Count; i++)
			{
				width.push_back(dcPrinter.GetTextExtent(array_b[i]));    
			}

			//
			double lineHeight = width[0].cy * 1.5;//����ÿһ�еĸ߶�
			double lineCount = (pt.y - width[0].cy * 12) / lineHeight;//ÿҳ��ӡ������
			if(lineCount < 0)
				return false;
			long id = 0;
			CString str;
			int k = 0, iPage = 0;//ҳ��
			double leftWidth, cLeftGaP = 800;//cLeftGaP������յľ��룬�ǲ���ġ�leftWidth��Ҫ��ӡ���ֶ���ֽ������˵ľ����Ǳ仯��
			while(array_a[id].valid && id < 1024)//ѭ��һ���Ǵ�ӡһҳ
			{
				//start a page����ӡһҳ
				if (dcPrinter.StartPage() < 0)
				{
					AfxMessageBox("�޷���ʼ��ҳ", NULL, MB_OK | MB_ICONSTOP);
					dcPrinter.AbortDoc();
					return false;
				}
				else
				{
					iPage++;
					str.Format("��%2dҳ", iPage);
					dcPrinter.TextOut((int)cLeftGaP, (int)-lineHeight * 3, str);
					//��ӡ��һ�еı�ͷ
					leftWidth = cLeftGaP;//��һ�е����ݵ�����ߵľ��룬����������յĴ�С
					for (int j = 0; j < Count; j++)
					{
						dcPrinter.TextOut((int)leftWidth, (int)-lineHeight * 5, array_b[j]);
						leftWidth += width[j].cx;
					}
					//��ӡ����
					leftWidth = cLeftGaP;
					for(int m = 0; m < lineCount; m++)//һ��ѭ��Ϊһ��
					{
						if (array_a[id].valid)
						{
							CArgs args;
							parse_arg_ey(array_a[id].data, &args, ";"); 
							str.Format("%3d", id);
							dcPrinter.TextOut((int)leftWidth, (int)-lineHeight * (6 + m), str);
							for (int k = 0; k < Count-1; k++)
							{
								leftWidth += width[k].cx;//�ӵڶ��еĿ�ȿ�ʼ
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
				dcPrinter.EndPage();//��ҳ����
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
//λͼ��ӡ,FileName��Ҫ��ӡ���ļ���·�����ļ���
bool CPrint::printBitmap(CString FileName)
{
	CDC memDC, dc;
	//CClientDC dc(this);
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//��ȡ��Ļ�ľ��

	//ȡ��Ļ�ֱ���,��λͼ�ĳ��Ϳ�
	int bmpWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int bmpHeight = ::GetSystemMetrics(SM_CYSCREEN);
	memDC.CreateCompatibleDC(&dc);
	CBitmap* bitmap = new CBitmap();

	//ת��FileName�����ͣ���CString��ΪLPCTSTR
	USES_CONVERSION;
	LPCTSTR lpszBmpFile = A2CT((LPCSTR)FileName);

	//����Ҫ��ӡ��λͼ
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
	//��ӡ
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
			hDC = printInfo.m_pPD->CreatePrinterDC();//������ӡ��
		}
		if (hDC != NULL)//�����ɹ�
		{
			prtDC.Attach(hDC);
			paper_size.cx = prtDC.GetDeviceCaps(HORZSIZE);//210��ֽ�ŵĿ��
			paper_size.cy = prtDC.GetDeviceCaps(VERTSIZE);//297��ֽ�ŵĸ߶�
			xLongPPI = prtDC.GetDeviceCaps(LOGPIXELSX);//600,ֽ�ſ��ÿ�߼�Ӣ���������
			yLongPPI = prtDC.GetDeviceCaps(LOGPIXELSY);//600��ֽ�ſ��ÿ�߼�Ӣ���������
		}
		else
		{
			AfxMessageBox("�޷��ҵ���ӡ��");
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
		AfxMessageBox("�޷��ҵ���ӡ��");
		prtDC.Detach();
		prtDC.DeleteDC();
		return false;
	}

	prtDC.SetMapMode(MM_TEXT);//Sets the mapping mode.

	for (int i = 0; i < total_pages; i++)
	{
		prtDC.StartPage();
		strPageNumber.Format("��%dҳ����%dҳ", ++printed_pages, total_pages);

		if ( i == (total_pages - 1) && total_pages > 1)//���һҳ
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

//�����excel���str��ʾexcel��ÿ�е��ֶ�����array_c�Ǵ��ݼ�¼�е���ϸ����
bool CPrint::outFile(CString str, CPMCEvent array_c[])
{
	//����count��ֵ��countΪ���������������˴�����str�ж��ŵĸ���ǡ�õ���count��ֵ����Ϊѭ���ı߽�
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
				//������id
				id++;
				str.Format("%3d,", id);
				fprintf(pFile, "%s", str);
				//����ֶ�ÿ�е�����
				for (int i = 0; i < count; i++)
				{				
					str.Format("%s,", args.argv[i]);
					fprintf(pFile, "%s", str);
				}
				//�������һ��\n����
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
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//��ȡ��Ļ�ľ��

	//ȡ��Ļ�ֱ���,��λͼ�ĳ��Ϳ�
	int bmpWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int bmpHeight = ::GetSystemMetrics(SM_CYSCREEN);
	memDC.CreateCompatibleDC(&dc);
	CBitmap* bitmap = new CBitmap();

	//ת��FileName�����ͣ���CString��ΪLPCTSTR
	USES_CONVERSION;
	LPCTSTR lpszBmpFile = A2CT((LPCSTR)strFileName);

	//����Ҫ��ӡ��λͼ
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
		AfxMessageBox("�޷��ҵ�ȱʡ��ӡ��", NULL,MB_OK | MB_ICONSTOP);
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
		docinfo.lpszDocName = _T("����Ա����");

		//mox+++

		CPrintInfo Info;
		Info.m_rectDraw.SetRect(0,0,dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES));
		Info.SetMaxPage (1);
		///

		// if it fails, complain and exit gracefully
		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			AfxMessageBox("�޷���ʼ����ӡ��",NULL,MB_OK|MB_ICONSTOP);
		}
		else
		{
			// start a page
			if (dcPrinter.StartPage() < 0)
			{
				AfxMessageBox("�޷���ʼ��ҳ",NULL,MB_OK|MB_ICONSTOP);
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
//	DataFont.CreatePointFont(120,"����",pDC);
//
//	CString strFileName=strFileName1;
//
//	BSTR   bstr = strFileName.AllocSysString();
//	Bitmap* pPicture = Bitmap(bstr);
//	pPicture->RotateFlip(Gdiplus::Rotate90FlipNone);
//	HBITMAP hBitmap;//����bmp�ľ��
//	pPicture->GetHBITMAP(NULL,&hBitmap);//��ȡ���
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
//	//SizeToPlace(STP_FIX, r, CRect(0,0,bitmap.bmWidth, bitmap.bmHeight));//�������û�и���,�����Լ�дһ��,ע�͵�Ҳ��,���ܾ��ǵ����ߴ��
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
HDC hDC,                  ��ӡ��dc
HBITMAP hBmp,         ����ӡλͼ
int iX,                          λͼ�����Ͻǵ�x����
int iY,                          λͼ�����Ͻǵ�y����
double dScaleX,         ��ӡ���ֱ��ʺ���Ļ�ֱ���x����ı�ֵ����û��ָ��λͼ��ʾ�߶ȺͿ�ȵ�ʱ���õ���
double dScaleY,         ��ӡ���ֱ��ʺ���Ļ�ֱ���y����ı�ֵ����û��ָ��λͼ��ʾ�߶ȺͿ�ȵ�ʱ���õ���
int iWidth=0,         λͼ����ʾ���
int iLength=0         λͼ����ʾ�߶�
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