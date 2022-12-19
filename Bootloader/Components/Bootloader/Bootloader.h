//==============================================================================
#ifndef _BOOTLOADER_H
#define _BOOTLOADER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Bootloader_Types.h"
//==============================================================================
//functions:

xResult BootloaderInit(void* parent);
void BootloaderTimeSynchronization();
void BootloaderHandler();
void BootloaderHardFaultError();

xResult BootloaderMoveIRQVector(uint32_t address);
void BootloaderSystemReset();

xResult BootloaderRequestStartApp();
xResult BootloaderStartBoot();

xResult BootloaderErase(uint32_t start_address, uint32_t end_address);
xResult BootloaderRead(uint32_t start_address, uint8_t* memory, uint32_t size);
xResult BootloaderWrite(uint32_t start_address, uint8_t* memory, uint32_t size);
xResult BootloaderLoad(uint8_t* memory, uint32_t size);
xResult BootloaderResetFirmwareError();

xResult BootloaderInitLoadFirmware(uint32_t size, uint32_t crc);
xResult BootloaderCancelLoadFirmware();
xResult BootloaderConfirmLoadFirmware();

xResult BootloaderLockMemory();
xResult BootloaderUnLockMemory();

xResult BootloaderDisableIRQ();
xResult BootloaderEnableIRQ();

void BootloaderInitAppInput();
void BootloaderInitBootInput();
void BootloaderInitSystemEnd();

uint32_t BootloaderCalculateMemoryCrc(uint32_t start_address, uint32_t memory_size);
//==============================================================================
//export:

extern BootloaderT Bootloader;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
