// Changing_Current_Directory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <list>

#define BUFSIZE MAX_PATH

template<typename T>
std::list<T> ergodicFileList(T t)
{

	LPWIN32_FIND_DATAA find_data;
	if (!FindFirstFile("*", find_data))
	{
		printf("find faild\n");
	}
	
}
int main()
{
	char c[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaasssssssss.txt";
	DWORD size = GetCurrentDirectory(0, NULL);
	TCHAR *buf = new TCHAR[size];
	size = GetCurrentDirectory(size, (LPWSTR)buf);
	
	for (int i = 0; i < size; i++)
	{
		std::cout << (char*)(buf + i);
	}
	printf("\n");
	BOOL b = SetCurrentDirectory(L"D:/");
	if (b)
	{
		printf("SetCurrentDirectory successful!\n");
	}
	else
	{
		printf("Call SetCD faild! Error = %d\n", GetLastError());
	}
	
	return 0;
}
