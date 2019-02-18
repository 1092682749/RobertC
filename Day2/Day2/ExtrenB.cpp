#include "pch.h"
#include "ExtrenB.h"
#include<iostream>
#include "ExternA.h"
#include "ExternA.cpp"
using namespace std;

extern int i;
extern const int ii;
// 为什么可以重新定义
//extern constexpr  int x = 10;

ExtrenB::ExtrenB()
{
}


ExtrenB::~ExtrenB()
{
}

void showExternVariable()
{
	i++;
	cout << i << endl;
	cout << ii << endl;
}
