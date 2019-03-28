// File_Management_CMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#define BUFSIZE 1024
enum operation
{
	COPY = 1,
	MOVE,
};
void DisplayFiles()
{

	WIN32_FIND_DATA fileData;
	HANDLE hCurrentFile = INVALID_HANDLE_VALUE;
	hCurrentFile = FindFirstFile(TEXT("*"), &fileData);
	if (hCurrentFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "查找文件失败\n";
	}
	do
	{
		if (fileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		}
		_tprintf(TEXT("   %s\n"), fileData.cFileName);
		//system("color 07");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
	} while (FindNextFile(hCurrentFile, &fileData));
	// CloseHandle(hCurrentFile);
}
// Copy和Move
void MyCopyFile(const char* oldFile, const char* newFile, int flag)
{
	// 转换字节序列为宽字节序列
	TCHAR wOldFile[200] = { 0 }, wNewFile[200] = { 0 };
	MultiByteToWideChar(CP_UTF8, NULL, oldFile, strlen(oldFile), wOldFile, 200);
	MultiByteToWideChar(CP_UTF8, NULL, newFile, strlen(newFile), wNewFile, 200);

	if (flag == COPY)
	{
		if (!CopyFile(wOldFile, wNewFile, FALSE))
		{
			std::cout << "拷贝失败\n";
			return;
		}
		std::cout << "拷贝成功\n";
	}
	else if (flag == MOVE)
	{
		if (!MoveFile(wOldFile, wNewFile))
		{
			std::cout << "移动失败\n";
			return;
		}
		std::cout << "移动成功\n";
	}
	
}
void print_help()
{
	std::cout << "copy [filename] [filename]\n";
	std::cout << "move [filename] [filename]\n";
	std::cout << "delete [filename]\n";
	std::cout << "ls\n";
	std::cout << "cd  [path]\n";
	std::cout << "append [filename]\n";
}
void MyDeleteFile(const char* fileName)
{
	TCHAR wFileName[200] = { 0 };
	MultiByteToWideChar(CP_UTF8, NULL, fileName, strlen(fileName), wFileName, 200);
	
	if (!DeleteFile(wFileName))
	{
		std::cout << "删除失败\n";
		return;
	}
	std::cout << "删除成功\n";
}

void MyCreateFile(const char* fileName)
{
	TCHAR wFileName[200] = { 0 };
	MultiByteToWideChar(CP_UTF8, NULL, fileName, strlen(fileName), wFileName, 200);
	HANDLE hFile = CreateFile(wFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "创建文件失败\n";
		return;
	}
	std::cout << "创建文件成功\n";
	CloseHandle(hFile);
}
void MyAppendFile(const char* fileName)
{
	TCHAR wFileName[200] = { 0 };
	char outBuf[BUFSIZE] = { 0 };
	MultiByteToWideChar(CP_UTF8, NULL, fileName, strlen(fileName), wFileName, 200);
	HANDLE hFile = CreateFile(wFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "打开文件失败\n";
		return;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	std::cout << "请输入要写入的内容：";
	std::cin >> outBuf;
	if (!WriteFile(hFile, outBuf, BUFSIZE, NULL, NULL))
	{
		CloseHandle(hFile);
		std::cout << "写入文件时发生错误\n";
		return;
	}
	std::cout << "写入完成\n";
	CloseHandle(hFile);
}
int main()
{
	TCHAR currentPath[BUFSIZE] = { 0 };
	GetCurrentDirectory(BUFSIZE, currentPath);
	char cmd[10] = { 0 }, fileName1[200] = { 0 }, fileName2[200] = { 0 };
	while (true)
	{
		_tprintf(TEXT("%s>"), currentPath);
		std::cin >> cmd;
		if (strcmp("copy", cmd) == 0)
		{
			std::cin >> fileName1;
			std::cin >> fileName2;
			MyCopyFile(fileName1, fileName2, COPY);
		}
		else if (strcmp("move", cmd) == 0)
		{
			std::cin >> fileName1;
			std::cin >> fileName2;
			MyCopyFile(fileName1, fileName2, MOVE);
		}
		else if (strcmp("delete", cmd) == 0)
		{
			std::cin >> fileName1;
			MyDeleteFile(fileName1);
		}
		else if (strcmp("ls", cmd) == 0)
		{
			DisplayFiles();
		}
		else if (strcmp("cd", cmd) == 0)
		{
			std::cin >> fileName1;
			TCHAR tempStr[200] = { 0 };
			MultiByteToWideChar(CP_UTF8, NULL, fileName1, strlen(fileName1), tempStr, 200);
			if (!SetCurrentDirectory(tempStr))
			{
				std::cout << "设置目录失败，请检查输入\n";
			}
			GetCurrentDirectory(BUFSIZE, currentPath);
		}
		else if (strcmp("create", cmd) == 0)
		{
			std::cin >> fileName1;
			MyCreateFile(fileName1);
		}
		else if (strcmp("append", cmd) == 0)
		{
			std::cin >> fileName1;
			MyAppendFile(fileName1);
		}
		else if (strcmp("-h", cmd) == 0)
		{
			print_help();
		}
		else
		{
			std::cout << "输入 -h 以获得帮助\n";
		}
	}
	return 0;
}
