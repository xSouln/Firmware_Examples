//==============================================================================
#ifndef _HABIBAND_CONTROL_H
#define _HABIBAND_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Habiband_Types.h"
//==============================================================================
//functions:

xResult HabibandInit(HabibandT* device, void* parent);
void HabibandTimeSynchronization(HabibandT* device);
void HabibandControlHandler(HabibandT* device);

void HabibandEventListener(HabibandT* device, HabibandEventSelector selector, uint32_t args, uint32_t count);
xResult HabibandRequestListener(HabibandT* device, HabibandRequestSelector selector, uint32_t args, uint32_t count);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
