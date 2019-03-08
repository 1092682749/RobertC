
#include "pch.h"
#include <Windows.h>
#include <iostream>

#define THREADCOUNT 4

DWORD THREADIDS[THREADCOUNT];
HANDLE THREADS[THREADCOUNT];

// 线程函数:函数要去文件映射对象里去读取数据
void ThreadFun(LPVOID lpParam)
{
	HANDLE tFm;
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
		tFm = OpenFileMapping(FILE_MAP_ALL_ACCESS,
			TRUE,
			L"File_a_txt");
		address = MapViewOfFile(tFm, FILE_MAP_READ, 0, 0, 0);
		std::cout << (LPSTR)address << "\n";
		CloseHandle(tFm);
		break;
	default:
		std::cout << "Wait faild\n";
		break;
	}
	

}
int main()
{
	// 打开指定文件
	HANDLE hFile = CreateFile(L"C:/Users/i-robert/Desktop/a.txt",
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	// 为打开的文件创建file-mapping-object
	HANDLE hFileMapping = CreateFileMapping(hFile,
		NULL,
		PAGE_EXECUTE_READWRITE,
		0,
		0,
		L"File_a_txt");
	LPVOID fileAddress = MapViewOfFile(
		hFileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
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
	Sleep(1000);
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
