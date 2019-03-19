// Read_Big_File_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "MySocketUtils.h"

#define READBUFSIZE 1024
#define INTSIZE 4

SOCKET client;
char cmd[1024] = { 0 };
char fileArg[200] = { 0 }, fileArg2[200] = { 0 };
SOCKET dataListenSock = INVALID_SOCKET;
SOCKET dataConnectSock = INVALID_SOCKET;

void waitDataConnect(LPVOID lpParam)
{
	char* msg = (char*)lpParam;
	char data[1024];
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
	std::cout << "Success Data Connect!" << data << "\n";
}
void ReadServerResponse(LPVOID lpParam)
{
	char outBuf[1024] = { 0 };
	SOCKET s = (SOCKET)lpParam;
	while (true)
	{
		if (recv(s, outBuf, 1024, 0) == SOCKET_ERROR)
		{
			break;
		}
		std::cout << outBuf << "\n";
	}
}
void putFile(const char* fileName)
{
	TCHAR wFileName[1024] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, fileName, 1024, wFileName, 1024);
	HANDLE hUploadFile = CreateFile(wFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char readFileBuf[READBUFSIZE];
	DWORD readNumber = 0;

	do
	{
		if (!ReadFile(hUploadFile, readFileBuf, READBUFSIZE, &readNumber, NULL))
		{
			std::cout << "Read Faild!\n";
			break;
		}
		send(dataConnectSock, readFileBuf, readNumber, 0);
	} while (readNumber > 0);
}
void getFile(char* &fileName)
{

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
	addrinfo *ai;
	char host[] = "127.0.0.1", msg[] = "9000";
	int retCode = 0;
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
	HANDLE hDataThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)waitDataConnect, msg, NULL, NULL);
	HANDLE hReadThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadServerResponse, &client, NULL, NULL);
	// 传输指令
	while (true)
	{
		WaitForSingleObject(hDataThread, INFINITE);
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
		CombineMessage(3, cmdLine, message);
		
		int cmpCode = strcmp("put", cmd);
		if (strcmp("put", cmd) == 0)
		{
			std::cout << "我将发送一个put命令\n";
			cmd[3] = ' ';
			send(client, message, sizeof(message), 0);
			putFile(fileArg);

		}
		else if (strcmp("get", cmd) == 0)
		{
			std::cout << "我将发送一个get命令\n";
			cmd[3] = ' ';
			CopyMemory(cmd + 4, fileArg, strlen(fileArg));
			if (send(client, message, 1024, 0) == SOCKET_ERROR)
			{
				std::cout << "Occur SOCKET_ERROR\n";
				break;
			}
		}
		else if (strcmp("ext", cmd) == 0)
		{
			std::cout << "程序即将退出\n";
			cmd[3] = ' ';
			CopyMemory(cmd + 4, fileArg, strlen(fileArg));
			send(client, message, 1024, 0);
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
