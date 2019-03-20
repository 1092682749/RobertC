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
#define SAVEPATH ""
#define IPLEN 32
#define INTSIZE 4
SOCKET sock;
char OK_STATUS[] = "OK!";
char CLOSE_MESSAGE[] = "连接已关闭";

#pragma comment(lib, "Ws2_32.lib")

enum ERRORTYPE
{
	DATA_SOCKET_ERROR = 1,
	CMD_SOCKET_ERROR,
	FILE_HANDLE_ERROR,
	WRITE_OR_READ_ERROR
};
typedef struct ConnectInfo
{
	SOCKET s;
	sockaddr_in sa;
}*pConnectInfo;

void parseMessage(int argc, char* args[], const char* message)
{
	int len = 0, point = 0;
	for (int i = 0; i < argc; i++)
	{
		CopyMemory(&len, message + point, INTSIZE);
		CopyMemory(args[i], message + point + INTSIZE, len);
		point += INTSIZE + len;
	}
}

int HandPutCMD(int argc, char* args[], const SOCKET &s)
{
	if (s == INVALID_SOCKET)
	{
		std::cout << "该数据socket已经为无效\n";
		return DATA_SOCKET_ERROR;
	}
	std::string str = SAVEPATH;
	str.append(args[2]);
	std::cout << args[2] << "\n";
	HANDLE hUploadFile = CreateFileA(str.data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hUploadFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "hUploadFile invalid\n";
		return FILE_HANDLE_ERROR;
	}
	char readFileBuf[BUFSIZE];
	int readNumber = 0;
	DWORD writeNumber = 0;
	do
	{
		readNumber = recv(s, readFileBuf, BUFSIZE, 0);
		if (!WriteFile(hUploadFile, readFileBuf, readNumber, &writeNumber, NULL))
		{
			std::cout << "WriteFile faild\n";
			return WRITE_OR_READ_ERROR;
		}
	} while (readNumber >= BUFSIZE);
	std::cout << "传输完成\n";
	CloseHandle(hUploadFile);
	return 0;
}
int HandGetCMD(int argc, char* args[], const SOCKET &s)
{
	char readBuf[BUFSIZE] = { 0 };
	DWORD readNumber = 0;
	HANDLE hGetFile = CreateFileA(args[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hGetFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "FILE_HANDLE_ERROR\n";
		return FILE_HANDLE_ERROR;
	}
	do
	{
		if (!ReadFile(hGetFile, readBuf, BUFSIZE, &readNumber, NULL))
		{
			std::cout << "WRITE_OR_READ_ERROR\n";
			return WRITE_OR_READ_ERROR;
		}
		if (readNumber == 0)
		{
			std::cout << "读取完成\n";
			break;
		}
		if (send(s, readBuf, readNumber, 0) == SOCKET_ERROR)
		{
			std::cout << "DATA_SOCKET_ERROR\n";
			return DATA_SOCKET_ERROR;
		}
	} while (readNumber > 0);
	CloseHandle(hGetFile);
	return 0;
}


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
			std::stringstream ostr;
			ostr << readBuf;
			int port;
			ostr >> port;
			printf("客户端待连PORT:%d\n", port);
			// 端口有效则连入客户端
			if (port > 1024 && port < 10000)
			{
				char cFromIp[2 * IPLEN];
				memset(cFromIp, 0, 2 * IPLEN);
				WideCharToMultiByte(CP_UTF8, 0, fromIp, IPLEN, cFromIp, 2 * IPLEN, NULL, NULL);
				MySocketUtils::SocketFactory::GetClientSocket(dataSock, cFromIp, readBuf, clientAddrResult);
				if (connect(dataSock, clientAddrResult->ai_addr, clientAddrResult->ai_addrlen) != SOCKET_ERROR)
				{
					send(dataSock, OK_STATUS, strlen(OK_STATUS), 0);
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
			char cmd[1024] = { 0 };
			char fileArg[200] = { 0 }, fileArg2[200] = { 0 };
			std::string str = SAVEPATH;
			str.append(readBuf);
			std::cout << readBuf << "\n";
			char* args[3];
			for (int i = 0; i < 3; i++)
			{
				args[i] = new char[1024];
				memset(args[i], 0, 1024);
			}
			parseMessage(3, args, readBuf);
			CopyMemory(cmd, args[0], sizeof(args[0]));
			CopyMemory(fileArg, args[1], sizeof(args[1]));
			CopyMemory(fileArg2, args[2], sizeof(args[2]));
			
			if (strcmp("put", cmd) == 0)
			{
				std::cout << readBuf << "\n";
				std::cout << "这是一个put命令\n";
				if (HandPutCMD(3, args, dataSock) != 0)
				{
					break;
				}
			}
			else if (strcmp("get", cmd) == 0)
			{
				std::cout << "这是一个get命令\n";
				if (HandGetCMD(3, args, dataSock) != 0)
				{
					break;
				}
			}
			else if (strcmp("ext", cmd) == 0)
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
	send(clientSock, CLOSE_MESSAGE, strlen(CLOSE_MESSAGE), 0);
	CloseHandle(hFile);
	closesocket(clientSock);
	closesocket(dataSock);
}

int main()
{
	print_UI_Welcome();
	printf("Start up...");
	
	
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
/*
	该协议(smftp)报文格式分为三段(Simple File Transfer Protocol)
	commendType:([int][string]) int类型的参数使用四个字节用来标识string长度；
	filename1([int][string]) 参数类型意义同上
	filename2([int][string]) 同上
*/
