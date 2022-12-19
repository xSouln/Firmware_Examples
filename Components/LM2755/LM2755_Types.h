//==============================================================================
#ifndef _LM2755_TYPES_H
#define _LM2755_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "LM2755_Config.h"
#include "LM2755/LM2755.h"
//==============================================================================
//types:

typedef enum
{
	LM2755_LedRed,
	LM2755_LedGreen,
	LM2755_LedBlue,

} LM2755_Leds;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_LedRedBit = 1 << LM2755_LedRed,
	LM2755_LedGreenBit = 1 << LM2755_LedGreen,
	LM2755_LedBlueBit = 1 << LM2755_LedBlue,

} LM2755_LedsSelector;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t UpdateTime;

} LM2755_ControlOptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	LM2755_T Driver;

	LM2755_ControlOptionsT Options;

} LM2755_ControlT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
