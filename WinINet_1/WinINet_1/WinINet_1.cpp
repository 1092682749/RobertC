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
	DWORD statusCode = 200, size = 0;
	LPCSTR outBuffer = NULL;
	HINTERNET hInternetRoot;
	hInternetRoot = InternetOpen(TEXT("WinInet Example"),
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = InternetConnect(hInternetRoot, L"wuxia.duowan.com", 80,
		NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	LPCWSTR type = L"text/*";
	HINTERNET request = HttpOpenRequest(hConnect, L"GET", L"/1902/413722262060.html", NULL, L"", NULL, 0, 0);
	BOOL success = HttpSendRequest(request, NULL, 0, NULL, 0);
	if (success)
	{
		HttpQueryInfo(request, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&statusCode, &size, NULL);
		HttpQueryInfo(request, HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID)outBuffer, &size, NULL);
		outBuffer = new char[size];
		HttpQueryInfo(request, HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID)outBuffer, &size, NULL);
		std::cout << outBuffer;
		if (statusCode == 200)
		{
			InternetQueryDataAvailable(request, &size, 0, 0);
			if (size > 0)
			{
				DWORD readNumber;
				outBuffer = (char*)malloc(size);
				memset((void *)outBuffer, 0, size);
				InternetReadFile(request, (LPVOID)outBuffer,
					size, &readNumber);
				std::cout << "读取的字节数为：" << readNumber << std::endl;
				std::cout << "content is: \n" << outBuffer << std::endl;
			}
		}
	}
	
    std::cout << "Hello World!\n"; 
}
