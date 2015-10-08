#include "StdAfx.h"
#include "FAVLipBoard.h"

#pragma warning(disable : 4996)	// disable bogus deprecation warning

//_____ D E C L A R A T I O N _______________________________________________
CFAVLipBoard::CFAVLipBoard(void)
{
}


CFAVLipBoard::~CFAVLipBoard(void)
{
}

//===========================================================================
CString CFAVLipBoard::GetText(void)
//===========================================================================
{
	CString str;
	str = _T("");

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	if (::OpenClipboard(NULL))
	{
		// Request a pointer to the text on the clipboard.
		HGLOBAL hGlobal = ::GetClipboardData(CF_TEXT);

		// If there was no text on the clipboard, we have
		// been returned a NULL handle.
		if (hGlobal)
		{
			// Now we have a global memory handle to the text
			// stored on the clipboard. We have to lock this global
			// handle so that we have access to it.
			LPSTR lpszData = (LPSTR) ::GlobalLock(hGlobal);

			if (lpszData)
			{
				str = lpszData;

				// Now, simply unlock the global memory pointer
				// and close the clipboard.
				::GlobalUnlock(hGlobal);
			}
		}
		::CloseClipboard();
	}
	return str;
}

//===========================================================================
BOOL CFAVLipBoard::GetText(LPSTR lpszBuffer, int nBufSize)
//===========================================================================
{
	ASSERT(lpszBuffer);
	if (lpszBuffer == NULL)
		return FALSE;

	if (nBufSize == 0)
		return FALSE;

	lpszBuffer[0] = 0;

	BOOL bSuccess = FALSE;

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	if (::OpenClipboard(NULL))
	{
		// Request a pointer to the text on the clipboard.
		HGLOBAL hGlobal = ::GetClipboardData(CF_TEXT);

		// If there was no text on the clipboard, we have
		// been returned a NULL handle.
		if (hGlobal)
		{
			// Now we have a global memory handle to the text
			// stored on the clipboard. We have to lock this global
			// handle so that we have access to it.
			LPSTR lpszData = (LPSTR) ::GlobalLock(hGlobal);

			if (lpszData)
			{
				// Now get the size of the text on the clipboard.
#if _MFC_VER < 0x700
				DWORD nSize = ::GlobalSize(hGlobal);
#else
				SIZE_T nSize = ::GlobalSize(hGlobal);
#endif

				// Make sure the text on the clipboard is not longer
				// that the buffer that was allocated for it. If it was
				// snip the text on the clipboard so that it fits.
				if (nSize >= (UINT)nBufSize)
					nSize = nBufSize - 1;

				// Now, copy the text into the return buffer. Note that
				// there will be at least one nul at the end
				ZeroMemory(lpszBuffer, nBufSize);
				CopyMemory(lpszBuffer, lpszData, nSize);

				// Now, simply unlock the global memory pointer
				// and close the clipboard.
				::GlobalUnlock(hGlobal);
				bSuccess = TRUE;
			}
		}
		::CloseClipboard();
	}

	return bSuccess;
}


int CFAVLipBoard::GetTextLength(void)
{
#if _MFC_VER < 0x700
	DWORD nSize = 0;		// Size of clipboard data
#else
	SIZE_T nSize = 0;		// Size of clipboard data
#endif

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	if (::OpenClipboard(NULL))
	{
		// Request a pointer to the text on the clipboard.
		HGLOBAL hGlobal = ::GetClipboardData(CF_TEXT);

		// If there was no text on the clipboard, we have
		// been returned a NULL handle.
		if (hGlobal)
		{
			// Now we have a global memory handle to the text
			// stored on the clipboard. We have to lock this global
			// handle so that we have access to it.
			LPSTR lpszData = (LPSTR) ::GlobalLock(hGlobal);

			if (lpszData)
			{
				// Now get the size of the text on the clipboard.
				nSize = ::GlobalSize(hGlobal);

				// Now, simply unlock the global memory pointer
				// and close the clipboard.
				::GlobalUnlock(hGlobal);
			}
		}
		::CloseClipboard();
	}

	return (int)nSize;
}


BOOL CFAVLipBoard::SetText(LPCTSTR lpszBuffer)
{
	BOOL bSuccess = FALSE;

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed. After opening, empty the clipboard so we
	// can put our text on it.
	if (::OpenClipboard(NULL))
	{
		::EmptyClipboard();

		// Get the size of the string in the buffer that was
		// passed into the function, so we know how much global
		// memory to allocate for the string.
		int nSize = (int)_tcslen(lpszBuffer);

		// Allocate the memory for the string.
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT, (nSize+1)*sizeof(TCHAR));

		// If we got any error during the memory allocation,
		// we have been returned a NULL handle.
		if (hGlobal)
		{
			// Now we have a global memory handle to the text
			// stored on the clipboard. We have to lock this global
			// handle so that we have access to it.
			LPTSTR lpszData = (LPTSTR) ::GlobalLock(hGlobal);

			if (lpszData)
			{
				// Now, copy the text from the buffer into the allocated
				// global memory pointer
				_tcscpy(lpszData, lpszBuffer);

				// Now, simply unlock the global memory pointer,
				// set the clipboard data type and pointer,
				// and close the clipboard.
				::GlobalUnlock(hGlobal);
#ifdef _UNICODE
				::SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
				::SetClipboardData(CF_TEXT, hGlobal);
#endif
				bSuccess = TRUE;
			}
		}
		::CloseClipboard();
	}

	return bSuccess;
}
