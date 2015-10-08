// FAVRMCDlg.h : header file
//
#if !defined(AFX_XPFAVRMCDLG_H__B8A2B201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_)
#define AFX_XPFAVRMCDLG_H__B8A2B201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//_____ I N C L U D E S _____________________________________________________
#include ".\FAVEPC\FAVSenseBtn.h"
#include ".\FAVEPC\FAVListBox.h"
#include "UARTDll.h"			// insert DLL.h
#pragma  comment (lib, "UARTDll.lib")

#define	SNPAIRING	1000
#define	SNPAIRING_R	1001
#define	RMCBUTTON	1002

//_____ D E C L A R A T I O N _______________________________________________
// CFAVRMCDlg dialog

class CFAVRMCDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFAVRMCDlg)

// Construction
public:
	CFAVRMCDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFAVRMCDlg();

// CFAVRMCDlg Data
	//{{AFX_DATA(CFAVRMCDlg)
	enum { IDD = IDD_RMC_DIALOG };
	CUARTDllApp		m_SerialComm;
	CDC				m_backDC;
	CBitmap			m_backBmp;
	BOOL			m_bBK;
	BOOL			m_bBtnPower;
	CToolTipCtrl	m_ToolTipsRMC;
	UINT			m_nBackground;
	UINT			m_nText;
	UINT			m_nRecText;
	CString			m_strRMCLogFile;	//FAVEPC Utility.log
	CFAVSenseBtn	m_ButtonPair;
	CFAVSenseBtn	m_ButtonPower;
	CFAVSenseBtn	m_ButtonBulbOFF;
	CFAVSenseBtn	m_ButtonBulbON;
	CFAVSenseBtn	m_ButtonBuleON;
	CFAVSenseBtn	m_ButtonBuleOFF;
	CFAVSenseBtn	m_ButtonGreenON;
	CFAVSenseBtn	m_ButtonGreenOFF;
	CFAVSenseBtn	m_ButtonRedON;
	CFAVSenseBtn	m_ButtonRedOFF;


	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:

	BOOL m_bRMCLogFile;
	CFAVListBox m_RMCListBox;
	CListCtrl m_ListCtrl;
	// Generated message map functions
	//{{AFX_MSG(CFAVRMCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRmcSend();
	afx_msg void OnCbnSelchangeRmcCmdData();
	afx_msg void OnBnClickedRmcLogfile();
	afx_msg void OnBnClickedRmcDemoPower();
	afx_msg void OnBnClickedRmcDemoBulbon();
	afx_msg void OnBnClickedRmcDemoBulboff();
	afx_msg void OnBnClickedRmcDemoRon();
	afx_msg void OnBnClickedRmcDemoRoff();
	afx_msg void OnBnClickedRmcDemoGon();
	afx_msg void OnBnClickedRmcDemoBon();
	afx_msg void OnBnClickedRmcDemoGoff();
	afx_msg void OnBnClickedRmcDemoBoff();
	afx_msg void OnBnClickedRmcDemoPair();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	
};
#endif // !defined(AFX_XPFAVRMCDLG_H__B8A2B201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_)