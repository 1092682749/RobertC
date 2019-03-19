// str_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

int main()
{
	char s1[10] = { 0 }, s2[10] = { 0 }, s3[10] = { 0 }, s[1024] = { 0 };
	std::cin >> s1;
	std::cin >> s2;
	std::cin >> s3;
	getchar();
	int len1 = strlen(s1);
	CopyMemory(s, &len1, 4);
	CopyMemory(s + 4, s1, sizeof(s1));

	int len2 = strlen(s2);
	CopyMemory(s + len1 + 4, &len2, 4);
	CopyMemory(s + len1 + 4 + 4, s2, sizeof(s2));

	int len3 = strlen(s3);
	CopyMemory(s + len1 + len2 + 4 + 4, &len3, 4);
	CopyMemory(s + len1 + len2 + 4 + 4 + 4, s3, sizeof(s3));
	int point = 0;
	for (int i = 0; i < 3; i++)
	{
		int l;
		CopyMemory(&l, s + point, 4);
		std::cout << "第" << i << "个字符len：" << l << "\n";
		char str[10] = { 0 };
		CopyMemory(str, s + point + 4, l);
		std::cout << "它的实际内容是：" << str << "\n"; 
		point += l + 4 ;
	}
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
