//==============================================================================
#ifndef _LSM6DSOX_CONFIG_H
#define _LSM6DSOX_CONFIG_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Config.h"
//==============================================================================
//defines:

#define LSM6DSOX_TEMPERATURE_UPDATE_TIME 1000
#define LSM6DSOX_GYROSCOPE_UPDATE_TIME 20
#define LSM6DSOX_ACCELEROMETER_UPDATE_TIME 20

#define LSM6DSOX_GYROSCOPE_BUFFER_SIZE_MASK (0x7f)
#define LSM6DSOX_GYROSCOPE_BUFFER_SIZE (LSM6DSOX_GYROSCOPE_BUFFER_SIZE_MASK + 1)

#define LSM6DSOX_ACCELEROMETER_BUFFER_SIZE_MASK (0x7f)
#define LSM6DSOX_ACCELEROMETER_BUFFER_SIZE (LSM6DSOX_ACCELEROMETER_BUFFER_SIZE_MASK + 1)

#define LSM6DSOX_TEMPERATURE_BUFFER_SIZE_MASK (0x7f)
#define LSM6DSOX_TEMPERATURE_BUFFER_SIZE (LSM6DSOX_TEMPERATURE_BUFFER_SIZE_MASK + 1)
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
