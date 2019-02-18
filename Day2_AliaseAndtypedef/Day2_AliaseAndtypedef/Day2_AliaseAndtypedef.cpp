// Day2_AliaseAndtypedef.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <new>
#include<vector>
template <typename T> class MyAlloc;
using MyIntVector = std::vector<int, MyAlloc<int>>;
template <typename T> struct MyAlloc
{
	typedef T value_type;
	MyAlloc() 
	{
		std::cout << "MyAlloc construction" << std::endl;
	}
	template <typename U> MyAlloc(const MyAlloc<U>&) {}
	bool operator==(const MyAlloc&)const { return true; }
	bool operator!=(const MyAlloc&)const { return false; }

	T* allocate(const size_t n)const
	{
		if (n == 0)
		{
			return nullptr;
		}
		if (n > static_cast<size_t>(-1) / sizeof(T))
		{
			throw std::bad_array_new_length();
		}
		void * const pv = malloc(n * sizeof(T));
		if (!pv)
		{
			throw std::bad_alloc();
		}
		return static_cast<T*>(pv);
	}
	void deallocate(T* const p, size_t)const
	{
		free(p);
	}
};
int main()
{
	MyIntVector foov = { 1701, 1764, 1664 };
	for (auto a : foov)
	{
		std::cout << a << " " << std::endl;
	}
    std::cout << "Hello World!\n"; 
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
