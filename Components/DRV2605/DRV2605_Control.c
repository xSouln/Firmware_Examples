//==============================================================================
#include "DRV2605_Control.h"
//==============================================================================
/**
 * @brief DRV2605 сontrol main handler
 * @param device
 */
void DRV2605_ControlHandler(DRV2605_ControlT* device)
{
	if (device->UpdateTime)
	{
		return;
	}

	device->UpdateTime = device->Options.UpdateTime;
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 * @param device
 */
void DRV2605_TimeSynchronization(DRV2605_ControlT* device)
{
	if (device->UpdateTime)
	{
		device->UpdateTime--;
	}
}
//==============================================================================
/**
 * @brief initially, it is recommended to call the initialization of the adapter
 * @param device
 * @param parent binding to the parent object
 * @return xResult
 */
xResult DRV2605_ControlInit(DRV2605_ControlT* device, void* parent)
{
	if (device)
	{
		device->Description = "DRV2605_ControlT";
		device->Parent = parent;

		device->Options.UpdateTime = DRV2605_UPDATE_TIME;

		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================
