
#ifndef FRITESTPLANAPPLICATION_H
#define FRITESTPLANAPPLICATION_H

/* NFC FRI include files*/
extern "C"
{
	#include "phNfcTypes.h"
	#include "phNfcCompId.h"
	#include "phNfcStatus.h"
	#include "phNfcInterface.h"
	#include "phNfcHalTypes.h"
	#include "phDal4Nfc_DeferCall.h"
	#include "phDal4Nfc.h"
	#include "phOsalNfc.h"
	#include "phHal4Nfc.h"
	#include "phOsalNfc_Timer.h"
	#include "phDal4Nfc_DeferCall.h"
	#include "Windows.h"
	#include "stdio.h"
}
extern phHal_sHwReference_t psHwRef;

/* Message definition for main test */
#define     HAL_OPEN                    (1) 
#define     HAL_CLOSE                   (2)
#define     HAL_GET_CAPABILITIES        (3)
#define     HAL_NOTIFICATION_REGISTER   (4)
#define     HAL_CONFIG_DISCOVERY        (5)
#define     HAL_START_APPLICATION       (6)
#define		HAL_DOWNLOAD				(7)


/* Tag TYPE */
#define		NDEF_TYPE1				(1)
#define		NDEF_TYPE2				(2)
#define		NDEF_TYPE3				(3)
#define		NDEF_TYPE4				(4)
#define		NDEF_MIFARE				(5)
#define		TAG_TYPE_A              (6)
#define		TAG_TYPE_B  			(7)
#define		TAG_FELICA				(8)
#define		TAG_JEWEL				(9)
#define		TAG_MIFARE_1K			(10)
#define		TAG_MIFARE_UL			(11) 
#define	    TAG_MIFARE_DESFIRE		(12)
#define		TAG_NON_NDEF			(13)
#define		TAG_ISO15693			(14)
#define		TAG_MIFARE_4K			(15)

/* Test Name Single Reader */
#define		SR1						(1)
#define		SR2						(2)
#define		SR3						(3)
#define		SR4						(4)
#define		SR5						(5)
#define		SR6						(6)
#define		SR7						(7)
#define		SR8						(8)
#define		SR9						(9)
#define		SR10					(10)
#define		SR11					(11)
#define		SR12					(12)
#define		SR13					(13)
#define		CE1						(14)
#define		P2P1     		        (15)
#define		P2P2		            (16)

/* Test Status */
#define		NB_TEST					(17)
#define		TEST_SUCCESS			(0)
#define		TEST_FAILED				(1)
#define		TEST_NOT_PASSED			(2)

/* Test Mode */
#define READER_MODE					(1)
#define DUAL_READER_MODE			(2)
#define	P2P_MODE					(3)

#ifdef Debug_Print
#define APP_PRINT( ... )  printf(__VA_ARGS__);
#else
#define APP_PRINT( ... ) 
#endif

#define MifareUL_CardEnable
#define MifareStd_CardEnable
#define ISO14443_4A_CardEnable
#define ISO14443_4B_CardEnable
#define Jewel_CardEnable
#define NFC_CardEnable
#define Felica_CardEnable
#define ISO15693_CardEnable
	

typedef struct Test
{
    uint8_t  nbTest;
	uint8_t  status;
}Test_t;

typedef struct Test_Status
{
    Test_t   test[NB_TEST];
}Test_Status_t;

typedef struct ph_Tag_Info
{

	phHal_eRemDevType_t				Target_Type;
	phHal_eRemDevType_t				Multi_Target_Type[5];
	uint8_t							Target_UidLength;
	uint8_t							Target_Uid[PHHAL_MAX_UID_LENGTH];
	uint8_t							Target_SAK;
	uint8_t							NofTarget;

}ph_Tag_Info_t;

typedef enum Mifare_Transceive
{
	Authentication=1,
	Mifare_Read,
	Mifare_Write,
	Mifare_Write16_0,
	Invalid_CMD,
	Raw_CMD,

}Mifare_Transceive;

typedef enum Jewel_Tranceive
{
	Read_All=1,
	Read_ID,
}Jewel_Tranceive;

typedef enum TypeB_Tranceive
{
	Select_Application = 1,
}TypeB_Tranceive;

typedef enum TypeA_Tranceive
{
	Get_Application_Id = 1,
	Select_Appli,
	Get_File_Id,
}TypeA_Tranceive;

typedef enum Felica_Transceive
{
	Check=1,
	Raw,
	Update,
}Felica_Transceive;
typedef enum ISO15693_Transceive
{
	ISO15693_Read = 1,
	ISO15693_Write,
	
}ISO15693_Transceive;


/* Application Functions */
NFCSTATUS WaitForEvent(void);
void PostMessage(uint32_t SourceID, uint32_t DestID, phOsalNfc_Message_t * pMsg);  
void SetColor(unsigned short color);

void PresenceCheckFunction(void *context);

/* Polling Loop Functions */
NFCSTATUS NotificationRegisterPollingLoop(void *context);
NFCSTATUS ConfigStartPollingLoop(void *context);
void DisplayPollingLoopTestResult();

/* Polling Loop Callbacks */
void ConfigDiscoveryCB(void *context,NFCSTATUS status);

void TagRegisterNotificationCB (void  *context, phHal_eNotificationType_t type, phHal4Nfc_NotificationInfo_t info, NFCSTATUS status);                                       
void TagTranceiveCB(void* pContext,phHal_sRemoteDevInformation_t *rem_dev_info,phNfc_sData_t* pResBuffer,NFCSTATUS Status);
void TagConnect_ResponseCB( void  *context,phHal_sRemoteDevInformation_t *rem_dev_info, NFCSTATUS status);
void TagDisconnect_ResponseCB(void*	pContext,phHal_sRemoteDevInformation_t *rem_dev_info, NFCSTATUS status);
void Presence_CheckCB(void *context, NFCSTATUS status);

void Get_Tag_ID(ph_Tag_Info_t	 *TagInfo );
void Mifare_UL_ReadSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer);
void Mifare_UL_WriteBlock(uint8_t index_Block, uint8_t  *MifareRead_Buffer);
void Mifare_1K_ReadSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer);
void Mifare_1K_WriteSector(uint8_t index_Sector, uint8_t  *MifareRead_Buffer);
void Jewel_Read_Complete();
void Felica_Raw_Command();
void TypeB_Calypso_Exchange_Apdu();
void TypeA_Desfire_Exchange_Apdu();
void TypeA_Exchange_Apdu();
#endif /* end of FRITESTPLANAPPLICATION_H */