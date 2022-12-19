//==============================================================================
#include "Bootloader/Bootloader_Component.h"
#include "Habiband_Bootloader.h"
//==============================================================================
extern int BootloaderPortTxSend(uint8_t* data, uint32_t data_size);
//==============================================================================
extern uint8_t habiband_operation_data[HABIBAND_BOOTLOADER_OPERATION_BUFFER_SIZE];
extern uint32_t habiband_operation_data_size;
//==============================================================================
static xResult PrivateHabibandBootloaderRequestStartBootHandler(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestStartBootT* request = (HabibandBootloaderRequestStartBootT*)data;

	if (Bootloader.InBoot)
	{
		return xResultAccept;
	}

	if (size != sizeof(HabibandBootloaderRequestStartBootT))
	{
		return xResultTypeMismatch;
	}

	if (!HabibandBootloader.Status.SessionKeyConfirmed
	|| (request->Key ^ HABIBAND_BOOTLOADER_REQUEST_START_BOOT_KEY) != HabibandBootloader.ConfirmedSessionKey)
	{
		return xResultPermissionError;
	}

	if (Bootloader.FirmwareLoadingInit && HabibandBootloader.Status.OperationInProgress)
	{
		return xResultBusy;
	}

	HabibandBootloader.Status.OperationInProgress = false;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationStartBoot;
	HabibandBootloader.Status.OperationResult = xResultAccept;
	HabibandBootloader.Status.OperationResult = BootloaderStartBoot();

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderRequestResetHandler()
{
	if (Bootloader.FirmwareLoadingInit && HabibandBootloader.Status.OperationInProgress)
	{
		HabibandBootloader.Status.OperationInProgress = false;
		HabibandBootloader.Status.Operation = HabibandBootloaderOperationReset;
		HabibandBootloader.Status.OperationResult = xResultAccept;
		BootloaderSystemReset();
		HabibandBootloader.Status.OperationResult = xResultError;

		return xResultError;
	}

	return xResultBusy;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderStartAppHandler()
{
	if (!Bootloader.InBoot)
	{
		return xResultAccept;
	}

	if (Bootloader.FirmwareLoadingInit && HabibandBootloader.Status.OperationInProgress)
	{
		return xResultBusy;
	}

	HabibandBootloader.Status.OperationInProgress = true;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationStartApp;
	HabibandBootloader.Status.OperationResult = xResultAccept;
	//HabibandBootloader.OperationDelay = 100;

	HabibandBootloader.Status.OperationResult = BootloaderRequestStartApp();

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderRequestInitLoadFirmwareHandler(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestInitLoadFirmwareT* request = (HabibandBootloaderRequestInitLoadFirmwareT*)data;

	if (!HabibandBootloader.Status.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	if (size != sizeof(HabibandBootloaderRequestInitLoadFirmwareT))
	{
		return xResultTypeMismatch;
	}

	if (!HabibandBootloader.Status.SessionKeyConfirmed
	|| (request->Key ^ HABIBAND_BOOTLOADER_REQUEST_INIT_LOAD_FIRMWARE_KEY) != HabibandBootloader.ConfirmedSessionKey)
	{
		return xResultPermissionError;
	}

	if (Bootloader.FirmwareLoadingInit || HabibandBootloader.Status.OperationInProgress)
	{
		return xResultBusy;
	}

	if(!request->Size && !request->Crc)
	{
		return xResultInvalidParameter;
	}

	HabibandBootloader.Status.OperationInProgress = true;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationInitLoadFirmware;
	HabibandBootloader.Status.OperationResult = xResultAccept;

	HabibandBootloader.ImageSize = request->Size;
	HabibandBootloader.LoadedImageSize = 0;
	HabibandBootloader.ImageCrc = request->Crc;
	HabibandBootloader.OperationDelay = 200;
	HabibandBootloader.NotificationsUpdateTime = 0;

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderRequestLoadHandler(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestLoadT* request = (HabibandBootloaderRequestLoadT*)data;

	if (!HabibandBootloader.Status.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	if (size < sizeof(HabibandBootloaderRequestLoadT))
	{
		return xResultTypeMismatch;
	}

	if (!Bootloader.FirmwareLoadingInit || !HabibandBootloader.Status.SessionKeyConfirmed)
	{
		return xResultPermissionError;
	}

	data += sizeof(HabibandBootloaderRequestLoadT);
	size -= sizeof(HabibandBootloaderRequestLoadT);

	if (size == request->DataSize && (HabibandBootloader.LoadedImageSize + size) <= HabibandBootloader.ImageSize)
	{
		HabibandBootloader.Status.Operation = HabibandBootloaderOperationLoad;
		HabibandBootloader.Status.OperationResult = BootloaderLoad(data, size);
		HabibandBootloader.Status.OperationInProgress = HabibandBootloader.ImageSize < HabibandBootloader.LoadedImageSize;

		HabibandBootloader.LoadedImageSize += size;

		HabibandBootloader.OperationDelay = 50;

		return xResultAccept;
	}

	return xResultInvalidParameter;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderConfirmationLoadFirmwareHandler(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestConfirmationLoadFirmwareT* request = (HabibandBootloaderRequestConfirmationLoadFirmwareT*)data;

	if (!HabibandBootloader.Status.InBoot
	|| !HabibandBootloader.Status.FirmwareLoadingInit)
	{
		return xResultOperationNotAvailable;
	}

	if (size != sizeof(HabibandBootloaderRequestConfirmationLoadFirmwareT))
	{
		return xResultTypeMismatch;
	}

	if (!HabibandBootloader.Status.SessionKeyConfirmed
	|| (request->Key ^ HABIBAND_BOOTLOADER_REQUEST_CONFIRMATION_LOAD_FIRMWARE_KEY) != HabibandBootloader.ConfirmedSessionKey)
	{
		return xResultPermissionError;
	}

	HabibandBootloader.Status.OperationInProgress = true;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationConfirmationLoadFirmware;
	HabibandBootloader.Status.OperationResult = xResultAccept;

	HabibandBootloader.OperationDelay = 50;

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderCancelLoadFirmwareHandler()
{
	if (!HabibandBootloader.Status.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	HabibandBootloader.Status.OperationInProgress = true;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationCancelLoadFirmware;
	HabibandBootloader.Status.OperationResult = xResultAccept;

	BootloaderCancelLoadFirmware();

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderGetSessionKeyHandler()
{
	HabibandBootloader.Status.OperationInProgress = true;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationGetSessionKey;
	HabibandBootloader.Status.OperationResult = xResultAccept;
	HabibandBootloader.SessionKey = random();

	uint32_t response_session_key = HabibandBootloader.SessionKey ^ HABIBAND_BOOTLOADER_REQUEST_GET_SESSION_KEY;

	*((uint32_t*)habiband_operation_data) = response_session_key;
	habiband_operation_data_size = sizeof(response_session_key);

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateHabibandBootloaderConfirmSessionKeyHandler(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestConfirmationSessionKeyT* request = (HabibandBootloaderRequestConfirmationSessionKeyT*)data;

	if (size != sizeof(HabibandBootloaderRequestConfirmationSessionKeyT))
	{
		return xResultTypeMismatch;
	}

	if ((request->Key ^ HABIBAND_BOOTLOADER_REQUEST_CONFIRM_SESSION_KEY) != HabibandBootloader.SessionKey)
	{
		return xResultInvalidParameter;
	}

	HabibandBootloader.Status.OperationInProgress = false;
	HabibandBootloader.Status.Operation = HabibandBootloaderOperationConfirmSessionKey;
	HabibandBootloader.Status.OperationResult = xResultAccept;
	HabibandBootloader.Status.SessionKeyConfirmed = true;
	HabibandBootloader.ConfirmedSessionKey = HabibandBootloader.SessionKey;

	return xResultAccept;
}
//==============================================================================
/**
 * @brief receive and processes requests received via BLE
 * @param data 
 * @param size 
 */
void HabibandBootloaderRequestsReceiver(uint8_t* data, uint16_t size)
{
	HabibandBootloaderRequestHeaderT* header = (HabibandBootloaderRequestHeaderT*)data;
	HabibandBootloader.Status.Request = header->Operation;
	habiband_operation_data_size = 0;

	if (size < sizeof (HabibandBootloaderRequestHeaderT))
	{
		HabibandBootloader.Status.Request = 0xff;
		HabibandBootloader.Status.RequestResult = xResultInvalidParameter;
		return;
	}

	data += sizeof(HabibandBootloaderRequestHeaderT);
	size -= sizeof(HabibandBootloaderRequestHeaderT);

	switch (header->Operation)
	{
		case HabibandBootloaderOperationReset:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderRequestResetHandler();
			break;

		case HabibandBootloaderOperationStartBoot:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderRequestStartBootHandler(data, size);
			break;

		case HabibandBootloaderOperationStartApp:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderStartAppHandler();
			break;

		case HabibandBootloaderOperationInitLoadFirmware:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderRequestInitLoadFirmwareHandler(data, size);
			break;

		case HabibandBootloaderOperationLoad:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderRequestLoadHandler(data, size);
			break;

		case HabibandBootloaderOperationConfirmationLoadFirmware:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderConfirmationLoadFirmwareHandler(data, size);
			break;

		case HabibandBootloaderOperationCancelLoadFirmware:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderCancelLoadFirmwareHandler();
			break;

		case HabibandBootloaderOperationGetSessionKey:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderGetSessionKeyHandler();
			break;

		case HabibandBootloaderOperationConfirmSessionKey:
			HabibandBootloader.Status.RequestResult = PrivateHabibandBootloaderConfirmSessionKeyHandler(data, size);
			break;

		default:
			HabibandBootloader.Status.Request = 0xff;
			HabibandBootloader.Status.RequestResult = xResultInvalidParameter;
			return;
	}

	HabibandBootloaderAddToResponseBuffer(HabibandBootloader.Status.Request,
											HabibandBootloader.Status.RequestResult,
											habiband_operation_data,
											habiband_operation_data_size);
}
//==============================================================================
