// Day4_socket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
int main()
{
	SOCKET ServeSocket, s2;
	addrinfo ai, *result;
	BOOL optFlag = TRUE;
	ai.ai_family = AF_INET;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_protocol = IPPROTO_TCP;
	ai.ai_flags = AI_PASSIVE;
	int ret = 0;
	ret = getaddrinfo(NULL, "9090", &ai, &result);
	const char *hostname = "127.0.0.1";
	s2 = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	ServeSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	bind(ServeSocket, (sockaddr*)hostname, sizeof(hostname));


	listen(ServeSocket, SOMAXCONN);

	bind(s2, (sockaddr*)hostname, sizeof(hostname));

	SOCKET clientSocket;
	clientSocket = accept(ServeSocket, NULL, NULL);
	
	if (clientSocket != INVALID_SOCKET)
	{
		
	}
	setsockopt(ServeSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)TRUE, sizeof(BOOL));

    std::cout << "Hello World!\n"; 
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
