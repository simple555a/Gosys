/*
	从网上搜集的垃圾代码，唉，这帮人离开mfc就不会写程序啊
*/
#include "precomp.h"
#pragma hdrstop
#include <sys/stat.h>

// LoadBMP		- Loads a BMP file and creates a logical palette for it.
// Returns		- TRUE for success
// sBMPFile		- Full path of the BMP file
// phDIB		- Pointer to a HGLOBAL variable to hold the loaded bitmap
//			  Memory is allocated by this function but should be 
//			  released by the caller.
// pPal			- Will hold the logical palette
RTKGUIAPI BOOL WINAPI  RtkGuiLoadBMP(LPCTSTR sBMPFile, HGLOBAL *phDIB, LOGPALETTE * pLP)
{
	FILE * fp;
	BITMAPFILEHEADER bmfHeader;
	long nFileLen;
	HGLOBAL hDIB;
	struct _stat s;

	if( _stat(sBMPFile, &s) ){
		return 0;
	}
	
	fp = fopen(sBMPFile, "rb");
	if(!fp){
		return FALSE;
	}

	nFileLen = s.st_size;

	// Read file header
	if (fread(&bmfHeader, 1, sizeof(bmfHeader), fp) != sizeof(bmfHeader)){
		fclose(fp);
		return FALSE;
	}

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')){
		fclose(fp);
		return FALSE;
	}

	hDIB = ::GlobalAlloc(GMEM_FIXED, nFileLen);
	if (hDIB == 0){
		fclose(fp);
		return FALSE;
	}

	// Read the remainder of the bitmap file.
	if (fread(hDIB, 1, nFileLen - sizeof(BITMAPFILEHEADER), fp) !=
		nFileLen - sizeof(BITMAPFILEHEADER) 
	){
		::GlobalFree(hDIB);
		fclose(fp);
		return FALSE;
	}
	
	BITMAPINFO & bmInfo = *(LPBITMAPINFO)hDIB ;

	int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	// Create the palette
	if( nColors <= 256 && pLP){
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( int i=0; i < nColors; i++){
			pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		CreatePalette( pLP );
	}

	*phDIB = hDIB;

	fclose(fp);

	return TRUE;
}

RTKGUIAPI void WINAPI RtkGuiDrawDIB(HDC hdc, HGLOBAL hDIB, int x, int y)
{
	LPVOID	lpDIBBits;		// Pointer to DIB bits
	BOOL	bSuccess=FALSE; 	// Success/fail flag
	
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors +
			bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	::SetDIBitsToDevice(hdc,
				   x,					// DestX
				   y,					// DestY
				   bmInfo.bmiHeader.biWidth,		// nDestWidth
				   bmInfo.bmiHeader.biHeight,		// nDestHeight
				   0,					// SrcX
				   0,					// SrcY
				   0,					// nStartScan
				   bmInfo.bmiHeader.biHeight,		// nNumScans
				   lpDIBBits,				// lpBits
				   (LPBITMAPINFO)hDIB,			// lpBitsInfo
				   DIB_RGB_COLORS); 			// wUsage
}
