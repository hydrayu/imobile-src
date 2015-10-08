#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "iMobile_RFID.h"
#include <mmsystem.h>

phHal_sSupProtocol_t          reginfo;  			

phHal_sDeviceCapabilities_t   Hal4Nfc_DevCaps;
phHal_sADD_Cfg_t              HalADDCfg;
phHal_sADD_Cfg_t			  sADDConfig; 	

uint32_t					  Ndef_context,Localcontext;

uint8_t						  TagType;
phNfc_sData_t		          ndef_message;


uint8_t						  TestOnGoing;
uint8_t						  TagRemoved = FALSE;

extern pphOsalNfc_Message_t		  pMsg;
extern BOOL     EventCallBack; 
extern uint32_t ThreadMain_ID;
extern BOOL     EventCallBack;

uint8_t	 Read_Buff[500];
uint8_t  Block_Buffer[50];
// NDEF Parsing variables
uint8_t		IsChunked[1024] = {0},
			*RawRecords[1024] = {0},
			Buffer[1024] = {0};
uint32_t	NumberOfRawRecords=0;

// Type B Buffer
uint8_t SelectApplication_CD_LIGTH[14] = {0x94,0xA4,0x04,0x00,0x08,0x31,0x54,0x49,0x43,0x2E,0x49,0x43,0x41,0x36};

// Type A Desfire Buffer
uint8_t GetApplicationId  = 0x6A;

uint8_t SelectCardManager [13] = {0x00, 0xa4, 0x04, 0x00, 0x07, 0xa0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00};

uint8_t FelicaRawCmd [16]={0x10,0xF0,0x00,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x11,0x22,0x33,0x44,0x55,0x66,0x77};

/**** for Jewel Raw Command *******/
typedef enum phTst_eJewelCmdList
{
	phTst_eJewel_ReadAll        = 0x00U, /**< Jewel command:\n
										 - This command sends the data buffer directly 
										 to the remote device */
	phTst_eJewel_Read1          = 0x01U, /**< Read one byte. */
	phTst_eJewel_Read8          = 0x02U, /**< Read four bytes. */
	phTst_eJewel_Read4          = 0x03U, /**< Read eight bytes. */
	phTst_eJewel_Write1E        = 0x53U, /**< Erase and Write one byte */
	phTst_eJewel_Write8E        = 0x54U, /**< Erase and Write four bytes */
	phTst_eJewel_Write4E        = 0x55U, /**< Erase and Write eight bytes */
	phTst_eJewel_Write1NE       = 0x1AU, /**< Write one byte Without Erase */
	phTst_eJewel_Write8NE       = 0x1BU, /**< Write four bytes Without Erase */
	phTst_eJewel_Write4NE       = 0x1CU, /**< Write eight bytes Without Erase */
	phTst_eJewel_RID            = 0x78U, /**< Read the identification information */
	phTst_eJewel_ReadSeg        = 0x10U, /**< Read the data segment */
	phTst_eJewel_Read           = 0x21U, /**< Read the data from the specified block  */
	phTst_eJewel_Write          = 0x22U, /**< Write the data to specified block  */
	phTst_eJewel_Invalid        = 0xFFU  /**< Invalid jewel command */
}phTst_eJewelCmdList_t;


/**********transceive var***********/
phHal_sTransceiveInfo_t				Transceive_Param;
ph_Tag_Info_t						Tag_Info;
phHal4Nfc_TransactInfo_t			TransferInfo;

uint8_t								Transceive_Opt_Mifare=0;
uint8_t					            Transceive_Opt_Jewel=0;
uint8_t					            Transceive_Opt_TypeB=0;
uint8_t					            Transceive_Opt_TypeA=0;
uint8_t					            Transceive_Opt_Felica=0;
uint8_t								Transceive_Opt_15693=0;
uint8_t								Block_NO=0;
uint8_t								Write_Buffer[300];                                                           
uint8_t								Read_Buffer[300];

phHal4Nfc_NotificationInfo_t global_info;
uint16_t DeviceIndex = 0;
uint8_t MultipleProtocolTag = 0;

/* Internal Function */ 
void TagInfo(phHal_sRemoteDevInformation *TagInfo);

void Print_Remote_Dev_Info(phHal_sRemoteDevInformation_t *Rem_Dev_Info);

int Transceive_Config(uint8_t nb_block);

void Mifare_Read_Block(uint8_t nb_block);
void Mifare_Write_Block(uint8_t nb_block);
void Mifare_Authenticate(uint8_t sector_nb);	
void Mifare_1K_Read_Sector(uint8_t index_Sector);

void GenerateKey2(int vk)
{
	KEYBDINPUT kb = {0};
	INPUT Input = {0};
	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));

	::ZeroMemory(&kb, sizeof(KEYBDINPUT));
	::ZeroMemory(&Input, sizeof(INPUT));
	kb.dwFlags = KEYEVENTF_KEYUP;
	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));
}

void GenerateKey(BYTE vk, BYTE scan)
{
	keybd_event(vk, scan, 0, 0);
	keybd_event(vk, scan, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
}

void sendToKeyboard(UINT64 UID)
{
	char msg[256];
	unsigned int i;

	memset(msg, 0, 256);
	sprintf (msg, "%u", UID);

	for (i=0; i<strlen(msg) && i<256; i++) {
		//GenerateKey2(msg[i]);
		GenerateKey(msg[i], 0);
	}
	// Send a Return to Keyboard...
	//GenerateKey2(VK_RETURN);
	GenerateKey(VK_RETURN, 0);
}

void  Get_Tag_ID(ph_Tag_Info_t *TagInfo )
{
	switch(Tag_Info.Target_Type)
	{
		case phHal_eISO14443_A_PICC:
		case phHal_eMifare_PICC:
		case phHal_eISO14443_B_PICC:
	    case phHal_eJewel_PICC:
	    case phHal_eFelica_PICC:
		case phHal_eISO15693_PICC:
			UINT64 UID;
			UID = 0;
			for(int i=Tag_Info.Target_UidLength-1; i>=0; --i)
			{
				UID <<= 8;
				UID |= Tag_Info.Target_Uid[i];
			}
			printf("%u",UID);
			sendToKeyboard(UID);
			// SystemXXX: Asterisk, Default, Exclamation, Exit, Hand, Question, Start, Welcome
			// PlaySound(TEXT("SystemAsterisk"), NULL, SND_ALIAS); 
			// SND_ALIAS_SYSTEMXXXX: ASTERISK, DEFAULT, EXCLAMATION, EXIT, HAND, QUESTION, START, WELCOME
			PlaySound((LPCTSTR)SND_ALIAS_SYSTEMDEFAULT, NULL, SND_ALIAS_ID);
			break;
		default:
			printf("XXXX UNKNOWN Tag Type XXXX");
			break;
	}
	printf ("\n> ");
}	

void  Mifare_UL_ReadSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer)
{
	uint8_t Temp;
	APP_PRINT("\n>>>>>>>>> READ MIFARE UL <<<<<<<<<<<\n");
	
	if(index_Sector < 4)
	{
		Mifare_Read_Block(index_Sector<<2);
		MifareRead_Buffer = Read_Buffer;
		APP_PRINT("\n");
		for(Temp=0; Temp<16;Temp++)
		{
			APP_PRINT("0x%02X ",MifareRead_Buffer [Temp]);
		}	
	}
}
void  Mifare_UL_WriteBlock(uint8_t index_Block, uint8_t  *MifareRead_Buffer)
{
	uint8_t Temp;
	APP_PRINT("\n>>>>>>>>> WRITE  MIFARE UL <<<<<<<<<<<\n");
	
	if((index_Block >=4) && (index_Block <16))
	{
		for(Temp=0;Temp<4;Temp++)
			Block_Buffer[Temp]=MifareRead_Buffer[Temp];
		Mifare_Write_Block(index_Block);
	}
}

void Mifare_1K_ReadSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer)
{
	uint8_t index_Block = 4*index_Sector,i=0,Temp;
	APP_PRINT("\n***** SECTOR %d AUTHENTICATION *****\n",index_Sector);
	
	Mifare_Authenticate(index_Block+1);
	for(i=0;i<4;i++)
	{
		Mifare_Read_Block(index_Block);
		MifareRead_Buffer = Read_Buffer;
		APP_PRINT("\n");
		for(Temp=0; Temp<16;Temp++)
		{
			APP_PRINT("0x%02X ",MifareRead_Buffer [Temp]);
		}
		index_Block++;
	}	
}

// write blocks 0,1,2 of one sector (block 3 is sector trailer) except block 0 of sector 0
void Mifare_1K_WriteSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer)
{
	uint8_t index_Block = 4*index_Sector,i=0,Temp;
	APP_PRINT("\n***** SECTOR %d AUTHENTICATION *****\n",index_Sector);
	
	Mifare_Authenticate(index_Block+1);
	
	for(i=1;i<4;i++)
	{
		if (index_Sector > 0) // this function doesn't allow to change block 0 sector 0
		{
			for(Temp=0; Temp<16;Temp++)
			{
				Block_Buffer[Temp]=MifareRead_Buffer[Temp+(i-1)*16];
			}
			Mifare_Write_Block(index_Block);
		}	
		index_Block++;
	}
}

void SetTagEnable()
{
	#ifdef MifareUL_CardEnable
	reginfo.MifareUL =TRUE;
	#else
	reginfo.MifareUL =FALSE;
	#endif
	
	#ifdef MifareStd_CardEnable
	reginfo.MifareStd =TRUE;
	#else
	reginfo.MifareStd =FALSE;
	#endif

	#ifdef ISO14443_4A_CardEnable
	reginfo.ISO14443_4A=TRUE;
	#else
	reginfo.ISO14443_4A=FALSE;
	#endif
	
	#ifdef ISO14443_4B_CardEnable
	reginfo.ISO14443_4B =TRUE;
	#else
	reginfo.ISO14443_4B=FALSE;
	#endif

	#ifdef Jewel_CardEnable
	reginfo.Jewel=TRUE;
	#else
	reginfo.Jewel=FALSE;
	#endif

	#ifdef NFC_CardEnable
	reginfo.NFC=TRUE;
	#else
	reginfo.NFC=FALSE;
	#endif

	#ifdef Felica_CardEnable
	reginfo.Felica=TRUE;
	#else
	reginfo.Felica=FALSE;
	#endif

	#ifdef ISO15693_CardEnable
	reginfo.ISO15693=TRUE;	
	#else
	reginfo.ISO15693=FALSE;
	#endif

}


NFCSTATUS NotificationRegisterPollingLoop(void *context)
{
	NFCSTATUS Status = NFCSTATUS_SUCCESS;

	/* Tag Notification registration */
	APP_PRINT("\n-------------------------------------------\n         TAG NOTIFICATION REGISTER  \n-------------------------------------------\n");
	
	SetTagEnable();

	Status =phHal4Nfc_RegisterNotification(&psHwRef,
		eRegisterTagDiscovery, (pphHal4Nfc_Notification_t)TagRegisterNotificationCB, &context);

	if (Status == NFCSTATUS_SUCCESS)
	{
		APP_PRINT("> DONE\n ");
	}
	else
	{
		APP_PRINT("\n> Tag Notification Register  ERROR\n");
		PresenceCheckFunction(&context);
	}
		
	EventCallBack = 0;
	return Status;
}

NFCSTATUS ConfigStartPollingLoop(void *context)
{
	NFCSTATUS Status = NFCSTATUS_SUCCESS;

	sADDConfig.PollDevInfo.PollEnabled = TRUE;
		
	#ifdef ISO14443_4A_CardEnable	
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso14443A = TRUE;
	#else
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso14443A = FALSE;
	#endif
	
	#ifdef ISO14443_4B_CardEnable
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso14443B = TRUE;
	#else
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso14443B = FALSE;
	#endif

	#ifdef Felica_CardEnable
	sADDConfig.PollDevInfo.PollCfgInfo.EnableFelica212 = TRUE;
	#else
	sADDConfig.PollDevInfo.PollCfgInfo.EnableFelica212 = FALSE;
	#endif

	#ifdef NFC_CardEnable
	sADDConfig.PollDevInfo.PollCfgInfo.EnableNfcActive = TRUE;
	#else
	sADDConfig.PollDevInfo.PollCfgInfo.EnableNfcActive = FALSE;
	#endif

	#ifdef ISO15693_CardEnable
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso15693 = TRUE;
	#else
	sADDConfig.PollDevInfo.PollCfgInfo.EnableIso15693 = FALSE;
	#endif

	Status = phHal4Nfc_ConfigureDiscovery(&psHwRef, NFC_DISCOVERY_CONFIG,&sADDConfig, ConfigDiscoveryCB, &context);	

	return Status;
}


/*****************************/
/*    Discovery Callback     */
/*****************************/
void ConfigDiscoveryCB(void *context,NFCSTATUS status)
{
	if ( status == NFCSTATUS_SUCCESS )
	{
		APP_PRINT("\n>  pooling loop configuration done !\n");
		printf("> ");
		EventCallBack = FALSE;
	}
	else
	{
		APP_PRINT("\n>  pooling loop configuration ERROR  \n");
		PresenceCheckFunction(&context);
	}
}

/*************************************/
/*    Tag Notification  Callback     */
/*************************************/
void TagRegisterNotificationCB (void  *context, 
								phHal_eNotificationType_t    type, 
								phHal4Nfc_NotificationInfo_t info, 
								NFCSTATUS status)
{  
	NFCSTATUS Status = NFCSTATUS_SUCCESS;
	// temp under construction _ variable globale info
	global_info = info;
	DeviceIndex = 0;
	if ( status == NFCSTATUS_SUCCESS || status == NFCSTATUS_MULTIPLE_PROTOCOLS)
	{
		APP_PRINT("\n>>>>>>>>> A TAG HAS BEEN DETECTED <<<<<<<<<<<\n");
			
		EventCallBack = 0;

		if(status == NFCSTATUS_MULTIPLE_PROTOCOLS)
		{
			APP_PRINT("A Tag with Multiple protocols has been detected(e.g. SMX)\n");
			APP_PRINT("\n Number of Remote Device (or multiple protocols) = %d\n",info.psDiscoveryInfo->NumberOfDevices);
				
			// in this example a card Mifare+ISO14443-4 card is used, the ISO14443-4 is stored at offset [1] of the table	
			// we choose here to connect to the ISO14443-4 part
			MultipleProtocolTag = 1;
			Status = phHal4Nfc_Connect(&psHwRef,info.psDiscoveryInfo->ppRemoteDevInfo[1],TagConnect_ResponseCB,&context);
		}
		else
		{
			MultipleProtocolTag = 0;
			Status = phHal4Nfc_Connect(&psHwRef,info.psDiscoveryInfo->ppRemoteDevInfo[0],TagConnect_ResponseCB,&context);
		}
		while (DeviceIndex < info.psDiscoveryInfo->NumberOfDevices)
		{
			Print_Remote_Dev_Info(info.psDiscoveryInfo->ppRemoteDevInfo[DeviceIndex]);
			Tag_Info.Target_Type = info.psDiscoveryInfo->ppRemoteDevInfo[DeviceIndex]->RemDevType;
			DeviceIndex++;
		}

		//TagInfo(&Tag_RemDevInfo_CB[0]);
						
		TagInfo(*(info.psDiscoveryInfo->ppRemoteDevInfo));
			
		if (Status != NFCSTATUS_PENDING)
		{
			SetColor(12);
			APP_PRINT("\n> Tag Connect ERROR  \n");
			SetColor(7);
			PresenceCheckFunction(&context);
		}
		else
		{
			// Wait for Event 
			WaitForEvent();
		}
	}
}

/*******************************************/
/*    Tag information  Callbacks           */
/*******************************************/
void TagInfo(phHal_sRemoteDevInformation_t *TagInfo)
{
	uint8_t i;
	
	SetColor(14);
	APP_PRINT("\n>>>>>>>>> TAG INFO: ");
	
	switch(Tag_Info.Target_Type)
	{
	case phHal_eISO14443_A_PICC:
		APP_PRINT("TYPE ISO14443_4A <<<<<<<<<<<\n");
		SetColor(7);
				
		Tag_Info.Target_SAK = TagInfo[0].RemoteDevInfo.Iso14443A_Info.Sak;
		
		Tag_Info.Target_UidLength = TagInfo[0].RemoteDevInfo.Iso14443A_Info.UidLength;
		for(i=0;i<Tag_Info.Target_UidLength;i++)
			Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Iso14443A_Info.Uid[i];
		
		if(Tag_Info.Target_SAK  == 0x20)
			TagType = TAG_MIFARE_DESFIRE;
		else
			TagType = TAG_TYPE_A;
		break;

	case phHal_eMifare_PICC:
		APP_PRINT("TYPE MIFARE <<<<<<<<<<<\n");
		SetColor(7);
		
		Tag_Info.Target_SAK = TagInfo[0].RemoteDevInfo.Iso14443A_Info.Sak;
		Tag_Info.Target_UidLength = TagInfo[0].RemoteDevInfo.Iso14443A_Info.UidLength;

		for(i=0;i<Tag_Info.Target_UidLength;i++)
		   Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Iso14443A_Info.Uid[i];
		
		if(Tag_Info.Target_SAK  == 0x08)
			TagType = TAG_MIFARE_1K;
		else if(Tag_Info.Target_SAK  == 0x18)
			TagType = TAG_MIFARE_4K;
		else if(Tag_Info.Target_SAK  == 0x00)
			TagType = TAG_MIFARE_UL;
		break;

	case phHal_eISO14443_B_PICC:
		APP_PRINT("TYPE ISO14443_4B <<<<<<<<<<<\n");
		SetColor(7);
		TagType = TAG_TYPE_B;
		Tag_Info.Target_UidLength = PHHAL_PUPI_LENGTH;
		for(i=0;i<Tag_Info.Target_UidLength;i++)
			Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Iso14443B_Info.AtqB.AtqResInfo.Pupi[i];
		TagType = TAG_TYPE_B;
		break;

	case phHal_eJewel_PICC:
		APP_PRINT("TYPE JEWEL <<<<<<<<<<<\n");
		SetColor(7);
		
		TagType = TAG_JEWEL;
        Tag_Info.Target_UidLength = TagInfo[0].RemoteDevInfo.Jewel_Info.UidLength;
		for(i=0;i<Tag_Info.Target_UidLength;i++)
			Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Jewel_Info.Uid[i];
		break;

	case phHal_eFelica_PICC:
		APP_PRINT("TYPE FELICA <<<<<<<<<<<\n");
		SetColor(7);
		TagType = TAG_FELICA;
		Tag_Info.Target_UidLength = TagInfo[0].RemoteDevInfo.Felica_Info.IDmLength;
		for(i=0;i<Tag_Info.Target_UidLength;i++)
			Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Felica_Info.IDm[i];
		break;
	case phHal_eISO15693_PICC:
		APP_PRINT("TYPE ISO15693 <<<<<<<<<<<\n");
		SetColor(7);
		
		TagType = TAG_ISO15693;
		Tag_Info.Target_UidLength = TagInfo[0].RemoteDevInfo.Iso15693_Info.UidLength;
		for(i=0;i<Tag_Info.Target_UidLength;i++)
		    Tag_Info.Target_Uid[i] = TagInfo[0].RemoteDevInfo.Iso15693_Info.Uid[i];
		break;

	default:
		APP_PRINT("UNKNOWN TYPE <<<<<<<<<<<\n");
		break;
	}
}

void Print_Remote_Dev_Info(phHal_sRemoteDevInformation_t *Rem_Dev_Info)
{
	APP_PRINT("\n Remote device type detected : ");
	
	// to be completed : display remote device information (uid, sak, etc)
	switch(Rem_Dev_Info->RemDevType)
	{
	case phHal_eMifare_PICC : 
		APP_PRINT("\n>>>>> Mifare_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_A_PICC :
		APP_PRINT("\n>>>>> ISO14443_A_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_4A_PICC:
		APP_PRINT("\n>>>>> ISO14443_4A_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_3A_PICC:
		APP_PRINT("\n>>>>> ISO14443_3A_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_B_PICC:
		APP_PRINT("\n>>>>> ISO14443_B_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_4B_PICC:
		APP_PRINT("\n>>>>> ISO14443_4B_PICC <<<<<<\n");
		break;
	case phHal_eISO14443_BPrime_PICC:
		APP_PRINT("\n>>>>> ISO14443_BPrime_PICC <<<<<<\n");
		break;
	case phHal_eFelica_PICC:
		APP_PRINT("\n>>>>> Felica_PICC <<<<<<\n");
		break;
	case phHal_eJewel_PICC:
		APP_PRINT("\n>>>>> Jewel_PICC <<<<<<\n");
		break;
	case phHal_eISO15693_PICC:
		APP_PRINT("\n>>>>> ISO15693_PICC <<<<<<\n");
		break;
	default:
		break;
	}
}

/*******************************************/
/*    Tag Connect/Disconnect  Callbacks     */
/*******************************************/
void TagConnect_ResponseCB( void  *context,
						   phHal_sRemoteDevInformation_t *rem_dev_info,
						   NFCSTATUS status)
{
	if ( status == NFCSTATUS_SUCCESS )
	{	
		APP_PRINT("\n>>>>>>>>> TAG CONNECTED <<<<<<<<<<<\n");
		EventCallBack = 0;
	}
	else
	{
		SetColor(12);
		APP_PRINT("\n> Tag connection ERROR  \n");
		SetColor(7);
		PresenceCheckFunction(&context);
	}
}

void TagDisconnect_ResponseCB(void*	pContext,phHal_sRemoteDevInformation_t *rem_dev_info ,
							  NFCSTATUS status
)
{
	if ( status == NFCSTATUS_SUCCESS )
	{
		// printf("\n>>>>>>>>> TAG DISCONNECTED <<<<<<<<<<<\n");
		
		switch(TagType)
		{
			case TAG_TYPE_A:
				Transceive_Opt_TypeA=0;
				break;
			case TAG_TYPE_B:
				Transceive_Opt_TypeB=0;
				break;
			case TAG_FELICA:
				Transceive_Opt_Felica=0;
				break;
			case TAG_JEWEL:
				Transceive_Opt_Jewel = 0;
				break;
			case TAG_MIFARE_1K:
				Transceive_Opt_Mifare=0;
				break;
			case TAG_MIFARE_DESFIRE:
				Transceive_Opt_Mifare=0;
				break;
			case TAG_MIFARE_UL:
				Transceive_Opt_Mifare=0;
				break;
			case TAG_ISO15693:
				Transceive_Opt_15693=0;
				break;
			case TAG_MIFARE_4K:
				Transceive_Opt_Mifare=0;
				break;
			default:
				break;
		}
	}
	else
	{
		APP_PRINT("\n> Tag Disconnect failed \n");
		
		SetColor(14);
		APP_PRINT("\n**************   END TEST    **************\n");
		SetColor(7);
		
		PresenceCheckFunction(&pContext);
	}
}

/*******************************************/
/*       Presence Check Callback           */
/*******************************************/
void PresenceCheckFunction(void *context)
{	
	NFCSTATUS Status = NFCSTATUS_SUCCESS;

	APP_PRINT("> Please Remove the TAG from the RF field\n");

	while(!TagRemoved)
	{
		Status = phHal4Nfc_PresenceCheck(&psHwRef,Presence_CheckCB,&context);                      

		if ( Status != NFCSTATUS_PENDING)
		{
			APP_PRINT("\n> Presence Check Error  \n");
			TagRemoved = TRUE;
		}
		else
		{
			/* Wait for Event */
			WaitForEvent();
		}
	}

	TagRemoved = FALSE;
	/* Disconnect the TAG */
	Status = phHal4Nfc_Disconnect(&psHwRef,
		global_info.psDiscoveryInfo->ppRemoteDevInfo[MultipleProtocolTag],
		NFC_DISCOVERY_CONTINUE,
		TagDisconnect_ResponseCB,&context); 

	if ( Status != NFCSTATUS_PENDING)
	{
		SetColor(12);
		APP_PRINT("\n> Presence Check Error  \n");
		SetColor(7);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

void Presence_CheckCB(void *context, NFCSTATUS status)
{
	if(status!=NFCSTATUS_SUCCESS)
	{
		APP_PRINT("\n>>>>>>>>> TAG REMOVED <<<<<<<<<<<\n");
		TagRemoved = TRUE;
		EventCallBack = FALSE;
	}
	else
	{
		EventCallBack = FALSE;
	}
}

void TagTranceiveCB(void* pContext,phHal_sRemoteDevInformation_t *rem_dev_info,phNfc_sData_t* pResBuffer,NFCSTATUS Status)
{
	uint8_t i;
	if(Status==NFCSTATUS_SUCCESS)
	{
		switch(rem_dev_info->RemDevType)
		{
		case phHal_eMifare_PICC:
			switch(Transceive_Opt_Mifare)
			{
			case Authentication:
				APP_PRINT("\n>>>>>>>>> SECTOR AUTHENTICATION OK <<<<<<<<<<<\n");
				
				break;
			case Mifare_Read:
				for(i=0; i<pResBuffer->length;i++)
				{
					Read_Buffer[i] = pResBuffer->buffer[i];
				}	
					
				break;
			case Mifare_Write:
				APP_PRINT("\n>>>>>>>>> WRITE OK <<<<<<<<<<<\n");
				
				break;
			}
			break;
		case phHal_eJewel_PICC:
			switch(Transceive_Opt_Jewel)
			{
			case Read_All:
				APP_PRINT("\n>>>>>>>>> READ ALL OK <<<<<<<<<<<\n");
				
				break;
			case Read_ID:
				APP_PRINT("\n>>>>>>>>> READ ID OK <<<<<<<<<<<\n");
				
				break;
			 }
			break;
		case phHal_eISO14443_B_PICC:
			switch(Transceive_Opt_TypeB)
			{
			case Select_Application:
				if((pResBuffer->buffer[pResBuffer->length-2]== 0x90 && pResBuffer->buffer[pResBuffer->length-1]== 0x00)
					|| pResBuffer->buffer[pResBuffer->length-2]== 0x61)
				{
					APP_PRINT("\n>>>>>>>>> SELECT CD LIGTH APPPLICATION OK <<<<<<<<<<<\n");
					APP_PRINT("Response APDU: ");
					
					for(i=0; i<pResBuffer->length;i++)
						APP_PRINT("0x%02X ",pResBuffer->buffer[i]);
				}
				else 
					// 61xx = normal processing, SW2 indicates the number of remaining databytes
					// 62xx to 6Fxx = error codes see ISO7816 spec for details
					if (pResBuffer->buffer[pResBuffer->length-2]>= 0x62)
					{
						// the test is considered has success as soon as the card responded
						// (even if the CD light application is not on the card)
						APP_PRINT("\n>>>>>>>>> SELECT CD LIGTH APPPLICATION ERROR <<<<<<<<<<<\n");
						APP_PRINT("Response APDU: ");

						for(i=0; i<pResBuffer->length;i++)
							APP_PRINT("0x%02X ",pResBuffer->buffer[i]);
					}
					else
					{
						SetColor(14);
						APP_PRINT("\n**************   END TEST TypeB  **************\n");
						SetColor(7);
						PresenceCheckFunction(&pContext);
					}
				break;
			 }
			break;
		case phHal_eISO14443_A_PICC:
			switch(Transceive_Opt_TypeA)
			{
			case Get_Application_Id:
				APP_PRINT("\n>>>>>>>>> GET DESFIRE APPLICATION IDs OK <<<<<<<<<<<\n");
				
				break;
			case Select_Appli:
				if((pResBuffer->buffer[pResBuffer->length-2]== 0x90 && pResBuffer->buffer[pResBuffer->length-1]== 0x00)
					|| pResBuffer->buffer[pResBuffer->length-2]== 0x61)
				{
					APP_PRINT("\n>>>>>>>>> SELECT CARD MANAGER OK<<<<<<<<<<<\n");
					APP_PRINT("Response APDU: ");
					
					for(i=0; i<pResBuffer->length;i++)
						APP_PRINT("0x%02X ",pResBuffer->buffer[i]);
				}
				else 
					// 61xx = normal processing, SW2 indicates the number of remaining databytes
					// 62xx to 6Fxx = error codes see ISO7816 spec for details
					if (pResBuffer->buffer[pResBuffer->length-2]>= 0x62)
					{
						// the test is considered has success as soon as the card responded
						// (even if the CD light application is not on the card)
					
						APP_PRINT("\n>>>>>>>>> SELECT CARD MANAGER KO <<<<<<<<<<<\n");
						APP_PRINT("Response APDU: ");
					
						for(i=0; i<pResBuffer->length;i++)
							APP_PRINT("0x%02X ",pResBuffer->buffer[i]);
					}
					else
					{
						SetColor(14);
						APP_PRINT("\n**************   END TEST TypeA  **************\n");
						SetColor(7);
					
						PresenceCheckFunction(&pContext);
					}
				break;
			 }
			break;
		case phHal_eFelica_PICC:
			switch(Transceive_Opt_Felica)
			{
			case Raw:
				APP_PRINT("\n>>>>>>>>> FELICA RAW COMMAND OK <<<<<<<<<<<\n");
				
				break;
			 }
			break;
		case phHal_eISO15693_PICC:
			switch(Transceive_Opt_15693)
			{
			
			case ISO15693_Read:
				//for(i=0; i<pResBuffer->length;i++)
				//	APP_PRINT("0x%02X ",pResBuffer->buffer[i]);
				APP_PRINT("\n>>>>>>>>> READ OK <<<<<<<<<<<\n");
				break;
			case ISO15693_Write:
				APP_PRINT("\n>>>>>>>>> WRITE OK <<<<<<<<<<<\n");
				
				break;
			}
			break;	
		}
		EventCallBack = FALSE;
	}
	else
	{
		switch(Tag_Info.Target_Type)
		{
		case phHal_eMifare_PICC:
			switch(Transceive_Opt_Mifare)
			{
			case Authentication:
				APP_PRINT("\n> Sector Authenticate failed \n");
				
				break;
			case Mifare_Read:
				APP_PRINT("\n> Read Block failed \n");
				
				break;
			case Mifare_Write:
				APP_PRINT("\n> Write Block failed \n");
				break;
			}
			break;
		case phHal_eJewel_PICC:
			switch(Transceive_Opt_Jewel)
			{
			case Read_All:
				APP_PRINT("\n> Read All failed \n");
				
				break;
			case Read_ID:
				APP_PRINT("\n> Read ID failed \n");
				
				break;
			 }
			break;

		case phHal_eISO14443_B_PICC:
			switch(Transceive_Opt_TypeB)
			{
			case Select_Application:
				APP_PRINT("\n> SELECT Calypso Apllication failed \n");
				
				break;
			 }
			break;

		case phHal_eISO14443_A_PICC:
			switch(Transceive_Opt_TypeA)
			{
			case Get_Application_Id:
				APP_PRINT("\n> Get Desfire Application failed \n");
				
				break;
			case Select_Appli:
				APP_PRINT("\n> Select Application failed \n");
				
				break;
			 }
		case phHal_eFelica_PICC:
			switch(Transceive_Opt_Felica)
			{
			case Raw:
				APP_PRINT("\n> Felica Raw Command failed \n");
				
				break;
			 }
		case phHal_eISO15693_PICC:	
			APP_PRINT("\n> Read ISO15693 Block failed \n");
			
			break;
		}
		
		SetColor(14);
		APP_PRINT("\n**************   END TEST    **************\n");
		SetColor(7);

		PresenceCheckFunction(&pContext);
	}
}

int Transceive_Config(uint8_t nb_block)
{
	static uint8_t Temp=0,i;

	Temp = 0;

	switch(Tag_Info.Target_Type)
	{
	case phHal_eMifare_PICC:
		switch(Transceive_Opt_Mifare)
		{
		case Authentication:
			Transceive_Param.cmd.MfCmd = phHal_eMifareAuthentA;
			Transceive_Param.addr = nb_block; 
			for(; Temp<Tag_Info.Target_UidLength ; Temp++)
				Write_Buffer[Temp]=*(Tag_Info.Target_Uid+Temp);
			Write_Buffer[Temp++]=0xFF;
			Write_Buffer[Temp++]=0xFF;
			Write_Buffer[Temp++]=0xFF;
			Write_Buffer[Temp++]=0xFF;
			Write_Buffer[Temp++]=0xFF;
			Write_Buffer[Temp++]=0xFF;
			break; 

		case Mifare_Read:
			Temp = 0;
			Transceive_Param.cmd.MfCmd = phHal_eMifareRead;
			Transceive_Param.addr = nb_block; 
			break;

		case Mifare_Write:
			Temp = 0;
			if((TagType == TAG_MIFARE_1K)||(TagType == TAG_MIFARE_4K))
			{
				Transceive_Param.cmd.MfCmd = phHal_eMifareWrite16;
				Transceive_Param.addr = nb_block; 
				for(i=0;i<16;i++)
					Write_Buffer[Temp++] = Block_Buffer[i];
				//Write_Buffer[Temp++]=0x00;
			}
			else if(TagType == TAG_MIFARE_UL)
			{
				Transceive_Param.cmd.MfCmd = phHal_eMifareWrite4;
				Transceive_Param.addr = nb_block; 
				for(i=0;i<4;i++)
					Write_Buffer[Temp++] = Block_Buffer[i];
			}
			break;
		default:
			break;
		}
         break;
	case phHal_eJewel_PICC:
        Temp = 0;
		switch(Transceive_Opt_Jewel)
		{
		case Read_All:
			Transceive_Param.cmd.JewelCmd = phHal_eJewel_Raw;
            Write_Buffer[Temp++]=phTst_eJewel_ReadAll;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			for(i=0; i<Tag_Info.Target_UidLength; Temp++,i++)
			{
				Write_Buffer[Temp]=*(Tag_Info.Target_Uid+i);
			}
			break;
		case Read_ID:
			Transceive_Param.cmd.JewelCmd = phHal_eJewel_Raw;
			Write_Buffer[Temp++]=phTst_eJewel_RID;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			Write_Buffer[Temp++]=0x00;
			break;
		}
		break;
	case phHal_eISO14443_B_PICC:
		if (Transceive_Opt_TypeB == Select_Application) {
			Transceive_Param.cmd.Iso144434Cmd = phHal_eIso14443_4_Raw;
			for(i=0;i<14;i++)
				Write_Buffer[Temp++]= SelectApplication_CD_LIGTH[i];
		}
		break;
	case phHal_eISO14443_A_PICC:
		switch(Transceive_Opt_TypeA)
		{
		case Get_Application_Id:
			Transceive_Param.cmd.Iso144434Cmd = phHal_eIso14443_4_Raw;
			Write_Buffer[Temp++]= GetApplicationId;
			break;
		case Select_Appli:
			Transceive_Param.cmd.Iso144434Cmd = phHal_eIso14443_4_Raw;
			
			for (i=0;i<13;i++)
				Write_Buffer[Temp++]= SelectCardManager[i];
			break;
		}
		break;
	case phHal_eFelica_PICC:
		Transceive_Param.cmd.FelCmd =  phHal_eFelica_Raw;
		for(i=0;i<16;i++)
			Write_Buffer[Temp++]= FelicaRawCmd[i];
		break;
	case phHal_eISO15693_PICC:
		Transceive_Param.cmd.Iso15693Cmd = phHal_eIso15693_Cmd ;
	        break;	
	}
	return Temp;
}

/*******************************************/
/*    TAG MIFARE  FUNCTION                 */
/*******************************************/
void Mifare_Authenticate(uint8_t sector_nb)
{
	NFCSTATUS Status;

	/* Block Authenticate */
	Transceive_Opt_Mifare = Authentication;
	Transceive_Param.sSendData.length = Transceive_Config(sector_nb);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);

	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Mifare   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

void Mifare_Read_Block(uint8_t nb_block)
{
	NFCSTATUS Status;

	/* Read Block */
	if((TagType == TAG_MIFARE_1K)||(TagType == TAG_MIFARE_4K))
	{
		APP_PRINT("\n>>>>>>>>> READ Block %d <<<<<<<<<<<\n",nb_block);
	}
	else if(TagType == TAG_MIFARE_UL)
	{
		APP_PRINT("\n>>>>>>>>> READ Sector %d <<<<<<<<<<<\n",nb_block/4);
	}
	
	Transceive_Opt_Mifare = Mifare_Read;
	Transceive_Param.sSendData.length = Transceive_Config(nb_block);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Mifare   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

void Mifare_Write_Block(uint8_t nb_block)
{
	NFCSTATUS Status;
	
	/* Write Block */
	APP_PRINT("\n>>>>>>>>> Write Block %d <<<<<<<<<<<\n",nb_block);
      
	Transceive_Opt_Mifare = Mifare_Write;
	Transceive_Param.sSendData.length = Transceive_Config(nb_block);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Mifare   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

/*******************************************/
/*    TAG JEWEL  FUNCTION                  */
/*******************************************/
void Jewel_Read_Complete()
{
	NFCSTATUS Status;

	/* Read Complete */
	printf("\n>>>>>>>>> READ Complete (96 Bytes) <<<<<<<<<<<\n");
	Transceive_Opt_Jewel = Read_All;
	Transceive_Param.sSendData.length = Transceive_Config(0x00);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Jewel   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

/*******************************************/
/*    TAG FELICA  FUNCTION                 */
/*******************************************/
void Felica_Raw_Command()
{
	NFCSTATUS Status;
	printf("\n>>>>>>>>> FELICA RAW COMMAND <<<<<<<<<<<\n");
	Transceive_Opt_Felica = Raw;
	Transceive_Param.sSendData.length = Transceive_Config(0x00);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param, *(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Felica   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

/*******************************************/
/*    TAG TYPE B CALYPSO  FUNCTION         */
/*******************************************/
void TypeB_Calypso_Exchange_Apdu()
{
	NFCSTATUS Status;

	printf("\n>>>>>>>>> SELECT CD Ligth Application <<<<<<<<<<<\n");
	Transceive_Opt_TypeB = Select_Application;
	Transceive_Param.sSendData.length = Transceive_Config(0x00);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST TypeB   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

/*******************************************/
/*          TAG DESFIRE  FUNCTION          */
/*******************************************/
void TypeA_Desfire_Exchange_Apdu()
{
	NFCSTATUS Status;

	printf("\n>>>>>>>>> GET DESFIRE APPLICATION IDs <<<<<<<<<<<\n");
	Transceive_Opt_TypeA = Get_Application_Id;
	Transceive_Param.sSendData.length = Transceive_Config(0x00);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;

	Status = phHal4Nfc_Transceive(&psHwRef,&Transceive_Param,
		*(global_info.psDiscoveryInfo->ppRemoteDevInfo),TagTranceiveCB,&Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST Mifare Desfire   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}

/*******************************************/
/*          TAG TYPE A  FUNCTION           */
/*******************************************/
void TypeA_Exchange_Apdu()
{
	NFCSTATUS Status;

	printf("\n>>>>>>>>> SELECT CARD MANAGER  <<<<<<<<<<<\n");
	Transceive_Opt_TypeA = Select_Appli;
	Transceive_Param.sSendData.length = Transceive_Config(0x00);
	Transceive_Param.sSendData.buffer = Write_Buffer;
	Transceive_Param.sRecvData.buffer = Read_Buffer;
	Transceive_Param.sRecvData.length = 300;
	// in this example a card Mifare+ISO14443-4 card is used, the ISO14443-4 is stored at offset [1] of the table
	Status = phHal4Nfc_Transceive(&psHwRef, 
		&Transceive_Param, global_info.psDiscoveryInfo->ppRemoteDevInfo[1], TagTranceiveCB, &Localcontext);
	if(Status != NFCSTATUS_PENDING)
	{
		SetColor(14);
		APP_PRINT("\n**************   END TEST TypeA   **************\n");
		SetColor(7);
		PresenceCheckFunction(&Localcontext);
	}
	else
	{
		/* Wait for Event */
		WaitForEvent();
	}
}
