// CustomMessageSender.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define FIX

#ifndef FIX
#include "windows.h"
#include <stdio.h>
#include <string.h>
#endif // FIX

#define SERVICE_NAME _T("CustomSvc")
//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION1
#define SERVICE_CONTROL_CUSTOM_MESSAGE 0x0085

#ifndef FIX
//Name given to the pipe
#define g_szPipeName "\\\\.\\Pipe\\iMobileServicePipe"
//Pipe name format - \\.\pipe\pipename
#define BUFFER_SIZE 1024

HANDLE hPipe;
char szCmd[BUFFER_SIZE];
DWORD cbCmd;
#endif // FIX

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE hMyService,hSCM;
	BOOL bSuccess;
	SERVICE_STATUS status;

#ifndef FIX
	if (argc > 1) {
		char szBuffer[BUFFER_SIZE];
		DWORD cbBytes;
		strcpy_s(szBuffer, argv[1]);

		hPipe = CreateFile(
			_T(g_szPipeName),
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (INVALID_HANDLE_VALUE == hPipe) {
			printf ("\nError while creating Pipe: %d", GetLastError());
		}
		else {
			BOOL bResult = WriteFile(
				hPipe,
				szBuffer,
				strlen(szBuffer)+1,
				&cbBytes,
				NULL);
			if ((!bResult) || (strlen(szBuffer)+1 != cbBytes)) {
				printf ("\nError occurred while writing to the server: %d", GetLastError());
			}
			else {
				printf ("\nSend APP %s to iMobile service.", szBuffer);
#endif //FIX
				hSCM = OpenSCManager(0,0,SC_MANAGER_CONNECT);
				if(!hSCM) {
					printf("Open SCM failed with error %u",GetLastError());
				}
				else {
					hMyService = OpenService(hSCM,SERVICE_NAME,SERVICE_USER_DEFINED_CONTROL);
					if(!hMyService)	{
						printf("Open SCM failed with error %u",GetLastError());
					}
					else {
						bSuccess = ControlService(hMyService,SERVICE_CONTROL_CUSTOM_MESSAGE,&status);
						if(!bSuccess) {
							printf("Control Service failed with error %u",GetLastError());
						}
						
						CloseServiceHandle(hMyService);
					}
					CloseServiceHandle(hSCM);
				}
#ifndef FIX
			}
			CloseHandle(hPipe);
		}
	}
#endif //FIX
	return 0;
}

