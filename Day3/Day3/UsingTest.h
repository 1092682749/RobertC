#pragma once
class UsingTest
{
public:
	UsingTest();
	UsingTest(const UsingTest&);
	~UsingTest();
	void f(char);
	void g(char);
};
class SubUsingTest:public UsingTest
{
public:
	void f(char);
	void g(char);
};
class A
{
public:
	void show(int);
	void reshow(char);
};
class B : A
{
public:
	using A::show;
	using A::reshow;
	void show(int);
	void reshow(int);
};
static int myInt = 10;
extern int DefinedElsewhere;