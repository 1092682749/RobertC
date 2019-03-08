
#include "pch.h"
#include <Windows.h>
#include <iostream>

int main()
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -100000000LL;
	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, L"timer_object");
	if (hTimer == NULL)
	{
		std::cout << "CreateWaitableTimer faild\n";
		return -1;
	}
	std::cout << "I will waiting 10s\n";
	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		std::cout << "SetWaitableTimer faild\n";
		return -2;
	}
	WaitForSingleObject(hTimer, INFINITE);
	return 0;
}