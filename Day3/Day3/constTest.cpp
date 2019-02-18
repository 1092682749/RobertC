#include "pch.h"
#include "constTest.h"
#include<iostream>
using namespace std;

constTest::constTest()
{
	this->id = 10;
	this->s = "asd";
}


constTest::~constTest()
{
}

void constTest::read() const
{
	// 不能在const函数里修改成员变量
	/*this->s = "asd";
	this->id = 10;*/
	// 但是可以修改static成员
	this->version = 10;
	// 不能调用非const成员函数
	//this->write();
	// 但是可以调用const成员函数。这都是为了维持只读性
	this->read1();
	cout << this->s << endl;
}

void constTest::write()
{

}
void constTest::read1() const
{

}
