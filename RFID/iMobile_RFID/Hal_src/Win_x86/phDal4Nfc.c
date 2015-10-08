
//  $Revision: 1.71 $

#include <phDal4Nfc.h>
#include <phOsalNfc.h>
#include <phNfcStatus.h>
#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include <phDal4Nfc_DeferCall.h>

/* DAL Defines related to COM port */
#define PURGE_FLAGS             PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR 
#define READ_TIMEOUT            5000      /* milliseconds */
#define AMOUNT_TO_READ          256      /* Local Buffer Size */
#define MAX_WRITE_BUFFER        2048
#define MAX_READ_BUFFER         2048

#define PHOSALNFC_MESSAGE_BASE WM_USER+0x3FF     /*  For OSAL massage */

#define DEFAULT_SERIAL_PORT     "COM1"
/* DAL driver variables and structures */
static HANDLE                   h_serial_port = NULL;           /* COM port Handle */
static HANDLE                   h_threadsEvent = NULL;          /* Thread Start event Handle */


static phDal4Nfc_SContext_t     sDalNfc_Ctxt ;
static pphDal4Nfc_SContext_t    psDalNfc_Ctxt ;
static phHal_sHwReference_t     *DalNfc_HwRef;
static phOsalNfc_Message_t      gphDal4Nfc_PostOsalMsg;

static DWORD                    sHalThreadID; 
static BOOL                     sDalConfig_Status = FALSE;
static CRITICAL_SECTION         lpCriticalSection;

static void phDal4Nfc_DeferredCb (void  *params);

#ifdef NXP_MESSAGING
static  phDal4Nfc_DeferMsg_t            sReadDeferedCalldInfo={0};
static  phDal4Nfc_DeferMsg_t            sWriteDeferedCalldInfo={0};
#endif

/*SC18IM700- I2C*/
#define I2C_START          0x53
#define I2C_STOP           0x50
#define I2C_READ_INT_REG   0x52
#define I2C_WRITE_INT_REG  0x57
#define I2C_READ_GPIO      0x49
#define I2C_WRITE_GPIO     0x4F

#define I2C_SLAVEADDR      0x50/*0xC6-LED controller 0x40IO- Expander board 0xC6-LED controller*/   

/*SC18IM700 internal regs*/
#define I2C_REG_BRG0       0x00
#define I2C_REG_BRG1       0x01
#define I2C_REG_PORTCONF1  0x02
#define I2C_REG_PORTCONF2  0x03
#define I2C_REG_IOSTATE    0x04
#define I2C_REG_ADDR       0x06
#define I2C_REG_CLKL       0x07
#define I2C_REG_CLKH       0x08
#define I2C_REG_TIMEOUT    0x09
#define I2C_REG_STATUS     0x0A
#define I2C_BRG0_VAL       0xF0  /*9600 baud rate*/
#define I2C_BRG1_VAL       0x02  /*9600 baud rate*/
#define I2C_PORTCONF1_VAL  0xFF  /*FF-open drain output,0x55-input only*/
#define I2C_PORTCONF2_VAL  0x55
#define I2C_IOSTATE_VAL    0x0F
#define I2C_ADDR_VAL       0x26
#define I2C_CLKL_VAL       0x13
#define I2C_CLKH_VAL       0x13
#define I2C_TIMEOUT_VAL    0x66

#define BUFSIZE_I2C        256
/*#define DAL_PRINT_ENABLE  */

#ifdef DAL_TRACE
#define MAX_TRACE_BUFFER    150

#define DAL_PRINT( str )  phOsalNfc_DbgString(str)
#define DAL_DEBUG(str, arg)     \
{                                       \
    char        trace[MAX_TRACE_BUFFER];                    \
    snprintf(trace,MAX_TRACE_BUFFER,str,arg);   \
    phOsalNfc_DbgString(trace);                 \
}

#define DAL_PRINT_BUFFER(msg,buf,len)       \
{                                       \
    uint16_t    i = 0;                  \
    char        trace[MAX_TRACE_BUFFER];                    \
    snprintf(trace,MAX_TRACE_BUFFER,"\n\t %s:",msg);    \
    phOsalNfc_DbgString(trace);                 \
    phOsalNfc_DbgTrace(buf,len);            \
    phOsalNfc_DbgString("\r");              \
}
#else
#define DAL_PRINT( str ) 
#define DAL_DEBUG(str, arg) 
#define DAL_PRINT_BUFFER(msg,buf,len)   
#endif




/*structure holds members related for both read and write operations*/
typedef struct Dal_RdWr_st
{
    /*members for write related operations*/
    OVERLAPPED   osWriter;               /* Overlapped ReadStructure */
    BOOL         fWaitingOnWrite;
    DWORD        nNoBytesWritten; 
    uint8_t      *plWrBuf;               /* Local Buffer */
    DWORD        length;         
    BOOL         fWriteThreadDone;
    HANDLE       h_writeThread;          /* Write thread Hanlde */
    BOOL         isWriteBusy;
    DWORD        sWriterThreadID;
    /*members for read related operations*/ 
    DWORD        ReadLength; 
    OVERLAPPED   osReader;              /* Overlapped ReadStructure */
    BOOL         fWaitingOnRead;
    BOOL         fWaitingOnWaitRead;
    BOOL         fCancelWaitRead;
    DWORD        nNoBytesRead;
    DWORD        nNoBytesWaitRead;
    DWORD        nWaitReadLenght;
    uint8_t      *plRdBuf;            /* Local Buffer  */
    uint8_t      *plWaitRdBuf;        /* Local Buffer */
    BOOL         fReadThreadDone;
    HANDLE       h_readThread;        /* Read thread Hanlde */
    BOOL         isReadBusy;
    BOOL         isWaitReadBusy;
    DWORD        sReaderThreadID;
    HANDLE       readcancel;
    HANDLE       EventList[2];
    HANDLE       readevthandle;
    HANDLE       writeevthandle;

}phDal4Nfc_RdWr_t;
static phDal4Nfc_RdWr_t     gphDal4NfcRdWr_t;
static phDal4Nfc_RdWr_t     gphDal4NfcRd_t;
/* Local Function prototypes */
static NFCSTATUS phDal4Nfc_StartThreads(void);
static DWORD WINAPI phDal4Nfc_ReaderThread(void);
static DWORD WINAPI phDal4Nfc_WriterThread(void);
static DWORD phDal4Nfc_SetUpSerialPort(void);
static void phDal4Nfc_FillMsg(phDal4Nfc_Message_t *pDalMsg,phOsalNfc_Message_t *pOsalMsg);/* fill the message structure*/
static BOOL phDal4Nfc_PostMessage(DWORD  ThreadID,phDal4Nfc_Message_t *pDalMsg,phOsalNfc_Message_t *pOsalMsg);
#ifdef NXP_MESSAGING
extern DWORD dwGetMsgThreadId;
#endif 
/* API definitions */
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Register

PURPOSE:  DAL register function.

-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_Register( phNfcIF_sReference_t  *psRefer,
                             phNfcIF_sCallBack_t if_cb, void *psIFConf )
{
    NFCSTATUS               result = NFCSTATUS_SUCCESS;
    phNfcLayer_sCfg_t       *psconfig = (phNfcLayer_sCfg_t *)psIFConf;
    

    if ((NULL != psRefer) && 
        (NULL != psRefer->plower_if) &&
        (NULL != if_cb.receive_complete) && 
        (NULL != if_cb.send_complete) 
        )
    {
        /* Register the LLC functions to the upper layer */
        psRefer->plower_if->init = phDal4Nfc_Init;
        psRefer->plower_if->release = phDal4Nfc_Shutdown;
        psRefer->plower_if->send = phDal4Nfc_Write;
        psRefer->plower_if->receive = phDal4Nfc_Read;
        psRefer->plower_if->receive_wait = phDal4Nfc_ReadWait;
        psRefer->plower_if->transact_abort = phDal4Nfc_ReadWaitCancel;
        psRefer->plower_if->unregister = phDal4Nfc_Unregister;

        
        if (NULL != psDalNfc_Ctxt)
        {
            /* Copy the DAL context to the upper layer */
            psRefer->plower_if->pcontext = psDalNfc_Ctxt;
            /* Register the callback function from the upper layer */
            psDalNfc_Ctxt->cb_if.receive_complete = if_cb.receive_complete;
            psDalNfc_Ctxt->cb_if.send_complete = if_cb.send_complete;
            psDalNfc_Ctxt->cb_if.notify = if_cb.notify;
            /* Get the upper layer context */
            psDalNfc_Ctxt->cb_if.pif_ctxt = if_cb.pif_ctxt;
            /* Update the error state */
            result = NFCSTATUS_SUCCESS;
        }
        else
        {
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_NOT_INITIALISED);
        }
    }
    else /*Input parameters invalid*/
    {
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_PARAMETER);
    }
    return result;
}

/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Unregister

PURPOSE:  DAL unregister function.

-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_Unregister(void *pContext, void *pHwRef )
{
    NFCSTATUS               result = NFCSTATUS_SUCCESS;
        
    if ((NULL == pContext) && (NULL == pHwRef))
    {
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_PARAMETER);
    }
    else
    {
        if (NULL != psDalNfc_Ctxt)
        {
            /* Register the callback function from the upper layer */
            psDalNfc_Ctxt->cb_if.receive_complete = NULL;
            psDalNfc_Ctxt->cb_if.send_complete = NULL ;
            psDalNfc_Ctxt->cb_if.notify = NULL ;
            /* Get the upper layer context */
            psDalNfc_Ctxt->cb_if.pif_ctxt =  NULL ;
            /* gphDal4NfcRd_t.isReadBusy = FALSE; */
        }
        else
        {
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_NOT_INITIALISED);
        }
    }
    return result;
}


/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Init

PURPOSE:  DAL Init function.

-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_Init(void *pContext, void *pHwRef )
{
    NFCSTATUS        result = NFCSTATUS_SUCCESS;
    BOOL            retcheck ;
#ifdef UART_I2C
    static uint8_t*  gpphDal4Nfc_WrBuf_I2c ;           /* Local Buffer pointer */
    static DWORD     gphDal4Nfc_Length_I2c = 0;        /* additional bytes(i2c) to be added to the buffer*/
    char*            i2cbuf_temp_ptr  ;
#endif

    if ((NULL != pContext) && (NULL != pHwRef))
    {
        pContext  = psDalNfc_Ctxt;
        DalNfc_HwRef = (phHal_sHwReference_t *)pHwRef;
        if ( sDalNfc_Ctxt.hw_valid == TRUE )
        {
            /*com port is configured at the application level*/
            h_serial_port = DalNfc_HwRef->p_board_driver;
            if (h_serial_port == INVALID_HANDLE_VALUE)
            {
                result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
            }
            else
            {
                /* Clear Serial port Buffers */
                retcheck = PurgeComm(h_serial_port, PURGE_FLAGS);
            }
        }
        else
        {
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
    }
    else /*Input parametrs invalid*/
    {
        result = NFCSTATUS_INVALID_PARAMETER;
    }

    return result; 
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Shutdown

PURPOSE:  DAL Shutdown function.

-----------------------------------------------------------------------------*/

NFCSTATUS phDal4Nfc_Shutdown( void *pContext, void *pHwRef)
{
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    sDalNfc_Ctxt.hw_valid = FALSE;
    /* Reset the DAL context values to NULL */
    (void)memset((void *)&sDalNfc_Ctxt,0,sizeof(sDalNfc_Ctxt));
    psDalNfc_Ctxt = NULL;  
    return result;
}

NFCSTATUS 
phDal4Nfc_ConfigRelease(
    void    *pHwRef)
{
    NFCSTATUS   result = NFCSTATUS_SUCCESS;
    BOOL        purgecomm;

    /* Clear Serial port Buffers */
    purgecomm = PurgeComm(h_serial_port, PURGE_FLAGS);
    /* Terminate the threads */
    if(!TerminateThread(gphDal4NfcRd_t.h_readThread,FALSE))
    {
        /*Windows Error :Thread Closing isn't successed.*/
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    /* Terminate the threads */
    if(!TerminateThread(gphDal4NfcRdWr_t.h_writeThread,FALSE))
    {
        /*Windows Error :Thread Closing isn't successed.*/
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    if(CloseHandle(h_serial_port) == 0)   
    {
        /*Windows Error */
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    if(CloseHandle(gphDal4NfcRd_t.h_readThread) == 0)   
    {
        /*Windows Error :*/
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    if(CloseHandle(gphDal4NfcRdWr_t.h_writeThread) == 0)   
    {
        /*Windows Error :*/
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    h_serial_port = NULL;
    
    if (NULL != DalNfc_HwRef)
    {
        DalNfc_HwRef = NULL;
    }
    /* Reset the Writer Thread values to NULL */
            (void)memset((void *)&gphDal4NfcRdWr_t,0,sizeof(gphDal4NfcRdWr_t));
    /* Reset the Reader Thread values to NULL */
        (void)memset((void *)&gphDal4NfcRd_t,0,sizeof(gphDal4NfcRd_t));
        
    /************************************************/
    DeleteCriticalSection(&lpCriticalSection);
      
    
    return result;
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Write

PURPOSE:  DAL Write function.

-----------------------------------------------------------------------------*/

NFCSTATUS phDal4Nfc_Write( void *pContext, void *pHwRef,uint8_t *pBuffer, uint16_t length)
{
    BOOL retval;
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    if ((NULL != pContext) && (NULL != pHwRef)&&
        (NULL != pBuffer) && (0 != length))
    {
        if( sDalNfc_Ctxt.hw_valid== TRUE)
        {
            if((!gphDal4NfcRdWr_t.isWriteBusy)&&
                (!gphDal4NfcRdWr_t.fWaitingOnWrite))
            {
                DAL_DEBUG("phDal4Nfc_Write(): %d\n", length);
                /* Make a copy of the passed arguments */
                gphDal4NfcRdWr_t.plWrBuf = pBuffer;
                gphDal4NfcRdWr_t.length  = length;
                /* Change the write state so that thread can take over the write */
                gphDal4NfcRdWr_t.isWriteBusy = TRUE;
                /* Just set variable here. This is the trigger for the Write thread */
                gphDal4NfcRdWr_t.fWaitingOnWrite = TRUE;
                /* Update the error state */ 
                result = NFCSTATUS_PENDING;

                retval=SetEvent(gphDal4NfcRdWr_t.writeevthandle);

            }
            else 
            {
                /* Driver is BUSY with previous Write */
                DAL_PRINT("phDal4Nfc_Write() : Busy \n");
                result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_BUSY) ;    
            }
        }/*end if( sDalNfc_Ctxt.hw_valid== TRUE)*/
        else 
        {
            /* TBD :Additional error code : NOT_INITIALISED */
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }

    }/*end if-Input parametrs valid-check*/
    else 
    {
        result = NFCSTATUS_INVALID_PARAMETER;
    }
    return result; 
}

/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Read

PURPOSE:  DAL Read  function.

-----------------------------------------------------------------------------*/

NFCSTATUS phDal4Nfc_Read( void *pContext, void *pHwRef,uint8_t *pBuffer, uint16_t length)
{
    BOOL retval,retval1;
    NFCSTATUS result = NFCSTATUS_SUCCESS;

    if ((NULL != pContext) && (NULL != pHwRef)&&
        (NULL != pBuffer) && (0 != length))
    {
        if ( sDalNfc_Ctxt.hw_valid== TRUE)
        {
            if((!gphDal4NfcRd_t.isReadBusy)&&
                (!gphDal4NfcRd_t.fWaitingOnRead))
            {
                DAL_DEBUG("*****DAl Read called  length : %d\n", length);


                /* Make a copy of the passed arguments */
                gphDal4NfcRd_t.plRdBuf = pBuffer;      
                gphDal4NfcRd_t.ReadLength  = length;
                /* Change the Read state so that thread can take over the read */
                gphDal4NfcRd_t.isReadBusy = TRUE;
                /* Just set variable here. This is the trigger for the Reader thread */
                gphDal4NfcRd_t.fWaitingOnRead = TRUE;
                retval = ResetEvent(gphDal4NfcRd_t.readcancel);
                /* Update the erro state */           
                result = NFCSTATUS_PENDING;

                retval1 = SetEvent(gphDal4NfcRd_t.readevthandle);

            }               
            else
            {
                /* Driver is BUSY with prev Read */
                DAL_PRINT("DAL BUSY\n");
                /* Make a copy of the passed arguments */
                gphDal4NfcRd_t.plRdBuf = pBuffer;      
                gphDal4NfcRd_t.ReadLength  = length;
                result = NFCSTATUS_PENDING;
                retval1 = SetEvent(gphDal4NfcRd_t.readevthandle);
            }
        } 
        else /*if( sDalNfc_Ctxt.hw_valid== TRUE)*/
        {
            /* TBD :Additional error code : NOT_INITIALISED */
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
    }/*end if-Input parametrs valid-check*/
    else 
    {
        result = NFCSTATUS_INVALID_PARAMETER;
    }
    DAL_DEBUG("*****DAl Read called  result : %x\n", result);
    return result;
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_ReadWait

PURPOSE:  DAL Read wait function.

-----------------------------------------------------------------------------*/

NFCSTATUS phDal4Nfc_ReadWait(void *pContext, void *pHwRef,uint8_t *pBuffer, uint16_t length)
{
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    if ((NULL != pContext) && (NULL != pHwRef)&&
        (NULL != pBuffer) && (0 != length))
    {
        if ( sDalNfc_Ctxt.hw_valid== TRUE)
        {
            if((!gphDal4NfcRdWr_t.isReadBusy)&&
                (!gphDal4NfcRdWr_t.isWaitReadBusy)&&
                (!gphDal4NfcRdWr_t.fWaitingOnRead))
            {
                /* Save the copy of passed arguments */
                gphDal4NfcRdWr_t.plWaitRdBuf = pBuffer;      
                gphDal4NfcRdWr_t.nWaitReadLenght = length;
                /* Change the Read state so that thread can take over the read */
                gphDal4NfcRdWr_t.isWaitReadBusy = TRUE;
                /* Just set variable here. This is the trigger for the Reader thread */
                gphDal4NfcRdWr_t.fWaitingOnWaitRead = TRUE;
                /* Update the error state */             
                result = NFCSTATUS_SUCCESS;
            }
            else
            {
                /* Driver is BUSY with prev Read */
                result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_BUSY);     
            }
        }/*end if( sDalNfc_Ctxt.hw_valid== TRUE)*/
        else 
        {
            /* TBD :Additional error code : NOT_INITIALISED */
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
    }/*end if -Input parametrs valid-check*/
    else 
    {
        result = NFCSTATUS_INVALID_PARAMETER;
    }
    return result;
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_ReadWaitCancel

PURPOSE: Cancel the Read wait function.

-----------------------------------------------------------------------------*/

NFCSTATUS phDal4Nfc_ReadWaitCancel( void *pContext, void *pHwRef)
{
    BOOL retval;
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    DAL_PRINT("DAl Read cancel called \n");
    if ((NULL != pContext) && (NULL != pHwRef))
    {
        if ( sDalNfc_Ctxt.hw_valid== TRUE)
        {
            /* Clear The Comm Port Event */
            if (!SetCommMask(h_serial_port, 0x0000))
            {
                /* Windows Error */
                result = NFCSTATUS_BOARD_COMMUNICATION_ERROR;
            }
            /* Cancel the Wait read */
            retval = SetEvent(gphDal4NfcRd_t.readcancel);
        }
        else 
        {
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
    }
    else 
    {
        result = NFCSTATUS_INVALID_PARAMETER;
    }
    return result;
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_Config

PURPOSE: Configure the serial port.

-----------------------------------------------------------------------------*/
NFCSTATUS phDal4Nfc_Config(pphDal4Nfc_sConfig_t config,void **phwref)
{
    static HANDLE                   serial_port=NULL;           /* COM port Handle */
    NFCSTATUS                       retstatus = NFCSTATUS_SUCCESS;
    BOOL                            retcheck=0;
    pphDal4Nfc_sConfig_t             pconfig = config;
    
    if((config != NULL)&&(phwref != NULL))
    {
        /* open communication port handle */
        serial_port = CreateFileA(pconfig->comPort, /*name of the object */
            (uint32_t)GENERIC_READ |(uint32_t)GENERIC_WRITE,/* for read/write access*/
            0,              /* Share mode */
            0,              /* Pointer to the security attribute */
            OPEN_EXISTING,  /*Opens the file. The function fails if the file does not exist*/
            FILE_FLAG_OVERLAPPED,/*multiple read/write operations pending on a device at a time are supported*/
            0);             /*this function does not copy the extended attributes to the new file*/
        if (serial_port == INVALID_HANDLE_VALUE)
        {       
            retcheck = CloseHandle(serial_port);
            *phwref = NULL;
            retstatus = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
        else
        {
            *phwref = serial_port;

        }
        /* Iniatilize the DAL context */
        (void)memset(&sDalNfc_Ctxt,0,sizeof(phDal4Nfc_SContext_t));
        psDalNfc_Ctxt = &sDalNfc_Ctxt;
        InitializeCriticalSection(&lpCriticalSection);
        /*com port is configured at the application level*/
        h_serial_port = serial_port;
        if (h_serial_port == INVALID_HANDLE_VALUE)
        {
            return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_INVALID_DEVICE);
        }
        /* Initialise all the internal DAL variables */

        /* Reset the Writer Thread values to NULL */
        (void)memset((void *)&gphDal4NfcRdWr_t,0,sizeof(gphDal4NfcRdWr_t));
        gphDal4NfcRdWr_t.fReadThreadDone     = TRUE;
        gphDal4NfcRdWr_t.fWriteThreadDone    = TRUE;

        /* Reset the Reader Thread values to NULL */
        (void)memset((void *)&gphDal4NfcRd_t,0,sizeof(gphDal4NfcRd_t));
        gphDal4NfcRd_t.fReadThreadDone     = TRUE;
        gphDal4NfcRd_t.fWriteThreadDone    = TRUE;

#if 0
        /* Read Thread */
        gphDal4NfcRd_t.fWaitingOnRead      = FALSE;
        gphDal4NfcRd_t.fWaitingOnWaitRead  = FALSE;
        gphDal4NfcRd_t.fCancelWaitRead     = FALSE;
        gphDal4NfcRd_t.fReadThreadDone     = TRUE;
        gphDal4NfcRd_t.h_readThread        = NULL;
        gphDal4NfcRd_t.isReadBusy          = FALSE;
        gphDal4NfcRd_t.isWaitReadBusy      = FALSE;
        gphDal4NfcRd_t.length              = 0x00;
        gphDal4NfcRd_t.nNoBytesRead        = 0x00;
        gphDal4NfcRd_t.nNoBytesWaitRead    = 0x00;
        gphDal4NfcRd_t.nWaitReadLenght     = 0x00;
        gphDal4NfcRd_t.plRdBuf = NULL;
        gphDal4NfcRd_t.plWaitRdBuf = NULL;
        gphDal4NfcRd_t.sReaderThreadID     = 0x00;
        gphDal4NfcRd_t.fWaitingOnWrite     = FALSE;
        gphDal4NfcRd_t.fWriteThreadDone    = TRUE;
        gphDal4NfcRd_t.h_writeThread       = NULL;
        gphDal4NfcRd_t.isWriteBusy         = FALSE;
        gphDal4NfcRd_t.ReadLength          = 0x00;
        gphDal4NfcRd_t.nNoBytesWritten     = 0x00;
        gphDal4NfcRd_t.plWrBuf = NULL;
        gphDal4NfcRd_t.sWriterThreadID     = 0x00;

#endif

        /* Configure the COM port */
        if(ERROR_SUCCESS != phDal4Nfc_SetUpSerialPort())
        {
            return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
        }
        /* Get The HAL threadID needed for Message posting */
#ifdef NXP_MESSAGING
        sHalThreadID =(DWORD) dwGetMsgThreadId;
#endif  
        gphDal4NfcRd_t.readevthandle = CreateEvent(  NULL,                 /* default security attributes*/
            FALSE,                 /* Auto-reset event*/
            FALSE,                /* initial state is nonsignaled*/
            NULL);                /* object name*/
        if (gphDal4NfcRd_t.readevthandle == NULL) 
        {
            return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);           /**Error Code To be Changed*/ 
        }
        gphDal4NfcRd_t.readcancel = CreateEvent(  NULL,                 /* default security attributes*/
            FALSE,                 /* Auto-reset event*/
            FALSE,                /* initial state is nonsignaled*/
            NULL);                /* object name*/
        if ( gphDal4NfcRd_t.readcancel == NULL) 
        {
            return PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);              /**Error Code To be Changed*/ 
        }
        gphDal4NfcRdWr_t.writeevthandle = CreateEvent(NULL,                 /* default security attributes*/
            FALSE,                 /* Auto-reset event*/
            FALSE,                /* initial state is nonsignaled*/
            NULL);                /* object name*/
        if (gphDal4NfcRdWr_t.writeevthandle == NULL)                         /** Error Code To be Changed*/ 
        {
            retstatus = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED)  ;
        }

        /* Start Read and Write Threads */
        if(NFCSTATUS_SUCCESS != phDal4Nfc_StartThreads())
        {
            retstatus =  PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
        }
        else
        {
            sDalNfc_Ctxt.hw_valid = TRUE; 

        }
#ifdef UART_I2C
        /*SC18Im700 Initialisation*/
        i2cbuf_temp_ptr = gpphDal4Nfc_WrBuf_I2c;
        *(gpphDal4Nfc_WrBuf_I2c ) = I2C_WRITE_INT_REG;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_BRG0;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_BRG0_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_BRG1;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_BRG1_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_PORTCONF1;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_PORTCONF1_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_PORTCONF2;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_PORTCONF2_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_IOSTATE;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_IOSTATE_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = 0x00;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_ADDR;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_ADDR_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_CLKL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_CLKL_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_CLKH;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_CLKH_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_REG_TIMEOUT;
        *(++gpphDal4Nfc_WrBuf_I2c ) = I2C_TIMEOUT_VAL;
        *(++gpphDal4Nfc_WrBuf_I2c) = I2C_STOP;
        gphDal4Nfc_Length_I2c = 21;
        gpphDal4Nfc_WrBuf_I2c = i2cbuf_temp_ptr;
        /* Issue a write command   */
        if (!WriteFile(h_serial_port,gpphDal4Nfc_WrBuf_I2c , gphDal4Nfc_Length_I2c,
            &gphDal4NfcRdWr_t.nNoBytesWritten, &gphDal4NfcRdWr_t.osWriter))
        {
            if (GetLastError() != ERROR_IO_PENDING)
            {
                /* Report write failure or timeout */
                result = PHNFCSTVAL(CID_NFC_DAL, 
                                NFCSTATUS_BOARD_COMMUNICATION_ERROR);
            }
        }
#endif
    }
    else
    {
        retstatus = NFCSTATUS_INVALID_PARAMETER;
    }

    return retstatus;
}

/*********************** Static Functions **************************/

/*-----------------------------------------------------------------------------

FUNCTION:  ReaderThread(LPVOID lpV)

PURPOSE: Continusly keep reedin the data and once done inform app through
callback

-----------------------------------------------------------------------------*/

static DWORD WINAPI phDal4Nfc_ReaderThread(void)
{
    DWORD     dwRes=0;                                      /* result from WaitForSingleObject */
    BOOL      retvalue;
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    BOOL       retcheck,retRead;
    uint8_t   retry_cnt=0;
    
    
    int   MsgType= PHDAL4NFC_READ_MESSAGE;
    int   *pmsgType=&MsgType;
    
    phDal4Nfc_Message_t      sMsg;
    phOsalNfc_Message_t      OsalMsg ;

#ifdef UART_I2C
    static uint8_t*   gpphDal4Nfc_WrBuf_I2c ;             /* Local Buffer pointer */
    static DWORD      gphDal4Nfc_Length_I2c = 0;          /* additional bytes(i2c) to be added to the buffer*/
    uint8_t*          i2cbuf_temp_ptr;
#endif      
        
    /* Create the overlapped event. Must be closed before exiting
    to avoid a handle leak. This event is used READ API and the Reader thread*/

    gphDal4NfcRd_t.osReader.hEvent= CreateEvent(NULL, TRUE, FALSE, NULL);
    if (gphDal4NfcRd_t.osReader.hEvent == NULL)
    {
        /* Error creating overlapped event; abort.*/
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED); 

    }
    retcheck = SetEvent(h_threadsEvent);

    /* Initialize the Event list */
    gphDal4NfcRd_t.EventList[0]=gphDal4NfcRd_t.osReader.hEvent;
    gphDal4NfcRd_t.EventList[1]=gphDal4NfcRd_t.readcancel;

    retRead=ResetEvent(gphDal4NfcRd_t.EventList[0]);
    retRead=ResetEvent(gphDal4NfcRd_t.EventList[1]);

    /* Get The ReaderThread threadID needed for Message posting */
    gphDal4NfcRd_t.sReaderThreadID = GetCurrentThreadId();
    DAL_PRINT("RX Thread \n");
    while(gphDal4NfcRd_t.fReadThreadDone) /* Thread Loop */
    {
        retvalue = 1;
        gphDal4NfcRd_t.fWaitingOnRead = ((gphDal4NfcRd_t.fWaitingOnRead == 1)?1:0);
        gphDal4NfcRd_t.fCancelWaitRead = 0;
        gphDal4NfcRd_t.fWaitingOnWaitRead = 0;
        
        dwRes=WaitForSingleObject(gphDal4NfcRd_t.readevthandle, INFINITE);
        retRead=ResetEvent(gphDal4NfcRd_t.readevthandle);
        
        /* Check for the read request from user */
        if (gphDal4NfcRd_t.fWaitingOnRead)
        {
#ifdef UART_I2C/* send number of bytes to be read*/
            i2cbuf_temp_ptr = gpphDal4Nfc_WrBuf_I2c;
            *(gpphDal4Nfc_WrBuf_I2c ) = I2C_START ;
            *(gpphDal4Nfc_WrBuf_I2c + 1) = I2C_SLAVEADDR | 0x01;/* slave address -For read*/
            *(gpphDal4Nfc_WrBuf_I2c + 2) = (uint8_t)gphDal4NfcRd_t.length;/*No. of bytes to be read*/
            *(gpphDal4Nfc_WrBuf_I2c + 3) = I2C_STOP;
            gpphDal4Nfc_WrBuf_I2c = i2cbuf_temp_ptr;
            gphDal4Nfc_Length_I2c = 4;

            /* Issue a wirte command to  */
            if (!WriteFile(h_serial_port,gpphDal4Nfc_WrBuf_I2c , gphDal4Nfc_Length_I2c,
                &gphDal4NfcRd_t.nNoBytesWritten, &gphDal4NfcRd_t.osWriter))
            {
                result = ((GetLastError() != ERROR_IO_PENDING)? 
                    PHNFCSTVAL(CID_NFC_DAL, 
                        NFCSTATUS_BOARD_COMMUNICATION_ERROR):
                    NFCSTATUS_SUCCESS);
            }
#endif/*UART_I2C*/
            /* Issue read operation.*/
            gphDal4NfcRd_t.nNoBytesRead=0;
            DAL_DEBUG("\n*New *** *****Request Length = %d",gphDal4NfcRd_t.ReadLength);
            (void)memset(gphDal4NfcRd_t.plRdBuf,0,gphDal4NfcRd_t.ReadLength);

            /* Wait for Write Completion */
        
    
            if (!ReadFile(h_serial_port, gphDal4NfcRd_t.plRdBuf,gphDal4NfcRd_t.ReadLength,
                &gphDal4NfcRd_t.nNoBytesRead, &gphDal4NfcRd_t.osReader))
            {
                if (GetLastError() != ERROR_IO_PENDING)
                {
                    /* TBD : Wondows Error, report it.*/
                    result = PHNFCSTVAL(CID_NFC_DAL, 
                                    NFCSTATUS_BOARD_COMMUNICATION_ERROR);
                }
                else
                {
                    DAL_PRINT("Dal Read File called Overlapped Wait \n");
                    
                    dwRes=WaitForMultipleObjects(2,gphDal4NfcRd_t.EventList,FALSE,INFINITE);
                    switch(dwRes)
                    {
                        /* Read completed.*/
                    case WAIT_OBJECT_0:
                        DAL_PRINT("Overlapped: Event Received from the device\n");
                        if (!GetOverlappedResult(h_serial_port, &gphDal4NfcRd_t.osReader,
                            &gphDal4NfcRd_t.nNoBytesRead, FALSE))
                        {
                            /* TBD : Wondows Error,  report it.*/
                            result=NFCSTATUS_READ_FAILED;
                        }
                        else
                        {
                            if(gphDal4NfcRd_t.nNoBytesRead >=8)
                            {
                                DAL_PRINT_BUFFER("ReadBuff:",gphDal4NfcRd_t.plRdBuf,gphDal4NfcRd_t.nNoBytesRead);

                            }
#ifndef SIMPLIED_DEFFEREDCALL
                            sMsg.eMsgType= PHDAL4NFC_READ_MESSAGE;

                            
                            phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                            
                            phDal4Nfc_DeferredCall((pphDal4Nfc_DeferFuncPointer_t)phDal4Nfc_DeferredCb,(void *)pmsgType);
                            (void)memset(&sMsg,0,sizeof(phDal4Nfc_Message_t));
                            (void)memset(&OsalMsg,0,sizeof(phOsalNfc_Message_t)); 
#endif

                        }
                        break;
                        /* Read Cancel */
                    case (WAIT_OBJECT_0+1):
                        retRead=ResetEvent(gphDal4NfcRd_t.readcancel);
                        gphDal4NfcRd_t.nNoBytesRead=0;
                        /* Clear Serial port Buffers */
                        retcheck = PurgeComm(h_serial_port, PURGE_FLAGS);
                        /* Update the state */
                        DAL_PRINT("Read Cancel\n");
                        break;
                    case WAIT_TIMEOUT:
                        retry_cnt++;
                        DAL_PRINT("Overlapped Read Result Time Out\n");
                        if(retry_cnt > 0)
                        {
                            /*  Reset flag so that another opertion can be issued.*/
                            gphDal4NfcRd_t.fWaitingOnRead = FALSE;
                            result = PHNFCSTVAL(CID_NFC_DAL, 
                                            NFCSTATUS_BOARD_COMMUNICATION_ERROR);
                        }
                        /* Operation isn't complete yet. fWaitingOnRead flag isn't
                        changed since I'll loop back around, and I don't want
                        to issue another read until the first one finishes.
                        This is a good time to do some background work.*/
                        break;                       
                    default:
                        retry_cnt++;
                        /* Error in the WaitForSingleObject; abort.
                        This indicates a problem with the OVERLAPPED structure's
                        event handle.*/
                        break;
                    }/*end of switch*/
                }/*end of else*/
            } /* End of File Read if */
            else
            {   
                /* read completed immediately */
                
                sMsg.eMsgType= PHDAL4NFC_READ_MESSAGE;
                /* Update the state */

                phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                retry_cnt = 0;
                phDal4Nfc_DeferredCall((pphDal4Nfc_DeferFuncPointer_t)phDal4Nfc_DeferredCb,(void *)pmsgType);
                                
            }
        }/* End of if (gphDal4NfcRdWr_t.fWaitingOnRead) */
        else if (gphDal4NfcRd_t.fWaitingOnWaitRead) /* Wait Read Loop */
        {
#ifdef UART_I2C/* send number of bytes to be read*/
            i2cbuf_temp_ptr = gpphDal4Nfc_WrBuf_I2c;
            *(gpphDal4Nfc_WrBuf_I2c ) = I2C_START ;
            *(gpphDal4Nfc_WrBuf_I2c + 1) = I2C_SLAVEADDR | 0x01;                /* slave address -For read*/
            *(gpphDal4Nfc_WrBuf_I2c + 2) = (uint8_t)gphDal4NfcRdWr_t.length;/*No. of bytes to be read*/
            *(gpphDal4Nfc_WrBuf_I2c + 3) = I2C_STOP;
            gpphDal4Nfc_WrBuf_I2c = i2cbuf_temp_ptr;
            gphDal4Nfc_Length_I2c = 4;
            /* Issue a write command to  */
            if (!WriteFile(h_serial_port,gpphDal4Nfc_WrBuf_I2c , gphDal4Nfc_Length_I2c,
                &gphDal4NfcRdWr_t.nNoBytesWritten, &gphDal4NfcRdWr_t.osWriter))
            {
                result = ((GetLastError() != ERROR_IO_PENDING)? 
                                    PHNFCSTVAL(CID_NFC_DAL, 
                                            NFCSTATUS_BOARD_COMMUNICATION_ERROR):
                                    NFCSTATUS_SUCCESS);
            }
#endif/*UART_I2C*/
        
            if (!ReadFile(h_serial_port, gphDal4NfcRd_t.plWaitRdBuf,
                gphDal4NfcRd_t.nWaitReadLenght, 
                &gphDal4NfcRd_t.nNoBytesWaitRead, &gphDal4NfcRd_t.osReader)) 
            {
                if (GetLastError() != ERROR_IO_PENDING)     
                {
                    /*  TBD : Wondows Error; report it.*/
                    result = PHNFCSTVAL(CID_NFC_DAL,
                                NFCSTATUS_BOARD_COMMUNICATION_ERROR);
                }
                else
                {
                    dwRes = WaitForSingleObject(gphDal4NfcRd_t.osReader.hEvent, READ_TIMEOUT);
                    switch(dwRes)
                    {
                        /* Read completed.*/
                    case WAIT_OBJECT_0:
                        if (!GetOverlappedResult(h_serial_port, &gphDal4NfcRd_t.osReader, 
                            &gphDal4NfcRd_t.nNoBytesWaitRead, FALSE))
                        {
                            /*  TBD : Wondows Error; report it.*/
                            result = PHNFCSTVAL(CID_NFC_DAL, 
                                        NFCSTATUS_BOARD_COMMUNICATION_ERROR);
                        }
                        else
                        {
                            sMsg.eMsgType= PHDAL4NFC_READWAIT_MESSAGE;

                            
                            phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                            gphDal4NfcRd_t.nNoBytesRead=0;
                            gphDal4NfcRd_t.ReadLength=0;
                            phDal4Nfc_DeferredCall((pphDal4Nfc_DeferFuncPointer_t)phDal4Nfc_DeferredCb,(void *)pmsgType);
                            result = (NFCSTATUS)((retvalue == 0)? 
                                            PHNFCSTVAL(CID_NFC_DAL, 
                                                    NFCSTATUS_BOARD_COMMUNICATION_ERROR):
                                            result);
                            /* Update the state */
                            
                        }
                        break;
                    case WAIT_TIMEOUT:
                        gphDal4NfcRd_t.isWaitReadBusy = FALSE;
                        /*  Reset flag so that another opertion can be issued.*/
                        gphDal4NfcRd_t.fWaitingOnWaitRead = FALSE;

                        retry_cnt++;
                        if(retry_cnt > 5)
                        {
                            /*  Reset flag so that another opertion can be issued.*/
                            gphDal4NfcRd_t.fWaitingOnRead = FALSE;
                            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_BOARD_COMMUNICATION_ERROR);

                        }
                        /* Operation isn't complete yet. fWaitingOnRead flag isn't
                        changed since I'll loop back around, and I don't want
                        to issue another read until the first one finishes.
                        This is a good time to do some background work.*/
                        break;                       
                    default:
                        /* Error in the WaitForSingleObject; abort.
                        This indicates a problem with the OVERLAPPED structure's
                        event handle.*/
                        retry_cnt++;
                        break;
                    } /* End of Switch */
                }/* end of Else */
            }/* RedFile*/
            else
            {
                sMsg.eMsgType= PHDAL4NFC_READWAIT_MESSAGE;
                phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                retry_cnt=0;
                /* Update the state */
                gphDal4NfcRd_t.isReadBusy = FALSE;
                /*  Reset flag so that another opertion can be issued.*/
                gphDal4NfcRd_t.fWaitingOnRead = FALSE;
                gphDal4NfcRd_t.nNoBytesRead=0;
                gphDal4NfcRd_t.ReadLength=0;
                phDal4Nfc_DeferredCall((pphDal4Nfc_DeferFuncPointer_t)
                                            phDal4Nfc_DeferredCb,(void *)pmsgType);
              
                /* Update the state */
                gphDal4NfcRd_t.isWaitReadBusy = FALSE;
                /*  Reset flag so that another opertion can be issued.*/
                gphDal4NfcRd_t.fWaitingOnWaitRead = FALSE;
            }/*end-else*/

        } /* End of else if (gphDal4NfcRdWr_t.fWaitingOnWaitRead) */
        else if (gphDal4NfcRd_t.fCancelWaitRead)
        {

            DAL_PRINT("Read Wait is Canceled\n");
            
        }
        else if ( result != NFCSTATUS_SUCCESS )
        {
            /* Report it to user */
            sMsg.transactInfo.status  = result;
            /*map to OSAL msg format*/  
            OsalMsg.eMsgType = PH_DAL4NFC_MESSAGE_BASE;
            OsalMsg.pMsgData = (void*)&sMsg;
            /* Update the state */
            gphDal4NfcRd_t.nNoBytesRead=0;
            gphDal4NfcRd_t.ReadLength=0;
            phDal4Nfc_DeferredCall((pphDal4Nfc_DeferFuncPointer_t)
                                phDal4Nfc_DeferredCb,(void *)pmsgType);
            
        } /*end-else if ( result != NFCSTATUS_SUCCESS )*/
        else
        {
            continue;
        }

    } /* End of thread Loop*/
    /* Close the reader event since job is done */
    retcheck = CloseHandle(gphDal4NfcRd_t.osReader.hEvent);
    return TRUE;
}

/*-----------------------------------------------------------------------------

FUNCTION:  phDal4Nfc_WriterThread(LPVOID lpV)

PURPOSE: Continusly keep reedin the data and once done inform app through
callback

-----------------------------------------------------------------------------*/
static DWORD WINAPI phDal4Nfc_WriterThread(void)
{
    BOOL            retvalue = 1;
    DWORD           dwRes=0; /* result from WaitForSingleObject */
    NFCSTATUS       result = NFCSTATUS_SUCCESS;
    BOOL            retcheck = 1;
    BOOL            retWrite;
    phDal4Nfc_Message_t      sMsg;
    phOsalNfc_Message_t      OsalMsg ;
    int     MsgType=PHDAL4NFC_WRITE_MESSAGE;
    int * pmsgType=&MsgType;


#ifdef UART_I2C
    static uint8_t* gpphDal4Nfc_WrBuf_I2c ;     /* Local Buffer pointer */
    static DWORD    gphDal4Nfc_Length_I2c = 0;  /* additional bytes(i2c) to be added to the buffer*/
    uint8_t*        i2cbuf_temp_ptr = NULL;
    uint8_t         i2cbuf[BUFSIZE_I2C] = {0},i = 0;
#endif
    

    /* Create the overlapped event. Must be closed before exiting
    to avoid a handle leak. This event is used READ API and the Reader thread*/
    gphDal4NfcRdWr_t.osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (gphDal4NfcRdWr_t.osWriter.hEvent == NULL)
    {
        /* TBD : Windows error ?? Error creating overlapped event; abort.*/
        result =  PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED); 
    }
    /* Get The WriterThread threadID needed for Message posting */
    gphDal4NfcRdWr_t.sWriterThreadID = GetCurrentThreadId();
    /* Set the thread started Event, to make sure that both threads are ready
    in the initialisation state*/
    while(gphDal4NfcRdWr_t.fWriteThreadDone) /* Thread Loop */
    {

        retvalue = 1;
        DAL_PRINT("TX Started\n");
        dwRes = WaitForSingleObject(gphDal4NfcRdWr_t.writeevthandle, INFINITE);
        retWrite = ResetEvent(gphDal4NfcRdWr_t.writeevthandle);
        /* Check for the write request from user */
        if(gphDal4NfcRdWr_t.fWaitingOnWrite)
        {

#ifdef UART_I2C
            i2cbuf_temp_ptr = (uint8_t*)gphDal4NfcRdWr_t.plWrBuf;
            i2cbuf[0] = I2C_START;
            i2cbuf[1] = I2C_SLAVEADDR;
            i2cbuf[2] = (uint8_t)gphDal4NfcRdWr_t.length;
            i2cbuf[2+i2cbuf[2]+1] = I2C_STOP;
            for(i=1;i<=(uint8_t)gphDal4NfcRdWr_t.length;i++)
            {
                i2cbuf[2 + i] = *(i2cbuf_temp_ptr++);
            }
            gphDal4Nfc_Length_I2c = i2cbuf[2] + 4 ;
            gpphDal4Nfc_WrBuf_I2c = &i2cbuf;
            gphDal4NfcRdWr_t.plWrBuf =  &i2cbuf;
            gphDal4NfcRdWr_t.length  = gphDal4Nfc_Length_I2c;
#endif
            /* Issue a write */
            gphDal4NfcRdWr_t.nNoBytesWritten=0;
            
            if (!WriteFile(h_serial_port,gphDal4NfcRdWr_t.plWrBuf ,gphDal4NfcRdWr_t.length,
                &gphDal4NfcRdWr_t.nNoBytesWritten, &gphDal4NfcRdWr_t.osWriter))
            {
                /* Check the write status  */
                if (GetLastError() != ERROR_IO_PENDING)
                {
                    /* Report write failure or timeout */
                    result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_BOARD_COMMUNICATION_ERROR);
                }
                else
                {
                    /* Write is pending.GetOverlappedResult is similar to WaitForSingleObject */
                    DAL_PRINT("Waiting the Write Overlepped Result\n");
                    if (!GetOverlappedResult(h_serial_port, &gphDal4NfcRdWr_t.osWriter,
                        &gphDal4NfcRdWr_t.nNoBytesWritten, TRUE))
                    {
                        /* TBD : Windows Error */
                        DAL_PRINT("***** INVALID ERROR CODE\n");
                        result =  PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
                    }
                    else
                    {
                        /* Write operation completed successfully. Populalate the 
                        message and inform the User*/
                        sMsg.eMsgType= PHDAL4NFC_WRITE_MESSAGE;
                        sMsg.pContext= psDalNfc_Ctxt->cb_if.pif_ctxt;
                        phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                        /* Post a message to the HAL Thread */
                        /* Update the state */
                        gphDal4NfcRdWr_t.isWriteBusy = FALSE;
                        /*  Reset flag so that another opertion can be issued.*/
                        gphDal4NfcRdWr_t.fWaitingOnWrite = FALSE;
                        phDal4Nfc_DeferredCall(phDal4Nfc_DeferredCb,pmsgType);
                        DAL_DEBUG("DAl Overlapped Writer thread called  length : %d\n", 
                                                gphDal4NfcRdWr_t.nNoBytesWritten);
                    }
                }
            }/*end writeFile*/
            else /* Results ready */
            {
                
                /* Write operation completed successfully.*/
                sMsg.eMsgType = PHDAL4NFC_WRITE_MESSAGE;
                sMsg.pContext = psDalNfc_Ctxt;
                phDal4Nfc_FillMsg(&sMsg,&OsalMsg);
                /* Post a message to the HAL Thread */
                /* Update the state */
                gphDal4NfcRdWr_t.isWriteBusy = FALSE;
                /*  Reset flag so that another opertion can be issued.*/
                gphDal4NfcRdWr_t.fWaitingOnWrite = FALSE;

                gphDal4NfcRdWr_t.nNoBytesWritten=0;
                phDal4Nfc_DeferredCall(phDal4Nfc_DeferredCb,pmsgType);  
                DAL_DEBUG("NON Overlapped Write :DAl Writer thread called  length : %d\n",
                                            gphDal4NfcRdWr_t.nNoBytesWritten);
                
            }/*end else  Results ready */
        }/*end of if(gphDal4NfcRdWr_t.fWaitingOnWrite)*/
        else if ( result != NFCSTATUS_SUCCESS )
        {
            
            /* Report it to user */
            sMsg.transactInfo.status  = result;
            /*map to OSAL msg format*/    
            OsalMsg.eMsgType = PH_DAL4NFC_MESSAGE_BASE;
            OsalMsg.pMsgData = (void*)&sMsg;
            /* Update the state */
            gphDal4NfcRdWr_t.isWriteBusy = FALSE;
            /*  Reset flag so that another opertion can be issued.*/
            gphDal4NfcRdWr_t.fWaitingOnWrite = FALSE;
            gphDal4NfcRdWr_t.nNoBytesWritten = 0;

            DAL_DEBUG("DAl Writer thread error called  length : %d\n", 
                                gphDal4NfcRdWr_t.nNoBytesWritten);

        }/*end elseif*/
        else
        {
            continue;
        }
        retWrite=ResetEvent(gphDal4NfcRdWr_t.writeevthandle);

    }/* End of thread Loop*/
    /*Close the reader evet at thead exit */
    retcheck = CloseHandle(gphDal4NfcRdWr_t.osWriter.hEvent);
    return TRUE;
}

/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_StartThreads

PURPOSE: Creates the Reader/Status and Writer threads

-----------------------------------------------------------------------------*/
static NFCSTATUS phDal4Nfc_StartThreads(void)
{
    DWORD dwReadStatId;
    DWORD dwWriterId;
    DWORD dwEventRes =WAIT_FAILED;
    NFCSTATUS result = NFCSTATUS_SUCCESS;
    BOOL retcheck, retThrdPrio;
    /* Create an Event to see if both the threads have started. This evet is 
    set in the second started Thread, to make sure that both threads are ready */
    h_threadsEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(h_threadsEvent==NULL)
    {
        result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
    }
    /* Start the Reader Thread First */ 
    if(result == NFCSTATUS_SUCCESS)
    {
        gphDal4NfcRdWr_t.h_writeThread = CreateThread( NULL, /* default security attributes*/
            0,    /*use default stack size  */
            (LPTHREAD_START_ROUTINE) phDal4Nfc_WriterThread, /*thread function name*/
            (LPVOID) NULL, /*argument to thread function */
            0,    /*use default creation flags */
            &dwWriterId );/*returns the thread identifier */

        retThrdPrio=SetThreadPriority(gphDal4NfcRdWr_t.h_writeThread,THREAD_PRIORITY_HIGHEST);
        if (gphDal4NfcRdWr_t.h_writeThread == NULL)
        {
            /* To avoid Handle Leak */
            retcheck = CloseHandle(h_threadsEvent);
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
        }
    }
    /* Start the reader Thread */
    if(result == NFCSTATUS_SUCCESS)
    {
        gphDal4NfcRd_t.h_readThread = CreateThread( NULL, /* default security attributes*/
            0,    /*use default stack size  */
            (LPTHREAD_START_ROUTINE)phDal4Nfc_ReaderThread,/*thread function name*/
            (LPVOID) NULL, /*argument to thread function */
            0,    /*use default creation flags */
            &dwReadStatId); /*returns the thread identifier */

        retThrdPrio=SetThreadPriority(gphDal4NfcRd_t.h_readThread,THREAD_PRIORITY_TIME_CRITICAL);

        if (gphDal4NfcRd_t.h_readThread== NULL)
        {
            /* To avoid Handle Leak */
            retcheck = CloseHandle(h_threadsEvent);
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
        }
    }/*end if-starter thread*/

    /* Wait for Starting Event */
    retcheck = ResetEvent(h_threadsEvent);
    dwEventRes = WaitForSingleObject(h_threadsEvent,100);
    /* Reset Event after thread startup */
    retcheck = ResetEvent(h_threadsEvent);
    /* To avoid Handle Leak */
    if(result == NFCSTATUS_SUCCESS)
    {
        if(CloseHandle(h_threadsEvent) == 0)   
        {
            /*Thread Closing isn't successed.*/
            result = PHNFCSTVAL(CID_NFC_DAL, NFCSTATUS_FAILED);
        }  
    }
    return result;
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_SetUpSerialPort

PURPOSE: Setups the serial port.

-----------------------------------------------------------------------------*/
static DWORD phDal4Nfc_SetUpSerialPort(void)
{
    DCB             dcb1;
    BOOL retcheck ;
    /* TimeoutsDefault: We need ReadIntervalTimeout here to cause
    the read operations that we do to actually timeout and become
    overlapped. Specifying 1 here causes ReadFile to return very
    quickly so that our reader thread will continue execution.*/

    COMMTIMEOUTS gTimeoutsDefault;
    memset (&gTimeoutsDefault, 0, sizeof (COMMTIMEOUTS));
    gTimeoutsDefault.ReadIntervalTimeout = 100;
    SetLastError(ERROR_SUCCESS);

    /*Get the current comm settings */
    dcb1.DCBlength = sizeof (dcb1);
    retcheck = GetCommState(h_serial_port, &dcb1);
    if(!retcheck)
    {
        printf("GetCommStateFailed \n", GetLastError());
    }
    dcb1.Parity = NOPARITY;
    dcb1.StopBits = ONESTOPBIT;
    dcb1.BaudRate = CBR_115200;
    dcb1.ByteSize = 8;
    dcb1.fBinary = TRUE;
    dcb1.fDtrControl = DTR_CONTROL_DISABLE;
    dcb1.fRtsControl = RTS_CONTROL_DISABLE;
    /* Apply the settings */
    retcheck = SetCommState(h_serial_port, &dcb1);
    if(!retcheck)
    {
        DAL_PRINT("SetCommStateFailed \n");
    }
    /* Set proper Timeouts */
    retcheck = SetCommTimeouts(h_serial_port, &gTimeoutsDefault);
    if(!retcheck)
    {
        DAL_PRINT("SetCommTimeoutsFailed \n");
    }
    /* set comm buffer sizes 2048 ,2048*/
    retcheck = SetupComm(h_serial_port, MAX_READ_BUFFER, MAX_WRITE_BUFFER);
    if(!retcheck)
    {
        DAL_PRINT("SetupCommFailed \n");
    };
#if 0
    /* Specify a set of events to be monitored for a communications device */
    retcheck = SetCommMask(h_serial_port,EV_TXEMPTY);
    if(!retcheck)
    {
        DAL_PRINT("SetCommMaskFailed \n");
    }
#endif
    return GetLastError();
}

/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_FillMsg

PURPOSE: Fill the Dal Message structure.

-----------------------------------------------------------------------------*/
static void phDal4Nfc_FillMsg(phDal4Nfc_Message_t *pDalMsg,phOsalNfc_Message_t *pOsalMsg)
{
  if(NULL != DalNfc_HwRef)
  {
    if(pDalMsg->eMsgType == PHDAL4NFC_WRITE_MESSAGE)
    {
        pDalMsg->transactInfo.length  = (uint8_t)gphDal4NfcRdWr_t.nNoBytesWritten;
        pDalMsg->transactInfo.buffer = NULL;
        pDalMsg->transactInfo.status  = NFCSTATUS_SUCCESS;
        pDalMsg->pHwRef  = DalNfc_HwRef;
        pDalMsg->writeCbPtr = psDalNfc_Ctxt->cb_if.send_complete;
        pOsalMsg->eMsgType = PH_DAL4NFC_MESSAGE_BASE;
        pOsalMsg->pMsgData = pDalMsg;
        return;
    }
    else if(pDalMsg->eMsgType == PHDAL4NFC_READ_MESSAGE)
    {
        pDalMsg->transactInfo.length  = (uint8_t)gphDal4NfcRd_t.nNoBytesRead;
        pDalMsg->transactInfo.buffer = gphDal4NfcRd_t.plRdBuf;
        pDalMsg->pContext= psDalNfc_Ctxt->cb_if.pif_ctxt;
    }
    else/* (sMsg.eMsgType == PHDAL4NFC_READWAIT_MESSAGE)*/
    {
        pDalMsg->transactInfo.length  = (uint8_t)gphDal4NfcRdWr_t.nNoBytesWaitRead;
        pDalMsg->transactInfo.buffer = gphDal4NfcRdWr_t.plWaitRdBuf;
        pDalMsg->pContext= psDalNfc_Ctxt;
    }
    pDalMsg->transactInfo.status  = NFCSTATUS_SUCCESS;
    pDalMsg->pHwRef  = DalNfc_HwRef;
    pDalMsg->readCbPtr = psDalNfc_Ctxt->cb_if.receive_complete;    
    /*map to OSAL msg format*/  
    pOsalMsg->eMsgType = PH_DAL4NFC_MESSAGE_BASE;
    pOsalMsg->pMsgData = pDalMsg;
  }
  
}
/*-----------------------------------------------------------------------------

FUNCTION: phDal4Nfc_PostMessage

PURPOSE: Generic post message function.

-----------------------------------------------------------------------------*/
static BOOL phDal4Nfc_PostMessage(DWORD  ThreadID,phDal4Nfc_Message_t *pDalMsg,phOsalNfc_Message_t *pOsalMsg)
{
    BOOL retvalue = 1;
    phDal4Nfc_DeferMsg_t*     defer_call=NULL;  
    EnterCriticalSection(&lpCriticalSection);         
#if 0

    retvalue = PostThreadMessage((DWORD)sHalThreadID,
        (UINT)(PHOSALNFC_MESSAGE_BASE),
        (WPARAM)pOsalMsg,
        (LPARAM)ThreadID);

#endif
    defer_call=(phDal4Nfc_DeferMsg_t*)phOsalNfc_GetMemory(sizeof(phDal4Nfc_DeferMsg_t));
    defer_call->params = (void*)phOsalNfc_GetMemory(sizeof(phDal4Nfc_Message_t));
    (void)memcpy(defer_call->params,pDalMsg,sizeof(phDal4Nfc_Message_t));

    LeaveCriticalSection(&lpCriticalSection);

    return retvalue;
}


static void phDal4Nfc_DeferredCb (void  *params) 
{
    int*    pParam=NULL;
    phNfc_sTransactionInfo_t TrasactionInfo={0};
        
    pParam=(int*)params;
    
    WaitForSingleObject(h_threadsEvent, 10);
    switch(*pParam)
    {
        case PHDAL4NFC_READ_MESSAGE: 
            DAL_PRINT(" Dal deferred read called \n");
            TrasactionInfo.buffer=gphDal4NfcRd_t.plRdBuf;
            TrasactionInfo.length=(uint16_t)gphDal4NfcRd_t.nNoBytesRead;
            TrasactionInfo.status=NFCSTATUS_SUCCESS;
            gphDal4NfcRd_t.isReadBusy = FALSE;
            /*  Reset flag so that another opertion can be issued.*/
            gphDal4NfcRd_t.fWaitingOnRead = FALSE;
            if ((NULL != psDalNfc_Ctxt) && (NULL != psDalNfc_Ctxt->cb_if.receive_complete))
            {
                psDalNfc_Ctxt->cb_if.receive_complete(psDalNfc_Ctxt->cb_if.pif_ctxt,
                                                        DalNfc_HwRef,&TrasactionInfo);
            }
            
            break;
        case PHDAL4NFC_READWAIT_MESSAGE:
            /*dalMsg->readCbPtr(dalMsg->pContext, dalMsg->pHwRef, &dalMsg->transactInfo);*/
            break;
        case PHDAL4NFC_WRITE_MESSAGE:
            DAL_PRINT(" Dal deferred write called \n");
            /* DAL_DEBUG("dalMsg->transactInfo.length : %d\n", dalMsg->transactInfo.length); */
        
            TrasactionInfo.buffer=NULL;
            TrasactionInfo.length=(uint16_t)gphDal4NfcRdWr_t.nNoBytesWritten;
            TrasactionInfo.status=NFCSTATUS_SUCCESS;
            if ((NULL != psDalNfc_Ctxt) && (NULL != psDalNfc_Ctxt->cb_if.send_complete))
            {
                psDalNfc_Ctxt->cb_if.send_complete(psDalNfc_Ctxt->cb_if.pif_ctxt,
                                                    DalNfc_HwRef,&TrasactionInfo);
            }
            break;
        default:
            break;
        }

}


#ifdef NXP_MESSAGING
void phDal4Nfc_DeferredCall(pphDal4Nfc_DeferFuncPointer_t func, void *param) 
{
    uint16_t                retvalue = 0;
    
      EnterCriticalSection(&lpCriticalSection);
     
    if(PHDAL4NFC_READ_MESSAGE==(* (int*)param))
    {
        sReadDeferedCalldInfo.def_call=func;
        sReadDeferedCalldInfo.params=param;
        retvalue = (uint16_t)PostThreadMessage((DWORD)sHalThreadID,
        (UINT)(PHOSALNFC_MESSAGE_BASE),
        (WPARAM)&sReadDeferedCalldInfo,
        (LPARAM)gphDal4NfcRdWr_t.sReaderThreadID);
    }
    else
    {
        sWriteDeferedCalldInfo.def_call=func;
        sWriteDeferedCalldInfo.params=param;
        retvalue = (uint16_t)PostThreadMessage((DWORD)sHalThreadID,
        (UINT)(PHOSALNFC_MESSAGE_BASE),
        (WPARAM)&sWriteDeferedCalldInfo,
        (LPARAM)gphDal4NfcRdWr_t.sWriterThreadID);

    }
    LeaveCriticalSection(&lpCriticalSection);

    /*memset(&gphDal4Nfc_PostOsalMsg,0,sizeof(phOsalNfc_Message_t));
    gphDal4Nfc_PostOsalMsg.eMsgType = PH_DAL4NFC_MESSAGE_BASE;
    gphDal4Nfc_PostOsalMsg.pMsgData = (void*)&sReadDeferedCalldInfo;
    */
}
#endif


/****************************************** END OF FILE ***************************************************/

