// FAVEPC UtilityDlg.h : header file
//
#if !defined(AFX_XPFAVEPCUTILITYDLG_H__B39AA2CD_691D_51D8_07C8_0802AB2DF10C__INCLUDED_)
#define AFX_XPFAVEPCUTILITYDLG_H__B39AA2CD_691D_51D8_07C8_0802AB2DF10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef MODELDIALOG
#define	MODELDIALOG
#endif

//_____ I N C L U D E S _____________________________________________________
#include ".\FAVEPC\FAVTabCtrl.h"
#include "FAVConnectDlg.h"
#include "FAVReaderDlg.h"
#include "FAVRMCDlg.h"

//_____ D E C L A R A T I O N _______________________________________________
// CFAVEPCUtilityDlg dialog
class CFAVEPCUtilityDlg : public CDialogEx
{
// Construction
public:
	CFAVEPCUtilityDlg(CWnd* pParent = NULL);	// standard constructor

// CFAVEPCUtilityDlg Data
	//{{AFX_DATA(CFAVEPCUtilityDlg)
	enum { IDD = IDD_FAVEPCUTILITY_DIALOG };
	CFAVTabCtrl		m_TabCtrl;
	CUARTDllApp		m_SerialComm;
	CImageList		m_TabImage;

	CFAVConnectDlg	m_ConnectDlg;
	CFAVReaderDlg	m_ReaderDlg;
	CFAVRMCDlg		m_RMCDlg;
#ifdef MODELESSDIALOG
	BOOL			m_bShow;
#endif//MODELESSDIALOG
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void CreateCenterDialog(int nIDC, CDialogEx &rControl) const;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFAVEPCUtilityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTabpage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};

#endif // !defined(AFX_XPFAVEPCUTILITYDLG_H__B39AA2CD_691D_51D8_07C8_0802AB2DF10C__INCLUDED_)