// Get_IP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	sockaddr_in *sinp;
	char hostname[255] = { 0 };
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_family = AF_INET;
	//hint.ai_flags = AI_PASSIVE;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	gethostname(hostname, 255);
	std::cout << GetLastError() << "\n";
	// gethostbyname(hostname);
	getaddrinfo("localhost", NULL, &hint, &ailist);
	char ipbuf[16] = { 0 };
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		aip->ai_family == AF_INET;
		sinp = (sockaddr_in *)aip->ai_addr;    //为什么是for 循环 ，先向下看
		int i;
		sinp->sin_addr.s_addr;
		printf("%s\n", inet_ntop(AF_INET, sinp, ipbuf, 16));
		printf("%s\n", ipbuf);
		printf(" \n");
	}
	return 0;
}
