//==============================================================================
#ifndef ADP5360_H_
#define ADP5360_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
//includes:

#include <stdint.h>
//==============================================================================
//defines:

/* I2C Registers */
#define ADP5360_REG_MANUF_ID							0x00
#define ADP5360_REG_REV									0x01
#define ADP5360_REG_CHARGER_VBUS_ILIM					0x02
#define ADP5360_REG_CHARGER_TERMINATION_SETTING			0x03
#define ADP5360_REG_CHARGER_CURRENT_SETTING				0x04
#define ADP5360_REG_CHARGER_VOLTAGE_THRESHOLD 			0x05
#define ADP5360_REG_CHARGER_TIMER_SETTING 				0x06
#define ADP5360_REG_CHARGER_FUNCTION_SETTING			0x07
#define ADP5360_REG_CHARGER_STATUS1						0x08
#define ADP5360_REG_CHARGER_STATUS2						0x09
#define ADP5360_REG_BATTERY_THERMISTOR_CONTROL			0x0A
#define ADP5360_REG_THERMISTOR_60C_THRESHOLD			0x0B
#define ADP5360_REG_THERMISTOR_45C_THRESHOLD			0x0C
#define ADP5360_REG_THERMISTOR_10C_THRESHOLD			0x0D
#define ADP5360_REG_THERMISTOR_0C_THRESHOLD				0x0E
#define ADP5360_REG_THR_VOLTAGE_LOW						0x0F
#define ADP5360_REG_THR_VOLTAGE_HIGH					0x10
#define ADP5360_REG_BATTERY_PROTECTION_CONTROL			0x11
#define ADP5360_REG_BATTERY_PROTECTION_UNDERVOLTAGE		0x12
#define ADP5360_REG_BATTERY_PROTECTION_OVERCHARGE		0x13
#define ADP5360_REG_BATTERY_PROTECTION_OVERVOLTAGE		0x14
#define ADP5360_REG_BATTERY_PROTECTION_CHARGE_OVERCHARGE 0x15
#define ADP5360_REG_V_SOC_0								0x16
#define ADP5360_REG_V_SOC_5								0x17
#define ADP5360_REG_V_SOC_19							0x18
#define ADP5360_REG_V_SOC_11							0x19
#define ADP5360_REG_V_SOC_28							0x1A
#define ADP5360_REG_V_SOC_41							0x1B
#define ADP5360_REG_V_SOC_55							0x1C
#define ADP5360_REG_V_SOC_69							0x1D
#define ADP5360_REG_V_SOC_84							0x1E
#define ADP5360_REG_V_SOC_100							0x1F
#define ADP5360_REG_BAT_CAP								0x20
#define ADP5360_REG_BAT_SOC								0x21
#define ADP5360_REG_BAT_SOCACM_CTL						0x22
#define ADP5360_REG_BAT_SOCACM_H						0x23
#define ADP5360_REG_BAT_SOCACM_L						0x24
#define ADP5360_REG_VBAT_READ_H							0x25
#define ADP5360_REG_VBAT_READ_L							0x26
#define ADP5360_REG_FUEL_GAUGE_MODE						0x27
#define ADP5360_REG_SOC_RESET							0x28
#define ADP5360_REG_BUCK_CONF							0x29
#define ADP5360_REG_BUCK_OUTPUT_VOLTAGE					0x2A
#define ADP5360_REG_BUCK_BOOST_CONF						0x2B
#define ADP5360_REG_BUCK_BOOST_OUTPUT_VOLTAGE			0x2C
#define ADP5360_REG_SUPERVISORY_SETTING					0x2D
#define ADP5360_REG_FAULT								0x2E
#define ADP5360_REG_PGOOD_STATUS						0x2F
#define ADP5360_REG_PGOOD1_MASK							0x30
#define ADP5360_REG_PGOOD2_MASK							0x31
#define ADP5360_REG_INTERRUPT_ENABLE1					0x32
#define ADP5360_REG_INTERRUPT_ENABLE2					0x33
#define ADP5360_REG_INTERRUPT_FLAG1						0x34
#define ADP5360_REG_INTERRUPT_FLAG2						0x35
#define ADP5360_REG_SHIPMODE							0x36

/* Register Values */
#define ADP5360_VAL_MANUF_ID							0x10
#define ADP5360_VAL_REV									0x08

#define ENABLE_CHARGING									0x01
#define DISABLE_CHARGING								0x00
//==============================================================================
//types:

/* result of used functions */
typedef enum
{
	ADP5360_ResultAccept,
	ADP5360_ResultError,
	ADP5360_ResultNullPointer,
	ADP5360_ResultBusy,
	ADP5360_ResultNotSupported

} ADP5360_Result;
//------------------------------------------------------------------------------

typedef enum
{
	BATTERY_TEMP_OFF,
	BATTERY_TEMP_COLD,
	BATTERY_TEMP_COOL,
	BATTERY_TEMP_WARM,
	BATTERY_TEMP_HOT,
	BATTERY_TEMP_OK,
} BatteryTemperatureTypeDef;
//------------------------------------------------------------------------------

typedef enum
{
	BATTERY_CHARGER_OFF,
	BATTERY_CHARGER_TRICKLE_CHARGE,
	BATTERY_CHARGER_FASTCHARGE_CC,
	BATTERY_CHARGER_FASTCHARGE_CV,
	BATTERY_CHARGER_CHARGE_COMPLETE,
	BATTERY_CHARGER_LDO_MODE,
	BATTERY_CHARGER_TIMER_EXPIRED,
	BATTERY_CHARGER_BATTERY_DETECTION,
} ChargerStatusTypeDef;
//------------------------------------------------------------------------------

typedef enum
{
	BATTERY_CONNECTION_UNDEFINED,
	BATTERY_CONNECTION_NOT_CONNECTED,
	BATTERY_CONNECTION_IS_CONNECT,
	BATTERY_CONNECTION_VBSNS_VTRK,
	BATTERY_CONNECTION_VTRK_VBSNS_VWEAK,
	BATTERY_CONNECTION_VBSNS_VWEAK

} ADP5360_BatteryConnectionStates;
//------------------------------------------------------------------------------

typedef enum
{
	ADP5360_SwitchingStateDisable,
	ADP5360_SwitchingStateEnable

} ADP5360_SwitchingStates;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t ICHG : 5; //1 bit : 10mA //Fast Charge Current Programming Bus.
		uint8_t IEND : 3; //1 bit : 2.5mA //Termination Current Programming Bus.
	};
	uint8_t Value;

} ADP5360_CurrentSettingsT;
//------------------------------------------------------------------------------

typedef enum
{
	ADP5360_VWEAK_2700mV,
	ADP5360_VWEAK_2800mV,
	ADP5360_VWEAK_2900mV,
	ADP5360_VWEAK_3000mV,
	ADP5360_VWEAK_3100mV,
	ADP5360_VWEAK_3200mV,
	ADP5360_VWEAK_3300mV,
	ADP5360_VWEAK_3400mV,

} ADP5360_VWEAK_Values;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t VWEAK : 3; //Weak Battery Voltage Rising Threshold.
		uint8_t IEND : 3; //1 bit : 2.5mA
	};
	uint8_t Value;

} ADP5360_VoltageThreasholdT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t ProtectionFunctionIsEnabled : 1;
		uint8_t ProtectionTriggeredMode : 1;
		uint8_t OvercurrentProtectionMode : 1;
		uint8_t DischargeOvercurrentProtectionMode : 1;
		uint8_t ChargingOvervoltageProtectionMode : 1;
	};
	uint8_t Value;

} ADP5360_BatteryProtectionControlT; //Address 0x11 Bit Descriptions
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t ChargingIsEnabled : 1;
		uint8_t AdaptiveCurrentLimitIsActive : 1;
		uint8_t EndOfChargeIsAllowed : 1;
		uint8_t LDO_IsEnabled : 1;
		uint8_t ISOFET : 1;
		uint8_t Bit5 : 1;
		uint8_t CoolStateChargingCurrentMode, : 1;
		uint8_t JEITA : 1;
	};
	uint8_t Value;

} ADP5360_ChargerFunctionSettingT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t TemperatureShutdown : 1;
		uint8_t Bit1 : 1;
		uint8_t WatchdogTimeout : 1;
		uint8_t Bit3 : 1;
		uint8_t OverchargeOvervoltage : 1;
		uint8_t OverchargeOvercurrent : 1;
		uint8_t OverdischargeOvercurrent : 1;
		uint8_t OverdischargeUndervoltage : 1;
	};
	uint8_t Value;

} ADP5360_FaultStatusT;
//------------------------------------------------------------------------------

typedef struct
{
	uint8_t voltage;
	uint8_t capacity;
	uint8_t charge_voltage;
	uint8_t charge_current;

} BatteryConfigTypeDef;
//------------------------------------------------------------------------------

typedef struct
{
	uint16_t Voltage;
	uint8_t Capacity;
	BatteryTemperatureTypeDef Temperature;
	ChargerStatusTypeDef ChargeState;
	ADP5360_BatteryConnectionStates ConnectionState;

} BatteryDataTypeDef;
//------------------------------------------------------------------------------

typedef enum
{
	ADP5360_ValueIdle,
	ADP5360_ValueSwitchingState

} ADP5360_ValueSelector;
//------------------------------------------------------------------------------

typedef enum
{
	ADP5360_RequestIdle,
	ADP5360_RequestDelay

} ADP5360_RequestSelector;
//------------------------------------------------------------------------------

typedef ADP5360_Result (*ADP5360_WriteAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
typedef ADP5360_Result (*ADP5360_ReadAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);

typedef ADP5360_Result (*ADP5360_RequestListenerT)(void* driver, ADP5360_RequestSelector selector, uint32_t args, uint32_t count);
typedef ADP5360_Result (*ADP5360_SetValueActionT)(void* driver, ADP5360_ValueSelector selector, uint32_t value);
typedef uint32_t (*ADP5360_GetValueActionT)(void* driver, ADP5360_ValueSelector selector);
//------------------------------------------------------------------------------
/**
 * @brief LL I2C control interface
 */
typedef struct
{
	/* handle of a low-level function */
	void* Handle;
	
	ADP5360_WriteAction Write;
	ADP5360_ReadAction Read;
	
} ADP5360_BusT;
//------------------------------------------------------------------------------

/**
 * @brief general purpose interface
 */
typedef struct
{
	ADP5360_RequestListenerT RequestListener;

	ADP5360_GetValueActionT GetValue;
	ADP5360_SetValueActionT SetValue;

} ADP5360_InterfaceT;
//------------------------------------------------------------------------------

typedef struct
{
	uint8_t Manufacture;
	uint8_t Revision;
	
} ADP5360_IdT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t DriverInit : 1;
		uint8_t BusInit : 1;
		uint8_t SwitchingIsEnable : 1;
	};
	
	uint8_t Value;
	
} ADP5360_StateT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;
	
	ADP5360_BusT* Bus;
	ADP5360_InterfaceT* Interface;
	
	ADP5360_IdT Id;

	ADP5360_StateT State;
	BatteryDataTypeDef Battery;
	BatteryConfigTypeDef Config;
	ADP5360_FaultStatusT FaultStatus;
	
} ADP5360_T;
//==============================================================================
//functions:

/* Function definition */
ADP5360_Result ADP5360_Init(ADP5360_T* driver, void* parent, ADP5360_BusT* bus, ADP5360_InterfaceT* interface);
void ADP5360_DeInit(ADP5360_T* driver);

ADP5360_Result ADP5360_WriteValue(ADP5360_T* driver, uint8_t reg, uint8_t val);

ADP5360_Result ADP5360_SetSwitchingState(ADP5360_T* driver, ADP5360_SwitchingStates state);
ADP5360_SwitchingStates ADP5360_GetSwitchingState(ADP5360_T* driver);

ADP5360_Result ADP5360_EnableCharging(ADP5360_T* driver);
ADP5360_Result ADP5360_DisableCharging(ADP5360_T* driver);
ADP5360_Result ADP5360_DetectBattery(ADP5360_T* driver);
ADP5360_Result ADP5360_EnableManualReset(ADP5360_T* driver);
ADP5360_Result ADP5360_EnableShutdownMode(ADP5360_T* driver);
ADP5360_Result ADP5360_ReadRegister(ADP5360_T* driver, uint8_t reg, uint8_t *val);
ADP5360_Result ADP5360_WriteRegister(ADP5360_T* driver, uint8_t reg, uint8_t val, uint8_t mask);
ADP5360_Result ADP5360_EnableGauging(ADP5360_T* driver);

ADP5360_Result ADP5360_UpdateVoltage(ADP5360_T* driver);
ADP5360_Result ADP5360_UpdateCapacity(ADP5360_T* driver);
ADP5360_Result ADP5360_UpdateTemperature(ADP5360_T* driver);
ADP5360_Result ADP5360_UpdateChargeState(ADP5360_T* driver);
ADP5360_Result ADP5360_Configure(ADP5360_T* driver, BatteryConfigTypeDef *battery);
ADP5360_Result ADP5360_UpdateData(ADP5360_T* driver);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
