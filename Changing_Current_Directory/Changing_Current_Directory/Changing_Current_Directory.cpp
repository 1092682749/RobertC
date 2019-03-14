// Changing_Current_Directory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <list>

#define BUFSIZE MAX_PATH


std::list<WIN32_FIND_DATA> ergodicFileList()
{
	std::list<WIN32_FIND_DATA> l;
	HANDLE hFirstFile;
	WIN32_FIND_DATA find_data;
	LARGE_INTEGER liFileSize;
	if ((hFirstFile = FindFirstFile(L"*", &find_data)) == INVALID_HANDLE_VALUE)
	{
		_tprintf(L"find faild\n");
	}
	do
	{
		WIN32_FIND_DATA fd = find_data;
		l.push_back(fd);
		if (find_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(L"this is a directory\n");
		}
		else
		{
			liFileSize.LowPart = find_data.nFileSizeLow;
			liFileSize.HighPart = find_data.nFileSizeHigh;
			_tprintf(L"%s %ld bytes\n", find_data.cFileName, liFileSize.QuadPart);
		} 
	} while (FindNextFile(hFirstFile, &find_data));
	return l;
}
typedef struct MyStruct
{
	int a;
}*PMS;
void test(PMS &s)
{
	s->a = 100;
}
void testN(MyStruct &s)
{
	s.a = 99;
}
void testP(PMS s)
{
	s->a = 1000;
}
int main()
{
	DWORD size = GetCurrentDirectory(0, NULL);
	TCHAR *buf = new TCHAR[size];
	size = GetCurrentDirectory(size, (LPWSTR)buf);
	
	for (int i = 0; i < size; i++)
	{
		std::cout << (char*)(buf + i);
	}
	_tprintf(L"\n");
	BOOL b = SetCurrentDirectory(L"D:/a");
	if (b)
	{
		_tprintf(L"SetCurrentDirectory successful!\n");
	}
	else
	{
		_tprintf(L"Call SetCD faild! Error = %d\n", GetLastError());
	}
	
	PMS p = (PMS)malloc(sizeof(MyStruct));
	p->a = 10;
	testN(*p);
	test(p);
	testP(p);
	std::cout << p->a << "\n";
	std::list<WIN32_FIND_DATA> lis = ergodicFileList();
	std::list< WIN32_FIND_DATA>::iterator iter = lis.begin();
		while (iter != lis.end())
		{
			WIN32_FIND_DATA fd = (*iter);
			iter++;
		}
	return 0;
}
