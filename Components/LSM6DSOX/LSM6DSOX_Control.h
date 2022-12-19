//==============================================================================
#ifndef _LSM6DSOX_CONTROL_H
#define _LSM6DSOX_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "LSM6DSOX_Types.h"
//==============================================================================
//functions:

xResult LSM6DSOX_ControlInit(LSM6DSOX_ControlT* device, void* parent);
void LSM6DSOX_TimeSynchronization(LSM6DSOX_ControlT* device);
void LSM6DSOX_ControlHandler(LSM6DSOX_ControlT* device);

void LSM6DSOX_PutGyroscopeValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_GyroscopeValueT value);
int LSM6DSOX_ReadGyroscopeValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points);
uint16_t LSM6DSOX_GyroscopeValuesInBuffer(LSM6DSOX_ControlT* device);
void LSM6DSOX_ResetGyroscopeBuffer(LSM6DSOX_ControlT* device);

void LSM6DSOX_PutAccelerometerValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_AccelerometerValueT value);
int LSM6DSOX_ReadAccelerometerValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points);
uint16_t LSM6DSOX_AccelerometerValuesInBuffer(LSM6DSOX_ControlT* device);
void LSM6DSOX_ResetAccelerometerBuffer(LSM6DSOX_ControlT* device);

void LSM6DSOX_PutTemperatureValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_TemperatureValueT value);
int LSM6DSOX_ReadTemperatureValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points);
uint16_t LSM6DSOX_TemperatureValuesInBuffer(LSM6DSOX_ControlT* device);
void LSM6DSOX_ResetTemperatureBuffer(LSM6DSOX_ControlT* device);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
