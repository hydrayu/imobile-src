// FAVRMCDlg.cpp : implementation file
//
//_____ I N C L U D E S _____________________________________________________
#include "stdafx.h"
#include "FAVEPC Utility.h"
#include "FAVRMCDlg.h"
#include "afxdialogex.h"
#include ".\FAVEPC\FAVEnGroupboxCtrl.h"


UINT nPairCnt(0);
UINT uRMCCnt(0);
// CFAVRMCDlg dialog
//_____ F U N  C T I O N ____________________________________________________
IMPLEMENT_DYNAMIC(CFAVRMCDlg, CDialogEx)

//===========================================================================
CFAVRMCDlg::CFAVRMCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFAVRMCDlg::IDD, pParent)
//===========================================================================
{
	m_bBK				= TRUE;
	m_bBtnPower			= FALSE;
	m_nBackground		= CFAVListBox::Silver;
	m_nText				= CFAVListBox::Black;
	m_nRecText			= CFAVListBox::Default1;
	m_strRMCLogFile		= (_T("FAVEPC Utility_RMC.log"));
}


CFAVRMCDlg::~CFAVRMCDlg()
{
}


//===========================================================================
void CFAVRMCDlg::DoDataExchange(CDataExchange* pDX)
//===========================================================================
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RMC_DEMO_PAIR, m_ButtonPair);
	DDX_Control(pDX, IDC_RMC_DEMO_POWER, m_ButtonPower);
	DDX_Control(pDX, IDC_RMC_DEMO_BULBOFF, m_ButtonBulbOFF);
	DDX_Control(pDX, IDC_RMC_DEMO_BULBON, m_ButtonBulbON);
	DDX_Control(pDX, IDC_RMC_DEMO_BON, m_ButtonBuleON);
	DDX_Control(pDX, IDC_RMC_DEMO_BOFF, m_ButtonBuleOFF);
	DDX_Control(pDX, IDC_RMC_DEMO_GON, m_ButtonGreenON);
	DDX_Control(pDX, IDC_RMC_DEMO_GOFF, m_ButtonGreenOFF);
	DDX_Control(pDX, IDC_RMC_DEMO_RON, m_ButtonRedON);
	DDX_Control(pDX, IDC_RMC_DEMO_ROFF, m_ButtonRedOFF);
	DDX_Check(pDX, IDC_RMC_LOGFILE, m_bRMCLogFile);
	DDX_Control(pDX, IDC_RMC_LISTBOX_DATA, m_RMCListBox);
	DDX_Control(pDX, IDC_RMC_SEQUENCES_DATA, m_ListCtrl);
}


//===========================================================================
BEGIN_MESSAGE_MAP(CFAVRMCDlg, CDialogEx)
//===========================================================================
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RMC_SEND, &CFAVRMCDlg::OnBnClickedRmcSend)
	ON_CBN_SELCHANGE(IDC_RMC_CMD_DATA, &CFAVRMCDlg::OnCbnSelchangeRmcCmdData)
	ON_BN_CLICKED(IDC_RMC_LOGFILE, &CFAVRMCDlg::OnBnClickedRmcLogfile)
	ON_BN_CLICKED(IDC_RMC_DEMO_POWER, &CFAVRMCDlg::OnBnClickedRmcDemoPower)
	ON_BN_CLICKED(IDC_RMC_DEMO_BULBON, &CFAVRMCDlg::OnBnClickedRmcDemoBulbon)
	ON_BN_CLICKED(IDC_RMC_DEMO_BULBOFF, &CFAVRMCDlg::OnBnClickedRmcDemoBulboff)
	ON_BN_CLICKED(IDC_RMC_DEMO_RON, &CFAVRMCDlg::OnBnClickedRmcDemoRon)
	ON_BN_CLICKED(IDC_RMC_DEMO_ROFF, &CFAVRMCDlg::OnBnClickedRmcDemoRoff)
	ON_BN_CLICKED(IDC_RMC_DEMO_GON, &CFAVRMCDlg::OnBnClickedRmcDemoGon)
	ON_BN_CLICKED(IDC_RMC_DEMO_BON, &CFAVRMCDlg::OnBnClickedRmcDemoBon)
	ON_BN_CLICKED(IDC_RMC_DEMO_GOFF, &CFAVRMCDlg::OnBnClickedRmcDemoGoff)
	ON_BN_CLICKED(IDC_RMC_DEMO_BOFF, &CFAVRMCDlg::OnBnClickedRmcDemoBoff)
	ON_BN_CLICKED(IDC_RMC_DEMO_PAIR, &CFAVRMCDlg::OnBnClickedRmcDemoPair)
END_MESSAGE_MAP()


// CFAVRMCDlg message handlers
//===========================================================================
BOOL CFAVRMCDlg::OnInitDialog()
//===========================================================================
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//! @defgroup Setting groups initialization
	//! @{
	//! @{ Key Define
	CEdit* pEditKeyDef = (CEdit*)GetDlgItem(IDC_RMC_KEYDEFINE_DATA);
	pEditKeyDef->SetWindowText(_T("SN pairing"));
	//! @}

	//! @{ Command
	CComboBox* pCMDCBox = (CComboBox*)GetDlgItem(IDC_RMC_CMD_DATA);
	pCMDCBox->InsertString(0,_T("M0"));
	pCMDCBox->InsertString(1,_T("M1"));
	pCMDCBox->SetCurSel(0);
	//! @}

	//! @{ Key
	CEdit* pEditKey = (CEdit*)GetDlgItem(IDC_RMC_KEY_DATA);
	pEditKey->SetWindowText(_T("50"));
	//! @}

	//! @{ Flag
	CComboBox* pFLAGCBox = (CComboBox*)GetDlgItem(IDC_RMC_FLAG_DATA);
	pFLAGCBox->InsertString(0,_T("0: No Double Key"));
	pFLAGCBox->InsertString(1,_T("1: Flag1"));
	pFLAGCBox->InsertString(2,_T("2: Flag2"));
	pFLAGCBox->InsertString(3,_T("3: Flag1 + Flag2"));
	pFLAGCBox->InsertString(4,_T("4: Flag3"));
	pFLAGCBox->InsertString(5,_T("5: Flag1 + Flag3"));
	pFLAGCBox->InsertString(6,_T("6: Flag2 + Flag3"));
	pFLAGCBox->SetCurSel(0);
	//! @}

	//! @{ SN
	CEdit* pEditSN = (CEdit*)GetDlgItem(IDC_RMC_SN_DATA);
	pEditSN->LimitText(8);
	pEditSN->SetWindowText(_T("FFFFFFFF"));
	pEditSN->EnableWindow(FALSE);
	//! @}

	//! @{ Number of times
	CEdit* pEditTimes = (CEdit*)GetDlgItem(IDC_RMC_TIMES_DATA);
	pEditTimes->LimitText(2);
	pEditTimes->SetWindowText(_T("37"));
	//! @}
	EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_SETTINGS), FALSE);		//Close groups
	//! @}


	//! @defgroup Send Sequences groups initialization
	//! @{
	//! @{ ListCtrl
	m_ListCtrl.InsertColumn(0, "Key Define", LVCFMT_CENTER, 100);
	m_ListCtrl.InsertColumn(1, "Command", LVCFMT_CENTER, 75);
	m_ListCtrl.InsertColumn(2, "Key", LVCFMT_CENTER, 40);
	m_ListCtrl.InsertColumn(3, "Flag", LVCFMT_CENTER, 50);
	m_ListCtrl.InsertColumn(4, "SN", LVCFMT_CENTER, 80);
	m_ListCtrl.InsertColumn(5, "Times", LVCFMT_CENTER, 40);
	m_ListCtrl.SetColumnWidth(5,LVSCW_AUTOSIZE_USEHEADER);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);
	// http://msdn.microsoft.com/zh-tw/magazine/cc164140(en-us).aspx
	// Fixed column width
	CHeaderCtrl* pHeaderCtrl = (CHeaderCtrl*)m_ListCtrl.GetHeaderCtrl();
	pHeaderCtrl-> EnableWindow(FALSE);
	//! @}
	//EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_SEQUENCES), FALSE);		//Close groups
	//! @}

	// Create tooltips
	m_ToolTipsRMC.Create(this, TTS_ALWAYSTIP);
	::SendMessage(m_ToolTipsRMC.m_hWnd, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	CString str = _T("");
	str.Format(_T("Power"));
	m_ToolTipsRMC.AddTool(GetDlgItem(IDC_RMC_DEMO_POWER), str);
	str.Format(_T("Light OFF"));
	m_ToolTipsRMC.AddTool(GetDlgItem(IDC_RMC_DEMO_BULBOFF), str);

	CRect BtnRectPower; 
	m_ButtonPower.GetWindowRect(BtnRectPower);
	ScreenToClient(BtnRectPower);
	m_ButtonPower.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectPower);
	m_ButtonPower.SetRgnStyle(0);
	m_ButtonPower.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonPower.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonPower.SetBitmapId(0, IDB_RMCDEMO_POWER_NOMAL, IDB_RMCDEMO_POWER_MOVE,
		IDB_RMCDEMO_POWER_DOWN, IDB_RMCDEMO_POWER_MOVE);

	CRect BtnRectBulbOFF; 
	m_ButtonBulbOFF.GetWindowRect(BtnRectBulbOFF);
	ScreenToClient(BtnRectBulbOFF);
	m_ButtonBulbOFF.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectBulbOFF);
	m_ButtonBulbOFF.SetRgnStyle(0);
	m_ButtonBulbOFF.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBulbOFF.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBulbOFF.SetBitmapId(1, IDB_RMCDEMO_BULBOFF_NORMAL, IDB_RMCDEMO_BULBOFF_MOVE,
		IDB_RMCDEMO_BULBOFF_DOWN, IDB_RMCDEMO_BULBOFF_MOVE);

	CRect BtnRectBulbON; 
	m_ButtonBulbON.GetWindowRect(BtnRectBulbON);
	ScreenToClient(BtnRectBulbON);
	m_ButtonBulbON.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectBulbON);
	m_ButtonBulbON.SetRgnStyle(0);
	m_ButtonBulbON.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBulbON.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBulbON.SetBitmapId(2, IDB_RMCDEMO_BULBON_NORMAL, IDB_RMCDEMO_BULBON_MOVE,
		IDB_RMCDEMO_BULBON_DOWN, IDB_RMCDEMO_BULBON_MOVE);

	CRect BtnRectBON; 
	m_ButtonBuleON.GetWindowRect(BtnRectBON);
	ScreenToClient(BtnRectBON);
	m_ButtonBuleON.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectBON);
	m_ButtonBuleON.SetRgnStyle(0);
	m_ButtonBuleON.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBuleON.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBuleON.SetBitmapId(3, IDB_RMCDEMO_BON_NORMAL, IDB_RMCDEMO_BON_MOVE,
		IDB_RMCDEMO_BON_DOWN, IDB_RMCDEMO_BON_MOVE);

	CRect BtnRectBOFF; 
	m_ButtonBuleOFF.GetWindowRect(BtnRectBOFF);
	ScreenToClient(BtnRectBOFF);
	m_ButtonBuleOFF.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectBOFF);
	m_ButtonBuleOFF.SetRgnStyle(0);
	m_ButtonBuleOFF.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBuleOFF.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonBuleOFF.SetBitmapId(4, IDB_RMCDEMO_BOFF_NOMAL, IDB_RMCDEMO_BOFF_MOVE,
		IDB_RMCDEMO_BOFF_DOWN, IDB_RMCDEMO_BOFF_MOVE);

	CRect BtnRectGON; 
	m_ButtonGreenON.GetWindowRect(BtnRectGON);
	ScreenToClient(BtnRectGON);
	m_ButtonGreenON.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectGON);
	m_ButtonGreenON.SetRgnStyle(0);
	m_ButtonGreenON.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonGreenON.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonGreenON.SetBitmapId(5, IDB_RMCDEMO_GON_NORMAL, IDB_RMCDEMO_GON_MOVE,
		IDB_RMCDEMO_GON_DOWN, IDB_RMCDEMO_GON_MOVE);

	CRect BtnRectGOFF; 
	m_ButtonGreenOFF.GetWindowRect(BtnRectGOFF);
	ScreenToClient(BtnRectGOFF);
	m_ButtonGreenOFF.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectGOFF);
	m_ButtonGreenOFF.SetRgnStyle(0);
	m_ButtonGreenOFF.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonGreenOFF.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonGreenOFF.SetBitmapId(6, IDB_RMCDEMO_GOFF_NORMAL, IDB_RMCDEMO_GOFF_MOVE,
		IDB_RMCDEMO_GOFF_DOWN, IDB_RMCDEMO_GOFF_MOVE);

	CRect BtnRectRON; 
	m_ButtonRedON.GetWindowRect(BtnRectRON);
	ScreenToClient(BtnRectRON);
	m_ButtonRedON.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectRON);
	m_ButtonRedON.SetRgnStyle(0);
	m_ButtonRedON.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonRedON.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonRedON.SetBitmapId(7, IDB_RMCDEMO_RON_NORMAL, IDB_RMCDEMO_RON_MOVE,
		IDB_RMCDEMO_RON_DOWN, IDB_RMCDEMO_RON_MOVE);

	CRect BtnRectROFF; 
	m_ButtonRedOFF.GetWindowRect(BtnRectROFF);
	ScreenToClient(BtnRectROFF);
	m_ButtonRedOFF.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectROFF);
	m_ButtonRedOFF.SetRgnStyle(0);
	m_ButtonRedOFF.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonRedOFF.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonRedOFF.SetBitmapId(8, IDB_RMCDEMO_ROFF_NORMAL, IDB_RMCDEMO_ROFF_MOVE,
		IDB_RMCDEMO_ROFF_DOWN, IDB_RMCDEMO_ROFF_MOVE);

	CRect BtnRectPair; 
	m_ButtonPair.GetWindowRect(BtnRectPair);
	ScreenToClient(BtnRectPair);
	m_ButtonPair.SetBackBmp(IDB_RMCDEMO_BK1, BtnRectPair);
	m_ButtonPair.SetRgnStyle(0);
	m_ButtonPair.SetRgnMask(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonPair.SetMaskBitmapId(IDB_RMCDEMO_MASK, TRUE);
	m_ButtonPair.SetBitmapId(9, IDB_RMCDEMO_PAIR_NORMAL, IDB_RMCDEMO_PAIR_MOVE,
		IDB_RMCDEMO_PAIR_DOWN, IDB_RMCDEMO_PAIR_MOVE);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//===========================================================================
void CFAVRMCDlg::OnPaint()
//===========================================================================
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	if(m_bBK)
	{
		m_bBK = FALSE;
		m_backBmp.LoadBitmap(IDB_RMCDEMO_BK1);
		m_backDC.CreateCompatibleDC(&dc);
		m_backDC.SelectObject(m_backBmp);
		
	}
	dc.BitBlt(10, 20, 210, 360, &m_backDC, 0, 0, SRCCOPY);
}


//===========================================================================
void CFAVRMCDlg::OnTimer(UINT_PTR nIDEvent)
//===========================================================================
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_RMC_MSG);

	switch (nIDEvent){
	case SNPAIRING:
		nPairCnt++;
		TRACE(_T(">>> nPairCnt = %d\n"), nPairCnt);
		if(nPairCnt == 0x37){
			nPairCnt = 0;
			KillTimer(nIDEvent);
			SetTimer(SNPAIRING_R, 90, NULL);
		}
		break;//SNPAIRING
	case SNPAIRING_R:
		KillTimer(nIDEvent);
		if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
		{
			pStatic->SetWindowText(m_SerialComm.GetCMDData(CMD_RMC));
		}
		else
		{
			//m_SerialComm.m_strCMDRMC = (_T("M"));
			pStatic->SetWindowText(_T("M"));
			TRACE(_T(">>> SN Pairing Fail\n"));
		}

		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}


		// Refresh the Static Control
		GetDlgItem(IDC_RMC_MSG)->GetWindowRect(&rc);   
		ScreenToClient(&rc);     
		InvalidateRect(rc); 
		EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_MAPGROUP), TRUE);

		break;//SNPAIRING_R
	case RMCBUTTON:
		KillTimer(nIDEvent);

		if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
		{
			uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
				m_SerialComm.GetCMDData(CMD_RMC));
			ASSERT(uRMCCnt != LB_ERR);
			if (uRMCCnt != LB_ERR)
			{
				// Verify colors
				int nText = m_RMCListBox.GetTextColor(uRMCCnt);
				int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
				//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
				if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
				{
					ASSERT(FALSE);
				}
			}
		}
		else
		{
			TRACE(_T(">>> Read Fail\n"));
		}
		/*GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_BULBON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_BULBOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_BON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_BOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_GON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_GOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_RON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_ROFF)->EnableWindow(TRUE);*/
		break;//SNPAIRING_R
	}
	CDialogEx::OnTimer(nIDEvent);
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcSend()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_SEND)->EnableWindow(FALSE);
	EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_MAPGROUP), FALSE);

	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_RMC_MSG);
	pStatic->SetWindowText(_T("Please waiting..."));

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,50,0,37"));

	char cchCMD[] = COMMAND_SN;
	m_SerialComm.WriteCMD(CMD_RMC, cchCMD, sizeof(cchCMD), WRITE_DEFAULT_TIMEOUT);

	SetTimer(SNPAIRING, 90, NULL);
}


//===========================================================================
void CFAVRMCDlg::OnCbnSelchangeRmcCmdData()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CComboBox* pCbox = (CComboBox*)GetDlgItem(IDC_RMC_CMD_DATA);

	switch (pCbox->GetCurSel()){
	case 0:
		GetDlgItem(IDC_RMC_SN_DATA)->EnableWindow(FALSE);
		break;//0
	case 1:
		GetDlgItem(IDC_RMC_SN_DATA)->EnableWindow(TRUE);
		break;//1
	}
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcLogfile()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bRMCLogFile)
		m_RMCListBox.SetRMCLogFile(m_strRMCLogFile);
	else
		m_RMCListBox.SetRMCLogFile(_T(""));
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoPower()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(FALSE);

	if(m_bBtnPower){
		m_bBtnPower = FALSE;
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,73,0,2"));
		char chCMD[] = COMMAND_LOF_ON;
		if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
		{
			TRACE(_T(">>> Write Fail in Light ON/OFF Button.\n"));
			GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(TRUE);
			return;
		}

		m_nRecText = CFAVListBox::Red;

		Sleep(182);
		if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
		{
			uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
				m_SerialComm.GetCMDData(CMD_RMC));
			ASSERT(uRMCCnt != LB_ERR);
			if (uRMCCnt != LB_ERR)
			{
				// Verify colors
				int nText = m_RMCListBox.GetTextColor(uRMCCnt);
				int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
				//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
				if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
				{
					ASSERT(FALSE);
				}
			}
		}
		else
		{
			TRACE(_T(">>> Read Fail\n"));
		}
		GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_POWER)->SetFocus();
	}
	else{
		m_bBtnPower = TRUE;
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,53,0,2"));
		char chCMD[] = COMMAND_LOF_OFF;
		if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
		{
			TRACE(_T(">>> Write Fail in Light ON/OFF Button.\n"));
			GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(TRUE);
			return;
		}

		m_nRecText = CFAVListBox::Red;

		Sleep(182);
		if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
		{
			uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
				m_SerialComm.GetCMDData(CMD_RMC));
			ASSERT(uRMCCnt != LB_ERR);
			if (uRMCCnt != LB_ERR)
			{
				// Verify colors
				int nText = m_RMCListBox.GetTextColor(uRMCCnt);
				int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
				//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
				if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
				{
					ASSERT(FALSE);
				}
			}
		}
		else
		{
			TRACE(_T(">>> Read Fail\n"));
		}
		GetDlgItem(IDC_RMC_DEMO_POWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RMC_DEMO_POWER)->SetFocus();
	}
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoBulbon()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BULBON)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,54,0,2"));

	char chCMD[] = COMMAND_LU;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in Light UP Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_BULBON)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Yellow;

	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_BULBON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BULBON)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoBulboff()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BULBOFF)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,74,0,2"));

	char chCMD[] = COMMAND_LD;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in Light Down Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_BULBOFF)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Yellow;
	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_BULBOFF)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BULBOFF)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoRon()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_RON)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,73,0,2"));

	//char chCMD[] = COMMAND_RON;
	char chCMD[] = COMMAND_LOF_ON;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in RED ON Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_RON)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Red;
	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_RON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_RON)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoRoff()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_ROFF)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,53,0,2"));

	//char chCMD[] = COMMAND_ROFF;
	char chCMD[] = COMMAND_LOF_OFF;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in RED OFF Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_ROFF)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Red;
	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_ROFF)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_ROFF)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoGon()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_GON)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,62,0,2"));

	char chCMD[] = COMMAND_GON;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in GREEN ON Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_GON)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Green;
	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_GON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_GON)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoBon()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BON)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,52,0,2"));

	char chCMD[] = COMMAND_BON;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in BLUE ON Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_BON)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Blue;

	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_BON)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BON)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoGoff()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_GOFF)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,63,0,2"));

	char chCMD[] = COMMAND_GOFF;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in GREEN OFF Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_GOFF)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Green;
	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_GOFF)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_GOFF)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoBoff()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BOFF)->EnableWindow(FALSE);

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,53,0,2"));

	char chCMD[] = COMMAND_BOFF;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in BLUE OFF Button.\n"));
		GetDlgItem(IDC_RMC_DEMO_BOFF)->EnableWindow(TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Blue;

	Sleep(182);
	if(m_SerialComm.ReadCMD(CMD_RMC, READ_DEFAULT_TIMEOUT))
	{
		uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nRecText, (CFAVListBox::COLOR)m_nBackground, 
			m_SerialComm.GetCMDData(CMD_RMC));
		ASSERT(uRMCCnt != LB_ERR);
		if (uRMCCnt != LB_ERR)
		{
			// Verify colors
			int nText = m_RMCListBox.GetTextColor(uRMCCnt);
			int nBackground = m_RMCListBox.GetBackgroundColor(uRMCCnt);
			//TRACE(_T("from XListBox:  text=%d  background=%d\n"), nText, nBackground);
			if (((nText != m_nText) && (nText != m_nRecText)) || (nBackground != m_nBackground))
			{
				ASSERT(FALSE);
			}
		}
	}
	else
	{
		TRACE(_T(">>> Read Fail\n"));
	}
	GetDlgItem(IDC_RMC_DEMO_BOFF)->EnableWindow(TRUE);
	GetDlgItem(IDC_RMC_DEMO_BOFF)->SetFocus();
}


//===========================================================================
void CFAVRMCDlg::OnBnClickedRmcDemoPair()
//===========================================================================
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_MAPGROUP), FALSE);

	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_RMC_MSG);
	pStatic->SetWindowText(_T("Please waiting..."));

	uRMCCnt = m_RMCListBox.AddLine((CFAVListBox::COLOR)m_nText, (CFAVListBox::COLOR)m_nBackground, _T(">>> M0,73,0,37"));

	char chCMD[] = COMMAND_SN;
	if(m_SerialComm.WriteCMD(CMD_RMC, chCMD, sizeof(chCMD), WRITE_DEFAULT_TIMEOUT))
	{
		TRACE(_T(">>> Write Fail in SN Pairing Button.\n"));
		EnableGroupboxControls(::GetDlgItem(m_hWnd, IDC_RMC_MAPGROUP), TRUE);
		return;
	}

	m_nRecText = CFAVListBox::Red;
	SetTimer(SNPAIRING, 90, NULL);
}

