// FAVLNODlg.cpp : implementation file
//
//_____ I N C L U D E S _____________________________________________________
#include "stdafx.h"
#include "FAVEPC Utility.h"
#include "FAVLNODlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CFAVLNODlg dialog

IMPLEMENT_DYNAMIC(CFAVLNODlg, CDialog)
//===========================================================================
CFAVLNODlg::CFAVLNODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFAVLNODlg::IDD, pParent)
//===========================================================================
{
	//{{AFX_DATA_INIT(CFAVLNODlg)
	m_bLineNumbers = TRUE;
	//}}AFX_DATA_INIT
	m_nGutterWidth = 5;
	m_rgbLinenoColor = RGB(80,80,80);
	//m_rgbGutterColor = RGB(245,245,245);
	m_rgbGutterColor = RGB(140,200,200);
}

CFAVLNODlg::~CFAVLNODlg()
{
}

//===========================================================================
void CFAVLNODlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFAVLNODlg)
	DDX_Control(pDX, IDC_GUTTER_COLOR, m_btnGutterColor);
	DDX_Control(pDX, IDC_LINENO_COLOR, m_btnLinenoColor);
	DDX_Check(pDX, IDC_LINE_NUMBERS, m_bLineNumbers);
	//}}AFX_DATA_MAP
	DDX_ColourPicker(pDX, IDC_LINENO_COLOR, m_rgbLinenoColor);
	DDX_ColourPicker(pDX, IDC_GUTTER_COLOR, m_rgbGutterColor);
}


BEGIN_MESSAGE_MAP(CFAVLNODlg, CDialog)
	//ON_BN_CLICKED(IDC_LINE_NUMBERS, &CFAVLNODlg::OnClickedLineNumbers)
	ON_BN_CLICKED(IDC_LINE_NUMBERS, OnClickedLineNumbers)
END_MESSAGE_MAP()


// CFAVLNODlg message handlers

//===========================================================================
BOOL CFAVLNODlg::OnInitDialog()
//===========================================================================
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_GUTTER_WIDTH_SPIN);
	ASSERT(pSpin);
	CEdit *pEdit = (CEdit *) GetDlgItem(IDC_GUTTER_WIDTH);
	pSpin->SetBuddy(pEdit);
	pSpin->SetRange32(1, 9);
	pSpin->SetPos(m_nGutterWidth);

	pEdit->LimitText(1);

	OnClickedLineNumbers();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//===========================================================================
void CFAVLNODlg::OnOK()
//===========================================================================
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString s = _T("");
	CEdit *pEdit = (CEdit *) GetDlgItem(IDC_GUTTER_WIDTH);
	pEdit->GetWindowText(s);
	m_nGutterWidth = _ttoi(s);
	if (m_nGutterWidth == 0)
		m_nGutterWidth = 1;

	CDialog::OnOK();
}


//===========================================================================
void CFAVLNODlg::OnClickedLineNumbers()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_STATICGB), m_bLineNumbers);
}
