

#include "pch.h"
#include <Windows.h>
#include <iostream>
char nameArr[3][10] = { "Thread_A", "Thread_B", "Thread_C" };
HANDLE ghMutex;
void ThreadMainFun(LPVOID lpParam);

void ThreadCall();

DWORD thread_name;

HANDLE hHeap;
int main()
{
	thread_name = TlsAlloc();
	HANDLE hThreads[3];
	ghMutex = CreateMutex(NULL, FALSE, TEXT("M"));
	for (int i = 0; i < 3; i++)
	{
		int *ii = (int*)malloc(sizeof(int));
		*ii = i;
		hThreads[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadMainFun, ii, 0, NULL);
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
	LPVOID baseAddress = HeapAlloc(hHeap, LPTR, 10 * sizeof(char));
	CopyMemory(baseAddress, nameArr[i], strlen(nameArr[i]) * sizeof(char));
	TlsSetValue(thread_name, baseAddress);
	ThreadCall();
}

void ThreadCall()
{
	WaitForSingleObject(ghMutex, INFINITE);
	LPVOID name = TlsGetValue(thread_name);
	char pFormatName[10] = { 0 };
	CopyMemory(pFormatName, name, 8);
	printf("%s\n", pFormatName);
	HeapFree(hHeap, NULL, name);
	ReleaseMutex(ghMutex);
}