// Test_Chinese.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Sock.h"
#include <tchar.h>

int main()
{
	SOCKET sock;
	char port[] = "8080";
	char buf[1024];
	TCHAR dBuf[1024];
	memset(buf, 0, 1024);
	memset(dBuf, 0, 1024);
	WSADATA wsaData;
	addrinfo *addrResult;
	MySocketUtils::SocketFactory::GetSocket(sock, port, wsaData, addrResult);

	if (sock == INVALID_SOCKET)
	{
		return -1;
	}

	bind(sock, addrResult->ai_addr, addrResult->ai_addrlen);
	listen(sock, 10);
	SOCKET client = accept(sock, NULL, NULL);
	int readNum = recv(client, buf, 1024, 0);
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, dBuf, readNum);
	std::cout << buf << "\n";
	printf("%s\n", buf);
	char message[] = "中文消息";
	send(client, (char*)message, sizeof(message), 0);
	closesocket(client);
	return 0;
}
