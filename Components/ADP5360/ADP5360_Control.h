//==============================================================================
#ifndef _ADP5360_CONTROL_H
#define _ADP5360_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "ADP5360_Types.h"
//==============================================================================
//functions:

xResult ADP5360_ControlInit(ADP5360_ControlT* device, void* parent);

void ADP5360_TimeSynchronization(ADP5360_ControlT* device);
void ADP5360_ControlHandler(ADP5360_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
