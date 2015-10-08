// CFAVLipBoard.h : header file
// Provides static methods that facilitate transferring data to and from the system Clipboard.
#if !defined(AFX_XPLIPBOARD_H__B49AC258_524D_1200_93C8_0012A524F1BC__INCLUDED_)
#define AFX_XPLIPBOARD_H__B49AC258_524D_1200_93C8_0012A524F1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFAVLipBoard
{
public:
	CFAVLipBoard(void);
	~CFAVLipBoard(void);
	static CString GetText(void);
	static BOOL GetText(LPSTR lpszBuffer, int nBufSize);
	static int GetTextLength(void);
	static BOOL SetText(LPCTSTR lpszBuffer);
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_XPLIPBOARD_H__B49AC258_524D_1200_93C8_0012A524F1BC__INCLUDED_)