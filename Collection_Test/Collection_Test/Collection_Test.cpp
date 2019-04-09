// Collection_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <array>
enum Direction {
	DOWN = 1,
	UP = 0,
};
class SomeData {
	

public:
	int age;
	char flag;
	SomeData(int a, int b)
	{
		age = a;
		flag = 'c';
		std::cout << "construct\n";
	}
};

std::vector<int> findDiagonalOrder(std::vector<std::vector<int>>& matrix) {
	std::vector<int> res(0);
	if (matrix.size() <= 0) return res;
	int M = matrix.size(), N = matrix.at(0).size();
	M--; N--;
	int x = 0, y = 0;
	Direction d = Direction::UP;
	while (x <= N && y <= M)
	{
		res.push_back(matrix.at(y).at(x));
		if (d == UP)
		{
			if (y - 1 < 0 || x + 1 > N)
			{
				if (x + 1 > N)
				{
					y++;
				} 
				else
				{
					x++;
				}
				d = Direction::DOWN;
				continue;
			}
			else
			{
				x++; y--;
				//res.push_back(matrix.at(x).at(y));
			}

		}
		if (d == DOWN)
		{
			if (y + 1 > M || x - 1 < 0)
			{
				if (y + 1 > M)
				{
					x++;
				}
				else
				{
					y++;
				}
				d = Direction::UP;
				continue;
			}
			else
			{
				x--; y++;
				//res.push_back(matrix.at(x).at(y));
			}
		}
	}
	return res;
}

int main()
{
	std::vector<int> nums;
	nums.push_back(1);
	nums.push_back(2);
	auto it = nums.begin();
	auto itend = nums.end();
	std::vector<int>::iterator itt = nums.begin();
	std::vector<int>::iterator ittend = nums.end();
	for (auto item : nums)
	{
		std::cout << item << "\n";
	}
	while (it < itend)
	{
		auto item = *it;
		std::cout << item << "\n";
		it++;
	}
	//int a = *(++nums.begin());
	//auto ittt = nums.begin();
	//std::cout << *ittt << "\n";
	//std::cout << a << "\n";
	//for (auto item : nums)
	//{
	//	std::cout << item << "\n";
	//}
	// const iteraor 就是一个常量指针
	/*auto cit = nums.cbegin();
	cit++;
	std::cout << *cit << "\n";*/
	auto rit = nums.rbegin();
	std::cout << *rit << "\n";
	rit++;
	std::cout << *rit << "\n";
	auto rend = nums.rend();
	// std::cout << *(rend++) << "\n";
	// std::array<int> as;
	SomeData s(1, 2);
	std::vector<SomeData> ss(10, SomeData(1, 2));
	std::cout << sizeof(s) << "\n";
	std::cout << sizeof(ss) << "\n";
	std::cout << sizeof(SomeData) << "\n";
	// delete &s;
	auto ssit = ss.begin();
	(*ssit).age = 2;
	for (SomeData item : ss)
	{
		std::cout << item.age << "\n";
	}
	std::vector<std::vector<int>> matrix = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} }, m2 = { {1} };

	findDiagonalOrder(m2);
	return 0;
}
