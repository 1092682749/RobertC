// memory_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
// A short program to demonstrate dynamic memory allocation
// using a structured exception handler.

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <exception>
#define FETCH_AGAINST(B) B ? TRUE : FALSE // For exit
#define PAGELIMIT 1
LPSTR lpNextPage;
DWORD dwPageSize;
DWORD dwPages = 0;

INT exceptionHandler(DWORD eCode)
{
	LPVOID lpResult;
	if (eCode != EXCEPTION_ACCESS_VIOLATION)
	{
		std::cout << "eCode is: " << eCode << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}
	if (dwPages > PAGELIMIT)
	{
		std::cout << "exception out of pages" << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}
	lpResult = VirtualAlloc(
		lpNextPage,
		dwPageSize,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	if (lpResult == NULL)
	{
		std::cout << "alloc fault" << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}
	std::cout << "alloc success!" << std::endl;
	lpNextPage = (LPSTR)((PCHAR)lpNextPage + dwPageSize);
	return EXCEPTION_CONTINUE_EXECUTION;
}
int _tmain()
{
	LPSTR lpPtr;
	SYSTEM_INFO si;
	std::cout << sizeof(si) << std::endl;
	std::cout << sizeof(LPVOID) << std::endl;
	GetSystemInfo(&si);
	dwPageSize = si.dwPageSize;
	LPVOID baseAddress = NULL;
	baseAddress = VirtualAlloc(NULL, PAGELIMIT * si.dwPageSize, MEM_RESERVE, PAGE_NOACCESS);
	if (baseAddress == NULL)
	{
		std::cout << "alloc memory faild" << std::endl;
	}
	lpPtr = lpNextPage = (LPSTR)baseAddress;
	for (int i = 0; i < PAGELIMIT * si.dwPageSize; i++)
	{
		_try{
			*(lpPtr + i) = 'a';
		}
		_except(exceptionHandler(GetExceptionCode()))
		{
			std::cout << "exit process" << std::endl;
			ExitProcess(GetLastError());
		}
	}
	
	VirtualFree(baseAddress, 10 * si.dwPageSize, MEM_RELEASE);
	return 0;
}