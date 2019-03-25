// Memory_A.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "UI.h"

#define MAPSIZE 1024

TCHAR SHARE_SPACE_NAME[] = L"Share_Memory";

enum OPTION_TYPE
{
	SHARE_FILE = 1,
	SHARE_MEMORY,
	READ,
	WRITE,
	UNMAP,
	EXIT
};

int main()
{
	int option;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	HANDLE hFileMapping = NULL;
	LPVOID baseAddress = NULL;
	char fileName[200] = { 0 };
	char input[1024] = { 0 };
	TCHAR wFileName[1024] = { 0 };
	while (true)
	{
		print_UI();
		std::cin >> option;
		switch (option)
		{
		case SHARE_FILE:
			std::cout << "请输入文件名:";
			std::cin >> fileName;
			MultiByteToWideChar(CP_UTF8, 0, fileName, strlen(fileName), wFileName, 1024);
			hFile = CreateFile(wFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
			hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, SHARE_SPACE_NAME);
			if (hFileMapping == NULL)
			{
				std::cout << GetLastError() << "\n";
				return -2;
			}
			baseAddress = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
			if (baseAddress == NULL)
			{
				std::cout << GetLastError() << "\n";
				return -3;
			}
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return -1;
			}
			std::cout << "创建成功！\n";
			break;
		case SHARE_MEMORY:
			if (baseAddress != NULL)
			{
				std::cout << "请先取消对当前共享区域的占用\n";
				break;
			}
			hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAPSIZE, SHARE_SPACE_NAME);
			if (hFileMapping == NULL)
			{
				std::cout << "共享内存失败ERROR=" << GetLastError() << "\n";
				return -2;
			}
			baseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			if (baseAddress == NULL)
			{
				std::cout << "创建视图失败ERROR=" << GetLastError() << "\n";
				return -3;
			}
			std::cout << "创建共享区域成功\n";
			break;
		case READ:
			printf("%s\n", (char*)baseAddress);
			break;
		case WRITE:
		{
			char buf[MAPSIZE] = { 0 };
			std::cout << "请输入内容:";
			std::cin >> buf;
			CopyMemory(baseAddress, buf, strlen(buf));
			break;
		}
		case UNMAP:
			if (baseAddress != NULL)
			{
				UnmapViewOfFile(baseAddress);
			}
			if (hFileMapping != NULL)
			{
				CloseHandle(hFileMapping);
			}
			break;
		case EXIT:
			if (baseAddress != NULL)
			{
				UnmapViewOfFile(baseAddress);
			}
			if (hFileMapping != NULL)
			{
				CloseHandle(hFileMapping);
			}
			return 0;
		default:
			std::cout << "无效输入\n";
			break;
		}
	}





	/*HANDLE hFile = CreateFile(L"D:/a/to.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	HANDLE hFileMaping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, SHARE_SPACE_NAME);
	if (hFileMaping == NULL)
	{
		std::cout << GetLastError() << "\n";
		return -2;
	}
	LPVOID baseAddress = MapViewOfFile(hFileMaping, FILE_MAP_WRITE, 0, 0, 0);
	if (baseAddress == NULL)
	{
		std::cout << GetLastError() << "\n";
		return -3;
	}
	*/
	//std::cin >> input;
	//CopyMemory(baseAddress, input, strlen(input));
	//UnmapViewOfFile(baseAddress);
	//CloseHandle(hFileMaping);
	return 0;
}
