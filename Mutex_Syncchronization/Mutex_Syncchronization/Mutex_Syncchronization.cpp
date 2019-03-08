// Mutex_Syncchronization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

#define THREADCOUNT 2

HANDLE ghMutex;

DWORD WINAPI WriteToDatabase(LPVOID);

void ThreadFun_Attmpte_Obtain_Mutex(LPVOID lpParam)
{
	DWORD id = GetCurrentThreadId();
	DWORD res = WaitForSingleObject(ghMutex, INFINITE);
	switch (res)
	{
	case WAIT_OBJECT_0:
	//case 0:
		std::cout << "My id is " << id << ",And I have Mutex\n";
		std::cout << "Do worke.......\n";
		std::cout << "ReleaseMUtex\n";
		ReleaseMutex(ghMutex);
		break;
	case WAIT_ABANDONED:
		std::cout << "Mutex Abandoned And I get it\n";
	default:
		break;
	}
}
int main()
{
	HANDLE aThread[THREADCOUNT];
	DWORD TID[THREADCOUNT];

	ghMutex = CreateMutex(NULL,
		TRUE,
		L"Mutex_To_Multiple_Thread");											// 创建互斥锁
	if (ghMutex == NULL)
	{
		std::cout << "CreateMytex faild\n";
		return -1;
	}

	for (int i = 0; i < THREADCOUNT; i++)
	{
		aThread[i] = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)ThreadFun_Attmpte_Obtain_Mutex,
			&i,
			0,
			&TID[i]
		);																		// 创建线程
		if (aThread[i] == NULL)
		{
			std::cout << "CreateThread faild\n";
			return -2;
		}
	}
	std::cout << "I will sleep 5s\n";
	int timeCount = 5;															// 让主线程倒计时，在倒计时结束之前不释放锁
	while (timeCount-- > 0)
	{
		std::cout << "Count down " << timeCount << "s \n";
		Sleep(1000);
	}
	std::cout << "main thread wake\n";											
	ReleaseMutex(ghMutex);														// 释放Mutex
	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);
	return 0;
}