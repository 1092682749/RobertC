
#include "pch.h"
#include <Windows.h>
#include <strsafe.h>
#include <iostream>

#define BUFSIZE 4096

int main(int argc, char* argv[])
{
	TCHAR chNewEnv[BUFSIZ];
	LPTSTR lpszCurrentVariable;
	DWORD dwFlags = 0;
	TCHAR szAppName[] = TEXT("C:/Users/i-robert/source/repos/Changing_Environment_Variables_3/Debug/Changing_Environment_Variables_3.exe");
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL fSuccess;

	lpszCurrentVariable = (LPTSTR)chNewEnv;
	if (FAILED(StringCchCopy(lpszCurrentVariable, BUFSIZE, TEXT("MySetting=A"))))
	{
		printf("String copy failed\n");
		return FALSE;
	}

	lpszCurrentVariable += lstrlen(lpszCurrentVariable) + 1;

	if (FAILED(StringCchCopy(lpszCurrentVariable, BUFSIZE, TEXT("MyVersion=2"))))
	{
		printf("String copy faild\n");
		return FALSE;
	}
	lpszCurrentVariable += lstrlen(lpszCurrentVariable) + 1;
	*lpszCurrentVariable = 0;

	SecureZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

#ifdef UNICODE
	dwFlags = CREATE_UNICODE_ENVIRONMENT;
#endif // UNICODE
	fSuccess = CreateProcess(szAppName, NULL, NULL, NULL, TRUE, dwFlags, chNewEnv, NULL, &si, &pi);
	if (!fSuccess)
	{
		printf("CreateProcess failed(%d)\n", GetLastError());
		return FALSE;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	return 0;
}