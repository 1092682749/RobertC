// Read_Big_File.cpp : This file contains the 'main' function. Program execution begins and ends there.

#pragma once

#define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include "SocketUtils.h"
#include <tchar.h>
#include "SMFTPUI.h"



#define BUFSIZE 1024
#define SAVEPATH "D:/a/"
#define IPLEN 32


#pragma comment(lib, "Ws2_32.lib")


typedef struct ConnectInfo
{
	SOCKET s;
	sockaddr_in sa;
}*pConnectInfo;

// 工作线程用来处理连接
void WorkThreadFun(LPVOID lpParam)
{
	char readBuf[BUFSIZE];
	pConnectInfo ci = (pConnectInfo)lpParam;
	SOCKET clientSock = ci->s, dataSock = INVALID_SOCKET;
	TCHAR fromIp[IPLEN];
	memset(fromIp, 0, IPLEN);
	InetNtop(AF_INET, &((ci->sa).sin_addr), fromIp, IPLEN);
	_tprintf(L"Connect is come! Client address is: %s:%d \n", fromIp, ntohs((ci->sa).sin_port));

	// std::cout << clientAddr.sa_data << "\n";
	DWORD readByteNumber = 0, readCount = 0, writeByteNumber = 0;
	char *fixName;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	addrinfo *clientAddrResult;
	do
	{
		readCount++;
		readByteNumber = recv(clientSock, readBuf, BUFSIZE, 0);
		if (readByteNumber <= 0 || readByteNumber == SOCKET_ERROR)
		{
			break;
		}
		// 这将是一个建立数据连接的指令，要从中读取出远程端口，然后建立数据连接
		if (readCount == 1)
		{

			fixName = readBuf;
			std::cout << readBuf << "\n";
			std::stringstream ostr;
			ostr << readBuf;
			int port;
			ostr >> port;
			printf("PORT:%d", port);
			// 端口有效则连入客户端
			if (port > 1024 && port < 10000)
			{
				char cFromIp[2 * IPLEN];
				memset(cFromIp, 0, 2 * IPLEN);
				WideCharToMultiByte(CP_UTF8, 0, fromIp, IPLEN, cFromIp, 2 * IPLEN, NULL, NULL);
				MySocketUtils::SocketFactory::GetClientSocket(dataSock, cFromIp, readBuf, clientAddrResult);
				if (connect(dataSock, clientAddrResult->ai_addr, clientAddrResult->ai_addrlen) != SOCKET_ERROR)
				{
					send(dataSock, "test", 4, 0);
				}
			}
			else
			{
				printf("无法解析客户端ip\n");
				closesocket(dataSock);
				closesocket(clientSock);
				return;
			}
			
		}
		// 该分支用来解析命令
		else
		{
			std::string str = SAVEPATH;
			str.append(readBuf);
			std::cout << readBuf << "\n";
			char cmdHeader[10];
			memset(cmdHeader, 0, 10);
			CopyMemory(cmdHeader, readBuf, 3);
			if (strcmp("put", cmdHeader) == 0)
			{
				std::cout << readBuf << "\n";
				std::cout << "这是一个put命令\n";
			}
			else if (strcmp("get", cmdHeader) == 0)
			{
				std::cout << "这是一个get命令\n";
			}
			else if (strcmp("ext", cmdHeader) == 0)
			{
				std::cout << "关闭和客户端" << fromIp << "的连接\n";
				break;
			}
			/*hFile = CreateFileA(str.data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("Error = %d", GetLastError());
				return;
			}
			if (!WriteFile(hFile, readBuf, readByteNumber, &writeByteNumber, NULL))
			{
				printf("Error = %d", GetLastError());
				return;
			}*/
		}
	} while (readByteNumber > 0);
	CloseHandle(hFile);
	closesocket(clientSock);
}

int main()
{
	print_UI_Welcome();
	printf("Start up...");
	
	SOCKET sock;
	addrinfo *addrResult = NULL;
	
	int retCode = MySocketUtils::SocketFactory::GetSocket(sock, "8080", addrResult);
	
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

	int clientLen = 32;
	
	sockaddr_in clientAddr;
	printf("\r Wait connect...\n");


	// mian线程用来接受连接
	while (true)
	{
		SOCKET clientSock = accept(sock, (sockaddr*)&clientAddr, &clientLen);

		if (clientSock == INVALID_SOCKET)
		{
			printf("Error connect\n");
			continue;
		}
		pConnectInfo pCi = (pConnectInfo)malloc(sizeof(ConnectInfo));
		pCi->s = clientSock;
		pCi->sa = clientAddr;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThreadFun, (LPVOID)pCi, 0, NULL);
	}
	closesocket(sock);
	return 0;
}
