//==============================================================================
#ifndef _MAX30208_TYPES_H
#define _MAX30208_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "MAX30208_Config.h"
#include "MAX30208/max30208.h"
//==============================================================================
//types:

typedef float MAX30208_TemperatureT;
//------------------------------------------------------------------------------

typedef struct
{
	MAX30208_TemperatureT Values[MAX30208_TEMPERATURE_BUFFER_SIZE];

	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} MAX30208_TemperatureBufferT;
//------------------------------------------------------------------------------

typedef enum
{
	MAX30208_ConversionStateIdle,
	MAX30208_ConversionStateStarting,
	MAX30208_ConversionStateStarted,
	MAX30208_ConversionStateComplite,

} MAX30208_ConversionStates;
//------------------------------------------------------------------------------

typedef struct
{
	uint8_t Conversion : 3;

} MAX30208_StatusT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t TemperatureChanged : 1;
		uint8_t TemperaturePointsChanged : 1;
		uint8_t TemperatureIsUpdate : 1;
		uint8_t TemperatureUpdateError : 1;
	};

	uint8_t Value;

} MAX30208_EventsT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t UpdateTime;

} MAX30208_OptionsT;
//------------------------------------------------------------------------------

typedef struct DeviceMAX30208_T
{
	void* Description;
	void* Parent;

	MAX30208_T Driver;

	MAX30208_OptionsT Options;
	MAX30208_StatusT Status;
	MAX30208_EventsT Events;

	float Temperature;
	MAX30208_TemperatureBufferT TemperaturePoints;

} MAX30208_ControlT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
