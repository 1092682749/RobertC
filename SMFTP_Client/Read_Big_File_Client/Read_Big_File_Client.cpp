// Read_Big_File_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "MySocketUtils.h"
#include "SMFTP_CMD.h"

#define READBUFSIZE 1024
#define INTSIZE 4

enum ERRORTYPE
{
	DATA_SOCKET_ERROR = 1,
	CMD_SOCKET_ERROR,
	FILE_HANDLE_ERROR,
	WRITE_OR_READ_ERROR
};

SOCKET client;
char cmd[1024] = { 0 };
char fileArg[200] = { 0 }, fileArg2[200] = { 0 };
SOCKET dataListenSock = INVALID_SOCKET;
SOCKET dataConnectSock = INVALID_SOCKET;
fd_set sSet;

void waitDataConnect(LPVOID lpParam)
{
	char* msg = (char*)lpParam;
	char data[1024] = { 0 };
	addrinfo *ai;
	MySocketUtils::SocketFactory::GetSocket(dataListenSock, msg, ai);
	bind(dataListenSock, ai->ai_addr, ai->ai_addrlen);
	listen(dataListenSock, 1);
	dataConnectSock = accept(dataListenSock, NULL, NULL);
	if (dataConnectSock == INVALID_SOCKET)
	{
		std::cout << "Error!\n";
		return;
	}
	recv(dataConnectSock, data, 1024, 0);
	std::cout << "Success Data Connect!\nResponse: " << data << "\n";
}
void ReadServerResponse(LPVOID lpParam)
{
	char outBuf[1024] = { 0 };
	SOCKET *s = (SOCKET*)lpParam;
	int readNumber = 0;
	while (true)
	{
		readNumber = recv(*s, outBuf, 1024, 0);
		if (readNumber == SOCKET_ERROR)
		{
			std::cout << GetLastError() << "\n";
			continue;
		}
		if (readNumber > 0)
		{
			std::cout << outBuf << "\n";
		}
	}
}
int putFile(const char* fileName)
{
	TCHAR wFileName[1024] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, fileName, 1024, wFileName, 1024);
	HANDLE hUploadFile = CreateFile(wFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hUploadFile == INVALID_HANDLE_VALUE)
	{
		return FILE_HANDLE_ERROR;
	}
	char readFileBuf[READBUFSIZE];
	DWORD readNumber = 0;

	do
	{
		if (!ReadFile(hUploadFile, readFileBuf, READBUFSIZE, &readNumber, NULL))
		{
			std::cout << "Read Faild!\n";
			return WRITE_OR_READ_ERROR;
		}
		if (readNumber == 0)
		{
			break;
		}
		if (send(dataConnectSock, readFileBuf, readNumber, 0) == SOCKET_ERROR)
		{
			return DATA_SOCKET_ERROR;
		}
		/*std::cout << "我发送了" << readNumber << "个字节\n";*/
	} while (readNumber > 0);
	std::cout << "传输完成\n";
	CloseHandle(hUploadFile);
	return 0;
}
int getFile(const char* fileName)
{
	FD_SET(dataConnectSock, &sSet);
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	char readBuf[READBUFSIZE];
	DWORD readNumber = 0, writeNumber = 0;
	HANDLE hGetFile = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hGetFile == INVALID_HANDLE_VALUE)
	{
		return FILE_HANDLE_ERROR;
	}
	do
	{
		readNumber = recv(dataConnectSock, readBuf, READBUFSIZE, 0);
		if (readNumber == SOCKET_ERROR)
		{
			return DATA_SOCKET_ERROR;
		}
		if (!WriteFile(hGetFile, readBuf, readNumber, &writeNumber, NULL))
		{
			std::cout << "Writr faild\n";
			return WRITE_OR_READ_ERROR;
		}
		/*std::cout << "写入文件" << writeNumber << "个字节\n";*/
	} while (readNumber >= READBUFSIZE);
	std::cout << "传输完成\n";
	CloseHandle(hGetFile);
	return 0;
}
void CombineMessage(int argc, char* args[], char* message)
{
	int len = 0;
	int point = 0;
	for (int i = 0; i < argc; i++)
	{
		len = strlen(args[i]);
		CopyMemory(message + point, &len, INTSIZE);
		CopyMemory(message + point + INTSIZE, args[i], len);
		point += len + INTSIZE;
	}
}
int main()
{
	FD_ZERO(&sSet);
	
	addrinfo *ai;
	char host[12] = { 0 }, msg[] = "9000";
	int retCode = 0;
	std::cout << "请输入要连接的服务器IP:>";
	std::cin >> host;
	retCode = MySocketUtils::SocketFactory::GetClientSocket(client, host, "8080", ai);
	if (retCode < 0)
	{
		return retCode;
	}

	retCode = connect(client, ai->ai_addr, ai->ai_addrlen);
	if (retCode == SOCKET_ERROR)
	{
		return retCode;
	}
	if (send(client, msg, sizeof(msg), 0) < 0)
	{
		return -1;
	}
	FD_SET(client, &sSet);
	HANDLE hDataThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)waitDataConnect, msg, NULL, NULL);
	HANDLE hReadThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadServerResponse, &client, NULL, NULL);
	WaitForSingleObject(hDataThread, INFINITE);
	// 传输指令
	
	while (true)
	{
		if (dataConnectSock == INVALID_SOCKET)
		{
			std::cout << "数据连接建立失败！\n";
			break;
		}
		std::cout << "smftp>";
		char* cmdLine[3];
		char message[1024] = { 0 };
		std::cin >> cmd;
		cmdLine[0] = cmd;

		std::cin >> fileArg;
		cmdLine[1] = fileArg;

		std::cin >> fileArg2;
		cmdLine[2] = fileArg2;

		getchar();
		// 生成报文缓冲
		CombineMessage(3, cmdLine, message);
		// 发送报文
		int s = sizeof(message);
		if (send(client, message, sizeof(message), 0) == SOCKET_ERROR)
		{
			std::cout << "Occur SOCKET_ERROR\n";
			break;
		}
		//根据命令类型进行处理
		if (strcmp("put", cmd) == 0)
		{
			std::cout << "开始上传\n";
		
			if (putFile(fileArg) != 0)
			{
				break;
			}

		}
		else if (strcmp(SMFTP_GET, cmd) == 0)
		{
			std::cout << "开始下载\n";
			if (getFile(fileArg2) != 0)
			{
				break;
			}
		}
		else if (strcmp(SMFTP_EXIT, cmd) == 0)
		{
			std::cout << "程序即将退出\n";
			break;
		}
	}
	closesocket(client);
	return 0;
}
/*
	该协议(smftp)报文格式分为三段(Simple File Transfer Protocol)
	commendType:([int][string]) int类型的参数使用四个字节用来标识string长度；
	filename1([int][string]) 参数类型意义同上
	filename2([int][string]) 同上
*/
