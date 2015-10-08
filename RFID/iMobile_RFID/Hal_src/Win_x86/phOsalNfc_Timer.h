
//  $Revision: 1.19 $

#ifndef PHOSALNFC_TIMER_H 
#define PHOSALNFC_TIMER_H 


/* -----------------Include files ---------------------------------------*/ 
#include <phNfcTypes.h>
#ifdef PH_NFC_CUSTOMINTEGRATION
#include <phNfcCustomInt.h>
#else
#ifdef NXP_MESSAGING
#include <nfc_osal_deferred_call.h>
#endif

#ifdef NXP_MESSAGING
/**
 * \ingroup grp_osal_nfc
 *\brief Deferred message specific info declaration.
 * This type information packed as WPARAM  when \ref PH_OSALNFC_MESSAGE_BASE type windows message 
 * is posted to message handler thread.
 */ 
typedef struct phOsalNfc_DeferedCalldInfo
{
		nfc_osal_def_call_t  pDeferedCall;/**< pointer to Deferred callback */
		void				*pParam;/**< contains timer message specific details*/
}phOsalNfc_DeferedCalldInfo_t;
#endif

/* ---------------- Macros ----------------------------------------------*/
/**
*\ingroup grp_osal_nfc
* OSAL timer message .This message type will be posted to calling application thread.
*/
#define PH_OSALNFC_TIMER_MSG			  (0x315)    
/**
 * \ingroup grp_osal_nfc
 * Invalid timer ID type.This ID used indicate timer creation is failed.
 */
#define PH_OSALNFC_INVALID_TIMER_ID		  (0xFFFF)

/*!
 * \ingroup grp_osal_nfc
 * \brief Timer callback interface which will be called once registered timer time out 
 * expires.
 * \param[in] TimerId  Timer Id for which callback is called.
 * \retval  None
 */
typedef void (*ppCallBck_t)(uint32_t TimerId); 

/* -----------------Structures and Enumerations -------------------------*/

/**
 * \ingroup grp_osal_nfc
 **\brief Timer message structure definition.
 * Timer Message Structure contains timer specific informations like timer identifier
 * and timer callback.
 *
 */
typedef struct phOsalNfc_TimerMsg
{
   uint32_t			TimerId;/**< Timer ID*/
   ppCallBck_t		pCallBck;/**< pointer to Timer Callback*/
}phOsalNfc_Timer_Msg_t,*pphOsalNfc_TimerMsg_t;

/* -----------------Exported Functions----------------------------------*/

/**
 * \ingroup grp_osal_nfc
 * \brief Allows to create new timer.
 * This API creates a cyclic timer.Incase valid timer is created returned timer ID will be
 * other than \ref PH_OSALNFC_INVALID_TIMER_ID.Incase returned timer id is \ref PH_OSALNFC_INVALID_TIMER_ID
 * indicates timer creation is failed.
 * When Timer created ,By default timer is not started.Application has to start timer explicitly 
 * needs using \ref phOsalNfc_Timer_Start() API.
 *
 * \param[in]  void
 * \retval     Valid timer Timer ID. 
 *			   \note <br><br> If timer ID value is PH_OSALNFC_INVALID_TIMER_ID
 *				Indicates timer is not created.
 * \retval     PH_OSALNFC_INVALID_TIMER_ID,Indicates timer creation failed.
 *
 *\msc
 *Application,phOsalNfc;
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Create()",URL="\ref phOsalNfc_Timer_Create"];
 *Application<<phOsalNfc [label="Returns Valid timer ID"];
 *\endmsc
 */

uint32_t phOsalNfc_Timer_Create (void);
/**
 * \ingroup grp_osal_nfc
 * \brief Allows to start already created idle timer.
 * This function  starts the requested timer. If the timer is already running, 
 * timer stops and restarts with the new timeout value and new callback function in case any.
 *
 * * \note The old timeout and callback reference are not valid any more if timer is restarted 
 * again.<br><br> Notification are periodic and notification stops only when timer is stoped .
 * 
 * \param[in]  TimerId		valid timer ID obtained during timer creation.
 * \param[in]  RegTimeCnt	Requested time out in Milliseconds.
 *							\note  In windows environment timer resolution should be more than
 *                          50 mSec. 
 *							Incase time out value is below 50 mSec accuracy of timer  behaviour
 *							is not gauranteed .
 *
 * \param[in]  Application_callback    Application  Callback interface to be called when timer 
 *									   expires.
 *
 *\msc
 *Application,phOsalNfc;
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Create()",URL="\ref phOsalNfc_Timer_Create"];
 *Application<<phOsalNfc [label="TIMERID"];
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Start(TIMERID)",URL="\ref phOsalNfc_Timer_Start"];
 *--- [label=" : On timer time out expired "];
 *phOsalNfc=>phOsalNfc [label="nfc_osal_deferred_call()",URL="\ref nfc_osal_deferred_call"];
 *Application<-phOsalNfc[label="PH_OSALNFC_TIMER_MSG"];
 \endmsc
 */

void phOsalNfc_Timer_Start(uint32_t     TimerId,
						  uint32_t     RegTimeCnt, 
						  ppCallBck_t  Application_callback);

/**
 * \ingroup grp_osal_nfc
 * \brief Allows to stop the timer which is already started.
 * This API allows to stop running timer.Incase timer is stopped,timer callback will not be 
 * notified any more.However once timer is started again ,it starts notifying 
 *timer callback ,once timeout expires.
 *
 *\param[in] TimerId				valid timer ID obtained suring timer creation.
 *\param[in] Application_callback   Application  Callback interface to be called when timer expires.
 *
 *\msc
 *Application,phOsalNfc;
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Create()",URL="\ref phOsalNfc_Timer_Create"];
 *Application<<phOsalNfc [label="TIMERID"];
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Start(TIMERID)",URL="\ref phOsalNfc_Timer_Start"];
 * --- [label=" : On timer time out expired "];
 *phOsalNfc=>phOsalNfc [label="nfc_osal_deferred_call()",URL="\ref nfc_osal_deferred_call"];
 *Application<-phOsalNfc[label="PH_OSALNFC_TIMER_MSG"];
 *Application=>phOsalNfc [label="phOsalNfc_Timer_Stop(TIMERID)",URL="\ref phOsalNfc_Timer_Stop"]; 
 \endmsc
 */
void phOsalNfc_Timer_Stop(uint32_t TimerId);

/**
 * \ingroup grp_osal_nfc
 * \brief Allows to delete the timer which is already created.
 * This  API allows to delete timer which already created.Incase timer is running
 * timer is stopped first and timer is deleted. Incase invalid timer ID ,this function 
 * doesn't return any error code.Application has to explicitly ensure timer ID sent
 * is valid.
 *\param[in]  TimerId	timer identieir to delete the timer.
 */
void phOsalNfc_Timer_Delete(uint32_t TimerId);


#endif /* PHOSALNFC_TIMER_H */


/* below code is  for FRI 1.0 OSAL timer  .Currently it is disabled. */
#ifdef FRI_V1

/**
 *  \name Phase Timer
 *
 * File: \ref phFriNfc_ADD_Timer.h
 *
 */
/*@{*/
#define PH_OSALNFC_TIMER_FILEREVISION "$Revision: 1.19 $" /**< \ingroup grp_file_attributes */
#define PH_OSALNFC_TIMER_FILEALIASES  "$Aliases: NFC_FRI1.1_WK912_R21_1,NFC_FRI1.1_WK914_PREP1,NFC_FRI1.1_WK914_R22_1,NFC_FRI1.1_WK914_R22_2,NFC_FRI1.1_WK916_R23_1,NFC_FRI1.1_WK918_R24_1,NFC_FRI1.1_WK920_PREP1,NFC_FRI1.1_WK920_R25_1,NFC_FRI1.1_WK922_PREP1,NFC_FRI1.1_WK922_R26_1,NFC_FRI1.1_WK924_PREP1,NFC_FRI1.1_WK924_R27_1,NFC_FRI1.1_WK926_R28_1,NFC_FRI1.1_WK928_R29_1,NFC_FRI1.1_WK930_R30_1,NFC_FRI1.1_WK934_PREP_1,NFC_FRI1.1_WK934_R31_1,NFC_FRI1.1_WK941_PREP1,NFC_FRI1.1_WK941_PREP2,NFC_FRI1.1_WK941_1,NFC_FRI1.1_WK943_R32_1,NFC_FRI1.1_WK949_PREP1,NFC_FRI1.1_WK943_R32_10,NFC_FRI1.1_WK943_R32_13,NFC_FRI1.1_WK943_R32_14,NFC_FRI1.1_WK1007_R33_1,NFC_FRI1.1_WK1007_R33_4,NFC_FRI1.1_WK1017_PREP1,NFC_FRI1.1_WK1017_R34_1,NFC_FRI1.1_WK1017_R34_2,NFC_FRI1.1_WK1023_R35_1 $"      /**< \ingroup grp_file_attributes */
/*@}*/


#include <phNfcTypes.h>

/**
 * \name phase timer-states Definitions
 *
 * These are the definitions of phase timer-states that can be assigned to a phase timer.
 *
 */
/*@{*/
#define PH_OSALNFC_TIMER_RESET      0 /**< \ingroup grp_fri_nfc_auto_dev_dis_seq
                                                           *   Reset State.
                                                           */
#define PH_OSALNFC_TIMER_COUNTING   1 /**< \ingroup grp_fri_nfc_auto_dev_dis_seq
                                                           *   Counting State.
                                                           */
/*@}*/


/*!
 *  TODO: updated this documentation
 *  \ingroup grp_osal_nfc_dev_dis_seq
 *
 *  \brief PhaseTimer Handler definition to Reset the count down.
 *
 *  The generic, (can be user-defined) handler function for the individual Phase Timer resetting count down.
 *  
 *  \par Parameter: Phase Timer
 *       Phase timer pointer has then to :
 *          - access his own IntegrationContext embeded into the \ref phOsalNfc_Timer_t structure
 *          - Reset the \ref CountDownState variable to Reset once the count down has been reset
 *          - has soon as the CountDown stops, reset to zero the \ref CountDownState variable 
 */

typedef NFCSTATUS (*pphOsalNfc_Timer_CbExpired_t)(void* context, NFCSTATUS status);


/*!
 *  \ingroup grp_fri_nfc_auto_dev_dis_seq
 *
 *  \brief PhaseTimer Handler definition to Start the count down.
 *
 *  The generic, (can be user-defined) handler function for the individual Phase Timer start count down.
 *
 *  \par Parameter: Phase Timer
 *       Phase timer pointer has then to :
 *          - Access user own IntegrationContext embeded into the \ref phOsalNfc_Timer_t structure
 *          - Set the \ref CountDownState variable to Counting once it the count down has started (before this function returns)
 *          - As soon as the CountDown stops, set the \ref CountDownState variable to Reset
 *  \par Parameter: CountDown 
 *       Value in msec of the Count Down 
 */

typedef NFCSTATUS (*pphOsalNfc_Timer_StartCountDown_t)(struct phOsalNfc_Timer* PhaseTimer, pphOsalNfc_Timer_CbExpired_t TimerExpiredFunc, void* TimerExpiredFuncCtx, uint32_t    CountDown);

/*!
 *  \ingroup grp_osal_nfc_dev_dis_seq
 *
 *  \brief PhaseTimer Handler definition to Reset the count down.
 *
 *  The generic, (can be user-defined) handler function for the individual Phase Timer resetting count down.
 *
 *  \par Parameter: Phase Timer
 *       Phase timer pointer has then to :
 *          - access his own IntegrationContext embeded into the \ref phOsalNfc_Timer_t structure
 *          - Reset the \ref CountDownState variable to Reset once the count down has been reset
 *          - has soon as the CountDown stops, reset to zero the \ref CountDownState variable 
 */

typedef NFCSTATUS (*pphOsalNfc_Timer_ResetCountDown_t)(struct phOsalNfc_Timer* PhaseTimer);


/**
 * \ingroup grp_osal_nfc_seq
 *
 * Generic Phase Timer. The integration has to ensure it is implemented, if used.
 *
 */
typedef struct phOsalNfc_Timer
{

    uint8_t                                      CountDownState;      /**< State of the phase timer (e.g.: Reset, Counting) */
    void                                         *PlatformContext;  /**< Pointer to Context used by the integration in \ref StartCountDown 
                                                                                  and \ref ResetCountDown functions.
                                                                                  It has to be provided by the user!*/
    pphOsalNfc_Timer_StartCountDown_t  StartCountDown;      /**< Pointer to Start Count Down function. 
                                                                                  It has to be provided by the user!*/
    pphOsalNfc_Timer_ResetCountDown_t  ResetCountDown;      /**<  Pointer to Reset Count Down function. 
                                                                                  It has to be provided by the user!*/

    pphOsalNfc_Timer_CbExpired_t       CbTimerExpired;
    void*                              CbTimerExpiredCtx;
} phOsalNfc_Timer_t;



/**
 * \ingroup grp_fri_nfc_auto_dev_dis_cr
 *
 * \brief This Function allows to easily initialize a timer.
 *  
 *  We must initialize here all needed members: function pointers, state, etc...
 *
 * \param[in,out] osalTimer   Pointer to an uninitialized timer.
 *
 * \param[in]     StartCountDown   Pointer to the Integration-defined function to be used for starting the timer's count down
 *
 * \param[in]     ResetCountDown   Pointer to the Integration-defined function to be used for resetting the timer's count down
 *
 * \param[in]     Context   (Optional) Pointer to the Integration-defined context to be used by the timer
 *
 * \retval NFCSTATUS_SUCCESS                Operation successful.
 * \retval NFCSTATUS_INSUFFICIENT_RESOURCES  Not enough resources available.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_Timer_Initialize( phOsalNfc_Timer_t                    *osalTimer, 
                                      pphOsalNfc_Timer_StartCountDown_t    StartCountDown,
                                      pphOsalNfc_Timer_ResetCountDown_t    ResetCountDown,
                                      void                                 *IntegrationContext);

/**
 * \ingroup grp_osal_nfc_seq
 *
 *  \brief This function allows to start the timer.
 *
 *  When the timer expires, the TimerExpiredFunc will be called. 
 *
 * \param[in,out] osalTimer           Pointer to an uninitialized timer.
 *
 * \param[in]     TimerExpiredFunc    Pointer to the Integration-defined function to be used when the timer expires
 *
 * \param[in]     TimerExpiredFuncCtx Pointer to the Integration-defined context to be passed as a parameter of the TimerExpiredFunc function
 *
 * \param[in]     CountDown           Duration of the timer
 *
 * \retval NFCSTATUS_SUCCESS               Operation successful.
 * \retval NFCSTATUS_INVALID_PARAMETER     At least one parameter value is invalid.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_Timer_StartCountDown (struct phOsalNfc_Timer*       osalTimer, 
                                          pphOsalNfc_Timer_CbExpired_t  TimerExpiredFunc, 
                                          void*                         TimerExpiredFuncCtx,
                                          uint32_t                      CountDown);
/**
 * \ingroup grp_osal_nfc_seq
 *
 *  \brief This function allows to stop and reset timer.
 *
 * \param[in,out] osalTimer  Pointer to an uninitialized Timer.
 *
 * \retval NFCSTATUS_SUCCESS               Operation successful.
 * \retval NFCSTATUS_INVALID_PARAMETER     At least one parameter value is invalid.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_Timer_ResetCountDown (struct phOsalNfc_Timer* osalTimer);

/**
 * \ingroup grp_osal_nfc_seq
 *
 *  \brief This Function allocates the resources of a timer.
 *         This shall be the first one to be called for intializing this timer.
 *
 * \param[in,out] osalTimer  The pointer to the Osal timer
 *
 * \retval NFCSTATUS_SUCCESS               Operation successful.
 * \retval NFCSTATUS_INVALID_PARAMETER     At least one parameter value is invalid.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_Timer_Create (struct phOsalNfc_Timer* osalTimer);

/*!
 *  \ingroup grp_osal_nfc_seq
 *
 *  \brief This function frees the resources used by the osal timer.
 *         It shall be called when the timer will not to be used anymore.
 *
 * \param[in,out] osalTimer  The pointer to the Osal timer
 *
 * \retval NFCSTATUS_SUCCESS               Operation successful.
 * \retval NFCSTATUS_INVALID_PARAMETER     At least one parameter value is invalid.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_Timer_Release (struct phOsalNfc_Timer* osalTimer);
#endif /* #ifdef PH_NFC_CUSTOMINTEGRATION */
#endif
