// Process_And_Thread_Creat_Process.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>

#include <iostream>

int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WCHAR charArr[] = L"C:/Users/i-robert/source/repos/Share_Memory_A/Debug/Share_Memory_A.exe";
	LPWSTR name = charArr;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	BOOL is = CreateProcess(NULL, // 要启动的子进程的名字
		name, // 命令函参数，由于名字参数为NULL因此该参数的第一个空格符前的字符串会作为子进程的名字。
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	if (!is)
	{
		printf("创建失败: %d\n", GetLastError());
		return -1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}


