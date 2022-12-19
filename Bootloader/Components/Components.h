//==============================================================================
#ifndef _COMPONENTS_H
#define _COMPONENTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Components_Config.h"
#include "Bootloader/Bootloader_Component.h"
#include "Habiband/Habiband_Bootloader.h"
//==============================================================================
//functions:

int ComponentsInit(void* parent);

extern void ComponentsHandler();
extern void ComponentsTimeSynchronization();
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
