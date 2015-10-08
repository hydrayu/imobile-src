// FAVXPBtn.cpp : implementation file
//

#include "stdafx.h"
//#include "FAVEPC Utility.h"
#include "FAVXPBtn.h"

//_____ F U N  C T I O N ____________________________________________________
// CFAVXPBtn

IMPLEMENT_DYNAMIC(CFAVXPBtn, CButton)
//===========================================================================
CFAVXPBtn::CFAVXPBtn()
//===========================================================================
{
	m_State = bsNormal;
	m_BtnEnable = FALSE;
}

//===========================================================================
CFAVXPBtn::~CFAVXPBtn()
//===========================================================================
{
	m_State = bsNormal;
}

//===========================================================================
BEGIN_MESSAGE_MAP(CFAVXPBtn, CButton)
//===========================================================================
	//{{AFX_MSG_MAP(CFAVXPBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CFAVXPBtn message handlers
//===========================================================================
void CFAVXPBtn::OnLButtonDown(UINT nFlags, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	m_State = bsDown;
	InvalidateRect(NULL,TRUE);
	//CButton::OnLButtonDown(nFlags, point);
}

//===========================================================================
void CFAVXPBtn::OnLButtonUp(UINT nFlags, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	if(m_State != bsNormal)
	{
		m_State = bsNormal;
		ReleaseCapture();
		InvalidateRect(NULL,TRUE);
	}	
	::SendMessage(GetParent()->m_hWnd,WM_COMMAND, GetDlgCtrlID(), (LPARAM) m_hWnd);
	//CButton::OnLButtonUp(nFlags, point);
}

//===========================================================================
void CFAVXPBtn::OnMouseMove(UINT nFlags, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(hRgn);
	BOOL ret = PtInRegion(hRgn, point.x, point.y);
	if(ret)
	{
		if(m_State == bsDown)
			return ;
		if(m_State != bsHot)
		{
			m_State = bsHot;
			InvalidateRect(NULL,TRUE);
			SetCapture();
		}
	} 
	else
	{
		m_State = bsNormal;
		InvalidateRect(NULL,TRUE);
		ReleaseCapture();
	}	
	DeleteObject(hRgn);	

	CButton::OnMouseMove(nFlags, point);
}

//===========================================================================
void CFAVXPBtn::OnPaint()
//===========================================================================
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages
	CString		strText;
	CRect		RC;
	CFont		Font;
	CFont*		pOldFont;
	CBrush		Brush;
	CBrush*		pOldBrush;
	CPoint		PT(2,2);
	dc.SetBkMode(TRANSPARENT);
	Font.CreateFont(16, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, 
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_SWISS, _T("Arial"));
	pOldFont = dc.SelectObject(&Font);

	if(m_State == bsNormal)
	{
		Brush.CreateSolidBrush( RGB(230, 230, 230));
		dc.SetTextColor(RGB(0, 0, 0));
	}
	else if(m_State == bsDown)
	{
		Brush.CreateSolidBrush(RGB(100, 100, 180));
		dc.SetTextColor(RGB(250, 250, 0));
	}
	else if(m_State == bsHot)
	{
		Brush.CreateSolidBrush(RGB(230, 230, 130));
		dc.SetTextColor(RGB(50, 50, 250));
	}

	pOldBrush = dc.SelectObject(&Brush);
	GetClientRect(&RC);	
	dc.RoundRect(&RC, PT);
	HRGN hRgn = CreateRectRgn(RC.left, RC.top, RC.right, RC.bottom);
	SetWindowRgn(hRgn, TRUE);
	DeleteObject(hRgn);

	GetWindowText(strText);
	dc.DrawText(strText, &RC, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	Font.DeleteObject();
	Brush.DeleteObject();

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldBrush);
}
