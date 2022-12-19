//==============================================================================
#ifndef _BOOTLOADER_COMPONENT_H
#define _BOOTLOADER_COMPONENT_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
//includes:

#include "Bootloader.h"
//==============================================================================
//functions:

int BootloaderComponentInit(void* parent);
//==============================================================================
//export:

extern BootloaderT Bootloader;

extern void BootloaderComponentTimeSynchronization();
extern void BootloaderComponentHandler();
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif

