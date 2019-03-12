
#include "pch.h"
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "winhttp.lib")

DWORD ChooseAuthScheme(DWORD dwSupportedSchemes)
{
	//  It is the server's responsibility only to accept 
	//  authentication schemes that provide a sufficient
	//  level of security to protect the servers resources.
	//
	//  The client is also obligated only to use an authentication
	//  scheme that adequately protects its username and password.
	//
	//  Thus, this sample code does not use Basic authentication  
	//  becaus Basic authentication exposes the client's username
	//  and password to anyone monitoring the connection.

	if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_NEGOTIATE)
		return WINHTTP_AUTH_SCHEME_NEGOTIATE;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_NTLM)
		return WINHTTP_AUTH_SCHEME_NTLM;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_PASSPORT)
		return WINHTTP_AUTH_SCHEME_PASSPORT;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_DIGEST)
		return WINHTTP_AUTH_SCHEME_DIGEST;
	else
		return 0;
}

struct SWinHttpSampleGet
{
	LPCWSTR szServer;
	LPCWSTR szPath;
	BOOL fUseSSL;
	LPCWSTR szServerUsername;
	LPCWSTR szServerPassword;
	LPCWSTR szProxyUsername;
	LPCWSTR szProxyPassword;
};

void WinHttpAuthSample(IN SWinHttpSampleGet *pGetRequest)
{
	DWORD dwStatusCode = 0;
	DWORD dwSupportedSchemes;
	DWORD dwFirstScheme;
	DWORD dwSelectedScheme;
	DWORD dwTarget;
	DWORD dwLastStatus = 0;
	DWORD dwSize = sizeof(DWORD);
	BOOL  bResults = FALSE;
	BOOL  bDone = FALSE;

	DWORD dwProxyAuthScheme = 0;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	INTERNET_PORT nPort = (pGetRequest->fUseSSL) ?
		8443 :
		8080;

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession,
			pGetRequest->szServer,
			nPort, 0);

	// Create an HTTP request handle.
	if (hConnect)
	{
		PCTSTR rgpszAcceptTypes[] = { TEXT("text/*"), NULL};
		hRequest = WinHttpOpenRequest(hConnect,
			L"GET",
			pGetRequest->szPath,
			*rgpszAcceptTypes,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			(pGetRequest->fUseSSL) ?
			WINHTTP_FLAG_SECURE : 0);
	}
		

	// Continue to send a request until status code 
	// is not 401 or 407.
	if (hRequest == NULL)
		bDone = TRUE;

	while (!bDone)
	{
		//  If a proxy authentication challenge was responded to, reset
		//  those credentials before each SendRequest, because the proxy  
		//  may require re-authentication after responding to a 401 or  
		//  to a redirect. If you don't, you can get into a 
		//  407-401-407-401- loop.
		if (dwProxyAuthScheme != 0)
			bResults = WinHttpSetCredentials(hRequest,
				WINHTTP_AUTH_TARGET_PROXY,
				dwProxyAuthScheme,
				pGetRequest->szProxyUsername,
				pGetRequest->szProxyPassword,
				NULL);
		// Send a request.
		bResults = WinHttpSendRequest(hRequest,
			//WINHTTP_NO_ADDITIONAL_HEADERS,
			L"username:dyz",
			-1L,
			WINHTTP_NO_REQUEST_DATA,
			0,
			0,
			0);

		// End the request.
		if (bResults)
			bResults = WinHttpReceiveResponse(hRequest, NULL);

		// Resend the request in case of 
		// ERROR_WINHTTP_RESEND_REQUEST error.
		if (!bResults && GetLastError() == ERROR_WINHTTP_RESEND_REQUEST)
			continue;

		// Check the status code.
		if (bResults)
			bResults = WinHttpQueryHeaders(hRequest,
				WINHTTP_QUERY_STATUS_CODE |
				WINHTTP_QUERY_FLAG_NUMBER,
				NULL,
				&dwStatusCode,
				&dwSize,
				NULL);

		if (bResults)
		{
			switch (dwStatusCode)
			{
			case 200:
				// The resource was successfully retrieved.
				// You can use WinHttpReadData to read the 
				// contents of the server's response.
				printf("The resource was successfully retrieved.\n");
				bDone = TRUE;
				break;

			case 401:
				// The server requires authentication.
				printf(" The server requires authentication. Sending credentials...\n");

				// Obtain the supported and preferred schemes.
				bResults = WinHttpQueryAuthSchemes(hRequest,
					&dwSupportedSchemes,
					&dwFirstScheme,
					&dwTarget);

				// Set the credentials before resending the request.
				if (bResults)
				{
					dwSelectedScheme = ChooseAuthScheme(dwSupportedSchemes);

					if (dwSelectedScheme == 0)
						bDone = TRUE;
					else
						bResults = WinHttpSetCredentials(hRequest,
							dwTarget,
							dwSelectedScheme,
							pGetRequest->szServerUsername,
							pGetRequest->szServerPassword,
							NULL);
				}

				// If the same credentials are requested twice, abort the
				// request.  For simplicity, this sample does not check
				// for a repeated sequence of status codes.
				if (dwLastStatus == 401)
					bDone = TRUE;

				break;

			case 407:
				// The proxy requires authentication.
				printf("The proxy requires authentication.  Sending credentials...\n");

				// Obtain the supported and preferred schemes.
				bResults = WinHttpQueryAuthSchemes(hRequest,
					&dwSupportedSchemes,
					&dwFirstScheme,
					&dwTarget);

				// Set the credentials before resending the request.
				if (bResults)
					dwProxyAuthScheme = ChooseAuthScheme(dwSupportedSchemes);

				// If the same credentials are requested twice, abort the
				// request.  For simplicity, this sample does not check 
				// for a repeated sequence of status codes.
				if (dwLastStatus == 407)
					bDone = TRUE;
				break;

			default:
				// The status code does not indicate success.
				printf("Error. Status code %d returned.\n", dwStatusCode);
				bDone = TRUE;
			}
		}

		// Keep track of the last status code.
		dwLastStatus = dwStatusCode;

		// If there are any errors, break out of the loop.
		if (!bResults)
			bDone = TRUE;
	}

	// Report any errors.
	if (!bResults)
	{
		DWORD dwLastError = GetLastError();
		printf("Error %d has occurred.\n", dwLastError);
	}

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}
int main()
{
	SWinHttpSampleGet getRequest;
	//getRequest = *((SWinHttpSampleGet*)malloc(sizeof(SWinHttpSampleGet)));
	getRequest.fUseSSL = true;
	getRequest.szPath = L"/test";
	getRequest.szServer = L"127.0.0.1";
	getRequest.szServerUsername = L"123";
	getRequest.szProxyPassword = L"123";
	//WinHttpAuthSample(&getRequest);
	char (*ip)[10];
	char *lp;
	lp = (char*)malloc(sizeof(char) * 10);
	memset(lp, 'u', 10);
	ip = (char(*)[10])lp;
	lp[1] = '1';
	lp[0] = '0';
	//char(*ipp)[10][10];
	(*ip)[2] = '2';
	std::cout << *ip << "\n";
	std::cout << lp << "======================\n";
	std::cout << &(*ip) << "\n" << &lp << "\n";
	printf("%x\n", lp);
	printf("%x\n", ip);
	printf("%x\n", *ip);
	printf("%c", *((*ip)));
	printf("%c", *lp);
	char *p;
	char **pp;
	return 0;
}