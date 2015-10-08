#pragma once

//_____ I N C L U D E S _____________________________________________________
#include "FAVColourPopup.h"


//_____ D E C L A R A T I O N _______________________________________________
// CXColourPicker window
void AFXAPI DDX_ColourPicker(CDataExchange * pDX, int nIDC, COLORREF& crColor);
void AFXAPI DDX_ColourPickerCustom(CDataExchange * pDX, int nIDC, CString& strCustomColors);

// CFAVColourPicker
class CFAVColourPicker : public CButton
{
	DECLARE_DYNAMIC(CFAVColourPicker)

// Construction
public:
	CFAVColourPicker();
	virtual ~CFAVColourPicker();

// Attributes
protected:
	int			m_nSelectedRow, m_nSelectedCol;
	BOOL		m_bTrackSelection;
	BOOL		m_bPopupActive;
	COLORREF	m_crColor;
	COLORREF	m_crEnabledColor;
	COLORREF	m_CustomColors[16];
	CRect		m_ArrowRect;
	COLORREF	m_DefaultColor;
	CBrush		m_brush;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFAVColourPicker)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSize GetArrowSize(void);
public:
	void SetColor(COLORREF crColor);
	COLORREF GetColor(void);
	void SetCustomColors(COLORREF * pCustomColors);
	void GetCustomColors(COLORREF * pCustomColors);
	void SetCustomColors(LPCTSTR lpszCustomColors);
	CString GetCustomColors(void);

	// Generated message map functions
	//{{AFX_MSG(CFAVColourPicker)
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg LONG OnSelchange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelEndCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnBnClicked();
	DECLARE_MESSAGE_MAP()

};


