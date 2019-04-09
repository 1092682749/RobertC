// Thread_Synchronization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#define MUTEX TEXT("Mutex_Write")
#define THREAD_COUNT 5
#define MAPLENGTH 1024

HANDLE ghMutex;
char criticalDataBuf[1024] = { 0 };
char pc[] = { 'a', 'b', 'c', 'd', 'e' };
volatile int pointPosition = 0;

// 使用同步的线程函数
void ThreadFun(LPVOID lpParam)
{
	// 等待获取Mutex对象
	DWORD resCode = WaitForSingleObject(ghMutex, INFINITE);
	std::cout << "我拿到的result是：" << pointPosition << "\n";
	std::cout << "对应的字符是:" << pc[pointPosition] << "\n";
	switch (resCode)
	{
		
	case WAIT_OBJECT_0:
		for (int i = 0; i < 100; i++)
		{
			// 把字符复制到字符数组里
			CopyMemory(criticalDataBuf + i + (pointPosition * 100), &(pc[pointPosition]), 1);
		}
		std::cout << "\n";
		pointPosition++;
		// 释放Mutex对象
		ReleaseMutex(ghMutex);
		break;
	case WAIT_ABANDONED:
		std::cout << "Mutex Abandoned And I get it\n";
		break;
	default:
		std::cout << "获取互斥锁失败\n";
		break;
	}
	
}

// 未使用同步的线程函数
void NoSyncThreadFun(LPVOID lpParam)
{
	std::cout << "我拿到的result是：" << pointPosition << "\n";
	std::cout << "对应的字符是:" << pc[pointPosition] << "\n";
	for (int i = 0; i < 100; i++)
	{
		CopyMemory(criticalDataBuf + i + (pointPosition * 100), &(pc[pointPosition]), 1);
	}
	std::cout << "\n";
	pointPosition++;
}

int main()
{
	HANDLE hThreadArr[THREAD_COUNT];
	DWORD TIDs[THREAD_COUNT];
	HANDLE hMapping = NULL;

	ghMutex = CreateMutex(NULL, FALSE, MUTEX);
	if (ghMutex == NULL)
	{
		std::cout << "创建互斥锁失败\n";
		return -1;
	}
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		// 使用Mutex同步的线程
		 hThreadArr[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFun, NULL, 0, &TIDs[i]);
		 // 未同步的线程
		// hThreadArr[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NoSyncThreadFun, NULL, 0, &TIDs[i]);
		if (hThreadArr[i] == NULL)
		{
			std::cout << "创建线程失败\n";
			continue;
		}
	}
	// 等待所有线程完成他们的工作
	WaitForMultipleObjects(THREAD_COUNT, hThreadArr, TRUE, INFINITE);
	std::cout << "pointPosition is:" << pointPosition << "\n";
	printf("字符串内容为：%s\n", criticalDataBuf);
	return 0;
}
