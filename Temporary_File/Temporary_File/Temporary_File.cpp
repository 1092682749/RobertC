// Temporary_File.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

#define BUFFSIZE 1024

int main()
{
	TCHAR wPath[MAX_PATH];
	LPWSTR fullName = new TCHAR[MAX_PATH];
	TCHAR buf[BUFFSIZE];
	DWORD dwAlreadySize = 0;
	BOOL bRes;
	DWORD pathLength = GetTempPath(MAX_PATH, wPath);
	// 创建临时文件并打开
	if (pathLength > MAX_PATH || (pathLength < 0))
	{
		return -1;
	}
	UINT uUnique = GetTempFileName(wPath, L"tempFile", 0, fullName);
	if (uUnique == 0)
	{
		return -2;
	}
	HANDLE hTempFile = CreateFile(fullName, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTempFile == INVALID_HANDLE_VALUE)
	{
		return -3;
	}
	// 读取本地文件
	HANDLE hLocalFile = CreateFile(L"D:/a/local.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	do
	{
		bRes = ReadFile(hLocalFile, buf, BUFFSIZE, &dwAlreadySize, NULL);
		if (!bRes)
		{
			std::cout << "Error = " << GetLastError() << "\n";
			return -4;
		}
		bRes = WriteFile(hTempFile, buf, dwAlreadySize, NULL, NULL);
		if (!bRes)
		{
			std::cout << "Error = " << GetLastError() << "\n";
			return -5;
		}
	} while (dwAlreadySize != 0);
	CloseHandle(hTempFile);
	CloseHandle(hLocalFile);
	// 将临时文件移动到新的文件里
	bRes = MoveFileEx(fullName, L"D:/a/moveex.txt", MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
	if (!bRes)
	{
		std::cout << "Error = " << GetLastError() << "\n";
		return -6;
	}
	return 0;
}
