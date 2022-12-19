//==============================================================================
#include "AFE49I30_Adapter.h"
//==============================================================================
static AFE49I30_Result AFE49I30_PrivateWrite(AFE49I30_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	int result = HAL_I2C_Mem_Write(&hi2c3,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			1000);
	
	return result;
}
//==============================================================================
static AFE49I30_Result AFE49I30_PrivateRead(AFE49I30_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	int result = HAL_I2C_Mem_Read(&hi2c3,
			address,
			reg,
			I2C_MEMADD_SIZE_8BIT,
			data,
			data_size,
			1000);
	
	return result;
}
//==============================================================================
static void AFE49I30_PrivateSetResetState(AFE49I30_RESET_STATE state)
{
	switch((uint8_t)state)
	{
		case AFE49I30_RESET_STATE_ON : AFE_RESET_N_GPIO_Port->ODR &= ~AFE_RESET_N_Pin; return;
		case AFE49I30_RESET_STATE_OFF : AFE_RESET_N_GPIO_Port->ODR |= AFE_RESET_N_Pin; return;
	}
}
//------------------------------------------------------------------------------
static AFE49I30_RESET_STATE AFE49I30_PrivateGetResetState()
{
	return (AFE_RESET_N_GPIO_Port->ODR & AFE_RESET_N_Pin) > 0
	? AFE49I30_RESET_STATE_OFF : AFE49I30_RESET_STATE_ON;
}
//==============================================================================
AFE49I30_Result AFE49I30_PrivateRequestListener(void* device, AFE49I30_RequestSelector selector, uint32_t args, uint32_t count)
{
	switch((uint8_t)selector)
	{
		case AFE49I30_RequestDelay:
			HAL_Delay(args);
			break;

		default : return AFE49I30_ResultNotSupported;
	}

	return AFE49I30_ResultAccept;
}
//------------------------------------------------------------------------------
uint32_t AFE49I30_PrivateGetValueAction(void* device, AFE49I30_ValueSelector selector)
{
	switch((uint8_t)selector)
	{
		case AFE49I30_ValueResetState:
			return AFE49I30_PrivateGetResetState();
	}

	return 0;
}
//------------------------------------------------------------------------------
AFE49I30_Result AFE49I30_PrivateSetValueAction(void* device, AFE49I30_ValueSelector selector, uint32_t value)
{
	switch((uint8_t)selector)
	{
		case AFE49I30_ValueResetState:
			AFE49I30_PrivateSetResetState(value);
			break;

		default : return AFE49I30_ResultNotSupported;
	}

	return AFE49I30_ResultAccept;
}
//==============================================================================
static AFE49I30_BusT AFE49I30_PrivateBus =
{
	.Handle = &hi2c3,

	.Write = (AFE49I30_WriteAction)AFE49I30_PrivateWrite,
	.Read = (AFE49I30_ReadAction)AFE49I30_PrivateRead
};
//------------------------------------------------------------------------------
static AFE49I30_InterfaceT AFE49I30_PrivateInterface =
{
	.RequestListener = (AFE49I30_RequestListenerT)AFE49I30_PrivateRequestListener,
	.GetValue = (AFE49I30_GetValueActionT)AFE49I30_PrivateGetValueAction,
	.SetValue = (AFE49I30_SetValueActionT)AFE49I30_PrivateSetValueAction

};
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return AFE49I30_Result 
 */
AFE49I30_Result AFE49I30_AdapterInit(AFE49I30_ControlT* device)
{
	if (device)
	{
		AFE49I30_PrivateSetResetState(AFE49I30_RESET_STATE_OFF);

		return AFE49I30_Init(&device->Driver, device, &AFE49I30_PrivateBus, &AFE49I30_PrivateInterface);
	}

	return AFE49I30_ResultError;
}
//==============================================================================
