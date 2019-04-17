// DB_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include  <Windows.h>
#include <conio.h>
#include <mysqlx/xdevapi.h>


int main()
{
	/*int ch = 0;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(h, &cb);
	COORD poi;
	poi.X = cb.dwCursorPosition.X;
	poi.Y = cb.dwCursorPosition.Y;
	while (true)
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 0 || ch == 0xE0) ch = _getch();
			if (ch == 72)
			{
				std::cout << "up" << "\n";
			}
			else
			{
				std::cout << (char)ch << "\n";
			}
		}
		SetConsoleCursorPosition(h, poi);
		poi.Y++;

		std::cout << "↓\n";
		Sleep(180);
		system("cls");
	}*/



	// Scope controls life-time of objects such as session or schema

	
		mysqlx::Session sess("dyzhello.club", 33060, "root", "dyz");
		mysqlx::Schema db = sess.getSchema("testredis");
		// or Schema db(sess, "test");

		mysqlx::Collection myColl = db.getCollection("User");
		// or Collection myColl(db, "my_collection");

		mysqlx::DocResult myDocs = myColl.find("username like :param")
			.limit(1)
			.bind("param", "S%").execute();

		std::cout << myDocs.fetchOne();
	
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
