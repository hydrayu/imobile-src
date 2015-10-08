// FAVColourPicker.cpp : implementation file
//

#include "stdafx.h"
#include "..\FAVEPC Utility.h"
#include "FAVColourPicker.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma warning(disable : 4996)	// disable bogus deprecation warning

#define NO_COLOR ((COLORREF)-1)

// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


void AFXAPI DDX_ColourPicker(CDataExchange * pDX, int nIDC, COLORREF& crColor)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	CFAVColourPicker * pColorPicker = (CFAVColourPicker *) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
	{
		crColor = pColorPicker->GetColor();
	}
	else // initializing
	{
		pColorPicker->SetColor(crColor);
	}
}

void AFXAPI DDX_ColourPickerCustom(CDataExchange * pDX, int nIDC, CString& strCustomColors)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	CFAVColourPicker * pColorPicker = (CFAVColourPicker *) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
	{
		strCustomColors = pColorPicker->GetCustomColors();
	}
	else // initializing
	{
		pColorPicker->SetCustomColors(strCustomColors);
	}
}


// CFAVColourPicker

IMPLEMENT_DYNAMIC(CFAVColourPicker, CButton)
//===========================================================================
CFAVColourPicker::CFAVColourPicker()
//===========================================================================
{
	m_crEnabledColor = m_crColor = ::GetSysColor(COLOR_3DFACE);    // initial color
	m_bTrackSelection = FALSE;
	m_nSelectedRow = -1;
	m_nSelectedCol = -1;

	m_crColor = RGB(255,255,255);
	m_DefaultColor = ::GetSysColor(COLOR_APPWORKSPACE);
	m_bPopupActive = FALSE;
	m_brush.CreateHatchBrush(HS_BDIAGONAL, GetSysColor(COLOR_BTNSHADOW));

	for (int i = 0; i < 16; i++)
		m_CustomColors[i] = RGB(255,255,255);
}

//===========================================================================
CFAVColourPicker::~CFAVColourPicker()
//===========================================================================
{
	if (m_brush.GetSafeHandle())
		m_brush.DeleteObject();
}

//===========================================================================
BEGIN_MESSAGE_MAP(CFAVColourPicker, CButton)
//===========================================================================
	//{{AFX_MSG_MAP(CFAVColourPicker)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELCHANGE, OnSelchange)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, &CFAVColourPicker::OnBnClicked)
END_MESSAGE_MAP()



// CFAVColourPicker message handlers



const int g_ciArrowSizeX = 7;
const int g_ciArrowSizeY = 4;
//===========================================================================
void CFAVColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//===========================================================================
{
	// TODO:  Add your code to draw the specified item
	ASSERT(lpDrawItemStruct);

	CDC*    pDC      = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT    state    = lpDrawItemStruct->itemState;
    CRect   rDraw    = lpDrawItemStruct->rcItem;
	CRect	rArrow;

	if (m_bPopupActive)
		state |= ODS_SELECTED|ODS_FOCUS;

	//******************************************************
	//**                  Draw Outer Edge
	//******************************************************
	UINT uFrameState = DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;

	if (state & ODS_SELECTED)
		uFrameState |= DFCS_PUSHED;

	if (state & ODS_DISABLED)
		uFrameState |= DFCS_INACTIVE;

	pDC->DrawFrameControl(&rDraw,
						  DFC_BUTTON,
						  uFrameState);


	if (state & ODS_SELECTED)
		rDraw.OffsetRect(1,1);

	//******************************************************
	//**                     Draw Focus
	//******************************************************
	if (state & ODS_FOCUS)
    {
		RECT rFocus = {rDraw.left,
					   rDraw.top,
					   rDraw.right - 1,
					   rDraw.bottom};

        pDC->DrawFocusRect(&rFocus);
    }

	rDraw.DeflateRect(::GetSystemMetrics(SM_CXEDGE),
					  ::GetSystemMetrics(SM_CYEDGE));

	//******************************************************
	//**                     Draw Arrow
	//******************************************************
	rArrow.left		= rDraw.right - g_ciArrowSizeX - 2*::GetSystemMetrics(SM_CXEDGE);///2;
	rArrow.right	= rArrow.left + g_ciArrowSizeX;
	rArrow.top		= (rDraw.bottom + rDraw.top)/2 - g_ciArrowSizeY / 2;
	rArrow.bottom	= (rDraw.bottom + rDraw.top)/2 + g_ciArrowSizeY / 2;


	CSize SizeArrow = GetArrowSize();
	int nArrowWidth = SizeArrow.cx; //g_ciArrowSizeX;
	int nArrowLeft = rArrow.left + 1;
	int nArrowTop = rArrow.top;
	while (nArrowWidth > 0)
	{
		CPoint point(nArrowLeft, nArrowTop);
		nArrowLeft += 1;
		nArrowTop += 1;
		pDC->MoveTo(point);
		point.x += nArrowWidth;
		nArrowWidth -= 2;
		pDC->LineTo(point);
	}

	rDraw.right = rArrow.left - ::GetSystemMetrics(SM_CXEDGE)/2;

	//******************************************************
	//**                   Draw Separator
	//******************************************************
	pDC->DrawEdge(&rDraw,
				  EDGE_ETCHED,
				  BF_RIGHT);

	rDraw.right -= (::GetSystemMetrics(SM_CXEDGE) * 2) + 1 ;

	//******************************************************
	//**                     Draw Color
	//******************************************************
	if ((state & ODS_DISABLED) == 0)
	{
		if (m_crColor == NO_COLOR)
		{
			pDC->FillRect(&rDraw, &m_brush);
		}
		else
		{
			pDC->FillSolidRect(&rDraw,
							   (m_crColor == CLR_DEFAULT)
							   ? m_DefaultColor
							   : m_crColor);

			::FrameRect(pDC->m_hDC,
						&rDraw,
						(HBRUSH)::GetStockObject(BLACK_BRUSH));
		}
	}
}


//===========================================================================
void CFAVColourPicker::PreSubclassWindow()
//===========================================================================
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
	CButton::PreSubclassWindow();
}



//===========================================================================
int CFAVColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
//===========================================================================
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


//===========================================================================
BOOL CFAVColourPicker::OnEraseBkgnd(CDC* pDC)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	CRect rectClient;        // Rectangle for entire client area
	GetClientRect(&rectClient);

	// Fill the rectangle
	pDC->FillSolidRect(&rectClient, m_crColor);

	return TRUE;
}


//===========================================================================
void CFAVColourPicker::OnEnable(BOOL bEnable)
//===========================================================================
{
	// TODO: Add your message handler code here
	if (bEnable)
	{
		m_crColor = m_crEnabledColor;
	}
	else
	{
		m_crEnabledColor = m_crColor;
		m_crColor = RGB(192,192,192);
	}
	RedrawWindow();

	CButton::OnEnable(bEnable);
}


//===========================================================================
afx_msg LONG CFAVColourPicker::OnSelchange(WPARAM wParam, LPARAM lParam)
//===========================================================================
{
	if (m_bTrackSelection)
		SetColor((COLORREF) wParam);

	return TRUE;
}


//===========================================================================
afx_msg LRESULT CFAVColourPicker::OnSelEndCancel(WPARAM wParam, LPARAM lParam)
//===========================================================================
{
	SetColor((COLORREF) wParam);

	CWnd * pParent = GetParent();
	if (pParent)
	{
		pParent->SendMessage(CPN_CLOSEUP, wParam, (LPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDCANCEL, wParam, (LPARAM) GetDlgCtrlID());
	}

	return TRUE;
}


//===========================================================================
afx_msg LRESULT CFAVColourPicker::OnSelEndOK(WPARAM wParam, LPARAM lParam)
//===========================================================================
{
	m_nSelectedRow = LOWORD(lParam);
	m_nSelectedCol = HIWORD(lParam);

	CWnd * pParent = GetParent();
	if (pParent)
	{
		pParent->SendMessage(CPN_CLOSEUP, wParam, (LPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDOK, wParam, (LPARAM) GetDlgCtrlID());
	}

	COLORREF crNewColor = (COLORREF) wParam;
	if (crNewColor != m_crColor)
	{
		SetColor(crNewColor);
		if (pParent)
			pParent->SendMessage(CPN_SELCHANGE, wParam, (LPARAM) GetDlgCtrlID());
	}

	return TRUE;
}


//===========================================================================
CSize CFAVColourPicker::GetArrowSize(void)
//===========================================================================
{
	// Get size dimensions of edges
	CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

	// Get size of dropdown arrow
	int nArrowWidth  = ::GetSystemMetrics(SM_CXHTHUMB) / 3 + 2;
	int nArrowHeight = ::GetSystemMetrics(SM_CYVTHUMB) / 3 + 2;
	CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

	// Make sure arrow size is an odd number (just looks neater)
	if ((ArrowSize.cx & 1) == 0)
	{
		ArrowSize.cx++;
		ArrowSize.cy++;
	}

	return ArrowSize;
}


//===========================================================================
void CFAVColourPicker::SetColor(COLORREF crColor)
//===========================================================================
{
	m_crEnabledColor = m_crColor = crColor;
	RedrawWindow();
}


//===========================================================================
COLORREF CFAVColourPicker::GetColor(void)
//===========================================================================
{
	return m_crColor;
}


//===========================================================================
void CFAVColourPicker::SetCustomColors(COLORREF * pCustomColors)
//===========================================================================
{
	ASSERT(pCustomColors);
	for (int i = 0; i < 16; i++)
		m_CustomColors[i] = pCustomColors[i];
}


//===========================================================================
void CFAVColourPicker::GetCustomColors(COLORREF * pCustomColors)
//===========================================================================
{
	ASSERT(pCustomColors);
	for (int i = 0; i < 16; i++)
		pCustomColors[i] = m_CustomColors[i];
}


//===========================================================================
void CFAVColourPicker::SetCustomColors(LPCTSTR lpszCustomColors)
//===========================================================================
{
	int i = 0;

	// initialize to white
	for (i = 0; i < 16; i++)
		m_CustomColors[i] = RGB(255,255,255);

	ASSERT(lpszCustomColors);
	if (lpszCustomColors == NULL)
		return;

	TCHAR szCustomColors[256];
	memset(szCustomColors, 0, sizeof(szCustomColors));
	_tcsncpy(szCustomColors, lpszCustomColors, _countof(szCustomColors)-2);

	TCHAR *cp = NULL;
	cp = _tcstok(szCustomColors, _T(" "));

	for (i = 0; i < 16; i++)
	{
		if (!cp)
			break;
		m_CustomColors[i] = _tcstoul(cp, NULL, 16);
		cp = _tcstok(NULL, _T(" "));
	}
}


//===========================================================================
CString CFAVColourPicker::GetCustomColors(void)
//===========================================================================
{
	CString strCustomColors = _T("");
	CString str = _T("");
	for (int i = 0; i < 16; i++)
	{
		str.Format(_T("%06X"), m_CustomColors[i]);
		if (!strCustomColors.IsEmpty())
			strCustomColors += _T(" ");
		strCustomColors += str;
	}
	return strCustomColors;
}


//===========================================================================
BOOL CFAVColourPicker::OnBnClicked()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CRect rect;
	GetWindowRect(rect);

	// note that m_CustomColors will be updated by CXColorPopup handler
	// when user selects a color
	new CFAVColourPopup(this, CPoint(rect.left, rect.bottom), m_crColor, m_CustomColors);

	CWnd * pParent = GetParent();
	if (pParent)
		pParent->SendMessage(CPN_DROPDOWN, (LPARAM)m_crColor, (WPARAM) GetDlgCtrlID());

	// docs say I should return FALSE to stop the parent also getting the message.
	// HA! What a joke.
	return TRUE;
}
