//==============================================================================
#ifndef _BOOTLOADER_TYPES_H
#define _BOOTLOADER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Bootloader_AdapterBase.h"
//==============================================================================
//types:

typedef enum
{
	BootloaderOperationIdle,
	BootloaderOperationErase,
	BootloaderOperationWrite,
	BootloaderOperationRead,
	BootloaderOperationLoad,
	BootloaderOperationUnLockMemory,
	BootloaderOperationLockMemory,

} BootloaderOperation;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t InBoot : 1;
		uint8_t IsInit : 1;
		uint8_t OperationInProgress : 1;
	};
	uint8_t Vlaue;

} BootloaderState;
//------------------------------------------------------------------------------

typedef struct
{
	struct
	{
		uint8_t OperationInProgress : 1;
		uint8_t Reserved : 7;
	};

	BootloaderOperation Operation;
	xResult Result;

} BootloaderStatusT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t BootloaderStartAddress;
	uint32_t BootloaderEndAddres;

	uint32_t AppStartAddress;
	uint32_t AppEndAddress;

} BootloaderInfoT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t BootIsEnabled : 1;
		uint8_t SystemIsInit : 1;
	};

	uint8_t Value;

} BootloaderRemarkStateT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t AppHardFaultError : 1;
		uint8_t AppWatchdogError : 1;
		uint8_t AppStartingHardFaultError : 1;
	};

	uint8_t Value;

} BootloaderRemarkErrorT;
//------------------------------------------------------------------------------

typedef struct
{
	BootloaderRemarkStateT State;
	BootloaderRemarkErrorT Error;
	BootloaderRemarkErrorT ErrorMask;
	uint8_t Reserved;

	uint32_t Crc;
	uint32_t Key;
	uint32_t FirmwareSize;

} BootloaderRemarkT;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t TimeOut;

} BootloaderOptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	BootloaderAdapterBaseT Adapter;

	struct
	{
		uint8_t InBoot : 1;
		uint8_t IsInit : 1;
		uint8_t SystemIsInit : 1;
		uint8_t FirmwareLoadingInit : 1;

		uint8_t AppCrcError : 1;
		uint8_t RemarkKeyError : 1;
		uint8_t AppAddressError : 1;
		uint8_t AppSizeError : 1;
	};

	BootloaderStatusT Status;
	BootloaderInfoT Info;
	BootloaderOptionsT Options;

	BootloaderRemarkT Remark;

	uint32_t FirmwareLoadedSize;
	uint32_t FirmwareSize;
	uint32_t FirmwareCrc;

	uint32_t AppSectionCrc;

	uint32_t Crc;

	uint16_t TimeOut;

} BootloaderT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
