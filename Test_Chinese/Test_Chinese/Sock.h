#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

namespace MySocketUtils
{
	class SocketFactory
	{
	public:
		static WORD version;
		static int socketType;
		static int protocol;
		static char *port;
		static char *ip;
		static int family;
		static int flags;
		static int GetSocket(SOCKET &sock, char *port, WSADATA &wsaData, addrinfo* &addrResult)
		{
			WSAStartup(version, &wsaData);
			addrinfo ai;
			memset(&ai, 0, sizeof(addrinfo));
			ai.ai_flags = flags;
			ai.ai_family = family;
			ai.ai_protocol = protocol;
			ai.ai_socktype = socketType;
			SocketFactory::port = port;
			int retCode = getaddrinfo(NULL, port, &ai, &addrResult);
			sock = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
			return 0;
		}
	};
	WORD SocketFactory::version = MAKEWORD(2, 2);
	int SocketFactory::protocol = IPPROTO_TCP;
	int SocketFactory::socketType = SOCK_STREAM;
	int SocketFactory::family = AF_INET;
	int SocketFactory::flags = AI_PASSIVE;
	char* SocketFactory::port = NULL;
	
}