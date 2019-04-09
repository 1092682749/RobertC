// THARED_TLS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

#define THREADCOUNT 3
DWORD thread_name;
HANDLE hThreadEvent;
HANDLE hMainEvents[THREADCOUNT];
char nameArr[3][10] = { "one", "tow", "three" };

void ThreadFun(LPVOID lpParam)
{
	int i = *(int*)lpParam;
	
	char *name = nameArr[i];
	
	if (!TlsSetValue(thread_name, name))
	{
		std::cout << "设置线程本地存储失败\n";
		return;
	}
	//char *getName = (char*)TlsGetValue(thread_name);
	//std::cout << "我是线程" << i << "我已经准备好了\n";
	//SetEvent(hMainEvents[i]);
	WaitForSingleObject(hThreadEvent, INFINITE);
	std::cout << "我是线程" << i << "name是：" << (char*)TlsGetValue(thread_name) << "\n";
	SetEvent(hThreadEvent);
}
int main()
{
	thread_name = TlsAlloc();
	HANDLE hThreads[THREADCOUNT];
	
	
	hThreadEvent = CreateMutex(NULL, FALSE, TEXT("MUTEX"));
	for (int i = 0; i < THREADCOUNT; i++)
	{
		hMainEvents[i] = CreateEvent(NULL, TRUE, FALSE, L"ME");
	}
	for (int i = 0; i < THREADCOUNT; i++)
	{
		int *ii = (int*)malloc(sizeof(int));
		*ii = i;
		hThreads[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFun, ii, 0, NULL);
	}
	/*WaitForMultipleObjects(THREADCOUNT, hMainEvents, TRUE, INFINITE);
	SetEvent(hThreadEvent);*/
	WaitForMultipleObjects(THREADCOUNT, hThreads, TRUE, INFINITE);
	TlsFree(thread_name);
	return 0;
}
