// FAVXPBtn.h : header file
//
#if !defined(AFX_XPFAVXPBTN_H__FA06A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_)
#define AFX_XPFAVXPBTN_H__FA06A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ButtonState  {bsNormal,bsHot,bsDown};
//_____ D E C L A R A T I O N _______________________________________________
// CFAVXPBtn

class CFAVXPBtn : public CButton
{
	DECLARE_DYNAMIC(CFAVXPBtn)
// Construction
public:
	CFAVXPBtn();
	int         m_State;
	BOOL		m_BtnEnable;

// Implementation
public:
	virtual ~CFAVXPBtn();
	void XPBtnState(BOOL bValue){ this->m_BtnEnable = bValue;}

	// Generated message map functions
protected:
	

	//{{AFX_MSG(CFAVXPBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};

#endif // !defined(AFX_XPFAVXPBTN_H__FA06A2A1_6717_51D4_17C8_38025B24F10C__INCLUDED_
