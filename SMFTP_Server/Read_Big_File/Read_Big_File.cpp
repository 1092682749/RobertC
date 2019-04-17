// Read_Big_File.cpp : This file contains the 'main' function. Program execution begins and ends there.

#pragma once

#define WIN32_LEAN_AND_MEAN // 删除一些头文件
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include "SocketUtils.h"
#include <tchar.h>
#include "SMFTPUI.h"

#define SMFTP_PUT "put"
#define SMFTP_GET "get"
#define SMFTP_EXIT "ext"


#define BUFSIZE 1024
#define SAVEPATH ""
#define IPLEN 32
#define INTSIZE 4
SOCKET sock;
char OK_STATUS[] = "OK!";
char endFlag[16] = { 0 };
char CLOSE_MESSAGE[] = "Connection closed";

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
}*PConnectInfo;

// 解析控制报文
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

// 处理put命令
int HandPutCMD(int argc, char* args[], const SOCKET &s)
{
	if (s == INVALID_SOCKET)
	{
		std::cout << "Invalid socket\n";
		return DATA_SOCKET_ERROR;
	}
	std::string str = SAVEPATH;
	str.append(args[2]);
	std::cout << "put file name: " << args[2] << "\n";
	HANDLE hUploadFile = CreateFileA(str.data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hUploadFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "hUploadFile invalid\n";
		return FILE_HANDLE_ERROR;
	}
	char readFileBuf[BUFSIZE];
	int readNumber = 0;
	DWORD writeNumber = 0;
	/* 标志位用来读取每个数据报文的前14个字节
	在该示例程序中只在最后一个报文中加入标志信息
	在实际传输中应该为每个报文段加入头部标志位
	*/
	char flagBit[16] = { 0 };
	
	do
	{
		
		readNumber = recv(s, readFileBuf, BUFSIZE, 0);
		// std::cout << "读取了" << readNumber << "字节\n" << readFileBuf << "\n";
		if (readNumber == 15)
		{
			CopyMemory(flagBit, readFileBuf, 15);
			// std::cout << endFlag << "\n" << flagBit << "\n";
			if (strcmp(endFlag, flagBit) == 0) break;
		}
		
		if (!WriteFile(hUploadFile, readFileBuf, readNumber, &writeNumber, NULL))
		{
			std::cout << "WriteFile faild\n";
			return WRITE_OR_READ_ERROR;
		}
	} while (readNumber > 0);
	std::cout << "Transmission complete\n";

	CloseHandle(hUploadFile);
	return 0;
}

// 处理get命令
int HandGetCMD(int argc, char* args[], const SOCKET &s)
{
	char readBuf[BUFSIZE] = { 0 };
	DWORD readNumber = 0;
	HANDLE hGetFile = CreateFileA(args[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	printf("get file name: %s\n", args[1]);
	if (hGetFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "FILE_HANDLE_ERROR\n";
		return FILE_HANDLE_ERROR;
	}
	// std::cout << GetLastError() << "\n";
	long readCount = 0;
	do
	{
		readCount++;
		if (!ReadFile(hGetFile, readBuf, BUFSIZE, &readNumber, NULL))
		{
			std::cout << "WRITE_OR_READ_ERROR\n";
			return WRITE_OR_READ_ERROR;
		}
		// std::cout << "read bytes: " << readNumber << "\n";
		if (readNumber == 0)
		{
			std::cout << "Read completion\n";
			break;
		}
		if (send(s, readBuf, readNumber, 0) == SOCKET_ERROR)
		{
			std::cout << "DATA_SOCKET_ERROR\n";
			return DATA_SOCKET_ERROR;
		}
	} while (readNumber > 0);
	// 发送结束标志 等待一段时间发送，基础5秒，防止TCP粘包
	long sleepTime = 5000 + (readCount / 1024) * 500;
	std::cout << "Delayed " << sleepTime << "ms send end message\n";
	Sleep(sleepTime);
	send(s, endFlag, strlen(endFlag), 0);
	std::cout << "Transmission complete\n";
	CloseHandle(hGetFile);
	return 0;
}


// 工作线程用来处理连接
void WorkThreadFun(LPVOID lpParam)
{
	char readBuf[BUFSIZE];
	PConnectInfo ci = (PConnectInfo)lpParam;
	SOCKET clientSock = ci->s, dataSock = INVALID_SOCKET;
	TCHAR fromIp[IPLEN];
	memset(fromIp, 0, IPLEN);
	// 网络字节序转换成点分十进制字符串
	InetNtop(AF_INET, &((ci->sa).sin_addr), fromIp, IPLEN);
	_tprintf(L"Connect is come! Client address is: %s:%d \n", fromIp, ntohs((ci->sa).sin_port));

	
	DWORD readByteNumber = 0, writeByteNumber = 0;
	BOOL bFirstRead = TRUE; // 该值用来判断是否为端口报文
	char *fixName;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	addrinfo *clientAddrResult;
	do
	{
		readByteNumber = recv(clientSock, readBuf, BUFSIZE, 0); // 读取客户端数据
		if (readByteNumber <= 0 || readByteNumber == SOCKET_ERROR)
		{
			break;
		}
		// 这将是一个建立数据连接的指令，要从中读取出远程端口，然后建立数据连接
		if (bFirstRead)
		{
			bFirstRead = FALSE;
			
			std::stringstream ostr;
			ostr << readBuf;
			int port;
			ostr >> port;
			printf("Client Open PORT:%d\n", port);
			// 端口有效则连入客户端
			if (port > 1024 && port < 10000)
			{
				char cFromIp[2 * IPLEN];
				memset(cFromIp, 0, 2 * IPLEN);
				WideCharToMultiByte(CP_UTF8, 0, fromIp, IPLEN, cFromIp, 2 * IPLEN, NULL, NULL);
				MySocketUtils::SocketFactory::GetClientSocket(dataSock, cFromIp, readBuf, clientAddrResult);
				if (connect(dataSock, clientAddrResult->ai_addr, clientAddrResult->ai_addrlen) != SOCKET_ERROR)
				{
					send(dataSock, OK_STATUS, strlen(OK_STATUS), 0); // 发送OK报文
				}
			}
			else
			{
				printf("Not a valid port number \n");
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
			//std::string str = SAVEPATH;
			//str.append(readBuf);
			//std::cout << readBuf << "\n";
			char* args[3];
			for (int i = 0; i < 3; i++)
			{
				args[i] = new char[1024];
				memset(args[i], 0, 1024);
			}
			parseMessage(3, args, readBuf); // 解析报文将参数放入args数组
			CopyMemory(cmd, args[0], sizeof(args[0]));
			CopyMemory(fileArg, args[1], sizeof(args[1]));
			CopyMemory(fileArg2, args[2], sizeof(args[2]));
			// 处理不同的命令
			if (strcmp(SMFTP_PUT, cmd) == 0)
			{
				// std::cout << readBuf << "\n";
				std::cout << "Receiving files\n";
				if (HandPutCMD(3, args, dataSock) != 0)
				{	
					break;
				}
			}
			else if (strcmp(SMFTP_GET, cmd) == 0)
			{
				std::cout << "Sending files\n";
				if (HandGetCMD(3, args, dataSock) != 0)
				{
					break;
				}
			}
			else if (strcmp(SMFTP_EXIT, cmd) == 0)
			{
				std::cout << "Close the connection with the client" << fromIp << "\n";
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

// 初始化结束报文
void initEndMessage(char* buf)
{
	
	endFlag[0] = 'e'; endFlag[1] = 'n'; endFlag[2] = 'd';
	for (int i = 3; i < 15; i++)
	{
		endFlag[i] = '1';
	}
}
int main()
{
	print_UI_Welcome();
	printf("Start up...");
	initEndMessage(endFlag);
	
	addrinfo *addrResult = NULL;
	
	// 获取socket
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

		// 创建一个结构体向工作线程传递参数
		PConnectInfo pCi = (PConnectInfo)malloc(sizeof(ConnectInfo));
		pCi->s = clientSock;
		pCi->sa = clientAddr;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThreadFun, (LPVOID)pCi, 0, NULL);
	}
	closesocket(sock);
	return 0;
}
/*
	该协议(smftp)控制报文格式分为三段(Simple File Transfer Protocol)
	commendType:([int][string]) int类型的参数使用四个字节用来标识string长度；
	filename1([int][string]) 参数类型意义同上
	filename2([int][string]) 同上
*/
