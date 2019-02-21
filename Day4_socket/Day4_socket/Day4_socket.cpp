// Day4_socket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <rpc.h>
#include <ntdsapi.h>
#include <stdio.h>
#include <tchar.h>
#include <mstcpip.h>
#pragma comment(lib, "Ws2_32.lib")
// link with fwpuclnt.lib for Winsock secure socket extensions
#pragma comment(lib, "fwpuclnt.lib")

// link with ntdsapi.lib for DsMakeSpn function
#pragma comment(lib, "ntdsapi.lib")

#define RECV_DATA_BUF_SIZE 256

int SecureTcpConnect(IN const struct sockaddr *serverAddr,
	IN ULONG serverAddrLen,
	IN const wchar_t *serverSPN,
	IN const SOCKET_SECURITY_SETTINGS *securitySettings,
	IN ULONG settingsLen)
{
	int iResult = 0;
	int sockErr = 0;
	SOCKET sock = INVALID_SOCKET;
	WSABUF wsaBuf = { 0 };
	char cs[] = { '1', '2', '3', '4', '5', '6', '7', '8' };
	char *dataBuf = cs;
	DWORD  bytesSent = 0;
	char recvBuf[RECV_DATA_BUF_SIZE] = { 0 };
	DWORD bytesRecvd = 0;
	DWORD flags = 0;
	SOCKET_PEER_TARGET_NAME *peerTargetName = NULL;
	DWORD serverSpnStringLen = (DWORD)wcslen(serverSPN);
	DWORD peerTargetNameLen = sizeof(SOCKET_PEER_TARGET_NAME) +
		(serverSpnStringLen * sizeof(wchar_t));
	sock = WSASocket(serverAddr->sa_family, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (sock == INVALID_SOCKET)
	{
		iResult = WSAGetLastError();
		wprintf(L"WSASocket returned errot %ld\n", iResult);
		goto cleanup;
	}
	sockErr = WSASetSocketSecurity(sock, securitySettings, settingsLen, NULL, NULL);
	if (sockErr == SOCKET_ERROR)
	{
		iResult == WSAGetLastError();
		wprintf(L"WSASetSocketSecurity returned error %ld\n", iResult);
		goto cleanup;
	}
	peerTargetName = (SOCKET_PEER_TARGET_NAME*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, peerTargetNameLen);
	if (!peerTargetName)
	{
		iResult = ERROR_NOT_ENOUGH_MEMORY;
		wprintf(L"Out of memory\n");
		goto cleanup;
	}
	peerTargetName->SecurityProtocol = securitySettings->SecurityProtocol;
	peerTargetName->PeerTargetNameStringLen = serverSpnStringLen;
	RtlCopyMemory((BYTE*)peerTargetName->AllStrings, (BYTE*)serverSPN, serverSpnStringLen * sizeof(wchar_t));
	sockErr = WSASetSocketPeerTargetName(sock, peerTargetName, peerTargetNameLen, NULL, NULL);
	if (sockErr == SOCKET_ERROR)
		iResult = WSAGetLastError(); {
		wprintf(L"WSAConnect returned error %ld\n", iResult);
		goto cleanup;
	}
	wprintf(L"Awcure connetion established to the server\n");
	wsaBuf.len = (ULONG)strlen(dataBuf);
	wsaBuf.buf = dataBuf;
	sockErr = WSASend(sock, &wsaBuf, 1, &bytesSent, 0, NULL, NULL);
	if (sockErr == SOCKET_ERROR)
	{
		iResult = WSAGetLastError();
		wprintf(L"WSASend returned error %ld\n", iResult);
		goto cleanup;
	}
	wprintf(L"Send %d bytes of data to the server\n", bytesSent);
	wsaBuf.len = RECV_DATA_BUF_SIZE;
	wsaBuf.buf = recvBuf;
	sockErr = WSARecv(sock, &wsaBuf, 1 & bytesRecvd, &flags, NULL, NULL);
	if (sockErr == SOCKET_ERROR)
	{
		iResult = WSAGetLastError();
		wprintf(L"WSARecv returned error %ld\n", iResult);
		goto cleanup;
	}
	wprintf(L"Received %d bytes of data from the server\n", bytesRecvd);
cleanup:
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}
	if (peerTargetName)
	{
		HeapFree(GetProcessHeap(), 0, peerTargetName);
	}
	return iResult;
}
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
