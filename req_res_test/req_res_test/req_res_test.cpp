// req_res_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class A {
	int value;
public:
	A(int v)
	{
		this->value = v;
	}
	int getA() const
	{
		return value;
	}
	A operator+(const A &a)
	{
		return (this->value + a.getA());
	}
};
int main()
{
	A a(1), b(2);
	A c = a + b;
	std::cout << c.getA();
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	addrinfo hint, *result;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_socktype = SOCK_STREAM;
	getaddrinfo(NULL, "8080", &hint, &result);
	SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(sock, result->ai_addr, result->ai_addrlen);
	listen(sock, 10);
	SOCKET conSock = accept(sock, NULL, NULL);
	std::cout << "连接成功,等待接收数据\n";
	char readBuf[1024] = { 0 };
	char outBuf[1024] = { 0 };
	int readNumber = 0;
	while (true)
	{
		
		readNumber = recv(conSock, readBuf, 1024, 0);
		std::cout << "接收数据\n" << readBuf << "\n";
		send(conSock, readBuf, readNumber, 0);
	}
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
