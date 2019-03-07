

#include "pch.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

int main()
{
	std::cout << "I'm exe3" << std::endl;
	LPTSTR lpszVariable;
	LPTCH lpvEnv;
	lpvEnv = GetEnvironmentStrings();

	if (lpvEnv == NULL)
	{
		printf("GetEnvironmentStrings failed(%d)\n", GetLastError());
		return 0;
	}

	lpszVariable = lpvEnv;

	while (*lpszVariable)
	{
		_tprintf(TEXT("%s\n"), lpszVariable);
		lpszVariable += lstrlen(lpszVariable) + 1;
	}
	FreeEnvironmentStrings(lpvEnv);

	return 0;
}