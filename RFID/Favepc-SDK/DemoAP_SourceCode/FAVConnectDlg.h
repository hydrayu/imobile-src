// FAVConnectDlg.h : header file
//
#if !defined(AFX_XPFAVCONNECTDLG_H__B3CBA2A1_6917_50D4_07C8_0802AB2DF10C__INCLUDED_)
#define AFX_XPFAVCONNECTDLG_H__B3CBA2A1_6917_50D4_07C8_0802AB2DF10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	CREATEPORT		1
#define	CREATEOK		2
#define	SENSEBTN
//_____ I N C L U D E S _____________________________________________________
#include "UARTDll.h"			// insert DLL.h
#pragma  comment (lib, "UARTDll.lib")

#ifdef SENSEBTN
#include ".\FAVEPC\FAVSenseBtn.h"
#else
#include ".\FAVEPC\FAVXPBtn.h"
#endif // SenseBtn


//_____ D E C L A R A T I O N _______________________________________________
// CFAVConnectDlg dialog

class CFAVConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFAVConnectDlg)

public:
	CFAVConnectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFAVConnectDlg();

// CFAVConnectDlg Data
	//{{AFX_DATA(CFAVConnectDlg)
	enum { IDD = IDD_CONNECT_DIALOG };
	CUARTDllApp		m_UARTComm;
#ifdef SENSEBTN
	CFAVSenseBtn	m_ButtonConnect;
	CFAVSenseBtn	m_ButtonOK;
#else
	//	CFAVXPBtn		m_ButtonConnect;
	//	CFAVXPBtn		m_ButtonOK;
#endif // SenseBtn
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON		m_hIcon;
	BOOL		m_bGetSerialPort;
	BOOL		m_bCOMSel;

	BOOL COMContrast(CString str);
	DECLARE_MESSAGE_MAP()
public:
	
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedConnectConnectbtn();
	afx_msg void OnBnClickedConnectOkbtn();
	afx_msg void OnBnClickedConnectCheck();
	virtual BOOL OnInitDialog();
};
#endif // !defined(AFX_XPFAVCONNECTDLG_H__B3CBA2A1_6917_50D4_07C8_0802AB2DF10C__INCLUDED_)
