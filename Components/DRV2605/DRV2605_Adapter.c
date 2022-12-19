//==============================================================================
#include "DRV2605_Adapter.h"
//==============================================================================
static DRV2605_Result Write(DRV2605_T* driver, uint8_t addres, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	DRV2605_Result result = HAL_I2C_Mem_Write(&hi2c1,
			addres,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			1000);
	
	return result;
}
//==============================================================================
static DRV2605_Result Read(DRV2605_T* driver, uint8_t addres, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	DRV2605_Result result = HAL_I2C_Mem_Read(&hi2c1,
			addres,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			1000);
	
	return result;
}
//==============================================================================
static void SetResetState(DRV2605_RESET_STATE state)
{
	switch((uint8_t)state)
	{
		case DRV2605_RESET_STATE_ON : MOTOR_RESET_N_GPIO_Port->ODR &= ~MOTOR_RESET_N_Pin; return;
		case DRV2605_RESET_STATE_OFF : MOTOR_RESET_N_GPIO_Port->ODR |= MOTOR_RESET_N_Pin; return;
	}
}
//==============================================================================
static DRV2605_RESET_STATE GetResetState()
{
	if (MOTOR_RESET_N_GPIO_Port->ODR & MOTOR_RESET_N_Pin)
	{
		return DRV2605_RESET_STATE_OFF;
	}
	return DRV2605_RESET_STATE_ON;
}
//==============================================================================
static DRV2605_BusT DRV2605_Bus =
{
	.Handle = &hi2c1,

	.Write = (DRV2605_WriteAction)Write,
	.Read = (DRV2605_ReadAction)Read,
};
//==============================================================================
static DRV2605_ResetStateT DRV2605_ResetControl =
{
	.Set = SetResetState,
	.Get = GetResetState
};
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return xResult 
 */
xResult DRV2605_AdapterInit(DRV2605_ControlT* device)
{
	if (device)
	{
		SetResetState(DRV2605_RESET_STATE_OFF);

		return DRV2605_init(&device->Driver, device, &DRV2605_Bus, &DRV2605_ResetControl);
	}
	
	return xResultError;
}
//==============================================================================
