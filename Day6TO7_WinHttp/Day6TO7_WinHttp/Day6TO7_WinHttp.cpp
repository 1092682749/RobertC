// Day6TO7_WinHttp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <winhttp.h>
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")
// 1
int main_1()
{
	DWORD data;
	DWORD dwSize = sizeof(DWORD);
	HINTERNET hSession = WinHttpOpen(L"A WinHTTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0
	);
	HINTERNET conn = NULL, req = NULL;
	bool res = false;
	if (hSession)
	{
		conn = WinHttpConnect(hSession, L"www.wingtiptoys.com",
			INTERNET_DEFAULT_HTTP_PORT, 0);
		if (conn)
		{
			req = WinHttpOpenRequest(conn, L"PUT", L"writetst.txt", NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				0);
		}
		if (req)
		{
			res = WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS,
				0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
		}
		if (!res)
		{
			printf("Error %d has occurred. \n", GetLastError());
		}
		if (req)
		{
			WinHttpCloseHandle(req);
		}
		if (conn)
		{
			WinHttpCloseHandle(conn);
		}
		if (WinHttpQueryOption(
			hSession,
			WINHTTP_OPTION_CONNECT_TIMEOUT,
			&data, &dwSize
		))
		{
			printf("Connection timeout: %u ms\n\n", data);
		}
		else
		{
			printf("Error %u in WinHttpQueryOption. \n", GetLastError());
		}
		WinHttpCloseHandle(hSession);
	}
	else
	{
		printf("Error %u in WinHttpQueryOption. \n", GetLastError());
	}
    std::cout << "Hello World!\n"; 
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
