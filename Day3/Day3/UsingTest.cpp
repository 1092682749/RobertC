#include "pch.h"
#include "UsingTest.h"
#include <iostream>


// UsingTest class implement
UsingTest::UsingTest()
{
}


UsingTest::UsingTest(const UsingTest &u)
{
	std::cout << "copy construction" << std::endl;
}


UsingTest::~UsingTest()
{
}

void UsingTest::f(char c)
{
	std::cout <<"UsingTest" << c << std::endl;
}


void UsingTest::g(char c)
{
	std::cout <<"UsingTest" << c << std::endl;
}

// SubUsingTest class implement

void SubUsingTest::f(char c)
{
	std::cout << "SubUsingTest" << c << std::endl;
}

void SubUsingTest::g(char c)
{
	std::cout << "SubUsingTest" << c << std::endl;
}

// A class implement

void A::show(int i)
{
	std::cout << "AAAAAAAAA: " << i << std::endl;
}

void A::reshow(char c)
{
	std::cout << "AAAAAAAA: " << c << std::endl;
}

// B class implement
void B::show(int i)
{
	std::cout << "BBBBBB: " << i << std::endl;
}
void B::reshow(int i)
{
	std::cout << "BBBBBBBBB: " << i << std::endl;
}