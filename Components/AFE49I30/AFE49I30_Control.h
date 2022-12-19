//==============================================================================
#ifndef _AFE49I30_CONTROL_H
#define _AFE49I30_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "AFE49I30_Types.h"
//==============================================================================
//functions:

xResult AFE49I30_ControlInit(AFE49I30_ControlT* device, void* parent);
void AFE49I30_ControlHandler(AFE49I30_ControlT* pdevice);
void AFE49I30_TimeSynchronization(AFE49I30_ControlT* device);

void AFE49I30_ResetBuffer(AFE49I30_ControlT* device);

uint16_t AFE49I30_PPG_ValuesInBuffer(AFE49I30_ControlT* device);
uint16_t AFE49I30_ECG_ValuesInBuffer(AFE49I30_ControlT* device);

int AFE49I30_Read_ECG_ValuesFromBuffer(AFE49I30_ControlT* device, uint8_t* buffer, uint16_t max_points);
int AFE49I30_Read_PPG_ValuesFromBuffer(AFE49I30_ControlT* device, uint8_t* buffer, uint16_t max_points);

int AFE49I30_EnableECG(AFE49I30_ControlT* device);
int AFE49I30_EnablePPG(AFE49I30_ControlT* device);

int AFE49I30_StopMeasure(AFE49I30_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
