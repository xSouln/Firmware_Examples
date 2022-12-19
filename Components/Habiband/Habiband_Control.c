//==============================================================================
#include "Habiband_Control.h"
#include "MAX30208/MAX30208_Adapter.h"
#include "LSM6DSOX/LSM6DSOX_Adapter.h"
#include "AFE49I30/AFE49I30_Adapter.h"
#include "ADP5360/ADP5360_Adapter.h"
#include "LM2755/LM2755_Adapter.h"
//==============================================================================
extern void Custom_BLEAplicationHandler();
//==============================================================================
static uint8_t habiband_buffer[0xff + 1];
static uint16_t habiband_ble_delay_time;
//==============================================================================
/**
 * @brief responsible for updating characteristics based on subscriptions
 * @param device 
 */
static void HabibandBLEHandler(HabibandT* device)
{
	uint16_t points_count = 0;

	if (!device->BLE.Interface || !device->BLE.Status.IsConnected || habiband_ble_delay_time)
	{
		return;
	}

	if (device->BLE.Notifications.Gyroscope
	&& LSM6DSOX_GyroscopeValuesInBuffer(&device->LSM6DSOX) >= device->BLE.ValuesInCharacteristic.Gyroscope)
	{
		points_count = LSM6DSOX_ReadGyroscopeValuesFromBuffer(&device->LSM6DSOX, habiband_buffer, device->BLE.ValuesInCharacteristic.Gyroscope);
		if (points_count)
		{
			device->BLE.Interface->RequestListener(device,
					HabibandBLE_RequestWriteGyroscopePoints,
					(uint32_t)habiband_buffer,
					sizeof(LSM6DSOX_GyroscopeValueT) * points_count);
		}
	}

	if (device->BLE.Notifications.Accelerometer
	&& LSM6DSOX_AccelerometerValuesInBuffer(&device->LSM6DSOX) >= device->BLE.ValuesInCharacteristic.Accelerometer)
	{
		points_count = LSM6DSOX_ReadAccelerometerValuesFromBuffer(&device->LSM6DSOX, habiband_buffer, device->BLE.ValuesInCharacteristic.Accelerometer);
		if (points_count)
		{
			device->BLE.Interface->RequestListener(device,
					HabibandBLE_RequestWriteAccelerometerPoints,
					(uint32_t)habiband_buffer,
					sizeof(LSM6DSOX_AccelerometerValueT) * points_count);
		}
	}

	if (device->BLE.Notifications.ECG_Points
	&& AFE49I30_ECG_ValuesInBuffer(&device->AFE49I30) >= device->BLE.ValuesInCharacteristic.ECG)
	{
		points_count = AFE49I30_Read_ECG_ValuesFromBuffer(&device->AFE49I30, habiband_buffer, device->BLE.ValuesInCharacteristic.ECG);
		if (points_count)
		{
			device->BLE.Interface->RequestListener(device,
					HabibandBLE_RequestWriteECG_Points,
					(uint32_t)habiband_buffer,
					sizeof(AFE49I30_ECG_ValueT) * points_count);
		}
	}
	else if (device->BLE.Notifications.PPG_Points
	&& AFE49I30_PPG_ValuesInBuffer(&device->AFE49I30) >= device->BLE.ValuesInCharacteristic.PPG)
	{
		points_count = AFE49I30_Read_PPG_ValuesFromBuffer(&device->AFE49I30, habiband_buffer, device->BLE.ValuesInCharacteristic.PPG);
		if (points_count)
		{
			device->BLE.Interface->RequestListener(device,
					HabibandBLE_RequestWritePPG_Points,
					(uint32_t)habiband_buffer,
					sizeof(AFE49I30_PPG_ValueT) * points_count);
		}
	}

	if ((device->BLE.Notifications.Temperature && device->MAX30208.Events.TemperatureChanged)
	|| device->BLE.Requests.Read.Temperature)
	{
		device->MAX30208.Events.TemperatureChanged = false;
		device->BLE.Requests.Read.Temperature = false;

		device->BLE.Interface->RequestListener(device,
												HabibandBLE_RequestWriteTemperature,
												(uint32_t)&device->MAX30208.Temperature,
												sizeof(device->MAX30208.Temperature));
	}
}
//------------------------------------------------------------------------------
/**
 * @brief Habiband сontrol main handler
 * @param device 
 */
void HabibandControlHandler(HabibandT* device)
{
	MAX30208_ControlHandler(&device->MAX30208);
	LSM6DSOX_ControlHandler(&device->LSM6DSOX);
	AFE49I30_ControlHandler(&device->AFE49I30);
	ADP5360_ControlHandler(&device->ADP5360);
	LM2755_ControlHandler(&device->LM2755);

	HabibandBLEHandler(device);
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void HabibandTimeSynchronization(HabibandT* device)
{
	MAX30208_TimeSynchronization(&device->MAX30208);
	LSM6DSOX_TimeSynchronization(&device->LSM6DSOX);
	AFE49I30_TimeSynchronization(&device->AFE49I30);
	ADP5360_TimeSynchronization(&device->ADP5360);
	LM2755_TimeSynchronization(&device->LM2755);

	if (habiband_ble_delay_time)
	{
		habiband_ble_delay_time--;
	}
}
//==============================================================================
/**
 * @brief event listener
 * @param device 
 * @param selector HabibandEventSelector
 * @param args attached objects
 * @param count attached objects count
 */
void HabibandEventListener(HabibandT* device, HabibandEventSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint8_t)selector)
	{
		case HabibandEventBLE_Connected:
			device->BLE.Status.IsConnected = true;
			device->BLE.Events.ConnectionStateChanged = true;
			HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_ConnectionStateEnableEvent, args, count);
			break;

		case HabibandEventBLE_Disconnected:
			device->BLE.Status.IsConnected = false;
			device->BLE.Requests.Value = 0;
			device->BLE.Notifications.Value = 0;
			device->BLE.Events.ConnectionStateChanged = true;
			HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_ConnectionStateDisableEvent, args, count);
			AFE49I30_StopMeasure(&device->AFE49I30);
			break;

		case HabibandEventBLE_TemperatureNotificationEnnabled:
			device->BLE.Notifications.Temperature = true;
			device->BLE.Events.TemperatureNotificationStateChanged = true;
			break;

		case HabibandEventBLE_TemperatureNotificationDisabled:
			device->BLE.Notifications.Temperature = false;
			device->BLE.Events.TemperatureNotificationStateChanged = true;
			break;

		case HabibandEventBLE_AccelerometerNotificationEnnabled:
			device->BLE.Notifications.Accelerometer = true;
			device->BLE.Events.AccelerometerNotificationStateChanged = true;
			break;

		case HabibandEventBLE_AccelerometerNotificationDisabled:
			device->BLE.Notifications.Accelerometer = false;
			device->BLE.Events.AccelerometerNotificationStateChanged = true;
			break;

		case HabibandEventBLE_GyroscopeNotificationEnnabled:
			device->BLE.Notifications.Gyroscope = true;
			device->BLE.Events.GyroscopeNotificationStateChanged = true;
			break;

		case HabibandEventBLE_GyroscopeNotificationDisabled:
			device->BLE.Notifications.Gyroscope = false;
			device->BLE.Events.GyroscopeNotificationStateChanged = true;
			break;

		case HabibandEventBLE_ECG_PointsNotificationEnnabled:
			device->BLE.Notifications.ECG_Points = true;
			device->BLE.Notifications.PPG_Points = false;
			device->BLE.Events.ECG_PointsNotificationStateChanged = true;
			device->BLE.Events.PPG_PointsNotificationStateChanged = false;
			AFE49I30_EnableECG(&device->AFE49I30);
			break;

		case HabibandEventBLE_ECG_PointsNotificationDisabled:
			device->BLE.Notifications.ECG_Points = false;
			device->BLE.Events.ECG_PointsNotificationStateChanged = true;
			AFE49I30_StopMeasure(&device->AFE49I30);
			break;

		case HabibandEventBLE_PPG_PointsNotificationEnnabled:
			device->BLE.Notifications.ECG_Points = false;
			device->BLE.Notifications.PPG_Points = true;
			device->BLE.Events.ECG_PointsNotificationStateChanged = false;
			device->BLE.Events.PPG_PointsNotificationStateChanged = true;
			AFE49I30_EnablePPG(&device->AFE49I30);
			break;

		case HabibandEventBLE_PPG_PointsNotificationDisabled:
			device->BLE.Notifications.PPG_Points = false;
			device->BLE.Events.PPG_PointsNotificationStateChanged = true;
			AFE49I30_StopMeasure(&device->AFE49I30);
			break;

	}
}
//------------------------------------------------------------------------------
/**
 * @brief request listener
 * @param device 
 * @param selector HabibandRequestSelector
 * @param args attached objects
 * @param count attached objects count
 * @return xResult 
 */
xResult HabibandRequestListener(HabibandT* device, HabibandRequestSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint8_t)selector)
	{
		default: return xResultNotSupported;
	}
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult HabibandInit(HabibandT* device, void* parent)
{
	if (device)
	{
		device->Description = "HabibandT";
		device->Parent = parent;

		MAX30208_AdapterInit(&device->MAX30208);
		MAX30208_ControlInit(&device->MAX30208, device);

		LSM6DSOX_AdapterInit(&device->LSM6DSOX);
		LSM6DSOX_ControlInit(&device->LSM6DSOX, device);

		AFE49I30_AdapterInit(&device->AFE49I30);
		AFE49I30_ControlInit(&device->AFE49I30, device);

		ADP5360_AdapterInit(&device->ADP5360);
		ADP5360_ControlInit(&device->ADP5360, device);

		LM2755_AdapterInit(&device->LM2755);
		LM2755_ControlInit(&device->LM2755, device);

		LM2755_SetHighLevel(&device->LM2755.Driver, LM2755_DIODE_ALL, 0);

		return xResultAccept;
	}

	return xResultError;
}
//==============================================================================
