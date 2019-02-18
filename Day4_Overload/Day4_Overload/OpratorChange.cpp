#include "pch.h"
#include "OpratorChange.h"
#include <iostream>
#include <vector>

OpratorChange::OpratorChange()
{
}


OpratorChange::~OpratorChange()
{
}

OpratorChange::operator double()
{
	std::cout << "oprator double" << std::endl;
	return 100.00;
}

OpratorChange::operator int()
{
	std::cout << "oprator int" << std::endl;
	return 100;
}

C1::C1(int i) {}

C2::C2(long l) {}

std::vector<unsigned> C::get_data() &
{
	std::cout << "i am left call" << std::endl;
	
	return this->v;
}

std::vector<unsigned> C::get_data() &&
{
	std::cout << "i am right call" << std::endl;
	return this->v;
}
