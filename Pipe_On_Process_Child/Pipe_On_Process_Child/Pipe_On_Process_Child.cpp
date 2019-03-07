// Pipe_On_Process_Child.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

#define BUFSIZE 1024
int main()
{
	char readBuf[BUFSIZE], writeBuf[BUFSIZE];
	HANDLE hRead, hWrite;
	BOOL bSuccess;
	DWORD number = 0;
	hRead = GetStdHandle(STD_INPUT_HANDLE);
	hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"<<std::flush;
	if (hRead == INVALID_HANDLE_VALUE || hWrite == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	HANDLE hFile = CreateFile(L"C:/Users/i-robert/Desktop/ab.txt", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char outBuf[] = "Child Process";
	WriteFile(hWrite, outBuf, strlen(outBuf), NULL, NULL);
	while (true)
	{
		bSuccess = ReadFile(hRead, readBuf, BUFSIZE, &number, NULL);
		if (!bSuccess || number == 0)
		{
			break;
		}
		else
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			bSuccess = WriteFile(hFile, readBuf, number, NULL, NULL);
			FlushFileBuffers(hFile);
			if (!bSuccess)
			{
				break;
			}
		}
	}
	return 0;
}
