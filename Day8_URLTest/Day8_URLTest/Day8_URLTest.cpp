// Day8_URLTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "user32.lib")

int main()
{
	URL_COMPONENTS urlComp;
	LPCWSTR pwszUrl1 = L"https://dyzhello.club/static/page/readArticle.html?id=15448";
	DWORD dwUrlLen = 0;
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);
	urlComp.dwSchemeLength = (DWORD)-1;
	urlComp.dwHostNameLength = (DWORD)-1;
	urlComp.dwUrlPathLength = (DWORD)-1;
	urlComp.dwExtraInfoLength = (DWORD)-1;
	if (!WinHttpCrackUrl(pwszUrl1, (DWORD)wcslen(pwszUrl1), 0, &urlComp))
	{
		printf("Error %u in WinHttpCrackUrl. \n", GetLastError());
	}
	else
	{
		wchar_t lpExtra[] = L"?id=15450";
		urlComp.lpszExtraInfo = lpExtra;
		WinHttpCreateUrl(&urlComp, 0, NULL, &dwUrlLen);
		LPWSTR pwszUrl2 = new WCHAR[dwUrlLen];
		if (!WinHttpCreateUrl(&urlComp, 0, pwszUrl2, &dwUrlLen))
		{
			printf("Error %u in WinHttpCreaterUrl. \n", GetLastError());
		}
		else
		{
			printf("Old URL: %SnNew URL: %S\n", pwszUrl1, pwszUrl2);
		}
	}
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
