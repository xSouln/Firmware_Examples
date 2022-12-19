//==============================================================================
#ifndef _HABIBAND_TYPES_H
#define _HABIBAND_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "Habiband_Config.h"
#include "Habiband_BLE.h"
#include "Habiband/Habiband_Bootloader.h"
#include "LSM6DSOX/LSM6DSOX_Control.h"
#include "MAX30208/MAX30208_Control.h"
#include "AFE49I30/AFE49I30_Control.h"
#include "ADP5360/ADP5360_Control.h"
#include "LM2755/LM2755_Control.h"
//==============================================================================
//types:

/**
 * @brief available events
 */
typedef enum
{
	HabibandEventIdle,
	HabibandEventTemperatureChanged,

	HabibandEventBLE_Disconnected,
	HabibandEventBLE_Connected,
	HabibandEventBLE_TemperatureNotificationEnnabled,
	HabibandEventBLE_TemperatureNotificationDisabled,
	HabibandEventBLE_AccelerometerNotificationEnnabled,
	HabibandEventBLE_AccelerometerNotificationDisabled,
	HabibandEventBLE_GyroscopeNotificationEnnabled,
	HabibandEventBLE_GyroscopeNotificationDisabled,
	HabibandEventBLE_ECG_PointsNotificationEnnabled,
	HabibandEventBLE_ECG_PointsNotificationDisabled,
	HabibandEventBLE_PPG_PointsNotificationEnnabled,
	HabibandEventBLE_PPG_PointsNotificationDisabled,
	HabibandEventBLE_ECG_StatusNotificationEnnabled,
	HabibandEventBLE_ECG_StatusNotificationDisabled,

} HabibandEventSelector;
//------------------------------------------------------------------------------

typedef enum
{
	HabibandRequestIdle,
	HabibandRequestDelay

} HabibandRequestSelector;
//------------------------------------------------------------------------------

typedef enum
{
	HabibandValueIdle,

} HabibandValueSelector;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	HabibandBLE_T BLE;

	LSM6DSOX_ControlT LSM6DSOX;
	MAX30208_ControlT MAX30208;
	AFE49I30_ControlT AFE49I30;
	ADP5360_ControlT ADP5360;
	LM2755_ControlT LM2755;

} HabibandT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
