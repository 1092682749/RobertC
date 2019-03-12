
// iotest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "..\CommonFiles\CmnHdr.h"           // See Appendix A.
#include "..\CommonFiles\IOCompletionPort.h" // See Appendix A.
#include "..\CommonFiles\EnsureCleanup.h"    // See Appendix A.

#include <WindowsX.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define US_PER_S (1000UL * 1000UL)

typedef struct {
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;
} Timer;

__inline static void timer_start(Timer *timer) {
	QueryPerformanceFrequency(&timer->freq);
	QueryPerformanceCounter(&timer->start);
}

__inline static LONGLONG timer_elapsed_us(Timer *timer) {
	QueryPerformanceCounter(&timer->end);
	return (((timer->end.QuadPart - timer->start.QuadPart) * US_PER_S) / timer->freq.QuadPart);
}

///////////////////////////////////////////////////////////////////////////////


// Each I/O Request needs an OVERLAPPED structure and a data buffer
class CIOReq : public OVERLAPPED {
public:
	CIOReq() {
		Internal = InternalHigh = 0;
		Offset = OffsetHigh = 0;
		hEvent = NULL;
		m_nBuffSize = 0;
		m_pvData = NULL;
	}

	~CIOReq() {
		if (m_pvData != NULL)
			VirtualFree(m_pvData, 0, MEM_RELEASE);
	}

	BOOL AllocBuffer(SIZE_T nBuffSize) {
		m_nBuffSize = nBuffSize;
		m_pvData = VirtualAlloc(NULL, m_nBuffSize, MEM_COMMIT, PAGE_READWRITE);
		return(m_pvData != NULL);
	}

	BOOL Read(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL) {
		if (pliOffset != NULL) {
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		return(::ReadFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}

	BOOL Write(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL) {
		if (pliOffset != NULL) {
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		return(::WriteFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}

private:
	SIZE_T m_nBuffSize;
	PVOID  m_pvData;
};


///////////////////////////////////////////////////////////////////////////////


#define BUFFSIZE              (8 * 1024 * 1024) // The size of an I/O buffer
#define MAX_PENDING_IO_REQS   96           // The maximum # of I/Os


// The completion key values indicate the type of completed I/O.
#define CK_READ  1
#define CK_WRITE 2
#define CK_EXIT 3


///////////////////////////////////////////////////////////////////////////////


BOOL FileCopy(PCTSTR pszFileSrc, PCTSTR pszFileDst) {

	BOOL bOk = FALSE;    // Assume file copy fails
	LARGE_INTEGER liFileSizeSrc = { 0 }, liFileSizeDst;

	try {
		{
			// Open the source file without buffering & get its size
			CEnsureCloseFile hFileSrc = CreateFile(pszFileSrc, GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
			if (hFileSrc.IsInvalid()) goto leave;

			// Get the file's size
			GetFileSizeEx(hFileSrc, &liFileSizeSrc);
			liFileSizeSrc.QuadPart = (LONGLONG)32 * 1024 * 1024 * 1024;
			// Nonbuffered I/O requires sector-sized transfers.
			// I'll use buffer-size transfers since it's easier to calculate.
			liFileSizeDst.QuadPart = chROUNDUP(liFileSizeSrc.QuadPart, BUFFSIZE);

			// Open the destination file without buffering & set its size
			CEnsureCloseFile hFileDst = CreateFile(pszFileDst, GENERIC_WRITE,
				0, NULL, CREATE_ALWAYS,
				FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, hFileSrc);
			if (hFileDst.IsInvalid()) goto leave;

			// File systems extend files synchronously. Extend the destination file 
			// now so that I/Os execute asynchronously improving performance.
			SetFilePointerEx(hFileDst, liFileSizeDst, NULL, FILE_BEGIN);
			SetEndOfFile(hFileDst);

			// Create an I/O completion port and associate the files with it.
			CIOCP iocp(0);
			//iocp.AssociateDevice(hFileSrc, CK_READ);  // Read from source file
			iocp.AssociateDevice(hFileDst, CK_WRITE); // Write to destination file

			// Initialize record-keeping variables
			CIOReq ior[MAX_PENDING_IO_REQS];
			LARGE_INTEGER liNextReadOffset = { 0 };
			int nReadsInProgress = 0;
			int nWritesInProgress = 0;

			// Prime the file copy engine by simulating that writes have completed.
			// This causes read operations to be issued.
			for (int nIOReq = 0; nIOReq < _countof(ior); nIOReq++) {

				// Each I/O request requires a data buffer for transfers
				chVERIFY(ior[nIOReq].AllocBuffer(BUFFSIZE));
				nWritesInProgress++;
				iocp.PostStatus(CK_WRITE, 0, &ior[nIOReq]);
			}

			BOOL bResult = FALSE;

			Timer timer;
			LONGLONG elapsed;
			double speed;
			timer_start(&timer);

			// Loop while outstanding I/O requests still exist
			while (/*(nReadsInProgress > 0) || (nWritesInProgress > 0)*/TRUE) {

				// Suspend the thread until an I/O completes
				ULONG_PTR CompletionKey;
				DWORD dwNumBytes;
				CIOReq* pior;
				bResult = iocp.GetStatus(&CompletionKey, &dwNumBytes, (OVERLAPPED**)&pior, INFINITE);

				switch (CompletionKey) {
				case CK_WRITE:  // Read completed, write to destination
					nWritesInProgress--;
					bResult = pior->Write(hFileDst/*, &liNextReadOffset*/);  // Write to same offset read from source
					if (!bResult)
					{
						DWORD  dwError = GetLastError();
						if (dwError != ERROR_IO_PENDING)
						{
							printf_s("[ERROR %u] %d in WriteFileGather\n", dwError);
							return -6;
						}
						else
						{
							nWritesInProgress++;
							//printf_s("%d\r", nWritesInProgress++);
						}
					}

					liNextReadOffset.QuadPart += BUFFSIZE;
					break;
				}
				if (liNextReadOffset.QuadPart >= liFileSizeDst.QuadPart)
				{
					break;
				}
			}
			bOk = TRUE;
			elapsed = timer_elapsed_us(&timer);
			speed = (double)1.0*(liFileSizeSrc.QuadPart / 1024 / 1024) * US_PER_S / elapsed;
			printf("Received %lld(GB),%0.1f(MB/s)\n", liFileSizeSrc.QuadPart >> 20, speed);
		}
	leave:;
	}
	catch (...) {
	}

	if (bOk) {
		// The destination file size is a multiple of the page size. Open the
		// file WITH buffering to shrink its size to the source file's size.
		CEnsureCloseFile hFileDst = CreateFile(pszFileDst, GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFileDst.IsValid()) {

			SetFilePointerEx(hFileDst, liFileSizeSrc, NULL, FILE_BEGIN);
			SetEndOfFile(hFileDst);
		}
	}

	return(bOk);
}

int main()
{
	std::cout << "Hello World!\n";
	BOOL nRet = FileCopy(L"D:\\Testdata\\test.bin", L"D:\\Testdata\\FileCopy.cpy");
}

