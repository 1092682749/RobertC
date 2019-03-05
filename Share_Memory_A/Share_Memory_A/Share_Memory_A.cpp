// Share_Memory_A.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>
#define BUFF_SIZE 225
const char *message = "I am a messgae from A";
LPCWSTR name = TEXT("shareSpace");
int main(int argc, TCHAR *argv[])
{
	std::cout << argc << std::endl;
	for (size_t i = 0; i < argc; i++)
	{
		printf("%s\n", argv[i]);
		std::cout << argv[i] << std::endl;
	}
	HANDLE fm = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		BUFF_SIZE,
		name
	);
	if (fm == NULL)
	{
		return -1;
	}
	LPSTR pBuff = (LPSTR)MapViewOfFile(fm,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		BUFF_SIZE);
	if (pBuff == NULL)
	{
		return -1;
	}
	CopyMemory(pBuff, message, (strlen(message) * sizeof(char)));
	char *recvBuf = new char[10240];
	while (true)
	{
		std::cin >> recvBuf;
		CopyMemory(pBuff, recvBuf, sizeof(recvBuf));
	}
	getchar();
	UnmapViewOfFile(pBuff);
	CloseHandle(fm);
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
