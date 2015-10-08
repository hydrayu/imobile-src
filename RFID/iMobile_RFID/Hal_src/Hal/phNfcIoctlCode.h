
// $Revision: 1.11 $

#ifndef PHNFCIOCTLCODE_H /* */
#define PHNFCIOCTLCODE_H /* */

/**
 *  \name IOCTL Codes
 * 
 *  File: \ref phNfcIoctlCode.h
 * 
 */
/*@{*/
#define PHNFCIOCTLCODE_FILEREVISION "$Revision: 1.11 $" /**< \ingroup grp_file_attributes */
#define PHNFCIOCTLCODE_FILEALIASES  "$Aliases: NFC_FRI1.1_WK1023_R35_1 $"     /**< \ingroup grp_file_attributes */
/*@}*/



/* The Return Status for the IOCTL Operation */
#define NFC_IO_SUCCESS                      (0x90U)
#define NFC_IO_ERROR                        (0x9FU)




#define	NFC_GPIO_READ				        (0xF82AU)

#define	NFC_FW_DOWNLOAD				        (0x09FFU)
#define	NFC_FW_DOWNLOAD_CHECK		        (0x09F7U)

#define NFC_ANTENNA_CWG                     (0x989FU)


/* The PN544 DEVICE Management Control : 0x90*/
#define DEVMGMT_CTL							(0x90U)


/* Ioctl codes for PN544 System Tests */
#define DEVMGMT_TEST_MASK					(0xFFU)
#define	DEVMGMT_ANTENNA_TEST				((DEVMGMT_CTL << 8)|(0x20U))
#define	DEVMGMT_SWP_TEST					((DEVMGMT_CTL << 8)|(0x21U))
#define	DEVMGMT_NFCWI_TEST					((DEVMGMT_CTL << 8)|(0x22U))
#define	DEVMGMT_PRBS_TEST					((DEVMGMT_CTL << 8)|(0x25U))

#define NFC_MEM_READ                        (0xD0U)
#define NFC_MEM_WRITE                       (0xD1U)


#if 0
#define	DEVMGMT_HOSTINTERFACE_TEST		    ((DEVMGMT_CTL << 8)|(0x23U))
#endif


#endif /* PHNFCIOCTLCODE */




