// Read_Big_File.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <stdio.h>
//
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sock;
	addrinfo ai, *addrResult;
	memset(&ai, 0, sizeof(addrinfo));
	ai.ai_flags = AI_PASSIVE;
	ai.ai_family = AF_INET;
	ai.ai_protocol = IPPROTO_TCP;
	ai.ai_socktype = SOCK_STREAM;
	int retCode = getaddrinfo(NULL, "8080", &ai, &addrResult);
	if (retCode != 0)
	{
		return -2;
	}
	sock = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		return -1;
	}
	retCode = bind(sock, addrResult->ai_addr, addrResult->ai_addrlen);
	if (retCode != 0)
	{
		return -3;
	}
	retCode = listen(sock, 10);
	if (retCode == SOCKET_ERROR)
	{
		return -4;
	}
	sockaddr addr;
	int clientLen = 16;
	SOCKET clientSock = accept(sock, &addr, &clientLen);
	std::cout << addr.sa_data << "\n";
	// SOCKET clientSock = accept(sock, NULL, NULL);
	return 0;
}
