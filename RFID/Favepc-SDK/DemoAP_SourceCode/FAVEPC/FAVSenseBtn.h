// FAVSenseBtn.h : header file
//
/**************************************************************************
* FILE:			FAVSenseBtn.h
*-------------------------------------------------------------------
* COMPANY:		FAVEPC     
* VERSION:		V1.0
* CREATED:		2012/6/17 ¤U¤È 12:27:56
* DATE:			2012/06/29
* AUTHOR:		Bruce
* COMPILER:		Visual Studio 2010
*-------------------------------------------------------------------
* PURPOSE:
*				Button derivative class

* NOTES:
*				None
* Global Variables:
*				None
*-------------------------------------------------------------------
* REVISION	Date			User	Description
* V1.0		2012/06/17		Bruce	1. First create version
* V1.1		2012/06/29		Bruce	1. Add button style function
* 
*-------------------------------------------------------------------
***************************************************************************/
#if !defined(AFX_XPFAVSENSEBTN_H__FA07A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_)
#define AFX_XPFAVSENSEBTN_H__FA07A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxext.h"
#define	 ROUNDRECT		0
#define	 ELLIPSE		1
#define	 UPTRIANGLE		2
#define	 DOWNTRIANGLE	3

//_____ D E C L A R A T I O N _______________________________________________
// CFAVSenseBtn
class CFAVSenseBtn : public CBitmapButton
{
	DECLARE_DYNAMIC(CFAVSenseBtn)
// Construction
public:
	CFAVSenseBtn();
	BOOL		m_BtnEnable;
	UINT		m_uBtnEnable;
	UINT		m_uBtnDown;
	UINT		m_uBtnNomal;
	UINT		m_uBtnMove;
	
	BOOL		bInSenseBtn;
	COLORREF	TextColor;

	void SetTextColor(COLORREF crColor); 
	void SetBtnState(BOOL bValue){ this->m_BtnEnable = bValue;}
	BOOL GetBtnState(){return this->m_BtnEnable;}
protected:
	// Add Round rect button function
	CPen		m_BoundryPen;

	CPen		m_InsideBoundryPenLeft;
	CPen		m_InsideBoundryPenRight;
	CPen		m_InsideBoundryPenTop;
	CPen		m_InsideBoundryPenBottom;

	CPen		m_InsideBoundryPenLeftSel;
	CPen		m_InsideBoundryPenRightSel;
	CPen		m_InsideBoundryPenTopSel;
	CPen		m_InsideBoundryPenBottomSel;

	// Button BK color: valid and invalid
	CBrush		m_FillActive;
	CBrush		m_FillInactive;

	// Mouse state
	BOOL		m_bOver;
	BOOL		m_bTracking;
	BOOL		m_bSelected;
	BOOL		m_bFocus;

	CBitmap		m_NormalBitmap[12];
	CBitmap		m_PressedBitmap[12];
	CBitmap		m_FocusBitmap[12];
	CBitmap		m_OverBitmap[12];

	CBitmap		m_MaskBitmap;
	CBitmap		m_BackBitmap;

	BOOL		m_bIsMask;
	BOOL		m_bIsMaskRgn;
	BOOL		m_bIsBackBmp;
	CRect		m_rectBackRect;
	

public:
	UINT		m_nBtnStyle;
	//enum BTNSTYLE{ROUNDRECT, ELLIPSE, UPTRIANGLE, DOWNTRIANGLE}m_BtnStyle;
	void SetRgnStyle(UINT nBtnStyle);
	void SetBitmapId(UINT nIndex, int nNormal, int nMove, int nPressed, int nOver);
	void SetMaskBitmapId(int mask, bool action);
	static HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance = RGB(0,0,0));
	void SetRgnMask(int nMaskBmpId, bool nAction);
	void SetBackBmp(int nBgdBmpId, CRect rect);

// Overrides	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFAVSenseBtn)
public: 
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawBK(CDC *pDC, UINT ResID);
	void DrawEllipse(CDC* pDC, CRect* pRect, CBitmap* bmResID, BOOL bOldFlag);
	virtual void DrawGradientFill(CDC* pDC, CRect* rect);
	virtual void DrawInsideBorder(CDC* pDC, CRect* rect);
	virtual ~CFAVSenseBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFAVSenseBtn)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};


#endif // !defined(AFX_XPFAVSENSEBTN_H__FA07A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_)