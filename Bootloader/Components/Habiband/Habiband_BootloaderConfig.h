//==============================================================================
#ifndef _HABIBAND_BOOTLOADER_CONFIG_H
#define _HABIBAND_BOOTLOADER_CONFIG_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Config.h"
//==============================================================================
//defines:

#define HABIBAND_BOOTLOADER_REQUEST_START_BOOT_KEY 					0x4E62B165
#define HABIBAND_BOOTLOADER_REQUEST_INIT_LOAD_FIRMWARE_KEY			0x548632ac
#define HABIBAND_BOOTLOADER_REQUEST_CONFIRMATION_LOAD_FIRMWARE_KEY	0x6A68BDE7
#define HABIBAND_BOOTLOADER_REQUEST_GET_SESSION_KEY					0x35cd87ba
#define HABIBAND_BOOTLOADER_REQUEST_CONFIRM_SESSION_KEY 			0xf2e5d23c

//#define HABIBAND_BOOTLOADER_KEY_MASK_CONFIRM_SESSION_KEY			0xf2e5d23c
//#define HABIBAND_BOOTLOADER_KEY_MASK_GET_SESSION_KEY				0x35cd87ba
//#define HABIBAND_BOOTLOADER_KEY_MASK_SESSION_KEY_ACCEPT				0xC6263761
//#define HABIBAND_BOOTLOADER_KEY_MASK_INIT_LOAD_FIRMWARE				0x548632ac
//#define HABIBAND_BOOTLOADER_KEY_MASK_START_BOOT 					0x4E62B165
//#define HABIBAND_BOOTLOADER_KEY_MASK_START_APP 						0x87A27999
//#define HABIBAND_BOOTLOADER_KEY_MASK_CONFIRMATION_LOAD_FIRMWARE 	0x6A68BDE7
//#define HABIBAND_BOOTLOADER_KEY_MASK_APPROVING_KEY					0xD7E2F928

#define HABIBAND_BOOTLOADER_IMAGE_BUFFER_SIZE_MASK 0x3ff
#define HABIBAND_BOOTLOADER_IMAGE_BUFFER_SIZE (HABIBAND_BOOTLOADER_IMAGE_BUFFER_SIZE_MASK + 1)

#define HABIBAND_BOOTLOADER_RESPONSE_BUFFER_CONTENT_SIZE 20
#define HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE_MASK 0xf
#define HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE (HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE_MASK + 1)

#define HABIBAND_BOOTLOADER_OPERATION_BUFFER_SIZE 256

#define HABIBAND_BOOTLOADER_NOTIFICATIONS_UPDATE_PERIOD 200
#define HABIBAND_BOOTLOADER_HANDLER_UPDATE_PERIOD 5
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
