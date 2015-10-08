
// * $Revision: 1.1 $

#ifndef PHDAL4NFC_DEFER_CALL_H
#define PHDAL4NFC_DEFER_CALL_H
/**
*  \name DAL NFC 
*
* File: \ref phDal4Nfc_DeferCall.h
*
*/
/*@{*/
#define PHDAL4NFC_DEFER_CALL_FILEREVISION "$Revision: 1.1 $" /**< \ingroup grp_file_attributes */
#define PHDAL4NFC_DEFER_CALL_FILEALIASES    "$ $"     /**< \ingroup grp_file_attributes */
/*@}*/
/*************************** Includes *******************************/

/*********************** End of includes ****************************/
/***************************** Macros *******************************/

/************************ End of macros *****************************/

/********************** Callback functions **************************/
typedef  void (*pphDal4Nfc_DeferFuncPointer_t) (void*);
/******************* End of Callback functions **********************/

/********************* Structures and enums *************************/
typedef struct {
    pphDal4Nfc_DeferFuncPointer_t def_call;
    void* params;
} phDal4Nfc_DeferMsg_t;

/****************** End of structures and enums *********************/

/******************** Function declarations *************************/

void phDal4Nfc_DeferredCall(pphDal4Nfc_DeferFuncPointer_t func, void *param);

/****************** End of function declarations ********************/

#endif /* PHDAL4NFC_DEFERRED_CALL_H */

