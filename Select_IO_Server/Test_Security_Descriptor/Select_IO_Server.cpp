// Test_Security_Descriptor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int clientCount = 0;
SOCKET clientSockArr[10] = { INVALID_SOCKET };
FD_SET fdReadSet;
char refuseBuf[] = "Server queue is full";

void RecvThread(LPVOID lpParam)
{
	while (true)
	{
		FD_ZERO(&fdReadSet);
		timeval val{ 2, 0 };
		// 找到所有的有效socket
		for (int i = 0; i < clientCount; i++)
		{
			if (clientSockArr[i] == INVALID_SOCKET)
			{
				// std::cout << "arr element invalid\n";
				continue;
			}
			// 放入fd_set
			FD_SET(clientSockArr[i], &fdReadSet);
		}

		// select会把fd_set里的不可读写的fd清除，留下可读写的
		int ready = select(0, &fdReadSet, NULL, NULL, &val);

		//std::cout << ready << "\n";
		for (int i = 0; i < clientCount; i++)
		{
			if (clientSockArr[i] == INVALID_SOCKET)
			{
				// std::cout << "arr element invalid\n";
				continue;
			}
			// 判断该套接字是否还在set里，换句话说就是是否可读
			int isset = FD_ISSET(clientSockArr[i], &fdReadSet) > 0;
			//std::cout << "is set " << isset << "\n";
			if (isset > 0)
			{
				char readBuf[1024] = { 0 };
				int readNumber = recv(clientSockArr[i], readBuf, 1024, 0);
				if (readNumber == 0 || WSAECONNRESET == WSAGetLastError()) // 说明连接已被关闭
				{
					closesocket(clientSockArr[i]);
					clientSockArr[i] = INVALID_SOCKET;
					continue;
				}
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
	bool have = true;
	while (true)
	{
		SOCKET connectSock = accept(serverSock, NULL, NULL);
		if (connectSock == INVALID_SOCKET)
		{
			std::cout << "invalid socket \n";
			continue;
		}
		if (clientCount >= 10)
		{
			
		}
		have = false;
		// 遍历数组找到空位，如果没有空位就发送一个回复并关闭连接
		for (int i = 0; i < 10; i++)
		{
			if (clientSockArr[i] == INVALID_SOCKET)
			{
				clientSockArr[i] = connectSock;
				have = true;
			}
		}
		if (!have)
		{
			send(connectSock, refuseBuf, strlen(refuseBuf), 0);
			closesocket(connectSock);
			continue;
		}
	}
	return 0;
}
