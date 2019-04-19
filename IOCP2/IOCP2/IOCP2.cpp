// IOCP2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
HANDLE gIocp;
typedef struct IO_Data
{
	OVERLAPPED Overlapped;
	WSABUF DataBuf;
	DWORD number;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

void initWSA()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
struct Iocp_key {
	SOCKET s;
};
void WorkThread(LPVOID lpParam)
{
	DWORD readNumer;
	Iocp_key *key;
	IO_Data *io_data;
	while (true)
	{
		// std::cout << "TTTT Error " << GetLastError() << "\n";
		if (GetQueuedCompletionStatus(gIocp, &readNumer, (PULONG_PTR)&key, (LPOVERLAPPED *)&io_data, 10))
		{
			std::cout << "TTTT Error " << GetLastError() << "\n";
			std::cout << "buf is " << io_data->DataBuf.buf << "\n";
			// 第二个参数
			DWORD flag = 0;
			DWORD dwRecv = 0;
		

			
			WSARecv(key->s, &io_data->DataBuf, 1, &io_data->number, &flag, &io_data->Overlapped, NULL);
			if (key->s == INVALID_SOCKET || key->s == NULL)
			{
				std::cout << "seocket invalid\n";
			}
			std::cout << "TTTT Error " << GetLastError() << "\n";
		}
		if (GetLastError()!=258)
		{
			std::cout << "TTTT Error " << GetLastError() << "\n";
		}
	}
	
}
int main()
{
	gIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == gIocp)
	{
		std::cout << GetLastError() << "\n";
		return -1;
	}
	initWSA();
	addrinfo hint, *addrResult;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_socktype = SOCK_STREAM;
	getaddrinfo(NULL, "8080", &hint, &addrResult);
	SOCKET sock = WSASocket(AF_INET, addrResult->ai_socktype, addrResult->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	std::cout << "error " << GetLastError() << "\n";
	for (int i = 0; i < 4; i++)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThread, NULL, 0, NULL);
	}
	std::cout << "error " << GetLastError() << "\n";
	bind(sock, addrResult->ai_addr, addrResult->ai_addrlen);
	std::cout << "error " << GetLastError() << "\n";
	listen(sock, 10);
	std::cout << "error " << GetLastError() << "\n";
	sockaddr_in clientAddr;
	// WSAAccept(sock, (sockaddr*)&clientAddr, sizeof(sockaddr_in));
	int len = 32;
	while (true)
	{
		std::cout << "等待开始\n";
		SOCKET clientSock = accept(sock, (sockaddr*)&clientAddr, &len);
		//SOCKET clientSock = accept(sock, NULL, NULL);
		std::cout << "ccccc\n";
		Iocp_key accpetKey{ clientSock };
		CreateIoCompletionPort((HANDLE)clientSock, gIocp, (ULONG_PTR)(&accpetKey), NULL);
		char buf[1024];
		WSABUF wsaBuf{ 1024, buf };
		DWORD Flags = 0;
		DWORD dwRecv = 0;
		OVERLAPPED overLapped;
		IO_Data io_data;
		memset(&io_data, 0, sizeof(IO_Data));

		io_data.DataBuf = wsaBuf;
		WSARecv(clientSock, &io_data.DataBuf, 1, &dwRecv, &Flags, &io_data.Overlapped, NULL);
	}
	
	return 0;
}
