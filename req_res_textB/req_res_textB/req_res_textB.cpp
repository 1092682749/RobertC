// req_res_textB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	int retCode = 0;
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrinfo hint, *result;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_socktype = SOCK_STREAM;
	getaddrinfo("localhost", "8080", &hint, &result);
	SOCKET conSock = socket(result->ai_flags, result->ai_socktype, result->ai_protocol);
	if (conSock == SOCKET_ERROR)
	{
		std::cout << GetLastError() << "\n";
	}
	connect(conSock, result->ai_addr, result->ai_addrlen);
	// std::cout << GetLastError();
	char outBuf[1024] = { 0 };
	char readBuf[1024] = { 0 };
	while (true)
	{
		std::cout << "请输入要发送的数据\n";
		std::cin >> outBuf;
		send(conSock, outBuf, strlen(outBuf), 0);
		recv(conSock, readBuf, 1024, 0);
		printf("返回数据%s\n", readBuf);
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
