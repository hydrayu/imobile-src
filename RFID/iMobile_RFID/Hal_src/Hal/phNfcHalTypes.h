
// $Revision: 1.106 $


#ifndef PHNFCHALTYPES_H /* */
#define PHNFCHALTYPES_H /* */

/**
 *  \name HAL Types
 *
 * File: \ref phNfcHalTypes.h
 *
 */

/*@{*/
#define PHNFCHALTYPES_FILEREVISION "$Revision: 1.106 $" /**< \ingroup grp_file_attributes */
#define PHNFCHALTYPES_FILEALIASES  "$Aliases: NFC_FRI1.1_WK1007_SDK,NFC_FRI1.1_WK1017_PREP1,NFC_FRI1.1_WK1017_R34_1,NFC_FRI1.1_WK1017_R34_2,NFC_FRI1.1_WK1023_R35_1 $"     /**< \ingroup grp_file_attributes */
/*@}*/

#include <phNfcTypes.h>
#include <phNfcCompId.h>
#include <phNfcConfig.h>


/**
 *
 *  \name HAL Overall Definitions
 *
 *  Definitions applicable to a variety of purposes and functions/features.
 *
 */
 /*@{*/

#define PHHAL_COMMON_MAX_STRING_LENGTH  0x40U     /**< \ingroup grp_hal_common
                                                     Maximum vendor name length in bytes. */
#define PHHAL_UNKNOWN_DEVICE_TYPE       0x00U   /**< \ingroup grp_hal_common
                                                     Unknown device type. */
#define PHHAL_SERIAL_DEVICE             0x01U  /**< \ingroup grp_hal_common
                                                     Serial device type.  */
#define PHHAL_USB_DEVICE                0x02U   /**< \ingroup grp_hal_common
                                                     USB device type. */
#define PHHAL_I2C_DEVICE                0x03U   /**< \ingroup grp_hal_common
                                                     I2C device type. */
#define PHHAL_SPI_DEVICE                0x04U   /**< \ingroup grp_hal_common
                                                     SPI device type. */
#define PHHAL_PARALLEL_DEVICE           0x05U   /**< \ingroup grp_hal_common
                                                     Parallel device type. */
#define PHHAL_NFCID_LENGTH              0x0AU  /**< \ingroup grp_hal_common
                                                     Maximum length of NFCID 1..3. */

#define PHHAL_MAX_DATASIZE              0xFBU       /* 256 * Maximum Data size sent
                                                     * by the HAL
                                                     */

#define PHHAL_ATQA_LENGTH               0x02U       /**< ATQA length */
#define PHHAL_MAX_UID_LENGTH            0x0AU       /**< Maximum UID length expected */
#define PHHAL_MAX_ATR_LENGTH            0x30U       /**< Maximum ATR_RES (General Bytes) 
                                                     *   length expected */

#define PHHAL_ATQB_LENGTH               0x0BU       /**< ATQB length */

#define PHHAL_PUPI_LENGTH               0x04U       /**< PUPI length */
#define PHHAL_APP_DATA_B_LENGTH         0x04U       /**< Application Data length for Type B */
#define PHHAL_PROT_INFO_B_LENGTH        0x03U       /**< Protocol info length for Type B */
#define PHHAL_FEL_SYS_CODE_LEN          0x02U       /**< Felica System Code Length */
#define PHHAL_FEL_ID_LEN                0x08U       /**< Felica current ID Length */
#define PHHAL_FEL_PM_LEN                0x08U       /**< Felica current PM Length */
#define PHHAL_15693_UID_LENGTH          0x08U       /**< Length of the Inventory bytes for
                                                         ISO15693 Tag */

#define VENDOR_NAME_LEN                 0x14U
#define MAX_TRANSFER_UNIT               0x21U 
#define SESSIONID_SIZE                  0x08U
#define MAX_AID_LEN                     0x10U
#define MAX_UICC_PARAM_LEN              0xFFU

#define MIFARE_BITMASK                  0x08U
#define ISO_14443_BITMASK               0x20U
#define ISO_14443_DETECTED              0x20U
#define NFCIP_BITMASK                   0x40U
#define NFCIP_DETECTED                  0x40U

#define MAX_TARGET_SUPPORTED            MAX_REMOTE_DEVICES

#define NFC_HW_PN65N                    0x10U

#define NXP_NFCIP_NFCID2_ID             0x01FEU

#define NXP_FULL_VERSION_LEN            0x0BU


/*@}*/

/**
 * Data Buffer Structure to hold the Data Buffer
 *
 * This structure holds the Data in the Buffer of the specified 
 * size.
 * 
 */

typedef struct phNfc_sData
{
    uint8_t             *buffer;
    uint32_t            length;
}phNfc_sData_t;


/** \ingroup  grp_hal_common
 *
 * \if hal
 *  \brief Protocol Support Information
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Supported Protocols Structure </em> holds all protocol supported by the current NFC
 *  device.
 *
 *  \note All members of this structure are output parameters [out].
 *
 */
typedef struct phHal_sSupProtocol
{
    unsigned int MifareUL    : 1;  /**< Protocol Mifare Ultra Light or 
                                   any NFC Forum Type-2 tags */
    unsigned int MifareStd   : 1;  /**< Protocol Mifare Standard. */
    unsigned int ISO14443_4A : 1;  /**< Protocol ISO14443-4 Type A.  */
    unsigned int ISO14443_4B : 1;  /**< Protocol ISO14443-4 Type B.  */
    unsigned int ISO15693    : 1;  /**< Protocol ISO15693 HiTag.  */
    unsigned int Felica      : 1;  /**< Protocol Felica. */
    unsigned int NFC         : 1;  /**< Protocol NFC. */
    unsigned int Jewel       : 1;  /**< Protocol Innovision Jewel Tag. */
    /*** TODO: Add SWP, ETSI HCI to this list **/
} phHal_sSupProtocol_t;


/** \ingroup grp_hal_common
 *
 *
 * \if hal
 *  \brief Information related to the NFC Device
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Device Information Structure </em> holds information
 *  related to the NFC IC read during initialization time. 
 *  It allows the caller firware, hardware version, the model id,
 *  HCI verison supported and vendor name. Refer to the NFC Device 
 *  User Manual on how to interpret each of the values. In addition
 *  it also contains capabilities of the NFC Device such as the 
 *  protocols supported in Reader and emulation mode
 *
 */

typedef struct phHal_sDeviceCapabilities
{
    /* */
    uint32_t                hal_version; /**< \ingroup grp_hal_common
                                              HAL 4.0 Version Information. */
    uint32_t                fw_version; /**< \ingroup grp_hal_common
                                              Firmware Version Info. */
    uint32_t                hw_version; /**< \ingroup grp_hal_common
                                              Hardware Version Info. */
    uint8_t                 model_id;   /**< \ingroup grp_hal_common
                                              IC Variant . */
    uint8_t                 hci_version; /**< \ingroup grp_hal_common
                                              ETSI HCI Version Supported */
    utf8_t                  vendor_name[VENDOR_NAME_LEN]; /**< \ingroup grp_hal_common
                                              Vendor name (Null terminated string)*/
    uint8_t                 full_version[NXP_FULL_VERSION_LEN];

    phHal_sSupProtocol_t    ReaderSupProtocol; /**< Supported protocols 
                                                      (Bitmapped) in Reader mode. */
    phHal_sSupProtocol_t    EmulationSupProtocol;    /**< Supported protocols
                                                       (Bitmapped) in Emulation 
                                                        mode. */

}phHal_sDeviceCapabilities_t;


/**
 * \ingroup grp_hal_common
 *
 * \if hal
 *  \brief Hardware Reference - The Peripheral Representation
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The Hardware Reference structure is filled as part of the open function and
 *  contains information regarding connected peripheral NFC device. It also
 *  stores the refernce to the communication driver passed by the HAL client
 *  for usage during communication with the NFC Device 
 *
 * \note The caller can consider this structure atomic, no interpretation is required 
 *       for HAL operation.
 * 
 * \sa phHal4Nfc_Open .
 * 
 */

typedef struct phHal_sHwReference
{
    /**<  Will be usable/valid after the Open function. */
    void                            *p_board_driver;
    /**<  Session Identifier for the established session */
    uint8_t                         session_id[SESSIONID_SIZE];
    /**<  SMX  Connected TRUE/FALSE */
    uint8_t                         smx_connected; 
    /**<  UICC  Connected TRUE/FALSE */
    uint8_t                         uicc_connected;
    /**<  UICC  Reader Mode is Active TRUE/FALSE */
    uint8_t                         uicc_rdr_active;
    /**<  Device information. */
    phHal_sDeviceCapabilities_t     device_info;
    /**<  Context of the HAL Layer */
    void                            *hal_context;
    /**<  Context of the DAL Layer */
    void                            *dal_context;
} phHal_sHwReference_t;


/** \ingroup grp_hal_common
 *
 * \if hal
 * \brief Hardware configuration - Configuration Parameters for the NFC Device
 * \else
 * \brief HAL-Specific
 * \endif
 *
 *  The parameters used to configure the device during the initialisation.
 *  This structure is used internally by the HAL implementation and is filled
 *  up based on various configuration parameters from the config file
 * \note None.
 *
 */

typedef struct phHal_sHwConfig
{
    
    uint8_t             session_id[SESSIONID_SIZE]; /**<  Session Identifier for 
                                                     the established session */

    uint8_t             clk_req; /**<  Clock Request Setting */

    uint8_t             input_clk; /**<  Input Clock Setting */

} phHal_sHwConfig_t;



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* This data structure is not used anymore and will be removed in subsequent
   release */
typedef struct phHal_sDepFlags
{
   unsigned int MetaChaining : 1;    
   unsigned int NADPresent   : 1;    
} phHal_sDepFlags_t;

/* This data structure is not used anymore and will be removed in subsequent
   release */

typedef struct phHal_sDepAdditionalInfo
{
    phHal_sDepFlags_t DepFlags;   
    uint8_t NAD;                  
} phHal_sDepAdditionalInfo_t;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



/** \ingroup grp_hal_common
 *
 * \if hal
 *  \brief Enumerated MIFARE Commands
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Mifare Command List Enumerator </em> lists all available Mifare native commands.
 *
 * \note None.
 *
 */

typedef enum phHal_eMifareCmdList
{
    phHal_eMifareRaw        = 0x00U,     /**< This command performs raw transcations .
                                              Format of the phLibNfc_sTransceiveInfo_t 
                                              content in this case shall be as below: 
                                                ?  cmd: filed shall set to  phHal_eMifareRaw . 
                                                ?  addr : doesn't carry any significance.
                                                ?  sSendData : Shall contain formatted raw buffer 
                                                                based on MIFARE commands type used. 
                                                                Formatted buffer shall follow below 
                                                                formating scheme.
 
                                              CmdType+ Block No + CommandSpecific data + 2 byte CRC
                                              Ex: With  Write  4 byte command  on block 8  looks as 
                                              " 0xA2,0x08,0x01,0x02,0x03,0x04,CRC1,CRC2 
                                              Note :  For MIFARE Std card we recommend use MIFARE 
                                                      commands directly.
                                           */
    phHal_eMifareAuthentA   = 0x60U,     /**< Mifare Standard:\n
                                              This command performs an authentication with KEY A for a sector.\n
                                              Format of the phLibNfc_sTransceiveInfo_t content in this case is : 
                                                ?      cmd: field shall set to  phHal_eMifareAuthentA . 
                                                ?      addr : indicates MIFARE block address. 
                                                           Ex: 0x08 indicates block 8 needs to be authenticated.
                                                ?      sSendData : Shall contain authentication key values. 
                                                                    sSendData ,buffer shall contain authentication 
                                                                    key values 01 02 03 04 05 06 authenticates 
                                                                    block 08 with the key 0x01[..]06. If this 
                                                                    command fails, then  user needs to reactivate 
                                                                    the remote Mifare card.                                            
                                          */
    phHal_eMifareAuthentB   = 0x61U,     /**< Mifare Standard:\n
                                              This command performs an authentication with KEY B for a sector.\n
                                              Format of the phLibNfc_sTransceiveInfo_t content in this case is : 
                                                ?      cmd: field shall set to  phHal_eMifareAuthentB . 
                                                ?      addr : indicates MIFARE block address. 
                                                           Ex: 0x08 indicates block 8 needs to be authenticated.
                                                ?      sSendData : Shall contain authentication key values. 
                                                                    sSendData ,buffer shall contain authentication 
                                                                    key values 01 02 03 04 05 06 authenticates 
                                                                    block 08 with the key 0x01[..]06. If this 
                                                                    command fails, then  user needs to reactivate 
                                                                    the remote Mifare card.   
                                          */
    phHal_eMifareRead16     = 0x30U,     /**< Mifare Standard and Ultra Light:\n
                                              Read 16 Bytes from a Mifare Standard block or 4 Mifare Ultra Light pages.\n
                                              Format of the phLibNfc_sTransceiveInfo_t content in this case is : 
                                                ?      cmd: field shall set to  phHal_eMifareRead16 . 
                                                ?      addr : memory adress to read.   
                                                ?      sRecvData : Shall contain buffer of size 16 
                                                                    to read the data into.                                                                  
                                             
                                              If this command fails, the user needs to reactivate the 
                                              the remote Mifare card
                                          */
    phHal_eMifareRead       = 0x30U,
    phHal_eMifareWrite16    = 0xA0U,     /**< Mifare Standard and Ultra Light:\n
                                              Write 16 Bytes to a Mifare Standard block or 4 Mifare Ultra Light pages.\n
                                              Format of the phLibNfc_sTransceiveInfo_t content in this case is : 
                                                ?      cmd: field shall set to  phHal_eMifareWrite16 . 
                                                ?      addr : starting memory adress to write from.   
                                                ?      sSendData : Shall contain buffer of size 16 containing
                                                                    the data bytes to be written.                                                                  
                                             
                                              If this command fails, the user needs to reactivate the 
                                              the remote Mifare card
                                          */
    phHal_eMifareWrite4     = 0xA2U,     /**< Mifare Ultra Light:\n
                                              Write 4 bytes.\n
                                              Format of the phLibNfc_sTransceiveInfo_t content in this case is : 
                                                ?      cmd: field shall set to  phHal_eMifareWrite4 . 
                                                ?      addr : starting memory adress to write from.   
                                                ?      sSendData : Shall contain buffer of size 4 containing
                                                                    the data bytes to be written.                                                                  
                                             
                                              If this command fails, the user needs to reactivate the 
                                              the remote Mifare card
                                          */
    phHal_eMifareInc        = 0xC1U,     /**< Increment. */
    phHal_eMifareDec        = 0xC0U,     /**< Decrement. */
    phHal_eMifareTransfer   = 0xB0U,     /**< Tranfer.   */
    phHal_eMifareRestore    = 0xC2U,     /**< Restore.   */
    phHal_eMifareReadSector = 0x38U,     /**< Read Sector.   */
    phHal_eMifareWriteSector= 0xA8U,     /**< Write Sector.   */
    phHal_eMifareInvalidCmd = 0xFFU      /**< Invalid Command */
} phHal_eMifareCmdList_t;


/** \ingroup grp_hal_common
 *
 *  The <em> T=Cl Command List Enumerator </em> lists all available T=Cl Commands.
 *
 * \note None.
 *
 */
typedef enum phHal_eIso14443_4_CmdList
{
    phHal_eIso14443_4_Raw             = 0x00U /**< ISO 14443-4 Exchange command:\n
                                                 - This command sends the data buffer directly 
                                                 to the remote device */

} phHal_eIso14443_4_CmdList_t;



/** \ingroup grp_hal_common
 *
 *  The <em> ISO15693 Command List Enumerator </em> lists all available ISO15693 Commands.
 *
 * \note None.
 *
 */

typedef enum phHal_eIso15693_CmdList
{
#if 0
    phHal_eIso15693_Raw             = 0x00U, /**< ISO 15693 Exchange Raw command:\n
                                                 - This command sends the data buffer directly 
                                                 to the remote device */
#endif
    phHal_eIso15693_Cmd             = 0x20U, /**< ISO 15693 Exchange command:\n
                                                 - This command is used to access the card 
                                                 to the remote device */
    phHal_eIso15693_Invalid         = 0xFFU      /**< Invalid Command */
} phHal_eIso15693_CmdList_t;

/** \ingroup grp_hal_common
 *
 *  The <em> Felica Command List Enumerator </em> lists all available Felica Commands.
 *
 * \note None.
 *
 */

typedef enum phHal_eFelicaCmdList
{
    phHal_eFelica_Raw             = 0xF0U, /**< Felica Raw command:\n
                                                 - This command sends the data buffer directly 
                                                 to the remote device */
    phHal_eFelica_Check           = 0x00, /**< Felica Check command:\n
                                                 - This command checks the data from the Felica
                                                  remote device */
    phHal_eFelica_Update          = 0x01, /**< Felica Update command:\n
                                                 - This command updates the data onto the Felica
                                                  remote device */
    phHal_eFelica_Invalid         = 0xFFU      /**< Invalid Command */
} phHal_eFelicaCmdList_t;


typedef enum phHal_eJewelCmdList
{
    phHal_eJewel_Raw            = 0x00U, /**< Jewel command:\n
                                                 - This command sends the data buffer directly 
                                                 to the remote device */
    phHal_eJewel_Invalid        = 0xFFU  /**< Invalid jewel command */
}phHal_eJewelCmdList_t;



/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Reader A RF Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Reader A structure </em> includes the available information
*  related to the discovered ISO14443A remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/
typedef struct phHal_sIso14443AInfo
{
    uint8_t         Uid[PHHAL_MAX_UID_LENGTH];      /**< UID information of the TYPE A
                                                       Tag Discovered */
    uint8_t         UidLength;                      /**< UID information length, shall not be greater 
                                                    than PHHAL_MAX_UID_LENGTH i.e., 10 */
    uint8_t         AppData[PHHAL_MAX_ATR_LENGTH]; /**< Application data information of the 
                                                        tag discovered (= Historical bytes for 
                                                        type A) */  
    uint8_t         AppDataLength;                  /**< Application data length */
    uint8_t         Sak;                            /**< SAK informationof the TYPE A
                                                       Tag Discovered */
    uint8_t         AtqA[PHHAL_ATQA_LENGTH];        /**< ATQA informationof the TYPE A
                                                       Tag Discovered */
    uint8_t         MaxDataRate;                    /**< Maximum data rate supported by the TYPE A
                                                       Tag Discovered */
    uint8_t         Fwi_Sfgt;                       /**< Frame waiting time and start up frame guard 
                                                    time as defined in ISO/IEC 14443-4[7] for 
                                                    type A */
}phHal_sIso14443AInfo_t;

/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Reader B RF Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Reader B structure </em> includes the available information
*  related to the discovered ISO14443B remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/

typedef struct phHal_sIso14443BInfo 
{
    union phHal_uAtqBInfo
    {
        struct phHal_sAtqBInfo
        {
            uint8_t         Pupi[PHHAL_PUPI_LENGTH];            /**< PUPI information  of the TYPE B
                                                                    Tag Discovered */
            uint8_t         AppData[PHHAL_APP_DATA_B_LENGTH];   /**< Application Data  of the TYPE B
                                                                    Tag Discovered */
            uint8_t         ProtInfo[PHHAL_PROT_INFO_B_LENGTH]; /**< Protocol Information  of the TYPE B
                                                                    Tag Discovered */
        }AtqResInfo;
        uint8_t         AtqRes[PHHAL_ATQB_LENGTH];          /**< ATQB Response Information of TYPE B
                                                                Tag Discovered */
    }AtqB;
    uint8_t         HiLayerResp[PHHAL_MAX_ATR_LENGTH];  /**< Higher Layer Response information  
                                                             in answer to ATRRIB Command for Type B */  
    uint8_t         HiLayerRespLength;                  /**< Higher Layer Response length */
    uint8_t         Afi;                                /**< Application Family Identifier of TYPE B
                                                                Tag Discovered */
    uint8_t         MaxDataRate;                        /**< Maximum data rate supported by the TYPE B
                                                             Tag Discovered */
}phHal_sIso14443BInfo_t;

typedef struct phHal_sIso14443BPrimeInfo 
{
    /* TODO: This will be updated later */
    void *BPrimeCtxt;
}phHal_sIso14443BPrimeInfo_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Jewel Reader RF Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Jewel Reader structure </em> includes the available information
*  related to the discovered Jewel remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/

typedef struct phHal_sJewelInfo 
{
    uint8_t         Uid[PHHAL_MAX_UID_LENGTH];      /**< UID information of the TYPE A
                                                         Tag Discovered */
    uint8_t         UidLength;                      /**< UID information length, shall not be greater 
                                                    than PHHAL_MAX_UID_LENGTH i.e., 10 */
    uint8_t         HeaderRom0; /**< Header Rom byte zero */
    uint8_t         HeaderRom1; /**< Header Rom byte one */

}phHal_sJewelInfo_t;

/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Felica Reader RF Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Felica Reader structure </em> includes the available information
*  related to the discovered Felica remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/

typedef struct phHal_sFelicaInfo
{
    uint8_t     IDm[(PHHAL_FEL_ID_LEN + 2)];              /**< Current ID of Felica tag */
    uint8_t     IDmLength;                          /**< IDm length, shall not be greater 
                                                    than PHHAL_FEL_ID_LEN i.e., 8 */
    uint8_t     PMm[PHHAL_FEL_PM_LEN];              /**< Current PM of Felica tag */
    uint8_t     SystemCode[PHHAL_FEL_SYS_CODE_LEN]; /**< System code of Felica tag */
}phHal_sFelicaInfo_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Reader 15693 RF Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Reader A structure </em> includes the available information
*  related to the discovered ISO15693 remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/

typedef struct phHal_sIso15693Info
{
    uint8_t         Uid[PHHAL_15693_UID_LENGTH];      /**< UID information of the 15693
                                                       Tag Discovered */
    uint8_t         UidLength;                      /**< UID information length, shall not be greater 
                                                    than PHHAL_15693_UID_LENGTH i.e., 8 */
    uint8_t         Dsfid;                          /**< DSF information of the 15693
                                                       Tag Discovered */
    uint8_t         Flags;                          /**< Information about the Flags
                                                        in the 15693 Tag Discovered */
    uint8_t         Afi;                            /**< Application Family Identifier of
                                                          15693 Tag Discovered */
}phHal_sIso15693Info_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief NFC Data Rate Supported between the Reader and the Target
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHalNfc_eDataRate enum </em> lists all the Data Rate 
*  values to be used to determine the rate at which the data is transmitted
*  to the target.
*
*  \note None.
*/


typedef enum phHalNfc_eDataRate{
    phHal_eDataRate_106    = 0x00U, 
    phHal_eDataRate_212, 
    phHal_eDataRate_424, 
    /* phHal_eDataRate_848, 
    phHal_eDataRate_1696, 
    phHal_eDataRate_3392, 
    phHal_eDataRate_6784,*/
    phHal_eDataRate_RFU 
}phHalNfc_eDataRate_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief NFCIP1 Gate Information Container 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> NFCIP1 structure </em> includes the available information
*  related to the discovered NFCIP1 remote device. This information 
*  is updated for every device discovery.
*  \note None.
*
*/

typedef struct phHal_sNfcIPInfo 
{
    /* Contains the random NFCID3I conveyed with the ATR_REQ. 
        always 10 bytes length 
        or contains the random NFCID3T conveyed with the ATR_RES.
        always 10 bytes length */
    uint8_t         NFCID[PHHAL_MAX_UID_LENGTH];    
    uint8_t         NFCID_Length;
    /* ATR_RES = General bytes length, Max length = 48 bytes */
    uint8_t         ATRInfo[PHHAL_MAX_ATR_LENGTH];  
    uint8_t         ATRInfo_Length;
    /**< SAK information of the tag discovered */
    uint8_t         SelRes;
    /**< ATQA information of the tag discovered */
    uint8_t         SenseRes[PHHAL_ATQA_LENGTH];
    /**< Is Detection Mode of the NFCIP Target Active */
    uint8_t         Nfcip_Active;
    /**< Maximum frame length supported by the NFCIP device */  
    uint16_t        MaxFrameLength;
    /**< Data rate supported by the NFCIP device */
    phHalNfc_eDataRate_t         Nfcip_Datarate;
}phHal_sNfcIPInfo_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Specific Information Container
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Remote Device Information Union </em> includes the available Remote Device Information
*  structures. Following the device detected, the corresponding data structure is used.
*
*  \note None.
*
*/

typedef union phHal_uRemoteDevInfo
{
    phHal_sIso14443AInfo_t          Iso14443A_Info;
    phHal_sIso14443BInfo_t          Iso14443B_Info;
    phHal_sIso14443BPrimeInfo_t     Iso14443BPrime_Info;
    phHal_sNfcIPInfo_t              NfcIP_Info;
    phHal_sFelicaInfo_t             Felica_Info;
    phHal_sJewelInfo_t              Jewel_Info;
    phHal_sIso15693Info_t           Iso15693_Info;
}phHal_uRemoteDevInfo_t;

/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief RF Device Type Listing
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> RF Device Type List </em> is used to identify the type of
*  remote device that is discovered/connected. There seperate
*  types to identify a Remote Reader (denoted by _PCD) and 
*  Remote Tag (denoted by _PICC)
*  \note None.
*
*/

typedef enum phHal_eRFDevType
{
    phHal_eUnknown_DevType        = 0x00U,

    /* Specific PCD Devices */
    phHal_eISO14443_A_PCD,
    phHal_eISO14443_B_PCD,
    phHal_eISO14443_BPrime_PCD,
    phHal_eFelica_PCD,
    phHal_eJewel_PCD,
    phHal_eISO15693_PCD,
    /* Generic PCD Type */
    phHal_ePCD_DevType,

    /* Generic PICC Type */
    phHal_ePICC_DevType,
    /* Specific PICC Devices */
    phHal_eISO14443_A_PICC,
    phHal_eISO14443_4A_PICC,
    phHal_eISO14443_3A_PICC,
    phHal_eMifare_PICC,
    phHal_eISO14443_B_PICC,
    phHal_eISO14443_4B_PICC,
    phHal_eISO14443_BPrime_PICC,
    phHal_eFelica_PICC,
    phHal_eJewel_PICC,
    phHal_eISO15693_PICC,

    /* NFC-IP1 Device Types */
    phHal_eNfcIP1_Target, 
    phHal_eNfcIP1_Initiator, 

    /* Other Sources */
    phHal_eInvalid_DevType

}phHal_eRFDevType_t;

/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Remote Device Type Listing
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Remote Device Type List </em> is used to identify the type of
*  remote device that is discovered/connected
*  \note This is same as RF Device Type List.
*
*/

typedef phHal_eRFDevType_t 
            phHal_eRemDevType_t;

/** \ingroup grp_hal_common
 *
 *
 * \if hal
 *  \brief Common Command Attribute
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Hal Command Union </em> includes each available type of Commands.
 *
 * \note None.
 *
 */

typedef union phHal_uCommand
{
  phHal_eMifareCmdList_t         MfCmd;         /**< Mifare command structure.  */
  phHal_eIso14443_4_CmdList_t    Iso144434Cmd;  /**< ISO 14443-4 command structure.  */
  phHal_eFelicaCmdList_t         FelCmd;        /**< Felica command structure.  */
  phHal_eJewelCmdList_t          JewelCmd;      /**< Jewel command structure.  */
  phHal_eIso15693_CmdList_t      Iso15693Cmd;   /**< ISO 15693 command structure.  */
}phHal_uCmdList_t;


/** \ingroup grp_hal_nfci
 *
 * \if hal
 *  \brief Remote Device Information Structure
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Remote Device Information Structure </em> holds information about one single Remote
 *  Device detected by the polling function .\n
 *  It lists parameters common to all supported remote devices.
 *
 *  \note 
 *
 *  \if hal
 *   \sa \ref phHal4Nfc_ConfigureDiscovery and \ref phHal4Nfc_Connect
 *  \else
 *   \sa 
 *  \endif
 *
 */

typedef struct phHal_sRemoteDevInformation
{
    uint8_t                    SessionOpened;       /**< [out] Boolean 
                                                     *   Flag indicating the validity of
                                                     *   the handle of the remote device. */
    phHal_eRemDevType_t        RemDevType;          /**< [out] Remote device type which says that remote 
                                                    is Reader A or Reader B or NFCIP or Felica or 
                                                    Reader B Prime or Jewel*/
    phHal_uRemoteDevInfo_t     RemoteDevInfo;       /**< Union of available Remote Device.
                                                     *   \ref phHal_uRemoteDevInfo_t Information.  */
}phHal_sRemoteDevInformation_t;



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* This data structure is not used anymore and will be removed in subsequent
   release */

typedef struct phHal_sDevInputParam
{
    uint8_t FelicaPollPayload[5];          
                                           
                                           
    uint8_t NfcPollPayload[5];             
                                           
                                           
    uint8_t NFCIDAuto;                     
                                           
                                           
    uint8_t NFCID3i[PHHAL_NFCID_LENGTH];
                                           
                                           
                                           
    uint8_t DIDiUsed;                      
                                           
    uint8_t CIDiUsed;                      
                                           
    uint8_t NfcNADiUsed;                   
                                           
    /*+ MantisId : 31 - JP - 09-01-2006 */
        /*uint8_t TClNADiUsed; */          
                                           
    /*- MantisId : 31 - JP - 09-01-2006 */
    uint8_t GeneralByte[48];               
                                           
                                           
    uint8_t GeneralByteLength;             
                                           
    
    uint8_t ISO14443_4B_AFI;               
                                           
} phHal_sDevInputParam_t;




/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*    TARGET STRUCTURES */


/** \ingroup  grp_hal_common
 *
 * \if hal
 *  \brief Transceive Information Data Structure for sending commands/response 
 *         to the remote device
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Transceive Information Data Structure </em> is used to pass the 
 *  Command, Address (only required for MIFARE) and the send and receive data
 *  data structure (buffer and length) for communication with remote device
 *
 *
 */

typedef struct phHal_sTransceiveInfo
{
    phHal_uCmdList_t                cmd;

    /** \internal Address Field required for only Mifare
     *  Family Proprietary Cards.
     *  The Address Size is Valid only upto 255 Blocks limit
     *  i:e for Mifare 4K
     */
    uint8_t                         addr;
    phNfc_sData_t                   sSendData;
    phNfc_sData_t                   sRecvData;
}phHal_sTransceiveInfo_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Input information for the Type A tags
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sIso14443ACfg structure </em> holds the information 
*  required for the NFC device to be used during ISO14443A target discovery
*
*  \note None.
*/
typedef struct phHal_sIso14443ACfg
{
    uint8_t     Auto_Activation;       /**< Enable Auto Activation for 
                                    Technology A \n
                                    If set to 0, the activation procedure will stop 
                                    after Select (SAK has been received). 
                                    The host could evaluate SAK value and then decide:
                                        - to start communicating with the remote card 
                                          using proprietary commands (see NXP_MIFARE_RAW 
                                          and NXP_MIFARE_CMD)
                                    or
                                        - to activate the remote card up to ISO14443-4 
                                          level (RATS and PPS) using 
                                          CONTINUE ACTIVATION command 
                                    If set to 1, activation follows the flow described in
                                    ETSI HCI specification (restrict detection to 
                                    ISO14443-4 compliant cards).
                                    */
}phHal_sIso14443ACfg_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Input information for the Type B tags
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sIso14443BCfg structure </em> holds the information 
*  required for the NFC device to be used during ISO14443B target discovery
*
*  \note None.
*/
typedef struct phHal_sIso14443BCfg
{
    uint8_t     AppFamily_ID;       /**< Application Family Identifier for 
                                    Technology B, 0x00 means all application */
}phHal_sIso14443BCfg_t;

/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Input information for the Felica tags
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sFelicaCfg_t structure </em> holds the information 
*  required for the NFC device to be used during Felica target discovery
*
*  \note None.
*/

typedef struct phHal_sFelicaCfg
{
    uint8_t     SystemCode[PHHAL_FEL_SYS_CODE_LEN];     /**< System code for Felica tags */

}phHal_sFelicaCfg_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief Poll Device Information for conifiguring the discovery wheel
          Reader and Card Emulation Phases
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sPollDevInfo_t enum </em> is used to enable/disable 
*  phases of the discovery wheel related to specific reader types and 
*  card emulation phase
*  \note Enabling specific Reader technology when NFCIP1 speed is set in the
*        phHal_ADD_Cfg is implicitly done in HAL. Use this structure to only
*        enable/disable Card Reader Functionality
*/
typedef struct phHal_sPollDevInfo
{
    unsigned                    EnableIso14443A : 1;      /**< Flag to enable 
                                                        Reader A discovery */
    unsigned                    EnableIso14443B : 1;      /**< Flag to enable 
                                                        Reader B discovery */
    unsigned                    EnableFelica212 : 1;   /**< Flag to enable
                                                        Felica 212 discovery */
    unsigned                    EnableFelica424 : 1;   /**< Flag to enable
                                                        Felica 424 discovery */
    unsigned                    EnableIso15693 : 1;     /**< Flag to enable 
                                                        ISO 15693 discovery */
    unsigned                    EnableNfcActive : 1; /**< Flag to enable 
                                                        Active Mode of NFC-IP discovery. 
                                                        This is updated internally 
                                                        based on the NFC-IP speed.
                                                        */
    unsigned                    RFU : 1;                /**< Reserved for future use */
    unsigned                    DisableCardEmulation : 1;    /**< Flag to 
                                                            disable the card emulation */
}phHal_sPollDevInfo_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Identifies Type of Host
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to identify the type of the host providing the
*  information or the notification to the Terminal host.
*  \note None.
*/

typedef enum phHal_HostType {
    /* 
     * This type identifies the host controller
     * in the NFC device
     */
    phHal_eHostController       = 0x00U,
    /* 
     * This type identifies the Host Device
     * controlling the NFC device.
     */
    phHal_eTerminalHost         = 0x01U,
    /* 
     * This type identifies the uicc host
     * connnected to the NFC device
     */
    phHal_eUICCHost             = 0x02U,
    /* Host type is unknown */
    phHal_eUnknownHost          = 0xFFU
}phHal_HostType_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief P2P speed for the Initiator
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_eP2PMode enum </em> lists all the NFCIP1 speeds 
*  to be used for configuring the NFCIP1 discovery
*
*  \note None.
*/

typedef enum phHal_eP2PMode
{
    phHal_eDefaultP2PMode  = 0x00U,
    phHal_ePassive106 = 0x01U,
    phHal_ePassive212 = 0x02U, 
    phHal_ePassive424 = 0x04U, 
    phHal_eActive     = 0x08U, 
    phHal_eP2P_ALL    = 0x0FU,
    phHal_eInvalidP2PMode = 0xFFU
}phHal_eP2PMode_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Identities the type of Notification
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to specify the type of notification notified
*  to the upper layer. This classifies the notification into two types
*  one for the discovery notifications and the other for all the remaining
*  event notifications
*  \note None.
*/


typedef enum phHal_eNotificationType{
    INVALID_NFC_NOTIFICATION    = 0x00U, /* Invalid Notification */
    NFC_DISCOVERY_NOTIFICATION,         /* Remote Device Discovery Notification */
    NFC_EVENT_NOTIFICATION              /* Event Notification from the other hosts */
}phHal_eNotificationType_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Identifies the type of event notification
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to identify the type of the event notified
*  to the Terminal host.
*  \note None.
*/

typedef enum phHal_Event {


    /* Transaction Events */
    NFC_EVT_END_OF_TRANSACTION   = 0x11U ,
    NFC_EVT_TRANSACTION          = 0x12U ,
    NFC_EVT_START_OF_TRANSACTION = 0x20U ,

    /* Field Events */
    NFC_EVT_FIELD_ON             = 0x31U,
    NFC_EVT_FIELD_OFF            = 0x34U,

    /* Card/Target Activation Events */
    NFC_EVT_ACTIVATED           = 0x33U,
    NFC_EVT_DEACTIVATED         = 0x32U,

    NFC_EVT_PROTECTED           = 0x24U ,

    /* Reader Phases configuration request by UICC */
    NFC_UICC_RDPHASES_ACTIVATE_REQ = 0x43U,
    NFC_UICC_RDPHASES_DEACTIVATE_REQ = 0x44U,

    /* Connectivity and Triggering Events - Future Use */
    NFC_EVT_CONNECTIVITY         = 0x10U ,
    NFC_EVT_OPERATION_ENDED      = 0x13U ,

    /* NXP Specific System Information Events */
    NFC_INFO_TXLDO_OVERCUR       = 0x71U,
    NFC_INFO_MEM_VIOLATION       = 0x73U,
    NFC_INFO_TEMP_OVERHEAT       = 0x74U

}phHal_Event_t;

typedef struct phHal_sUiccInfo
{
    /* AID and Parameter Information is obtained if the
     * eventType is NFC_EVT_TRANSACTION.
     */
    phNfc_sData_t           aid;
    phNfc_sData_t           param;

}phHal_sUiccInfo_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Event notification Information
* \else
*  \brief HAL-Specific
* \endif
*
*  This structure provides the information about the event notified
*  to the terminal host.
*  \note None.
*/

typedef struct phHal_sEventInfo 
{
    /* Type of the host issuing the event */
    phHal_HostType_t    eventHost;
    /* Type of the source issuing the event */
    phHal_eRFDevType_t  eventSource;
    /* Type of the source issuing the event */
    phHal_Event_t       eventType;
    union   uEventInfo
    {
        /* Parameter information Information is obtained if the eventType is
         * NFC_EVT_TRANSACTION for UICC.
         */
        phHal_sUiccInfo_t       uicc_info;
        /* AID Information is obtained if the eventType is
         * NFC_EVT_TRANSACTION.
         */
        phNfc_sData_t           aid;
        /* Overheat Status Information is obtained if the eventType is
         * NFC_INFO_TEMP_OVERHEAT.
         */
        uint8_t                 overheat_status;
        /* rd_phases Information is obtained if the eventType is
         * NFC_UICC_RDPHASES_ACTIVATE_REQ.
         */
        uint8_t                 rd_phases;
        /* Remote Device Information is obtained if the eventType is
         * NFC_EVT_ACTIVATED.
         */
        phHal_sRemoteDevInformation_t *pRemoteDevInfo;
    }eventInfo;
}phHal_sEventInfo_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Host/Uicc Emulation Support
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sEmuSupport structure </em> holds the type 
*   of the target emulation supported.
*
*  \note None.
*/

typedef struct phHal_sEmuSupport
{
    unsigned int TypeA:1;
    unsigned int TypeB:1;
    unsigned int TypeBPrime:1;
    unsigned int TypeFelica:1;
    unsigned int TypeMifare:1;
    unsigned int TypeNfcIP1:1;
    unsigned int RFU:2;

}phHal_sEmuSupport_t;


/** \ingroup grp_hal_nfci
*
* \if hal
*  \brief P2P Information for the Initiator
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sNfcIPCfg </em> holds the P2P related information
*  use by the NFC Device during P2P Discovery and connection
*
*  \note None.
*/

typedef struct phHal_sNfcIPCfg 
{
    /* ATR_RES = General bytes length, Max length = 48 bytes */
    uint8_t         generalBytesLength;
    uint8_t         generalBytes[PHHAL_MAX_ATR_LENGTH];  

    /* TODO: This will be updated later for any additional params*/
}phHal_sNfcIPCfg_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Enumeration used to choose which type of parameters 
*         are to be configured
* \else
*  \brief HAL-Specific
* \endif
*
*  
*  \note None.
*/
typedef enum phHal_eConfigType
{
    NFC_INVALID_CONFIG  =   0x00U, /**< Invalid Configuration */
    NFC_RF_READER_CONFIG, /**< Reader Parmaeters */
    NFC_P2P_CONFIG,       /**< NFCIP1 Parameters */
    NFC_SE_PROTECTION_CONFIG, /**< Secure Element 
                                   Protection Cofiguration */
    NFC_EMULATION_CONFIG  /**< Emulation Parameters */
}phHal_eConfigType_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Discovery Configuration Mode
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to choose the Discovery Configuration
*  Mode :- Configure and Start, Stop or Start with last set 
*  configuration
*  \note None.
*/

typedef enum phHal_eDiscoveryConfigMode
{
    NFC_DISCOVERY_CONFIG  = 0x00U,/**< Configure discovery with values 
                                       in phHal_sADD_Cfg_t and start 
                                       discovery */
    NFC_DISCOVERY_START,         /**< Start Discovery with previously set
                                      configuration */
    NFC_DISCOVERY_STOP,          /**< Stop the Discovery */
    NFC_DISCOVERY_RESUME       /**< Resume the Discovery with previously 
                                   *  set configuration.
                                   *  This is valid only when the Target
                                   *  is not connected.
                                   */
}phHal_eDiscoveryConfigMode_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Target or Tag Release Mode
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration defines various modes of releasing an acquired target 
*  or tag.
*  \note None.
*/
typedef enum phHal_eReleaseType
{
    NFC_INVALID_RELEASE_TYPE    =0x00U,/**<Invalid release type */
    NFC_DISCOVERY_RESTART,      /**< Release current target and 
                                     restart discovery within same technology*/
    NFC_DISCOVERY_CONTINUE,    /**< Release current target and continue
                                    discovery with next technology in the wheel */
    NFC_SMARTMX_RELEASE    /**< Release SmartMX from wired mode to previous mode
                                (Virtual or Off) */
}phHal_eReleaseType_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Configuration of specific Emulation Feature
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to choose configuration for a specific
*  emulation feature.
*  \note None.
*/

typedef enum phHal_eEmulationType
{
    NFC_UNKNOWN_EMULATION       = 0x00U, /**< Invalid Configuration */
    NFC_HOST_CE_A_EMULATION     = 0x01U, /**< Configure parameters for Type A 
                                              card emulation from host */
    NFC_HOST_CE_B_EMULATION     = 0x02U, /**< Configure parameters for Type B 
                                              card emulation from host */
    NFC_B_PRIME_EMULATION       = 0x03U, /**< Configure parameters for Type B' 
                                              card emulation from host */
    NFC_FELICA_EMULATION        = 0x04U, /**< Configure parameters for Type F 
                                              card emulation from host */
    NFC_MIFARE_EMULATION        = 0x06U, /**< Configure parameters for MIFARE 
                                              card emulation - For Future Use */
    NFC_SMARTMX_EMULATION       = 0x07U, /**< Configure parameters for SmartMX 
                                            */
    NFC_UICC_EMULATION          = 0x08U  /**< Configure parameters for UICC 
                                            emulation */
}phHal_eEmulationType_t;

#if 0
/** \ingroup grp_hal_nfct
 *
 * \if hal
 *  \brief Information for Target Mode Start-Up
 * \else
 *  \brief HAL-Specific
 * \endif
 *
 *  The <em> Target Information Structure </em> required to start Target mode. 
 *  It contains all the information for the Target mode.
 *
 *  \note None.
 *
 */

typedef struct phHal_sTargetInfo
{
    uint8_t                 enableEmulation;
    phHal_sNfcIPCfg_t       targetConfig;
} phHal_sTargetInfo_t;
#endif


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Mode of operation for SmartMX
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to choose the mode of operation for the SmartMx Module.
*  Default static configuration at initialization time.
*  \note None.
*/

typedef enum phHal_eSmartMX_Mode{
    eSmartMx_Wired      = 0x00U, /* SmartMX is in Wired Mode */
    eSmartMx_Default,            /* SmartMX is in Default Configuration Mode */
    eSmartMx_Virtual,            /* SmartMx in the Virutal Mode */
    eSmartMx_Off                 /* SmartMx Feature is Switched off */
} phHal_eSmartMX_Mode_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Mode of operation for SWP
* \else
*  \brief HAL-Specific
* \endif
*
*  This enumeration is used to choose the mode of operation for the SWP Link 
*  for UICC Module. Default static configuration at initialization time.
*  \note None.
*/

typedef enum phHal_eSWP_Mode{
    eSWP_Switch_Off      = 0x00U,   /* SWP Link is Switched off */
    eSWP_Switch_Default,            /* SWP is in Default Configuration Mode */
    eSWP_Switch_On                  /* SWP Link is Switched on */
} phHal_eSWP_Mode_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the SmartMX 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sSmartMX_Cfg structure </em> holds the information 
*   to configure the SmartMX Module in the NFC Device.
*
*  \note None.
*/


typedef struct phHal_sSmartMX_Cfg
{
    uint8_t                 enableEmulation;
    uint8_t                 lowPowerMode;
    phHal_eSmartMX_Mode_t   smxMode;
}phHal_sSmartMX_Cfg_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the UICC 
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sUiccEmuCfg structure </em> holds the information 
*   to configure the UICC Host.
*
*  \note None.
*/


typedef struct phHal_sUiccEmuCfg
{
    uint8_t             enableUicc;
    uint8_t             uiccEmuSupport;
    uint8_t             uiccReaderSupport;
    uint8_t             lowPowerMode;
    /* TODO: This will be updated later */
}phHal_sUiccEmuCfg_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the Type A Host Emulation Feature
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sHostEmuCfg_A structure </em> holds the information 
*   to configure the Host Emulation for Type A.
*
*  \note None.
*/

typedef struct phHal_sHostEmuCfg_A
{
    uint8_t                 enableEmulation;
    phHal_sIso14443AInfo_t  hostEmuCfgInfo;
    uint8_t                 enableCID;
}phHal_sHostEmuCfg_A_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the Type B Host Emulation Feature
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sHostEmuCfg_B structure </em> holds the information 
*   to configure the Host Emulation for Type B.
*
*  \note None.
*/

typedef struct phHal_sHostEmuCfg_B
{
    uint8_t                 enableEmulation;
    phHal_sIso14443BInfo_t  hostEmuCfgInfo;
}phHal_sHostEmuCfg_B_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the Felica Host Emulation Feature
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sHostEmuCfg_F structure </em> holds the information 
*   to configure the Felica Host Emulation.
*
*  \note None.
*/


typedef struct phHal_sHostEmuCfg_F
{
    uint8_t                 enableEmulation;
}phHal_sHostEmuCfg_F_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the Emulation
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sEmulationCfg structure </em> holds the information 
*   required for the device to act as a Tag or NFCIP1 Target.
*
*  \note phHal_sHostEmuCfg_F_t Type F emulation is not presently supported
*  is reserved for future use. 
*/

typedef struct phHal_sEmulationCfg
{
    phHal_HostType_t        hostType;
    phHal_eEmulationType_t  emuType;
    union phHal_uEmuConfig
    {
        phHal_sSmartMX_Cfg_t    smartMxCfg;
        phHal_sHostEmuCfg_A_t   hostEmuCfg_A;
        phHal_sHostEmuCfg_B_t   hostEmuCfg_B;
        phHal_sHostEmuCfg_F_t   hostEmuCfg_F;
        phHal_sUiccEmuCfg_t     uiccEmuCfg;
    }config;
}phHal_sEmulationCfg_t;

/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for the Configuring the Reader parameters
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sReaderCfg structure </em> holds the information 
*   to configure the Reader A or Reader B parameters.
*
*  \note None.
*/

typedef struct phHal_sReaderCfg
{
    phHal_eRFDevType_t    readerType;
    union phHal_uReaderCfg
    {
        phHal_sIso14443ACfg_t       Iso14443ACfg;
        phHal_sIso14443BCfg_t       Iso14443BCfg;
    }config;
}phHal_sReaderCfg_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Information for Configuring the Protected Mode for 
*  the Secure Elements.
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_sSEProtectionCfg structure </em> holds the 
*  information to configure the Secure Element Protection configuration.
*
*  \note None.
*/

typedef struct phHal_sSEProtectionCfg
{
    uint8_t         mode;
}phHal_sSEProtectionCfg_t;


/** \ingroup  grp_hal_common
*
* \if hal
*  \brief Poll configuration structure
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> Poll configuration structure </em> holds information about the 
*  enabling the the type of discovery required by the application. This 
*  structure is the input parameter for the discovery call
*
*  \note All members of this structure are input parameters [out].
*
*  \if hal
*   \sa \ref phHal4Nfc_Connect, \ref phHal4Nfc_ConfigParameters, 
*            \ref phHal_eP2PMode_t and \ref phHal4Nfc_Disconnect.
*  \endif
*
*/

typedef struct phHal_sADD_Cfg
{
    union 
    {
        phHal_sPollDevInfo_t        PollCfgInfo;        /**<  Enable/Disable Specific 
                                                              Reader Functionality and 
                                                              Card Emulation */ 
        unsigned                    PollEnabled;     /** Can be used to set polling 'Off'
                                                      by setting PollEnabled to zero */

    }PollDevInfo;
    uint32_t                    Duration;           /**< Duration of virtual or idle 
                                                    period in microseconds in the step size
                                                    of 48 microseconds.If duration is set less
                                                    than  48 microseconds then default value is
                                                    used.For more details please refer PN 544 
                                                    user manual*/
    uint8_t                     NfcIP_Mode ;      /**< Select the P2P
                                                    speeds using phHal_eP2PMode_t type.
                                                    This is used to enable NFC-IP Discovery 
                                                    The related Reader Type will be implicitly
                                                    selected */
    uint8_t                     NfcIP_Tgt_Disable;   /**< Flag to 
                                                   disable the NFCIP1 TARGET */
}phHal_sADD_Cfg_t;


/** \ingroup grp_hal_common
*
* \if hal
*  \brief Configuration information.
* \else
*  \brief HAL-Specific
* \endif
*
*  The <em> \ref phHal_uConfig structure </em> holds the information 
*   required for Configuring the Device.
*
*  \note None.
*/


typedef union phHal_uConfig
{
    phHal_sEmulationCfg_t emuConfig;  
    phHal_sNfcIPCfg_t     nfcIPConfig;    /**< Gives the information about
                                           *  the General Bytes for NFC-IP 
                                           *  Communication.
                                           */
    phHal_sReaderCfg_t       readerConfig;
    phHal_sSEProtectionCfg_t protectionConfig;
}phHal_uConfig_t;


#endif

/* EOF */


