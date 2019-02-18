// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "UsingTest.h"
#include "staticTest.h"

void ifRetain(int addNum)
{
	static int i;
	i += addNum;
	std::cout << "i is: " << i << std::endl;
}
struct C {
	void Test(int value) {
		static int var = 0;
		if (var == value)
			std::cout << "var == value" << std::endl;
		else
			std::cout << "var != value" << std::endl;

		var = value;
	}
};

int main()
{
	int myInt = 10;
    std::cout << "Hello World!\n"; 
	// Does the subclass override the method of the parent class?(yes)
	SubUsingTest sub1;
	UsingTest parent1;
	sub1.f('a');
	parent1.f('b');


	// In the without keyword of virtual, whether to show polymorphism or not.(no)
	UsingTest *parent2 = &sub1;
	(*parent2).f('c');
	UsingTest *parent3 = new SubUsingTest();
	parent3->f('d');

	// How do subclasses transform into parent classes?
	parent1 = sub1;
	parent1.f('e');

	// using contain same signature
	B b;
	b.show(1);
	b.reshow('1');
	b.reshow(1);
	

	// static
	for (size_t i = 0; i < 5; i++)
	{
		ifRetain(i);
	}
	staticTest s1, s2, s3;
	s3.version = 10;
	std::cout << "s1 is: " << s1.version << std::endl;
	std::cout << "s2 is: " << s2.version << std::endl;
	C c1;
	C c2;
	c1.Test(100);
	c2.Test(100);
	int DefinedHere;
	{
		// refers to DefinedHere in the enclosing scope
		extern int DefinedHere;
	}
	// const
	char *const  *cc;
	char *const *c;
	c = cc;
	const int maxarray = 255;
	char store_char[maxarray];  // allowed in C++; not allowed in C
	const char *mybuf = "test1";
	const char *bptr = mybuf;
	bptr = "test2";
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
