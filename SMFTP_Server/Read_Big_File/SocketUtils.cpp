#include "SocketUtils.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

WORD MySocketUtils::SocketFactory::version = MAKEWORD(2, 2);
int MySocketUtils::SocketFactory::protocol = IPPROTO_TCP;
int MySocketUtils::SocketFactory::socketType = SOCK_STREAM;
int MySocketUtils::SocketFactory::family = AF_INET;
int MySocketUtils::SocketFactory::flags = AI_PASSIVE;
char* MySocketUtils::SocketFactory::port = NULL;
char* MySocketUtils::SocketFactory::host = NULL;

int MySocketUtils::SocketFactory::GetSocket(SOCKET &sock, const char* port, addrinfo* &addrResult)
{
	static BOOL loaded = FALSE;
	if (!loaded)
	{
		loaded = TRUE;
		WSADATA wsaData;
		WSAStartup(version, &wsaData);
	}

	addrinfo ai;
	memset(&ai, 0, sizeof(addrinfo));
	ai.ai_flags = flags;
	ai.ai_family = family;
	ai.ai_protocol = protocol;
	ai.ai_socktype = socketType;

	int retCode = getaddrinfo((PCSTR)host, port, &ai, &addrResult);
	if (retCode != 0)
	{
		return -2;
	}
	sock = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		return -1;
	}
	return 0;
}

int MySocketUtils::SocketFactory::GetClientSocket(SOCKET &sock, char* host, const char *port, addrinfo* &addrResult)
{
	MySocketUtils::SocketFactory::host = host;
	return MySocketUtils::SocketFactory::GetSocket(sock, port, addrResult);
}
