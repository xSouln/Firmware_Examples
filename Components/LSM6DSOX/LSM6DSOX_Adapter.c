//==============================================================================
#include "LSM6DSOX_Adapter.h"
//==============================================================================
static int32_t LSM6DSOX_Write(void* handle, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	int result = HAL_I2C_Mem_Write((I2C_HandleTypeDef*)handle,
			0xD4,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			10);
	
	return result;
}
//------------------------------------------------------------------------------
static int32_t LSM6DSOX_Read(void* handle, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	int result = HAL_I2C_Mem_Read((I2C_HandleTypeDef*)handle,
			0xD4,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			10);
	
	return result;
}
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return xResult 
 */
xResult LSM6DSOX_AdapterInit(LSM6DSOX_ControlT* device)
{
	if (device)
	{
		device->Bus.handle = &hi2c1;
		device->Bus.write_reg = LSM6DSOX_Write;
		device->Bus.read_reg = LSM6DSOX_Read;
		
		lsm6dsox_device_id_get(&device->Bus, &device->Id);
		
		return xResultAccept;
	}
	
	return xResultError;
}
//==============================================================================
