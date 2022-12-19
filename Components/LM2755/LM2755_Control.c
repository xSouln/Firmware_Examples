//==============================================================================
#include "LM2755_Control.h"
//==============================================================================
static uint16_t LM2755_UpdateTime;
static uint8_t LM2755_Brightness;
//==============================================================================
/**
 * @brief LM2755 сontrol main handler
 * @param device 
 */
void LM2755_ControlHandler(LM2755_ControlT* device)
{
	if (LM2755_UpdateTime)
	{
		return;
	}

	LM2755_UpdateTime = device->Options.UpdateTime;
	
	LM2755_Brightness++;
	LM2755_Brightness &= 0x0f;

	LM2755_SetHighLevel(&device->Driver, LM2755_DIODE3_BIT, LM2755_Brightness);
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void LM2755_TimeSynchronization(LM2755_ControlT* device)
{
	if (LM2755_UpdateTime)
	{
		LM2755_UpdateTime--;
	}
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult LM2755_ControlInit(LM2755_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "LM2755_ControlT";
		device->Parent = parent;

		device->Options.UpdateTime = LM2755_UPDATE_TIME;

		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
