// Decompression_File.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <lzexpand.h>
#include <tchar.h>

#pragma comment(lib, "Lz32.lib")

// 解压暂时无效

int main()
{
	OFSTRUCT ofs, ofsNewFile;
	TCHAR fileName[] = L"D:/a/jsjczxtdsb.zip", newFileName[] = L"D:/a/jsjdir";
	INT hFile = LZOpenFile(fileName, &ofs, OF_READ);
	if (hFile < 0)
	{
		_tprintf(L"LZOpenFile faild!\n");
		return -1;
	}
	INT hnFile = LZOpenFile(newFileName, &ofsNewFile, OF_CREATE);
	if (hnFile < 0)
	{
		_tprintf(L"LZOpenFile faild!\n");
		return -2;
	}
	//HANDLE hDesFile = CreateFile(L"D:/a/myUnZip", GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LONG lRes = LZCopy(hFile, hnFile);
	if (lRes < 0)
	{
		_tprintf(L"LZCopyFile faild\n");
		return -3;
	}
	_tprintf(L"Copy success!");
	return 0;
}
