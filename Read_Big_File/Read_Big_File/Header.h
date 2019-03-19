#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace MySocketUtils
{
	enum EndType
	{
		END_TYPE_SERVER,
		END_TYPE_CLIENT
	};
	class SocketFactory
	{
	public:
		static WORD version;
		static int socketType;
		static int protocol;
		static char *port;
		static char *ip;
		static char *host;
		static int family;
		static int flags;
		// 获取监听socket
		static int GetSocket(SOCKET &sock, const char *port, addrinfo *&addrResult)
		{
			WSADATA wsaData;
			WSAStartup(version, &wsaData);
			addrinfo ai;
			memset(&ai, 0, sizeof(addrinfo));
			ai.ai_flags = flags;
			ai.ai_family = family;
			ai.ai_protocol = protocol;
			ai.ai_socktype = socketType;

			int retCode = getaddrinfo(host, port, &ai, &addrResult);
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
		// 获取连接socket
		static int GetClientSocket(SOCKET &sock, char* host, const char *port, addrinfo* &addrResult)
		{
			SocketFactory::host = host;
			return SocketFactory::GetSocket(sock, port, addrResult);
		}
	};
	WORD SocketFactory::version = MAKEWORD(2, 2);
	int SocketFactory::protocol = IPPROTO_TCP;
	int SocketFactory::socketType = SOCK_STREAM;
	int SocketFactory::family = AF_INET;
	int SocketFactory::flags = AI_PASSIVE;
	char* SocketFactory::port = NULL;
	char* SocketFactory::host = NULL;
}
