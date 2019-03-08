
#include "pch.h"
#include <Windows.h>
#include <iostream>

#define THREADCOUNT 4

DWORD THREADIDS[THREADCOUNT];
HANDLE THREADS[THREADCOUNT];

// 线程函数:函数要去文件映射对象里去读取数据
void ThreadFun(LPVOID lpParam)
{
	HANDLE thFm;
	LPVOID address;
	DWORD dwWaitResult;
	std::cout << "Thread run...\n";
	HANDLE *phEvent = (HANDLE*)lpParam;
	dwWaitResult = WaitForSingleObject(*phEvent, INFINITE);
	std::cout << "Compelet wait...\n";
	switch (dwWaitResult)
	{
	case WAIT_OBJECT_0:
		std::cout << "Wait result is: WAIT_OBJECT_0\n";
		thFm = OpenFileMapping(FILE_MAP_ALL_ACCESS,
			TRUE,
			L"File_a_txt");
		if (thFm == INVALID_HANDLE_VALUE)
		{
			std::cout << "OpenFileMapping faild\n";
			return;
		}
		address = MapViewOfFile(thFm, FILE_MAP_READ, 0, 0, 0);
		std::cout << (LPSTR)address << "\n";
		CloseHandle(thFm);
		break;
	default:
		std::cout << "Wait faild\n";
		break;
	}
}
/*
	线程创建早于filemapping如果同步失败，线程将无法OpenFileMapping
*/
int main()
{
	// 创建事件对象并初始为无信号
	HANDLE hEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		L"WriteEvent"
	);
	for (int i = 0; i < THREADCOUNT; i++)
	{
		THREADS[i] = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)ThreadFun,
			&hEvent,
			0,
			&THREADIDS[i]
		);
	}
	// 打开指定文件
	HANDLE hFile = CreateFile(L"C:/Users/i-robert/Desktop/a.txt",
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	// 一秒后更改文件内容
	Sleep(1000);
	SetFilePointer(hFile, 0, NULL, FILE_END);
	char outBuf[] = "\r\nMain Thread Write Data\r\n";
	WriteFile(hFile, outBuf, sizeof(outBuf), NULL, NULL);
	FlushFileBuffers(hFile);
	// 为打开的文件创建file-mapping-object
	HANDLE hFileMapping = CreateFileMapping(hFile,	// 文件句柄
		NULL,										// 文件安全属性，指向一个SECURITY_ATTRIBUTES结构体
		PAGE_EXECUTE_READWRITE,						// 访问权限
		0,											// 对象大小（高位）
		0,											// 对象大小（低位）
		L"File_a_txt");								// 对象命名
	LPVOID fileAddress = MapViewOfFile(
		hFileMapping,								// 文件映射对象的句柄
		FILE_MAP_ALL_ACCESS,						// 访问权限
		0,											// 偏移量（高位）
		0,											// 偏移量（低位）
		0);											// 映射到视图的字节数
	
	
	std::cout << "Change event state\n";
	if (!SetEvent(hEvent))
	{
		std::cout << "SetEvent faild!";
		return -2;
	}
	WaitForMultipleObjects(THREADCOUNT, THREADS, TRUE, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hFile);
	CloseHandle(hFileMapping);
	return 0;
}
