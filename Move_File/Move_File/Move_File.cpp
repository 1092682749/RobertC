// Move_File.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>

// 回调函数
DWORD CALLBACK CopyProgressRoutine(
	LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID lpData)
{
	std::cout << "\r%" << TotalBytesTransferred.QuadPart * 100 % TotalFileSize.QuadPart;
	return PROGRESS_CONTINUE;
}
DWORD CALLBACK CopyProgress(
	LARGE_INTEGER TotalFileSize, // total file size, in bytes
	LARGE_INTEGER TotalBytesTransferred, // total number of bytes transferred
	LARGE_INTEGER StreamSize, // total number of bytes for this stream
	LARGE_INTEGER StreamBytesTransferred, // total number of bytes transferred for this stream
	DWORD dwStreamNumber, // the current stream
	DWORD dwCallbackReason, // reason for callback
	HANDLE hSourceFile, // handle to the source file
	HANDLE hDestinationFile, // handle to the destination file
	LPVOID lpData // passed by CopyFileEx
)
{
	static int nRecord = 0;
	nRecord++;
	_tprintf(L"回调次数：%d 已传输：%08X:%08X 文件大小：%08X:%08X ",
		nRecord,
		TotalBytesTransferred.HighPart,
		TotalBytesTransferred.LowPart,
		TotalFileSize.HighPart,
		TotalFileSize.LowPart);
	return PROGRESS_CONTINUE;
}
int main()
{
	DWORD bufferSize = 0;
	bufferSize = GetCurrentDirectory(0, NULL);
	TCHAR *buff = new TCHAR[bufferSize];
	GetCurrentDirectory(bufferSize, buff);
	_tprintf(L"Current Directory is: %s\n", buff);
	// 设置当前目录为D
	if (!SetCurrentDirectory(L"D:/"))
	{
		_tprintf(L"SetCurrentDirectory faild!\n");
	}
	// 移动文件若果文件存在 Move file 不能成功回调
	/*if (!MoveFileWithProgress(L"/a/Evernote_6.17.9.553.exe", L"/a/aa/Evernote_6.17.9.553.exe", (LPPROGRESS_ROUTINE)CopyProgress, NULL, MOVEFILE_COPY_ALLOWED))
	{
		_tprintf(L"Move faild!\n");
		return -1;
	}*/
	// Copy file
	if (!CopyFileEx(L"/a/Evernote_6.17.9.553.exe", L"/a/aa/Evernote_6.17.9.553.exe", (LPPROGRESS_ROUTINE)CopyProgressRoutine, NULL, FALSE, COPY_FILE_FAIL_IF_EXISTS))
	{
		_tprintf(L"Move faild!\n");
			return -1;
	}
	_tprintf(L"\nCompleted!\n");
	return 0;
}
