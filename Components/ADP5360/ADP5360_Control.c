//==============================================================================
#include "ADP5360_Control.h"
//==============================================================================
static uint16_t ADP5360_UpdateTime;
//==============================================================================
/**
 * @brief main handler
 * @param device 
 */
void ADP5360_ControlHandler(ADP5360_ControlT* device)
{
	if (ADP5360_UpdateTime)
	{
		return;
	}

	ADP5360_UpdateTime = device->Options.UpdateTime;

	ADP5360_UpdateData(&device->Driver);

	if (device->Driver.Battery.ChargeState != BATTERY_CHARGER_CHARGE_COMPLETE)
	{
		if (!device->Driver.State.SwitchingIsEnable)
		{
			ADP5360_SetSwitchingState(&device->Driver, ADP5360_SwitchingStateEnable);
		}
	}
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 * @param device 
 */
void ADP5360_TimeSynchronization(ADP5360_ControlT* device)
{
	if (ADP5360_UpdateTime)
	{
		ADP5360_UpdateTime--;
	}
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device 
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult ADP5360_ControlInit(ADP5360_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "ADP5360_ControlT";
		device->Parent = parent;

		device->Options.UpdateTime = ADP5360_UPDATE_TIME;

		ADP5360_EnableCharging(&device->Driver);

		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
