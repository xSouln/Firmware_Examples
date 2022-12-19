//==============================================================================
#ifndef _BOOTLOADER_CONFIG_H
#define _BOOTLOADER_CONFIG_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Config.h"
//==============================================================================
//defines:

#define BOOTLOADER_OPERATION_TIMEOUT 1000

#define BOOTLOADER_FLASH_MEMORY_SIZE			FLASH_SIZE
#define BOOTLOADER_FLASH_MEMORY_START_ADDRESS   0x08000000UL
#define BOOTLOADER_FLASH_MEMORY_END_ADDRESS     (BOOTLOADER_FLASH_MEMORY_START_ADDRESS + BOOTLOADER_FLASH_MEMORY_SIZE)
#define BOOTLOADER_APP_START_ADDRESS 			0x08020000UL
#define BOOTLOADER_BOOT_START_ADDRESS 			0x08000000UL
#define BOOTLOADER_REMARK_CONFIRMATION_KEY 		0x49105364UL
#define BOOTLOADER_REMARK_ADDRESS 				0x08010000UL
#define BOOTLOADER_REMARK_ERROR_MASK 			0xff
#define BOOTLOADER_REMARK_KEY					0xD987EBD9UL
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
