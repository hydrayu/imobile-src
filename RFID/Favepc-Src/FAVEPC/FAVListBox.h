// CFAVListBox.h : header file

#if !defined(AFX_XPLISTBOX_H__B39AA258_724D_1100_93C8_0002A522F1BC__INCLUDED_)
#define AFX_XPLISTBOX_H__B39AA258_724D_1100_93C8_0002A522F1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//_____ D E F I N I T I O N S _______________________________________________
#define		CCB_MAX_COLORS	18		// Colors In List
#define		MAXTABSTOPS		100
//_____ D E C L A R A T I O N _______________________________________________
// CFAVListBox
class CFAVListBox : public CListBox
{
	DECLARE_DYNAMIC(CFAVListBox)

public:
	CFAVListBox();
	virtual ~CFAVListBox();

// Attributes
public:
	int m_cxExtent;
	int m_nTabPosition;
	BOOL m_bColor;
	COLORREF m_ColorWindow;
	COLORREF m_ColorHighlight;
	COLORREF m_ColorWindowText;
	COLORREF m_ColorHighlightText;

	// NOTE - following list must be kept in sync with ColorPickerCB.cpp
	enum COLOR {	Black,  White, Maroon,  Green,
					Olive,  Navy,  Purple,  Teal,
					Silver, Gray,  Red,     Lime,
					Yellow, Blue,  Fuschia, Aqua,
					Default1, Default2};

	void EnableColor(BOOL bEnable);
// Operations
public:
	int AddLine(COLOR tc, COLOR bc, LPCTSTR lpszLine);
	int AddString(LPCTSTR lpszItem);
	void EnableLineNumbers(BOOL bEnable) {m_bLineNumbers = bEnable;}
	int FindString(int nStartAfter, LPCTSTR lpszItem) const;
	int FindStringExact(int nStartAfter, LPCTSTR lpszItem) const;
	COLOR GetBackgroundColor(int nIndex) const;
	COLOR GetTextColor(int nIndex) const;
	int GetText(int nIndex, LPTSTR lpszBuffer) const;
	void GetText(int nIndex, CString& rString) const;
	int GetTextLen(int nIndex) const;
	int GetTextWithColor(int nIndex, LPTSTR lpszBuffer) const;
	void GetTextWithColor(int nIndex, CString& rString) const;
	int GetVisibleLines(void);
	int InsertString(int nIndex, LPCTSTR lpszItem, COLOR tc = Black, COLOR bc = White);
	// http://www.codeproject.com/Articles/1388/Calling-Conventions-Demystified
	int _cdecl Printf(COLOR tc, COLOR bc, UINT nID, LPCTSTR lpszFmt, ...);
	virtual void ResetContent(void);
	int SelectString(int nStartAfter, LPCTSTR lpszItem);
	void SetBackgroundColor(int nIndex, COLOR bc);

	void SetContextMenuId(UINT nId){ m_nContextMenuId = nId; }
	virtual void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void SetGutterColor(COLORREF crGutter){ m_ColorGutter = crGutter; }

	void SetGutterWidth(int nWidth){ m_nGutterWidth = nWidth; }
	void SetLineNoColor(COLORREF crLineNo){ m_ColorLineNo = crLineNo; }
	void SetReaderLogFile(LPCTSTR lpszLogFile) { m_strLogFile = lpszLogFile; }
	void SetRMCLogFile(LPCTSTR lpszLogFile) { m_strRMCLogFile = lpszLogFile; }
	void SetTabPosition(int nSpacesPerTab);
	void SetTextColor(int nIndex, COLOR tc);


// Implementation
protected:
	int			m_nTabStopPositions[MAXTABSTOPS];
	int			m_nSpaceWidth;
	int			m_nAveCharWidth;
	int			m_nGutterWidth;
	UINT		m_nContextMenuId;
	COLORREF	m_ColorGutter;
	COLORREF	m_ColorLineNo;
	CString		m_strLogFile;
	CString		m_strRMCLogFile;
	BOOL		m_bLineNumbers;

	int SearchString(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const;

	virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);

	// Generated message map functions
	//{{AFX_MSG(CFAVListBox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditClearAll();
	afx_msg void OnEditSelectAll();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_XPLISTBOX_H__B39AA258_724D_1100_93C8_0002A522F1BC__INCLUDED_)
