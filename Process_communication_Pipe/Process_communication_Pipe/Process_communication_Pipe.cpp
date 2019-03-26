// Process_communication_Pipe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#define BUFSIZE 1024
#define PIPE_NAME TEXT("\\\\.\\pipe\\PIPE")
int main()
{
	DWORD readNumber = 0;
	HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, 2, BUFSIZE, BUFSIZE, 0, NULL);
	ConnectNamedPipe(hPipe, NULL);
	char buf[1024] = { 0 };
	char flag;
	do
	{
		std::cout << "请输入w/r>";
		flag = getchar();
		getchar();
		// std::cout << flag << "\n";
		switch (flag)
		{
		case 'w':
			std::cout << "input:";
			std::cin >> buf;
			getchar();
			WriteFile(hPipe, buf, strlen(buf), &readNumber, NULL);
			break;
		case 'r':
			ReadFile(hPipe, buf, BUFSIZE, &readNumber, NULL);

			printf("%s\n", buf);
			break;
		default:
			std::cout << "只能输入w(Write)/r(Read)\n";
			break;
		}
		

	} while (true);

	return 0;
}
