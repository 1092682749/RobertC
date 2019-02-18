#include "pch.h"
#include "ExternA.h"

int i = 999;
extern const int ii = 666;
extern constexpr int x = 9;
ExternA::ExternA()
{
}


ExternA::~ExternA()
{
}
extern "C" int printf(const char *fmt, ...);
extern "C" {
	// add your #include statements here
#include <stdio.h>
}
