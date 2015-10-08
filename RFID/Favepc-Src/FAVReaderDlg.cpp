// FAVReaderDlg.cpp : implementation file
//
//_____ I N C L U D E S _____________________________________________________

#include "stdafx.h"
#include "FAVEPC Utility.h"
#include "FAVReaderDlg.h"
#include "afxdialogex.h"

//_____ D E F I N I T I O N S _______________________________________________
UINT uCnt(0);
//extern BOOL	m_bConnectFlag;					// RS232 connect flag
//_____ F U N  C T I O N ____________________________________________________
// CFAVReaderDlg dialog

IMPLEMENT_DYNAMIC(CFAVReaderDlg, CDialogEx)

//===========================================================================
CFAVReaderDlg::CFAVReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFAVReaderDlg::IDD, pParent)
//===========================================================================
{
	//{{AFX_DATA_INIT(CFAVReaderDlg)
	m_bRunStop			= FALSE;	
	m_nBackground		= 1;
	m_nText				= CFAVListBox::Default1;
	m_nRecText			= CFAVListBox::Default2;
	m_nUseColor			= 1;
	m_strRead			= (_T(""));
	m_strWrite			= (_T(""));
	m_strWriteList		= (_T(""));
	m_strLogFile		= (_T("FAVEPC Utility_Reader.log"));

	m_bLineNumbers		= TRUE;
	m_nGutterWidth		= 5;
	m_rgbLinenoColor	= RGB(80,80,80);
	m_rgbGutterColor	= RGB(140,200,200);

	m_bLogFile			= FALSE;

	//  m_nMemBankIndex = 0;
	m_strAddress = _T("");
	m_strWordsLength = _T("");
	//}}AFX_DATA_INIT
	m_nMemBankIndex = 0;
}

//===========================================================================
CFAVReaderDlg::~CFAVReaderDlg()
//===========================================================================
{
}


//===========================================================================
void CFAVReaderDlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_READER_LOGFILE, m_bLogFile);
	DDX_Control(pDX, IDC_READER_READEPC, m_ButtonReadEPC);
	DDX_Control(pDX, IDC_READER_READ, m_ButtonRead);
	DDX_Control(pDX, IDC_READER_WRITE, m_ButtonWrite);

	DDX_Control(pDX, IDC_READER_READEPC_DATA, m_editEPCData);
	DDX_Control(pDX, IDC_READER_READ_DATA, m_editReadData);
	DDX_Control(pDX, IDC_READER_WRITE_DATA, m_editWriteData);
	DDX_Text(pDX, IDC_READER_ADDRESS_DATA, m_strAddress);
	DDV_MaxChars(pDX, m_strAddress, 4);
	DDX_Text(pDX, IDC_READER_LENGTH_DATA, m_strWordsLength);
	DDV_MaxChars(pDX, m_strWordsLength, 2);
	DDX_CBIndex(pDX, IDC_READER_MEMBANK_DATA, m_nMemBankIndex);
	DDV_MinMaxInt(pDX, m_nMemBankIndex, 0, 3);

	DDX_Control(pDX, IDC_READER_LISTBOX_DATA, m_ListBox);
}


//===========================================================================
BEGIN_MESSAGE_MAP(CFAVReaderDlg, CDialogEx)
//===========================================================================
	//{{AFX_MSG_MAP(CFAVReaderDlg)
	ON_BN_CLICKED(IDC_READER_READEPC, &CFAVReaderDlg::OnBnClickedReaderReadepc)
	ON_BN_CLICKED(IDC_READER_REPEAT, &CFAVReaderDlg::OnBnClickedReaderRepeat)
	ON_BN_CLICKED(IDC_READER_READ, &CFAVReaderDlg::OnBnClickedReaderRead)
	ON_BN_CLICKED(IDC_READER_WRITE, &CFAVReaderDlg::OnBnClickedReaderWrite)
	ON_BN_CLICKED(IDC_READER_LOGFILE, &CFAVReaderDlg::OnBnClickedReaderLogfile)
	ON_BN_CLICKED(IDC_READER_LNO, &CFAVReaderDlg::OnBnClickedReaderLno)
	ON_CBN_SELCHANGE(IDC_READER_MEMBANK_DATA, &CFAVReaderDlg::OnCbnSelchangeReaderMembankData)
	ON_EN_CHANGE(IDC_READER_WRITE_DATA, &CFAVReaderDlg::OnEnChangeReaderWriteData)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFAVReaderDlg message handlers
//===========================================================================
BOOL CFAVReaderDlg::OnInitDialog()
//===========================================================================
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_READER_MEMBANK_DATA);
	pCbox->InsertString(0,_T("Reserved"));
	pCbox->InsertString(1,_T("EPC"));
	pCbox->InsertString(2,_T("TID"));
	pCbox->InsertString(3,_T("USER"));
	pCbox->SetCurSel(1);

	CEdit* pEditAddress = (CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA);
	pEditAddress->LimitText(4);
	pEditAddress->SetWindowText(_T("2"));

	CEdit* pEditLength = (CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA);
	pEditLength->LimitText(2);
	pEditLength->SetWindowText(_T("6"));

	m_ListBox.SetContextMenuId(IDR_LISTBOX_MENU);
	// Create tooltips
	m_ToolTips.Create(this, TTS_ALWAYSTIP);
	::SendMessage(m_ToolTips.m_hWnd, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	CString str = _T("");
	str.Format(_T("Writes communication data to %s"), m_strLogFile);
	m_ToolTips.AddTool(GetDlgItem(IDC_READER_LOGFILE), str);

	//CString str = _T("");
	str.Format(_T("Please enter the Hex type.(0 ~ 3FFF)"));
	m_ToolTips.AddTool(GetDlgItem(IDC_READER_ADDRESS_DATA), str);
	str.Format(_T("Please enter the Hex type.(1 ~ 20)\r\nRead Length: 1 ~ 20\r\nWrite Length: 1 ~ 10"));
	m_ToolTips.AddTool(GetDlgItem(IDC_READER_LENGTH_DATA), str);
	// wade
	CButton* pBtn = (CButton*)GetDlgItem(IDC_READER_REPEAT);
	if (pBtn != NULL) 
		pBtn->SetCheck(1);
	SetTimer(STARTREAD, 1000, NULL);
	m_bGetOverResultWait = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//===========================================================================
BOOL CFAVReaderDlg::PreTranslateMessage(MSG* pMsg)
//===========================================================================
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;
		if(nVirtKey <= VK_HELP && (nVirtKey != VK_BACK) && (nVirtKey != VK_SHIFT) &&
			(nVirtKey != VK_TAB) && (nVirtKey != VK_SPACE))
		{
			return TRUE;
		}
		if(nVirtKey > 'F' && (nVirtKey < VK_NUMPAD0 || nVirtKey > VK_NUMPAD9))	
			return TRUE;
	}
	if(pMsg->message == WM_CHAR)
	{
		if(pMsg->wParam >= 'a' && pMsg->wParam <= 'f')
			pMsg->wParam -= 32;								// Turn to upper text
	}

	if((pMsg->hwnd == 0) || !IsWindow(pMsg->hwnd))
		return TRUE;

	m_ToolTips.RelayEvent(pMsg);

	if((pMsg->hwnd == 0) || !IsWindow(pMsg->hwnd))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void GenerateKey(byte vk, byte scan)
{
	if (vk <= 'Z' && vk >= 'A') keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(vk, scan, 0, 0);
	keybd_event(vk, scan, KEYEVENTF_KEYUP, 0);
	if (vk <= 'Z' && vk >= 'A') keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderReadepc()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_READER_READEPC)->EnableWindow(FALSE);

	static int count=0;
	CString strBuf = (_T(""));

	if(!m_bRunStop)
	{
		if(IsDlgButtonChecked(IDC_READER_REPEAT) == BST_CHECKED)
		{
			m_bRunStop = TRUE;
			GetDlgItem(IDC_READER_READEPC)->SetWindowText(_T("&Stop"));
			GetDlgItem(IDC_READER_REPEAT)->EnableWindow(FALSE);
			GetDlgItem(IDC_READER_READ)->EnableWindow(FALSE);
			GetDlgItem(IDC_READER_WRITE)->EnableWindow(FALSE);
			GetDlgItem(IDC_READER_LNO)->EnableWindow(FALSE);
			GetDlgItem(IDC_READER_LOGFILE)->EnableWindow(FALSE);
			SetTimer(MULTIEPC, 100, NULL);
		}
		else
		{
			char chCMD[] = COMMAND_Q;
			uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> Q"));

			if(m_SerialComm.WriteCMD(CMD_Q, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
			{
				TRACE(_T(">>> Write Fail in Read EPC Button.\n"));
				GetDlgItem(IDC_READER_READEPC)->EnableWindow(TRUE);
				return;
			}
			if(m_SerialComm.ReadCMD(CMD_Q, READ_DEFAULT_TIMEOUT))
			{
				strBuf = m_SerialComm.GetCMDData(CMD_Q);
				strBuf.Remove('Q');
				m_editEPCData.SetSel(0,-1);
				m_editEPCData.Clear();
				m_editEPCData.ReplaceSel(strBuf);
				if (strBuf.GetLength() > 0) {
					// wade, send to keyboard here
					for (int i=0; i<strBuf.GetLength(); i++) {
						GenerateKey((byte)strBuf[i], VkKeyScan(strBuf[i]));
					}
					GenerateKey(VK_RETURN, VkKeyScan(VK_RETURN));
				}

				uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
					m_SerialComm.GetCMDData(CMD_Q));

				ASSERT(uCnt != LB_ERR);

				if (uCnt != LB_ERR)
				{
					// verify colors
					int nText = m_ListBox.GetTextColor(uCnt);
					int nBackground = m_ListBox.GetBackgroundColor(uCnt);
					//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
					if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
					{
						ASSERT(FALSE);
					}
				}
			}
			GetDlgItem(IDC_READER_READEPC)->EnableWindow(TRUE);
		}
	}
	else//Stop run
	{
		m_bRunStop = FALSE;
		KillTimer(MULTIEPC);
		GetDlgItem(IDC_READER_READEPC)->SetWindowText(_T("Read &EPC"));
		GetDlgItem(IDC_READER_READEPC)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_REPEAT)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_READ)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_LNO)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_LOGFILE)->EnableWindow(TRUE);
	}
	GetDlgItem(IDC_READER_READEPC)->SetFocus();
}


//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderRepeat()
//===========================================================================
{
	// TODO: Add your control notification handler code here
}


//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderRead()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_READER_READ)->EnableWindow(FALSE);

	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_READER_MEMBANK_DATA);
	CEdit* pEditA = (CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA);
	CEdit* pEditL = (CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA);
	CString strBuf = (_T(""));

	m_nMemBankIndex = pCbox->GetCurSel();
	pEditA->GetWindowText(m_strAddress);
	if(m_strAddress == _T("")){
		AfxMessageBox(_T("Can not be ' '(null character), Please setting the Address."), MB_ICONHAND);
		GotoDlgCtrl((CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA));
		goto READEXIT;
	}

	pEditL->GetWindowText(m_strWordsLength);
	if(m_strWordsLength == _T("")){
		AfxMessageBox(_T("Can not be ' '(null character), Please setting the Length."), MB_ICONHAND);
		GotoDlgCtrl((CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA));
		goto READEXIT;
	}

	char chRead[12];
	memset(chRead,'\0',12);
	//CString strRead;
	m_strRead.Format(_T("R%1d,%s,%s"), m_nMemBankIndex, m_strAddress, m_strWordsLength);
	int nstrLength = m_strRead.GetLength();
	for(int i = 0; i < nstrLength; i++){
		chRead[i] = m_strRead[i];
	}
	chRead[nstrLength] = (char)0x0d;

	m_strRead = (_T(">>> ")) + m_strRead;
	uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, m_strRead);

	if(m_SerialComm.WriteCMD(CMD_R, chRead, (nstrLength+1), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in Read Button.\n"));
		goto READEXIT;
	}
	if(m_SerialComm.ReadCMD(CMD_R, READ_DEFAULT_TIMEOUT))
	{
		strBuf = m_SerialComm.GetCMDData(CMD_R);
		strBuf.Remove('R');
		if(strBuf == '0'){
			strBuf = _T("(0: Other Error)");
		}
		if(strBuf == '3'){
			strBuf = _T("(3: Memory Overrun)");
		}
		if(strBuf == '4'){
			strBuf = _T("(4: Memory Locked)");
		}
		if(strBuf == 'B'){
			strBuf = _T("(B: Insufficient Power)");
		}
		m_editReadData.SetSel(0,-1);
		m_editReadData.Clear();
		m_editReadData.ReplaceSel(strBuf);

		uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_R));

		ASSERT(uCnt != LB_ERR);

		if (uCnt != LB_ERR)
		{
			// verify colors
			int nText = m_ListBox.GetTextColor(uCnt);
			int nBackground = m_ListBox.GetBackgroundColor(uCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}

READEXIT:
	GetDlgItem(IDC_READER_READ)->EnableWindow(TRUE);
	GetDlgItem(IDC_READER_READ)->SetFocus();
}


//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderWrite()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_READER_WRITE)->EnableWindow(FALSE);

	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_READER_MEMBANK_DATA);
	CEdit* pEditA = (CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA);
	CEdit* pEditL = (CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA);
	CEdit* pEditW = (CEdit*)GetDlgItem(IDC_READER_WRITE_DATA);
	CStatic* pStaticW = (CStatic*)GetDlgItem(IDC_READER_WRITE_INFO);
	CString strBuf = (_T(""));
	CString strBuf_ = (_T(""));
	char* pchWrite = nullptr;

	m_nMemBankIndex = pCbox->GetCurSel();
	pEditA->GetWindowText(m_strAddress);
	if(m_strAddress == _T("")){
		AfxMessageBox(_T("Can not be ' '(null character), Please setting the Address."), MB_ICONHAND);
		GotoDlgCtrl((CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA));
		GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
		return;
	}

	pEditL->GetWindowText(m_strWordsLength);
	if(m_strWordsLength == _T("")){
		AfxMessageBox(_T("Can not be ' '(null character), Please setting the Length."), MB_ICONHAND);
		GotoDlgCtrl((CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA));
		GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
		return;
	}

	pEditW->GetWindowText(m_strWrite);
	int nWriteLength = m_strWrite.GetLength();
	int nWordsLength = CStringHexToInt(m_strWordsLength);

	if(nWriteLength > nWordsLength*4){
		AfxMessageBox(_T("Write data length is greater than the setting length.\r\nPlease reset the setting length."));
		GotoDlgCtrl((CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA));
		GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
		return;
	}

	if(nWriteLength < nWordsLength*4){
		int nZero;
		for(int i = 0; i < ( nWordsLength*4 - nWriteLength); i++){
			nZero = m_strWrite.Insert(nWriteLength+i,_T("0"));
		}
		m_editWriteData.SetSel(0,-1);
		m_editWriteData.Clear();
		m_editWriteData.ReplaceSel(m_strWrite);
	}

	m_strWriteList.Format(_T("W%1d,%s,%s,%s"), m_nMemBankIndex, m_strAddress, m_strWordsLength, m_strWrite);
	int nstrLength = m_strWriteList.GetLength(); 
	pchWrite = new char[nstrLength + 1];
	for(int i = 0; i < nstrLength; i++){
		*(pchWrite + i) = m_strWriteList[i];
	}
	*(pchWrite + nstrLength) = (char)0x0d;

	m_strWriteList = (_T(">>> ")) + m_strWriteList;
	uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, m_strWriteList);

	if(m_SerialComm.WriteCMD(CMD_W, pchWrite, (nstrLength + 1), WRITE_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in Write Button.\n"));
		goto WRITEERROR;
	}
	if(m_SerialComm.ReadCMD(CMD_W, READ_TIMEOUT))
	{
		strBuf = m_SerialComm.GetCMDData(CMD_W);
		//strBuf.Remove('R');
		switch (strBuf[0]){
		case '0':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(0: Other Error)");
			}
			else{
				strBuf.Remove('0');
				strBuf.Remove('Z');
				strBuf_.Format(_T("(0Z%s: Other Error, %s words is written)"), strBuf,strBuf);
			}
			break;//'0'
		case '3':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(3: Memory Overrun)");
			}
			else{
				strBuf.Remove('3');
				strBuf.Remove('Z');
				strBuf_.Format(_T("(3Z%s: Memory Overrun, %s words is written)"), strBuf,strBuf);
			}
			break;//'3'
		case '4':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(4: Memory Locked)");
			}
			else{
				strBuf.Remove('4');
				strBuf.Remove('Z');
				strBuf_.Format(_T("(4Z%s: Memory Locked, %s words is written)"), strBuf,strBuf);
			}
			break;//'4'
		case 'Z':
			strBuf.Remove('Z');
			strBuf_.Format(_T("(Z%s: %s words is written)"), strBuf,strBuf);
			break;//'Z'
		case 'B':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(B: Insufficient Power)");
			}
			else{
				strBuf.Remove('B');
				strBuf.Remove('Z');
				strBuf_.Format(_T("(BZ%s: Insufficient power, %s words is written)"), strBuf,strBuf);
			}
			break;//'X'
		case 'W':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(W: No Tag)");
			}
			else
				strBuf_ = strBuf;
			break;//'W'
		case 'X':
			strBuf_ = _T("(X: Out of spec.)");
			break;//'X'
		case 'F':
			if(strBuf.GetLength() == 1){
				strBuf_ = _T("(F: Non-specific error)");
			}
			else{
				strBuf.Remove('F');
				strBuf.Remove('Z');
				strBuf_.Format(_T("(FZ%s: Non-specific error, %s words is written)"), strBuf,strBuf);
			}
			break;//'F'
		default:
			strBuf_ = strBuf;
			break;
		}

		pStaticW->SetWindowText(strBuf_);
		RefreshControl(IDC_READER_WRITE_INFO);

		uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, m_SerialComm.GetCMDData(CMD_W));

		ASSERT(uCnt != LB_ERR);

		if (uCnt != LB_ERR)
		{
			// verify colors
			int nText = m_ListBox.GetTextColor(uCnt);
			int nBackground = m_ListBox.GetBackgroundColor(uCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
WRITEERROR:
	if(pchWrite){
		delete [] pchWrite;
	}
	pchWrite = NULL;

	GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
	GetDlgItem(IDC_READER_WRITE)->SetFocus();
}


//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderLogfile()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bLogFile)
		m_ListBox.SetReaderLogFile(m_strLogFile);
	else
		m_ListBox.SetReaderLogFile(_T(""));
}


//===========================================================================
void CFAVReaderDlg::OnBnClickedReaderLno()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	CFAVLNODlg dlg;

	dlg.m_bLineNumbers   = m_bLineNumbers;
	dlg.m_nGutterWidth   = m_nGutterWidth;
	dlg.m_rgbLinenoColor = m_rgbLinenoColor;
	dlg.m_rgbGutterColor = m_rgbGutterColor;

	if (dlg.DoModal())
	{
		m_bLineNumbers   = dlg.m_bLineNumbers;
		m_ListBox.EnableLineNumbers(m_bLineNumbers);

		if (m_bLineNumbers)
		{
			m_nGutterWidth   = dlg.m_nGutterWidth;
			m_rgbLinenoColor = dlg.m_rgbLinenoColor;
			m_rgbGutterColor = dlg.m_rgbGutterColor;
			m_ListBox.SetGutterWidth(m_nGutterWidth);
			m_ListBox.SetLineNoColor(m_rgbLinenoColor);
			m_ListBox.SetGutterColor(m_rgbGutterColor);
		}

		m_ListBox.RedrawWindow();
	}
}


//===========================================================================
void CFAVReaderDlg::OnCbnSelchangeReaderMembankData()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_READER_MEMBANK_DATA);
	CEdit* pEditAddress = (CEdit*)GetDlgItem(IDC_READER_ADDRESS_DATA);
	CEdit* pEditLength = (CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA);

	switch (pCbox->GetCurSel()){
	case 0:
		pEditAddress->SetWindowText(_T("0"));
		pEditLength->SetWindowText(_T("4"));
		break;//0
	case 1:
		pEditAddress->SetWindowText(_T("2"));
		pEditLength->SetWindowText(_T("6"));
		break;//1
	case 2:
		pEditAddress->SetWindowText(_T("0"));
		pEditLength->SetWindowText(_T("4"));
		break;//2
	case 3:
		pEditAddress->SetWindowText(_T("0"));
		pEditLength->SetWindowText(_T("1"));
		break;//3
	}
}


//===========================================================================
void CFAVReaderDlg::OnEnChangeReaderWriteData()
//===========================================================================
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* pEditW = (CEdit*)GetDlgItem(IDC_READER_WRITE_DATA);
	CEdit* pEditL = (CEdit*)GetDlgItem(IDC_READER_LENGTH_DATA);
	CString strW = _T("");
	CString strL = _T("");
	pEditW->GetWindowText(strW);
	pEditL->GetWindowText(strL);
	int nWriteLength = strW.GetLength();
	int nWordsLength = CStringHexToInt(strL);
	int nDiff;
	nDiff = nWordsLength*4 - nWriteLength;
	if(nDiff > 0)
		strW.Format(_T("Need to enter the %d text."), nDiff);
	else
		strW.Format(_T("More than %d text."), -nDiff);

	m_ToolTips.AddTool(GetDlgItem(IDC_READER_WRITE_DATA), strW);
}


//===========================================================================
void CFAVReaderDlg::OnTimer(UINT_PTR nIDEvent)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	CString	strBuf = (_T(""));
	char	chCMD[] = COMMAND_Q;

	KillTimer(nIDEvent);

	switch (nIDEvent){
	case MULTIEPC:
		// Counter the line
		uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> Q"));

		// Write Q CMD for repeat.
		if(m_SerialComm.WriteCMD(CMD_Q, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
		{
			TRACE(_T(">> Write Fail in Repeat Read EPC Button."));
		}

		// Read file
		if(m_SerialComm.ReadCMD(CMD_Q, READ_DEFAULT_TIMEOUT))
		{
			strBuf = m_SerialComm.GetCMDData(CMD_Q);
			strBuf.Remove('Q');
			// wade
			if (strBuf.GetLength() > 0) {
				m_editEPCData.SetSel(0,-1);
				m_editEPCData.Clear();
				m_editEPCData.ReplaceSel(strBuf);
				// wade, send to keyboard here
				for (int i=0; i<strBuf.GetLength(); i++) {
					GenerateKey((byte)strBuf[i], 0);
				}
				GenerateKey(VK_RETURN, 0);
			}
			// Counter the line
			uCnt = m_ListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
				m_SerialComm.GetCMDData(CMD_Q));

			ASSERT(uCnt != LB_ERR);

			if (uCnt != LB_ERR)
			{
				// Verify colors
				int nText = m_ListBox.GetTextColor(uCnt);
				int nBackground = m_ListBox.GetBackgroundColor(uCnt);

				if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
				{
					ASSERT(FALSE);
				}
			}
		}
		if(IsDlgButtonChecked(IDC_READER_REPEAT) == BST_CHECKED && m_bRunStop){
			if(m_bConnectFlag) {
				if (strBuf.GetLength() > 0)
					SetTimer(MULTIEPC, 1000, NULL);
				else
					SetTimer(MULTIEPC, 100, NULL);
			}
		}

		GetDlgItem(IDC_READER_READEPC)->EnableWindow(TRUE);
		break;// MULTIEPC

	case SINGLEEPC:
		GetDlgItem(IDC_READER_READEPC)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_REPEAT)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_READ)->EnableWindow(TRUE);
		GetDlgItem(IDC_READER_WRITE)->EnableWindow(TRUE);
		break;// SINGLEEPC
	case STARTREAD:
		CButton* pBtn = (CButton*)GetDlgItem(IDC_READER_READEPC);
		if (pBtn != NULL) pBtn->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_READER_READEPC, BN_CLICKED), (DWORD)pBtn->m_hWnd);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


//===========================================================================
HBRUSH CFAVReaderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//===========================================================================
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if(pWnd->GetDlgCtrlID() == IDC_READER_WRITE_INFO){
		// Set the text color to red
		pDC->SetTextColor(RGB(255, 0, 32));
		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(OPAQUE);
		// Return handle to our CBrush object
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	return hbr;
}


//===========================================================================
int CFAVReaderDlg::GetRandomColor(int nColor)
//===========================================================================
{
	// Retrieves the number of milliseconds that have elapsed since 
	// the system was started, up to 49.7 days.
	DWORD dwTicks = GetTickCount();
	int rc = (int) dwTicks & 0xFF;
	rc += 3*nColor;

	return (int) rc & 0xF;
}

//===========================================================================
int CFAVReaderDlg::CStringHexToInt(CString str)
//===========================================================================
{
	int nRet = 0;
	int count = 1;
	for(int i = str.GetLength()-1; i >= 0; --i)
	{
		int nNum = 0;
		char chText;
		chText = str.GetAt(i);
		if (chText >= '0' && chText <= '9')
		{
			nNum = chText - '0';
		}
		else if (chText >= 'A' && chText <= 'F')
		{
			nNum = chText - 'A' + 10;
		}
		else if (chText >= 'a' && chText <= 'f')
		{
			nNum = chText - 'a' + 10;
		}
		nRet += nNum*count;
		count *= 16;

	}
	return nRet;
}

//===========================================================================
void CFAVReaderDlg::RefreshControl(UINT uCtlID)
//===========================================================================
{
	CRect rc;     
	GetDlgItem(uCtlID)->GetWindowRect(&rc);   
	ScreenToClient(&rc);     
	InvalidateRect(rc);
}

