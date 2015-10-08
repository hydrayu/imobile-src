#include "stdafx.h"
#include "ComputerName.h"
#include "Winreg.h"
#include <Sddl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWinApp theApp;

using namespace std;

int getComputerName(LPCTSTR data)
{
	char computerName[255];
	HKEY hKey;
	LONG returnStatus;
	DWORD dwType=REG_SZ;
	DWORD dwSize = 255;
	int ret;

	returnStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName", NULL,
		KEY_ALL_ACCESS, &hKey);
	memset(computerName, 0, 255);
	if (returnStatus == ERROR_SUCCESS) {
		returnStatus = RegQueryValueEx(hKey,
			TEXT("ComputerName"), NULL, &dwType, (LPBYTE)&computerName, &dwSize);
		if (returnStatus == ERROR_SUCCESS) {
			strncpy ((char*)data, computerName, dwSize);
			ret = 0;
		}
		else ret = 1; 
	}
	else ret = 2;
	RegCloseKey(hKey);
	return ret;
}

int setComputerName(LPCTSTR data)
{
    HKEY cmpName, hostName;
    DWORD dwDisposition;
	int ret = 0;

	// activeCmpKey
    if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName"), 
      0, NULL, 0, KEY_WRITE, NULL, &cmpName, &dwDisposition) == ERROR_SUCCESS) 
    {
		long setRes = RegSetValueEx (cmpName, "ComputerName", 0, REG_SZ, (LPBYTE)data, strlen(data)+1);
		if (setRes != ERROR_SUCCESS)
			ret = 1;
	}
    else ret = 2;
	RegCloseKey(cmpName);

	if (ret == 0) {
		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName"), 
		  0, NULL, 0, KEY_WRITE, NULL, &cmpName, &dwDisposition) == ERROR_SUCCESS) 
		{
			long setRes = RegSetValueEx (cmpName, "ComputerName", 0, REG_SZ, (LPBYTE)data, strlen(data)+1);
			if (setRes != ERROR_SUCCESS) ret = 3;
		}
		else ret = 4;
		RegCloseKey(cmpName);

		if (ret == 0) {
			if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters"), 
			  0, NULL, 0, KEY_WRITE, NULL, &hostName, &dwDisposition) == ERROR_SUCCESS) 
			{
				long setRes = RegSetValueEx (hostName, "Hostname", 0, REG_SZ, (LPBYTE)data, strlen(data)+1);
				if (setRes != ERROR_SUCCESS) ret = 5;
			}
			else ret = 6;
        
			RegCloseKey(hostName);
		}
	}
	return ret;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// TODO: code your application's behavior here.
	if (argc == 2) {
		nRetCode = setComputerName((LPCSTR)argv[1]);
		if (nRetCode == 0) 
			printf("Success writing to Registry SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName\\ComputerName.\n");
		else if (nRetCode == 1)
			printf("Error writing to Registry SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName\\ComputerName.\n");
		else if (nRetCode == 2)
			printf ("RegCreateKeyEx(SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName) Error\n");
		else if (nRetCode == 3)
			printf("Error writing to Registry SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName\\ComputerName.\n");
		else if (nRetCode == 4)
			printf ("RegCreateKeyEx(SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName) Error\n");
		else if (nRetCode == 5)
			printf("Error writing to Registry SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Hostname.\n");
		else if (nRetCode == 6)
			printf ("RegCreateKeyEx(SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters) Error\n");
	} 
	else {
		char computerName[255];
		memset(computerName, 0, 255);
		nRetCode = getComputerName(computerName);
		if (nRetCode == 0)
			printf ("Computer Name is %s\n", computerName);
		else if (nRetCode == 1)
			printf("Could not RegQueryValueEx()\n");
	}

	return nRetCode;
}


