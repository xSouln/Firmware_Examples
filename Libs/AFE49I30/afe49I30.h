//==============================================================================
#ifndef AFE49I30_H_
#define AFE49I30_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include <stdint.h>
#include <stdbool.h>
//==============================================================================
//defines:

#define AFE49I30_ADDR                   (0x5B<<1)

#define AFE49I30_PAGE_SELECT						(0x01)
#define AFE49I30_TIMER_ENGINE_REG				(0x1D)
#define AFE49I30_FIFO_CONFIG_REG				(0x00)
#define AFE49I30_FIFO_SAMPLE_COUNT_REG	(0x6D)
#define AFE49I30_FIFO_DATA_REG					(0xFF)
#define AFE49I30_MAX_FIFO_SAMPLE				(128)
#define AFE49I30_PPG_FIFO_RDY_CNT				(12)
//==============================================================================
//macros:

#define AFE49I30_CHECK_SPEC_BIT(number,bit) ((number >> bit) & 1ULL)
#define AFE49I30_SET_SPEC_BIT(number,bit)   (number |= (1ULL << bit))
#define AFE49I30_CLEAR_SPEC_BIT(number,bit) (number &= ~(1ULL << bit))
//==============================================================================
//types:

/* result of used functions */
typedef enum
{
	AFE49I30_ResultAccept,
	AFE49I30_ResultError,
	AFE49I30_ResultNullPointer,
	AFE49I30_ResultBusy,
	AFE49I30_ResultNotSupported

} AFE49I30_Result;
//------------------------------------------------------------------------------

typedef enum _FE49I30_ECG_SAMPLE_RATE
{
	AFE49I30_ECG_SAMPLE_400,
	AFE49I30_ECG_SAMPLE_200,
	AFE49I30_ECG_SAMPLE_100,
	AFE49I30_ECG_SAMPLE_50,
	AFE49I30_ECG_SAMPLE_25,
	AFE49I30_PPG_SAMPLE_12,
	
} AFE49I30_ECG_SAMPLE_RATE;
//------------------------------------------------------------------------------

typedef enum _AFE49I30_ECG_GAIN
{
	AFE49I30_ECG_GAIN_1_61,
	AFE49I30_ECG_GAIN_2_15,
	AFE49I30_ECG_GAIN_3_23,
	
 } AFE49I30_ECG_GAIN;
 //------------------------------------------------------------------------------

typedef enum _AFE49I30_MODE
{
	AFE49I30_UNKNOWN_MODE,
	AFE49I30_ECG_MODE,
	AFE49I30_PPG_MODE,
	
} AFE49I30_MODE;
//------------------------------------------------------------------------------

typedef enum _AFE49I30_SLEEP_MODE
{
	AFE49I30_UNKNOWN_SLEEP_MODE,
	AFE49I30_ACTIVE_MODE,
	AFE49I30_LIGHT_SLEEP_MODE,
	AFE49I30_DEEP_SLEEP_MODE,
	
} AFE49I30_SLEEP_MODE;
//------------------------------------------------------------------------------

typedef enum _AFE49I30_PPG_PHASE
{
	AFE49I30_RED_AC,
	AFE49I30_IR_AC,
	AFE49I30_RED_DC,
	AFE49I30_IR_DC,
	
} AFE49I30_PPG_PHASE;
//------------------------------------------------------------------------------

typedef enum
{
	AFE49I30_RESET_STATE_ON,
	AFE49I30_RESET_STATE_OFF
	
} AFE49I30_RESET_STATE;
//------------------------------------------------------------------------------

typedef AFE49I30_Result (*AFE49I30_WriteAction)(void* bus, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
typedef AFE49I30_Result (*AFE49I30_ReadAction)(void* bus, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
//------------------------------------------------------------------------------
typedef struct
{
	void* Handle;

	AFE49I30_WriteAction Write;
	AFE49I30_ReadAction Read;

} AFE49I30_BusT;
//------------------------------------------------------------------------------

typedef enum
{
	AFE49I30_RequestIdle,
	AFE49I30_RequestDelay,

} AFE49I30_RequestSelector;
//------------------------------------------------------------------------------

typedef enum
{
	AFE49I30_ValueIdle,
	AFE49I30_ValueResetState,

} AFE49I30_ValueSelector;
//------------------------------------------------------------------------------

typedef AFE49I30_Result (*AFE49I30_RequestListenerT)(void* device, AFE49I30_RequestSelector selector, uint32_t args, uint32_t count);

typedef uint32_t (*AFE49I30_GetValueActionT)(void* device, AFE49I30_ValueSelector selector);
typedef AFE49I30_Result (*AFE49I30_SetValueActionT)(void* device, AFE49I30_ValueSelector selector, uint32_t value);
//------------------------------------------------------------------------------
typedef struct
{
	AFE49I30_RequestListenerT RequestListener;

	AFE49I30_GetValueActionT GetValue;
	AFE49I30_SetValueActionT SetValue;
	
} AFE49I30_InterfaceT;
//------------------------------------------------------------------------------

typedef struct
{
	AFE49I30_ECG_SAMPLE_RATE SamplingRate;
	AFE49I30_ECG_GAIN Gain;
	
} AFE49I30_OptionsT;
//------------------------------------------------------------------------------

typedef struct
{
	AFE49I30_ECG_SAMPLE_RATE SamplingRate;
	AFE49I30_SLEEP_MODE SleepMode;
	AFE49I30_MODE MeasureMode;

} AFE49I30_ConfigT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t DriverInit : 1;
		uint8_t BusInit : 1;
	};
	
	uint8_t Value;
	
} AFE49I30_StateT;
//------------------------------------------------------------------------------

typedef struct
{
    uint8_t  address;
    uint32_t value;
	
} AFE49I30_Reg_Add_Val;
//------------------------------------------------------------------------------

typedef union
{
	uint8_t Data[sizeof(uint32_t)];
	uint32_t Value;

} AFE49I30_FifoDataT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;
	
	AFE49I30_BusT* Bus;
	AFE49I30_InterfaceT* Interface;

	AFE49I30_FifoDataT TxFifo;
	AFE49I30_FifoDataT RxFifo;
	
	AFE49I30_StateT State;
	AFE49I30_OptionsT Options;
	AFE49I30_ConfigT Config;
	
} AFE49I30_T;
//==============================================================================
//functions:

void AFE49I30_Reset(AFE49I30_T* driver);
void AFE49I30_PowerDown(AFE49I30_T* driver);

AFE49I30_Result AFE49I30_GetRegValue(AFE49I30_T* driver, uint8_t address, uint32_t *data);
AFE49I30_Result AFE49I30_SetRegValue(AFE49I30_T* driver, uint8_t reg, uint32_t data);

AFE49I30_Result AFE49I30_GetFIFOSampleCount(AFE49I30_T* driver, uint8_t *fifoSample);
AFE49I30_Result AFE49I30_ECG_Init(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_PPG_Init(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_ConfigureAFEInSleepMode(AFE49I30_T* driver, AFE49I30_SLEEP_MODE sleep_mode);
AFE49I30_Result AFE49I30_GetCompOut(AFE49I30_T* driver, bool *rcomp_inp_high, bool *rcomp_inp_low, bool *rcomp_inm_high, bool *rcomp_inm_low);
AFE49I30_Result AFE49I30_EnableLeadOffDetection(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_DisableLeadOffDetection(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_StartFIFO(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_StopFIFO(AFE49I30_T* driver);
AFE49I30_Result AFE49I30_SetSamplingRate(AFE49I30_T* driver, AFE49I30_ECG_SAMPLE_RATE value);

AFE49I30_Result AFE49I30_Init(AFE49I30_T* driver,
								void* parent,
								AFE49I30_BusT* bus,
								AFE49I30_InterfaceT* interface);
	
AFE49I30_Result AFE49I30_DeInit(AFE49I30_T* driver);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
