#pragma once
class tl
{
public:
	thread_local static int i;
	tl();
	~tl();
};

