//==============================================================================
#ifndef LM2755_H
#define LM2755_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include <stdint.h>
//==============================================================================
//defines:

#define LM2755_ADDRESS_IS_ADR_PIN_LOW					(0x18 << 1)
#define LM2755_ADDRESS_IS_ADR_PIN_HIGH				(0x67 << 1)

#define LM2755_BRIGHTNESS_LEVEL_CONTROL_MASK	0x1f
#define LM2755_TIME_STEP_CONTROL_MASK					0x07
#define LM2755_TIME_DELAY_CONTROL_MASK				0xFF
#define LM2755_TIMING_CONTROL_MASK						0xFF

#define LM2755_TIME_RISE_CONTROL_MASK					0xff
#define LM2755_TIME_HIGH_CONTROL_MASK					0xff
#define LM2755_TIME_FALL_CONTROL_MASK					0xff
#define LM2755_TIME_LOW_CONTROL_MASK					0xff
//==============================================================================
//types:

/* result of used functions */
typedef enum
{
	LM2755_ResultAccept,
	LM2755_ResultError,
	LM2755_ResultNullPointer,
	LM2755_ResultBusy,
	LM2755_ResultNotSupported

} LM2755_Result;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_D1_REG_SELECT 			= (uint8_t)0xA0,
	LM2755_D2_REG_SELECT			= (uint8_t)0xB0,
	LM2755_D3_REG_SELECT			= (uint8_t)0xC0,
	
	LM2755_REG_SELECT_NEXT		= (uint8_t)0x10,

} LM2755_DIODE_SELECTED_REG;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_REG_HIGH_LEVEL			= 0x09,
	LM2755_REG_LOW_LEVEL			= 0x08,
	LM2755_REG_DELAY					= 0x01,
	LM2755_REG_TIME_RISE			= 0x05,
	LM2755_REG_TIME_HIGH			= 0x03,
	LM2755_REG_TIME_FALL			= 0x04,
	LM2755_REG_TIME_LOW				= 0x02,

} LM2755_DIODE_REG;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_REG_GENERAL_PURPOSE 		= 0x10,
	LM2755_REG_TIME_STEP 					= 0x20,

	LM2755_REG_D1_HIGH_LEVEL		= LM2755_D1_REG_SELECT | LM2755_REG_HIGH_LEVEL,
	LM2755_REG_D1_LOW_LEVEL			= LM2755_D1_REG_SELECT | LM2755_REG_LOW_LEVEL,
	LM2755_REG_D1_DELAY					= LM2755_D1_REG_SELECT | LM2755_REG_DELAY,
	LM2755_REG_D1_TIME_RISE			= LM2755_D1_REG_SELECT | LM2755_REG_TIME_RISE,
	LM2755_REG_D1_TIME_HIGH			= LM2755_D1_REG_SELECT | LM2755_REG_TIME_HIGH,
	LM2755_REG_D1_TIME_FALL			= LM2755_D1_REG_SELECT | LM2755_REG_TIME_FALL,
	LM2755_REG_D1_TIME_LOW			= LM2755_D1_REG_SELECT | LM2755_REG_TIME_LOW,

	LM2755_REG_D2_HIGH_LEVEL		= LM2755_D2_REG_SELECT | LM2755_REG_HIGH_LEVEL,
	LM2755_REG_D2_LOW_LEVEL			= LM2755_D2_REG_SELECT | LM2755_REG_LOW_LEVEL,
	LM2755_REG_D2_DELAY					= LM2755_D2_REG_SELECT | LM2755_REG_DELAY,
	LM2755_REG_D2_TIME_RISE			= LM2755_D2_REG_SELECT | LM2755_REG_TIME_RISE,
	LM2755_REG_D2_TIME_HIGH			= LM2755_D2_REG_SELECT | LM2755_REG_TIME_HIGH,
	LM2755_REG_D2_TIME_FALL			= LM2755_D2_REG_SELECT | LM2755_REG_TIME_FALL,
	LM2755_REG_D2_TIME_LOW			= LM2755_D2_REG_SELECT | LM2755_REG_TIME_LOW,

	LM2755_REG_D3_HIGH_LEVEL		= LM2755_D3_REG_SELECT | LM2755_REG_HIGH_LEVEL,
	LM2755_REG_D3_LOW_LEVEL			= LM2755_D3_REG_SELECT | LM2755_REG_LOW_LEVEL,
	LM2755_REG_D3_DELAY					= LM2755_D3_REG_SELECT | LM2755_REG_DELAY,
	LM2755_REG_D3_TIME_RISE			= LM2755_D3_REG_SELECT | LM2755_REG_TIME_RISE,
	LM2755_REG_D3_TIME_HIGH			= LM2755_D3_REG_SELECT | LM2755_REG_TIME_HIGH,
	LM2755_REG_D3_TIME_FALL			= LM2755_D3_REG_SELECT | LM2755_REG_TIME_FALL,
	LM2755_REG_D3_TIME_LOW			= LM2755_D3_REG_SELECT | LM2755_REG_TIME_LOW,

} LM2755_REG;
//------------------------------------------------------------------------------

enum LM2755_DIODES
{
	LM2755_DIODE1,
	LM2755_DIODE2,
	LM2755_DIODE3,
	
	LM2755_DIODES_COUNT
};
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_DIODE1_BIT = (1 << LM2755_DIODE1),
	LM2755_DIODE2_BIT = (1 << LM2755_DIODE2),
	LM2755_DIODE3_BIT = (1 << LM2755_DIODE3),

	LM2755_DIODE_ALL = 0xff,
	
	LM2755_DIODES_BITS_COUNT = LM2755_DIODES_COUNT

} LM2755_DIODES_BITS;
//------------------------------------------------------------------------------
/**
 * General Purpose Register Description
 * • Bit 0: enable output D1 with high current level.
 * • Bit 1: enable output D2 with high current level.
 * • Bit 2: enable output D3 with high current level.
 * • Bit 3: enable dimming waveform on output D1.
 * • Bit 4: enable dimming waveform on output D2.
 * • Bit 5: enable dimming waveform on output D3.
 * • Bit 6: enable external clock. '1' = External Clock Sync, '0' = Internal Clock Used
 * • Bit 7: If Bit 7 = 0 the charge pump is powered on before any dimming waveform is enabled. If Bit7 = 1 the dimming waveform
 * can be enabled before charge pump is powered on.
 */
typedef enum
{
	LM2755_D1_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT,
	LM2755_D2_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT,
	LM2755_D3_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT,

	LM2755_D1_ENABLE_DIMMING_WAVEFORM_BIT,
	LM2755_D2_ENABLE_DIMMING_WAVEFORM_BIT,
	LM2755_D3_ENABLE_DIMMING_WAVEFORM_BIT,

	LM2755_ENABLE_EXTERNAL_CLOCK_BIT,
	LM2755_CHARGE_PUMP_SWITCHING_SEQUENCE_BIT,

} LM2755_GENERAL_PURPOSE_CONTROL_BITS;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_D1_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL = (1 << LM2755_D1_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT),
	LM2755_D2_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL = (1 << LM2755_D2_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT),
	LM2755_D3_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL = (1 << LM2755_D3_ENABLE_OUTPUT_WITH_HIGH_CURRENT_LEVEL_BIT),

	LM2755_D1_ENABLE_DIMMING_WAVEFORM = (1 << LM2755_D1_ENABLE_DIMMING_WAVEFORM_BIT),
	LM2755_D2_ENABLE_DIMMING_WAVEFORM = (1 << LM2755_D2_ENABLE_DIMMING_WAVEFORM_BIT),
	LM2755_D3_ENABLE_DIMMING_WAVEFORM = (1 << LM2755_D3_ENABLE_DIMMING_WAVEFORM_BIT),

	LM2755_ENABLE_EXTERNAL_CLOCK = (1 << LM2755_ENABLE_EXTERNAL_CLOCK_BIT),
	LM2755_CHARGE_PUMP_SWITCHING_SEQUENCE = (1 << LM2755_CHARGE_PUMP_SWITCHING_SEQUENCE_BIT),

} LM2755_GENERAL_PURPOSE_CONTROL;
//------------------------------------------------------------------------------
/**
 * IDx LOW = (0.9)(31-nLOW) × IDx Fullscale
 * IDx HIGH = (0.9)(31-nHIGH) × IDx Fullscale
 * nHIGH and nLOW are numbers between 0 and 31 stored in the
 * brightness level registers. When the waveform enable bits are
 * set to '1', nHIGH and nLOW are the brightness level boundries.
 * These equations apply to all Dx outputs and their corresponding registers. A '0' code in the brightness control register sets
 * the current to an "off-state" (0mA).
 * superimposed LM2755_BRIGHTNESS_LEVEL_CONTROL_MASK
 */
typedef uint8_t LM2755_BrigtnessLevelControlT;
//------------------------------------------------------------------------------
/**
 * TStep = 50µsec × 2^(NSTEP+1) if the internal clock is used.
 * TStep = 2^(NSTEP+1) × (32 ÷ fSYNC ) if the external clock on the SYNC pin is used.
 * Bit 3 to Bit 7: Not used
 * superimposed LM2755_TIME_STEP_CONTROL_MASK
 */
typedef uint8_t LM2755_TimeStepControlT;
//------------------------------------------------------------------------------
/**
 * TDelay = TStep × ndelay
 * ndelay is stored in the Dx delay registers and N is stored in the Time Step Control register
 * 0 < ndelay < 0xff
 * superimposed LM2755_TIME_DELAY_CONTROL_MASK
 */
typedef uint8_t LM2755_TimeDelayControlT;
//------------------------------------------------------------------------------
/**
 * TPWM INTERNAL =50µs, N is a value stored in the Time Step
 * register, and nTrise nTfall, nThigh, nTlow are numbers between 0
 * and 255, stored in the timing control registers. The durations
 * of the rise, high, fall and low times are given by
 * trise/fall Total = TSTEP x (nhigh-nlow) x nTrise/fal
 * trise/fall Total = 50µs x (nhigh-nlow) when nTrise/fall = 0
 * thigh/low = TSTEP × (nhigh/low + 1)
 * superimposed LM2755_TIMING_CONTROL_MASK
 */
typedef uint8_t LM2755_TimingConrolT;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_RESET_STATE_DISABLE,
	LM2755_RESET_STATE_ENABLE
	
} LM2755_RESET_STATE;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_ValueIdle,
	LM2755_ValueResetState

} LM2755_ValueSelector;
//------------------------------------------------------------------------------

typedef enum
{
	LM2755_RequestIdle,
	LM2755_RequestDelay

} LM2755_RequestSelector;
//------------------------------------------------------------------------------

typedef LM2755_Result (*LM2755_WriteAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
typedef LM2755_Result (*LM2755_ReadAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);

typedef void (*LM2755_LockActionT)(void* driver);
typedef void (*LM2755_UnlockActionT)(void* driver);

typedef LM2755_Result (*LM2755_SetValueActionT)(void* driver, LM2755_ValueSelector selector, uint32_t value);
typedef uint32_t (*LM2755_GetValueActionT)(void* driver, LM2755_ValueSelector selector);

typedef LM2755_Result (*LM2755_RequestListenerT)(void* driver, LM2755_RequestSelector selector, uint32_t args, uint32_t size);
//------------------------------------------------------------------------------

typedef struct
{
	LM2755_RequestListenerT RequestListener;

	LM2755_GetValueActionT GetValue;
	LM2755_SetValueActionT SetValue;

} LM2755_InterfaceT;
//------------------------------------------------------------------------------
/**
 * Every byte put on the SDIO line must be eight bits long, with
 * the most significant bit (MSB) being transferred first. Each
 * byte of data has to be followed by an acknowledge bit. The
 * acknowledge related clock pulse is generated by the master.
 * The master releases the SDIO line (HIGH) during the acknowledge clock pulse. The LM2755 pulls down the SDIO line
 * during the 9th clock pulse, signifying an acknowledge.
 * The LM2755 generates an acknowledge after each byte has been received.
 * After the START condition, the I2C master sends a chip address. This address is seven bits long followed by an eighth
 * bit which is a data direction bit (R/W). The LM2755 address
 * is 18h is ADR is tied low and 67h if ADR is tied high . For the
 * eighth bit, a “0” indicates a WRITE and a “1” indicates a READ.
 * The second byte selects the register to which the data will be written.
 * The third byte contains data to write to the selected register
 */
typedef struct
{
	/* the bus control object of the used library id needed */
	void* Handle;

	/* E7 ADDR Chip Address Select Input. VIN = 0x67 << 1. Ground = 0x18 << 1. */
	uint8_t Address;

	LM2755_WriteAction Write;
	LM2755_ReadAction Read;

} LM2755_BusT;
//------------------------------------------------------------------------------

typedef struct
{
	LM2755_BrigtnessLevelControlT HighLevel;
	LM2755_BrigtnessLevelControlT LowLevel;

	LM2755_TimeDelayControlT TimeDelay;

	LM2755_TimingConrolT TimeRise;
	LM2755_TimingConrolT TimeHigh;
	LM2755_TimingConrolT TimeFall;
	LM2755_TimingConrolT TimeLow;

} LM2755_DiodeOptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	LM2755_GENERAL_PURPOSE_CONTROL GeneralPurpose;
	LM2755_TimeStepControlT TimeStep;

	LM2755_DiodeOptionsT Diodes[LM2755_DIODES_COUNT];

} LM2755_OptionsT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t DriverInit : 1;
		uint8_t BusInit : 1;
	};
	
	uint8_t Value;
	
} LM2755_StateT;
//------------------------------------------------------------------------------

typedef struct
{
	/* object title for object recognition if needed */
	void* Description;
	/* link to parent if needed */
	void* Parent;

	LM2755_BusT* Bus;
	
	LM2755_InterfaceT* Interface;
	
	LM2755_OptionsT* Options;
	LM2755_StateT State;

} LM2755_T;
//==============================================================================
//functions:

LM2755_Result LM2755_Init(LM2755_T* device,
							void* parent,
							LM2755_BusT* bus,
							LM2755_InterfaceT* interface,
							LM2755_OptionsT* options);

LM2755_Result LM2755_WriteToRegister(LM2755_T* device, LM2755_REG reg, uint8_t data);
LM2755_Result LM2755_ReadFromRegister(LM2755_T* device, LM2755_REG reg, uint8_t* data);

LM2755_Result LM2755_SetGeneralPurpose(LM2755_T* device, LM2755_GENERAL_PURPOSE_CONTROL control);
LM2755_Result LM2755_SetTimeStep(LM2755_T* device, LM2755_TimeStepControlT control);

LM2755_Result LM2755_SetHighLevel(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_BrigtnessLevelControlT control);
LM2755_Result LM2755_SetLowLevel(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_BrigtnessLevelControlT control);
LM2755_Result LM2755_SetDelay(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimeDelayControlT control);
LM2755_Result LM2755_SetTimeRise(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control);
LM2755_Result LM2755_SetTimeHigth(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control);
LM2755_Result LM2755_SetTimeFall(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control);
LM2755_Result LM2755_SetTimeLow(LM2755_T* device, LM2755_DIODES_BITS selected_diode, LM2755_TimingConrolT control);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
