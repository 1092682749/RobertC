// Reptile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>
#include <sstream>
#pragma comment(lib, "wininet.lib")

HANDLE hRoot;


std::string GetHtml(const TCHAR* url)
{
	DWORD codeStatus = 0, size = 10240, readNumber = 0;
	// 请求接受响应类型
	PCTSTR rgpszAcceptTypes[] = { TEXT("text/*"), NULL };
	// 响应缓冲
	TCHAR response[102400] = { 0 };
	char *readBuf;
	std::ostringstream os;
	// 打开根句柄
	hRoot = InternetOpenA("MY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hRoot == NULL)
	{
		std::cout << "hRoot invalid\n";
		return std::string("error");
	}
	// 打开连接句柄
	HANDLE hConnect = InternetConnect(hRoot, url, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, 1);
	if (hConnect == NULL)
	{
		std::cout << "hConnect invalid\n";
		return std::string("error");
	}
	// 创建请求句柄
	HANDLE hRequestPage = HttpOpenRequest(hConnect, L"GET", NULL, NULL, NULL, rgpszAcceptTypes, INTERNET_FLAG_SECURE, 2);
	if (hRequestPage == NULL)
	{
		std::cout << "hRequestPage invalid\n";
		return std::string("error");
	}
	// 发送请求
	if (HttpSendRequest(hRequestPage, NULL, NULL, NULL, 0))
	{
		// 查询响应头行
		if (HttpQueryInfo(hRequestPage, HTTP_QUERY_RAW_HEADERS_CRLF, response, &size, NULL))
		{
			_tprintf(L"%s", response);
			InternetQueryDataAvailable(hRequestPage, &size, 0, 0);
			if (size > 0)
			{
				readBuf = new char[size];
				do
				{
					if (!InternetReadFile(hRequestPage, readBuf, size, &readNumber))
					{
						std::cout << "read faild\n";
						return std::string("error");
					}
					os << readBuf;
				} while (readNumber > 0);
				std::cout << os.str() << "\n";
			}
		}
		std::cout << GetLastError() << "\n";
	}
	return os.str();
}
void DownImage(const char* imgUrl, const char* name)
{
	std::string path("D:/a/bilibiliImg/");
	path.append(name);
	char readBuf[1024] = { 0 };
	DWORD readNumber = 0;
	HANDLE hOpenUrl = InternetOpenUrlA(hRoot, imgUrl, NULL, NULL, 0, 0);
	HANDLE hFile = CreateFileA(path.data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "handle valid\n";
		return;
	}
	do
	{
		if (!InternetReadFile(hOpenUrl, readBuf, 1024, &readNumber))
		{
			std::cout << "无法读取\n";
			return;
		}
		std::cout << ".";
		WriteFile(hFile, readBuf, readNumber, NULL, NULL);
	} while (readNumber > 0);
	
}
int main()
{
	std::string content;
	content = GetHtml(L"www.bilibili.com");
	std::regex imgRegex("(ht|f)tp(s?)\\://([-A-Za-z0-9+&@#/%?=~_|!:,.;]*?(jpg|jpeg|gif|png))");
	std::smatch result;
	while (std::regex_search(content, result, imgRegex))
	{
		std::cout << result[0];
		std::string url = result[0];
		std::string fullName = result[3];
		int poi = fullName.find_last_of('/');
		DownImage(url.data(), fullName.substr(poi + 1).data());
		std::cout << "\n";
		content = result.suffix().str();
	}
	return 0;
}
