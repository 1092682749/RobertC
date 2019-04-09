// SO_REUSE_TEST.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <json/json.h>

#pragma comment(lib, "Ws2_32.lib")

SOCKET sock;

void ReadThread(LPVOID lpParam)
{
	Json::Reader reader;
	Json::Value value;
	std::ostringstream os;
	char readBuf[1024] = { 0 };
	while (true)
	{
		sockaddr_in  pAdd;
		int len = sizeof(sockaddr_in);
		recvfrom(sock, readBuf, 1024, 0, (sockaddr*)&pAdd, &len);
		//printf("%s\n", readBuf);
		//os << readBuf;
		reader.parse(readBuf, value);
		if (value["msg"].asString().find("assistInfo"))
		{
			std::string destinationIP;
			destinationIP = value["destinationIP"].asString();
			int desPort;
			desPort = value["destinationPort"].asInt();
			os << desPort;
			sockaddr_in destinationInfo;
			//InetPton(AF_INET, (PCWSTR)destinationIP.c_str(), &destinationInfo.sin_addr.s_addr);
			printf("目的ip: %s\n", destinationIP.c_str());
			// destinationInfo.sin_port = htonl(desPort);
			addrinfo hi, *hii;
			memset(&hi, 0, sizeof(hi));
			hi.ai_family = AF_INET;
			getaddrinfo(destinationIP.c_str(), os.str().c_str(), &hi, &hii);
			// 协助握手包
			char assistPacket[] = "================assist================\n";
			sendto(sock, assistPacket, strlen(assistPacket), 0, hii->ai_addr, hii->ai_addrlen);
		}
		printf("recv:%s\n", readBuf);
	}
}
int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	addrinfo hint, *result;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_flags = AI_PASSIVE;
	hint.ai_protocol = IPPROTO_UDP;
	getaddrinfo("127.0.0.1", "7777", &hint, &result);
	//////////////////////////
	char host[255] = { 0 };
	gethostname(host, 255);
	std::cout << host << "\n";
	char name[225] = { 0 };
	addrinfo *reult;
	GetAddrInfoA("localhost", NULL, NULL, &reult);
	std::cout << reult->ai_addr << "\n";
	printf("%s\n", reult->ai_addr);
	//std::cout << h->h_name << "\n";
	// std::cout << "Error = " << GetLastError() << "\n";
	/////////////////////////////////////////
	PADDRINFOA current = result;
	while (current != NULL)
	{
		printf("%s\n", current->ai_addr->sa_data);
		current = current->ai_next;
	}
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	char optval = 1;
	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	// bind(sock, result->ai_addr, result->ai_addrlen);
	char bytes[] = "{\"name\":\"AAA\",\"type\":\"TYPE_REGISTER\"}";
	printf("%s\n", bytes);
	sendto(sock, bytes, strlen(bytes), 0, result->ai_addr, result->ai_addrlen);
	HANDLE hReadThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadThread, NULL, 0, NULL);
	char outBuf[] = "{\"type\":\"assist\",\"destinationName\":\"AAA\"}";
	
	sendto(sock, outBuf, strlen(outBuf), 0, result->ai_addr, result->ai_addrlen);
    // std::cout << "Hello World!\n"; 
	WaitForSingleObject(hReadThread, INFINITE);
	return 0;
}