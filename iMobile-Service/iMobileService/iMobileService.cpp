/*********************************************************************************			
//Comment added by R.Jaisvar
//Using the basic service from V.Anish and a feature to launch an process into any session in Vista under the local system account  
*********************************************************************************
*/

#include "stdafx.h"
#include "iMobileService.h"
#include "Windows.h"
#include "Winsvc.h"
#include "time.h"
#include <WtsApi32.h>
#include <UserEnv.h>
#include <tlhelp32.h>
#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"UserEnv.lib")

/////////////////////////////////////////////////////////////////////////////
//Define custom message added by Jaisvar
/////////////////////////////////////////////////////////////////////////////

//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION 1 
#define SERVICE_CONTROL_CUSTOM_MESSAGE 0x0085
#define INI_FILE "C:\\iMobileService.ini"

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//Method added by Jaisvar
/////////////////////////////////////////////////////////////////////////////
BOOL LaunchAppIntoDifferentSession(char *exe_path);

/////////////////////////////////////////////////////////////////////////////

SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning=true;
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
BOOL InstallService();
BOOL DeleteService();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

////////////////////////////////////////////////////////////
// GLOBAL VARIABLES

char launcherCmds[10][1024];
int launchers=0;
char triggerCmd[1024];

#define MYLOG
#ifdef MYLOG
FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session

#define DEF_LOG_FILE "C:\\iMobile.log"
char log_file[1024] = DEF_LOG_FILE;

void setLogFile(char *filename) {
  if (strlen(filename) > 0) {
	  memcpy(log_file, filename, strlen(filename)+1);
  }
  else {
	  strcpy(log_file, DEF_LOG_FILE);
  }
}

#define LOG(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )

void log_print(char* filename, int line, char *fmt,...)
{
    va_list         list;
    char            *p, *r;
    int             e;
 
    if(SESSION_TRACKER > 0)
      fp = fopen (log_file,"a");
    else
      fp = fopen (log_file,"w");
	time_t now = time(NULL);
	struct tm *nPtr = localtime(&now);

    fprintf(fp, "%u/%u/%u %u:%u:%u ", 
		nPtr->tm_mon+1,
		nPtr->tm_mday,
		nPtr->tm_year+1990,
		nPtr->tm_hour,
		nPtr->tm_min,
		nPtr->tm_sec);

    va_start( list, fmt );
 
    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p,fp );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );
 
                fprintf(fp,"%s", r);
                continue;
            }
 
            /* integer */
            case 'd':
            {
                e = va_arg( list, int );
 
                fprintf(fp,"%d", e);
                continue;
            }
 
            default:
                fputc( *p, fp );
            }
        }
    }
    va_end( list );
    fprintf(fp," [%s][line: %d] ",filename,line);
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}
#else
#define LOG(...) printf(__VA_ARGS__ )
#endif //MYLOG

void parseIni()
{
	int stage=0;
	int trigs=0;
	char str[1024];
	FILE *fp;

	fp = fopen(INI_FILE, "r");
	if ((FILE*)NULL == fp) {
		LOG("Error while read ini file " INI_FILE);
		return;
	}
	launchers = 0;
	do {
		fgets(str, 1024, fp);
		if (strlen(str) < 10) return; // impossible file path there
		for (int i=strlen(str)-1; i>0; i--) {
			if (str[i] == '\r' || str[i] == '\n')
				str[i] = '\0';
		}
		if (str[0] == '-' && str[1] ==' ') {
			if (strcmp(str, "- trigger") == 0) {
				stage = 1; // read trigger
			}
			else if (strcmp(str, "- launcher") == 0) {
				stage = 2; // read launcher
			}
		}
		else {
			if (stage == 1) { // trigger command
				memcpy_s(triggerCmd, 1024, str, strlen(str));
			}
			else if (stage == 2 && launchers < 10) { // launcher command
				memcpy_s(launcherCmds[launchers], 1024, str, strlen(str));
				launchers++;
			}
		}
	} while (!feof(fp));
	fclose(fp);
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  if(argc>1)
  {
    if(strcmp(argv[1],"-i")==0)
    {
      if(InstallService())
        printf("\n\nService Installed Sucessfully\n");
      else
        printf("\n\nError Installing Service\n");
    }
    else if(strcmp(argv[1],"-d")==0)
    {
      if(DeleteService())
        printf("\n\nService UnInstalled Sucessfully\n");
      else
        printf("\n\nError UnInstalling Service\n");
    }
    else
    {
      printf("\n\nUnknown Switch Usage\n\nFor Install use iMobileService -i\n\nFor UnInstall use iMobileService -d\n");
    }
  }
  else
  {
    SERVICE_TABLE_ENTRY DispatchTable[]=
                {{"CustomSvc",ServiceMain},{NULL,NULL}};
    StartServiceCtrlDispatcher(DispatchTable);
  }
  return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
  m_ServiceStatus.dwServiceType = SERVICE_WIN32;
  m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
  m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
  m_ServiceStatus.dwWin32ExitCode = 0;
  m_ServiceStatus.dwServiceSpecificExitCode = 0;
  m_ServiceStatus.dwCheckPoint = 0;
  m_ServiceStatus.dwWaitHint = 0;

  m_ServiceStatusHandle = RegisterServiceCtrlHandler("CustomSvc", 
                                            ServiceCtrlHandler); 
  if (m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
  {
	  LOG("m_ServiceStatusHandle == 0");
	  return;
  }
  m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
  m_ServiceStatus.dwCheckPoint = 0;
  m_ServiceStatus.dwWaitHint = 0;
  if (!SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus)) {
	  LOG("!SetServiceStatus()");
  }
  parseIni();
  for (int i=0; i<launchers; i++) {
	  LaunchAppIntoDifferentSession(launcherCmds[i]);
  }
  bRunning=true;
  while(bRunning)
  {
    Sleep(3000);
  }
  return;
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
  switch(Opcode)
  {
	  ////////////////////////////////////////////////////////////////////////////////////
	  //Added By Jaisvar on 04/11/07 to recieve a custom message from a user app
	  
  case SERVICE_CONTROL_CUSTOM_MESSAGE:
	  parseIni(); // parse everytime...
	  LaunchAppIntoDifferentSession(triggerCmd); 
	  break; 
	case SERVICE_CONTROL_PAUSE: 
      m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
      break;
    case SERVICE_CONTROL_CONTINUE:
      m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
      break;
    case SERVICE_CONTROL_STOP:
      m_ServiceStatus.dwWin32ExitCode = 0;
      m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
      m_ServiceStatus.dwCheckPoint = 0;
      m_ServiceStatus.dwWaitHint = 0;

      SetServiceStatus (m_ServiceStatusHandle,&m_ServiceStatus);
      bRunning=false;
      break;
    case SERVICE_CONTROL_INTERROGATE:
      break; 
  }
  return;
}

BOOL InstallService()
{
  char strDir[1024];
  SC_HANDLE schSCManager,schService;
  GetCurrentDirectory(1024,strDir);
  strcat(strDir,"\\iMobileService.exe"); 
  schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

  if (schSCManager == NULL) 
    return false;
  LPCTSTR lpszBinaryPathName=strDir;

  schService = CreateService(schSCManager,"CustomSvc", 
        "iMobileService", // service name to display
     SERVICE_ALL_ACCESS, // desired access 
     SERVICE_WIN32_OWN_PROCESS, // service type 
     SERVICE_AUTO_START, // start type 
     SERVICE_ERROR_NORMAL, // error control type 
     lpszBinaryPathName, // service's binary 
     NULL, // no load ordering group 
     NULL, // no tag identifier 
     NULL, // no dependencies
     NULL, // LocalSystem account
     NULL); // no password

  if (schService == NULL)
    return false; 

  ::CloseServiceHandle(schService);
  return true;
}

BOOL DeleteService()
{
  SC_HANDLE schSCManager;
  SC_HANDLE hService;
  schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

  if (schSCManager == NULL)
    return false;
  hService=OpenService(schSCManager,"CustomSvc",SERVICE_ALL_ACCESS);
  if (hService == NULL)
    return false;
  if(DeleteService(hService)==0)
    return false;
  if(CloseServiceHandle(hService)==0)
    return false;

return true;
}

////////////////////////////////////////////////////////////////////////////////////
//Added By Jaisvar on 04/11/07 to Launch the process into a different session

/////////////////////////////////////////////////////////////////////////  
BOOL LaunchAppIntoDifferentSession(char *exe_path) 
{
   PROCESS_INFORMATION pi;
   STARTUPINFO si;
   BOOL bResult = FALSE;
   DWORD dwSessionId,winlogonPid;
   HANDLE hUserToken,hUserTokenDup,hPToken,hProcess;
   DWORD dwCreationFlags;

   if (NULL == exe_path || strlen(exe_path) < 10) {
	   LOG("Error while calling LaunchAppIntoDifferentSession(%s)", exe_path);
	   return 1;
   }

// Log the client on to the local computer.

   dwSessionId = WTSGetActiveConsoleSessionId();

//////////////////////////////////////////
   // Find the winlogon process
////////////////////////////////////////

   PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return 1 ;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		return 1 ;
	}

	do
	{
		if (_stricmp(procEntry.szExeFile, "winlogon.exe") == 0)
		{
			// We found a winlogon process...make sure it's running in the console session
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

////////////////////////////////////////////////////////////////////////

   WTSQueryUserToken(dwSessionId,&hUserToken);
   dwCreationFlags = NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE;
   ZeroMemory(&si, sizeof(STARTUPINFO));
   si.cb= sizeof(STARTUPINFO);
   si.lpDesktop = "winsta0\\default";
   ZeroMemory(&pi, sizeof(pi));
   TOKEN_PRIVILEGES tp;
   LUID luid;
   hProcess = OpenProcess(MAXIMUM_ALLOWED,FALSE,winlogonPid);

   if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY
									|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID
									|TOKEN_READ|TOKEN_WRITE,&hPToken))
   {
			   int abcd = GetLastError();
			   LOG("Process token open Error: %u\n",GetLastError()); 
   }

   if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))
   {
	   LOG("Lookup Privilege value Error: %u\n",GetLastError());
   }
   tp.PrivilegeCount =1;
   tp.Privileges[0].Luid =luid;
   tp.Privileges[0].Attributes =SE_PRIVILEGE_ENABLED;

   DuplicateTokenEx(hPToken,MAXIMUM_ALLOWED,NULL,SecurityIdentification,TokenPrimary,&hUserTokenDup);
   int dup = GetLastError();

   //Adjust Token privilege
   SetTokenInformation(hUserTokenDup,TokenSessionId,(void*)dwSessionId,sizeof(DWORD));

   if (!AdjustTokenPrivileges(hUserTokenDup,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES)NULL,NULL))
   {
	   int abc =GetLastError();
	   LOG("Adjust Privilege value Error: %u\n",GetLastError());
   }

   if (GetLastError()== ERROR_NOT_ALL_ASSIGNED)
   {
	 LOG("Token does not have the provilege\n");
   }

   LPVOID pEnv =NULL;

   if(CreateEnvironmentBlock(&pEnv,hUserTokenDup,TRUE))
   {
	   dwCreationFlags|=CREATE_UNICODE_ENVIRONMENT;
   }
   else
	  pEnv=NULL;

// Launch the process in the client's logon session.

   bResult = CreateProcessAsUser(
      hUserTokenDup,            // client's access token
	  _T(exe_path),
      NULL,     // command line
      NULL,              // pointer to process SECURITY_ATTRIBUTES
      NULL,              // pointer to thread SECURITY_ATTRIBUTES
      FALSE,             // handles are not inheritable
      dwCreationFlags,  // creation flags
      pEnv,              // pointer to new environment block 
      NULL,              // name of current directory 
      &si,               // pointer to STARTUPINFO structure
      &pi                // receives information about new process
   );
// End impersonation of client.

//GetLastError Shud be 0

   int iResultOfCreateProcessAsUser = GetLastError();

//Perform All the Close Handles task

  CloseHandle(hProcess);
  CloseHandle(hUserToken);
  CloseHandle(hUserTokenDup);
  CloseHandle(hPToken);
	
 return 0;
}
