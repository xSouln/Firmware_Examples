//==============================================================================
#include "Bootloader/Bootloader_Component.h"
#include "Habiband_Bootloader.h"
//==============================================================================
HabibandBootloaderT HabibandBootloader;
//==============================================================================
extern int BLE_BootloaderPortTxSend(uint8_t* data, uint32_t data_size);
extern int BLE_BootloaderStatusSend(uint8_t* data, uint32_t data_size);
//==============================================================================
uint8_t habiband_operation_data[HABIBAND_BOOTLOADER_OPERATION_BUFFER_SIZE];
uint32_t habiband_operation_data_size;
//==============================================================================
/**
 * @brief sending an operation status update
 * @param operation HabibandBootloaderOperations
 * @param data 
 * @param data_size 
 * @return xResult 
 */
static xResult HabibandBootloaderTransmitUpdateOperationState(uint16_t operation, uint8_t* data, uint16_t data_size)
{
	HabibandBootloadeUpdateHeaderT header =
	{
		.Operation = operation
	};

	memcpy(habiband_operation_data, &header, sizeof(header));
	habiband_operation_data_size = sizeof(header);

	memcpy(habiband_operation_data + habiband_operation_data_size, data, data_size);
	habiband_operation_data_size += data_size;

	return BLE_BootloaderPortTxSend(habiband_operation_data, habiband_operation_data_size)
			== 0 ? xResultAccept : xResultError;
}
//------------------------------------------------------------------------------
/**
 * @brief sending the download status via BLE
 * @return xResult 
 */
static xResult HabibandBootloaderTransmitUpdateLoadingFirmware()
{
	HabibandBootloaderLoadingStateT state =
	{
		.LoadedImageSize = HabibandBootloader.LoadedImageSize,
		.LoadedImageCrc = Bootloader.Crc,
	};

	return HabibandBootloaderTransmitUpdateOperationState(HabibandBootloaderOperationUpdateLoadingFirmware,
			(uint8_t*)&state,
			sizeof(state));
}
//==============================================================================
/**
 * @brief processes the generated queue of responses
 * for writing to characteristic CUSTOM_STM_BOOTLOADER_PORT_TX
 * and also updates the characteristic CUSTOM_STM_BOOTLOADER_STATUS
 */
static void HabibandBootloaderResponseHandler()
{
	if (!HabibandBootloader.Status.BLE_IsConected)
	{
		return;
	}

	if (HabibandBootloader.Status.BLE_NotificationTxIsEnabled)
	{
		if (HabibandBootloader.ResponseBuffer.HandlerIndex != HabibandBootloader.ResponseBuffer.TotalIndex)
		{
			uint16_t position = HabibandBootloader.ResponseBuffer.HandlerIndex;
			HabibandBootloadeResponseT* response = &HabibandBootloader.ResponseBuffer.Response[position];

			memcpy(habiband_operation_data, response, sizeof(HabibandBootloadeResponseT));
			habiband_operation_data_size = sizeof(HabibandBootloadeResponseT);

			if (BLE_BootloaderPortTxSend(habiband_operation_data, habiband_operation_data_size) == 0)
			{
				position++;
				position &= HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE_MASK;
				HabibandBootloader.ResponseBuffer.HandlerIndex = position;
			}
		}
	}

	if (HabibandBootloader.NotificationsUpdateTime)
	{
		return;
	}

	HabibandBootloader.NotificationsUpdateTime = HABIBAND_BOOTLOADER_NOTIFICATIONS_UPDATE_PERIOD;

	if (HabibandBootloader.Status.BLE_NotificationTxIsEnabled)
	{
		if (Bootloader.FirmwareLoadingInit)
		{
			HabibandBootloaderTransmitUpdateLoadingFirmware();
		}
	}

	if (HabibandBootloader.Status.BLE_NotificationStatus)
	{
		BLE_BootloaderStatusSend((uint8_t*)&HabibandBootloader.Status, sizeof(HabibandBootloader.Status));
	}
}
//------------------------------------------------------------------------------
/**
 * @brief Bootloader operation handler
 */
static void HabibandBootloaderOperationHandler()
{
	if (!HabibandBootloader.Status.OperationInProgress || HabibandBootloader.OperationDelay)
	{
		return;
	}

	switch ((uint8_t)HabibandBootloader.Status.Operation)
	{
		case HabibandBootloaderOperationInitLoadFirmware:
			HabibandBootloader.Status.OperationResult = BootloaderInitLoadFirmware(HabibandBootloader.ImageSize, HabibandBootloader.ImageCrc);
			break;

		case HabibandBootloaderOperationConfirmationLoadFirmware:
			HabibandBootloader.Status.OperationResult = BootloaderConfirmLoadFirmware();
			break;

		case HabibandBootloaderOperationStartApp:
			HabibandBootloader.Status.OperationResult = BootloaderRequestStartApp();
			break;

		default:
			HabibandBootloader.Status.OperationInProgress = false;
			break;
	}

	HabibandBootloader.Status.OperationInProgress = false;
	HabibandBootloader.NotificationsUpdateTime = 0;
}
//------------------------------------------------------------------------------
/**
 * @brief Habiband bootloader main handler
 */
void HabibandBootloaderHandler()
{
	BootloaderHandler();

	HabibandBootloader.Status.FirmwareLoadingInit = Bootloader.FirmwareLoadingInit;

	HabibandBootloaderOperationHandler();
	HabibandBootloaderResponseHandler();
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void HabibandBootloaderTimeSynchronization()
{
	BootloaderTimeSynchronization();
	
	if (HabibandBootloader.NotificationsUpdateTime)
	{
		HabibandBootloader.NotificationsUpdateTime--;
	}

	if (HabibandBootloader.HandlerUpdateTime)
	{
		HabibandBootloader.HandlerUpdateTime--;
	}

	if (HabibandBootloader.OperationDelay)
	{
		HabibandBootloader.OperationDelay--;
	}
}
//==============================================================================
/**
 * @brief adding responses to the queue for writing to the CUSTOM_STM_BOOTLOADER_PORT_TX characteristic
 * @param operation HabibandBootloaderOperations
 * @param result xResult
 * @param content 
 * @param content_size 
 */
void HabibandBootloaderAddToResponseBuffer(HabibandBootloaderOperations operation,
											xResult result,
											uint8_t* content,
											uint16_t content_size)
{
	if (HabibandBootloader.Status.BLE_NotificationTxIsEnabled)
	{
		uint16_t position = HabibandBootloader.ResponseBuffer.TotalIndex;
		HabibandBootloader.ResponseBuffer.Response[position].Header.Operation = operation;
		HabibandBootloader.ResponseBuffer.Response[position].Header.Result = result;

		uint16_t i = 0;
		while (i < content_size && i < HABIBAND_BOOTLOADER_RESPONSE_BUFFER_CONTENT_SIZE)
		{
			HabibandBootloader.ResponseBuffer.Response[position].Content[i] = content[i];
			i++;
		}

		position++;
		position &= HABIBAND_BOOTLOADER_RESPONSE_BUFFER_SIZE_MASK;
		HabibandBootloader.ResponseBuffer.TotalIndex = position;
	}
}
//------------------------------------------------------------------------------
/**
 * @brief clearing the response queue
 */
void HabibandBootloaderClearResponseBuffer()
{
	HabibandBootloader.ResponseBuffer.HandlerIndex = HabibandBootloader.ResponseBuffer.TotalIndex;
}
//==============================================================================
/**
 * @brief event listener from BLE
 * called from STM32_WPAN/App/custom_app.c
 * @param selector HabibandBootloaderBLE_EventSelector
 * @param args attached objects
 * @param count attached objects count
 */
void HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_EventSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint8_t)selector)
	{
		case HabibandBootloaderBLE_ConnectionStateEnableEvent:
			HabibandBootloader.Status.BLE_IsConected = true;
			break;

		case HabibandBootloaderBLE_ConnectionStateDisableEvent:
			HabibandBootloader.Status.BLE_IsConected = false;
			HabibandBootloader.Status.BLE_NotificationTxIsEnabled = false;
			HabibandBootloader.Status.BLE_NotificationStatus = false;
			HabibandBootloader.Status.SessionKeyConfirmed = false;
			break;

		case HabibandBootloaderBLE_NotificationTxEnableEvent:
			HabibandBootloader.Status.BLE_NotificationTxIsEnabled = true;
			break;

		case HabibandBootloaderBLE_NotificationTxDisableEvent:
			HabibandBootloader.Status.BLE_NotificationTxIsEnabled = false;
			break;

		case HabibandBootloaderBLE_NotificationStatusEnableEvent:
			HabibandBootloader.Status.BLE_NotificationStatus = true;
			break;

		case HabibandBootloaderBLE_NotificationStatusDisableEvent:
			HabibandBootloader.Status.BLE_NotificationStatus = false;
			break;
	}
}
//==============================================================================
/**
 * @brief initializing the input from the bootloader
 */
void HabibandBootloaderInitBootInput()
{
	HabibandBootloader.Status.InBoot = true;
	BootloaderInitBootInput();

	HabibandBootloader.Status.AppCrcError = Bootloader.AppCrcError;
	HabibandBootloader.Status.AppStartError = Bootloader.Remark.Error.AppHardFaultError
			| Bootloader.Remark.Error.AppWatchdogError
			| Bootloader.Remark.Error.AppStartingHardFaultError;
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the input from the application
 */
void HabibandBootloaderInitAppInput()
{
	HabibandBootloader.Status.InBoot = false;
	BootloaderInitAppInput();
}
//------------------------------------------------------------------------------
/**
 * @brief called upon completion of initialization of the microcontroller
 */
void HabibandBootloaderInitSystemEnd()
{
	BootloaderInitSystemEnd();
}
//==============================================================================
/**
 * @brief initialization of a component
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult HabibandBootloaderInit(void* parent)
{
	HabibandBootloader.Description = "HabibandBootloaderT";
	HabibandBootloader.Parent = parent;

	BootloaderComponentInit(&HabibandBootloader);

	return xResultAccept;
}
//==============================================================================
