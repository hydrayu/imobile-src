#include "time.h"
#include <stdio.h>
#include "stdlib.h"
#include <string>
#include "iMobile_RFID.h"
#include <Windows.h>
using namespace std;

/* Global variables */
char c;
NFCSTATUS Status = NFCSTATUS_SUCCESS;

// needed by DAL 
extern "C"
{
	DWORD    dwGetMsgThreadId;      /* Thread ID */
}

phDal4Nfc_sConfig_t Dal_Config;
void * gInterface;
uint32_t ThreadMain_ID = 0;
pphOsalNfc_Message_t pMsg;
BOOL     EventCallBack = TRUE;
BOOL     Init_Successfull = FALSE;
BOOL	 DeInit_Done = FALSE;
uint8_t				 Mode;
phHal_sHwReference_t psHwRef = {0};
phHal_sDeviceCapabilities_t   psDevCap = {1};


ph_Tag_Info_t	 *ReadTag_Info;
// FRI Contexts
uint32_t Init_context;
uint32_t Temp_context;
extern uint8_t  TagType;

static char comPort[10]={"COM3"};

uint16_t				Ioctl_Mode;
phNfc_sData_t			InParam;
phNfc_sData_t			Outparam;
uint8_t					Input_Buff[50];
uint8_t					Output_Buff[50];
int						length;

// EEDATA Settings
phNfc_sData_t      InputParam;
phNfc_sData_t      OutputParam;
uint8_t			   IoctlRespBuffer[1];

// startup mode,
// 0 for no-console(default)
// 1 for console,
// 2 for minimize
int startup_mode = 2;

/* Internal functions */
NFCSTATUS PortComConfig(const char *Com);
void Exit(void);

/* FRI Callbacks & application functions */
static void InitResponseCallBack(void *context, NFCSTATUS status);
static void DeInitResponseCallBack(void *context, NFCSTATUS status);
void GetBuildVersion();

void SetColor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

/* Send a message */
void PostMessage(uint32_t SourceID, uint32_t DestID, phOsalNfc_Message_t * pMsg)  
{
	uint16_t	retvalue=0;

    retvalue = PostThreadMessage((DWORD)DestID,
								(UINT)(PH_OSALNFC_MESSAGE_BASE),
								(WPARAM)pMsg,
								(LPARAM)SourceID);
	
    return;
}

/* Receive a message */
NFCSTATUS ReceiveMsg(uint32_t *pSourceID, phOsalNfc_Message_t* pMsg)
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
    mms=memset(pMsg, 0x0, sizeof(*pMsg));

    if (ret == 0   ||  NULL    ==  (void*)msg.wParam)
    {
        return PHNFCSTVAL(CID_NFC_NONE, NFCSTATUS_INSUFFICIENT_RESOURCES);
    }

    *pSourceID  =   (uint32_t)(msg.lParam);
    *pMsg    =   *(phOsalNfc_Message_t *)msg.wParam;

    return NFCSTATUS_SUCCESS;
}

NFCSTATUS Init_HAL_Open()
{
	/* Init HAL */
	APP_PRINT("\n-------------------------------------------\n         INITIALIZE HAL STACK  \n-------------------------------------------\n");
	
	pMsg->eMsgType = HAL_OPEN;
	PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
	/* Wait for Event */
	Status = WaitForEvent();
	
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	}  
	
	return Status;
}

NFCSTATUS GET_CAPABILITIES()
{
	APP_PRINT("\n-------------------------------------------\n         GET CAPABILITIES  \n-------------------------------------------");

	pMsg->eMsgType = HAL_GET_CAPABILITIES;
	PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
	/* Wait for Event */
	Status = WaitForEvent();
	APP_PRINT("\n-------------------------------------------\n         GET CAPABILITIES  \n-------------------------------------------");
	
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	}  
	
	return Status;
}

NFCSTATUS SignalStartApplication()
{
	APP_PRINT("\n-------------------------------------------\n         START APPLICATION \n-------------------------------------------");
	
	/* Notification Register */
	pMsg->eMsgType = HAL_START_APPLICATION;
	PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
	/* Wait for Event */
	Status = WaitForEvent();
	
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	}  
	
	return Status;
}

int main(int argc, char* argv[], char* envp[])
{   
	bool InitalStatus=false;

	char* comenv;
	if ((comenv = getenv("SERIAL_RFID")) != NULL) {
		strcpy_s(comPort, 10, comenv);
	}

	// -d to startup with debug console
	if (argc > 1) {
		for (int i=1; i<=argc-1; ++i) {
			if (strcmp("-h", argv[i]) == 0) {
				printf ("Usage: %s [Port] [-h] [-d] [-s] [-c config_port_number]\n", argv[0]);
				return 0;
			}
			else if (strcmp("-d", argv[i]) == 0)
				startup_mode = 1;
			else if (strcmp ("-m", argv[i]) == 0)
				startup_mode = 2;
			else if (strcmp ("-s", argv[i]) == 0)
				startup_mode = 0;
			else if (strcmp ("-c", argv[i]) == 0)
			{
				char filename[256];
				if ((i+1) < argc) {
					sprintf(filename, "%s", argv[i+1]);
				}
				FILE *fp = fopen(filename, "r");
				if (fp != NULL) {
					fscanf_s(fp, "%s", comPort);
				}
				++i;
			}
			else {
				int com;
				if (sscanf(argv[i], "COM%d", &com) == 0)
					sscanf (argv[i], "%d", &com);
				printf ("assign COM Port '%d'\n", com);
				if (com >= 1 && com <= 15) {
					memset(comPort, 0, 10);
					sprintf_s(comPort, 10, "COM%d", com);
					startup_mode = 2;
				}
			}
		}
	}
	// default without debug console
	else startup_mode = 2;

	if (strnlen_s(comPort, 10) > 0 && (Status = PortComConfig(comPort)) == NFCSTATUS_SUCCESS) {
		printf ("Connect to : %s\n", comPort);
	}
	else {
		for (int i=15; i>=1; --i) {
			sprintf_s(comPort, 10, "COM%d", i);
			if ((Status = PortComConfig(comPort)) == NFCSTATUS_SUCCESS) {
				printf ("[Connect to : %s\n", comPort);
				break;
			}
		}
	}
       
	if (Status == NFCSTATUS_SUCCESS)
	{
		if (startup_mode == 0) ShowWindow(GetConsoleWindow(), SW_HIDE);
		else if (startup_mode == 2) ShowWindow(GetConsoleWindow(), SW_MINIMIZE);

		// Opening a session with the NFC device.
        Init_HAL_Open();
		
		if(Init_Successfull)
		{
			// Start Application and run polling loop to get Tag status 
			SignalStartApplication();
			while(1)
			{
				// Get Tag UID  
				Get_Tag_ID(ReadTag_Info);

				//if( TagType == TAG_MIFARE_UL )
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = MIFARE UL <<<<<<<<<<<\n");	
				//	SetColor(7);
				//}
				//else if(TagType == TAG_MIFARE_1K	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = MIFARE 1K <<<<<<<<<<<\n");
				//	SetColor(7);
				//}
				//else if(TagType == TAG_ISO15693	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = ISO15693 <<<<<<<<<<<\n");
				//	SetColor(7);
				//}
				//else if(TagType == TAG_JEWEL	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = TAG JEWEL <<<<<<<<<<<\n");
				//	SetColor(7);
				//	Jewel_Read_Complete();
				//}
				//else if(TagType == TAG_FELICA	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = TAG FELICA <<<<<<<<<<<\n");
				//	SetColor(7);
				//	Felica_Raw_Command();
				//}
				//else if(TagType == TAG_TYPE_B	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = TAG TYPE B <<<<<<<<<<<\n");
				//	SetColor(7);
				//	TypeB_Calypso_Exchange_Apdu();
				//}
				//else if(TagType == TAG_MIFARE_DESFIRE	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = TAG MIFARE DESFIRE <<<<<<<<<<<\n");
				//	SetColor(7);
				//	TypeA_Desfire_Exchange_Apdu();
				//}
				//else if(TagType == TAG_TYPE_A 	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = TAG TYPE A <<<<<<<<<<<\n");
				//	SetColor(7);
				//	TypeA_Exchange_Apdu();
				//}
				//else if(TagType == TAG_MIFARE_4K	)
				//{
				//	SetColor(14);	
				//	printf("\n>>>>>>>>> TAG = MIFARE 4K <<<<<<<<<<<\n");
				//	SetColor(7);
				//}
				// Sometimes the client may need to check whether the tag is still present in the field or not 
				// and ending a session with connected tag
				// wait RFID close to retrieve .... wade
				PresenceCheckFunction(&Temp_context);
			} // while(1)
			//Closing a session with the NFC device but here not use the function
			Exit();
		} //Init_Successfull
	}
	if (startup_mode == 1) {
		printf ("<<< Press Enter to exit >>>");
		unsigned char c=getchar();
	}
	return 1;
}

void StartApplication()
{
	APP_PRINT("\n-------------------------------------------\n         NOTIFICATION REGISTER  \n-------------------------------------------");
            
	/* Notification Register */
	pMsg->eMsgType = HAL_NOTIFICATION_REGISTER;
	PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
	/* Wait for Event */
	Status = WaitForEvent();
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	}  
	APP_PRINT("\n-------------------------------------------\n CONFIGURE POLLING LOOP  \n-------------------------------------------\n");
            
	/* Config & Start Polling Loop */
	pMsg->eMsgType = HAL_CONFIG_DISCOVERY;
	PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
	/* Wait for Event */
	Status = WaitForEvent();
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	}  
	APP_PRINT("-------------------------------------------\n");
						
	/* Wait for Event */
	Status = WaitForEvent();
	if ((Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING))
	{
		Exit();
	} 
}

NFCSTATUS WaitForEvent()
{
	NFCSTATUS Status = NFCSTATUS_SUCCESS;
	phOsalNfc_Message_t      msg;
	uint32_t Source_ID = 0;  
	EventCallBack = TRUE;
	do
	{
		Status = ReceiveMsg (&Source_ID, &msg);
		Sleep(2);

		if ( Status == NFCSTATUS_SUCCESS)
		{
			/* Message from Main */
			if ( Source_ID == ThreadMain_ID)
			{
				/* Check the message Type */
				switch(msg.eMsgType)
				{
				case HAL_OPEN:
					Status =phHal4Nfc_Open(&psHwRef,eInitDefault,InitResponseCallBack,&Init_context);
					if (Status == NFCSTATUS_PENDING)
					{
						APP_PRINT("Init in progress ");
					}
					else
					{
						APP_PRINT("\n> HAL stack initialization ERROR (status=%x)  \n",Status);
						/* to exit the while loop */
						EventCallBack = 0;
					}
					break;
				case HAL_GET_CAPABILITIES:
					Status = phHal4Nfc_GetDeviceCapabilities(&psHwRef,&psDevCap,&Init_context); 
					
					EventCallBack = 0;
					break;
				case HAL_CLOSE:
					Status =phHal4Nfc_Close(&psHwRef,DeInitResponseCallBack,&Init_context);
					if (Status == NFCSTATUS_PENDING)
					{
						APP_PRINT("DeInit in progress ");
					}
					else
					{
						APP_PRINT("\n> HAL stack DeInitialization ERROR (status=%x)  \n",Status);
						
						/* to exit the while loop */
						EventCallBack = 0;
					}
					break;
				case HAL_NOTIFICATION_REGISTER:
					Status = NotificationRegisterPollingLoop(&Init_context);
					break;
				case HAL_START_APPLICATION:
					StartApplication();
					break;
				case HAL_CONFIG_DISCOVERY:
					Status = ConfigStartPollingLoop(&Init_context);
					if (Status == NFCSTATUS_PENDING)
					{
					       APP_PRINT(".");
					}
					else
					{
						APP_PRINT("\n> Configure PN544 discovery ERROR  \n");
						
						EventCallBack = 0;
					}	
					break;
				/* timer mngt */	
				case PH_OSALNFC_TIMER_MSG:
                    phOsalNfc_DeferedCalldInfo_t * pOsalDeferCallInfo;
                    pOsalDeferCallInfo=(phOsalNfc_DeferedCalldInfo_t*)(msg.pMsgData);
                    if (NULL != pOsalDeferCallInfo->pDeferedCall)
                    {
                        pOsalDeferCallInfo->pDeferedCall(pOsalDeferCallInfo->pParam);
                    }
                    else
                    {
                        APP_PRINT("\n> PH_OSALNFC_TIMER_MSG ERROR \n");
                    }
                    break;
				default:
					APP_PRINT("\n Unknown event : %d\n", msg.eMsgType);
					
					EventCallBack = 0;
					break;
				}
			}
			else
			{
				phDal4Nfc_DeferMsg_t    *defer_msg = NULL;
				defer_msg = (phDal4Nfc_DeferMsg_t*)(&msg);
				/* to show serial link activity */
				APP_PRINT(".");
				
				Sleep(5);
				defer_msg->def_call(defer_msg->params);
			}
		}
		else
		{
			APP_PRINT("\n WaitForEvent phOsalNfc_ReceiveMsg ERROR \n");
			/* to exit the while loop */
			EventCallBack = 0;
		}
	}while(EventCallBack);

	return Status;
}

void InitResponseCallBack(void *context, NFCSTATUS status)
{
	if ( status == NFCSTATUS_SUCCESS )
	{
	    APP_PRINT("\n> Stack Initialization successfull !\n");
		
		Init_Successfull = TRUE;
		DeInit_Done = FALSE;
		EventCallBack = FALSE;
	}
	else
	{
		Init_Successfull = FALSE;
		APP_PRINT("\n> Stack initialization ERROR\n");
	}
}

 void DeInitResponseCallBack(void *context, NFCSTATUS status)
{
	if ( status == NFCSTATUS_SUCCESS )
	{
		APP_PRINT("\n> Stack deinitialization successful\n");
		
		EventCallBack = FALSE;
		DeInit_Done = TRUE;
	}
	else
	{
		APP_PRINT("\n> Stack deinitialization ERROR\n");
	}
}

void Exit(void)
{
	NFCSTATUS Status = NFCSTATUS_SUCCESS;
	int c;
	if (Init_Successfull == TRUE)/* DEINIT  */
	{
		APP_PRINT("\n-------------------------------------------\n DEINITIALIZED HAL STACK \n-------------------------------------------\n");
              
		pMsg->eMsgType = HAL_CLOSE;
		PostMessage(ThreadMain_ID, ThreadMain_ID, pMsg);
		/* Wait for Event */
		Status = WaitForEvent();
	}
	if ( (Status != NFCSTATUS_SUCCESS ) && (Status !=NFCSTATUS_PENDING) && (DeInit_Done == FALSE)) 
	{
		Exit();
	}
	
#ifdef Debug_Print
	APP_PRINT("\n PRESS Enter TO EXIT APPLICATION");
	fflush(stdin);
	c = getchar();
#endif
	exit(1);  
}

NFCSTATUS PortComConfig(const char *Com)
{
	/* Get Main Task thread Id */
	ThreadMain_ID = GetCurrentThreadId();
	/* To share HAL threadID at driver level */
	dwGetMsgThreadId = ThreadMain_ID;

	Dal_Config.comPort = (char *) comPort;
	
	Status = phDal4Nfc_Config(&Dal_Config, &psHwRef.p_board_driver);
	if (Status != NFCSTATUS_SUCCESS)
	{
		APP_PRINT(">  Host Link Configuration ERROR \n--> Please verify your Serial (%s) interface setup ! \n", Dal_Config.comPort);
		return Status;
	}
	APP_PRINT("> Host link configuration successful  \n");
	APP_PRINT("> Please connect board to %s  \n", Dal_Config.comPort);

	/* Allocate memory for message exchange */
	pMsg = (phOsalNfc_Message_t*)malloc(sizeof(phOsalNfc_Message_t));
	pMsg->Size = NULL;
	pMsg->pMsgData = NULL;

	return Status;
}

void GetBuildVersion_CallBack(void* context, phNfc_sData_t* Outparam_Cb, NFCSTATUS status)
{
	if ( status == NFCSTATUS_SUCCESS )
	{
		APP_PRINT("\n NFC GetBuildVersion_CallBack SUCCESS !");
		APP_PRINT("\n FW VERSION BUILD%d", OutputParam.buffer[0]); 
	
		EventCallBack = 0;
	}
	else
	{
		APP_PRINT("NFC GetBuildVersion_CallBack KO !");
	}
}

void GetBuildVersion()
{
	NFCSTATUS Status;
	uint8_t EEDATA_BuildVersion[] = {0x00,0x9E,0xB1};
	/* Get Build Version */
	InputParam.buffer = EEDATA_BuildVersion;
	InputParam.length = 0x03;
	OutputParam.buffer = IoctlRespBuffer;
	OutputParam.length = 1;
	Status = phHal4Nfc_Ioctl(&psHwRef, NFC_MEM_READ,&InputParam, &OutputParam,GetBuildVersion_CallBack, &Init_context);
	APP_PRINT("\n-------------------------------------------\n         GET BUILD VERSION  \n-------------------------------------------\n");
	
	if(Status != NFCSTATUS_PENDING )
	{
	       APP_PRINT("NFC GetBuildVersion ERROR\n");
	}
	else
	{
		APP_PRINT("NFC GetBuildVersion pending\n");
		
		WaitForEvent();
	}
}



