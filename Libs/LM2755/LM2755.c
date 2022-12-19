/*
 * LM2755.c
 *
 *  Created on:
 *      Author: Souln
 */
#include "LM2755.h"
#include <string.h>
//==============================================================================
LM2755_Result LM2755_WriteToRegister(LM2755_T* device, LM2755_REG reg, uint8_t data)
{
	LM2755_Result result = device->Bus->Write(device, device->Bus->Address | 0x01, reg, &data, sizeof(data));

	return result;
}
//==============================================================================
LM2755_Result LM2755_ReadFromRegister(LM2755_T* device, LM2755_REG reg, uint8_t* data)
{
	LM2755_Result result = device->Bus->Read(device, device->Bus->Address | 0x01, reg, data, sizeof(uint8_t));

	return result;
}
//==============================================================================
LM2755_Result LM2755_SetGeneralPurpose(LM2755_T* device, LM2755_GENERAL_PURPOSE_CONTROL control)
{
	return LM2755_WriteToRegister(device, LM2755_REG_GENERAL_PURPOSE, control);
}
//==============================================================================
LM2755_Result LM2755_SetTimeStep(LM2755_T* device, LM2755_TimeStepControlT control)
{
	control &= LM2755_TIME_STEP_CONTROL_MASK;
	return LM2755_WriteToRegister(device, LM2755_REG_TIME_STEP, control);
}
//==============================================================================
LM2755_Result LM2755_SetHighLevel(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_BrigtnessLevelControlT control)
{
	control &= LM2755_BRIGHTNESS_LEVEL_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	uint8_t i = 0;
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_HIGH_LEVEL, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].HighLevel = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetLowLevel(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_BrigtnessLevelControlT control)
{
	control &= LM2755_BRIGHTNESS_LEVEL_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_LOW_LEVEL, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].LowLevel = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetDelay(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimeDelayControlT control)
{
	control &= LM2755_TIME_DELAY_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_DELAY, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].TimeDelay = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetTimeRise(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control)
{
	control &= LM2755_TIME_RISE_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_TIME_RISE, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].TimeRise = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetTimeHigth(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control)
{
	control &= LM2755_TIME_HIGH_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_TIME_HIGH, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].TimeHigh = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetTimeFall(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control)
{
	control &= LM2755_TIME_FALL_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_TIME_FALL, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].TimeFall = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
LM2755_Result LM2755_SetTimeLow(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control)
{
	control &= LM2755_TIME_LOW_CONTROL_MASK;
	LM2755_Result result = LM2755_ResultError;
	uint8_t i = 0;
	uint8_t diode_reg_select = LM2755_D1_REG_SELECT;
	
	while (selected_diode && i < LM2755_DIODES_COUNT)
	{
		if (selected_diode & 0x01)
		{
			result = LM2755_WriteToRegister(device, diode_reg_select | LM2755_REG_TIME_LOW, control);
			
			if (result == LM2755_ResultAccept)
			{
				device->Options->Diodes[i].TimeLow = control;
			}
		}
		
		selected_diode >>= 1;
		diode_reg_select += LM2755_REG_SELECT_NEXT;
		i++;
	}
	
	return result;
}
//==============================================================================
void LM2755_Reset(LM2755_T* device)
{
	device->Interface->SetValue(device, LM2755_ValueResetState, LM2755_RESET_STATE_ENABLE);
	device->Interface->RequestListener(device, LM2755_RequestDelay, 100, 0);
	device->Interface->SetValue(device, LM2755_ValueResetState, LM2755_RESET_STATE_DISABLE);
	device->Interface->RequestListener(device, LM2755_RequestDelay, 20, 0);
}
//==============================================================================
static LM2755_OptionsT default_options =
{
	.GeneralPurpose = LM2755_D1_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL
		| LM2755_D3_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL,

	.TimeStep = 0x07,
	
	.Diodes =
	{
		{
			.HighLevel = 0xff,
			.TimeHigh = 128,
			.TimeFall = 128
		},
		
		{
			.HighLevel = 0xff,
			.TimeHigh = 128,
			.TimeFall = 128
		},
		
		{
			.HighLevel = 0xff,
			.TimeHigh = 128,
			.TimeFall = 128
		},
	}
};
//==============================================================================
LM2755_Result LM2755_Init(LM2755_T* device,
													void* parent,
													LM2755_BusT* bus,
													LM2755_InterfaceT* interface,
													LM2755_OptionsT* options)
{
	if (device)
	{
		device->State.Value = 0;
		
		if (!bus || !bus->Write || !bus->Read)
		{
			return LM2755_ResultError;
		}
		
		if (!interface)
		{
			return LM2755_ResultError;
		}
		
		if (!options)
		{
			options = (LM2755_OptionsT*)&default_options;
		}
		
		device->Description = "LM2755_T";
		device->Parent = parent;
		device->Bus = bus;
		device->Interface = interface;
		device->Options = options;
		
		device->State.BusInit = 1;
		device->State.DriverInit = 1;

		LM2755_Reset(device);

		LM2755_SetGeneralPurpose(device, options->GeneralPurpose);
		LM2755_SetTimeStep(device, options->TimeStep);
		
		for (uint8_t i = 0; i < LM2755_DIODES_COUNT; i++)
		{
			LM2755_SetTimeHigth(device, 0x01 << i, (&device->Options->Diodes[i])->TimeHigh);
			LM2755_SetTimeLow(device, 0x01 << i, (&device->Options->Diodes[i])->TimeLow);

			LM2755_SetHighLevel(device, 0x01 << i, (&device->Options->Diodes[i])->HighLevel);
			LM2755_SetLowLevel(device, 0x01 << i, (&device->Options->Diodes[i])->LowLevel);
			
			LM2755_SetDelay(device, 0x01 << i, (&device->Options->Diodes[i])->TimeDelay);
			
			LM2755_SetTimeRise(device, 0x01 << i, (&device->Options->Diodes[i])->TimeRise);
			LM2755_SetTimeFall(device, 0x01 << i, (&device->Options->Diodes[i])->TimeFall);
		}

		return LM2755_ResultAccept;
	}
	return LM2755_ResultNullPointer;
}
//==============================================================================
