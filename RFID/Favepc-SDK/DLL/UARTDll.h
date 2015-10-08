// UARTDll.h : main header file for the UARTDll DLL
//
#if !defined(AFX_UARTDLL_H__1FAFEE50_88D1_11D6_89D7_00010302158B__INCLUDED_)
#define AFX_UARTDLL_H__1FAFEE50_88D1_11D6_89D7_00010302158B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//_____ I N C L U D E S _____________________________________________________
#include "resource.h"		// main symbols
#include "stdafx.h"

//_____ M A C R O S ________________________________________________________
//! @defgroup RS232 definitions
//! @{
#define AMOUNT_TO_READ          512
#define AMOUNT_TO_WRITE			512
#define NUM_READSTAT_HANDLES    3
#define STATUS_CHECK_TIMEOUT    1000
#define	WRITE_DEFAULT_TIMEOUT	120
#define	WRITE_TIMEOUT			240
#define	READ_DEFAULT_TIMEOUT	160
#define	READ_TIMEOUT			640
#define MAX_READ_BUFFER			1024
#define	MSGBUFSIZE				256		

//! @}
//! @defgroup ASCII definitions
//! @{
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#define	ASCII_SPACE		0x20
#define	ASCII_COMMA		0x2C
#define	ASCII_0			0x30
#define	ASCII_1			0x31
#define	ASCII_2			0x32
#define	ASCII_3			0x33
#define	ASCII_4			0x34
#define	ASCII_5			0x35
#define	ASCII_6			0x36
#define	ASCII_7			0x37
#define	ASCII_8			0x38
#define	ASCII_9			0x39

#define	ASCII_M			0x4D
#define	ASCII_Q			0x51
#define	ASCII_S			0x53
#define	ASCII_V			0x56
#define	ASCII_W			0x57
//! @}

//! @defgroup Command definitions
//! @{
//Reader tag command
#define	CMD_V		0x01
#define	CMD_S		0x02
#define	CMD_Q		0x03
#define	CMD_W		0x04
#define	CMD_R		0x05
#define CMD_READER	0xFF		//extra command

//Reader RMC command
#define	CMD_RMC		0x10



#define	COMMAND_V {(char)ASCII_V,(char)ASCII_CR}
#define	COMMAND_S {(char)ASCII_S,(char)ASCII_CR}
#define	COMMAND_Q {(char)ASCII_Q,(char)ASCII_CR}

#define	COMMAND_SN	{(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_7,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_3,(char)ASCII_7,\
	(char)ASCII_CR}
#define	COMMAND_LOF_ON	{(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_7,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_LOF_OFF	{(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_5,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_LU {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_5,(char)ASCII_4,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_LD {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_7,(char)ASCII_4,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_BON {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_5,(char)ASCII_2,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_BOFF {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_5,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_GON {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_6,(char)ASCII_2,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_GOFF {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_6,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_RON {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_7,(char)ASCII_2,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
#define	COMMAND_ROFF {(char)ASCII_M,(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_7,(char)ASCII_3,(char)ASCII_COMMA,\
	(char)ASCII_0,(char)ASCII_COMMA,\
	(char)ASCII_2,\
	(char)ASCII_CR}
//! @}


// CCOMPORTINFO dialog
#ifdef FAV_UARTEXPORTS
	#define UARTDLL  __declspec(dllexport) 
#else
	#define UARTDLL  __declspec(dllimport) 
	#pragma  comment (lib, "UARTDll.lib")
#endif//FAV_UARTEXPORTS

UARTDLL BOOL		m_bGetOverResultWait;		// Wait function type in read file
UARTDLL BOOL		m_bConnectFlag;				// UART connect flag


// CUARTDllApp
// See UARTDll.cpp for the implementation of this class
//_____ D E C L A R A T I O N _______________________________________________
class UARTDLL CUARTDllApp : public CWinApp
{
public:
	CUARTDllApp();

protected:
	// CFAVSerialComm Data
	//{{AFX_DATA(CFAVSerialComm)
	UINT		m_uRegIndex;
	UINT		m_uReaderIndex[16];
	UINT		m_uReaderSearchIndex[16];

	OVERLAPPED	m_osRead;
	DWORD		gdwEvtMask;						// Result from WaitCommEvent
	DWORD		gdwErrorFlags;


	UINT		m_uReaderCount;					// Reader counter to detected
	CString		m_strCMDV;
	CString		m_strCMDS;
	CString		m_strCMDQ;
	CString		m_strCMDR;
	CString		m_strCMDW;
	CString		m_strCMDRMC;
	CString		m_strCMDREADER;
	CString		m_strCOMPORT;
	HKEY		m_hKey; 
	//}}AFX_DATA
// Overrides
public:
	virtual BOOL InitInstance();
	
	
	BOOL		CreateCOMM(CString strCOM);
	BOOL		 ReceiveBuffer(UINT uDisp, char* lpBuf, DWORD dwBufLen);

	UINT		GetSerialPort(BOOL bSel, CArray<CString,CString> & );
	BOOL		COMPortAC(void);
	BOOL		COMPortMC(CString strCOM);
	BOOL		ReadCMD(UINT uDisp, DWORD dwTimeout);
	DWORD		WriteCMD(UINT uCMD, LPCVOID lpBuf, DWORD dwToWrite, DWORD dwTimeout);
	CString		GetCOM(void);
	UINT		GetCOMIndex(UINT index);
	CString		GetCMDData(UINT uCMD);
	DECLARE_MESSAGE_MAP()
	
};

#endif // !defined(AFX_UARTDLL_H__1FAFEE50_88D1_11D6_89D7_00010302158B__INCLUDED_)