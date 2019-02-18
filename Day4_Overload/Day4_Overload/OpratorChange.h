#include<vector>

#pragma once
class OpratorChange
{
public:
	operator int();
	operator double();
	OpratorChange();
	~OpratorChange();
};
class C1
{
public:
	C1(int);
};

class C2
{
public:
	C2(long);
};

class C
{
	std::vector<unsigned> v;
public:
	C() 
	{
		v.push_back(1);
	}
	std::vector<unsigned> get_data() &;
	std::vector<unsigned> get_data() && ;
};

