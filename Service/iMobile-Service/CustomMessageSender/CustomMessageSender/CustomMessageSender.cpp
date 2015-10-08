// CustomMessageSender.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <time.h>
#include <string>
using namespace std;

#define SERVICE_NAME _T("CustomSvc")
//CUSTOM MESSAGE FOR SERVICE TO LAUNCH AN APP INTO SESSION1
#define SERVICE_CONTROL_CUSTOM_MESSAGE 0x0085

// #define MYLOG
#ifdef MYLOG
FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session

#define DEF_LOG_FILE "C:\\iMobile-CustomMessageSender.log"
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

string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error) {
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr+bufLen);
			
			LocalFree(lpMsgBuf);
			result = error+"("+result+")";
			return result;
		}
	}
	return std::string();
}

int _tmain(int argc, _TCHAR* argv[])
{
	SC_HANDLE hMyService,hSCM;
	BOOL bSuccess;
	SERVICE_STATUS status;
	
	LOG("INFO: calling OpenSCManager()");
	hSCM = OpenSCManager(0,0,SC_MANAGER_CONNECT);
	if(!hSCM) {
		LOG("Error while calling OpenSCManager() failed with error %s", GetLastErrorStdStr());
		return 1;
	}
	else {
		LOG("INFO: calling OpenService()");
		hMyService = OpenService(hSCM,SERVICE_NAME,SERVICE_USER_DEFINED_CONTROL);
		if(!hMyService)	{
			LOG("Error: while calling OpenService() failed with error %s",GetLastErrorStdStr());
		}
		else {
			LOG("INFO: calling ControlService()");
			bSuccess = ControlService(hMyService,SERVICE_CONTROL_CUSTOM_MESSAGE,&status);
			if(!bSuccess) {
				LOG("Error: ControlService() failed with error %s", GetLastErrorStdStr());
			}
			
			CloseServiceHandle(hMyService);
		}
		CloseServiceHandle(hSCM);
	}

	return 0;
}

