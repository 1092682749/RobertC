#include "pch.h"
#include <Windows.h>
#include <iostream>

class CIOReq : public OVERLAPPED 
{
public:
	CIOReq() 
	{
		Internal = InternalHigh = 0;
		Offset = OffsetHigh = 0;
		hEvent = NULL;
		m_nBuffSize = 0;
		m_pvData = NULL;
	}
	~CIOReq()
	{
		if (m_pvData != NULL)
		{
			VirtualFree(m_pvData, 0, MEM_RELEASE);
		}
	}
	BOOL AllocBuffer(SIZE_T nBuffSize)
	{
		m_nBuffSize = nBuffSize;
		m_pvData = VirtualAlloc(NULL, m_nBuffSize, MEM_COMMIT, PAGE_READWRITE);
	}

	BOOL Read(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		LARGE_INTEGER r;
		return (::ReadFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}

	BOOL Write(HANDLE hDevice, PLARGE_INTEGER pliOffset = NULL)
	{
		if (pliOffset != NULL)
		{
			Offset = pliOffset->LowPart;
			OffsetHigh = pliOffset->HighPart;
		}
		return (::WriteFile(hDevice, m_pvData, m_nBuffSize, NULL, this));
	}
private:
	SIZE_T m_nBuffSize;
	PVOID m_pvData;
};

#define BUFFSIZE (64 * 1024)
#define MAX_PENDING_IO_REQS 4

#define CK_READ 1
#define CK_WRITE 2

BOOL FileCopy(PCTSTR pszFileSrc, PCTSTR pszFileDst)
{
	BOOL fOk = FALSE;
	chROUNDUP();
}
int main()
{
	try
	{
		
	}
	catch (...)
	{

	}
	return 0;
}