#pragma once
#include <windows.h>
#include <assert.h>

class CDrawUtil
{
public:

	static char* Bitmap2Bytes(HBITMAP hBitmap, int& len)
	{
		BITMAP bmpObj;
		HDC hDCScreen;
		int iRet;
		DWORD dwBmpSize;

		BITMAPFILEHEADER	bmfHeader;
		LPBITMAPINFO		lpbi;
		const DWORD dwSizeOfBmfHeader = sizeof(BITMAPFILEHEADER);
		DWORD dwSizeOfBmInfo = sizeof(BITMAPINFO);
		
		hDCScreen = GetDC(NULL);
		GetObject(hBitmap, sizeof(BITMAP), &bmpObj);
	
		HGLOBAL hDIB = GlobalAlloc(GHND, dwSizeOfBmInfo + 8); 
		lpbi = (LPBITMAPINFO)GlobalLock(hDIB);
		lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbi
		iRet = GetDIBits(hDCScreen, hBitmap, 0,
			(UINT)bmpObj.bmHeight, 
			NULL,
			lpbi, DIB_RGB_COLORS);
		ASSERT(iRet > 0);

		// only 16 and 32 bit images are supported.
		ASSERT(lpbi->bmiHeader.biBitCount == 16 || lpbi->bmiHeader.biBitCount == 32);
		if(lpbi->bmiHeader.biCompression == BI_BITFIELDS)
			dwSizeOfBmInfo += 8;

		dwBmpSize = lpbi->bmiHeader.biSizeImage;
		HGLOBAL hDIBBmData = GlobalAlloc(GHND, dwBmpSize); 
		char* lpbitmap = (char*)GlobalLock(hDIBBmData);

		iRet = GetDIBits(hDCScreen, hBitmap, 0,
			(UINT)bmpObj.bmHeight, 
			lpbitmap,
			lpbi, DIB_RGB_COLORS);    
		ASSERT(iRet > 0);

		DWORD dwSizeofDIB   =    dwBmpSize + dwSizeOfBmfHeader + dwSizeOfBmInfo;
		bmfHeader.bfOffBits = (DWORD)dwSizeOfBmfHeader + (DWORD)dwSizeOfBmInfo; 
		bmfHeader.bfSize = dwSizeofDIB; 
		bmfHeader.bfType = 0x4D42; //BM   
		bmfHeader.bfReserved1 = bmfHeader.bfReserved2 = 0;

		char* arrData = new char[dwSizeofDIB];
		memcpy(arrData, &bmfHeader, dwSizeOfBmfHeader);
		memcpy(arrData + dwSizeOfBmfHeader, lpbi, dwSizeOfBmInfo);
		memcpy(arrData + dwSizeOfBmfHeader + dwSizeOfBmInfo, lpbitmap, dwBmpSize);

		GlobalUnlock(hDIB);    
		GlobalUnlock(hDIBBmData);
		GlobalFree(hDIB);
		GlobalFree(hDIBBmData);
		ReleaseDC(NULL, hDCScreen);

		len = dwSizeofDIB;
		return arrData;
	}

	static HBITMAP Bytes2Bitmap(char arrData[], int iLen)
	{
		PBITMAPFILEHEADER	bmfHeader;
		PBITMAPINFO	pbi;
		HDC			hDC;
		HBITMAP		hBmpRet;
		int			iRet;
		int			iSizeOfBmInfo;
		const int	iSizeOfBmfHeader = sizeof(BITMAPFILEHEADER);
		
		// get the BITMAPFILEHEADER
		bmfHeader = (PBITMAPFILEHEADER)arrData;
		arrData += iSizeOfBmfHeader;

		// get the BITMAPINFO
		iSizeOfBmInfo = bmfHeader->bfOffBits - iSizeOfBmfHeader;
		pbi = (PBITMAPINFO)arrData;
		arrData += iSizeOfBmInfo;
		
		ASSERT(pbi->bmiHeader.biSizeImage == iLen - iSizeOfBmfHeader - iSizeOfBmInfo);

		// create the output DDB bitmap
		hDC = GetDC(NULL);
		hBmpRet = CreateCompatibleBitmap(hDC, 
			pbi->bmiHeader.biWidth, pbi->bmiHeader.biHeight);
		ASSERT(hBmpRet);

		// set the image...
		iRet = SetDIBits(hDC, hBmpRet, 0,
			pbi->bmiHeader.biHeight,
			arrData,
			pbi, DIB_RGB_COLORS);
		ASSERT(iRet > 0);

		::ReleaseDC(NULL, hDC);

		return hBmpRet;
	}
};