// Day3_constexpr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
using namespace std;
constexpr float exp(float x, int n)
{
	return n == 0 ? 1 :
		n % 2 == 0 ? exp(x * x, n / 2) :
		exp(x * x, (n - 1) / 2) * x;
}
constexpr float exp2(const float& x, const int& n)
{
	return n == 0 ? 1 :
		n % 2 == 0 ? exp(x * x, n / 2) :
		exp(x * x, (n - 1) / 2) * x;
}
template<typename T, int N> constexpr int length(const T(&ary)[N])
{
	return N;
}
constexpr int fac(int n)
{
	return n == 1 ? 1 : n * fac(n - 1);
}
class Foo
{
public:
	constexpr explicit  Foo(int i) : _i(i) {}
	constexpr int GetValue()
	{
		return _i;
	}
private:
	int _i;
};
class A {
public:
	int x;
	A(int x) : x(x)
	{
		cout << "Constructor" << endl;
	}
	A(A& a) : x(a.x)
	{
		cout << "Copy Constructor" << endl;
	}
	A& operator=(A& a)
	{
		x = a.x;
		cout << "Copy Assignment operator" << endl;
		return *this;
	}
	A(A&& a) : x(a.x)
	{
		cout << "Move Constructor" << endl;
	}
	A& operator=(A&& a)
	{
		x = a.x;
		cout << "Move Assignment operator" << endl;
		return *this;
	}
};

A GetA()
{
	return A(1);
}

A&& MoveA()
{
	return A(1);
}
int main()
{
	//foo is const:
	constexpr Foo foo(5);
	// foo = Foo(6); //Error!

	//Compile time:
	constexpr float x = exp(5, 3);
	constexpr float y{ exp(2, 5) };
	//constexpr int val = foo.GetValue();
	constexpr int f5 = fac(5);
	const int nums[]{ 1, 2, 3, 4 };
	const int nums2[length(nums) * 2]{ 1, 2, 3, 4, 5, 6, 7, 8 };

	//Run time:
	//cout << "The value of foo is " << foo.GetValue() << endl;
	int count = 10;
	int& countRef = count;
	auto myAuto = count;
    std::cout << "Hello World!\n"; 
	std::cout << myAuto << std::endl;
	myAuto = countRef;
	std::cout << myAuto << std::endl;
	decltype(myAuto) i;
	i = 1000;
	std::cout << i << std::endl;
	////////////////////////////////
	cout << "-------------------------1-------------------------" << endl;
	A a(1);
	cout << "-------------------------2-------------------------" << endl;
	A b = a;
	cout << "-------------------------3-------------------------" << endl;
	A c(a);
	cout << "-------------------------4-------------------------" << endl;
	b = a;
	cout << "-------------------------5-------------------------" << endl;
	A d = A(1);
	cout << "-------------------------6-------------------------" << endl;
	A e = std::move(a);
	cout << "-------------------------7-------------------------" << endl;
	A f = GetA();
	cout << "-------------------------8-------------------------" << endl;
	A&& g = MoveA();
	cout << "-------------------------9-------------------------" << endl;
	d = A(1);
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
