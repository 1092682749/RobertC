// Day3_union.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <queue>
using namespace std;
union RecordType    // Declare a simple union type
{
	char   ch;
	int    i;
	long   l;
	float  f;
	double d;
	int *int_ptr;
};
enum class WeatherDataType
{
	Temperature, Wind
};
struct TempData
{
	int StationId;
	time_t time;
	double current;
	double max;
	double min;
};
struct WindData
{
	int StatinId;
	time_t time;
	int speed;
	short direction;
};
struct Input
{
	WeatherDataType type;
	union
	{
		TempData temp;
		WindData wind;
	};
};
void Process_Temp(TempData t) {}
void Process_Wind(WindData w) {}
queue<Input> inputs;
void Initialize();
int main()
{
	RecordType r;
	RecordType *rp = &r;
	r.i = 1;
	r.ch = 'a';
	
	std::cout << (*rp).ch << std::endl;
	std::cout << rp->i << std::endl;
	std::cout << (*rp).i << std::endl;
    std::cout << "Hello World!\n"; 

	
	Initialize();
	while (!inputs.empty())
	{
		Input i = inputs.front();
		switch (i.type)
		{
		case WeatherDataType::Temperature:
			Process_Temp(i.temp);
			break;
		case WeatherDataType::Wind:
			Process_Wind(i.wind);
			break;
		default:
			break;
		}
	}
	return 0;
}
void Initialize()
{
	Input first, second;
	first.type = WeatherDataType::Temperature;
	first.temp = { 101, 1418855664, 91.8, 108.5, 67.2 };
	inputs.push(first);

	second.type = WeatherDataType::Wind;
	second.wind = { 204, 1418859354, 14, 27 };
	inputs.push(second);
}