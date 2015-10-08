// FAVListBox.cpp : implementation file
//

#include "stdafx.h"
//#include "FAVEPC Utility.h"
#include "FAVListBox.h"
#include "FAVLipBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)	// disable bogus deprecation warning
//_____ D E F I N I T I O N S ________________________________________________
// set resource handle (in case used in DLL)
#ifdef _USRDLL
#define AFXMANAGESTATE AfxGetStaticModuleState
#else
#define AFXMANAGESTATE AfxGetAppModuleState
#endif

// NOTE - following table must be kept in sync with ColorPickerCB.cpp
static COLORREF ColorTable[CCB_MAX_COLORS] = {	RGB(  0,   0,   0),		// Black
												RGB(255, 255, 255),		// White
												RGB(128,   0,   0),		// Maroon
												RGB(  0, 128,   0),		// Green
												RGB(128, 128,   0),		// Olive
												RGB(  0,   0, 128),		// Navy
												RGB(128,   0, 128),		// Purple
												RGB(  0, 128, 128),		// Teal
												RGB(192, 192, 192),		// Silver
												RGB(128, 128, 128),		// Gray
												RGB(255,   0,   0),		// Red
												RGB(  0, 255,   0),		// Lime
												RGB(255, 255,   0),		// Yellow
												RGB(  0,   0, 255),		// Blue
												RGB(255,   0, 255),		// Fuschia
												RGB(  0, 255, 255),		// Aqua
												RGB(255, 128,   0),		// Customer1/Default1
												RGB(  0,  64, 128)		// Customer2/Default2
											};	

//_____ F U N  C T I O N ____________________________________________________
// CFAVListBox
IMPLEMENT_DYNAMIC(CFAVListBox, CListBox)

//===========================================================================
CFAVListBox::CFAVListBox()
//===========================================================================
{

	m_cxExtent				= 0;
	m_nTabPosition			= 8;	// tab stops every 8 columns
	m_bColor				= TRUE;
	m_ColorWindow			= ::GetSysColor(COLOR_WINDOW);
	m_ColorHighlight		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_ColorWindowText		= ::GetSysColor(COLOR_WINDOWTEXT);
	m_ColorHighlightText	= ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	for (int i = 0; i < MAXTABSTOPS; i++)
		m_nTabStopPositions[i] = (i+1) * m_nTabPosition * m_nSpaceWidth;
	m_nSpaceWidth			= 7;
//	m_nAveCharWidth			= 0;
	m_nGutterWidth			= 5;
	m_nContextMenuId		= (UINT)-1;
	m_ColorGutter			= RGB(140,200,200);
	m_ColorLineNo			= RGB(80,80,80);
	m_strLogFile			= _T("");
	m_strRMCLogFile			= _T("");
	m_bLineNumbers			= TRUE;
}

//===========================================================================
CFAVListBox::~CFAVListBox()
//===========================================================================
{
}

//===========================================================================
BEGIN_MESSAGE_MAP(CFAVListBox, CListBox)
//===========================================================================
	//{{AFX_MSG_MAP(CFAVListBox)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_CLEAR, &CFAVListBox::OnEditClear)
	ON_COMMAND(ID_EDIT_COPY, &CFAVListBox::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CFAVListBox::OnEditCut)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CFAVListBox::OnEditClearAll)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CFAVListBox::OnEditSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CFAVListBox message handlers
//===========================================================================
void CFAVListBox::EnableColor(BOOL bEnable)
//===========================================================================
{
	m_bColor = bEnable;
}

//===========================================================================
int CFAVListBox::AddLine(COLOR tc, COLOR bc, LPCTSTR lpszLine)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	CString str1 = lpszLine;

	if (!m_bColor)
	{
		tc = Black;			// to force black-only text
		bc = White;
	}

	UINT nColor = (UINT) tc;
	ASSERT(nColor < CCB_MAX_COLORS);
	if (nColor >= CCB_MAX_COLORS)
		tc = Black;

	// don't display \r or \n characters
	int i = 0;
	while ((i = str1.FindOneOf(_T("\r\n"))) != -1)
		str1.SetAt(i, _T(' '));

	if (!str1.IsEmpty() && !m_strLogFile.IsEmpty())
	{
		// Retrieves the current system date and time.  The system 
		// time is expressed in Coordinated Universal Time (UTC).
		CTime ctReader = CTime::GetCurrentTime();
		SYSTEMTIME stReader;
		GetLocalTime(&stReader);
		UINT msReader = stReader.wMilliseconds;
		CString strMillisecond;
		if(msReader < 10){
			strMillisecond.Format(_T(":00%d"), msReader);
		}
		else if(msReader < 100){
			strMillisecond.Format(_T(":0%d"), msReader);
		}
		else{
			strMillisecond.Format(_T(":%d"), msReader);
		}

		// Show the time in a list box.
		CString strReader = ctReader.Format(_T("%Y/%m/%d %H:%M:%S"));
		strReader = strReader + strMillisecond; 

		TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH*2];
		szComputerName[0] = _T('\0');
		DWORD dwSize = sizeof(szComputerName)/sizeof(TCHAR)-1;
		GetComputerName(szComputerName, &dwSize);
		//GetUserName(szComputerName, &dwSize);
		strReader += _T('[');
		strReader += szComputerName;
		strReader += _T("] ");
		strReader += str1;
		strReader += _T("\r\n");

		// Write string to log file
		HANDLE hReaderFile = ::CreateFile(m_strLogFile, GENERIC_WRITE, FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hReaderFile != INVALID_HANDLE_VALUE)
		{
			// set to end of file - always append
			DWORD dwRC = ::SetFilePointer(	hReaderFile,		// handle to file
											0,					// bytes to move pointer
											NULL,				// bytes to move pointer
											FILE_END);			// starting point

			if (dwRC != INVALID_SET_FILE_POINTER)
			{
				DWORD dwWritten = 0;
				::WriteFile(hReaderFile,								// handle to file
							strReader,									// data buffer
							(DWORD)strReader.GetLength()*sizeof(TCHAR),	// number of bytes to write
							&dwWritten,									// number of bytes written
							NULL);										// overlapped buffer
			}

			::CloseHandle(hReaderFile);
		}
	}
	else if(!str1.IsEmpty() && !m_strRMCLogFile.IsEmpty()){
		// Retrieves the current system date and time.  The system 
		// time is expressed in Coordinated Universal Time (UTC).
		CTime ctRMC = CTime::GetCurrentTime();
		SYSTEMTIME stRMC;
		GetLocalTime(&stRMC);
		UINT msRMC = stRMC.wMilliseconds;
		CString strMillisecond;
		if(msRMC < 10){
			strMillisecond.Format(_T(":00%d"), msRMC);
		}
		else if(msRMC < 100){
			strMillisecond.Format(_T(":0%d"), msRMC);
		}
		else{
			strMillisecond.Format(_T(":%d"), msRMC);
		}

		// Show the time in a list box.
		CString strRMC = ctRMC.Format(_T("%Y/%m/%d %H:%M:%S"));
		strRMC = strRMC + strMillisecond; 

		TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH*2];
		szComputerName[0] = _T('\0');
		DWORD dwSize = sizeof(szComputerName)/sizeof(TCHAR)-1;
		GetComputerName(szComputerName, &dwSize);
		//GetUserName(szComputerName, &dwSize);
		strRMC += _T('[');
		strRMC += szComputerName;
		strRMC += _T("] ");
		strRMC += str1;
		strRMC += _T("\r\n");

		// Write string to log file
		HANDLE hRMCFile = ::CreateFile(m_strRMCLogFile, GENERIC_WRITE, FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hRMCFile != INVALID_HANDLE_VALUE)
		{
			// set to end of file - always append
			DWORD dwRC = ::SetFilePointer(	hRMCFile,			// handle to file
											0,					// bytes to move pointer
											NULL,				// bytes to move pointer
											FILE_END);			// starting point

			if (dwRC != INVALID_SET_FILE_POINTER)
			{
				DWORD dwWritten = 0;
				::WriteFile(hRMCFile,							// handle to file
					strRMC,										// data buffer
					(DWORD)strRMC.GetLength()*sizeof(TCHAR),	// number of bytes to write
					&dwWritten,									// number of bytes written
					NULL);										// overlapped buffer
			}

			::CloseHandle(hRMCFile);
		}
	}

	// First character in string is color -- add 1 to color to avoid asserts by CString class
	CString t = _T("");
	t += (TCHAR) (tc + 1);
	t += (TCHAR) (bc + 1);
	t += str1;

	// Try to add the string to the listbox
	i = CListBox::AddString(t);
	if (i == LB_ERRSPACE)
	{
		// Will get LB_ERRSPACE if listbox is out of memory
		int n = GetCount();

		if (n == LB_ERR)
			return LB_ERR;

		if (n < 2)
			return LB_ERR;

		// Try to delete some strings to free up some room --
		// don't spend too much time deleting strings, since we might be getting a burst of messages
		n = (n < 20) ? (n-1) : 20;
		if (n <= 0)
			n = 1;

		SetRedraw(FALSE);
		for (i = 0; i < n; i++)
			DeleteString(0);

		i = CListBox::AddString(t);

		SetRedraw(TRUE);
	}

	if (i >= 0)
	{
		SetTopIndex(i);
		SetCurSel(-1);
	}
	return i;
}

//===========================================================================
int CFAVListBox::AddString(LPCTSTR lpszItem)
//===========================================================================
{
	return AddLine(CFAVListBox::Black, CFAVListBox::White, lpszItem);
}

//===========================================================================
int CFAVListBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
//===========================================================================
{
	return SearchString(nStartAfter, lpszItem, FALSE);
}

//===========================================================================
int CFAVListBox::FindStringExact(int nStartAfter, LPCTSTR lpszItem) const
//===========================================================================
{
	return SearchString(nStartAfter, lpszItem, TRUE);
}

//===========================================================================
CFAVListBox::COLOR CFAVListBox::GetBackgroundColor(int nIndex) const
//===========================================================================
{
	COLOR BackgroundColor = White;

	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		int nLen = CListBox::GetTextLen(nIndex);
		if (nLen != LB_ERR)
		{
			TCHAR *buf = new TCHAR [nLen + CCB_MAX_COLORS];
			ASSERT(buf);
			if (buf && (GetTextWithColor(nIndex, buf) != LB_ERR))
			{
				// Get background color from second character in string -
				// NOTE: 1 was added to color index to avoid asserts by CString
				int iback = int (buf[1] - 1);
				BackgroundColor = (COLOR) iback;
			}
			if (buf)
				delete [] buf;
			buf = NULL;
		}
	}

	return BackgroundColor;
}

//===========================================================================
CFAVListBox::COLOR CFAVListBox::GetTextColor(int nIndex) const
//===========================================================================
{
	COLOR TextColor = Black;

	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		int nLen = CListBox::GetTextLen(nIndex);
		if (nLen != LB_ERR)
		{
			TCHAR *buf = new TCHAR [nLen + CCB_MAX_COLORS];
			ASSERT(buf);
			if (buf && (GetTextWithColor(nIndex, buf) != LB_ERR))
			{
				// set text color from first character in string -
				// NOTE: 1 was added to color index to avoid asserts by CString
				int itext = int (buf[0] - 1);
				TextColor = (COLOR) itext;
			}
			if (buf)
				delete [] buf;
			buf = NULL;
		}
	}

	return TextColor;
}

//===========================================================================
// GetText - for compatibility with CListBox (no color bytes)
int CFAVListBox::GetText(int nIndex, LPTSTR lpszBuffer) const
//===========================================================================
{
	ASSERT(lpszBuffer);
	if (lpszBuffer == NULL)
		return LB_ERR;

	lpszBuffer[0] = 0;

	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	int nRet = CListBox::GetText(nIndex, lpszBuffer);

	size_t n = _tcslen(lpszBuffer);
	if (n > 2)
		memcpy(&lpszBuffer[0], &lpszBuffer[2], (n-1)*sizeof(TCHAR));	// copy nul too

	return nRet;
}

//===========================================================================
// GetText - for compatibility with CListBox (no color bytes)
void CFAVListBox::GetText(int nIndex, CString& rString) const
//===========================================================================
{
	rString.Empty();

	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return;
	}

	CString str = _T("");
	CListBox::GetText(nIndex, str);
	if ((!str.IsEmpty()) && (str.GetLength() > 2))
		rString = str.Mid(2);
}

//===========================================================================
// GetTextLen - for compatibility with CListBox (no color bytes)
int CFAVListBox::GetTextLen(int nIndex) const
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	int nTextLen = CListBox::GetTextLen(nIndex);
	if (nTextLen != LB_ERR && nTextLen >= 2)
		nTextLen -= 2;
	return nTextLen;
}

//===========================================================================
// GetTextWithColor - get text string with color bytes
int CFAVListBox::GetTextWithColor(int nIndex, LPTSTR lpszBuffer) const
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	ASSERT(lpszBuffer);
	lpszBuffer[0] = 0;
	return CListBox::GetText(nIndex, lpszBuffer);
}

//===========================================================================
// GetTextWithColor - get text string with color bytes
void CFAVListBox::GetTextWithColor(int nIndex, CString& rString) const
//===========================================================================
{
	rString.Empty();

	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return;
	}

	CListBox::GetText(nIndex, rString);
}

//===========================================================================
int CFAVListBox::GetVisibleLines(void)
//===========================================================================
{
	int nCount = 0;

	CDC* pDC = GetDC();

	if (pDC)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		int h = tm.tmHeight + tm.tmInternalLeading;
		ReleaseDC(pDC);

		CRect rect;
		GetClientRect(&rect);
		nCount = rect.Height() / h;
	}
	return nCount;
}

//===========================================================================
// InsertString - override to add text color
int CFAVListBox::InsertString(	int nIndex,
								LPCTSTR lpszItem, 
								COLOR tc/*= Black*/, 
								COLOR bc/*= White*/)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	CString str = lpszItem;

	if (!m_bColor)
	{
		tc = Black;			// to force black-only text
		bc = White;
	}

	UINT nColor = (UINT) tc;
	ASSERT(nColor < CCB_MAX_COLORS);
	if (nColor >= CCB_MAX_COLORS)
		tc = Black;

	// Searches lpszItem string for the first character that matches  \r or \n.
	// Clear and don't display \r or \n characters
	int nPos = 0;
	while ((nPos = str.FindOneOf(_T("\r\n"))) != -1)
		str.SetAt(nPos, ' ');

	// First character in string is color -- add 1 to color
	// to avoid asserts by CString class
	CString t = _T("");
	t += (char) (tc + 1);
	t += (char) (bc + 1);
	t += str;

	// Try to insert the string into the listbox
	nPos = CListBox::InsertString(nIndex, t);

	return nPos;
}

//=============================================================================	
int _cdecl CFAVListBox::Printf(COLOR tc, COLOR bc, UINT nID, LPCTSTR lpszFmt, ...)
//=============================================================================	
{
	AFX_MANAGE_STATE(AFXMANAGESTATE())

	TCHAR buf[2048], fmt[1024];
	va_list marker;

	// load format string from string resource if
	// a resource ID was specified
	if (nID)
	{
		CString s = _T("");
		if (!s.LoadString(nID))
		{
			_stprintf(s.GetBufferSetLength(80), _T("Failed to load string resource %u"),
				nID);
			s.ReleaseBuffer(-1);
		}
		_tcsncpy(fmt, s, sizeof(fmt)/sizeof(TCHAR)-1);
	}
	else
	{
		// Format string was passed as parameter
		_tcsncpy(fmt, lpszFmt, sizeof(fmt)/sizeof(TCHAR)-1);
	}
	fmt[sizeof(fmt)/sizeof(TCHAR)-1] = 0;

	// Combine output string and variables
	va_start(marker, lpszFmt);
	_vsntprintf(buf, (sizeof(buf)/sizeof(TCHAR))-1, fmt, marker);
	va_end(marker);
	buf[sizeof(buf)/sizeof(TCHAR)-1] = 0;

	return AddLine(tc, bc, buf);
}

//===========================================================================
void CFAVListBox::ResetContent(void)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		//CFAVListBox::ResetContent();	// Delete all the items from the list box.
		// Delete every other item from the list box.
		// Get the indexes of all the selected items.
		UINT nCount = CListBox::GetSelCount();
		CArray<int,int> aryListBoxSel;
		aryListBoxSel.SetSize(nCount);
		CString _text;
		CListBox::GetSelItems(nCount, aryListBoxSel.GetData()); 
		for (UINT i = 0; i < nCount; i++)
		{
			CListBox::GetText(aryListBoxSel[i]-i, _text);
			CListBox::DeleteString(aryListBoxSel[i]-i);
		}

		// Dump the selection array.
		AFXDUMP(aryListBoxSel);
	}
}

//===========================================================================
int CFAVListBox::SelectString(int nStartAfter, LPCTSTR lpszItem)
//===========================================================================
{
	int nIndex = SearchString(nStartAfter, lpszItem, FALSE);
	if (nIndex != LB_ERR)
		SetCurSel(nIndex);
	return nIndex;
}

//===========================================================================
void CFAVListBox::SetBackgroundColor(int nIndex, COLOR bc)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		int nLen = CListBox::GetTextLen(nIndex);
		if (nLen != LB_ERR)
		{
			TCHAR *buf = new TCHAR [nLen + CCB_MAX_COLORS];
			ASSERT(buf);
			if (buf && (GetTextWithColor(nIndex, buf) != LB_ERR))
			{
				// Get text color from first character in string -
				// NOTE: 1 was added to color index to avoid asserts by CString
				int tc = (int) buf[0];

				// delete old string
				CListBox::DeleteString(nIndex);

				CString s = &buf[2];
				CString t = _T("");
				t += (char) tc;
				t += (char) (bc + 1);	// add 1 to color to avoid asserts by CString class
				t += s;
				CListBox::InsertString(nIndex, t);
			}
			if (buf)
				delete [] buf;
			buf = NULL;
		}
	}
}

//===========================================================================
void CFAVListBox::SetFont(CFont* pFont, BOOL bRedraw)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		CListBox::SetFont(pFont, bRedraw);

		CDC* pDC = GetDC();

		if (pDC)
		{
			CFont *pOldFont = pDC->SelectObject(pFont);

			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);
			int h = tm.tmHeight;
			SetItemHeight(0, h);

			pDC->SelectObject(pOldFont);
			// Retrieves the widths of individual characters in a consecutive group of characters
			// from the current font, using m_hAttribDC, the input device context.
			pDC->GetCharWidth((UINT) ' ', (UINT) ' ', &m_nSpaceWidth);
			pDC->GetCharWidth((UINT) '9', (UINT) '9', &m_nAveCharWidth);

			for (int i = 0; i < MAXTABSTOPS; i++)
				m_nTabStopPositions[i] = (i+1) * m_nAveCharWidth * m_nTabPosition;

			ReleaseDC(pDC);
		}

		m_cxExtent = 0;
	}
}

//===========================================================================
void CFAVListBox::SetTabPosition(int nSpacesPerTab)
//===========================================================================
{
	ASSERT(nSpacesPerTab > 0 && nSpacesPerTab < 11);

	m_nTabPosition = nSpacesPerTab;

	CDC* pDC = GetDC();

	if (pDC)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		pDC->GetCharWidth((UINT) ' ', (UINT) ' ', &m_nSpaceWidth);
		pDC->GetCharWidth((UINT) '9', (UINT) '9', &m_nAveCharWidth);

		for (int i = 0; i < MAXTABSTOPS; i++)
			m_nTabStopPositions[i] = (i+1) * m_nAveCharWidth * m_nTabPosition;

		ReleaseDC(pDC);
	}
}

//===========================================================================
void CFAVListBox::SetTextColor(int nIndex, COLOR tc)
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		int nLen = CListBox::GetTextLen(nIndex);
		if (nLen != LB_ERR)
		{
			TCHAR *buf = new TCHAR [nLen + CCB_MAX_COLORS];
			ASSERT(buf);
			if (buf && (GetTextWithColor(nIndex, buf) != LB_ERR))
			{
				// get background color from second character in string -
				// NOTE: 1 was added to color index to avoid asserts by CString
				int bc = (int) buf[1];

				// delete old string
				CListBox::DeleteString(nIndex);

				CString s = &buf[2];
				CString t = _T("");
				t += (char) (tc + 1);	// add 1 to color to avoid asserts by CString class
				t += (char) bc;
				t += s;
				CListBox::InsertString(nIndex, t);
			}
			if (buf)
				delete [] buf;
			buf = NULL;
		}
	}
}

//===========================================================================
int CFAVListBox::SearchString(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const
//===========================================================================
{
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return LB_ERR;
	}

	// start the search after specified index
	int nIndex = nStartAfter + 1;

	int nCount = GetCount();
	if (nCount == LB_ERR)
		return LB_ERR;

	// convert string to search for to lower case
	CString strItem = lpszItem;
	strItem.MakeLower();
	int nItemSize = strItem.GetLength();

	CString strText = _T("");

	// search until end
	for ( ; nIndex < nCount; nIndex++)
	{
		GetText(nIndex, strText);
		strText.MakeLower();
		if (!bExact)
			strText = strText.Left(nItemSize);
		if (strText == strItem)
			return nIndex;
	}

	// if we started at beginning there is no more to do, search failed
	if (nStartAfter == -1)
		return LB_ERR;

	// search until we reach beginning index
	for (nIndex = 0; (nIndex <= nStartAfter) && (nIndex < nCount); nIndex++)
	{
		GetText(nIndex, strText);
		strText.MakeLower();
		if (!bExact)
			strText = strText.Left(nItemSize);
		if (strText == strItem)
			return nIndex;
	}

	return LB_ERR;
}

//===========================================================================
int CFAVListBox::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
//===========================================================================
{

	// TODO:  Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2

	return 0;
}

//===========================================================================
void CFAVListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//===========================================================================
{
	// TODO:  Add your code to draw the specified item
	COLORREF oldtextcolor, oldbackgroundcolor;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	pDC->GetCharWidth((UINT) ' ', (UINT) ' ', &m_nSpaceWidth);
	pDC->GetCharWidth((UINT) 'c', (UINT) 'c', &m_nAveCharWidth);

	for (int i = 0; i < MAXTABSTOPS; i++)
		m_nTabStopPositions[i] = (i+1) * m_nAveCharWidth * m_nTabPosition;

	// draw focus rectangle when no items in listbox
	if (lpDrawItemStruct->itemID == (UINT)-1)
	{
		if (lpDrawItemStruct->itemAction & ODA_FOCUS)
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		return;
	}
	else
	{
		int selChange   = lpDrawItemStruct->itemAction & ODA_SELECT;
		int focusChange = lpDrawItemStruct->itemAction & ODA_FOCUS;
		int drawEntire  = lpDrawItemStruct->itemAction & ODA_DRAWENTIRE;

		if (selChange || drawEntire)
		{
			BOOL sel = lpDrawItemStruct->itemState & ODS_SELECTED;

			int nLen = CListBox::GetTextLen(lpDrawItemStruct->itemID);
			if (nLen != LB_ERR)
			{
				TCHAR *buf = new TCHAR [nLen + CCB_MAX_COLORS];
				ASSERT(buf);
				if (buf && (GetTextWithColor(lpDrawItemStruct->itemID, buf) != LB_ERR))
				{
					CRect rectItem(lpDrawItemStruct->rcItem);
					CSize size;

					if (m_bLineNumbers)
					{
						// draw gutter & line no.
						UINT index = GetTopIndex();
						UINT n = rectItem.top / rectItem.Height();
						index += n;
						//TRACE("index=%d\n", index);
						CString strLineNo = _T("");
						for (int i = 0; i < m_nGutterWidth; i++)
							strLineNo += _T('9');
						if (strLineNo.IsEmpty())
							strLineNo = _T('9');
						size = pDC->GetTextExtent(strLineNo);
						strLineNo = _T("");
						strLineNo.Format(_T("%d"), index+1);
						rectItem.right = rectItem.left + size.cx + 6;
						pDC->SetBkColor(m_ColorGutter);
						pDC->SetTextColor(m_ColorLineNo);
						// fill the gutter with the gutter color the fast way
						pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rectItem, NULL, 0, NULL);
						rectItem.left += 2;
						rectItem.right -= 2;
						pDC->DrawText(strLineNo, &rectItem, DT_RIGHT);
						// restore drawing rect
						rectItem.left = rectItem.right + 2;
						rectItem.right = lpDrawItemStruct->rcItem.right;
					}

					// set text color from first character in string -
					// NOTE: 1 was added to color index to avoid asserts by CString
					int itext = int (buf[0] - 1);

					// set background color from second character in string -
					// NOTE: 1 was added to color index to avoid asserts by CString
					int iback = int (buf[1] - 1);
					buf[0] = _T(' ');
					buf[1] = _T(' ');
					COLORREF textcolor = sel ? m_ColorHighlightText : ColorTable[itext];
					oldtextcolor = pDC->SetTextColor(textcolor);
					COLORREF backgroundcolor = sel ? m_ColorHighlight : ColorTable[iback];
					oldbackgroundcolor = pDC->SetBkColor(backgroundcolor);

					// fill the rectangle with the background color the fast way
					pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rectItem, NULL, 0, NULL);

					pDC->TabbedTextOut(rectItem.left+2, rectItem.top, &buf[2],
						(int)_tcslen(&buf[2]), MAXTABSTOPS, (LPINT)m_nTabStopPositions, 0);

					size = pDC->GetOutputTextExtent(&buf[2]);
					int nScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
					size.cx += nScrollBarWidth;	// in case of vertical scrollbar

					int cxExtent = (size.cx > m_cxExtent) ? size.cx : m_cxExtent;

					if (cxExtent > m_cxExtent)
					{
						m_cxExtent = cxExtent;
						SetHorizontalExtent(m_cxExtent+(m_cxExtent/32));
					}
				}
				if (buf)
					delete [] buf;
			}
		}

		if (focusChange || (drawEntire && (lpDrawItemStruct->itemState & ODS_FOCUS)))
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	}
}

//===========================================================================
void CFAVListBox::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
//===========================================================================
{
	// TODO:  Add your code to determine the size of specified item
}

//===========================================================================
void CFAVListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default

	CListBox::OnLButtonDblClk(nFlags, point);
}

//===========================================================================
void CFAVListBox::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here
	AFX_MANAGE_STATE(AFXMANAGESTATE())

	if (m_nContextMenuId == -1)
	{
		TRACE(_T("no context menu\n"));
		return;
	}
	CMenu menu;
	if (!menu.LoadMenu(m_nContextMenuId))
	{
		TRACE(_T("ERROR failed to load %d\n"), m_nContextMenuId);
		return;
	}

	menu.GetSubMenu(0)->TrackPopupMenu(0,
		point.x, point.y, this, NULL);
}

//===========================================================================
BOOL CFAVListBox::OnEraseBkgnd(CDC* pDC)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	if (m_bLineNumbers)
	{
		CFont *pOldFont = NULL;
		CFont *pFont = GetFont();
		if (pFont == NULL)
			pFont = GetParent()->GetFont();
		if (pFont)
			pOldFont = pDC->SelectObject(pFont);

		CRect rect;
		GetClientRect(&rect);
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		// Fill the background with the window color the fast way
		pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

		CString strLineNo = _T("");
		for (int i = 0; i < m_nGutterWidth; i++)
			strLineNo += _T('9');
		if (strLineNo.IsEmpty())
			strLineNo = _T('9');
		CSize size = pDC->GetTextExtent(strLineNo);
		CRect rectGutter(rect);
		rectGutter.right = rectGutter.left + size.cx + 6;
		pDC->SetBkColor(m_ColorGutter);

		// Fill the gutter with the gutter color the fast way
		pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rectGutter, NULL, 0, NULL);
		if (pOldFont)
			pDC->SelectObject(pOldFont);

		return TRUE;
	}
	else
	{
		return CListBox::OnEraseBkgnd(pDC);
	}
}

//===========================================================================
void CFAVListBox::OnEditClear()
//===========================================================================
{
	// TODO: Add your command handler code here
	ResetContent();
}

//===========================================================================
void CFAVListBox::OnEditCopy()
//===========================================================================
{
	// TODO: Add your command handler code here
	CString str = _T("");

	int nCount = GetCount();
	int nSel = 0;

	for (int i = 0; i < nCount; i++)
	{
		if (GetSel(i) > 0)
		{
			CString s = _T("");
			GetText(i, s);
			if (!s.IsEmpty())
			{
				nSel++;
				s.TrimLeft(_T("\r\n"));
				s.TrimRight(_T("\r\n"));
				if (s.Find(_T('\n')) == -1)
					s += _T("\r\n");
				s.Replace(_T("\t"), _T(" "));
				str += s;
			}
		}
	}

	if (!str.IsEmpty())
		CFAVLipBoard::SetText(str);
}

//===========================================================================
void CFAVListBox::OnEditCut()
//===========================================================================
{
	// TODO: Add your command handler code here
}

//===========================================================================
void CFAVListBox::OnEditClearAll()
//===========================================================================
{
	// TODO: Add your command handler code here
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		CListBox::ResetContent();

		m_cxExtent = 0;

		SetHorizontalExtent(m_cxExtent);
	}
}

//===========================================================================
void CFAVListBox::OnEditSelectAll()
//===========================================================================
{
	// TODO: Add your command handler code here
	if (!::IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
	}
	else
	{
		SelItemRange(TRUE, 0, GetCount()-1);
	}
}
