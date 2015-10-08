
// $Revision: 1.13 $

#ifndef PHNFCTYPES /* */
#define PHNFCTYPES /* */

/**
 *  \name NFC Types
 *
 * File: \ref phNfcTypes.h
 *
 */
/*@{*/
#define PHNFCTYPES_FILEREVISION "$Revision: 1.13 $" /**< \ingroup grp_file_attributes */
#define PHNFCTYPES_FILEALIASES  "$Aliases: NFC_FRI1.1_WK926_R28_1,NFC_FRI1.1_WK928_R29_1,NFC_FRI1.1_WK930_R30_1,NFC_FRI1.1_WK934_PREP_1,NFC_FRI1.1_WK934_R31_1,NFC_FRI1.1_WK941_PREP1,NFC_FRI1.1_WK941_PREP2,NFC_FRI1.1_WK941_1,NFC_FRI1.1_WK943_R32_1,NFC_FRI1.1_WK949_PREP1,NFC_FRI1.1_WK943_R32_10,NFC_FRI1.1_WK943_R32_13,NFC_FRI1.1_WK943_R32_14,NFC_FRI1.1_WK1007_R33_1,NFC_FRI1.1_WK1007_R33_4,NFC_FRI1.1_WK1017_PREP1,NFC_FRI1.1_WK1017_R34_1,NFC_FRI1.1_WK1017_R34_2,NFC_FRI1.1_WK1023_R35_1 $"     /**< \ingroup grp_file_attributes */
/*@}*/

#ifndef _WIN32
#include <stdint.h>
#else
#include <stdio.h>
#define snprintf _snprintf

#ifndef linux
/**
 *  \name Basic Type Definitions
 *
 *  Constant-length-type definition ('C99).
 *
 */
/*@{*/
#ifndef __int8_t_defined /* */
#define __int8_t_defined /* */
typedef signed   char   int8_t;         /**<  \ingroup grp_nfc_common
                                               8 bit signed integer */
#endif

#ifndef __int16_t_defined /* */
#define __int16_t_defined /* */
typedef signed   short  int16_t;        /**< \ingroup grp_nfc_common
                                             16 bit signed integer */
#endif

#ifndef __stdint_h
#ifndef __int32_t_defined /* */
#define __int32_t_defined /* */
typedef signed   long   int32_t;        /**< \ingroup grp_nfc_common
                                             32 bit signed integer */
#endif
#endif

#ifndef __uint8_t_defined /* */
#define __uint8_t_defined /* */
typedef unsigned char   uint8_t;        /**<  \ingroup grp_nfc_common
                                              8 bit unsigned integer */
#endif

#ifndef __uint16_t_defined /* */
#define __uint16_t_defined /* */
typedef unsigned short  uint16_t;       /**< \ingroup grp_nfc_common
                                             16 bit unsigned integer */
#endif

#ifndef __stdint_h
#ifndef __uint32_t_defined /* */
#define __uint32_t_defined /* */
typedef unsigned long   uint32_t;       /**< \ingroup grp_nfc_common
                                             32 bit unsigned integer */
#endif
#endif

#endif /* linux */

#endif /* _WIN32 */

#ifndef TRUE
#define TRUE			(0x01)			  /**< \ingroup grp_nfc_common
                                              Logical True Value */
#endif

#ifndef FALSE
#define FALSE			(0x00)			  /**< \ingroup grp_nfc_common
                                              Logical False Value */
#endif

typedef uint8_t			utf8_t;			/**< \ingroup grp_nfc_common
                                             UTF8 Character String */

typedef uint8_t			bool_t;			/**< \ingroup grp_nfc_common
												boolean data type */

typedef uint16_t        NFCSTATUS;      /**< \ingroup grp_nfc_common
                                             NFC-FRI and HAL return values 
                                         \ref phNfcStatus.h for different status
                                          values */

#ifndef NULL
#define NULL  ((void *)0)
#endif

/* This Macro to be used to resolve Unused and unreference
 * compiler warnings. 
 */

#define PHNFC_UNUSED_VARIABLE(x) for((x)=(x);(x)!=(x);)

/*@}*/

#endif /* PHNFCTYPES */

