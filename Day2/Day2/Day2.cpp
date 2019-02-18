// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "PointerTest.h"
#include <iostream>
#include <string>
#include "ExtrenB.h"
using namespace std;

int* f();
double dVar = 7.0;
void fun1()
{
	cout << "1111111111" << endl;
}
void fun2()
{
	cout << "2222222222222" << endl;
}
typedef void(*NAME) ();
void testPointerSpecifier()
{
	NAME funArr[2];
	funArr[0] = fun1;
	funArr[1] = fun2;
	funArr[0]();
	funArr[1]();
}
class Point
{
private:
	int x, y;
public:
	Point(int x, int y) : x(x), y(y)
	{
		cout << "my is a new point, my info is: x->"
			<< x
			<< " y-> "
			<< y << endl;
	}
	Point()
	{

	}
	void setPoint(const Point &p)
	{
		this->x = p.x;
		this->y = p.y;
	}
	void setPoints(initializer_list<Point> pointList)
	{
		// 利用指针加法遍历
		for (size_t i = 0; i < pointList.size(); i++)
		{
			Point p = { ((Point*)(pointList.begin() + i))->x, ((Point*)(pointList.begin() + i))->y };
		}
		// 利用auto关键字遍历list
		/*for (auto item : pointList)
		{
			cout << item.x << endl;
		}*/
	}
	Point getNewPoint(int x, int y)
	{
		return Point{ x, y };
	}
	Point getFixPoint()
	{
		return { 10, 10 };
	}
};
int main()
{
	// aliases and typedef
	using _intt = int;
	_intt i = 10;
	cout << i << endl;



	showExternVariable();


	////////////////////
	cout << dVar << endl;
	testPointerSpecifier();
	int mol{ 10 };
	cout << mol << endl;
	PointerTest p("wo"), pt("ni");
	void(PointerTest::* m)();
	m = &PointerTest::method;
	(p.*m)();
	(pt.*m)();
	cout << f() << endl;



	// ??????????????
	int PointerTest::* ppp;
	int ::PointerTest::* pp;
	char PointerTest::Inner::* a;


	// initialization test
	string hello = "Hello!";
	Point p1(1, 2);
	p1.setPoint({ 2, 2 });
	Point p2 = p2.getFixPoint();
	p2 = p2.getNewPoint(1, 1);
	p2.setPoints({ {1, 1}, {2, 2} });
	return 0;
}
int* f()
{
	int i(1);
	return &i;
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
