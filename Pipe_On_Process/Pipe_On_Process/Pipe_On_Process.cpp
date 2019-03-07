// Pipe_On_Process.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

HANDLE child_Out_W = NULL, child_Out_R = NULL, child_In_W = NULL, child_In_R = NULL;

#define BUFSIZE 1024
void CreateChildProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL bSuccess;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = child_Out_W;
	si.hStdInput = child_In_R;
	si.hStdOutput = child_Out_W;
	si.dwFlags |= STARTF_USESTDHANDLES;
	bSuccess = CreateProcess(L"C:/Users/i-robert/source/repos/Pipe_On_Process_Child/Debug/Pipe_On_Process_Child.exe",
		NULL, NULL, NULL,
		TRUE,
		0, NULL, NULL,
		&si,
		&pi);
	if (!bSuccess)
	{
		std::cout << "CreatePipe faild error code is: " << GetLastError() << "\n";
		return;
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
void WriteToPipe()
{
	char outBuf[] = "Father Process";
	//ZeroMemory(outBuf, BUFSIZ);
	if (!WriteFile(child_In_W, outBuf, strlen(outBuf), NULL, NULL))
	{
		std::cout << "Father WriteFile Faild!\n";
		return;
	}
	CloseHandle(child_Out_W);
}
void ReadFromPipe()
{
	BOOL bSuccess;
	char inBuf[BUFSIZE];
	DWORD number = 0;
	ZeroMemory(inBuf, BUFSIZE);
	HANDLE hFile = CreateFile(L"C:/Users/i-robert/Desktop/a.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("asd");
	}
	while (true)
	{
		std::cout << "Begin read...\n";
		bSuccess = ReadFile(child_Out_R, inBuf, BUFSIZE, &number, NULL);
		if (!bSuccess || number == 0)
		{
			break;
		}
		else
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			bSuccess = WriteFile(hFile, inBuf, number, NULL, NULL);
			FlushFileBuffers(hFile);
			if (!bSuccess)
			{
				break;
			}
		}
	}
}
int main()
{
	std::cout << "Lanuch\n";
	BOOL bSuccess;
	SECURITY_ATTRIBUTES sAttri;
	sAttri.nLength = sizeof(SECURITY_ATTRIBUTES);
	sAttri.bInheritHandle = TRUE;
	sAttri.lpSecurityDescriptor = NULL;
	// 创建子进程的out管道
	bSuccess = CreatePipe(&child_Out_R, &child_Out_W, &sAttri, 0);
	if (!bSuccess)
	{
		std::cout << "CreatePipe faild error code is: " << GetLastError() << "\n";
		return -1;
	}
	// 创建子进程in管道
	bSuccess = CreatePipe(&child_In_R, &child_In_W, &sAttri, 0);
	if (!bSuccess)
	{
		std::cout << "CreatePipe faild error code is: " << GetLastError() << "\n";
		return -2;
	}
	CreateChildProcess();
	WriteToPipe();
	ReadFromPipe();
	return 0;
}