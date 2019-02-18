#pragma once
#include<string>
using namespace std;
class PointerTest
{
private:
	int n;
	string name;
public:
	PointerTest();
	PointerTest(string s);
	~PointerTest();
	class Inner
	{
	public:
		char *c;
	};
	void method();
};

