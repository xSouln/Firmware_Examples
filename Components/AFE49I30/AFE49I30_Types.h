//==============================================================================
#ifndef _AFE49I30_TYPES_H
#define _AFE49I30_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "AFE49I30_Config.h"
#include "AFE49I30/afe49I30.h"
//==============================================================================
//types:

typedef int AFE49I30_ValueT;
//------------------------------------------------------------------------------

typedef struct
{
	AFE49I30_ValueT RED_AC;
	AFE49I30_ValueT IR_AC;
	AFE49I30_ValueT RED_DC;
	AFE49I30_ValueT IR_DC;

} AFE49I30_PPG_ValueT;
//------------------------------------------------------------------------------

typedef struct
{
	AFE49I30_ValueT ADC;

} AFE49I30_ECG_ValueT;
//------------------------------------------------------------------------------
/**
 * @brief The buffer is common to the AFE49I30_PPG_ValueT and AFE49I30_ECG_ValueT
 * points, since only one mode can be enabled at a time
 */
typedef struct
{
	AFE49I30_ValueT Values[AFE49I30_BUFFER_SIZE];

	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} AFE49I30_BufferT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t UpdateTime;

} AFE49I30_ControlOptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	AFE49I30_T Driver;

	AFE49I30_ControlOptionsT Options;

	uint32_t FifoData;
	uint32_t FifoSamplesCount;

	AFE49I30_BufferT Buffer;

} AFE49I30_ControlT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
