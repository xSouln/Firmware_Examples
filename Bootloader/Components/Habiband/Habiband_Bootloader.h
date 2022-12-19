//==============================================================================
#ifndef _HABIBAND_BOOTLOADER_H
#define _HABIBAND_BOOTLOADER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Habiband_BootloaderTypes.h"
//==============================================================================
//types:

typedef enum
{
	HabibandBootloaderBLE_IdleEvent,

	HabibandBootloaderBLE_ConnectionStateEnableEvent,
	HabibandBootloaderBLE_ConnectionStateDisableEvent,

	HabibandBootloaderBLE_NotificationTxEnableEvent,
	HabibandBootloaderBLE_NotificationTxDisableEvent,

	HabibandBootloaderBLE_NotificationStatusEnableEvent,
	HabibandBootloaderBLE_NotificationStatusDisableEvent

} HabibandBootloaderBLE_EventSelector;
//------------------------------------------------------------------------------

typedef struct
{
	HabibandBootloadeResponseT Response[HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE];
	uint16_t TotalIndex;
	uint16_t HandlerIndex;

} HabibandBootloaderResponseBufferT;
//------------------------------------------------------------------------------

typedef struct
{
	uint8_t Data[HABIBAND_BOOTLOADER_OPERATION_BUFFER_SIZE];
	uint16_t Size;

} HabibandBootloaderOperationBufferT;
//------------------------------------------------------------------------------

typedef struct
{
	struct
	{
		uint16_t InBoot : 1;
		uint16_t OperationInProgress : 1;
		uint16_t FirmwareLoadingInit : 1;
		uint16_t RequestUpdate : 1;

		uint16_t SessionKeyConfirmed : 1;
		uint16_t BLE_IsConected : 1;
		uint16_t BLE_NotificationTxIsEnabled : 1;
		uint16_t BLE_NotificationStatus : 1;

		uint16_t AppCrcError : 1;
		uint16_t AppStartError : 1;
	};

	HabibandBootloaderOperations Operation;
	xResult OperationResult;

	HabibandBootloaderOperations Request;
	xResult RequestResult;

} HabibandBootloaderStatusT;
//------------------------------------------------------------------------------

typedef struct
{
	uint32_t Address;
	uint32_t DataSize;
	//uint8_t Data[DataSize]; must be uint64_t aligned

} HabibandBootloaderResponseStatusT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;

	HabibandBootloaderStatusT Status;
	HabibandBootloaderResponseBufferT ResponseBuffer;

	uint32_t ImageSize;
	uint32_t LoadedImageSize;
	uint32_t ImageCrc;

	uint32_t ConfirmedSessionKey;
	uint32_t SessionKey;

	uint16_t OperationDelay;
	uint16_t NotificationsUpdateTime;
	uint16_t HandlerUpdateTime;

} HabibandBootloaderT;
//==============================================================================
//functions:

void HabibandBootloaderHandler();
void HabibandBootloaderTimeSynchronization();
xResult HabibandBootloaderInit(void* parent);
void HabibandBootloaderInitSystemEnd();
void HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_EventSelector selector, uint32_t args, uint32_t count);

xResult HabibandBootloaderStartBoot(HabibandBootloaderRequestStartBootT* request);
xResult HabibandBootloaderInitLoadFirmware(HabibandBootloaderRequestInitLoadFirmwareT* request);
xResult HabibandBootloaderConfirmationLoadFirmware(HabibandBootloaderRequestConfirmationLoadFirmwareT* request);

void HabibandBootloaderRequestsReceiver(uint8_t* data, uint16_t size);

void HabibandBootloaderInitAppInput();
void HabibandBootloaderInitBootInput();

void HabibandBootloaderClearResponseBuffer();
void HabibandBootloaderAddToResponseBuffer(HabibandBootloaderOperations operation, xResult result, uint8_t* content, uint16_t content_size);
//==============================================================================
//export:

extern HabibandBootloaderT HabibandBootloader;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
