//==============================================================================
#ifndef _BOOTLOADER_ADAPTER_BASE_H
#define _BOOTLOADER_ADAPTER_BASE_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "Bootloader_Config.h"
//==============================================================================
//types:

typedef enum
{
	BootloaderRequestIdle,
	BootloaderRequestSystemReset,
	BootloaderRequestDelay,
	BootloaderRequestMoveIRQVector,
	BootloaderRequestUnLockMemory,
	BootloaderRequestLockMemory,
	BootloaderRequestEnableIRQ,
	BootloaderRequestDisableIRQ,
	BootloaderRequestDisposeSystem,
	BootloaderRequestSetJumpAddress

} BootloaderRequestSelector;
//------------------------------------------------------------------------------

typedef enum
{
	BootloaderValueIdle,
	BootloaderValueMemoryLockState

} BootloaderValueSelector;
//------------------------------------------------------------------------------

typedef xResult (*BootloaderRequestListenerT)(void* device, BootloaderRequestSelector event, uint32_t args, uint32_t count);

typedef xResult (*BootloaderGetValueActionT)(void* device, BootloaderValueSelector event, uint32_t* value);
typedef xResult (*BootloaderSetValueActionT)(void* device, BootloaderValueSelector event, uint32_t value);

typedef xResult (*BootloaderReadActionT)(void* device, uint32_t start_address, uint8_t* data, uint32_t size, uint32_t mode);
typedef xResult (*BootloaderWriteActionT)(void* device, uint32_t start_address, uint8_t* data, uint32_t size, uint32_t mode);
typedef xResult (*BootloaderEraseActionT)(void* device, uint32_t start_address, uint32_t end_address, uint32_t mode);
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	void* Child;

	BootloaderRequestListenerT RequestListener;

	BootloaderReadActionT Read;
	BootloaderWriteActionT Write;
	BootloaderEraseActionT Erase;

	BootloaderGetValueActionT GetValue;
	BootloaderSetValueActionT SetValue;

} BootloaderAdapterBaseT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
