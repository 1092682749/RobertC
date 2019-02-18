#pragma once
#include <string>
class constTest
{
public:
	std::string s;
	int id;
	static int version;
	constTest();
	~constTest();
	void read() const;
	void read1()const;
	void write();
};

