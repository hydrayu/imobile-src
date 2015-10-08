
// $Revision: 1.21 $

#ifndef PHOSALNFC_H
#define PHOSALNFC_H


/** \defgroup grp_osal_nfc OSAL Component
 *
 *\note: API listed here encompasses Operating System Abstraction Layer interfaces required to be mapped to underlying OS platforms.
 *
 */

#include <phNfcTypes.h>


#ifdef PH_NFC_CUSTOMINTEGRATION
#include <phNfcCustomInt.h>
#else
#include <memory.h>

/**< OSAL Message Type */

#define PH_OSALNFC_MESSAGE_BASE  (WM_USER+0x3FF)    

/*!
 * \ingroup grp_osal_nfc
 *
 * OSAL Message structure contains message specific details like,
 * message type ,message specific data block details
 */
typedef struct phOsalNfc_Message
{
    uint32_t eMsgType;/**< Type of the message to be posted*/
    void   * pMsgData;/**< Pointer to message specific data block in case any*/
    uint16_t Size;/**< Size of the datablock*/
} phOsalNfc_Message_t,*pphOsalNfc_Message_t;

/*!
 * \ingroup grp_osal_nfc
 *
 * Enum definition contains  supported exception types
  */
typedef enum
{
    phOsalNfc_e_NoMemory,						/**<Memory allocation failed */
    phOsalNfc_e_PrecondFailed,					/**<precondition wasn't met */
    phOsalNfc_e_InternalErr,					/**<Unrecoverable error */
    phOsalNfc_e_UnrecovFirmwareErr,				/**<Unrecoverable firmware error */
    phOsalNfc_e_DALerror,						/**<Unrecoverable DAL error */
	phOsalNfc_e_Noerror							/**<No errortype */
} phOsalNfc_ExceptionType_t ;

/* OsalNfc Status Type */

/** \ingroup grp_retval1
    A new semaphore could not be created due to
    a system error. */
#define NFCSTATUS_SEMAPHORE_CREATION_ERROR                      (0x1010)

/** \ingroup grp_retval1
    The given semaphore could not be released due to
    a system error or invalid handle. */
#define NFCSTATUS_SEMAPHORE_PRODUCE_ERROR                       (0x1011) 

/** \ingroup grp_retval11
    The given semaphore could not be consumed due to a
    system error or invalid handle. */
#define NFCSTATUS_SEMAPHORE_CONSUME_ERROR                       (0x1012) 


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Thread Function Pointer Declaration.
 *
 * This points to the function ACTUALLY being the "workhorse" of the thread.
 * It is \b not the immediate thread procedure since the API of this function
 * depends on the OS. \n\n
 *
 * Rather, this function shall be called within the body of the thread procedure
 * defined by the underlying, OS-depending OSAL implementation.
 *
 * \b MANDATORY: \n
 *  YES
 */

typedef void(*pphOsalNfc_ThreadFunction_t)(void*);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Thread Creation.
 *
 * This function creates a thread in the underlying system. To delete the 
 * created thread use the phOsalNfc_CloseHandle function.
 *
 * In case of the platform is not able to create dynamically it own threads, 
 * the integrator can still modify phLibNfc_Int_Initialize function
 * to avoid the threads creation by handling with already created threads.
 * 
 *
 * \param[in,out] hThread    The handle: The caller has to prepare a void
 *                           pointer (that needs not to be initialized). The
 *                           value (content) of the pointer is set by the function.
 *
 * \param[in] ThreadFunction Pointer to a function within the
 *                           implementation that shall be called by the Thread
 *                           procedure. This represents the Thread main function.
 *                           When this function exits the thread exits.
 * \param[in] Param          A pointer to a user-defined location the thread
 *                            function receives.
 *
 * \retval NFCSTATUS_SUCCESS The operation was successful.
 * \retval NFCSTATUS_INSUFFICIENT_RESOURCES     At least one parameter value is invalid.
 *
 * \b MANDATORY: \n
 *  NO, The createThread function is not called directly by the stack. This function could be called
 *  by phLibNfc_Int_Initialize() in the integration file. It's up to the integrator to implement the 
 *  createThread function or to copy the source code in the integration file.
 */ 
NFCSTATUS phOsalNfc_CreateThread(void                        **hThread, 
                                 pphOsalNfc_ThreadFunction_t     ThreadFunction,
                                 void                         *Param);

/*!
 * \brief Semaphore Creation.
 * This function creates a semaphore in the underlying system. 
 * To delete the created thread use the phOsalNfc_CloseHandle function.
 * 
 * In case of the platform is not able to create dynamically it own semaphore,
 * the integrator can modify phLibNfc_Int_Initialize function to avoid the semaphores 
 * creation by handling with already created semaphores.
 *
 * \note: A wait function applied to the semaphore blocks when its value is zero.
 *
 * \param[in,out] hSemaphore The handle: The caller has to prepare a void
 *                           pointer (that needs not to be initialized). The
 *                           value (content) of the pointer is set by the function.
 *
 * \param[in] InitialValue   The initial value of the Semaphore.
 * \param[in] MaxValue       The maximum value of the Semaphore.
 *
 * \retval NFCSTATUS_SUCCESS The operation was successful.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_CreateSemaphore(void     **hSemaphore,
                                    uint8_t    InitialValue,
                                    uint8_t    MaxValue);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Closes the handle to a system object.
 *
 * This function closes the handle and frees all resources of a system object (thread or semaphore).
 * 
 * In case of the platform is not able to create thread and semaphore 
 * dynamically, this function has no effect and must return NFCSTATUS_SUCCESS.
 *
 * \param[in] hItem The handle of the system object.
 *
 * \retval NFCSTATUS_SUCCESS            The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER  At least one parameter value is invalid.
 *
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_CloseHandle(void *hItem);

/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Semaphore-Produce (never waiting).
 *
 * Increment the value of the semaphore. This function never blocks.
 * The purpose is to enable a waiting thread ("consumer") to continue
 * if it has bee waiting because of the Semaphore value set to zero.
 *
 * \param[in] hSemaphore The handle of the Semaphore.
 *
 * \retval NFCSTATUS_SUCCESS                  The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER        At least one parameter value is invalid.
 * \retval NFCSTATUS_SEMAPHORE_PRODUCE_ERROR  The semaphore can not be produced due to a system error or invalid handle .
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_ProduceSemaphore(void *hSemaphore);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Semaphore Consumption (waiting if value is zero).
 *
 * Decrement the value of the semaphore. When the internal value is
 * non-zero, the function continues. If the value is zero however, the
 * function blocks for an infinite time.
 *
 * \param[in] hSemaphore The handle of the Semaphore.
 *
 * \retval NFCSTATUS_SUCCESS                  The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER        At least one parameter value is invalid.
 * \retval NFCSTATUS_SEMAPHORE_PRODUCE_ERROR  The semaphore can not be consumed due to a system error or invalid handle .
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_ConsumeSemaphore(void *hSemaphore);


/*!
 * \ingroup grp_osal_nfc
 * \brief Raises exception
 *
 * The program jumps out of the current execution flow, i.e. this function doesn't return.
 * The given exception contains information on what has happened and how severe the error is.
 * @warning This function should only be used for exceptional error situations where there is no
 * means to recover.
 *
 * \param[in] eExceptiontype  exception Type.
 * \param[in] reason     This is an additional reason value that gives a vendor specific reason code
 *
 * \retval  None
 */

void phOsalNfc_RaiseException(phOsalNfc_ExceptionType_t    eExceptiontype,
							  uint16_t                   reason);

/*!
 * \ingroup grp_osal_nfc
 * \brief DebugTrace

 * Outputs trace log of requested size as string

 * \param[in] data Data block.
 * \param[in] size buffer size of the data block.
 *
 * \retval None
 */
void phOsalNfc_DbgTrace(uint8_t data[], uint32_t size);

/*!
 * \ingroup grp_osal_nfc
 * \brief This API prints given string to debug port.
 * \param[in] pString  pointer to buffer content to be displayed.
 * \retval  None
 */
void phOsalNfc_DbgString(const char *pString);

/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Semaphore Consumption Extended (waiting if value is zero).
 *
 * Decrement the value of the semaphore. When the internal value is
 * non-zero, the function continues. If the value is zero however, the
 * function blocks.
 *
 * \param[in] hSemaphore The handle of the Semaphore.
 * \param[in] TimeOut    The timeout value in ms.
 *
 * \retval NFCSTATUS_SUCCESS                  The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER        At least one parameter value is invalid.
 * \retval NFCSTATUS_SEMAPHORE_CONSUME_ERROR  The semaphore can not be consumed due to a system error or invalid handle .
 *
 * \b MANDATORY: \n
 *  YES
 */

NFCSTATUS phOsalNfc_ConsumeSemaphoreEx(void *hSemaphore, uint32_t Timeout);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Wait/Sleep/Suspend function.
 *
 * Suspends the current thread for the specified time. Enables the
 * scheduler to switch the context meanwhile.
 *
 * \param[in] WaitingTime Time in \b ms to wait.
 *
 * \b MANDATORY: \n
 *  YES
 */
void phOsalNfc_Suspend(uint32_t WaitingTime);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Function use to post a message to a thread.
 *
 * \param[in] SourceID   The Thread identifier of the sending thread.
 *                       This value might be different from the caller Id thread.
 * \param[in] DestID     Thread identifier of the destination thread
 * \param[in] pMsg       The message to be sent itself
 *
 * \retval NFCSTATUS_SUCCESS                The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER      Some caller provided parameter was invalid.
 * \retval NFCSTATUS_INSUFFICIENT_RESOURCES The operation could not complete because of lack of resources.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_PostMsg(uint32_t SourceID, uint32_t DestID, phOsalNfc_Message_t * pMsg);

/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Function use to receive a message.
 *
 * \param[out] pSourceID   The Thread identifier of the thread that had posted the message.
 * \param[in, out] pMsg    The message to be filled with the received data
 *
 * \retval NFCSTATUS_SUCCESS The operation was successful.
 * \retval NFCSTATUS_INVALID_PARAMETER Some caller provided parameter was invalid.
 * \retval NFCSTATUS_INSUFFICIENT_RESOURCES The operation could not complete because of lack of resources.
 *
 * \b MANDATORY: \n
 *  YES
 */
NFCSTATUS phOsalNfc_ReceiveMsg(uint32_t *pSourceID, phOsalNfc_Message_t* pMsg);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Get the task identifier of the caller thread.
 *
 * \retval takID of the caller thread
 *
 * \b MANDATORY: \n
 *  YES
 */
uint32_t phOsalNfc_GetTaskId(void);

/*!
 * \ingroup grp_osal_nfc
 * \brief Allocates some memory.
 *
 * \param[in] Size   Size, in uint8_t, to be allocated
 *
 * \retval NON-NULL value:  The memory was successfully allocated ; the return value points to the allocated memory location
 * \retval NULL:            The operation was not successful, certainly because of insufficient resources.
 *
 */
extern void * phOsalNfc_GetMemory(uint32_t Size);

/*!
 * \ingroup grp_osal_nfc
 * \brief This API allows to free already allocated memory.
 * \param[in] pMem  Pointer to the memory block to deallocated
 * \retval None
 */
void   phOsalNfc_FreeMemory(void * pMem);


/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Initializes Random Number Generator.
 * \retval None
 */
void   phOsalNfc_RandInit( void);

/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Gets Random number. \ref phOsalNfc_RandInit function shall have been called first.
 *
 * \retval Random number
 *
 * \b MANDATORY: \n
 *  YES
 */
uint16_t    phOsalNfc_RandGetNumber( void);

/*!
 * \ingroup1 grp_osal_nfc1
 * \brief Gets system ticks. 
 * This function is used for debug only (tracing) and performance measurement.
 * Thus, it is not mandatory to implement it; moreover, the units that are used is not important, as the
 * difference between 2 calls is the only information that matters.
 *
 * \retval System Ticks
 *
 * \b MANDATORY: \n
 *  NO, this function is used only in debug mode.
 */
uint32_t    phOsalNfc_GetTicks( );
#endif 


/*!
 * \ingroup grp_osal_nfc
 * \brief Compares the values stored in the source memory with the 
 * values stored in the destination memory.
 *
 * \param[in] src   Pointer to the Source Memory
 * \param[in] dest  Pointer to the Destination Memory
 * \param[in] n     Number of bytes to be compared.
 *
 * \retval Zero value:        The comparison was successful,
                    Both the memory areas contain the identical values.
 * \retval Non-Zero Value:    The comparison failed, both the memory
 *                  areas are non-identical.
 *
 */

int phOsalNfc_MemCompare ( void *src, void *dest, unsigned int n );


#endif /*  PHOSALNFC_H  */
