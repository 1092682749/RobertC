// Day4_Overload.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Utils.h"
#include "OpratorChange.h"

using namespace std;

void Print(int i)
{
	cout << "int fun" << endl;
	cout << i << endl;
}
void Print(double d)
{
	cout << "double fun" << endl;
	cout << d << endl;
}
//void PrintClass(C1 c)
//{
//	cout << "print c1" << endl;
//}
void PrintClass(C2 c)
{
	cout << "print c1" << endl;
}
void func(int i)
{
	cout << "call int" << endl;
}
void func(const char *c)
{
	cout << "call char*" << endl;
}
int main()
{
	// extern void func(int i);
	OpratorChange o;
	Print((int)o);
	Print((double)o);
	C1 c(1);
	PrintClass(1);
	func(1);
	const char *a = "asd";
	func(a);
	Utils u;
	cout << u.max(1, 100) << endl;
	cout << u.max(1.111, 2.222) << endl;
    std::cout << "Hello World!\n"; 
	C cc;
	auto v1 = cc.get_data();
	auto v2 = C().get_data();
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
