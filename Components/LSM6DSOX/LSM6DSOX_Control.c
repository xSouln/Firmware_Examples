//==============================================================================
#include "LSM6DSOX_Control.h"
//==============================================================================
static uint16_t LSM6DSOX_TemperatureUpdateTime;
static uint16_t LSM6DSOX_AccelerometerUpdateTime;
static uint16_t LSM6DSOX_GyroscopeUpdateTime;
//==============================================================================
/**
 * @brief LSM6DSOX сontrol main handler
 * @param device 
 */
void LSM6DSOX_ControlHandler(LSM6DSOX_ControlT* device)
{
	uint8_t rx_data;
	uint32_t word;

	lsm6dsox_read_reg(&device->Bus, 0x1e, &rx_data, 1); //STATUS_REG

	/**
	 * @brief reading the temperature value
	 */
	if (!LSM6DSOX_TemperatureUpdateTime)
	{
		LSM6DSOX_TemperatureUpdateTime = device->Options.TemperatureUpdateTime;

		lsm6dsox_read_reg(&device->Bus, 0x20, &rx_data, 1); //OUT_TEMP_L
		word = rx_data;
		lsm6dsox_read_reg(&device->Bus, 0x21, &rx_data, 1); //OUT_TEMP_H
		word |= (uint16_t)rx_data << 8;
		word &= 0x0fff;
		device->TemperatureValue = (float)word / 128 + 2;

		LSM6DSOX_PutTemperatureValueToBuffer(device, device->TemperatureValue);
	}

	/**
	 * @brief reading the acelerometer value
	 */
	if (!LSM6DSOX_AccelerometerUpdateTime)
	{
		LSM6DSOX_AccelerometerUpdateTime = device->Options.AccelerometerUpdateTime;

		device->AccelerometerValue.X = (lsm6dsox_read_byte(&device->Bus, 0x23) & 0xff) << 8; //OUTX_H_G
		device->AccelerometerValue.X |= lsm6dsox_read_byte(&device->Bus, 0x22) & 0xff; //OUTX_L_G

		device->AccelerometerValue.Y = (lsm6dsox_read_byte(&device->Bus, 0x25) & 0xff) << 8; //OUTY_H_G
		device->AccelerometerValue.Y |= lsm6dsox_read_byte(&device->Bus, 0x24) & 0xff; //OUTY_L_G

		device->AccelerometerValue.Z = (lsm6dsox_read_byte(&device->Bus, 0x27) & 0xff) << 8; //OUTZ_H_G
		device->AccelerometerValue.Z |= lsm6dsox_read_byte(&device->Bus, 0x26) & 0xff; //OUTZ_L_G

		LSM6DSOX_PutAccelerometerValueToBuffer(device, device->AccelerometerValue);
	}

	/**
	 * @brief reading the gyroscope value
	 */
	if (!LSM6DSOX_GyroscopeUpdateTime)
	{
		LSM6DSOX_GyroscopeUpdateTime = device->Options.GyroscopeUpdateTime;

		device->GyroscopeValue.X = (lsm6dsox_read_byte(&device->Bus, 0x29) & 0xff) << 8; //OUTX_H_A
		device->GyroscopeValue.X |= lsm6dsox_read_byte(&device->Bus, 0x28) & 0xff; //OUTX_L_A

		device->GyroscopeValue.Y = (lsm6dsox_read_byte(&device->Bus, 0x2B) & 0xff) << 8; //OUTY_H_A
		device->GyroscopeValue.Y |= lsm6dsox_read_byte(&device->Bus, 0x2A) & 0xff; //OUTY_L_A

		device->GyroscopeValue.Z = (lsm6dsox_read_byte(&device->Bus, 0x2D) & 0xff) << 8; //OUTZ_H_A
		device->GyroscopeValue.Z |= lsm6dsox_read_byte(&device->Bus, 0x2C) & 0xff; //OUTZ_L_A

		LSM6DSOX_PutGyroscopeValueToBuffer(device, device->GyroscopeValue);
	}
}
//==============================================================================
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void LSM6DSOX_TimeSynchronization(LSM6DSOX_ControlT* device)
{
	if (LSM6DSOX_TemperatureUpdateTime)
	{
		LSM6DSOX_TemperatureUpdateTime--;
	}

	if (LSM6DSOX_AccelerometerUpdateTime)
	{
		LSM6DSOX_AccelerometerUpdateTime--;
	}

	if (LSM6DSOX_GyroscopeUpdateTime)
	{
		LSM6DSOX_GyroscopeUpdateTime--;
	}
}
//==============================================================================
/**
 * @brief adding point to the buffer
 * @param device 
 * @param value 
 */
void LSM6DSOX_PutGyroscopeValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_GyroscopeValueT value)
{
	uint16_t index = device->GyroscopeBuffer.TotalIndex;

	device->GyroscopeBuffer.Values[index].X = value.X;
	device->GyroscopeBuffer.Values[index].Y = value.Y;
	device->GyroscopeBuffer.Values[index].Z = value.Z;
	index++;
	index &= LSM6DSOX_GYROSCOPE_BUFFER_SIZE_MASK;

	device->GyroscopeBuffer.TotalIndex = index;
	device->Events.GyroscopePointsChanged = true;
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param buffer destination memory
 * @param max_points the maximum number of points copied to the buffer
 * @return int - number of copied points
 */
int LSM6DSOX_ReadGyroscopeValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	LSM6DSOX_GyroscopeValueT* points = (LSM6DSOX_GyroscopeValueT*)buffer;
	uint16_t points_count = 0;

	while(device->GyroscopeBuffer.HandlerIndex != device->GyroscopeBuffer.TotalIndex
		&& points_count < max_points)
	{
		points[points_count] = device->GyroscopeBuffer.Values[device->GyroscopeBuffer.HandlerIndex];
		device->GyroscopeBuffer.HandlerIndex++;
		device->GyroscopeBuffer.HandlerIndex &= LSM6DSOX_GYROSCOPE_BUFFER_SIZE_MASK;

		points_count++;
	}

	return points_count;
}
//------------------------------------------------------------------------------
/**
 * @brief resetting the number of buffered points
 * @param device 
 */
void LSM6DSOX_ResetGyroscopeBuffer(LSM6DSOX_ControlT* device)
{
	device->GyroscopeBuffer.HandlerIndex = device->GyroscopeBuffer.TotalIndex;
}
//------------------------------------------------------------------------------
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t LSM6DSOX_GyroscopeValuesInBuffer(LSM6DSOX_ControlT* device)
{
	return ((int)device->GyroscopeBuffer.TotalIndex - device->GyroscopeBuffer.HandlerIndex) & LSM6DSOX_GYROSCOPE_BUFFER_SIZE_MASK;
}
//==============================================================================
/**
 * @brief adding point to the buffer
 * @param device 
 * @param value 
 */
void LSM6DSOX_PutAccelerometerValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_AccelerometerValueT value)
{
	uint16_t index = device->AccelerometerBuffer.TotalIndex;

	device->AccelerometerBuffer.Values[index].X = value.X;
	device->AccelerometerBuffer.Values[index].Y = value.Y;
	device->AccelerometerBuffer.Values[index].Z = value.Z;
	index++;
	index &= LSM6DSOX_ACCELEROMETER_BUFFER_SIZE_MASK;

	device->AccelerometerBuffer.TotalIndex = index;
	device->Events.AccelerometerPointsChanged = true;
}
//------------------------------------------------------------------------------
/**
 * @brief reading points from the buffer
 * @param device 
 * @param buffer destination memory
 * @param max_points the maximum number of points copied to the buffer
 * @return int - number of copied points
 */
int LSM6DSOX_ReadAccelerometerValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	LSM6DSOX_AccelerometerValueT* points = (LSM6DSOX_AccelerometerValueT*)buffer;
	uint16_t points_count = 0;

	while(device->AccelerometerBuffer.HandlerIndex != device->AccelerometerBuffer.TotalIndex
		&& points_count < max_points)
	{
		points[points_count] = device->AccelerometerBuffer.Values[device->AccelerometerBuffer.HandlerIndex];
		device->AccelerometerBuffer.HandlerIndex++;
		device->AccelerometerBuffer.HandlerIndex &= LSM6DSOX_ACCELEROMETER_BUFFER_SIZE_MASK;

		points_count++;
	}

	return points_count;
}
//------------------------------------------------------------------------------
/**
 * @brief resetting the number of buffered points
 * @param device 
 */
void LSM6DSOX_ResetAccelerometerBuffer(LSM6DSOX_ControlT* device)
{
	device->AccelerometerBuffer.HandlerIndex = device->AccelerometerBuffer.TotalIndex;
}
//------------------------------------------------------------------------------
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t LSM6DSOX_AccelerometerValuesInBuffer(LSM6DSOX_ControlT* device)
{
	return ((int)device->AccelerometerBuffer.TotalIndex - device->AccelerometerBuffer.HandlerIndex) & LSM6DSOX_ACCELEROMETER_BUFFER_SIZE_MASK;
}
//==============================================================================
/**
 * @brief adding point to the buffer
 * @param device 
 * @param value 
 */
void LSM6DSOX_PutTemperatureValueToBuffer(LSM6DSOX_ControlT* device, LSM6DSOX_TemperatureValueT value)
{
	uint16_t index = device->TemperatureBuffer.TotalIndex;

	device->TemperatureBuffer.Values[index] = value;
	index++;
	index &= LSM6DSOX_TEMPERATURE_BUFFER_SIZE_MASK;

	device->TemperatureBuffer.TotalIndex = index;
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
int LSM6DSOX_ReadTemperatureValuesFromBuffer(LSM6DSOX_ControlT* device, uint8_t* buffer, uint16_t max_points)
{
	LSM6DSOX_TemperatureValueT* points = (LSM6DSOX_TemperatureValueT*)buffer;
	uint16_t points_count = 0;

	while(device->TemperatureBuffer.HandlerIndex != device->TemperatureBuffer.TotalIndex
		&& points_count < max_points)
	{
		points[points_count] = device->TemperatureBuffer.Values[device->TemperatureBuffer.HandlerIndex];
		device->TemperatureBuffer.HandlerIndex++;
		device->TemperatureBuffer.HandlerIndex &= LSM6DSOX_TEMPERATURE_BUFFER_SIZE_MASK;

		points_count++;
	}

	return points_count;
}
//------------------------------------------------------------------------------
/**
 * @brief resetting the number of buffered points
 * @param device 
 */
void LSM6DSOX_ResetTemperatureBuffer(LSM6DSOX_ControlT* device)
{
	device->TemperatureBuffer.HandlerIndex = device->TemperatureBuffer.TotalIndex;
}
//------------------------------------------------------------------------------
/**
 * @brief gets the number of received points in the buffer
 * @param device 
 * @return uint16_t 
 */
uint16_t LSM6DSOX_TemperatureValuesInBuffer(LSM6DSOX_ControlT* device)
{
	return ((int)device->TemperatureBuffer.TotalIndex - device->TemperatureBuffer.HandlerIndex) & LSM6DSOX_TEMPERATURE_BUFFER_SIZE_MASK;
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult LSM6DSOX_ControlInit(LSM6DSOX_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "LSM6DSOX_ControlT";
		device->Parent = parent;

		device->Options.TemperatureUpdateTime = LSM6DSOX_TEMPERATURE_UPDATE_TIME;
		device->Options.GyroscopeUpdateTime = LSM6DSOX_GYROSCOPE_UPDATE_TIME;
		device->Options.AccelerometerUpdateTime = LSM6DSOX_ACCELEROMETER_UPDATE_TIME;

		//Restore default configuration
		lsm6dsox_reset_set(&device->Bus, PROPERTY_ENABLE);

		uint8_t rst;
		do
		{
			lsm6dsox_reset_get(&device->Bus, &rst);

		} while (rst);

		lsm6dsox_i3c_disable_set(&device->Bus, LSM6DSOX_I3C_DISABLE);
		//Enable Block Data Update
		lsm6dsox_block_data_update_set(&device->Bus, PROPERTY_ENABLE);
		//Set full scale
		lsm6dsox_xl_full_scale_set(&device->Bus, LSM6DSOX_2g);
		lsm6dsox_xl_data_rate_set(&device->Bus, LSM6DSOX_XL_ODR_OFF);

		//Set FIFO watermark (number of unread sensor data TAG + 6 bytes
		//stored in FIFO) to 10 samples
		//lsm6dsox_fifo_watermark_set(&dev_ctx, 0x1FF);
		//Set FIFO batch XL/Gyro ODR to 12.5Hz

		lsm6dsox_pin_int1_route_t int1_route;
		lsm6dsox_fifo_xl_batch_set(&device->Bus, LSM6DSOX_XL_BATCHED_AT_104Hz);
		//Set FIFO mode to Stream mode (aka Continuous Mode)
		//lsm6dsox_fifo_mode_set(&ctx, LSM6DSOX_STREAM_MODE);

		lsm6dsox_pin_int1_route_get(&device->Bus, &int1_route);
		int1_route.fifo_th = PROPERTY_ENABLE;
		int1_route.fifo_full = PROPERTY_ENABLE;
		int1_route.fifo_ovr = PROPERTY_ENABLE;
		int1_route.fifo_bdr = PROPERTY_ENABLE;
		lsm6dsox_pin_int1_route_set(&device->Bus, int1_route);

		//Set Output Data Rate
		lsm6dsox_xl_data_rate_set(&device->Bus, LSM6DSOX_XL_ODR_104Hz);

		lsm6dsox_compression_algo_init_set(&device->Bus, 1);
		lsm6dsox_compression_algo_set(&device->Bus, LSM6DSOX_CMP_ALWAYS);

		lsm6dsox_emb_sens_t emb_sens;
		emb_sens.fifo_compr = PROPERTY_ENABLE;
		lsm6dsox_embedded_sens_set(&device->Bus, &emb_sens);

		lsm6dsox_write_byte(&device->Bus, 0x11, 0x4A); //CTRL2_G

		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
