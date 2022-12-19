//==============================================================================
#ifndef _LSM6DSOX_TYPES_H
#define _LSM6DSOX_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "LSM6DSOX_Config.h"
#include "LSM6DSOX/lsm6dsox_reg.h"
//==============================================================================
//types:

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;

} LSM6DSOX_GyroscopeValueT;
//------------------------------------------------------------------------------

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;

} LSM6DSOX_AccelerometerValueT;
//------------------------------------------------------------------------------

typedef float LSM6DSOX_TemperatureValueT;
//------------------------------------------------------------------------------

typedef struct
{
	LSM6DSOX_GyroscopeValueT Values[LSM6DSOX_GYROSCOPE_BUFFER_SIZE];

	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} LSM6DSOX_GyroscopeBufferT;
//------------------------------------------------------------------------------

typedef struct
{
	LSM6DSOX_AccelerometerValueT Values[LSM6DSOX_ACCELEROMETER_BUFFER_SIZE];

	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} LSM6DSOX_AccelerometerBufferT;
//------------------------------------------------------------------------------

typedef struct
{
	LSM6DSOX_TemperatureValueT Values[LSM6DSOX_TEMPERATURE_BUFFER_SIZE];

	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} LSM6DSOX_TemperatureBufferT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t TemperatureValueChanged : 1;
		uint8_t TemperaturePointsChanged : 1;

		uint8_t GyroscopeValueChanged : 1;
		uint8_t GyroscopePointsChanged : 1;

		uint8_t AccelerometerValueChanged : 1;
		uint8_t AccelerometerPointsChanged : 1;
	};

	uint8_t Value;

} LSM6DSOX_EventsT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t GyroscopeUpdateTime;
	uint16_t AccelerometerUpdateTime;
	uint16_t TemperatureUpdateTime;

} LSM6DSOX_OptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	stmdev_ctx_t Bus;

	uint8_t Id;
	LSM6DSOX_OptionsT Options;

	LSM6DSOX_EventsT Events;

	LSM6DSOX_GyroscopeValueT GyroscopeValue;
	LSM6DSOX_AccelerometerValueT AccelerometerValue;
	LSM6DSOX_TemperatureValueT TemperatureValue;

	LSM6DSOX_GyroscopeBufferT GyroscopeBuffer;
	LSM6DSOX_AccelerometerBufferT AccelerometerBuffer;
	LSM6DSOX_TemperatureBufferT TemperatureBuffer;

} LSM6DSOX_ControlT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
