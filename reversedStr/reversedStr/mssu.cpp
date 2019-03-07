#include "pch.h"
#include "MyStrUtils.h"
void mssu::reverseString(std::vector<char> &a)
{
	char temp;
	for (size_t i = 0; i < (a.size() / 2); i++)
	{
		temp = a.at(i);
		a.at(i) = a.at(a.size() - 1 - i);
		a.at(a.size() - 1 - i) = temp;
	}
}