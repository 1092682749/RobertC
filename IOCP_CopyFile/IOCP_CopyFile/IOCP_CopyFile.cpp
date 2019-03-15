#include "pch.h"
#include <Windowsx.h>
#include <iostream>
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "CmnHdr.h"
#include "EnsureCleanup.h"
#include "IoCompletionPort.h"

#define IDC_PATHNAME                    100
#define IDC_SRCFILE                     101
#define IDC_SRCFILESIZE                 102
#define IDI_FILECOPY                    103
#define IDD_FILECOPY                    104

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        105
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif


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
		return(m_pvData != NULL);
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
	BOOL bOk = FALSE;
	// ���Ը�ֵ�������������һ��
	LARGE_INTEGER liFileSizeSrc = { 0 }, liFileSizeDst;
	try
	{
		{
			CEnsureCloseFile hFIleSrc = CreateFile(pszFileSrc,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
				NULL);
			if (hFIleSrc.IsInvalid()) goto leave;

			GetFileSizeEx(hFIleSrc, &liFileSizeSrc);
			liFileSizeDst.QuadPart = chROUNDUP(liFileSizeSrc.QuadPart, BUFFSIZE);
			CEnsureCloseFile hFileDst = CreateFile(
				pszFileDst, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, hFIleSrc
			);
			if (hFileDst.IsInvalid())
			{
				goto leave;
			}
			SetFilePointer(hFileDst, liFileSizeDst.QuadPart, NULL, FILE_BEGIN);
			SetEndOfFile(hFileDst);
			CIOCP iocp(0);
			iocp.AssociateDevice(hFIleSrc, CK_READ);
			iocp.AssociateDevice(hFileDst, CK_WRITE);

			CIOReq ior[MAX_PENDING_IO_REQS];
			LARGE_INTEGER liNextReadOffset = { 0 };
			int nReadsInProgress = 0;
			int nWritesInProGress = 0;

			for (int nIOReq = 0; nIOReq < _countof(ior); nIOReq++)
			{
				chVERIFY(ior[nIOReq].AllocBuffer(BUFFSIZE));
				nWritesInProGress++;
				iocp.PostStatus(CK_WRITE, 0, &ior[nIOReq]);
			}
			BOOL bResult = FALSE;

			while ((nReadsInProgress > 0) || (nWritesInProGress > 0))
			{
				ULONG_PTR CompletionKey;
				DWORD dwNumBytes;
				CIOReq *pior;
				bResult = iocp.GetStatus(&CompletionKey, &dwNumBytes, (OVERLAPPED**)&pior, INFINITE);
				switch (CompletionKey)
				{
				case CK_READ:
					nReadsInProgress--;
					bResult = pior->Write(hFileDst);
					nWritesInProGress++;
					break;
				case CK_WRITE:
					nWritesInProGress--;
					if (liNextReadOffset.QuadPart < liFileSizeDst.QuadPart)
					{
						bResult = pior->Read(hFIleSrc, &liNextReadOffset);
						nReadsInProgress--;
						liNextReadOffset.QuadPart += BUFFSIZE;
					}
					break;
				}

			}
			bOk = TRUE;
		
		}
	leave:;
	}
	catch (const std::exception&)
	{

	}
	if (bOk)
	{
		CEnsureCloseFile hFileDst = CreateFile(pszFileDst, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFileDst.IsValid())
		{
			SetFilePointerEx(hFileDst, liFileSizeSrc, NULL, FILE_BEGIN);
			SetEndOfFile(hFileDst);
		}
	}
	return (bOk);
}
BOOL Dlg_OnIntDialog(HWND hWnd, HWND hWndFocus, LPARAM IParam)
{
	chSETDLGICONS(hWnd, IDI_FILECOPY);

	EnableWindow(GetDlgItem(hWnd, IDOK), FALSE);
	return TRUE;
}

void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	TCHAR szPathname[_MAX_PATH];

	switch (id)
	{
	case IDCANCEL:
		EndDialog(hWnd, id);
		break;
	case IDOK:
		Static_GetText(GetDlgItem(hWnd, IDC_SRCFILE),
			szPathname, _countof(szPathname));
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		chMB(FileCopy(szPathname, TEXT("FileCopy.cpy")) ? "Success" : "Faild");
		break;
	case IDC_PATHNAME:
		OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
		ofn.hwndOwner = hWnd;
		TCHAR c[] = TEXT("*.*\0");
		ofn.lpstrFile = c;
		lstrcpy(szPathname, TEXT("*.*"));
		ofn.lpstrFile = szPathname;
		ofn.nMaxFile = _countof(szPathname);
		ofn.lpstrTitle = TEXT("Select file to copy");
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
		BOOL bOk = GetOpenFileName(&ofn);
		if (bOk)
		{
			Static_SetText(GetDlgItem(hWnd, IDC_SRCFILE), szPathname);
			CEnsureCloseFile hFile = CreateFile(szPathname, 0, 0, NULL,
				OPEN_EXISTING, 0, NULL);
			if (hFile.IsValid())
			{
				LARGE_INTEGER liFileSize;
				GetFileSizeEx(hFile, &liFileSize);
				SetDlgItemInt(hWnd, IDC_SRCFILESIZE, liFileSize.LowPart, FALSE);
			}
		}
		EnableWindow(GetDlgItem(hWnd, IDOK), bOk);
			break;
	}
}
INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnIntDialog);
		chHANDLE_DLGMSG(hWnd, WM_COMMAND, Dlg_OnCommand);
	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int)
{
	DialogBox(hInstExe, MAKEINTRESOURCE(IDD_FILECOPY), NULL, Dlg_Proc);
	return 0;
}