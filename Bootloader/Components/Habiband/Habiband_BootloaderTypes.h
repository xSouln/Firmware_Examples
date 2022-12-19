//==============================================================================
#ifndef _HABIBAND_BOOTLOADER_TYPES_H
#define _HABIBAND_BOOTLOADER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Config.h"
#include "Habiband_BootloaderConfig.h"
//==============================================================================
//types:

typedef enum
{
	HabibandBootloaderOperationIdle = 0x00,
	HabibandBootloaderOperationReset,
	HabibandBootloaderOperationStartBoot,
	HabibandBootloaderOperationStartApp,
	HabibandBootloaderOperationInitLoadFirmware,
	HabibandBootloaderOperationConfirmationLoadFirmware,
	HabibandBootloaderOperationWrite,
	HabibandBootloaderOperationRead,
	HabibandBootloaderOperationLoad,
	HabibandBootloaderOperationCancelLoadFirmware,
	HabibandBootloaderOperationGetSessionKey,
	HabibandBootloaderOperationConfirmSessionKey,

	HabibandBootloaderOperationUpdateIdle = 0x40,
	HabibandBootloaderOperationUpdateLoadingFirmware,

} HabibandBootloaderOperations;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t Operation;
	uint16_t Result;

} HabibandBootloadeResponseHeaderT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t Operation;

} HabibandBootloadeUpdateHeaderT;
//------------------------------------------------------------------------------

typedef struct
{
	HabibandBootloadeResponseHeaderT Header;
	uint8_t Content[HABIBAND_BOOTLOADER_RESPONSE_BUFFER_CONTENT_SIZE];

} HabibandBootloadeResponseT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t Operation;
	uint16_t Reserved;

} HabibandBootloaderRequestHeaderT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Key;

} HabibandBootloaderRequestStartBootT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Key;
	uint32_t Value;

} HabibandBootloaderRequestSetErrorMaskT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Key;
	uint32_t Size;
	uint32_t Crc;

} HabibandBootloaderRequestInitLoadFirmwareT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Key;

} HabibandBootloaderRequestConfirmationLoadFirmwareT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Key;

} HabibandBootloaderRequestConfirmationSessionKeyT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t DataSize;
	//uint8_t Data[DataSize]; must be uint64_t aligned

} HabibandBootloaderRequestLoadT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t LoadedImageSize;
	uint32_t LoadedImageCrc;

} HabibandBootloaderLoadingStateT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
