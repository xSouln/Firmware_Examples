//==============================================================================
#ifndef _MAX30208_CONTROL_H
#define _MAX30208_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "MAX30208_Types.h"
//==============================================================================
//functions:

xResult MAX30208_ControlInit(MAX30208_ControlT* device, void* parent);
void MAX30208_TimeSynchronization(MAX30208_ControlT* device);
void MAX30208_ControlHandler(MAX30208_ControlT* device);

void MAX30208_PutValueToBuffer(MAX30208_ControlT* device, MAX30208_TemperatureT value);
int MAX30208_ReadValueFromBuffer(MAX30208_ControlT* device, uint8_t* buffer, uint16_t max_points);
void MAX30208_ResetBuffer(MAX30208_ControlT* device);
uint16_t MAX30208_AccelerometerValuesInBuffer(MAX30208_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
