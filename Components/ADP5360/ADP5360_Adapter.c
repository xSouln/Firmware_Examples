//==============================================================================
#include "ADP5360_Adapter.h"
//==============================================================================
static ADP5360_Result ADP5360_PrivateWrite(ADP5360_T* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
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
//------------------------------------------------------------------------------
static ADP5360_Result ADP5360_PrivateRead(ADP5360_T* drver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size)
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
//------------------------------------------------------------------------------
static void ADP5360_PrivateSetSwitchingState(ADP5360_T* driver, ADP5360_SwitchingStates state)
{
	switch ((uint8_t)state)
	{
		case ADP5360_SwitchingStateDisable: ADP_STOP_SWITCHING_GPIO_Port->ODR &= ~ADP_STOP_SWITCHING_Pin;
		case ADP5360_SwitchingStateEnable: ADP_STOP_SWITCHING_GPIO_Port->ODR |= ADP_STOP_SWITCHING_Pin;
	}
}
//------------------------------------------------------------------------------
static ADP5360_SwitchingStates ADP5360_PrivateGetSwitchingState(ADP5360_T* driver)
{
	if (ADP_STOP_SWITCHING_GPIO_Port->ODR & ADP_STOP_SWITCHING_Pin)
	{
		return ADP5360_SwitchingStateEnable;
	}
	return ADP5360_SwitchingStateDisable;
}
//------------------------------------------------------------------------------
static ADP5360_Result ADP5360_PrivateSetValueAction(ADP5360_T* driver, ADP5360_ValueSelector selector, uint32_t value)
{
	switch ((uint8_t)selector)
	{
		case ADP5360_ValueSwitchingState:
			ADP5360_PrivateSetSwitchingState(driver, value);
			break;

		default: return ADP5360_ResultNotSupported;
	}

	return ADP5360_ResultAccept;
}
//------------------------------------------------------------------------------
static uint32_t ADP5360_PrivateGetValueAction(ADP5360_T* driver, ADP5360_ValueSelector selector)
{
	switch ((uint8_t)selector)
	{
		case ADP5360_ValueSwitchingState: return ADP5360_PrivateGetSwitchingState(driver);
		default: return 0;
	}
}
//------------------------------------------------------------------------------
static ADP5360_Result ADP5360_PrivateRequestListener(ADP5360_T* driver, ADP5360_RequestSelector selector, uint32_t args, uint32_t count)
{
	switch ((uint8_t)selector)
	{
		case ADP5360_RequestDelay:
			HAL_Delay(args);
			break;

		default: return ADP5360_ResultNotSupported;
	}

	return xResultAccept;
}
//==============================================================================
static ADP5360_BusT ADP5360_PrivateBus =
{
	.Handle = &hi2c3,

	.Write = (ADP5360_WriteAction)ADP5360_PrivateWrite,
	.Read = (ADP5360_ReadAction)ADP5360_PrivateRead,
};
//------------------------------------------------------------------------------
static ADP5360_InterfaceT ADP5360_PrivateInterface =
{
	.RequestListener = (ADP5360_RequestListenerT)ADP5360_PrivateRequestListener,
	.SetValue = (ADP5360_SetValueActionT)ADP5360_PrivateSetValueAction,
	.GetValue = (ADP5360_GetValueActionT) ADP5360_PrivateGetValueAction,
};
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @return ADP5360_Result
 */
ADP5360_Result ADP5360_AdapterInit(ADP5360_ControlT* device)
{
	if (device)
	{
		return ADP5360_Init(&device->Driver, device, &ADP5360_PrivateBus, &ADP5360_PrivateInterface);
	}

	return HAL_ERROR;
}
//==============================================================================
