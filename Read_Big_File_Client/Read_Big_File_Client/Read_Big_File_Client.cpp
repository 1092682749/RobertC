// Read_Big_File_Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "MySocketUtils.h"

char cmd[1024] = { 0 };
char fileArg[200] = { 0 };
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
int main()
{
	SOCKET client;
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
		std::cin >> cmd;
		std::cin >> fileArg;
		getchar();
		
		int cmpCode = strcmp("put", cmd);
		if (strcmp("put", cmd) == 0)
		{
			std::cout << "我将发送一个put命令\n";
			cmd[3] = ' ';
			CopyMemory(cmd + 4, fileArg, strlen(fileArg));
			send(client, cmd, 200, 0);
		}
		else if (strcmp("get", cmd) == 0)
		{
			std::cout << "我将发送一个get命令\n";
			cmd[3] = ' ';
			CopyMemory(cmd + 4, fileArg, strlen(fileArg));
			send(client, cmd, 200, 0);
		}
		else if (strcmp("ext", cmd) == 0)
		{
			std::cout << "程序即将退出\n";
			cmd[3] = ' ';
			CopyMemory(cmd + 4, fileArg, strlen(fileArg));
			send(client, cmd, 200, 0);
			break;
		}
	}
	closesocket(client);
	return 0;
}
