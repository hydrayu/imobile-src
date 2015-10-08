#pragma once

//_____ I N C L U D E S _____________________________________________________
#include ".\FAVEPC\FAVColourPicker.h"
#include ".\FAVEPC\EnableGroupboxControls.h"

//_____ D E C L A R A T I O N _______________________________________________
// CFAVLNODlg dialog

class CFAVLNODlg : public CDialog
{
	DECLARE_DYNAMIC(CFAVLNODlg)

public:
	CFAVLNODlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFAVLNODlg();

// Dialog Data
	//{{AFX_DATA(CFAVLNODlg)
	enum { IDD = IDD_LINE_NUMBERS };
	CFAVColourPicker	m_btnGutterColor;
	CFAVColourPicker	m_btnLinenoColor;
	BOOL				m_bLineNumbers;
	//}}AFX_DATA
	int					m_nGutterWidth;
	COLORREF			m_rgbLinenoColor;
	COLORREF			m_rgbGutterColor;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFAVLNODlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFAVLNODlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickedLineNumbers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
