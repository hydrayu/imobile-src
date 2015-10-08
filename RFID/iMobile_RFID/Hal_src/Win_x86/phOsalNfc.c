
// $Revision: 1.22 $

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <windows.h>
#include <memory.h>

#include <phNfcStatus.h>
#include <phNfcCompId.h>

#include <phOsalNfc.h>
#include <phDbgTrace.h>


/*#define PH_OSALNFC_MESSAGE_BASE WM_USER+0x3FF */   /*  As an example */
#define PHOSALNFC_ERR_STRLEN		(100U)
#define	MAX_PRINT_BUFSIZE			(0x450U)

char phOsalNfc_DbgTraceBuffer[MAX_PRINT_BUFSIZE];

static LONG phOsalNfc_ExceptionFilter( LPEXCEPTION_POINTERS pep ) ;

const char gphOsalNfc_ExceptionDetails[5][PHOSALNFC_ERR_STRLEN]= 
													{	"Exception: Memory Allocation failed",
														"Exception:Invalid precondition"	,									
														"Exception: Internal Error" ,
														 "Exception: Unrecoverable Error in Firmware" ,
														"Exception: DAL Error"
													};


typedef struct phOsalNfc_sOsalHandle
{
   pphOsalNfc_ThreadFunction_t   ThreadFunction;
   void                     * Params;
   HANDLE                     ObjectHandle;
   DWORD                      ThreadId;
} phOsalNfc_sOsalHandle_t;


 void * phOsalNfc_GetMemory(uint32_t size)
{
   void * pMem = (void *) malloc(size);
   
   return pMem;
  
}

 void phOsalNfc_FreeMemory(void * pMem)
{
    if(NULL !=  pMem)
        free(pMem);
}

void phOsalNfc_RaiseException(phOsalNfc_ExceptionType_t eExceptiontype, uint16_t reason)
{
	static BOOL bExceptionenabled=FALSE;
	uint16_t		uExceptiontype=0;
	LPTOP_LEVEL_EXCEPTION_FILTER tst;

	if(TRUE !=bExceptionenabled)
		{
			tst=SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)&phOsalNfc_ExceptionFilter);
			bExceptionenabled=TRUE;
		}
	
	if ((eExceptiontype> phOsalNfc_e_NoMemory)&&(eExceptiontype <=phOsalNfc_e_DALerror))
		{
			switch(eExceptiontype)
			{
				case		phOsalNfc_e_NoMemory : uExceptiontype =0; break;
				case		phOsalNfc_e_PrecondFailed :uExceptiontype =0; break;
				case		phOsalNfc_e_InternalErr :uExceptiontype =EXCEPTION_NONCONTINUABLE; break; 
				case		phOsalNfc_e_UnrecovFirmwareErr:uExceptiontype =EXCEPTION_NONCONTINUABLE; break;
				case		phOsalNfc_e_DALerror : uExceptiontype =EXCEPTION_NONCONTINUABLE; break;
				default:	break; 
			}
			/*phOsalNfc_ExceptionDetails*/
			/*phOsalNfc_DbgTrace("%s  Reason Code:%d\n",gphOsalNfc_ExceptionDetails[eExceptiontype],reason);*/
			if(uExceptiontype ==EXCEPTION_NONCONTINUABLE)
				RaiseException((DWORD)eExceptiontype, EXCEPTION_NONCONTINUABLE, 0, NULL);
			else
				RaiseException((DWORD)eExceptiontype, 0, 0, NULL);

		}
		else
		{
			 /* Invalid unknown exception */

		}

}


/* SetUnhandledExceptionFilter calls the ExceptionFilter function whenever exception occurs */ 
 static LONG phOsalNfc_ExceptionFilter( LPEXCEPTION_POINTERS pep ) 
{	 
	LONG uExceptionCode =0xFF;

	switch(pep->ExceptionRecord->ExceptionCode)
	{
		case phOsalNfc_e_NoMemory:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
			break;

		case phOsalNfc_e_PrecondFailed:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
			break;

		case phOsalNfc_e_InternalErr:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
			break;

		case phOsalNfc_e_UnrecovFirmwareErr:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
			break;

		case phOsalNfc_e_DALerror:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
			break;

		default:
			uExceptionCode= EXCEPTION_CONTINUE_EXECUTION;
	}

	return  uExceptionCode;
}



void phOsalNfc_DbgString(const char *pString)
{
	if(NULL!=pString)
	{
		printf(pString);
	}
	else
	{

	}

}
void phOsalNfc_DbgTrace(uint8_t data[], uint32_t size)

{	uint32_t nIndex=0;
	
	if(size != 0)
	{
		for( nIndex=0 ; nIndex < size; ++nIndex )
		{
			if(0 == nIndex % 10)
			{
				printf("\n\t\t\t");
			}
			printf("%02X ",data[nIndex]);
		}
		printf("\n\t Block Size is :%d\n",size);
	}
	else
	{

	}

#if 0
	int vsp;
	void * mem;
	size_t size=0;
    va_list ap; /*will point to each unnamed argument in turn*/
     char buffer[MAX_PRINT_BUFSIZE];
    if(NULL !=  format)
	{
		(void)memset(buffer, 0x0, sizeof(buffer));
		va_start(ap,format); /* point to first element after fmt*/
		vsp=vsprintf(buffer+strlen(buffer),format, ap);
		va_end(ap); /*cleanup*/ 
		size=strlen(format);
		vsp=printf(buffer);
	}
#endif
	

}

/* Below code is outcommented since it is not requiered for FRI1.1 Step1 */

#ifdef FRI_V1

NFCSTATUS phOsalNfc_PostMsg(uint32_t SourceID, uint32_t DestID, phOsalNfc_Message_t * pMsg)
{

    phOsalNfc_Message_t*				 pMsg_i      = NULL;
    uint16_t									 retvalue=0;
	NFCSTATUS							 Status = NFCSTATUS_SUCCESS;		
    
    if(NULL ==  pMsg)
	{
			Status =PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
	}
	else
	{
		pMsg_i    =   (void*) phOsalNfc_GetMemory(sizeof(phOsalNfc_Message_t));
		if(NULL ==  pMsg_i)
		{
			Status=PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
		}
		else
		{
			*pMsg_i    =   *pMsg;
			retvalue = (uint16_t)PostThreadMessage((DWORD)DestID,
                                 (UINT)(PH_OSALNFC_MESSAGE_BASE),
                                 (WPARAM)pMsg_i,
                                 (LPARAM)SourceID);
			if(0 == retvalue)
			 {
				
				 Status= PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
			 }
			else
			{
				Status= NFCSTATUS_SUCCESS;
			}
		 phOsalNfc_FreeMemory(pMsg_i);
		}

	}
	return Status;

}
 	

NFCSTATUS phOsalNfc_ReceiveMsg(uint32_t *pSourceID, phOsalNfc_Message_t* pMsg)

{
	void * mms;
    MSG         msg;
    uint16_t     ret;

    if(NULL    ==  pMsg ||  NULL    ==  pSourceID)
        return PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);

    ret = (uint16_t)GetMessage(   &msg,
                        NULL,
                        PH_OSALNFC_MESSAGE_BASE,
                        PH_OSALNFC_MESSAGE_BASE);
  
    *pSourceID  =   0;
    (void)memset(pMsg, 0x0, sizeof(*pMsg));

    if (ret == 0   ||  NULL    ==  (void*)msg.wParam)
    {
        return PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
    }

    *pSourceID  =   (uint32_t)(msg.lParam);
    *pMsg    =   *(phOsalNfc_Message_t *)msg.wParam;

    phOsalNfc_FreeMemory((void*)msg.wParam);

    return NFCSTATUS_SUCCESS;
}

uint32_t phOsalNfc_GetTaskId(void)
{
    uint32_t taskID = GetCurrentThreadId();
    return taskID;
}




DWORD WINAPI phOsalNfc_ThreadProcedure(LPVOID lpParameter)
{
    phOsalNfc_sOsalHandle_t* thread_handle = (phOsalNfc_sOsalHandle_t*) lpParameter;

    thread_handle->ThreadFunction(thread_handle->Params);
    return 0;
}

NFCSTATUS phOsalNfc_CreateThread(void                        **hThread, 
                                 pphOsalNfc_ThreadFunction_t     ThreadFunction,
                                 void                         *Param)
{
    NFCSTATUS creation_status = NFCSTATUS_SUCCESS;

    phOsalNfc_sOsalHandle_t *thread_handle = malloc(sizeof(phOsalNfc_sOsalHandle_t));

    if(thread_handle != NULL)
    {
        thread_handle->Params = Param;
        thread_handle->ThreadFunction = ThreadFunction;
        thread_handle->ObjectHandle = 
        CreateThread(NULL, 0, phOsalNfc_ThreadProcedure, thread_handle, 0, &(thread_handle->ThreadId) /*NULL*/);
        if (thread_handle->ObjectHandle != NULL)
        {
            *(phOsalNfc_sOsalHandle_t**) hThread = thread_handle;
        }
        else
        {
            creation_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
            free(thread_handle);
        }
    }
    else
    {
        creation_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
    }

    return creation_status;
}

NFCSTATUS phOsalNfc_CloseHandle(void *hItem)
{
    NFCSTATUS close_status = NFCSTATUS_SUCCESS;
    phOsalNfc_sOsalHandle_t *item_handle = (phOsalNfc_sOsalHandle_t*)hItem;

    if (item_handle != NULL)
    {
        CloseHandle(item_handle->ObjectHandle);
        free(item_handle);
        close_status = NFCSTATUS_SUCCESS;
    }
    else
    {
        close_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }
    return close_status;
}

void phOsalNfc_Suspend(uint32_t WaitingTime)
{
    Sleep(WaitingTime);
}

NFCSTATUS phOsalNfc_CreateSemaphore(void        **hSemaphore,
                                    uint8_t     InitialValue,
                                    uint8_t     MaxValue)
{
    NFCSTATUS creation_status = NFCSTATUS_SUCCESS;

    phOsalNfc_sOsalHandle_t *semaphore_handle = malloc(sizeof(phOsalNfc_sOsalHandle_t));

    if(semaphore_handle != NULL)
    {
        semaphore_handle->Params = NULL;
        semaphore_handle->ThreadFunction = NULL;

        semaphore_handle->ObjectHandle = 
            CreateSemaphore(NULL, InitialValue, MaxValue, NULL);

        if (semaphore_handle->ObjectHandle != NULL)
        {
            *(phOsalNfc_sOsalHandle_t**) hSemaphore = semaphore_handle;
        }
        else
        {
            creation_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_SEMAPHORE_CREATION_ERROR);
            free(semaphore_handle);
        }
    }
    else
    {
        creation_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
    }

    return creation_status;
}

NFCSTATUS phOsalNfc_ProduceSemaphore(void *hSemaphore)
{
    NFCSTATUS release_status;
    BOOL release_succeeded = FALSE;
    phOsalNfc_sOsalHandle_t *semaphore_handle = (phOsalNfc_sOsalHandle_t*) hSemaphore;
    
    if (semaphore_handle != NULL)
    {
        release_succeeded = ReleaseSemaphore(semaphore_handle->ObjectHandle, 1, NULL);

        if (release_succeeded == TRUE)
        {
            release_status = NFCSTATUS_SUCCESS;
        }
        else
        {
            release_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_SEMAPHORE_PRODUCE_ERROR);
        }
    }
    else
    {
        release_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }
    
    return release_status;
}

NFCSTATUS phOsalNfc_ConsumeSemaphore(void *hSemaphore)
{
    return phOsalNfc_ConsumeSemaphoreEx(hSemaphore, INFINITE);
}

NFCSTATUS phOsalNfc_ConsumeSemaphoreEx(void *hSemaphore, uint32_t Timeout)
{
   
    NFCSTATUS consume_status;
    phOsalNfc_sOsalHandle_t *semaphore_handle = (phOsalNfc_sOsalHandle_t*) hSemaphore;
    DWORD semaphore_status;

    if (semaphore_handle != NULL)
    { 
        semaphore_status = WaitForSingleObject(semaphore_handle->ObjectHandle, Timeout);
        if (semaphore_status == WAIT_OBJECT_0)
        {
            consume_status = NFCSTATUS_SUCCESS;
        }
        else
        {
            consume_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_SEMAPHORE_CONSUME_ERROR);
        }   
    }
    else
    {
        consume_status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }    
  
    return consume_status;        
}




void   phOsalNfc_RandInit( void)

{
    #ifndef WINCE	/*	This does not work under Windows CE */
		srand( (unsigned)time( NULL ) );
    #endif
}


uint16_t    phOsalNfc_RandGetNumber( void)

{

    #ifdef WINCE
		return (uint16_t)Random( );
    #else
		return ((uint16_t)rand( ));
    #endif

}


uint32_t    phOsalNfc_GetTicks( )
{
  LARGE_INTEGER ticksPerSecond;
  LARGE_INTEGER tick;   
  uint32_t  nbMicro =   0;

  /* high resolution counter's accuracy */
  QueryPerformanceFrequency(&ticksPerSecond);

  /* Current Counter value */
  QueryPerformanceCounter(&tick);

  nbMicro   =   (uint32_t)(((double)tick.QuadPart / (double)ticksPerSecond.QuadPart) * 1E6);
  return nbMicro ;
}


#endif
