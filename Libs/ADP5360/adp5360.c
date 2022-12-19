/*
 * adp5360.c
 *
 *  Created on: Mar 15, 2021
 *      Author: eInfochips
 */
#include "adp5360.h"
#include "stdio.h"
//==============================================================================
/* Global variables */
#define ADP5360_I2C_ADDRESS 0x8C
#define I2C_DELAY 10000
//==============================================================================
ADP5360_Result ADP5360_ReadRegister(ADP5360_T* driver, uint8_t reg, uint8_t *val)
{
	ADP5360_Result result = driver->Bus->Read(driver, ADP5360_I2C_ADDRESS, reg, val, 1);
	
	return result;
}
//==============================================================================
ADP5360_Result ADP5360_WriteRegister(ADP5360_T* driver, uint8_t reg, uint8_t val, uint8_t mask)
{
	uint8_t buf = 0;
	ADP5360_Result result = driver->Bus->Read(driver, ADP5360_I2C_ADDRESS, reg, &buf, 1);
	
	if (result != ADP5360_ResultAccept)
	{
		return result;
	}

	buf &= ~mask;
	buf |= val;
	
	result = driver->Bus->Write(driver, ADP5360_I2C_ADDRESS, reg, &buf, 1);
	
	return result;
}
//==============================================================================
ADP5360_Result ADP5360_WriteValue(ADP5360_T* driver, uint8_t reg, uint8_t val)
{
	ADP5360_Result result = driver->Bus->Write(driver, ADP5360_I2C_ADDRESS, reg, &val, 1);

	return result;
}
//==============================================================================
ADP5360_Result ADP5360_Init(ADP5360_T* driver,
							void* parent,
							ADP5360_BusT* bus,
							ADP5360_InterfaceT* interface)
{
	if(!driver || !bus || !bus->Read || !bus->Write || !interface)
	{
		return ADP5360_ResultError;
	}
	
	driver->Bus = bus;
	driver->Interface = interface;
	driver->State.BusInit = 1;
	
	driver->Description = "ADP5360_T";
	driver->Parent = parent;


	/* Verify Manufacture ID */
	ADP5360_ReadRegister(driver, ADP5360_REG_MANUF_ID, &driver->Id.Manufacture);
	if (driver->Id.Manufacture != ADP5360_VAL_MANUF_ID)
	{
		return ADP5360_ResultError;
	}

	/* Verify Revision ID */
	ADP5360_ReadRegister(driver, ADP5360_REG_REV, &driver->Id.Revision);
	if (driver->Id.Revision != ADP5360_VAL_REV)
	{
		return ADP5360_ResultError;
	}
	
	driver->State.DriverInit = 1;

	return ADP5360_ResultAccept;
}
//==============================================================================
void ADP5360_DeInit(ADP5360_T* driver)
{
	if (driver)
	{
		driver->State.Value = 0;
	}
}
//==============================================================================
int ADP5360_Set_Charge_Current(unsigned int value)
{
	return 0;
}
//==============================================================================
ADP5360_Result ADP5360_SetSwitchingState(ADP5360_T* driver, ADP5360_SwitchingStates state)
{
	driver->State.SwitchingIsEnable = state == ADP5360_SwitchingStateEnable;
	
	return driver->Interface->SetValue(driver, ADP5360_ValueSwitchingState, state);
}
//==============================================================================
ADP5360_SwitchingStates ADP5360_GetSwitchingState(ADP5360_T* driver)
{
	return driver->Interface->GetValue(driver, ADP5360_ValueSwitchingState);
}
//==============================================================================
ADP5360_Result ADP5360_EnableCharging(ADP5360_T* driver)
{
	return ADP5360_WriteRegister(driver,
			ADP5360_REG_CHARGER_FUNCTION_SETTING,
			0x01,
			0x01);
}
//==============================================================================
ADP5360_Result ADP5360_DisableCharging(ADP5360_T* driver)
{
	return ADP5360_WriteRegister(driver,
			ADP5360_REG_CHARGER_FUNCTION_SETTING,
			0x00,
			0x01);
}
//==============================================================================
ADP5360_Result ADP5360_UpdateConnectionState(ADP5360_T* driver)
{
	uint8_t regval = 0;
	ADP5360_BatteryConnectionStates state = BATTERY_CONNECTION_UNDEFINED;
	ADP5360_Result result = ADP5360_ReadRegister(driver, ADP5360_REG_CHARGER_STATUS2, &regval);
	
	if (result != ADP5360_ResultAccept)
	{
		return result;
	}
	
	switch (regval & 0x07)
	{
		case 0:
			state = BATTERY_CONNECTION_IS_CONNECT;
			break;
		case 1:
			state = BATTERY_CONNECTION_NOT_CONNECTED;
			break;
		case 2:
			state = BATTERY_CONNECTION_VBSNS_VTRK;
			break;
		case 3:
			state = BATTERY_CONNECTION_VTRK_VBSNS_VWEAK;
			break;
		case 4:
			state = BATTERY_CONNECTION_VBSNS_VWEAK;
			break;
	}

	driver->Battery.ConnectionState = state;

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_DetectCharger(ADP5360_T* driver)
{
	return 0;
}
//==============================================================================
ADP5360_Result ADP5360_EnableManualReset(ADP5360_T* driver)
{
	return ADP5360_WriteRegister(driver,
			ADP5360_REG_INTERRUPT_ENABLE2,
			0x80,
			0x80);
}
//==============================================================================
ADP5360_Result ADP5360_EnableShutdownMode(ADP5360_T* driver)
{
	return ADP5360_WriteRegister(driver,
			ADP5360_REG_INTERRUPT_ENABLE2,
			0x02,
			0x02);
}
//==============================================================================
ADP5360_Result ADP5360_UpdateVoltage(ADP5360_T* driver)
{
	uint8_t voltage_h, voltage_l;
	ADP5360_Result result;

	/* Read voltage high bits */
	result = ADP5360_ReadRegister(driver, ADP5360_REG_VBAT_READ_H, &voltage_h);
	if (result != ADP5360_ResultAccept)
	{
		return ADP5360_ResultError;
	}

	/* Read voltage low bits */
	result = ADP5360_ReadRegister(driver, ADP5360_REG_VBAT_READ_L, &voltage_l);
	if (result != ADP5360_ResultAccept)
	{
		return ADP5360_ResultError;
	}
	
	driver->Battery.Voltage = ((((uint16_t)voltage_h << 8) | voltage_l) >> 3);

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_UpdateCapacity(ADP5360_T* driver)
{
	uint8_t capacity;
	ADP5360_Result result;

	/* Read capacity  */
	result = ADP5360_ReadRegister(driver, ADP5360_REG_BAT_SOC, &capacity);
	if (result != ADP5360_ResultAccept)
	{
		return ADP5360_ResultError;
	}

	driver->Battery.Capacity = capacity;

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_UpdateTemperature(ADP5360_T* driver)
{
	uint8_t temp;
	BatteryTemperatureTypeDef bat_temp;
	ADP5360_Result result;

	/* Read Temperature  */
	result = ADP5360_ReadRegister(driver, ADP5360_REG_CHARGER_STATUS2, &temp);
	if (result != ADP5360_ResultAccept)
	{
		return ADP5360_ResultError;
	}

	switch ((temp & 0xE0) >> 5)
	{
	case 0:
		bat_temp = BATTERY_TEMP_OFF;
		break;
	case 1:
		bat_temp = BATTERY_TEMP_COLD;
		break;
	case 2:
		bat_temp = BATTERY_TEMP_COOL;
		break;
	case 3:
		bat_temp = BATTERY_TEMP_WARM;
		break;
	case 7:
		bat_temp = BATTERY_TEMP_OK;
		break;
	}

	driver->Battery.Temperature = bat_temp;

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_UpdateChargeState(ADP5360_T* driver)
{
	uint8_t temp = 0;
	ChargerStatusTypeDef chg_state = BATTERY_CHARGER_OFF;
	ADP5360_Result result;

	/* Read Charging state  */
	result = ADP5360_ReadRegister(driver, ADP5360_REG_CHARGER_STATUS1, &temp);
	if (result != ADP5360_ResultAccept)
	{
		return ADP5360_ResultError;
	}

	switch (temp & 0x07)
	{
	case 0:
		chg_state = BATTERY_CHARGER_OFF;
		break;

	case 1:
		chg_state = BATTERY_CHARGER_TRICKLE_CHARGE;
		break;

	case 2:
		chg_state = BATTERY_CHARGER_FASTCHARGE_CC;
		break;

	case 3:
		chg_state = BATTERY_CHARGER_FASTCHARGE_CV;
		break;

	case 4:
		chg_state = BATTERY_CHARGER_CHARGE_COMPLETE;
		break;

	case 5:
		chg_state = BATTERY_CHARGER_LDO_MODE;
		break;

	case 6:
		chg_state = BATTERY_CHARGER_TIMER_EXPIRED;
		break;

	case 7:
		chg_state = BATTERY_CHARGER_BATTERY_DETECTION;
		break;
	}
	
	driver->Battery.ChargeState = chg_state;

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_Configure(ADP5360_T* driver, BatteryConfigTypeDef *battery)
{
	ADP5360_Result result = ADP5360_ResultError;

	if (battery->voltage)
	{
		result = ADP5360_WriteRegister(driver,
										ADP5360_REG_CHARGER_TERMINATION_SETTING,
										battery->voltage,
										0xFC);

		if (result != ADP5360_ResultAccept)
		{
			return ADP5360_ResultError;
		}
		
		driver->Config.voltage = battery->voltage;
	}

	if (battery->charge_current)
	{
		result = ADP5360_WriteRegister(driver,
										ADP5360_REG_CHARGER_CURRENT_SETTING,
										battery->charge_current,
										0x1F);

		if (result != ADP5360_ResultAccept)
		{
			return ADP5360_ResultError;
		}
		
		driver->Config.charge_current = battery->charge_current;
	}

	return ADP5360_ResultAccept;
}
//==============================================================================
ADP5360_Result ADP5360_EnableGauging(ADP5360_T* driver)
{
	if (driver && driver->Bus)
	{
		return ADP5360_WriteRegister(driver, ADP5360_REG_FUEL_GAUGE_MODE, 0x01, 0x01);
	}
	return ADP5360_ResultError;
}
//==============================================================================
/* Read battery parameters */
ADP5360_Result ADP5360_UpdateData(ADP5360_T* driver)
{
	/* Voltage */
	ADP5360_UpdateVoltage(driver);

	/* Capacity */
	ADP5360_UpdateCapacity(driver);

	/* Temperature */
	ADP5360_UpdateTemperature(driver);

	/* Charging State */
	ADP5360_UpdateChargeState(driver);

	ADP5360_UpdateConnectionState(driver);

	ADP5360_ReadRegister(driver, ADP5360_REG_FAULT, &driver->FaultStatus.Value);

	driver->State.SwitchingIsEnable = ADP5360_GetSwitchingState(driver) == ADP5360_SwitchingStateEnable;

	return ADP5360_ResultAccept;
}
//==============================================================================
