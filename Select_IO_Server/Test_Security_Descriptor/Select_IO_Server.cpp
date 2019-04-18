// Test_Security_Descriptor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int clientCount = 0;
SOCKET clientSockArr[10];
FD_SET fdReadSet;
char refuseBuf[] = "Server queue is full";

void RecvThread(LPVOID lpParam)
{
	while (true)
	{
		if (clientCount <= 0)
		{
			continue;
		}
		FD_ZERO(&fdReadSet);
		timeval val{ 2, 0 };

		for (int i = 0; i < clientCount; i++)
		{
			if (clientSockArr[i] == INVALID_SOCKET)
			{
				std::cout << "arr element invalid\n";
				continue;
			}
			FD_SET(clientSockArr[i], &fdReadSet);
			std::cout << "get error " << GetLastError() << "\n";
		}
		int ready = select(0, &fdReadSet, NULL, NULL, &val);
		std::cout << "wsa error: " << WSAGetLastError() << "\n";
		std::cout << ready << "\n";
		for (int i = 0; i < clientCount; i++)
		{
			int isset = FD_ISSET(clientSockArr[i], &fdReadSet) > 0;
			std::cout << "is set " << isset << "\n";
			if (isset > 0)
			{
				char readBuf[1024] = { 0 };
				recv(clientSockArr[i], readBuf, 1024, 0);
				printf("ssss%s\n", readBuf);
			}
		}
	}
}

void SendThread(LPVOID lpParam)
{

}
int main()
{
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrinfo hint, *addrResult;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_socktype = SOCK_STREAM;

	getaddrinfo(NULL, "8080", &hint, &addrResult);

	SOCKET serverSock = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	bind(serverSock, addrResult->ai_addr, addrResult->ai_addrlen);

	listen(serverSock, 10);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThread, NULL, 0, NULL);
	while (true)
	{
		SOCKET connectSock = accept(serverSock, NULL, NULL);
		std::cout << "come";
		if (connectSock == INVALID_SOCKET)
		{
			std::cout << "invalid socket \n";
			continue;
		}
		if (clientCount >= 10)
		{
			send(connectSock, refuseBuf, strlen(refuseBuf), 0);
			continue;
		}
		clientSockArr[clientCount] = connectSock;
		clientCount++;
	}
	return 0;
}
