//==============================================================================
#ifndef _LM2755_CONTROL_H
#define _LM2755_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "LM2755_Types.h"
//==============================================================================
//functions:

xResult LM2755_ControlInit(LM2755_ControlT* device, void* parent);

void LM2755_ControlHandler(LM2755_ControlT* device);
void LM2755_TimeSynchronization(LM2755_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
