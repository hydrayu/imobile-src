#ifndef PHDAL4NFC_DEFERREDCALL_H
#define PHDAL4NFC_DEFERREDCALL_H

#ifdef PH_NFC_CUSTOMINTEGRATION
#include <phNfcCustomInt.h>
#else

typedef  void (*pphDal4Nfc_Deferred_Call_t/* *nfc_dal_def_call_t*/) (void*);

typedef struct {
    /*nfc_dal_def_call_t def_call;*/
    pphDal4Nfc_Deferred_Call_t def_call;
    void* params;
} phDal4Nfc_DeferredCall_Msg_t;/*nfc_def_call_msg_t;*/



void phDal4Nfc_DeferredCall(pphDal4Nfc_Deferred_Call_t func, void *param);
#endif 
#endif


