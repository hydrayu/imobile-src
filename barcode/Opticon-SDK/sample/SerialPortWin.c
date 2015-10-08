#include "SerialPortWin.h"
#include <tchar.h>

static void SetTimeout(SerialPort *serialPort);

OPT_BOOL SerialPort_Create(SerialPort *serialPort)
{
	memset(&serialPort->dcb, 0, sizeof(DCB));
	serialPort->dcb.BaudRate = CBR_9600;
	serialPort->dcb.ByteSize = 8;
	serialPort->dcb.fParity = FALSE;
	serialPort->dcb.Parity = NOPARITY;
	serialPort->dcb.StopBits = ONESTOPBIT;
	serialPort->dcb.fOutxCtsFlow = FALSE;
	serialPort->dcb.fRtsControl = RTS_CONTROL_ENABLE;
	serialPort->dcb.fOutxDsrFlow = FALSE;
	serialPort->dcb.fDtrControl  = DTR_CONTROL_DISABLE;
	serialPort->dcb.fDsrSensitivity = FALSE;
	serialPort->dcb.fTXContinueOnXoff = FALSE;
	serialPort->dcb.fOutX = FALSE;
	serialPort->dcb.fInX = FALSE;

	serialPort->portNumber = 0;
	serialPort->hSerialPort = INVALID_HANDLE_VALUE;
	memset(&(serialPort->readingOverLapped), 0, sizeof(OVERLAPPED));
	serialPort->readingOverLapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	memset(&(serialPort->waitingOverLapped), 0, sizeof(OVERLAPPED));
	serialPort->waitingOverLapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	memset(&(serialPort->writingOverLapped), 0, sizeof(OVERLAPPED));
	serialPort->writingOverLapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	serialPort->optHandle = Opt2D_ImplementSerialPortInterface(
		serialPort,
		SerialPort_IsOpen,
		SerialPort_Read,
		SerialPort_WaitForInput,
		SerialPort_Write,
		SerialPort_Flush,
		SerialPort_Delay,
		SerialPort_SetShotConfig,
		SerialPort_SetDownloadConfig,
		SerialPort_SetUserConfig
	);
	if (Opt2D_GetLastState() != OPT_STATE_SUCCESS)
	{
		return OPT_FALSE;
	}

	return serialPort->optHandle != INVALID_OPT_HANDLE_VALUE ? OPT_TRUE : OPT_FALSE;
}

void SerialPort_Destroy(SerialPort *serialPort)
{
	Opt2D_DestroyHandle(serialPort->optHandle);
	if (Opt2D_GetLastState() != OPT_STATE_SUCCESS)
	{
		return;
	}
	CloseHandle(serialPort->writingOverLapped.hEvent);
	CloseHandle(serialPort->waitingOverLapped.hEvent);
	CloseHandle(serialPort->readingOverLapped.hEvent);
}

OPT_BOOL SerialPort_Open(SerialPort *serialPort, int portNumber)
{
	TCHAR pathName[MAX_PATH];

	if (!SerialPort_IsOpen(serialPort))
	{
		serialPort->portNumber = portNumber;
		_stprintf(pathName, _T("\\\\.\\COM%d"), portNumber);
		serialPort->hSerialPort = CreateFile(
			pathName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL
		);
		if (GetLastError() != ERROR_SUCCESS)
		{
			return OPT_FALSE;
		}

		if (serialPort->hSerialPort != INVALID_HANDLE_VALUE)
		{
			SetCommState(serialPort->hSerialPort, &(serialPort->dcb));
			if (GetLastError() != ERROR_SUCCESS)
			{
				return OPT_FALSE;
			}
			SetTimeout(serialPort);
			if (GetLastError() != ERROR_SUCCESS)
			{
				return OPT_FALSE;
			}
		}

		return OPT_TRUE;
	}

	return OPT_FALSE;
}

void SerialPort_Close(SerialPort *serialPort)
{
	if (SerialPort_IsOpen(serialPort))
	{
		PurgeComm(serialPort->hSerialPort, PURGE_RXCLEAR);
		if (GetLastError() != ERROR_SUCCESS)
		{
			return;
		}
		CloseHandle(serialPort->hSerialPort);
		if (GetLastError() != ERROR_SUCCESS)
		{
			return;
		}
		serialPort->portNumber = 0;
	}
}

int SerialPort_GetPortNumber(SerialPort *serialPort)
{
	return serialPort->portNumber;
}

OPT_HANDLE SerialPort_GetOptHandle(SerialPort *serialPort)
{
	return serialPort->optHandle;
}

void SetTimeout(SerialPort *serialPort)
{
	COMMTIMEOUTS commTimeouts;

	GetCommState(serialPort->hSerialPort, &(serialPort->dcb));
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	memset(&commTimeouts, 0, sizeof(COMMTIMEOUTS));
	SetCommTimeouts(serialPort->hSerialPort, &commTimeouts);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	serialPort->oneSecondSize = min(1024, serialPort->dcb.BaudRate / 96);
	serialPort->oneCharTimeout = max(3200000 / serialPort->dcb.BaudRate, 100);
}

void SerialPort_SetDefault(SerialPort *serialPort)
{
	COMMCONFIG commConfig;
	DWORD size;
	TCHAR deviceName[MAX_PATH];
	
	GetCommConfig(serialPort->hSerialPort, &commConfig, &size);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	if (commConfig.dcb.BaudRate != 9600 &&
		commConfig.dcb.ByteSize != 8 &&
		commConfig.dcb.Parity != 0 &&
		commConfig.dcb.StopBits != 0 &&
		commConfig.dcb.fInX != 0)
	{
		commConfig.dcb.BaudRate = 9600;
		commConfig.dcb.ByteSize = 8;
		commConfig.dcb.Parity = 0;
		commConfig.dcb.StopBits = 0;
		commConfig.dcb.fInX = 0;
		serialPort->dcb = commConfig.dcb;
		SetCommConfig(serialPort->hSerialPort, &commConfig, size);
		if (GetLastError() != ERROR_SUCCESS)
		{
			return;
		}
		SetTimeout(serialPort);
	}
}

void SerialPort_OpenConfigDialog(HWND hParent, SerialPort *serialPort)
{
	COMMCONFIG commConfig;
	DWORD size;
	TCHAR deviceName[MAX_PATH];
	
	GetCommConfig(serialPort->hSerialPort, &commConfig, &size);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}

	_stprintf(deviceName, _T("COM%d"), serialPort->portNumber);
	CommConfigDialog(deviceName, hParent, &commConfig);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	serialPort->dcb = commConfig.dcb;
	SetCommConfig(serialPort->hSerialPort, &commConfig, size);
	if (GetLastError() != ERROR_SUCCESS)
	{
		return;
	}
	SetTimeout(serialPort);
}

OPT_BOOL OPT_CALL SerialPort_IsOpen(SerialPort *serialPort)
{

	return serialPort->hSerialPort != INVALID_HANDLE_VALUE ? OPT_TRUE : OPT_FALSE;
}

OPT_DWORD OPT_CALL SerialPort_Read(SerialPort *serialPort, OPT_CHAR *buffer, OPT_DWORD length)
{
	DWORD inputLength;
	DWORD readLength = 0;
	DWORD result;
	OPT_DWORD sumReadLength = 0;

	if (SerialPort_IsOpen(serialPort))
	{
		while (length > 0)
		{
			inputLength = min(length, serialPort->oneSecondSize);
			ReadFile(serialPort->hSerialPort, buffer, inputLength, &readLength, &(serialPort->readingOverLapped));
			if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_IO_PENDING)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return 0;
			}
			SetLastError(ERROR_SUCCESS);
			result = WaitForSingleObject(serialPort->readingOverLapped.hEvent, 1000);
			if (GetLastError() != ERROR_SUCCESS)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return 0;
			}
			switch (result)
			{
			case WAIT_OBJECT_0:
				GetOverlappedResult(serialPort->hSerialPort, &(serialPort->readingOverLapped), &readLength, FALSE);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return 0;
				}
				break;

			default:
				CancelIo(serialPort->hSerialPort);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return 0;
				}
				PurgeComm(serialPort->hSerialPort, PURGE_RXABORT);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return 0;
				}
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_TIMEOUT);
				return sumReadLength;
			}
			length -= readLength;
			buffer += readLength;
			sumReadLength += readLength;
		}

		Opt2D_SetLastState(OPT_STATE_SUCCESS);
		return sumReadLength;
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
		return 0;
	}
}

OPT_DWORD OPT_CALL SerialPort_WaitForInput(SerialPort *serialPort, OPT_DWORD timeoutMilliSeconds)
{
	DWORD result;
	DWORD eventNum;
	DWORD commError;
	COMSTAT comStat;

	if (SerialPort_IsOpen(serialPort))
	{
		ClearCommError(serialPort->hSerialPort, &commError, &comStat);
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return 0;
		}
		if (comStat.cbInQue > 0)
		{
			return (OPT_DWORD) comStat.cbInQue;
		}
		SetCommMask(serialPort->hSerialPort, EV_RXCHAR);
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return 0;
		}
		do
		{
			WaitCommEvent(serialPort->hSerialPort, &eventNum, &(serialPort->waitingOverLapped));
			if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_IO_PENDING)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return 0;
			}
			SetLastError(ERROR_SUCCESS);
			result = WaitForSingleObject(serialPort->waitingOverLapped.hEvent, (DWORD) timeoutMilliSeconds);
			if (GetLastError() != ERROR_SUCCESS)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return 0;
			}
			switch (result)
			{
			case WAIT_OBJECT_0:
				ClearCommError(serialPort->hSerialPort, &commError, &comStat);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return 0;
				}
				break;

			default:
				PurgeComm(serialPort->hSerialPort, PURGE_RXABORT);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return 0;
				}
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_TIMEOUT);
				return 0;
			}
		} while (comStat.cbInQue == 0);

		Opt2D_SetLastState(OPT_STATE_SUCCESS);
		return (OPT_DWORD) comStat.cbInQue;
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
		return 0;
	}
}

void OPT_CALL SerialPort_Write(SerialPort *serialPort, const OPT_CHAR *buffer, OPT_DWORD length)
{
	DWORD outputLength;
	DWORD writeLength;
	DWORD result;

	if (SerialPort_IsOpen(serialPort))
	{
		while (length > 0)
		{
			outputLength = min((DWORD) length, serialPort->oneSecondSize);
			WriteFile(serialPort->hSerialPort, buffer, outputLength, &writeLength, &(serialPort->writingOverLapped));
			if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_IO_PENDING)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return;
			}
			SetLastError(ERROR_SUCCESS);
			result = WaitForSingleObject(serialPort->writingOverLapped.hEvent, 1000);
			if (GetLastError() != ERROR_SUCCESS)
			{
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
				return;
			}
			switch (result)
			{
			case WAIT_OBJECT_0:
				GetOverlappedResult(serialPort->hSerialPort, &(serialPort->writingOverLapped), &writeLength, FALSE);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return;
				}
				break;

			default:
				CancelIo(serialPort->hSerialPort);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return;
				}
				PurgeComm(serialPort->hSerialPort, PURGE_RXABORT);
				if (GetLastError() != ERROR_SUCCESS)
				{
					Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
					return;
				}
				Opt2D_SetLastState(OPT_STATE_COMMUNICATION_TIMEOUT);
				return;
			}
			length -= writeLength;
			buffer += writeLength;
		}
		
		Opt2D_SetLastState(OPT_STATE_SUCCESS);
		return;
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
		return;
	}
}

void OPT_CALL SerialPort_Delay(SerialPort *serialPort, OPT_DWORD milliSeconds)
{
	Sleep((DWORD) milliSeconds);
}

void OPT_CALL SerialPort_Flush(SerialPort *serialPort)
{
	Opt2D_SetLastState(OPT_STATE_SUCCESS);
}

void OPT_CALL SerialPort_SetShotConfig(SerialPort *serialPort)
{
	DCB shotDCB;

	if (SerialPort_IsOpen(serialPort))
	{
		GetCommState(serialPort->hSerialPort, &(serialPort->dcb));
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return;
		}
		shotDCB = serialPort->dcb;
		shotDCB.BaudRate = CBR_115200;
		shotDCB.ByteSize = 8;
		shotDCB.fParity = FALSE;
		shotDCB.Parity = NOPARITY;
		shotDCB.StopBits = ONESTOPBIT;
		shotDCB.fOutxCtsFlow = FALSE;
		shotDCB.fRtsControl = RTS_CONTROL_ENABLE;
		shotDCB.fOutxDsrFlow = FALSE;
		shotDCB.fDtrControl  = DTR_CONTROL_DISABLE;
		shotDCB.fDsrSensitivity = FALSE;
		shotDCB.fTXContinueOnXoff = FALSE;
		shotDCB.fOutX = FALSE;
		shotDCB.fInX = FALSE;
		SetCommState(serialPort->hSerialPort, &shotDCB);
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return;
		}
		Opt2D_SetLastState(OPT_STATE_SUCCESS);
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
	}
}

void OPT_CALL SerialPort_SetDownloadConfig(SerialPort *serialPort)
{
	DCB downloadDCB;

	if (SerialPort_IsOpen(serialPort))
	{
		GetCommState(serialPort->hSerialPort, &(serialPort->dcb));
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return;
		}
		downloadDCB = serialPort->dcb;
		downloadDCB.BaudRate = CBR_115200;
		downloadDCB.ByteSize = 8;
		downloadDCB.fParity = FALSE;
		downloadDCB.Parity = NOPARITY;
		downloadDCB.StopBits = ONESTOPBIT;
		downloadDCB.fOutxCtsFlow = FALSE;
		downloadDCB.fRtsControl = RTS_CONTROL_ENABLE;
		downloadDCB.fOutxDsrFlow = FALSE;
		downloadDCB.fDtrControl  = DTR_CONTROL_DISABLE;
		downloadDCB.fDsrSensitivity = FALSE;
		downloadDCB.fTXContinueOnXoff = FALSE;
		downloadDCB.fOutX = FALSE;
		downloadDCB.fInX = FALSE;
		SetCommState(serialPort->hSerialPort, &downloadDCB);
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return;
		}
		Opt2D_SetLastState(OPT_STATE_SUCCESS);
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
	}
}

void OPT_CALL SerialPort_SetUserConfig(SerialPort *serialPort)
{
	if (SerialPort_IsOpen(serialPort))
	{
		SetCommState(serialPort->hSerialPort, &(serialPort->dcb));
		if (GetLastError() != ERROR_SUCCESS)
		{
			Opt2D_SetLastState(OPT_STATE_COMMUNICATION_FAILURE);
			return;
		}
		Opt2D_SetLastState(OPT_STATE_SUCCESS);
	}
	else
	{
		Opt2D_SetLastState(OPT_STATE_SERIAL_PORT_IS_DISCONNECTING);
	}
}
