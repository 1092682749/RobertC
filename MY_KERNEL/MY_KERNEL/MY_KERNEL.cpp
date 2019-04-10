// MY_KERNEL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>

#define HEAPSIZE 1024 * 1024

typedef struct Memory_Control_Block {
	LPVOID address;
	struct Memory_Control_Block *next;
	int size;

}MCB;
LPVOID heapHead = NULL;
LPVOID heapEnd;
MCB *tableHead = NULL;
void* MALLOC(int size) {
	MCB *createSpace = new MCB;
	
	if (heapHead == NULL)
	{
		HANDLE hHeap = GetProcessHeap();
		heapHead = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, HEAPSIZE);
		heapEnd = (LPVOID)((char*)heapHead + HEAPSIZE);
	}
	
	if (size > HEAPSIZE)
	{
		std::cout << "内存不足无法分配\n";
		return NULL;
	}
	createSpace->size = size;
	createSpace->next = NULL;
	if (tableHead == NULL)
	{
		createSpace->address = heapHead;
		tableHead = createSpace;
	}
	else
	{
		std::cout << "开始遍历\n";
		MCB *currentMBC = tableHead;
		while (currentMBC->next != NULL)
		{
			currentMBC = currentMBC->next;
		}
		createSpace->address = (LPVOID)((char* )currentMBC->address + currentMBC->size);
		
		currentMBC->next = createSpace;
	}
	return createSpace->address;
}
int main()
{
	/*char arr[] = "aaaaaa";
	char *pa = arr;
	std::cout << sizeof(*pa);*/
	char *cs = (char*)MALLOC(10);
	char *cs2 = (char*)MALLOC(10);
	for (int i = 0; i < 10; i++)
	{
		cs[i] = '1';
		cs2[i] = '2';
	}
	std::cout << cs << "\n";
	std::cout << cs2 << "\n";
	return 0;
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
