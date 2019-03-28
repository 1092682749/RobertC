// Virtual_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
class Base
{
public:
	virtual void fun1()
	{
		std::cout << "fun1\n";
	}
	void fun2()
	{
		std::cout << "fun2\n";
	}
};
class Sub: public Base
{
public:
	void fun1()
	{
		std::cout << "sub::fun1\n";
	}
	void fun2()
	{
		std::cout << "sub::fun2\n";
	}
};
class SSub : public Sub
{
public:
	void fun1()
	{
		std::cout << "ssub::fun1\n";
	}
	void fun2()
	{
		std::cout << "ssub::fun2\n";
	}
};
int main()
{
	typedef void(*FUN)(void);
	Base *b = new Sub;
	b->fun1();
	b->fun2();
	Sub *s = new SSub();
	s->fun1();
	s->fun2();
	std::cout << "aaaaaaaaaaaaaaaaaaaaaa\n";
	FUN f = FUN(*((int*)*((int*)b)));
	void(*ff)(void) = (void(*)(void))(*((int*)(*((int*)b))));
	ff();
	return 0;
}
