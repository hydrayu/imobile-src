//LOG()
#include <stdio.h>
#include <time.h>


#include <windows.h>
#include <tlhelp32.h>

#define MYLOG
#ifdef MYLOG
FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session

#define DEF_LOG_FILE "C:\\iMobile-inject.log"
char log_file[1024] = DEF_LOG_FILE;

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

/*
  �o��۷�ΡA�ܤ֦����U�X�I�z��:
  1. �{���X�M���A²��
  2. ������d��, �N��O�i�H�β{���� Visual Studio 2012 �ӽsĶ
  3. �ܽd�� dll (InjectDll.dll)
  4. �ƹ�W�A�󭫭n���O�A�ϥΤW�۷�²��A���u��
  5. �d�Ҥ� WinMain() ���M�g���O�s�Υ~���� exe, ���O�i�H����O�J Process ��
*/

/***************************************************************************************************/
//	Function: 
//		Inject
//	
//	Parameters:
//		HANDLE hProcess - The handle to the process to inject the DLL into.
//
//		const char* dllname - The name of the DLL to inject into the process.
//		
//		const char* funcname - The name of the function to call once the DLL has been injected.
//
//	Description:
//		This function will inject a DLL into a process and execute an exported function
//		from the DLL to "initialize" it. The function should be in the format shown below,
//		not parameters and no return type. Do not forget to prefix extern "C" if you are in C++
//
//			__declspec(dllexport) void FunctionName(void)
//
//		The function that is called in the injected DLL
//		-MUST- return, the loader waits for the thread to terminate before removing the 
//		allocated space and returning control to the Loader. This method of DLL injection
//		also adds error handling, so the end user knows if something went wrong.
/***************************************************************************************************/

void Inject(HANDLE hProcess, const char* dllname, const char* funcname)
{
//------------------------------------------//
// Function variables.						//
//------------------------------------------//

	// Main DLL we will need to load
	HMODULE kernel32	= NULL;

	// Main functions we will need to import
	FARPROC loadlibrary		= NULL;
	FARPROC getprocaddress	= NULL;
	FARPROC exitprocess		= NULL;
	FARPROC exitthread		= NULL;
	FARPROC freelibraryandexitthread = NULL;

	// The workspace we will build the codecave on locally
	LPBYTE workspace		= NULL;
	DWORD workspaceIndex	= 0;

	// The memory in the process we write to
	LPVOID codecaveAddress	= NULL;
	DWORD dwCodecaveAddress = 0;

	// Strings we have to write into the process
	char injectDllName[MAX_PATH + 1]	= {0};
	char injectFuncName[MAX_PATH + 1]	= {0};
	char injectError0[MAX_PATH + 1]		= {0};
	char injectError1[MAX_PATH + 1]		= {0};
	char injectError2[MAX_PATH + 1]		= {0};
	char user32Name[MAX_PATH + 1]		= {0};
	char msgboxName[MAX_PATH + 1]		= {0};

	// Placeholder addresses to use the strings
	DWORD user32NameAddr	= 0;
	DWORD user32Addr		= 0;
	DWORD msgboxNameAddr	= 0;
	DWORD msgboxAddr		= 0;
	DWORD dllAddr			= 0;
	DWORD dllNameAddr		= 0;
	DWORD funcNameAddr		= 0;
	DWORD error0Addr		= 0;
	DWORD error1Addr		= 0;
	DWORD error2Addr		= 0;

	// Where the codecave execution should begin at
	DWORD codecaveExecAddr = 0;

	// Handle to the thread we create in the process
	HANDLE hThread = NULL;

	// Temp variables
	DWORD dwTmpSize = 0;

	// Old protection on page we are writing to in the process and the bytes written
	DWORD oldProtect	= 0;	
	DWORD bytesRet		= 0;

//------------------------------------------//
// Variable initialization.					//
//------------------------------------------//

	// Get the address of the main DLL
	kernel32	= LoadLibrary("kernel32.dll");

	// Get our functions
	loadlibrary		= GetProcAddress(kernel32,	"LoadLibraryA");
	getprocaddress	= GetProcAddress(kernel32,	"GetProcAddress");
	exitprocess		= GetProcAddress(kernel32,	"ExitProcess");
	exitthread		= GetProcAddress(kernel32,	"ExitThread");
	freelibraryandexitthread = GetProcAddress(kernel32,	"FreeLibraryAndExitThread");

// This section will cause compiler warnings on VS8, 
// you can upgrade the functions or ignore them

	// Build names
	_snprintf(injectDllName, MAX_PATH, "%s", dllname);
	_snprintf(injectFuncName, MAX_PATH, "%s", funcname);
	_snprintf(user32Name, MAX_PATH, "user32.dll");
	_snprintf(msgboxName, MAX_PATH, "MessageBoxA");

	// Build error messages
	_snprintf(injectError0, MAX_PATH, "Error");
	_snprintf(injectError1, MAX_PATH, "Could not load the dll: %s", injectDllName);
	_snprintf(injectError2, MAX_PATH, "Could not load the function: %s", injectFuncName);

	// Create the workspace
	workspace = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024);

	// Allocate space for the codecave in the process
	codecaveAddress = VirtualAllocEx(hProcess, 0, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	dwCodecaveAddress = PtrToUlong(codecaveAddress);

// Note there is no error checking done above for any functions that return a pointer/handle.
// I could have added them, but it'd just add more messiness to the code and not provide any real
// benefit. It's up to you though in your final code if you want it there or not.

//------------------------------------------//
// Data and string writing.					//
//------------------------------------------//

	// Write out the address for the user32 dll address
	user32Addr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = 0;
	memcpy(workspace + workspaceIndex, &dwTmpSize, 4);
	workspaceIndex += 4;

	// Write out the address for the MessageBoxA address
	msgboxAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = 0;
	memcpy(workspace + workspaceIndex, &dwTmpSize, 4);
	workspaceIndex += 4;

	// Write out the address for the injected DLL's module
	dllAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = 0;
	memcpy(workspace + workspaceIndex, &dwTmpSize, 4);
	workspaceIndex += 4;

	// User32 Dll Name
	user32NameAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(user32Name) + 1;
	memcpy(workspace + workspaceIndex, user32Name, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// MessageBoxA name
	msgboxNameAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(msgboxName) + 1;
	memcpy(workspace + workspaceIndex, msgboxName, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Dll Name
	dllNameAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(injectDllName) + 1;
	memcpy(workspace + workspaceIndex, injectDllName, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Function Name
	funcNameAddr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(injectFuncName) + 1;
	memcpy(workspace + workspaceIndex, injectFuncName, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Error Message 1
	error0Addr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(injectError0) + 1;
	memcpy(workspace + workspaceIndex, injectError0, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Error Message 2
	error1Addr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(injectError1) + 1;
	memcpy(workspace + workspaceIndex, injectError1, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Error Message 3
	error2Addr = workspaceIndex + dwCodecaveAddress;
	dwTmpSize = (DWORD)strlen(injectError2) + 1;
	memcpy(workspace + workspaceIndex, injectError2, dwTmpSize);
	workspaceIndex += dwTmpSize;

	// Pad a few INT3s after string data is written for seperation
	workspace[workspaceIndex++] = 0xCC;
	workspace[workspaceIndex++] = 0xCC;
	workspace[workspaceIndex++] = 0xCC;

	// Store where the codecave execution should begin
	codecaveExecAddr = workspaceIndex + dwCodecaveAddress;

// For debugging - infinite loop, attach onto process and step over
	//workspace[workspaceIndex++] = 0xEB;
	//workspace[workspaceIndex++] = 0xFE;

//------------------------------------------//
// User32.dll loading.						//
//------------------------------------------//

// User32 DLL Loading
	// PUSH 0x00000000 - Push the address of the DLL name to use in LoadLibraryA
	workspace[workspaceIndex++] = 0x68;
	memcpy(workspace + workspaceIndex, &user32NameAddr, 4);
	workspaceIndex += 4;

	// MOV EAX, ADDRESS - Move the address of LoadLibraryA into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &loadlibrary, 4);
	workspaceIndex += 4;

	// CALL EAX - Call LoadLibraryA
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;

// MessageBoxA Loading
	// PUSH 0x000000 - Push the address of the function name to load
	workspace[workspaceIndex++] = 0x68;
	memcpy(workspace + workspaceIndex, &msgboxNameAddr, 4);
	workspaceIndex += 4;

	// Push EAX, module to use in GetProcAddress
	workspace[workspaceIndex++] = 0x50;

	// MOV EAX, ADDRESS - Move the address of GetProcAddress into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &getprocaddress, 4);
	workspaceIndex += 4;

	// CALL EAX - Call GetProcAddress
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;

	// MOV [ADDRESS], EAX - Save the address to our variable
	workspace[workspaceIndex++] = 0xA3;
	memcpy(workspace + workspaceIndex, &msgboxAddr, 4);
	workspaceIndex += 4;

//------------------------------------------//
// Injected dll loading.					//
//------------------------------------------//

/*
	// This is the way the following assembly code would look like in C/C++

	// Load the injected DLL into this process
	HMODULE h = LoadLibrary("mydll.dll");
	if(!h)
	{
		MessageBox(0, "Could not load the dll: mydll.dll", "Error", MB_ICONERROR);
		ExitProcess(0);
	}

	// Get the address of the export function
	FARPROC p = GetProcAddress(h, "Initialize");
	if(!p)
	{
		MessageBox(0, "Could not load the function: Initialize", "Error", MB_ICONERROR);
		ExitProcess(0);
	}

	// So we do not need a function pointer interface
	__asm call p

	// Exit the thread so the loader continues
	ExitThread(0);
*/

// DLL Loading
	// PUSH 0x00000000 - Push the address of the DLL name to use in LoadLibraryA
	workspace[workspaceIndex++] = 0x68;
	memcpy(workspace + workspaceIndex, &dllNameAddr, 4);
	workspaceIndex += 4;

	// MOV EAX, ADDRESS - Move the address of LoadLibraryA into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &loadlibrary, 4);
	workspaceIndex += 4;

	// CALL EAX - Call LoadLibraryA
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;

// Error Checking
	// CMP EAX, 0
	workspace[workspaceIndex++] = 0x83;
	workspace[workspaceIndex++] = 0xF8;
	workspace[workspaceIndex++] = 0x00;

// JNZ EIP + 0x1E to skip over eror code
	workspace[workspaceIndex++] = 0x75;
	workspace[workspaceIndex++] = 0x1E;

// Error Code 1
	// MessageBox
		// PUSH 0x10 (MB_ICONHAND)
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x10;

		// PUSH 0x000000 - Push the address of the MessageBox title
		workspace[workspaceIndex++] = 0x68;
		memcpy(workspace + workspaceIndex, &error0Addr, 4);
		workspaceIndex += 4;

		// PUSH 0x000000 - Push the address of the MessageBox message
		workspace[workspaceIndex++] = 0x68;
		memcpy(workspace + workspaceIndex, &error1Addr, 4);
		workspaceIndex += 4;

		// Push 0
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x00;

		// MOV EAX, [ADDRESS] - Move the address of MessageBoxA into EAX
		workspace[workspaceIndex++] = 0xA1;
		memcpy(workspace + workspaceIndex, &msgboxAddr, 4);
		workspaceIndex += 4;

		// CALL EAX - Call MessageBoxA
		workspace[workspaceIndex++] = 0xFF;
		workspace[workspaceIndex++] = 0xD0;

	// ExitProcess
		// Push 0
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x00;

		// MOV EAX, ADDRESS - Move the address of ExitProcess into EAX
		workspace[workspaceIndex++] = 0xB8;
		memcpy(workspace + workspaceIndex, &exitprocess, 4);
		workspaceIndex += 4;

		// CALL EAX - Call MessageBoxA
		workspace[workspaceIndex++] = 0xFF;
		workspace[workspaceIndex++] = 0xD0;

//	Now we have the address of the injected DLL, so save the handle

	// MOV [ADDRESS], EAX - Save the address to our variable
	workspace[workspaceIndex++] = 0xA3;
	memcpy(workspace + workspaceIndex, &dllAddr, 4);
	workspaceIndex += 4;

// Load the initilize function from it
	// PUSH 0x000000 - Push the address of the function name to load
	workspace[workspaceIndex++] = 0x68;
	memcpy(workspace + workspaceIndex, &funcNameAddr, 4);
	workspaceIndex += 4;

	// Push EAX, module to use in GetProcAddress
	workspace[workspaceIndex++] = 0x50;

	// MOV EAX, ADDRESS - Move the address of GetProcAddress into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &getprocaddress, 4);
	workspaceIndex += 4;

	// CALL EAX - Call GetProcAddress
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;

// Error Checking
	// CMP EAX, 0
	workspace[workspaceIndex++] = 0x83;
	workspace[workspaceIndex++] = 0xF8;
	workspace[workspaceIndex++] = 0x00;

// JNZ EIP + 0x1C to skip eror code
	workspace[workspaceIndex++] = 0x75;
	workspace[workspaceIndex++] = 0x1C;

// Error Code 2
	// MessageBox
		// PUSH 0x10 (MB_ICONHAND)
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x10;

		// PUSH 0x000000 - Push the address of the MessageBox title
		workspace[workspaceIndex++] = 0x68;
		memcpy(workspace + workspaceIndex, &error0Addr, 4);
		workspaceIndex += 4;

		// PUSH 0x000000 - Push the address of the MessageBox message
		workspace[workspaceIndex++] = 0x68;
		memcpy(workspace + workspaceIndex, &error2Addr, 4);
		workspaceIndex += 4;

		// Push 0
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x00;

		// MOV EAX, ADDRESS - Move the address of MessageBoxA into EAX
		workspace[workspaceIndex++] = 0xA1;
		memcpy(workspace + workspaceIndex, &msgboxAddr, 4);
		workspaceIndex += 4;

		// CALL EAX - Call MessageBoxA
		workspace[workspaceIndex++] = 0xFF;
		workspace[workspaceIndex++] = 0xD0;

	// ExitProcess
		// Push 0
		workspace[workspaceIndex++] = 0x6A;
		workspace[workspaceIndex++] = 0x00;

		// MOV EAX, ADDRESS - Move the address of ExitProcess into EAX
		workspace[workspaceIndex++] = 0xB8;
		memcpy(workspace + workspaceIndex, &exitprocess, 4);
		workspaceIndex += 4;

//	Now that we have the address of the function, we cam call it, 
// if there was an error, the messagebox would be called as well.

	// CALL EAX - Call ExitProcess -or- the Initialize function
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;

	// If we get here, the Initialize function has been called, 
	// so it's time to close this thread and optionally unload the DLL.

//------------------------------------------//
// Exiting from the injected dll.			//
//------------------------------------------//

// Call ExitThread to leave the DLL loaded
#if 1
	// Push 0 (exit code)
	workspace[workspaceIndex++] = 0x6A;
	workspace[workspaceIndex++] = 0x00;

	// MOV EAX, ADDRESS - Move the address of ExitThread into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &exitthread, 4);
	workspaceIndex += 4;

	// CALL EAX - Call ExitThread
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;
#endif

// Call FreeLibraryAndExitThread to unload DLL
#if 0
	// Push 0 (exit code)
	workspace[workspaceIndex++] = 0x6A;
	workspace[workspaceIndex++] = 0x00;

	// PUSH [0x000000] - Push the address of the DLL module to unload
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0x35;
	memcpy(workspace + workspaceIndex, &dllAddr, 4);
	workspaceIndex += 4;

	// MOV EAX, ADDRESS - Move the address of FreeLibraryAndExitThread into EAX
	workspace[workspaceIndex++] = 0xB8;
	memcpy(workspace + workspaceIndex, &freelibraryandexitthread, 4);
	workspaceIndex += 4;

	// CALL EAX - Call FreeLibraryAndExitThread
	workspace[workspaceIndex++] = 0xFF;
	workspace[workspaceIndex++] = 0xD0;
#endif

//------------------------------------------//
// Code injection and cleanup.				//
//------------------------------------------//

	// Change page protection so we can write executable code
	VirtualProtectEx(hProcess, codecaveAddress, workspaceIndex, PAGE_EXECUTE_READWRITE, &oldProtect);

	// Write out the patch
	WriteProcessMemory(hProcess, codecaveAddress, workspace, workspaceIndex, &bytesRet);

	// Restore page protection
	VirtualProtectEx(hProcess, codecaveAddress, workspaceIndex, oldProtect, &oldProtect);

	// Make sure our changes are written right away
	FlushInstructionCache(hProcess, codecaveAddress, workspaceIndex);

	// Free the workspace memory
	HeapFree(GetProcessHeap(), 0, workspace);

	// Execute the thread now and wait for it to exit, note we execute where the code starts, and not the codecave start
	// (since we wrote strings at the start of the codecave) -- NOTE: void* used for VC6 compatibility instead of UlongToPtr
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((void*)codecaveExecAddr), 0, 0, NULL);
	WaitForSingleObject(hThread, INFINITE); 

	// Free the memory in the process that we allocated
	VirtualFreeEx(hProcess, codecaveAddress, 0, MEM_RELEASE);
	CloseHandle(hThread);
}

#define MODIFY_EXE
#ifdef MODIFY_EXE
// Program entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Structures for creating the process
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	BOOL result = FALSE;

	// Hardcoded just for a demo, you will need to use a game/program of your own to test
	// It is not a good idea to use stuff in system32 due to the DEP enabled on those apps.
	// char* exeString = "\"c:\\windows\\system32\\winlogon.EXE";
	char* exeString = "\"C:\\Program Files (x86)\\IrfanView\\i_view32.exe";
	char* workingDir = "c:\\";

	// Holds where the DLL should be
	char dllPath[MAX_PATH + 1] = {0};

	// Set the static path of where the Inject DLL is, hardcoded just for a demo
	_snprintf(dllPath, MAX_PATH, "c:\\hookDll.dll");

	// Need to set this for the structure
	si.cb = sizeof(STARTUPINFO);

	// Try to load our process
	// ���H���i���ΤU�����I�s����n
	// result = CreateProcess(NULL, exeString, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, workingDir, &si, &pi);
	// ���O�n�t�X�N ResumeThread() ����
	// CREATE_SUSPENDED ���ܡA�Q�I�s�����η|�Q����
	// result = CreateProcess(NULL, exeString, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, workingDir, &si, &pi);
	result = CreateProcess(NULL, exeString, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, workingDir, &si, &pi);
	if(!result)
	{
		// MessageBox(0, "Process could not be loaded!", "Error", MB_ICONERROR);
		LOG("Cannot CreateProcess(%s)", exeString);
		return -1;
	}

	LOG("Inject(%d, %s, \"iMobileHotkeyInit\")", pi.hProcess, dllPath);
	// Inject the DLL, the export function is named 'Initialize'
	Inject(pi.hProcess, dllPath, "iMobileHotkeyInit");

	// Resume process execution
	// ResumeThread(pi.hThread); // �f�t �e�� CreateProcess(CREATE_SUSPENDED) �ϥ�

	// Standard return
	return 0;
}
#else
/*
���U�ܽd���O�p��O�J�@�� dll �� winlogon process ,
����S�O���O�A���O�J���N�O process.....
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	LOG("Will inject dll into winlogon, find winlogon first");
	// Holds where the DLL should be
	char dllPath[MAX_PATH + 1] = {0};
 
	// Set the static path of where the Inject DLL is, hardcoded just for a demo
	_snprintf(dllPath, MAX_PATH, "C:\\hookDll.dll"); 
 
	PTHREAD_START_ROUTINE pfnRemote = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");
	LOG("Please check DLL Path %s", dllPath);

	// find winlogon process
	PROCESSENTRY32 procEntry;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD dwSessionId, winlogonPid;
	dwSessionId = WTSGetActiveConsoleSessionId();
	winlogonPid = -1;

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		LOG("Error on  CreateToolhelp32Snapshot()");
		return 1;
	}
	else LOG("CreateToolhelp32Snapshot() %d, dwSessionId %d", hSnap, dwSessionId);

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		LOG("Error on Process32First()");
		return 2;
	}
	do
	{
		if (_stricmp(procEntry.szExeFile, "winlogon.exe") == 0)
		{
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) &&
				winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				LOG("Found winlogon process id %d", winlogonPid);
				break;
			}
		}
	} while (Process32Next(hSnap, &procEntry));

	if (winlogonPid >= 0) LOG("winlogonPid = %d", winlogonPid);

	HANDLE hProcess = ::OpenProcess(  PROCESS_ALL_ACCESS, false, winlogonPid);
 
	LOG("Inject() dll %s, and call iMobileHotkeyInit()???", dllPath);
	// Inject the DLL, the export function is named 'Initialize'
	Inject(hProcess, dllPath, "iMobileHotkeyInit");		//Note: we inject AFTER the window has been created in order to allow the injected DLL to perform subclassing

	//The injected DLL uses subclassing to allow resizing the window outside bounds of the windows desktop
	//SetWindowPos(hwnd, NULL, -50, -50, 1970, 1330, 20);		//IgnoreZOrder =0x04, DoNotActivate = 0x10
	ResumeThread(hProcess);

	return 0;
}
#endif