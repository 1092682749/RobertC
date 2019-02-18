#include "pch.h"
#include "PointerTest.h"
#include<iostream>
#include<string>
using namespace std;

PointerTest::PointerTest()
{
}

PointerTest::PointerTest(string s) {
	name = s;
};

PointerTest::~PointerTest()
{
}

void PointerTest::method()
{
	cout << "I am method." << "my instance is: " << name << endl;
}
