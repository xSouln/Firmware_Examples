//==============================================================================
#ifndef _HABIBAND_BLE_H
#define _HABIBAND_BLE_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "Habiband_Config.h"
//==============================================================================
//types:

typedef enum
{
	HabibandBLE_RequestIdle,

	HabibandBLE_RequestWriteTemperature,
	HabibandBLE_RequestWriteAccelerometerPoints,
	HabibandBLE_RequestWriteGyroscopePoints,
	HabibandBLE_RequestWriteECG_Points,
	HabibandBLE_RequestWritePPG_Points,
	HabibandBLE_RequestWriteECG_Status

} HabibandBLE_RequestSelector;
//------------------------------------------------------------------------------

typedef enum
{
	HabibandBLE_ValueIdle,

	HabibandBLE_ValueTemperatureCharacteristicSize,
	HabibandBLE_ValueAccelerometerPointsCharacteristicSize,
	HabibandBLE_ValueGyroscopePointsCharacteristicSize,
	HabibandBLE_ValueECG_PointsCharacteristicSize,
	HabibandBLE_ValuePPG_PointsCharacteristicSize,

} HabibandBLE_ValueSelector;
//------------------------------------------------------------------------------

typedef xResult (*HabibandBLE_RequestListenerT)(void* device, HabibandBLE_RequestSelector selector, uint32_t args, uint16_t count);
typedef uint32_t (*HabibandBLE_GetValueActionT)(void* device, HabibandBLE_ValueSelector selector);
//------------------------------------------------------------------------------

typedef struct
{
	HabibandBLE_RequestListenerT RequestListener;

	HabibandBLE_GetValueActionT GetValue;

} HabibandBLE_InterfaceT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		struct
		{
			uint32_t Temperature : 1;
			uint32_t Accelerometer : 1;
			uint32_t Gyroscope : 1;
			uint32_t ECG_Points : 1;
			uint32_t PPG_Points : 1;
			uint32_t ECG_Status : 1;

		} Read;
	};

	uint32_t Value;

} HabibandBLE_RequestsT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint32_t Temperature : 1;
		uint32_t Accelerometer : 1;
		uint32_t Gyroscope : 1;
		uint32_t ECG_Points : 1;
		uint32_t PPG_Points : 1;
		uint32_t EKG_Status : 1;
	};

	uint32_t Value;

} HabibandBLE_NotificationsT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint32_t ConnectionStateChanged : 1;
		uint32_t TemperatureNotificationStateChanged : 1;
		uint32_t AccelerometerNotificationStateChanged : 1;
		uint32_t GyroscopeNotificationStateChanged : 1;
		uint32_t ECG_PointsNotificationStateChanged : 1;
		uint32_t PPG_PointsNotificationStateChanged : 1;
		uint32_t ECG_StatusNotificationStateChanged : 1;
	};

	uint32_t Value;

} HabibandBLE_EventsT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint32_t IsConnected : 1;
	};

	uint32_t Value;

} HabibandBLE_StatusT;
//------------------------------------------------------------------------------
/**
 * @brief the number of points included in the maximum size of the characteristic
 */
typedef struct
{
	uint8_t Accelerometer;
	uint8_t Gyroscope;
	uint8_t ECG;
	uint8_t PPG;

} HabibandBLE_ValuesInCharacteristicT;
//------------------------------------------------------------------------------

typedef struct
{
	HabibandBLE_StatusT Status;
	HabibandBLE_RequestsT Requests;
	HabibandBLE_NotificationsT Notifications;
	HabibandBLE_EventsT Events;
	HabibandBLE_ValuesInCharacteristicT ValuesInCharacteristic;

	HabibandBLE_InterfaceT* Interface;

} HabibandBLE_T;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
