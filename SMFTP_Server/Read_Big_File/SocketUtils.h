#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

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
		static int GetSocket(SOCKET &sock, const char* port, addrinfo* &addrResult);
		// 获取连接socket
		static int GetClientSocket(SOCKET &sock, char* host, const char *port, addrinfo* &addrResult);
	};
}


