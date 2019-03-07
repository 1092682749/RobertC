
#include "pch.h"
#include <Windows.h>
#include <iostream>

// 线程函数:函数要去文件映射对象里去读取数据
void ThreadFun(LPVOID lpParam)
{

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
	return 0;
}
