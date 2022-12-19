//==============================================================================
#ifndef _BOOTLOADER_ADAPTER_H
#define _BOOTLOADER_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "Bootloader/Bootloader_Types.h"
//==============================================================================
//types:

typedef struct
{
	uint16_t PageSize;
	uint16_t PagesCount;

} BootloaderAdapterT;
//==============================================================================
//functions:

xResult BootloaderAdapterInit(BootloaderT* device, BootloaderAdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
