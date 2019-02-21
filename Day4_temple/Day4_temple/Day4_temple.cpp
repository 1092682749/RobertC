// Day4_temple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

using namespace std;

extern "C" int printf(const char*, ...);
template <class T> void f(T) {
	printf_s("Less specialized function called\n");
}

template <class T> void f(T*) {
	printf_s("More specialized function called\n");
}

template <class T> void f(const T*) {
	printf_s("Even more specialized function for const T*\n");
}
template <class T>
struct S
{
	template <class U> operator S<U>()
	{
		return S<U>();
	}
};
template <typename T> class X
{
public:
	void f(typename T::myType* mt) 
	{
		mt->see();
	};
};
class Yarg
{
public:
	struct myType 
	{
		void see()
		{
			cout << "see" << endl;
		}
	};
};
class without
{

};
void myFunction()
{
	cout << "global" << endl;
}
namespace mynamespace
{
	void myFunction()
	{
		cout << "mynamespace" << endl;
	}
}
void ff(int, int) { cout << "ff(int, int)" << endl; }
void ff(char, char) { cout << "ff(char, char)" << endl; }

template <class T1, class T2>
void ff(T1, T2)
{
	cout << "void ff(T1, T2)" << endl;
};
int a1[] = { 1, 2, 3 };
int a2[] = { 10, 20, 30 };
int *pBased; // 非静态局部变量不能再__based规范中使用
int main()
{

	char a[] = "hello";
	cout << a << endl;

	// 显示告诉编译器把pBased用作基指针，声明基于指针的指针pb
		// 这里我们初始化基指针的偏移量为0
	int __based(pBased)*pb = 0;

	pBased = &a1[0];
	std::cout << *pb << std::endl;                  // 1
	cout << *pBased << endl;
	std::cout << *(pb + 1) << std::endl;              // 2
	cout << *(pBased + 1) << endl;
	pBased = &a2[0];
	std::cout << *pb << std::endl;                  // 10
	std::cout << *(pb + 1) << std::endl;              // 20







	X<Yarg> x;
	//X<without> xx;
	x.f(new Yarg::myType());
	ff(1, 1);   // Equally good match; choose the nontemplate function.
	ff('a', 1); // Chooses the template function.
	ff<int, int>(2, 2);  // Template arguments explicitly specified.
	int i = 0;
	const int j = 0;
	int *pi = &i;
	const int *cpi = &j;
	mynamespace::myFunction();
	f(i);   // Calls less specialized function.
	f(pi);  // Calls more specialized function.
	f(cpi); // Calls even more specialized function.
	// Without partial ordering, these calls would be ambiguous.
    std::cout << "Hello World!\n"; 
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
