//==============================================================================
#ifndef _DRV2605_CONTROL_H
#define _DRV2605_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "DRV2605_Types.h"
//==============================================================================
//functions:

xResult DRV2605_ControlInit(DRV2605_ControlT* device, void* parent);

void DRV2605_ControlHandler(DRV2605_ControlT* device);
void DRV2605_TimeSynchronization(DRV2605_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
