#include "pch.h"
#include "MyIntUtils.h"
#include <math.h>
#include <exception>


int miu::reverse(int a)
{
	int plus = a > 0 ? 1 : -1;
	a = abs(a);
	int remainder = 0, newSum = 0;
	
		while ((a / 10) != 0)
		{
			remainder = a % 10;
			a = a / 10;
			if (newSum >= 2147483647 / 10 || newSum <= -2147483648 / 10)
			{
				return 0;
			}
			newSum *= 10;
			newSum += remainder;
		}
		if (newSum >= 2147483647 / 10 || newSum <= -2147483648 / 10)
		{
			return 0;
		}
		newSum = newSum * 10 + a;
	
	
	return newSum * plus;
}
