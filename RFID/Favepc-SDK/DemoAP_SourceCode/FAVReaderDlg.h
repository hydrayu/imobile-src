// FAVReaderDlg.h : header file
//
#if !defined(AFX_XPFAVREADERDLG_H__B8A1A201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_)
#define AFX_XPFAVREADERDLG_H__B8A1A201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	SINGLEEPC	1
#define	MULTIEPC	2
//_____ I N C L U D E S _____________________________________________________
#include "FAVConnectDlg.h"
#include ".\FAVEPC\FAVListBox.h"
#include ".\FAVEPC\FAVSenseBtn.h"
#include "FAVLNODlg.h"

//_____ D E C L A R A T I O N _______________________________________________
// CFAVReaderDlg dialog

class CFAVReaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFAVReaderDlg)

public:
	CFAVReaderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFAVReaderDlg();

// CFAVReaderDlg Data
	//{{AFX_DATA(CFAVReaderDlg)
	enum { IDD = IDD_READER_DIALOG };
	BOOL			m_bRunStop;				//Used the EPC repeat run and will turn into TRUE
	UINT			m_nBackground;
	UINT			m_nText;
	UINT			m_nRecText;
	UINT			m_nUseColor;
	CString			m_strRead;
	CString			m_strWrite;
	CString			m_strWriteList;
	CString			m_strLogFile;			//FAVEPC Utility.log
	// For palette
	BOOL			m_bLineNumbers;
	UINT			m_nGutterWidth;
	COLORREF		m_rgbLinenoColor;
	COLORREF		m_rgbGutterColor;

	CToolTipCtrl	m_ToolTips;
	CUARTDllApp		m_SerialComm;

	CFAVSenseBtn	m_ButtonReadEPC;
	CFAVSenseBtn	m_ButtonRead;
	CFAVSenseBtn	m_ButtonWrite;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation	
protected:
	BOOL			m_bLogFile;
	CEdit			m_editEPCData;
	CEdit			m_editReadData;
	CEdit			m_editWriteData;
	int				m_nMemBankIndex;
	CString			m_strAddress;
	CString			m_strWordsLength;
	CFAVListBox		m_ListBox;

	int GetRandomColor(int nColor);
	int CStringHexToInt(CString str);
	void RefreshControl(UINT uCtlID);

	// Generated message map functions
	//{{AFX_MSG(CFAVReaderDlg)
	afx_msg void OnBnClickedReaderReadepc();
	afx_msg void OnBnClickedReaderRepeat();
	afx_msg void OnBnClickedReaderRead();
	afx_msg void OnBnClickedReaderWrite();
	afx_msg void OnBnClickedReaderLogfile();
	afx_msg void OnBnClickedReaderLno();
	afx_msg void OnCbnSelchangeReaderMembankData();
	afx_msg void OnEnChangeReaderWriteData();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
public:
//	afx_msg void OnStnClickedReaderCpiData();
};
#endif // !defined(AFX_XPFAVREADERDLG_H__B8A1A201_711D_41D8_47C8_2802AB2DF10C__INCLUDED_)