
#include "pch.h"
#include <windows.h>
#include <iostream>
   //Comment this line to user wininet.
#ifdef USE_WINHTTP
    #include <winhttp.h>
    #pragma comment(lib, "winhttp.lib")
#else
    #include <wininet.h>
    #pragma comment(lib, "wininet.lib")
#endif





int main()
{
	DWORD statusCode, size;
	char *outBuffer = NULL;
	HINTERNET hInternetRoot;
	hInternetRoot = InternetOpen(TEXT("WinInet Example"),
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnect(hInternetRoot, L"www.dyzhello.club", 443,
		NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	PCTSTR rgpszAcceptTypes[] = { L"text/*", NULL}; // 客户端接收的数据类型
	HINTERNET request = HttpOpenRequest(hConnect, L"GET", L"/articleList", NULL, L"", rgpszAcceptTypes, INTERNET_FLAG_SECURE, 0);
	BOOL success = HttpSendRequest(request, NULL, NULL, NULL,
		0);
	if (success)
	{
		HttpQueryInfo(request, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&statusCode, &size, NULL);
		if (statusCode == 200)
		{
			InternetQueryDataAvailable(request, &size, 0, 0);
			if (size > 0)
			{
				DWORD readNumber;
				outBuffer = (char*)malloc(size);
				memset(outBuffer, 0, size);
				InternetReadFile(request, (LPVOID)outBuffer,
					size, &readNumber);
				std::cout << "读取的字节数为：" << readNumber << std::endl;
				std::cout << "content is: \n" << outBuffer << std::endl;
			}
		}
	}
    std::cout << "Hello World!\n"; 
}
