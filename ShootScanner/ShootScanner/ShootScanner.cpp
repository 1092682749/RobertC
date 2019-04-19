// ShootScanner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <strsafe.h>

void ShootScreen(const char* filename, HWND hWnd)
{
	HDC hdc = CreateDCA("DISPLAY", NULL, NULL, NULL);
	int32_t ScrWidth = 0, ScrHeight = 0;
	RECT rect = { 0 };
	if (hWnd == NULL)
	{
		ScrWidth = GetDeviceCaps(hdc, HORZRES);
		ScrHeight = GetDeviceCaps(hdc, VERTRES);
	}
	else
	{
		GetWindowRect(hWnd, &rect);
		ScrWidth = rect.right - rect.left;
		ScrHeight = rect.bottom - rect.top;
	}
	HDC hmdc = CreateCompatibleDC(hdc);

	HBITMAP hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight);
	HBITMAP holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen);

	BITMAP bm;
	GetObject(hBmpScreen, sizeof(bm), &bm);

	BITMAPINFOHEADER bi = { 0 };
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = bm.bmPlanes;
	bi.biBitCount = bm.bmBitsPixel;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = bm.bmHeight * bm.bmWidthBytes;
	// 图片的像素数据
	char *buf = new char[bi.biSizeImage];
	BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, rect.left, rect.top, SRCCOPY);
	GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWrite;
	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
	WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
	WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
	CloseHandle(hFile);
	hBmpScreen = (HBITMAP)SelectObject(hmdc, holdbmp);
}
BOOL GetDisplayMonitorInfo(int nDeviceIndex, LPSTR lpszMonitorInfo)
{
	FARPROC EnumDisplayDevices;
	HINSTANCE  hInstUser32;
	DISPLAY_DEVICE DispDev;
	char szSaveDeviceName[33];  // 32 + 1 for the null-terminator 
	BOOL bRet = TRUE;
	HRESULT hr;

	hInstUser32 = LoadLibraryA("c:/windows/User32.DLL");
	if (!hInstUser32) return FALSE;

	// Get the address of the EnumDisplayDevices function 
	EnumDisplayDevices = (FARPROC)GetProcAddress(hInstUser32, "EnumDisplayDevicesA");
	if (!EnumDisplayDevices) {
		FreeLibrary(hInstUser32);
		return FALSE;
	}

	ZeroMemory(&DispDev, sizeof(DispDev));
	DispDev.cb = sizeof(DispDev);

	// After the first call to EnumDisplayDevices,  
	// DispDev.DeviceString is the adapter name 
	if (EnumDisplayDevicesA(NULL, nDeviceIndex, (PDISPLAY_DEVICEA)&DispDev, 0))
	{
		WCHAR wszSaveDeviceName[33];
		MultiByteToWideChar(CP_UTF8, 0, szSaveDeviceName, sizeof(szSaveDeviceName), wszSaveDeviceName, 33);
		hr = StringCchCopy(wszSaveDeviceName, 33, DispDev.DeviceName);
		if (FAILED(hr))
		{
			// TODO: write error handler 
		}

		// After second call, DispDev.DeviceString is the  
		// monitor name for that device  
		EnumDisplayDevicesA(szSaveDeviceName, 0, (PDISPLAY_DEVICEA)&DispDev, 0);

		// In the following, lpszMonitorInfo must be 128 + 1 for  
		// the null-terminator. 
		hr = StringCchCopy(wszSaveDeviceName, 129, DispDev.DeviceString);
		if (FAILED(hr))
		{
			// TODO: write error handler 
		}

	}
	else {
		bRet = FALSE;
	}

	FreeLibrary(hInstUser32);

	return bRet;
}
int main()
{
	ShootScreen("D:/a/ss.png", NULL);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
