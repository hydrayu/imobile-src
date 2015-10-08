// FAVEPC UtilityDlg.cpp : implementation file
//
//_____ I N C L U D E S _____________________________________________________
#include "stdafx.h"
#include "FAVEPC Utility.h"
#include "FAVEPC UtilityDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//_____ D E C L A R A T I O N _______________________________________________
//extern CString g_strCOMPORT;
//extern HANDLE g_hComm;
extern CString g_strCOMPORT;
//_____ F U N  C T I O N ____________________________________________________
// CAboutDlg dialog used for App About
//===========================================================================
class CAboutDlg : public CDialogEx
//===========================================================================
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:

};


//===========================================================================
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
//===========================================================================
{
}


//===========================================================================
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CFAVEPCUtilityDlg dialog
//===========================================================================
CFAVEPCUtilityDlg::CFAVEPCUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFAVEPCUtilityDlg::IDD, pParent)
//===========================================================================
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#ifdef MODELESSDIALOG
	m_bShow = FALSE;
#endif//MODELESSDIALOG
}


//===========================================================================
void CFAVEPCUtilityDlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABPAGE, m_TabCtrl);
}


//===========================================================================
BEGIN_MESSAGE_MAP(CFAVEPCUtilityDlg, CDialogEx)
//===========================================================================
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABPAGE, &CFAVEPCUtilityDlg::OnTcnSelchangeTabpage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFAVEPCUtilityDlg message handlers
//===========================================================================
BOOL CFAVEPCUtilityDlg::OnInitDialog()
//===========================================================================
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// Use Icon
	UINT uIconID[] = {IDI_ICON_READER,IDI_ICON_RMC};
	m_TabImage.Create(16,16,ILC_COLOR24,sizeof(uIconID)/sizeof(UINT),10);
	m_TabImage.SetBkColor(RGB(0xEC,0xE9,0xD8));						// XP background color
	m_TabImage.Add(AfxGetApp()->LoadIcon(uIconID[0]));
	m_TabImage.Add(AfxGetApp()->LoadIcon(uIconID[1]));
	m_TabCtrl.SetImageList(&m_TabImage);



	// Add TabCtrl controller code.
	m_TabCtrl.InsertItem(0, _T("Reader"),0);
	m_TabCtrl.InsertItem(1, _T("RMC"),1);
	m_ReaderDlg.Create(IDD_READER_DIALOG, &m_TabCtrl);				// Call Create to create a modeless dialog box 
	m_RMCDlg.Create(IDD_RMC_DIALOG, &m_TabCtrl);					// using a dialog-box template from a resource.
	CRect TabCtrlRect;
	m_TabCtrl.GetClientRect(&TabCtrlRect);							// CWindow::GetClientRect : Retrieves the coordinates of the client area.
	m_ReaderDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_SHOWWINDOW);	//Changes the size, position, and Z-order of child, pop-up, and top-level windows.
	m_RMCDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_HIDEWINDOW); 
	// Read Information

	//m_pConnectDlg = &m_ConnectDlg;
	// Add COMPORTINFO controller code.
	CreateCenterDialog(IDD_CONNECT_DIALOG, m_ConnectDlg);

	SetTimer(1, 50, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


//===========================================================================
void CFAVEPCUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
//===========================================================================
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//===========================================================================
void CFAVEPCUtilityDlg::OnPaint()
//===========================================================================
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
#ifdef MODELDIALOG
		CDialogEx::OnPaint();
#else
		if(m_bShow)
			this->ShowWindow(SW_SHOW);
		else
			this->ShowWindow(SW_HIDE);
#endif//MODELDIALOG
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//===========================================================================
HCURSOR CFAVEPCUtilityDlg::OnQueryDragIcon()
//===========================================================================
{
	return static_cast<HCURSOR>(m_hIcon);
}


//===========================================================================
void CFAVEPCUtilityDlg::OnTcnSelchangeTabpage(NMHDR *pNMHDR, LRESULT *pResult)
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CRect TabCtrlRect;
	m_TabCtrl.GetClientRect(&TabCtrlRect);		// CWindow::GetClientRect : Retrieves the coordinates of the client area.

	switch(m_TabCtrl.GetCurSel())				// CTabCtrl::GetCurSel : Retrieves the currently selected tab in a tab control.
	{
	case 0:
		m_ReaderDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_SHOWWINDOW);
		m_RMCDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_HIDEWINDOW ); 
		break;
	case 1:
		m_ReaderDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_HIDEWINDOW);
		m_RMCDlg.SetWindowPos(NULL,0,0,(TabCtrlRect.right-10),(TabCtrlRect.bottom-25),SWP_SHOWWINDOW ); 
		break;
	}
	*pResult = 0;
}


//===========================================================================
HBRUSH CFAVEPCUtilityDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//===========================================================================
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	// Settings Window is belong to WS_EX_LAYERED property 
	::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE)|WS_EX_LAYERED);
	// The dialog have a 95 % transparent.
	::SetLayeredWindowAttributes(this->m_hWnd, 0, (255 * 98) / 100, LWA_ALPHA);
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


//===========================================================================
void CFAVEPCUtilityDlg::OnTimer(UINT_PTR nIDEvent)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	char chCMDV[] = COMMAND_V;
	char chCMDS[] = COMMAND_S;
	//CString strCOMPORT= (_T(""));
	CString strCOMPORT;
	switch(nIDEvent){
	case 1:
		strCOMPORT.Format(_T("%s , 38400, n, 8, 1"), g_strCOMPORT);
		m_ReaderDlg.GetDlgItem(IDC_READER_CPI_DATA)->SetWindowText(strCOMPORT);
		SetTimer(2, 50, NULL);
		break;
	case 2:
		if(m_SerialComm.WriteCMD(CMD_V, chCMDV, sizeof(chCMDV), WRITE_DEFAULT_TIMEOUT))
		{
			TRACE(_T(">>> Write Fail in Command V \n"));
		}
		if(m_SerialComm.ReadCMD(CMD_V, READ_DEFAULT_TIMEOUT)){
			m_ReaderDlg.GetDlgItem(IDC_READER_FV_DATA)->SetWindowText(m_SerialComm.GetCMDData(CMD_V));
		}
		SetTimer(3, 50, NULL);
		break;
	case 3:
		if(m_SerialComm.WriteCMD(CMD_S, chCMDS, sizeof(chCMDS), WRITE_DEFAULT_TIMEOUT))
		{
			TRACE(_T(">>> Write Fail in Command S \n"));
		}
		if(m_SerialComm.ReadCMD(CMD_S, READ_DEFAULT_TIMEOUT)){
			m_ReaderDlg.GetDlgItem(IDC_READER_RID_DATA)->SetWindowText(m_SerialComm.GetCMDData(CMD_S));
		}
		break;

	}
	CDialogEx::OnTimer(nIDEvent);
}


//===========================================================================
void CFAVEPCUtilityDlg::OnClose()
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	AfxGetApp()->m_pszAppName = _T("Close Utility");
	if(AfxMessageBox(_T("Sure you want to exit the application?"), MB_OKCANCEL|MB_ICONQUESTION)!= IDOK)
		return;
	CDialogEx::OnClose();
}


//===========================================================================
void CFAVEPCUtilityDlg::CreateCenterDialog(int nIDC, CDialogEx &rControl) const
	//===========================================================================
{
#ifdef MODELDIALOG
	// Model dialog
	rControl.DoModal();

#else
	// Modeless dialog
	CPoint Point;
	CRect DialogRect;
	int nWidth;
	int nHeight;

	rControl.Create(nIDC, this);
	rControl.GetClientRect(&DialogRect);
	// Calculate the height and width for MoveWindow().
	nWidth = DialogRect.Width();
	nHeight = DialogRect.Height();
	// Find the center point and convert to screen coordinates.
	Point.x = GetSystemMetrics(SM_CXSCREEN) / 2;
	Point.y = GetSystemMetrics(SM_CYSCREEN) / 2;
	Point.x -= nWidth / 2;
	Point.y -= nHeight / 2;
	// Move the window.
	rControl.SetWindowPos(NULL,Point.x,Point.y,DialogRect.right,DialogRect.bottom, SWP_NOSIZE|SWP_SHOWWINDOW);
#endif//MODELDIALOG
}
