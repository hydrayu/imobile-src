// hookDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include <time.h>

#pragma data_seg("Shared")		//these variables will be shared among all processes to which this dll is linked
HHOOK hkKey = NULL;				
HINSTANCE hInstHookDll=NULL;
#pragma data_seg()

#pragma comment(linker,"/section:Shared,rws")

#define MYLOG
#ifdef MYLOG
FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session

#define DEF_LOG_FILE "C:\\iMobile-hookDll.log"
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
		//If simple string
        if ( *p != '%' ) fputc( *p,fp );
        else {
            switch ( *++p )
            {
            /* string */
            case 's': {
                r = va_arg( list, char * );
                fprintf(fp,"%s", r);
                continue;
            }
 
            /* integer */
            case 'd': {
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
    fprintf(fp," [%s][log: %d] ",filename,line);
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}
#else
#define LOG(...) printf(__VA_ARGS__ )
#endif //MYLOG

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		LOG("ul_reason_for_call() == DLL_PROCESS_ATTACH(%d)", DLL_PROCESS_ATTACH);
		hInstHookDll = (HINSTANCE)hModule;		//we initialize our variable with the value that is passed to us
		break;
	default:
		break;
	}
    return TRUE;
}

// LRESULT __declspec(dllexport)__stdcall CALLBACK iMobileHotkey_Press(int nCode,WPARAM wParam, LPARAM lParam)		//this is the hook procedure
LRESULT CALLBACK iMobileHotkey_Press(int nCode,WPARAM wParam, LPARAM lParam)		//this is the hook procedure
{
	LOG("call iMobileHotkey_Press(%d)", nCode);
	//MSG *msg;																//a pointer to hold the MSG structure that is passed as lParam
	//char charCode;															//to hold the character passed in the MSG structure's wParam
	//if(nCode >=0 && nCode == HC_ACTION)										//if nCode is less than 0 or nCode is not HC_ACTION we will call CallNextHookEx
	//{
	//	msg=(MSG *)lParam;													//lParam contains pointer to MSG structure.
	//	//DWORD vkCode = ((KBDLLHOOKSTRUCT *)lParam)->vkCode;
	//	//DWORD scanCode = ((KBDLLHOOKSTRUCT *)lParam)->scanCode;
	//	//DWORD flags = ((KBDLLHOOKSTRUCT *)lParam)->flags;
	//	//if (nCode == HC_ACTION)
	//	//{
	//	//	LOG("vkCode %02X, scanCode %02X, flags %02X", vkCode, scanCode, flags);
	//	//}
	//	if(msg->message==WM_CHAR)											//we handle only WM_CHAR messages
	//	{
	//		charCode = msg->wParam;											//For WM_CHAR message, wParam is the character code of the key pressed
	//		LOG("'%02X'", charCode);
	//	}
	//	else {
	//		LOG("->message is %02X", msg->message);
	//	}
	//}
	return CallNextHookEx(hkKey,nCode,wParam,lParam);						//passing this message to target application
}

//set the hook
// void __stdcall iMobileHotkeyInit()
// void __declspec(dllexport)__stdcall iMobileHotkeyInit()
void __stdcall iMobileHotkeyInit()
{
	if(hkKey == NULL) {
		HWND hwnd = ::GetTopWindow(0);
		DWORD cpid = ::GetCurrentProcessId();
		while (hwnd)
		{
			DWORD pid = 0;
			DWORD dwThreadId = ::GetWindowThreadProcessId (hwnd, &pid);
			if (pid == cpid) {
				LOG("Found dll thread id %d, process id %d / %d", dwThreadId, pid, cpid);
				break;
			}
			hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
		}

		HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		LOG("hwnd = %d, hinst = %d", hwnd, hinst);
		hkKey = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)iMobileHotkey_Press, hinst, 0);
		if (hkKey != NULL)
			LOG("iMobileHotkeyInit() call SetWindowsHookEx(WH_KEYBOARD_LL) hInstHookDll = %d", hinst);
		else LOG("Error on SetWindowsHookExA(WH_KEYBOARD_LL)");
	}
	else
		LOG("Error, there already had hkKey, not need call SetWindowsHookEx()");
}