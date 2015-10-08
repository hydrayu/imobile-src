// FAVConnectDlg.cpp : implementation file
//
//_____ I N C L U D E S _____________________________________________________
#include "stdafx.h"
#include "FAVEPC Utility.h"
#include "FAVConnectDlg.h"
#include "afxdialogex.h"

//BOOL m_bConnectFlag;					// RS232 connect flag
CString g_strCOMPORT;
//_____ F U N  C T I O N ____________________________________________________
// CFAVConnectDlg dialog

IMPLEMENT_DYNAMIC(CFAVConnectDlg, CDialogEx)
//===========================================================================
CFAVConnectDlg::CFAVConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFAVConnectDlg::IDD, pParent)
//===========================================================================
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_READER_G);
	m_bGetSerialPort = FALSE;
	m_bCOMSel = FALSE;
}

CFAVConnectDlg::~CFAVConnectDlg()
{
}


//===========================================================================
void CFAVConnectDlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNECT_CONNECTBTN, m_ButtonConnect);
	DDX_Control(pDX, IDC_CONNECT_OKBTN, m_ButtonOK);
	DDX_Check(pDX, IDC_CONNECT_CHECK, m_bCOMSel);
}


//===========================================================================
BEGIN_MESSAGE_MAP(CFAVConnectDlg, CDialogEx)
//===========================================================================
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CONNECT_CONNECTBTN, &CFAVConnectDlg::OnBnClickedConnectConnectbtn)
	ON_BN_CLICKED(IDC_CONNECT_OKBTN, &CFAVConnectDlg::OnBnClickedConnectOkbtn)
	ON_BN_CLICKED(IDC_CONNECT_CHECK, &CFAVConnectDlg::OnBnClickedConnectCheck)
END_MESSAGE_MAP()



// CFAVConnectDlg message handlers
//===========================================================================
BOOL CFAVConnectDlg::OnInitDialog()
//===========================================================================
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
#ifdef SENSEBTN
/*	m_ButtonOK.SubclassDlgItem(IDC_CONNECT_OKBTN, this);

	// Load the bitmaps for this button.
	m_ButtonOK.LoadBitmaps(IDB_BUTTON_NOMAL, IDB_BUTTON_DOWN, IDB_BUTTON_MOVE, IDB_BUTTON_DISABLED);

	// Resize the button to be the size of the bitmaps.
	m_ButtonOK.SizeToContent();
	m_ButtonOK.SetTextColor(RGB(0,0,0));*/
#endif // SenseBtn
	GetDlgItem(IDC_CONNECT_OKBTN)->EnableWindow(FALSE);

#ifdef SENSEBTN
/*	m_ButtonConnect.SubclassDlgItem(IDC_CONNECT_CONNECTBTN, this);

	// Load the bitmaps for this button.
	m_ButtonConnect.LoadBitmaps(IDB_BUTTON_NOMAL, IDB_BUTTON_DOWN, IDB_BUTTON_MOVE, IDB_BUTTON_DISABLED);

	// Resize the button to be the size of the bitmaps.
	m_ButtonConnect.SizeToContent();
	m_ButtonConnect.SetTextColor(RGB(128,0,0));*/
#endif // SenseBtn
	GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONNECT_COMSEL)->EnableWindow(FALSE);

	SetTimer(CREATEPORT, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//===========================================================================
void CFAVConnectDlg::OnTimer(UINT_PTR nIDEvent)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	CString	strCOM;
#ifdef SENSEBTN
	//CFAVSenseBtn SenseBtn;
#else
	CFAVXPBtn XPBtn;
#endif // SenseBtn
	KillTimer(nIDEvent);
	switch (nIDEvent)
	{
	case  CREATEPORT:
		GetDlgItem(IDC_CONNECT_DISP_U)->SetWindowText(_T("Searching the Reader..."));
		if(!m_UARTComm.COMPortAC())
		{
			GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(_T("Communication port fail. Please plug in Reader again or ") \
				_T("configuring the serial port connection manually."));
			GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(TRUE);
		}
		else
		{
			m_bConnectFlag = TRUE;
			GetDlgItem(IDC_CONNECT_DISP_U)->SetWindowText(_T("The FAVEPC Reader was verified."));
			strCOM.Format(_T("Communication %s open."), m_UARTComm.GetCOM());
			g_strCOMPORT = m_UARTComm.GetCOM();
			GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(strCOM);
			SetTimer(CREATEOK, 500, NULL);
		}
		break;// CREATEPORT

	case  CREATEOK:
		GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONNECT_OKBTN)->EnableWindow(TRUE);
#ifdef SENSEBTN
		//SenseBtn.SetBtnState(TRUE);
#else
		XPBtn.XPBtnState(TRUE);
#endif // SenseBtn
		break;// CREATEOK
	}
	CDialogEx::OnTimer(nIDEvent);
}


//===========================================================================
void CFAVConnectDlg::OnClose()
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	AfxGetApp()->m_pszAppName = _T("Close Utility");
	if(AfxMessageBox(_T("Sure you want to exit the application?"), MB_OKCANCEL|MB_ICONQUESTION)!= IDOK)
		return;
	// Indicates to the system that a thread has made a request to terminate (quit).
	// It is typically used in response to a WM_DESTROY message.
	PostQuitMessage(0);
	CDialogEx::OnClose();
}


//===========================================================================
void CFAVConnectDlg::OnBnClickedConnectConnectbtn()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_CONNECT_COMSEL);
	CString str;
	CString	strCOM;
	char	chMsg[MSGBUFSIZE];

	if(!m_bConnectFlag)
	{
		m_bConnectFlag = TRUE;
		GetDlgItem(IDC_CONNECT_DISP_U)->SetWindowText(_T("Searching the Reader..."));
		GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(TRUE);
		GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(_T(" "));
		GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(FALSE);
		if(m_bCOMSel)
		{
			//pCbox->GetLBText(pCbox->GetCurSel(), str);
			pCbox->GetWindowText(str);
			// Contrast string
			if(!COMContrast(str)){
				_stprintf_s(chMsg, _T("The communication port %s could not be opened.\r\nDevice not available"), str);
				AfxMessageBox(chMsg, MB_ICONHAND);
				GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(TRUE);
				GetDlgItem(IDC_CONNECT_DISP_U)->SetWindowText(_T(" "));
				m_bConnectFlag = FALSE;
			}
			else
			{
				if(!m_UARTComm.COMPortMC(str))
				{
					m_bConnectFlag = FALSE;
					GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(_T("Communication port fail. Please plug in Reader again or ") \
						_T("configuring the serial port connection manually."));
					//GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
					GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(TRUE);
				}
				else
				{
					strCOM.Format(_T("Communication %s open."), m_UARTComm.GetCOM());
					g_strCOMPORT = m_UARTComm.GetCOM();
					GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(strCOM);
					//GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
					GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(FALSE);
					GetDlgItem(IDC_CONNECT_OKBTN)->EnableWindow(TRUE);
				}
				GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
			}
		}
		else
		{
			if(!m_UARTComm.COMPortAC())
			{
				m_bConnectFlag = FALSE;
				GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(_T("Communication port fail. Please plug in Reader again or ") \
					_T("configuring the serial port connection manually."));
				GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
				GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(TRUE);
			}
			else
			{
				strCOM.Format(_T("Communication %s open."), m_UARTComm.GetCOM());
				g_strCOMPORT = m_UARTComm.GetCOM();
				GetDlgItem(IDC_CONNECT_DISP_D)->SetWindowText(strCOM);
				GetDlgItem(IDC_CONNECT_DISP_U)->EnableWindow(FALSE);
				GetDlgItem(IDC_CONNECT_CONNECTBTN)->EnableWindow(FALSE);
				GetDlgItem(IDC_CONNECT_OKBTN)->EnableWindow(TRUE);
			}
		}
	}
}


//===========================================================================
void CFAVConnectDlg::OnBnClickedConnectOkbtn()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


//===========================================================================
void CFAVConnectDlg::OnBnClickedConnectCheck()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CArray<CString,CString> strArrayCOM;
	UINT	uPortCount;
	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_CONNECT_COMSEL);
	CString strCOMPORT;
	if(m_bCOMSel){
		GetDlgItem(IDC_CONNECT_COMSEL)->EnableWindow(FALSE);
		m_bGetSerialPort = FALSE;
		for(UINT i = pCbox->GetCount(); i > 0; i--){
			pCbox->DeleteString(i-1);
		}
		m_bCOMSel = FALSE;
	}
	else{
		if(!m_bGetSerialPort){
			m_bGetSerialPort = TRUE;
			uPortCount = m_UARTComm.GetSerialPort(TRUE, strArrayCOM);
			for(UINT i = 0; i < uPortCount; i++){
				strCOMPORT = strArrayCOM.GetAt(m_UARTComm.GetCOMIndex(i));
				pCbox->InsertString(i,strCOMPORT);
			}
			pCbox->SetCurSel(0);
		}
		GetDlgItem(IDC_CONNECT_COMSEL)->EnableWindow(TRUE);
		m_bCOMSel = TRUE;
	}
}


//===========================================================================
BOOL CFAVConnectDlg::COMContrast(CString str)
//===========================================================================
{
	CArray<CString,CString> strArrayCOM;
	UINT	uPortCount;

	uPortCount = m_UARTComm.GetSerialPort(TRUE, strArrayCOM);
	for(UINT i = 0; i < uPortCount; i++){
		if(str == strArrayCOM.GetAt(m_UARTComm.GetCOMIndex(i)))
			return TRUE;
	}
	return FALSE;
}



