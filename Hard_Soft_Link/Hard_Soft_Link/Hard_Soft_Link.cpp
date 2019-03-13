// Hard_Soft_Link.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
//#include <stdlib.h>
#include <fstream>

int main()
{
	char data[100];
	std::fstream of;
	of.open("C:/Users/i-robert/Desktop/14264131.txt");
	std::cin.getline(data, 100);
	//of.write(data, 100);
	of << data << std::endl;
	of.flush();
	std::cout << data << "\n";
	/*std::ofstream off;
	off.open("C:/Users/i-robert/Desktop/14264131.txt");
	std::cin.getline(data, 100);
	off.write(data, 100);
	off.flush();
	off.close();*/
	return 0;
}
//#include <fstream>
//#include <iostream>
//using namespace std;
//
//int main()
//{
//
//	char data[100];
//
//	// 以写模式打开文件
//	ofstream outfile;
//	outfile.open("C:/Users/i-robert/Desktop/14264131.txt");
//
//	cout << "Writing to the file" << endl;
//	cout << "Enter your name: ";
//	cin.getline(data, 100);
//
//	// 向文件写入用户输入的数据
//	outfile << data << endl;
//
//	cout << "Enter your age: ";
//	cin >> data;
//	cin.ignore();
//
//	// 再次向文件写入用户输入的数据
//	outfile << data << endl;
//
//	// 关闭打开的文件
//	outfile.close();
//
//	// 以读模式打开文件
//	ifstream infile;
//	infile.open("C:/Users/i-robert/Desktop/14264131.txt");
//
//	cout << "Reading from the file" << endl;
//	infile >> data;
//
//	// 在屏幕上写入数据
//	cout << data << endl;
//
//	// 再次从文件读取数据，并显示它
//	infile >> data;
//	cout << data << endl;
//
//	// 关闭打开的文件
//	infile.close();
//
//	return 0;
//}
