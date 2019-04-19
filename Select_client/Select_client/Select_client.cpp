// Select_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrinfo hint, *addrResult;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_socktype = SOCK_STREAM;

	getaddrinfo("localhost", "8080", &hint, &addrResult);

	SOCKET sock = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	connect(sock, addrResult->ai_addr, addrResult->ai_addrlen);
	std::cout << "error " << GetLastError() << "\n";
	char sendBuf[1024] = { 0 };
	while (true) {
		std::cin >> sendBuf;
		int sendNum = send(sock, sendBuf, strlen(sendBuf), 0);
		if (sendNum == -1)
		{
			std::cout << "last error: " << GetLastError() << "\n";
		}
		std::cout << "last error: " << GetLastError() << "\n";
		std::cout << sendNum << "\n";
	}
	closesocket(sock);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
