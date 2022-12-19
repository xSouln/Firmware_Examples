//==============================================================================
#ifndef _DRV2605_TYPES_H
#define _DRV2605_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "DRV2605_Config.h"
#include "DRV2605/drv2605.h"
//==============================================================================
//types:

typedef struct
{
	uint16_t UpdateTime;

} DRV2605_ControlOptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	DRV2605_T Driver;

	DRV2605_ControlOptionsT Options;

	uint16_t UpdateTime;

} DRV2605_ControlT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
