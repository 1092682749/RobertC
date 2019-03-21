// Memory_B.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

TCHAR SHARE_SPACE_NAME[] = L"Share_Memory";

int main()
{
	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SHARE_SPACE_NAME);
	if (hFileMapping == NULL)
	{
		return -1;
	}
	LPVOID baseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (baseAddress == NULL)
	{
		return -2;
	}
	char outBuf[1024] = { 0 };
	CopyMemory(outBuf, baseAddress, 1024);
	printf("%s\n", outBuf);
	return 0;
}

