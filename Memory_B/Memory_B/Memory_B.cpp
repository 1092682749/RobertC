// Memory_B.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "UI.h"

enum OPTION_TYPE
{
	OPEN = 1,
	READ,
	WRITE,
	UNMAP,
	EXIT,
};

TCHAR SHARE_SPACE_NAME[] = L"Share_Memory";

int main()
{
	HANDLE hFileMapping = NULL;
	LPVOID baseAddress = NULL;
	char outBuf[1024] = { 0 };
	int option = 0;
	while (true)
	{
		print_UI();
		std::cin >> option;
		switch (option)
		{
		case OPEN:
			hFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SHARE_SPACE_NAME);
			if (hFileMapping == NULL)
			{
				std::cout << "打开共享区域失败,请检查是否已经创建该区域\n";
				break;
			}
			baseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			if (baseAddress == NULL)
			{
				std::cout << "创建映射失败\n";
				break;
			}
			std::cout << "创建内存映射成功\n";
			break;
		case WRITE:
			std::cout << "请输入内容:";
			std::cin >> outBuf;
			CopyMemory(baseAddress, outBuf, strlen(outBuf));
			break;
		case READ:
			printf("%s\n", baseAddress);
			break;
		case UNMAP:
			if (baseAddress != NULL)
			{
				UnmapViewOfFile(baseAddress);
				baseAddress = NULL;
			}
			if (hFileMapping != NULL)
			{
				CloseHandle(hFileMapping);
				hFileMapping = NULL;
			}
			break;
		case EXIT:
			if (baseAddress != NULL)
			{
				UnmapViewOfFile(baseAddress);
				baseAddress = NULL;
			}
			if (hFileMapping != NULL)
			{
				CloseHandle(hFileMapping);
				hFileMapping = NULL;
			}
			return 0;
		default:
			std::cout << "无效输入\n";
			break;
		}

	}

	return 0;
}

