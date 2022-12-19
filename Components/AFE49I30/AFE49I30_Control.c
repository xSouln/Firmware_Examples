//==============================================================================
#include "AFE49I30/AFE49I30_Control.h"
//==============================================================================
static void AFE49I30_PutValuesToBuffer(AFE49I30_ControlT* device, AFE49I30_ValueT* values, uint16_t count);
static int AFE49I30_ReadValuesFromBuffer(AFE49I30_ControlT* device, AFE49I30_ValueT* values, uint16_t count);
//==============================================================================
static uint16_t AFE49I30_UpdateTime;
//==============================================================================
/**
 * @brief getting points from a chip
 * @param device 
 */
static void AFE49I30_ReadPPG(AFE49I30_ControlT* device)
{
	uint32_t rx_buffer = 0;
	AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_SAMPLE_COUNT_REG, &rx_buffer);
	device->FifoSamplesCount = ((rx_buffer & 0xff) + 1) / 4;

	for (uint16_t samples_count = 0; samples_count < device->FifoSamplesCount; samples_count++)
	{
		AFE49I30_PPG_ValueT value;

		AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_DATA_REG, &device->FifoData);
		value.RED_AC = device->FifoData;

		AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_DATA_REG, &device->FifoData);
		value.IR_AC = device->FifoData;

		AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_DATA_REG, &device->FifoData);
		value.RED_DC = device->FifoData;

		AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_DATA_REG, &device->FifoData);
		value.IR_DC = device->FifoData;

		AFE49I30_PutValuesToBuffer(device, (AFE49I30_ValueT*)&value, sizeof(value) / sizeof(AFE49I30_ValueT));
	}
}
//------------------------------------------------------------------------------
/**
 * @brief getting points from a chip
 * @param device 
 */
static void AFE49I30_ReadECG(AFE49I30_ControlT* device)
{
	uint32_t rx_buffer = 0;
	AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_SAMPLE_COUNT_REG, &rx_buffer);
	device->FifoSamplesCount = (rx_buffer & 0xff) + 1;

	for (uint16_t samples_count = 0; samples_count < device->FifoSamplesCount; samples_count++)
	{
		AFE49I30_ECG_ValueT value;

		AFE49I30_GetRegValue(&device->Driver, AFE49I30_FIFO_DATA_REG, &device->FifoData);
		value.ADC = device->FifoData;

		AFE49I30_PutValuesToBuffer(device, (AFE49I30_ValueT*)&value, sizeof(value) / sizeof(AFE49I30_ValueT));
	}
}
//------------------------------------------------------------------------------
/**
 * @brief AFE49I30 main handler
 * @param device 
 */
void AFE49I30_ControlHandler(AFE49I30_ControlT* device)
{
	if (AFE49I30_UpdateTime || device->Driver.Config.SleepMode != AFE49I30_ACTIVE_MODE)
	{
		return;
	}

	AFE49I30_UpdateTime = device->Options.UpdateTime;

	switch ((uint8_t)device->Driver.Config.MeasureMode)
	{
		case AFE49I30_PPG_MODE:
			AFE49I30_ReadPPG(device);
			break;

		case AFE49I30_ECG_MODE:
			AFE49I30_ReadECG(device);
			break;

	}
}
//==============================================================================
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void AFE49I30_TimeSynchronization(AFE49I30_ControlT* device)
{
	if (AFE49I30_UpdateTime)
	{
		AFE49I30_UpdateTime--;
	}
}
//==============================================================================
/**
 * @brief adding points to the buffer
 * @param device 
 * @param values 
 * @param count 
 */
static void AFE49I30_PutValuesToBuffer(AFE49I30_ControlT* device, AFE49I30_ValueT* values, uint16_t count)
{
	for (uint16_t i = 0; i < count; i++)
	{
		device->Buffer.Values[device->Buffer.TotalIndex] = values[i];
		device->Buffer.TotalIndex++;
		device->Buffer.TotalIndex &= AFE49I30_BUFFER_SIZE_MASK;
	}
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param values 
 * @param count the maximum number of points copied to the buffer
 * @return int - number of copied points 
 */
static int AFE49I30_ReadValuesFromBuffer(AFE49I30_ControlT* device, AFE49I30_ValueT* values, uint16_t count)
{
	uint16_t points = 0;
	while (points < count && device->Buffer.HandlerIndex != device->Buffer.TotalIndex)
	{
		values[points] = device->Buffer.Values[device->Buffer.HandlerIndex];
		points++;
		device->Buffer.HandlerIndex++;
		device->Buffer.HandlerIndex &= AFE49I30_BUFFER_SIZE_MASK;
	}

	return points;
}
//------------------------------------------------------------------------------
/**
 * @brief resetting the number of buffered points
 * @param device 
 */
void AFE49I30_ResetBuffer(AFE49I30_ControlT* device)
{
	device->Buffer.HandlerIndex = device->Buffer.TotalIndex;
}
//==============================================================================
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t AFE49I30_ECG_ValuesInBuffer(AFE49I30_ControlT* device)
{
	uint16_t count = 0;
	if (device->Driver.Config.MeasureMode == AFE49I30_ECG_MODE)
	{
		count = ((int)device->Buffer.TotalIndex - device->Buffer.HandlerIndex) & AFE49I30_BUFFER_SIZE_MASK;
		count /= (sizeof(AFE49I30_ECG_ValueT) / sizeof(AFE49I30_ValueT));
	}
	return count;
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param buffer destination memory
 * @param max_points the maximum number of points copied to the buffer
 * @return int - number of copied points
 */
int AFE49I30_Read_ECG_ValuesFromBuffer(AFE49I30_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	AFE49I30_ECG_ValueT* points = (AFE49I30_ECG_ValueT*)buffer;
	uint16_t points_in_buffer = 0;
	uint16_t points_count = 0;

	if (device->Driver.Config.MeasureMode == AFE49I30_ECG_MODE)
	{
		points_in_buffer = AFE49I30_ECG_ValuesInBuffer(device);

		while (points_count < points_in_buffer && points_count < max_points)
		{
			AFE49I30_ReadValuesFromBuffer(device,
										(AFE49I30_ValueT*)&points[points_count],
										sizeof(AFE49I30_ECG_ValueT) / sizeof(AFE49I30_ValueT));
			points_count++;
		}
	}

	return points_count;
}
//==============================================================================
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t AFE49I30_PPG_ValuesInBuffer(AFE49I30_ControlT* device)
{
	uint16_t count = 0;
	if (device->Driver.Config.MeasureMode == AFE49I30_PPG_MODE)
	{
		count = ((int)device->Buffer.TotalIndex - device->Buffer.HandlerIndex) & AFE49I30_BUFFER_SIZE_MASK;
		count /= (sizeof(AFE49I30_PPG_ValueT) / sizeof(AFE49I30_ValueT));
	}
	return count;
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param buffer destination memory
 * @param max_points the maximum number of points copied to the buffer
 * @return int - number of copied points
 */
int AFE49I30_Read_PPG_ValuesFromBuffer(AFE49I30_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	AFE49I30_PPG_ValueT* points = (AFE49I30_PPG_ValueT*)buffer;
	uint16_t points_in_buffer = 0;
	uint16_t points_count = 0;

	if (device->Driver.Config.MeasureMode == AFE49I30_PPG_MODE)
	{
		points_in_buffer = AFE49I30_PPG_ValuesInBuffer(device);

		while (points_count < points_in_buffer && points_count < max_points)
		{
			AFE49I30_ReadValuesFromBuffer(device,
										(AFE49I30_ValueT*)&points[points_count],
										sizeof(AFE49I30_PPG_ValueT) / sizeof(AFE49I30_ValueT));
			points_count++;
		}
	}

	return points_count;
}
//==============================================================================
/**
 * @brief putting the chip into sleep mode and disabling receiving and buffering points in the handler
 * @param device 
 * @return int 
 */
int AFE49I30_StopMeasure(AFE49I30_ControlT* device)
{
	if (device->Driver.Config.MeasureMode != AFE49I30_UNKNOWN_MODE)
	{
		return AFE49I30_ConfigureAFEInSleepMode(&device->Driver, AFE49I30_DEEP_SLEEP_MODE);
	}
	return -1;
}
//------------------------------------------------------------------------------
/**
 * @brief putting the chip into operation mode and enabling the receipt and buffering of points in the handler
 * @param device 
 * @return int 
 */
int AFE49I30_StartMeasure(AFE49I30_ControlT* device)
{
	if (device->Driver.Config.MeasureMode != AFE49I30_UNKNOWN_MODE)
	{
		return AFE49I30_ConfigureAFEInSleepMode(&device->Driver, AFE49I30_ACTIVE_MODE);
	}
	return -1;
}
//==============================================================================
/**
 * @brief enables PPG mode and disables ECG
 * @param device 
 * @return int 
 */
int AFE49I30_EnablePPG(AFE49I30_ControlT* device)
{
	if (device->Driver.Config.MeasureMode != AFE49I30_PPG_MODE)
	{
		AFE49I30_ResetBuffer(device);
		return AFE49I30_PPG_Init(&device->Driver);
	}

	if (device->Driver.Config.SleepMode != AFE49I30_ACTIVE_MODE)
	{
		return AFE49I30_StartMeasure(device);
	}
	return 0;
}
//------------------------------------------------------------------------------
/**
 * @brief enables ECG mode and disables PPG
 * @param device 
 * @return int 
 */
int AFE49I30_EnableECG(AFE49I30_ControlT* device)
{
	if (device->Driver.Config.MeasureMode != AFE49I30_ECG_MODE)
	{
		AFE49I30_ResetBuffer(device);
		return AFE49I30_ECG_Init(&device->Driver);
	}

	if (device->Driver.Config.SleepMode != AFE49I30_ACTIVE_MODE)
	{
		return AFE49I30_StartMeasure(device);
	}
	return 0;
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult AFE49I30_ControlInit(AFE49I30_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "AFE49I30_ControlT";
		device->Parent = parent;

		device->Options.UpdateTime = AFE49I30_UPDATE_TIME;

		device->Driver.Options.SamplingRate = AFE49I30_ECG_SAMPLE_400;

		//AFE49I30_EnablePPG(device);
		//AFE49I30_EnableECG(device);
		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
