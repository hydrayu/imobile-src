// FAVSenseBtn.cpp : implementation file
//

#include "stdafx.h"
#include "..\FAVEPC Utility.h"
#include "FAVSenseBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//_____ F U N  C T I O N ____________________________________________________
// CFAVSenseBtn

IMPLEMENT_DYNAMIC(CFAVSenseBtn, CBitmapButton)
//===========================================================================
CFAVSenseBtn::CFAVSenseBtn()
//===========================================================================
{
	m_uBtnDown		= IDB_BUTTON_DOWN01;
	m_uBtnNomal		= IDB_BUTTON_NOMAL01;
	m_uBtnEnable	= IDB_BUTTON_DISABLED01;
	m_uBtnMove		= IDB_BUTTON_MOVE01;
	bInSenseBtn		= FALSE;
	m_BtnEnable		= FALSE;

	m_BoundryPen.CreatePen(PS_INSIDEFRAME|PS_SOLID, 1, RGB(0, 0, 0));

	m_InsideBoundryPenTop.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(252, 210, 121));
	m_InsideBoundryPenBottom.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(229, 151, 0));
	m_InsideBoundryPenLeft.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(250, 196, 88)); 
	m_InsideBoundryPenRight.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(251, 202, 106));

	m_FillActive.CreateSolidBrush(RGB(223, 222, 236));
	m_FillInactive.CreateSolidBrush(RGB(222, 223, 236));

	m_InsideBoundryPenTopSel.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(162, 201, 255));
	m_InsideBoundryPenBottomSel.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(162, 201, 255));
	m_InsideBoundryPenLeftSel.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(153, 198, 252)); 
	m_InsideBoundryPenRightSel.CreatePen(PS_INSIDEFRAME|PS_SOLID, 2, RGB(162, 189, 252));

	m_bOver = m_bSelected = m_bTracking = m_bFocus = FALSE;
	m_bIsMask = FALSE;
	m_bIsMaskRgn = FALSE;
	m_bIsBackBmp = FALSE;
	
	m_nBtnStyle = ROUNDRECT;
}

//===========================================================================
CFAVSenseBtn::~CFAVSenseBtn()
//===========================================================================
{
	m_BoundryPen.DeleteObject();

	m_InsideBoundryPenTop.DeleteObject();
	m_InsideBoundryPenBottom.DeleteObject();
	m_InsideBoundryPenLeft.DeleteObject();
	m_InsideBoundryPenRight.DeleteObject();
	

	m_FillActive.DeleteObject();
	m_FillInactive.DeleteObject();

	m_InsideBoundryPenTopSel.DeleteObject();
	m_InsideBoundryPenBottomSel.DeleteObject();
	m_InsideBoundryPenLeftSel.DeleteObject();
	m_InsideBoundryPenRightSel.DeleteObject();
	
}

//===========================================================================
BEGIN_MESSAGE_MAP(CFAVSenseBtn, CBitmapButton)
//===========================================================================
	//{{AFX_MSG_MAP(CFAVSenseBtn)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	//ON_WM_MOUSELEAVE()
	//ON_WM_MOUSEHOVER()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CFAVSenseBtn message handlers
//===========================================================================
void CFAVSenseBtn::SetTextColor(COLORREF crColor) 
//===========================================================================
{ 
	TextColor = crColor; 
} 

//===========================================================================
void CFAVSenseBtn::OnTimer(UINT_PTR nIDEvent)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	CPoint point;
	GetCursorPos(&point);
	CRect BtnRect;						
	GetWindowRect(&BtnRect);

	if(BtnRect.PtInRect(point)){
		if(bInSenseBtn == TRUE)
			goto END;
		else{
			bInSenseBtn = TRUE;
			Invalidate();
		}
	}
	else{
		if(bInSenseBtn == FALSE)
			goto END;
		else{
			Invalidate();
			bInSenseBtn = FALSE;
		}
	}

END:
	CBitmapButton::OnTimer(nIDEvent);
}

//===========================================================================
void CFAVSenseBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//===========================================================================
{
	// TODO:  Add your code to draw the specified item
	//Use this member function to attach an hDC to the CDC object.
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	int nSaveDC = dc.SaveDC();

	//CRect rect;
	//GetClientRect(rect);
	CRect rect =  lpDrawItemStruct->rcItem;

	CString text;
	GetWindowText(text);
	
	UINT state = lpDrawItemStruct->itemState;

	// Setting Button state
	if(state & ODS_FOCUS){
		CRect FocTect(rect);
		FocTect.DeflateRect(2,2,2,2);
		dc.DrawFocusRect(&FocTect);
		lpDrawItemStruct->itemAction = ODA_FOCUS ;

		m_bFocus = TRUE;
		m_bSelected = TRUE;
	}
	else{
		m_bFocus = FALSE;
		m_bSelected = FALSE;
	}
	if (state & ODS_SELECTED || state & ODS_DEFAULT){
		m_bFocus = TRUE;
	}

	
	// According button state to draw map
	if (m_bOver)
	{
		switch (lpDrawItemStruct->CtlID){
			case IDC_CONNECT_OKBTN:
			case IDC_CONNECT_CONNECTBTN:
			case IDC_READER_READEPC:
			case IDC_READER_READ:
			case IDC_READER_WRITE:
				DrawBK(&dc, m_uBtnMove);
				dc.SetTextColor(RGB(255,0,0));
				break;//
			case IDC_RMC_DEMO_POWER:
				DrawEllipse(&dc, &rect, &m_OverBitmap[0], FALSE);
				break;//IDC_RMC_DEMO_POWER
			case IDC_RMC_DEMO_BULBOFF:
				DrawEllipse(&dc, &rect, &m_OverBitmap[1], FALSE);
				break;//IDC_RMC_DEMO_BULBOFF
			case IDC_RMC_DEMO_BULBON:
				DrawEllipse(&dc, &rect, &m_OverBitmap[2], FALSE);
				break;//IDC_RMC_DEMO_BULBON
			case IDC_RMC_DEMO_BON:
				DrawEllipse(&dc, &rect, &m_OverBitmap[3], FALSE);
				break;//IDC_RMC_DEMO_BON
			case IDC_RMC_DEMO_BOFF:
				DrawEllipse(&dc, &rect, &m_OverBitmap[4], FALSE);
				break;//IDC_RMC_DEMO_BOFF
			case IDC_RMC_DEMO_GON:
				DrawEllipse(&dc, &rect, &m_OverBitmap[5], FALSE);
				break;//IDC_RMC_DEMO_GON
			case IDC_RMC_DEMO_GOFF:
				DrawEllipse(&dc, &rect, &m_OverBitmap[6], FALSE);
				break;//IDC_RMC_DEMO_GOFF
			case IDC_RMC_DEMO_RON:
				DrawEllipse(&dc, &rect, &m_OverBitmap[7], FALSE);
				break;//IDC_RMC_DEMO_RON
			case IDC_RMC_DEMO_ROFF:
				DrawEllipse(&dc, &rect, &m_OverBitmap[8], FALSE);
				break;//IDC_RMC_DEMO_ROFF
			case IDC_RMC_DEMO_PAIR:
				DrawEllipse(&dc, &rect, &m_OverBitmap[9], FALSE);
				break;//IDC_RMC_DEMO_PAIR
			default:
				DrawBK(&dc, m_uBtnMove);
				dc.SetTextColor(RGB(255,0,0));
				break;
		}
		dc.RestoreDC(nSaveDC);
	}
	else
	{
		switch (lpDrawItemStruct->CtlID){
		case IDC_CONNECT_OKBTN:
		case IDC_CONNECT_CONNECTBTN:
		case IDC_READER_READEPC:
		case IDC_READER_READ:
		case IDC_READER_WRITE:
			DrawBK(&dc,m_uBtnNomal);
			dc.SetTextColor(RGB(0,0,0));
			break;//
		case IDC_RMC_DEMO_POWER:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[0], TRUE);
			break;//IDC_RMC_DEMO_POWER
		case IDC_RMC_DEMO_BULBOFF:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[1], TRUE);
			break;//IDC_RMC_DEMO_BULBOFF
		case IDC_RMC_DEMO_BULBON:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[2], TRUE);
			break;//IDC_RMC_DEMO_BULBON
		case IDC_RMC_DEMO_BON:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[3], TRUE);
			break;//IDC_RMC_DEMO_BON
		case IDC_RMC_DEMO_BOFF:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[4], TRUE);
			break;//IDC_RMC_DEMO_BOFF
		case IDC_RMC_DEMO_GON:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[5], TRUE);
			break;//IDC_RMC_DEMO_GON
		case IDC_RMC_DEMO_GOFF:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[6], TRUE);
			break;//IDC_RMC_DEMO_GOFF
		case IDC_RMC_DEMO_RON:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[7], TRUE);
			break;//IDC_RMC_DEMO_RON
		case IDC_RMC_DEMO_ROFF:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[8], TRUE);
			break;//IDC_RMC_DEMO_ROFF
		case IDC_RMC_DEMO_PAIR:
			DrawEllipse(&dc, &rect, &m_NormalBitmap[9], TRUE);
			break;//IDC_RMC_DEMO_PAIR
		default:
			DrawBK(&dc,m_uBtnNomal);
			dc.SetTextColor(RGB(0,0,0));
			break;
		}
		dc.RestoreDC(nSaveDC);
	}

	// Button Pressed(Down).
	if(state & ODS_SELECTED)
	{
		
		switch(lpDrawItemStruct->CtlID){
			case IDC_CONNECT_OKBTN:
			case IDC_CONNECT_CONNECTBTN:
			case IDC_READER_READEPC:
			case IDC_READER_READ:
			case IDC_READER_WRITE:
				DrawBK(&dc, m_uBtnDown);
				dc.SetTextColor(RGB(0,0,255));
				break;//IDC_READER_WRITE

			case IDC_RMC_DEMO_POWER:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[0], TRUE);
				break;//IDC_RMC_DEMO_POWER
			case IDC_RMC_DEMO_BULBOFF:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[1], TRUE);
				break;//IDC_RMC_DEMO_BULBOFF
			case IDC_RMC_DEMO_BULBON:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[2], TRUE);
				break;//IDC_RMC_DEMO_BULBON
			case IDC_RMC_DEMO_BON:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[3], TRUE);
				break;//IDC_RMC_DEMO_BON
			case IDC_RMC_DEMO_BOFF:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[4], TRUE);
				break;//IDC_RMC_DEMO_BOFF
			case IDC_RMC_DEMO_GON:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[5], TRUE);
				break;//IDC_RMC_DEMO_GON
			case IDC_RMC_DEMO_GOFF:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[6], TRUE);
				break;//IDC_RMC_DEMO_GOFF
			case IDC_RMC_DEMO_RON:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[7], TRUE);
				break;//IDC_RMC_DEMO_RON
			case IDC_RMC_DEMO_ROFF:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[8], TRUE);
				break;//IDC_RMC_DEMO_ROFF
			case IDC_RMC_DEMO_PAIR:
				DrawEllipse(&dc, &rect, &m_PressedBitmap[9], TRUE);
				break;//IDC_RMC_DEMO_PAIR
			default:
				DrawBK(&dc, m_uBtnDown);
				dc.SetTextColor(RGB(0,0,255));

				break;
		}
		dc.RestoreDC(nSaveDC);
	}

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(text,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

//===========================================================================
void CFAVSenseBtn::PreSubclassWindow()
//===========================================================================
{
	// TODO: Add your specialized code here and/or call the base class
	//SetTimer(1,30,NULL); 
	CBitmapButton::PreSubclassWindow();

	ModifyStyle(0, BS_OWNERDRAW | BS_NOTIFY);	// Add Button style owner draw attributes
}

//===========================================================================
BOOL CFAVSenseBtn::OnEraseBkgnd(CDC* pDC)
//===========================================================================
{
	
	//return true;
	return CBitmapButton::OnEraseBkgnd(pDC);
}

//===========================================================================
BOOL CFAVSenseBtn::PreTranslateMessage(MSG* pMsg)
//===========================================================================
{
	// TODO: Add your specialized code here and/or call the base class
	//if(pMsg->hwnd == this->GetSafeHwnd() && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
	{
		pMsg->lParam  = 0;
		pMsg->message = WM_LBUTTONDOWN;
	}
	if(pMsg->hwnd == this->GetSafeHwnd() && pMsg->message == WM_KEYUP && pMsg->wParam == VK_SPACE)
	{
		pMsg->lParam  = 0;
		pMsg->message = WM_LBUTTONUP;
	}
	//return CButton::PreTranslateMessage(pMsg);
	return CBitmapButton::PreTranslateMessage(pMsg);
}

//===========================================================================
void CFAVSenseBtn::DrawBK(CDC *pDC, UINT ResID)
//===========================================================================
{
	// Create an in-memory DC compatible with the
	// display DC we're using to paint
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);

	CRect rect;	
	GetClientRect(rect);

	CBitmap bitmap;
	BITMAP bitStruct;
	bitmap.LoadBitmap(ResID);
	bitmap.GetBitmap(&bitStruct);
	dcMemory.SelectObject(&bitmap);

	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0,
					bitStruct.bmWidth, bitStruct.bmHeight,SRCCOPY);
	dcMemory.DeleteDC();
	bitmap.DeleteObject();
}

//===========================================================================
void CFAVSenseBtn::DrawEllipse(CDC* pDC, CRect* pRect, CBitmap* bmResID, BOOL bOldFlag)
//===========================================================================
{
	// Create an in-memory DC compatible with the
	// display DC we're using to paint
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = nullptr;
	pOldBmp = dcMemory.SelectObject(bmResID);

	// Get the size of the bitmap
	BITMAP bitStruct;
	bmResID->GetObject(sizeof(bitStruct), &bitStruct);

	// Select the bitmap into the in-memory DC
	dcMemory.SelectObject(&pOldBmp);

	// Find a center point for the bitmap in the client area
	int nX = pRect->left +(pRect->Width() - bitStruct.bmWidth) / 2;
	int nY = pRect->top +(pRect->Height() - bitStruct.bmHeight) / 2;

	// Copy the bits from the in-memory DC into the on-
	// screen DC to actually do the painting. Use the center point
	// we computed for the target offset.
	if (m_bIsMask == TRUE)
	{
		CDC dcMask;
		dcMask.CreateCompatibleDC(pDC);

		if (m_bIsBackBmp == TRUE)
		{
			//CBitmap *pOldBmp;
			CDC dcBack;
			dcBack.CreateCompatibleDC(pDC);

			pOldBmp = dcMask.SelectObject(&m_MaskBitmap);
			dcBack.SelectObject(&m_BackBitmap);

			pDC->BitBlt(0, 0, pRect->Width(), pRect->Height(), 
						&dcBack, m_rectBackRect.left, m_rectBackRect.top, SRCCOPY);
		}

		if(bOldFlag){
			//CBitmap *pOldBmp;
			pOldBmp = dcMask.SelectObject(&m_MaskBitmap);
		}
		pDC->BitBlt(nX, nY, bitStruct.bmWidth, bitStruct.bmHeight, &dcMask, 
					0, 0, MERGEPAINT);
		pDC->BitBlt(nX, nY, bitStruct.bmWidth, bitStruct.bmHeight, &dcMemory, 
					0, 0, SRCAND);

		
		if(!bOldFlag)
		{
			ReleaseDC(&dcMask);
		}
	}
	else
	{
		pDC->BitBlt(nX, nY, bitStruct.bmWidth, bitStruct.bmHeight, &dcMemory,
					0,0,SRCCOPY);
	}

	dcMemory.DeleteDC();
}

//===========================================================================
void CFAVSenseBtn::OnMouseMove(UINT nFlags, CPoint point)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

//===========================================================================
LRESULT CFAVSenseBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	m_bOver = FALSE;         
	m_bTracking = FALSE;   
	InvalidateRect(NULL, FALSE);

	return 0;
	//CBitmapButton::OnMouseLeave();
}

//===========================================================================
LRESULT CFAVSenseBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	m_bOver = TRUE;
	InvalidateRect(NULL);	//re-draw

	return 0;
	//CBitmapButton::OnMouseHover(nFlags, point);
}

//===========================================================================
void CFAVSenseBtn::DrawGradientFill(CDC* pDC, CRect* rect)
//===========================================================================
{
	CBrush brushBk[64];
	int nWidth = rect->Width();	
	int nHeight = rect->Height();
	CRect rct;
	int i;
	for (i = 0; i < 64; i ++)
	{
		if (m_bOver)
		{
			if (m_bFocus)
				brushBk[i].CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 3)));
			else
				brushBk[i].CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 5)));
		}
		else
		{
			if (m_bFocus)
				brushBk[i].CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 4)));
			else
				brushBk[i].CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 5)));
		}
	}

	for (i = rect->top; i <= nHeight + 2; i ++) 
	{
		rct.SetRect(rect->left, i, nWidth + 2, i + 1);
		pDC->FillRect(&rct, &brushBk[((i * 63) / nHeight)]);
	}

	for (i = 0; i < 64; i ++)
		brushBk[i].DeleteObject();
}

//===========================================================================
void CFAVSenseBtn::DrawInsideBorder(CDC *pDC, CRect* rect)
//===========================================================================
{
	CPen *pLeft, *pRight, *pTop, *pBottom;

	if (m_bSelected && !m_bOver)
	{
		pLeft = & m_InsideBoundryPenLeftSel;
		pRight = &m_InsideBoundryPenRightSel;
		pTop = &m_InsideBoundryPenTopSel;
		pBottom = &m_InsideBoundryPenBottomSel;
	}
	else
	{
		pLeft = &m_InsideBoundryPenLeft;
		pRight = &m_InsideBoundryPenRight;
		pTop = &m_InsideBoundryPenTop;
		pBottom = &m_InsideBoundryPenBottom;
	}

	CPoint oldPoint = pDC->MoveTo(rect->left, rect->bottom - 1);
	CPen* pOldPen = pDC->SelectObject(pLeft);
	pDC->LineTo(rect->left, rect->top + 1);
	pDC->SelectObject(pRight);
	pDC->MoveTo(rect->right - 1, rect->bottom - 1);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pTop);
	pDC->MoveTo(rect->left - 1, rect->top);
	pDC->LineTo(rect->right - 1, rect->top);
	pDC->SelectObject(pBottom);
	pDC->MoveTo(rect->left, rect->bottom);
	pDC->LineTo(rect->right - 1, rect->bottom);
	pDC->SelectObject(pOldPen);
	pDC->MoveTo(oldPoint);

	if (m_bSelected && !m_bOver)
		DrawFocusRect(pDC->m_hDC,rect);
}

//===========================================================================
void CFAVSenseBtn::SetRgnStyle(UINT nStyle)
//===========================================================================
{
	m_nBtnStyle = nStyle;

	// Setting valid button region, include ROUNDRECT/ELLIPSE/UPTRIANGLE/DOWNTRIANGLE
	CRect rc;
	GetClientRect(&rc);

	CRgn rgn;
	if(m_nBtnStyle == ROUNDRECT)
	{
		rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 8, 8);

	}
	else if(m_nBtnStyle == ELLIPSE)
	{
		rgn.CreateEllipticRgn(rc.left, rc.top, rc.right, rc.bottom);

	}
	else if(m_nBtnStyle == UPTRIANGLE)
	{
		CPoint ptArray[] = {CPoint(rc.left+rc.Width()/2, rc.top),
							CPoint(rc.left,rc.bottom),
							CPoint(rc.right,rc.bottom)};

		VERIFY(rgn.CreatePolygonRgn(ptArray, 3, ALTERNATE));

	}
	else if(m_nBtnStyle == UPTRIANGLE)
	{
		CPoint ptArray[] = {CPoint(rc.left,rc.top),
							CPoint(rc.right,rc.top),
							CPoint(rc.left+rc.Width()/2,rc.bottom)};

		VERIFY(rgn.CreatePolygonRgn(ptArray, 3, ALTERNATE));
	}

	int nRes = SetWindowRgn(rgn, TRUE);
	if (nRes == 0)
	{
		TRACE(_T("Setting ROUNDRECT/ELLIPSE/UPTRIANGLE/DOWNTRIANGLE fail."));
	}

	rgn.DeleteObject();
}

//===========================================================================
void CFAVSenseBtn::SetBitmapId(UINT nIndex,int nNormal, int nMove, int nPressed, int nOver)
//===========================================================================
{
	if(m_NormalBitmap[nIndex].m_hObject)
	{
		m_NormalBitmap[nIndex].DeleteObject();
	}
	if(m_FocusBitmap[nIndex].m_hObject)
	{
		m_FocusBitmap[nIndex].DeleteObject();
	}
	if(m_PressedBitmap[nIndex].m_hObject)
	{
		m_PressedBitmap[nIndex].DeleteObject();
	}
	if(m_OverBitmap[nIndex].m_hObject)
	{
		m_OverBitmap[nIndex].DeleteObject();
	}

	m_NormalBitmap[nIndex].LoadBitmap(nNormal);
	m_FocusBitmap[nIndex].LoadBitmap(nMove);
	m_PressedBitmap[nIndex].LoadBitmap(nPressed);
	m_OverBitmap[nIndex].LoadBitmap(nOver);
}

//===========================================================================
void CFAVSenseBtn::SetMaskBitmapId(int mask, bool action)
//===========================================================================
{
	m_bIsMask = action;
	m_MaskBitmap.LoadBitmap(mask);
}

//===========================================================================
HRGN CFAVSenseBtn::BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
//===========================================================================
{
	HRGN hRgn = NULL;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
													sizeof(BITMAPINFOHEADER),	// biSize 
													bm.bmWidth,					// biWidth; 
													bm.bmHeight,				// biHeight; 
													1,							// biPlanes; 
													32,							// biBitCount 
													BI_RGB,						// biCompression; 
													0,							// biSizeImage; 
													0,							// biXPelsPerMeter; 
													0,							// biYPelsPerMeter; 
													0,							// biClrUsed; 
													0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO,
											DIB_RGB_COLORS, &pbits32, NULL, 0);
			if(hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}
	}

	return hRgn;
}

//===========================================================================
void CFAVSenseBtn::SetRgnMask(int nMaskBmpId, bool nAction)
//===========================================================================
{
	CBitmap bmp;
	HRGN hRgn;

	m_bIsMaskRgn = nAction;
	bmp.LoadBitmap(nMaskBmpId);
	hRgn = BitmapToRegion((HBITMAP)bmp, RGB(255,255,255));

	if (m_bIsMaskRgn == TRUE)
	{
		SetWindowRgn(hRgn,TRUE);
	}
}

//===========================================================================
void CFAVSenseBtn::SetBackBmp(int nBgdBmpId,CRect rect)
//===========================================================================
{
	m_bIsBackBmp = TRUE;
	m_rectBackRect = rect;
	m_BackBitmap.LoadBitmap(nBgdBmpId);
}

