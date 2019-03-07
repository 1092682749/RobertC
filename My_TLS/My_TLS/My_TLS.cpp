

#include "pch.h"
#include <Windows.h>
#include <iostream>

void ThreadMainFun(LPVOID lpParam);

void ThreadCall();

DWORD thread_name;

HANDLE hHeap;
int main()
{
	thread_name = TlsAlloc();
	DWORD TID;
	HANDLE hThreads[3];
	for (int i = 0; i < 3; i++)
	{
		hThreads[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadMainFun, &i, 0, &TID);
		if (hThreads[i] == NULL)
		{
			std::cout << "create thread faild\n";
		}
	}
	WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
	return 0;
}

void ThreadMainFun(LPVOID lpParam)
{
	hHeap = GetProcessHeap();
	int i = *((int *)lpParam);
	char name[10] = "I'am ";
	name[5] = i + ('1' - 1);
	name[6] = 0;
	//LPVOID baseAddress = HeapAlloc(LPTR, 10 * sizeof(char));
	LPVOID baseAddress = HeapAlloc(hHeap, LPTR, 10 * sizeof(char));
	CopyMemory(baseAddress, name, strlen(name) * sizeof(char));
	TlsSetValue(thread_name, baseAddress);
	ThreadCall();
}

void ThreadCall()
{
	LPVOID name = TlsGetValue(thread_name);
	std::cout << (char*)name << std::endl;
	//LocalFree((HLOCAL)name);
	HeapFree(hHeap, NULL, name);
}