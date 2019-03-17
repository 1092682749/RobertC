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
#include <string>
#include <time.h>
//#include <stdio.h>
//
#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 1024
#define SAVEPATH "C:/dyzFiles/code/"

char readBuf[BUFSIZE];
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
	// SOCKET clientSock = accept(sock, &addr, &clientLen);
	// std::cout << addr.sa_data << "\n";
	SOCKET clientSock = accept(sock, NULL, NULL);
	DWORD readByteNumber = 0, readCount = 0, writeByteNumber = 0;
	char *fixName;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	do
	{
		readCount++;
		readByteNumber = recv(clientSock, readBuf, BUFSIZE, 0);
		if (readByteNumber <= 0 || readByteNumber == SOCKET_ERROR)
		{
			break;
		}
		if (readCount == 1)
		{
				fixName = readBuf;
				std::cout << readBuf << "\n";
				std::string str = SAVEPATH;
				str.append(readBuf);
				std::cout << str << "\n";
				hFile = CreateFileA(str.data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		else
		{
			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("Error = %d", GetLastError());
				return -5;
			}
			if (!WriteFile(hFile, readBuf, readByteNumber, &writeByteNumber, NULL))
			{
				printf("Error = %d", GetLastError());
				return -6;
			}
		}
	} while (readByteNumber > 0);
	CloseHandle(hFile);
	closesocket(clientSock);
	closesocket(sock);
	return 0;
}
