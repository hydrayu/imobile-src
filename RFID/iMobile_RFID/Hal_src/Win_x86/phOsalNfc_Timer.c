
// $Revision: 1.25 $

/* ---------------------------Include files ------------------------------------*/
#include<stdio.h>
#include<windows.h>
#include <phOsalNfc_Timer.h>
#include<phOsalNfc.h>
#include <nfc_osal_deferred_call.h>
#include <mmsystem.h>


/* ------------------------------- Macros ------------------------------------*/

#define BASE_ADDRESS               (100)	/* defines the base address for generating timerid */ 
#define TimerRes							(5)	/* OSAL Generic timer resolution */
#define MAX_NO_TIMERS			    (5)	/*Maximum number of timers supported */



/* ------------------------------- Global Declrations ------------------------------------*/

/* Generic timer callback */

typedef  void (CALLBACK *pphOsalNfc_Timer_Cb_t)(	UINT uID, 
																		UINT uMsg, 
																		DWORD dwUser,
																		DWORD dw1,DWORD dw2
																		); 


static DWORD					gCallerThreadID=0xFFFF;
phOsalNfc_Timer_Msg_t			gphOsalNfc_Timer_Msg={0};
phOsalNfc_Message_t				gphOsalNfc_Msg ={0};

typedef enum eTimerStates /* Timer states enum */
{
	eTimerIdle = 0,
	eTimerRunning = 1,
	eTimerStopped = 2

}phOsalNfc_eTimerStates;

static uint8_t gphOsalNfc_Timer_Flag = 1;
/* --------------------Structures and enumerations --------------------------*/


/*structure for osaltimerlist being used by the application */
typedef struct phOsalNfc_Timer
{
   
   uint32_t						RegTimeCnt;						/*Registered time elapse in milliseconds */
   uint32_t						Timercnt;							/* Timercnt is used for internal count  */   
   ppCallBck_t					Application_callback;				/* Application defined callback */
   phOsalNfc_eTimerStates	eState;								/*Timer states */
   BOOL							bInUse;							 /*To check whether the timer is in use or not */
    
}phOsalNfc_Timer_t;


/* This structure is for generic timer */
typedef struct phOsalNfc_Genrictimer
{
	DWORD							dwTimer_id;	 /* ID of a timer  */
	pphOsalNfc_Timer_Cb_t		    pCallBack;		 /* generic timer callback  */		
	phOsalNfc_eTimerStates	        eState;			 /* state of a generic timer */
	uint8_t							uNoTimers;		 /* Number of timers which are alreday started */

}phOsalNfc_GeneralTimer;


/* generic timer callback */
static void CALLBACK phOsalNfc_GeneralTimerCallBck(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2);
static phOsalNfc_Timer_t		OsalTimerList[MAX_NO_TIMERS]={0};
static phOsalNfc_GeneralTimer	Generictimer={ 0xFFFF,phOsalNfc_GeneralTimerCallBck,eTimerIdle,0};
phOsalNfc_Message_t				PostOsalMsgOsal={0};
static void phOsalNfc_DeferredCall (void	*params);

nfc_osal_def_call_msg_t              osal_defer_call = {0};

#ifdef NXP_MESSAGING
phOsalNfc_DeferedCalldInfo_t		OsalDeferedCalldInfo={0};
#endif

/* Generic timer callback which is called when the registered time in multimedia timer expires */
 
static void CALLBACK phOsalNfc_GeneralTimerCallBck(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	DWORD id[MAX_NO_TIMERS];
	uint32_t cbindex = 0;

	
	/* check the entire osaltimerlist and hence notify on the expiration of timers */

	for(cbindex = 0; cbindex < MAX_NO_TIMERS ; cbindex++)
	{
		/* if the timer exists and its state is running and registered time is greater than zero
		and flag is 1 to make sure that timer has not run yet and having valid timerid */ 
		if((TRUE== OsalTimerList[cbindex].bInUse ) && 
			(Generictimer.eState == eTimerRunning) /*&& 
			(cbindex < MAX_NO_TIMERS)*/ || (gphOsalNfc_Timer_Flag == 1)
			&& (OsalTimerList[cbindex].RegTimeCnt))
			{		

				/*generic timerid for all the timers remains same since the multimedia 
				timer is started once */
				
				id[cbindex] = Generictimer.dwTimer_id;
				
				if(OsalTimerList[cbindex].Timercnt > 0)
				{
					/* Update each registered timers  count */
					/*while( OsalTimerList[cbindex].Timercnt > 0)*/
					{
						OsalTimerList[cbindex].Timercnt = OsalTimerList[cbindex].Timercnt - TimerRes;
						
					}
					/* once the timercnt becomes zero ,call the application defined callback with 
					the cbindex as its input parameter depicting the timerid = cbindex + baseaddress + 1 */
					if((OsalTimerList[cbindex].Timercnt <= 0)
						&& (NULL != OsalTimerList[cbindex].Application_callback) && 
						(eTimerStopped != OsalTimerList[cbindex].eState))
					{
						/*Message Handling*/
						gphOsalNfc_Msg.eMsgType = PH_OSALNFC_TIMER_MSG;
						gphOsalNfc_Msg.pMsgData =  (void *)&gphOsalNfc_Timer_Msg;
						/*OsalTimerList[cbindex].Application_callback(cbindex + BASE_ADDRESS + 1);*/
						gphOsalNfc_Timer_Msg.TimerId = (cbindex + BASE_ADDRESS + 1);
						gphOsalNfc_Timer_Msg.pCallBck =OsalTimerList[cbindex].Application_callback;
						osal_defer_call.params = (void *)&gphOsalNfc_Timer_Msg;
						nfc_osal_deferred_call(phOsalNfc_DeferredCall, (void *)&osal_defer_call);
						
						/*reinitialise Timercnt with the Registered Time */
						OsalTimerList[cbindex].Timercnt = OsalTimerList[cbindex].RegTimeCnt;



					}
					gphOsalNfc_Timer_Flag = 0;
				}
			}
	}
			
}

/* To create Timer ,OsalTimerList is initialised to its default values */

uint32_t phOsalNfc_Timer_Create(void)
{	
	uint16_t id=0xFFFF;
	uint8_t Index = 0;
	BOOL FreeTimerFound=FALSE,
		exit_for = FALSE;
	gCallerThreadID=GetCurrentThreadId();
	for(Index = 0; ((Index < MAX_NO_TIMERS) && (FALSE == exit_for)); Index++) 
	{
	/* check whether the timer is free 	If free then only it is created */
		if((FALSE==OsalTimerList[Index].bInUse) 
				&& (FreeTimerFound==FALSE)
				/*&& (Index < MAX_NO_TIMERS)*/)
		{
			id = BASE_ADDRESS + Index + 1;
	
			/* while creating timer mark it as in use and initialise other  members to default values */
			OsalTimerList[Index].bInUse = TRUE; 
			OsalTimerList[Index].Application_callback = 0;
			OsalTimerList[Index].RegTimeCnt = 0;
			OsalTimerList[Index].Timercnt = 0;
			OsalTimerList[Index].eState = eTimerIdle;
			FreeTimerFound=TRUE;
			exit_for = TRUE;
			
		}
		
	}
	
	/*if timer to be created is not available ,then return timer is not available*/

	if(Index == MAX_NO_TIMERS )
	{
		id = BASE_ADDRESS + Index + 1;
		FreeTimerFound=FALSE;
		return id;
	}

	return id;
			
}


/* This starts the timer */ 
void phOsalNfc_Timer_Start(uint32_t TimerId,
						  uint32_t  RegTimeCnt, 
						  ppCallBck_t   Application_callback/*phOsalNfc_TimerApplication_callback*/)
{
	MMRESULT tbp;
	uint32_t	uIndex=0;
	uint8_t Index = 0;
	
	uIndex = TimerId - BASE_ADDRESS - 1;


	/* to keep the count of timers which are running and when it becomes 0 then 
	multimedia timer can be deleted */ 
	/*Generictimer.uNoTimers = 0;*/
	
	
	for(Index = 0; (Index < MAX_NO_TIMERS) ; Index++)
	{
		/*validate the input parameters first and the state of the timer needs to be checked */
		if((Application_callback != NULL) && (TRUE==OsalTimerList[uIndex].bInUse ) 
											&& (TimerId >= BASE_ADDRESS) 
											&& (TimerId <= (BASE_ADDRESS+MAX_NO_TIMERS))
											&& ((OsalTimerList[uIndex].eState == eTimerIdle)
											/*||(OsalTimerList[uIndex].eState == eTimerRunning)*/
											||(OsalTimerList[uIndex].eState == eTimerStopped )) 
											&& (RegTimeCnt != 0)) 
		{
		/* Update Timer List with specific requested timer details */
			OsalTimerList[uIndex].Application_callback = Application_callback;
			OsalTimerList[uIndex].RegTimeCnt = RegTimeCnt;
			OsalTimerList[uIndex].Timercnt = RegTimeCnt; 
			OsalTimerList[uIndex].eState = eTimerRunning;
			++Generictimer.uNoTimers;
			
			/*
			||Generic Timer is used here to start the timer with the specified time in RegTimeCnt 
			||and upon the expiration of the time phOsalNfc_GeneralTimerCallBck gets called ,
			||the multimedia timer has to be started once.
			*/
			if(eTimerIdle == Generictimer.eState)
			{

				Generictimer.pCallBack = phOsalNfc_GeneralTimerCallBck;
				tbp=timeBeginPeriod(1);
				Generictimer.eState = eTimerRunning;
				
				Generictimer.dwTimer_id = (DWORD)timeSetEvent(TimerRes, 0,Generictimer.pCallBack , 0,/*TIME_ONESHOT*/TIME_PERIODIC);
				

		
			}
				
		}

		/*if input parameters are not valid ,then timer can't be started */
		else
		{
			
			gphOsalNfc_Timer_Flag = 0;
		}
	}
	
	if(Generictimer.uNoTimers == MAX_NO_TIMERS)
	{
		Generictimer.uNoTimers--;
	}
}	


void phOsalNfc_Timer_Stop(uint32_t TimerId)
{
	uint32_t Index = 0;
	
	BOOL TimerStopped=FALSE;
	/* to get the index of the osaltimerlist which is extracted through 
	TimerId which is required for further comparisons*/

	Index = TimerId - BASE_ADDRESS - 1;	

	/*TimerId should lie between BASE_ADDRESS and BASE_ADDRESS+MAX_NO_TIMERS */
	if((TimerId > BASE_ADDRESS) && (TimerId <= BASE_ADDRESS+MAX_NO_TIMERS))
	{
		/* To check whether the timer is created already and not stopped  */
			if((TRUE==OsalTimerList[Index].bInUse) 
			
			&& (Index < MAX_NO_TIMERS))
		{
			/* if timer is already running ,then stop it*/
			if(eTimerRunning == OsalTimerList[Index].eState)          
			{
				OsalTimerList[Index].eState =  eTimerStopped;
				OsalTimerList[Index].Application_callback = NULL;
				TimerStopped = TRUE;
				
			}

			/*if timer is in idle state. then stop it */
			else if(eTimerIdle == OsalTimerList[Index].eState)
			{
		
				TimerStopped = FALSE;
				
			}
			else 
			{
			
			}
			
		}
		
		/*if timer is not created ,then it cannot be stopped */
		
			if((Index<MAX_NO_TIMERS)&&(FALSE==OsalTimerList[Index].bInUse))
			{
				/* phOsalNfc_DbgString("Timer with id %d cannot be stopped since it is not created \n",TimerId);*/
				phOsalNfc_DbgString("Timer cannot be stopped since it is not created \n");
			}
		
	}
	
	/* if timer is not available, then it cannot be stopped */
	if(TimerId >= (BASE_ADDRESS+MAX_NO_TIMERS))
	{
		TimerStopped = FALSE;
		
	}
	
	
}
	

void phOsalNfc_Timer_Delete(uint32_t TimerId)
{
	MMRESULT tke,tep;
	uint32_t Index = 0;
	BOOL TimerDeleted = FALSE;
	
	/*
	||To get the index of the osaltimerlist which is extracted through TimerId which 
	||is required for further comparisons
	*/

	Index = TimerId - BASE_ADDRESS - 1;

	/*TimerId should lie between BASE_ADDRESS and BASE_ADDRESS+MAX_NO_TIMERS */
	
	if((TimerId > BASE_ADDRESS) && (TimerId <= BASE_ADDRESS+MAX_NO_TIMERS))
	{
		/* To check whether the timer is already deleted */
		if(/*(FALSE == TimerDeleted)
			&& */(Index < MAX_NO_TIMERS))
		{
			/* states need to be checked, delete operation can be performed only if 
			||	If timer is created
			||	Timer is running
			||	timer is stopped 
			*/

			if(((OsalTimerList[Index].eState == eTimerIdle) ||	(OsalTimerList[Index].eState == eTimerRunning)
				|| (OsalTimerList[Index].eState == eTimerStopped)))
			{
					if((0==Generictimer.uNoTimers)&&(eTimerRunning==Generictimer.eState))
					{
						
						Generictimer.eState=eTimerStopped;
						tke=timeKillEvent(Generictimer.dwTimer_id);
						tep=timeEndPeriod(1);
						/*Reinitialise Generic Timer structure once again */
						Generictimer.pCallBack= NULL;
						Generictimer.dwTimer_id=0xFFFF;
						Generictimer.eState=eTimerIdle;
						Generictimer.uNoTimers=0;
						
					}

					/*if Generictimer.uNoTimers != 0 then delete other timers */
					else
					{
						
						OsalTimerList[Index].bInUse = FALSE;
						TimerDeleted = /*1*/TRUE;
						OsalTimerList[Index].Application_callback = 0;
						OsalTimerList[Index].RegTimeCnt = 0;
						OsalTimerList[Index].Timercnt = 0;
						OsalTimerList[Index].eState = eTimerIdle;
						--Generictimer.uNoTimers;
					}
									
			}
			
		}

	}

}



static
void 
phOsalNfc_DeferredCall (void	*params) 
{
    phOsalNfc_Timer_Msg_t	*TimerMsg = NULL;
	nfc_osal_def_call_msg_t	*OsalDefMsg = NULL;
 
    OsalDefMsg = (nfc_osal_def_call_msg_t*) (params);
	if(OsalDefMsg!=NULL)
	{
        TimerMsg = (phOsalNfc_Timer_Msg_t*)(OsalDefMsg->params);
    }
    
    if((TimerMsg!=NULL)&&(NULL != TimerMsg->pCallBck))
	{
		TimerMsg->pCallBck(TimerMsg->TimerId); 
    }
}


#ifdef NXP_MESSAGING

void nfc_osal_deferred_call(nfc_osal_def_call_t func, void *param)
{
    uint16_t							retvalue = 0;
	
    OsalDeferedCalldInfo.pDeferedCall=func;
    OsalDeferedCalldInfo.pParam=param;

    PostOsalMsgOsal.eMsgType = PH_OSALNFC_TIMER_MSG;
    PostOsalMsgOsal.pMsgData = (void*)&OsalDeferedCalldInfo;

    retvalue = (uint16_t)PostThreadMessage((DWORD)gCallerThreadID,
                                          (UINT)(PH_OSALNFC_MESSAGE_BASE),
                                          (WPARAM)&PostOsalMsgOsal,
                                          (LPARAM)gCallerThreadID);




}

#endif






/* Below code is timer implementation for FRI 1.0. This code is disabled for FRI1.1.	*/

#if 0

#include <stdlib.h>
#include <Windows.h>
#include <assert.h>
#include <phNfcTypes.h>
#include <phOsalNfc.h>
#include <phOsalNfc_Timer.h>
#include <phNfcStatus.h>
#include <phNfcCompId.h>

/**
 * \ingroup grp_osal_nfc_seq
 *
 * Example of implementation of Phase Timer for x86 platform.
 *
 */

typedef struct phOsalNfc_TimerX86Nt
{
    void *                          hThread;
    void *                          hAbort;
    void *                          hStartCount;
    void *                          hResourcesFreed;
    void *                          semaphore;
    uint8_t                         cbCalled;
    uint8_t                         dontCallCB;
    uint32_t                        countDown;
    uint8_t                         freeResources;
} phOsalNfc_TimerX86Nt_t;

/*  This function will allow the user to easily initialize a Phase Timer.
    Here, we must initialize all needed members: function pointers, state, etc...
    Please note that the current API of this function has still some missing parameters (function pointers...)
    that should be added later on, during the implementation.
*/
NFCSTATUS phOsalNfc_Timer_Initialize(       phOsalNfc_Timer_t                     *OsalTimer, 
                                            pphOsalNfc_Timer_StartCountDown_t     StartCountDown,
                                            pphOsalNfc_Timer_ResetCountDown_t     ResetCountDown,
                                            void                                  *IntegrationContext)
{
    NFCSTATUS   status  =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);

    if(     (NULL == OsalTimer) 
        ||  (NULL == StartCountDown) 
        ||  (NULL == ResetCountDown)
        )
    {
        status = PHNFCSTVAL(CID_FRI_NFC_AUTO_DEV_DIS, NFCSTATUS_INVALID_PARAMETER);
    }
    else
    {
        OsalTimer->CountDownState     =   PH_OSALNFC_TIMER_RESET;
        OsalTimer->StartCountDown     =   StartCountDown;
        OsalTimer->ResetCountDown     =   ResetCountDown;
        OsalTimer->PlatformContext    =   IntegrationContext;
        OsalTimer->CbTimerExpired     =   NULL;
        OsalTimer->CbTimerExpiredCtx  =   NULL;
        status                        =   NFCSTATUS_SUCCESS;
    }    
    return status;
}

DWORD WINAPI phOsalNfc_Timer_Procedure(LPVOID iValue)
{
    DWORD       dwRes;
    phOsalNfc_Timer_t* osalTimer     =   (phOsalNfc_Timer_t*)       iValue;
    phOsalNfc_TimerX86Nt_t*  timer    =   (phOsalNfc_TimerX86Nt_t*)  osalTimer->PlatformContext;

    osalTimer->CountDownState   =   PH_OSALNFC_TIMER_RESET;

    while(!timer->freeResources)
    {
        osalTimer->CountDownState =   PH_OSALNFC_TIMER_RESET;

        /* Wait for Starting Event */
        dwRes = WaitForSingleObject(timer->hStartCount, INFINITE);

        phOsalNfc_ConsumeSemaphore( timer->semaphore );
        osalTimer->CountDownState   =   PH_OSALNFC_TIMER_COUNTING;
        timer->cbCalled   =   0;
        timer->dontCallCB    =   0;  
        phOsalNfc_ProduceSemaphore( timer->semaphore);

        /* Reseting Abort event */
        ResetEvent(timer->hAbort);

        /* Waiting for Abort event */
        dwRes = WaitForSingleObject(timer->hAbort, timer->countDown);

        while(NULL == timer->hThread)
        {
            phOsalNfc_Suspend(0);
        }

        phOsalNfc_ConsumeSemaphore( timer->semaphore );
        if(     WAIT_TIMEOUT == dwRes
            &&  NULL        !=  osalTimer->CbTimerExpired
            &&  0           ==  timer->dontCallCB
           )
        {
            /* The PhaseTimer has expired */
           osalTimer->CbTimerExpired(osalTimer->CbTimerExpiredCtx, NFCSTATUS_SUCCESS);
           timer->cbCalled       =   1;
        }
        else
        {
            /* The PhaseTimer has been cancelled */
        }
        phOsalNfc_ProduceSemaphore( timer->semaphore );
    }
    SetEvent(timer->hResourcesFreed);
    return TRUE;
}

NFCSTATUS phOsalNfc_Timer_StartCountDown(phOsalNfc_Timer_t* osalTimer, pphOsalNfc_Timer_CbExpired_t TimerExpiredFunc, void* TimerExpiredFuncCtx, uint32_t   CountDown)
{
    phOsalNfc_TimerX86Nt_t*  timer   =   (phOsalNfc_TimerX86Nt_t*)osalTimer->PlatformContext;
    NFCSTATUS   status =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);

    if(     (NULL == osalTimer) 
        ||  (NULL == timer) 
        ||  (NULL == TimerExpiredFunc) 
        ||  (NULL == TimerExpiredFuncCtx) 
        ||  (NULL == timer->hThread) 
        ||  (NULL == timer->hStartCount) 
        ||  (NULL == timer->hAbort) 
        ||  (NULL == timer->hResourcesFreed)
        ||  (NULL == timer->semaphore )
        )
    {
        status =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }
    else
    {
        osalTimer->CbTimerExpired    = TimerExpiredFunc;
        osalTimer->CbTimerExpiredCtx = TimerExpiredFuncCtx;
        /*  Thread already started: just ask it to wait again!  */
        timer->countDown = CountDown;
        ResetEvent(timer->hAbort);
        if(!SetEvent(timer->hStartCount))
        {
            assert(0);
        }
        while(PH_OSALNFC_TIMER_COUNTING != osalTimer->CountDownState)
        {
            /* Wait for PT to be Reset */
            phOsalNfc_Suspend(0);
        }    
        status  =   NFCSTATUS_SUCCESS;
    }    
    return status;
}

NFCSTATUS phOsalNfc_Timer_ResetCountDown(phOsalNfc_Timer_t* osalTimer)
{
    phOsalNfc_TimerX86Nt_t*  timer   =   (phOsalNfc_TimerX86Nt_t*)osalTimer->PlatformContext;
    NFCSTATUS   status =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);

    if(NULL ==  timer->hAbort)
    {
        status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }
    else if(PH_OSALNFC_TIMER_RESET   ==  osalTimer->CountDownState)
    {
        /* Already in reset state */
        status = NFCSTATUS_SUCCESS;
    }
    else if(!SetEvent(timer->hAbort))
    {
        /* Do the Reset */
        status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    }
    else
    {
        phOsalNfc_ConsumeSemaphore( timer->semaphore );
        if(     0   ==  timer->cbCalled )
        {
            timer->dontCallCB =   1;  
            status = NFCSTATUS_SUCCESS;
        }
        else
        {
            status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_CMD_ABORTED);
        }

        phOsalNfc_ProduceSemaphore( timer->semaphore );

        while(PH_OSALNFC_TIMER_RESET    !=  osalTimer->CountDownState)
        {
            /* Wait for PT to be Reset */
            phOsalNfc_Suspend(0);
        }
    }

    return status;
}



NFCSTATUS phOsalNfc_Timer_Create(phOsalNfc_Timer_t* osalTimer)
{
    NFCSTATUS   status  =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);
    phOsalNfc_TimerX86Nt_t*  timer;    

    if( (NULL != osalTimer))
    {
        timer                       =   phOsalNfc_GetMemory(sizeof(phOsalNfc_TimerX86Nt_t));
        osalTimer->PlatformContext  =   (void*) timer;
        (void)memset (timer, 0x00, sizeof(phOsalNfc_TimerX86Nt_t));
        timer->hAbort               =   CreateEvent(NULL, FALSE, FALSE, NULL);
        timer->hStartCount          =   CreateEvent(NULL, FALSE, FALSE, NULL);
        timer->hResourcesFreed      =   CreateEvent(NULL, FALSE, FALSE, NULL);
        timer->freeResources        =   0;
        timer->countDown            =   0,
        timer->cbCalled             =   0;
        timer->dontCallCB           =   0;  
        timer->hThread              =   CreateThread(NULL, 0, phOsalNfc_Timer_Procedure, osalTimer, 0, NULL);
        osalTimer->CbTimerExpired   =   NULL;
        osalTimer->CbTimerExpiredCtx=   NULL;
        status  =   phOsalNfc_CreateSemaphore( &timer->semaphore, 1, 1);
    }
    return status;
}

NFCSTATUS phOsalNfc_Timer_Release(phOsalNfc_Timer_t* osalTimer)
{
    phOsalNfc_TimerX86Nt_t*  timer ;    
    HANDLE  event;
    DWORD   dwRes;

    NFCSTATUS   status  =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);

    if( NULL == osalTimer)
    {
        status  =   PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);        
    }
    else
    {
      timer   =   (phOsalNfc_TimerX86Nt_t*)osalTimer->PlatformContext;
      
      if( (NULL == timer) || (NULL == timer->hThread) || (NULL == timer->hStartCount) ||
          (NULL == timer->hAbort) || (NULL == timer->hResourcesFreed) || (NULL == timer->semaphore ))
      {
          status = PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INVALID_PARAMETER);       
      }
      else
      {
          /*    Set CB to NULL, so that it is not called by the Timer */
          osalTimer->CbTimerExpired =   NULL;

          /*    Set Timeout to 0, so that wez don't wait useless for timer->countDown */
          timer->countDown      =   0;

          /*    Abort the timer, free resources, ... */
          event   =   timer->hResourcesFreed;
          timer->freeResources = 1;
          SetEvent(timer->hStartCount);
          event   =   timer->hResourcesFreed;
          dwRes = WaitForMultipleObjects(1, &event, FALSE, INFINITE);
          CloseHandle(timer->hThread);
          CloseHandle(timer->hStartCount);
          CloseHandle(timer->hAbort);
          CloseHandle(timer->hResourcesFreed);
          status  =   phOsalNfc_CloseHandle( timer->semaphore );
          timer->hThread          =   NULL;
          timer->hStartCount      =   NULL;
          timer->hAbort           =   NULL;
          timer->hResourcesFreed  =   NULL;
          timer->semaphore        =   NULL;
          phOsalNfc_FreeMemory(timer);
      } 
    }
    return status;
}


#endif