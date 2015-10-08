
 // * $Revision: 1.1 $
 
#ifndef PHOSALNFC_MSG_H
#define PHOSALNFC_MSG_H
#ifdef  _WIN32
#include <phOsalNfc_MsgX86.h>
#else
#ifdef __linux__
#include <phOsalNfc.h>
#include <phOsalNfc_Common.h>
#else
#include <phOsalNfc_MsgRtk.h>
#endif
#endif

#endif  /*  PHOSALNFC_MSG_H  */