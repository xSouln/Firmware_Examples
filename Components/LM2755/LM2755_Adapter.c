//==============================================================================
#include "LM2755_Adapter.h"
//==============================================================================
static LM2755_Result LM2755_PrivateWrite(LM2755_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	LM2755_Result result = HAL_I2C_Mem_Write(&hi2c1,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			100);
	
	return result;
}
//==============================================================================
static LM2755_Result LM2755_PrivateRead(LM2755_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	LM2755_Result result = HAL_I2C_Mem_Read(&hi2c1,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			100);
	
	return result;
}
//==============================================================================
static void LM2755_PrivateSetResetState(LM2755_RESET_STATE state)
{
	switch((uint8_t)state)
	{
		case LM2755_RESET_STATE_ENABLE : RGB_DRIVER_ENABLE_GPIO_Port->ODR &= ~RGB_DRIVER_ENABLE_Pin; return;
		case LM2755_RESET_STATE_DISABLE : RGB_DRIVER_ENABLE_GPIO_Port->ODR |= RGB_DRIVER_ENABLE_Pin; return;
	}
}
//==============================================================================
static LM2755_RESET_STATE LM2755_PrivateGetResetState()
{
	return (RGB_DRIVER_ENABLE_GPIO_Port->ODR & RGB_DRIVER_ENABLE_Pin) > 0
	? LM2755_RESET_STATE_DISABLE : LM2755_RESET_STATE_ENABLE;
}
//==============================================================================
static LM2755_Result LM2755_PrivateSetValueAction(LM2755_T* driver, LM2755_ValueSelector selector, uint32_t value)
{
	switch((uint8_t)selector)
	{
		case LM2755_ValueResetState:
			LM2755_PrivateSetResetState(value);
			break;

		default: return LM2755_ResultNotSupported;
	}

	return LM2755_ResultAccept;
}
//------------------------------------------------------------------------------
static uint32_t LM2755_PrivateGetValueAction(LM2755_T* driver, LM2755_ValueSelector selector)
{
	switch((uint8_t)selector)
	{
		case LM2755_ValueResetState:
			return LM2755_PrivateGetResetState();
	}

	return 0;
}
//------------------------------------------------------------------------------
static LM2755_Result LM2755_PrivateRequestListener(void* driver, LM2755_RequestSelector selector, uint32_t args, uint32_t size)
{
	switch((uint8_t)selector)
	{
		case LM2755_RequestDelay:
			HAL_Delay(args);
			break;

		default: return LM2755_ResultNotSupported;
	}

	return LM2755_ResultAccept;
}
//==============================================================================
static LM2755_BusT LM2755_PrivateBus =
{
	.Handle = &hi2c1,

	.Address = LM2755_ADDRESS_IS_ADR_PIN_LOW,

	.Write = (LM2755_WriteAction)LM2755_PrivateWrite,
	.Read = (LM2755_ReadAction)LM2755_PrivateRead
};
//------------------------------------------------------------------------------
static LM2755_InterfaceT LM2755_PrivateInterface =
{
	.RequestListener = (LM2755_RequestListenerT)LM2755_PrivateRequestListener,
	.SetValue = (LM2755_SetValueActionT)LM2755_PrivateSetValueAction,
	.GetValue = (LM2755_GetValueActionT)LM2755_PrivateGetValueAction
};
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return LM2755_Result 
 */
LM2755_Result LM2755_AdapterInit(LM2755_ControlT* device)
{
	if (device)
	{	
		return LM2755_Init(&device->Driver, device, &LM2755_PrivateBus, &LM2755_PrivateInterface, 0);
	}

	return LM2755_ResultError;
}
//==============================================================================
