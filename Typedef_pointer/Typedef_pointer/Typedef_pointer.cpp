// Typedef_pointer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main()
{
	typedef char *p;
	char c = 'c';
	char d = 'd';
	const char *lps = &c;
	char * const clps = &c;
	char const *ccp = &c;
	
	lps = &d;
	const p cp = &c;
	const p *ps = &cp;
	ps++;
	// (*ps)++;
	//std::cin >> c;
	//std::cout << lps;
	//lps++;
	//cp++;
	//clps++;
	*cp = 'n';
	//(*ccp)++;
	std::cout << c << "\n";
    std::cout << "Hello World!\n"; 
}
