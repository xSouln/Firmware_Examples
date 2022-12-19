//==============================================================================
#include "MAX30208_Control.h"
//==============================================================================
static uint8_t MAX30208_Fifo[2];
static uint16_t MAX30208_TemperatureUpdatePeriod;
static uint8_t MAX30208_DelayTime;
static uint8_t MAX30208_RetryCount;
//==============================================================================
void MAX30208_PutTemperatureToBuffer(MAX30208_ControlT* device, MAX30208_TemperatureT value);
//==============================================================================
/**
 * @brief MAX30208 сontrol main handler
 * @param device 
 */
void MAX30208_ControlHandler(MAX30208_ControlT* device)
{
	if (MAX30208_DelayTime)
	{
		return;
	}

	if (!MAX30208_TemperatureUpdatePeriod)
	{
		MAX30208_TemperatureUpdatePeriod = device->Options.UpdateTime;
		if (device->Status.Conversion == MAX30208_ConversionStateIdle)
		{
			device->Status.Conversion = MAX30208_ConversionStateStarting;
		}
	}

	switch (device->Status.Conversion)
	{
		case MAX30208_ConversionStateStarting:
			if (MAX30208_start_conversion(&device->Driver) == MAX30208_ResultAccept)
			{
				MAX30208_RetryCount = 5;
				device->Status.Conversion = MAX30208_ConversionStateStarted;
			}
			MAX30208_DelayTime = 100;
			break;

		case MAX30208_ConversionStateStarted:
			if (MAX30208_RetryCount == 0)
			{
				device->Events.TemperatureUpdateError = true;
				device->Status.Conversion = MAX30208_ConversionStateIdle;
			}

			if (MAX30208_conversion_is_complite(&device->Driver) != MAX30208_ResultAccept)
			{
				MAX30208_DelayTime = 100;
				MAX30208_RetryCount--;
				break;
			}
			MAX30208_RetryCount = 5;
			device->Status.Conversion = MAX30208_ConversionStateComplite;

		case MAX30208_ConversionStateComplite:
			if (MAX30208_RetryCount == 0)
			{
				device->Events.TemperatureUpdateError = true;
				device->Status.Conversion = MAX30208_ConversionStateIdle;
				break;
			}

			if (MAX30208_raw_data_get(&device->Driver, MAX30208_Fifo) != MAX30208_ResultAccept)
			{
				MAX30208_DelayTime = 10;
				MAX30208_RetryCount--;
				break;
			}

			device->Events.TemperatureIsUpdate = true;
			device->Events.TemperatureUpdateError = false;

			float temperature = MAX30208_get_celcius(MAX30208_Fifo);
			MAX30208_PutValueToBuffer(device, temperature);

			device->Status.Conversion = MAX30208_ConversionStateIdle;

			device->Events.TemperatureChanged = temperature != device->Temperature;
			device->Temperature = temperature;
			break;

		case MAX30208_ConversionStateIdle:
			break;
	}
}
//==============================================================================
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void MAX30208_TimeSynchronization(MAX30208_ControlT* device)
{
	if (MAX30208_DelayTime)
	{
		MAX30208_DelayTime--;
	}

	if (MAX30208_TemperatureUpdatePeriod)
	{
		MAX30208_TemperatureUpdatePeriod--;
	}
}
//==============================================================================
/**
 * @brief adding point to the buffer
 * @param device 
 * @param value 
 */
void MAX30208_PutValueToBuffer(MAX30208_ControlT* device, MAX30208_TemperatureT value)
{
	uint16_t index = device->TemperaturePoints.TotalIndex;

	device->TemperaturePoints.Values[index] = value;
	index++;
	index &= MAX30208_TEMPERATURE_BUFFER_SIZE_MASK;

	device->TemperaturePoints.TotalIndex = index;
	device->Events.TemperaturePointsChanged = true;
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param buffer destination memory
 * @param max_points the maximum number of points copied to the buffer
 * @return int - number of copied points
 */
int MAX30208_ReadValuesFromBuffer(MAX30208_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	MAX30208_TemperatureT* points = (MAX30208_TemperatureT*)buffer;
	uint16_t points_count = 0;

	while(device->TemperaturePoints.HandlerIndex != device->TemperaturePoints.TotalIndex
		//&& buffer_size >= sizeof(MAX30208_TemperatureT)
		&& points_count < max_points)
	{
		points[points_count] = device->TemperaturePoints.Values[device->TemperaturePoints.HandlerIndex];
		device->TemperaturePoints.HandlerIndex++;
		device->TemperaturePoints.HandlerIndex &= MAX30208_TEMPERATURE_BUFFER_SIZE_MASK;

		points_count++;
		//buffer_size -= sizeof(MAX30208_TemperatureT);
	}

	return points_count;
}
//------------------------------------------------------------------------------
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t MAX30208_AccelerometerValuesInBuffer(MAX30208_ControlT* device)
{
	return ((int)device->TemperaturePoints.TotalIndex - device->TemperaturePoints.HandlerIndex) & MAX30208_TEMPERATURE_BUFFER_SIZE_MASK;
}
//------------------------------------------------------------------------------
/**
 * @brief resetting the number of buffered points
 * @param device 
 */
void MAX30208_ResetBuffer(MAX30208_ControlT* device)
{
	device->TemperaturePoints.HandlerIndex = device->TemperaturePoints.TotalIndex;
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult MAX30208_ControlInit(MAX30208_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "MAX30208_ControlT";
		device->Parent = parent;
		device->Options.UpdateTime = MAX30208_TEMPERATURE_UPDATE_TIME;

		device->Driver.Mode->Set(0x11);

		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
