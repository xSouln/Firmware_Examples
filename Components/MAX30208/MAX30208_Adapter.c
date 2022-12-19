//==============================================================================
#include "MAX30208_Adapter.h"
//==============================================================================
static void MAX30208_SetMode(MAX30208_GPIO_MODE mode)
{
	(mode & 0x01) > 0 ? (BODY_TEMP_GPIO0_GPIO_Port->ODR |= BODY_TEMP_GPIO0_Pin) :
	(BODY_TEMP_GPIO0_GPIO_Port->ODR &= ~BODY_TEMP_GPIO0_Pin);
	
	(mode & 0x02) > 0 ? (BODY_TEMP_GPIO1_GPIO_Port->ODR |= BODY_TEMP_GPIO1_Pin) :
	(BODY_TEMP_GPIO1_GPIO_Port->ODR &= ~BODY_TEMP_GPIO1_Pin);
}
//------------------------------------------------------------------------------
static MAX30208_GPIO_MODE MAX30208_GetMode()
{
	uint8_t mode = (BODY_TEMP_GPIO1_GPIO_Port->ODR & BODY_TEMP_GPIO1_Pin) > 0;
	mode <<= 1;
	mode |= (BODY_TEMP_GPIO0_GPIO_Port->ODR & BODY_TEMP_GPIO0_Pin) > 0;
	
	return mode;
}
//------------------------------------------------------------------------------
static MAX30208_Result MAX30208_Write(MAX30208_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	MAX30208_Result result = HAL_I2C_Mem_Write(&hi2c3,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			100);
	
	return result;
}
//------------------------------------------------------------------------------
static MAX30208_Result MAX30208_Read(MAX30208_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	MAX30208_Result result =  HAL_I2C_Mem_Read(&hi2c3,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			100);
	
	return result;
}
//==============================================================================
static MAX30208_BusT Bus =
{
	.Handle = &hi2c1,

	.Write = (MAX30208_WriteAction)MAX30208_Write,
	.Read = (MAX30208_ReadAction)MAX30208_Read,
};
//------------------------------------------------------------------------------
static DRV2605_ModeControlT ModeControl =
{
	.Set = MAX30208_SetMode,
	.Get = MAX30208_GetMode
};
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return MAX30208_Result 
 */
MAX30208_Result MAX30208_AdapterInit(MAX30208_ControlT* device)
{
	if (device)
	{
		return MAX30208_init(&device->Driver, device, &Bus, &ModeControl);
	}

	return MAX30208_ResultError;
}
//==============================================================================
