// Memory_Guard_page.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>

int _tmain()
{
	LPVOID lpBaseAddress;
	DWORD dwPageSize;
	BOOL bLocked;
	SYSTEM_INFO sSysInfo;
	GetSystemInfo(&sSysInfo);
	dwPageSize = sSysInfo.dwPageSize;
	lpBaseAddress = VirtualAlloc(NULL, dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READONLY | PAGE_GUARD);
	if (lpBaseAddress == NULL)
	{
		printf("内存分配发生错误%d\n", GetLastError());
		return 0;
	}
	else
	{
		printf("提交%lu 字节在地址 0x%lp\n", dwPageSize, lpBaseAddress);
	}
	bLocked = VirtualLock(lpBaseAddress, dwPageSize);
	if (!bLocked)
	{
		printf("第一次尝试失败%p, 错误码0x%lx\n", lpBaseAddress, GetLastError());
	}
	else
	{
		printf("第一次尝试成功%lp\n", lpBaseAddress);
	}
	bLocked = VirtualLock(lpBaseAddress, dwPageSize);
	if (!bLocked)
	{
		printf("第二次尝试失败%p, 错误码0x%lx\n", lpBaseAddress, GetLastError());
	}
	else
	{
		_ftprintf(stderr, L"第二次尝试成功%lp\n", lpBaseAddress);
	}
    std::cout << "Hello World!\n";
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
