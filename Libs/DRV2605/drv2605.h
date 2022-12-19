//==============================================================================
#ifndef DRV2605_H
#define DRV2605_H
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

#define DRV2605_ADDR 				0xB4 // Device I2C address
#define DRV2605_ID					0x60 //device id

#define DRV2605_REG_STATUS 			0x00 // Status register
#define DRV2605_REG_MODE 			0x01 // Mode register
#define DRV2605_MODE_INTTRIG 		0x00 // Internal trigger mode
#define DRV2605_MODE_EXTTRIGEDGE 	0x01 // External edge trigger mode
#define DRV2605_MODE_EXTTRIGLVL 	0x02 // External level trigger mode
#define DRV2605_MODE_PWMANALOG 		0x03 // PWM/Analog input mode
#define DRV2605_MODE_AUDIOVIBE 		0x04 // Audio-to-vibe mode
#define DRV2605_MODE_REALTIME 		0x05 // Real-time playback (RTP) mode
#define DRV2605_MODE_DIAGNOS 		0x06 // Diagnostics mode
#define DRV2605_MODE_AUTOCAL 		0x07 // Auto calibration mode

#define DRV2605_REG_RTPIN 			0x02 // Real-time playback input register
#define DRV2605_REG_LIBRARY 		0x03 // Waveform library selection register
#define DRV2605_REG_WAVESEQ1 		0x04 // Waveform sequence register 1
#define DRV2605_REG_WAVESEQ2 		0x05 // Waveform sequence register 2
#define DRV2605_REG_WAVESEQ3 		0x06 // Waveform sequence register 3
#define DRV2605_REG_WAVESEQ4 		0x07 // Waveform sequence register 4
#define DRV2605_REG_WAVESEQ5 		0x08 // Waveform sequence register 5
#define DRV2605_REG_WAVESEQ6 		0x09 // Waveform sequence register 6
#define DRV2605_REG_WAVESEQ7 		0x0A // Waveform sequence register 7
#define DRV2605_REG_WAVESEQ8 		0x0B // Waveform sequence register 8

#define DRV2605_REG_GO 				0x0C // Go register
#define DRV2605_REG_OVERDRIVE 		0x0D // Overdrive time offset register
#define DRV2605_REG_SUSTAINPOS 		0x0E // Sustain time offset, positive register
#define DRV2605_REG_SUSTAINNEG 		0x0F // Sustain time offset, negative register
#define DRV2605_REG_BREAK 			0x10 // Brake time offset register
#define DRV2605_REG_AUDIOCTRL 		0x11 // Audio-to-vibe control register
#define DRV2605_REG_AUDIOLVL		0x12 // Audio-to-vibe minimum input level register
#define DRV2605_REG_AUDIOMAX		0x13 // Audio-to-vibe maximum input level register
#define DRV2605_REG_AUDIOOUTMIN		0x14 // Audio-to-vibe minimum output drive register
#define DRV2605_REG_AUDIOOUTMAX 	0x15 // Audio-to-vibe maximum output drive register
#define DRV2605_REG_RATEDV 			0x16 // Rated voltage register
#define DRV2605_REG_CLAMPV 			0x17 // Overdrive clamp voltage register
#define DRV2605_REG_AUTOCALCOMP		0x18 // Auto-calibration compensation result register
#define DRV2605_REG_AUTOCALEMP		0x19 // Auto-calibration back-EMF result register
#define DRV2605_REG_FEEDBACK 		0x1A // Feedback control register
#define DRV2605_REG_CONTROL1 		0x1B // Control1 Register
#define DRV2605_REG_CONTROL2 		0x1C // Control2 Register
#define DRV2605_REG_CONTROL3 		0x1D // Control3 Register
#define DRV2605_REG_CONTROL4 		0x1E // Control4 Register
#define DRV2605_REG_VBAT 			0x21 // Vbat voltage-monitor register
#define DRV2605_REG_LRARESON 		0x22 // LRA resonance-period register

#define WAVEFORMS_MAX				127

#define FEEDBACK_CTRL_ERM_MASK		0x7F
#define FEEDBACK_CTRL_LRA_MASK		0x80
#define FEEDBACK_CTRL_MASK			0x08
//==============================================================================
//types:

/* result of used functions */
typedef enum
{
	DRV2605_ResultAccept,
	DRV2605_ResultError,
	DRV2605_ResultNullPointer,
	DRV2605_ResultBusy,
	DRV2605_ResultNotSupported

} DRV2605_Result;
//------------------------------------------------------------------------------

typedef enum
{
	DRV2605_RESET_STATE_OFF,
	DRV2605_RESET_STATE_ON
	
} DRV2605_RESET_STATE;
//------------------------------------------------------------------------------

typedef DRV2605_Result (*DRV2605_WriteAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
typedef DRV2605_Result (*DRV2605_ReadAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
//------------------------------------------------------------------------------

typedef struct 
{
	DRV2605_RESET_STATE (*Get)();
	void (*Set)(DRV2605_RESET_STATE state);
	
} DRV2605_ResetStateT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Handle;
	
	DRV2605_WriteAction Write;
	DRV2605_ReadAction Read;
	
} DRV2605_BusT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t Overcurrent : 1;
		uint8_t Overtemp : 1;
		uint8_t TimeOut : 1;
		uint8_t DiagnosticResult : 1;
		
		uint8_t Bit4 : 1;
		uint8_t Id : 3;
	};
		
	uint8_t Value;
	
} DRV2605_StatusT;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t DriverInit : 1;
		uint8_t BusInit : 1;
	};
	
	uint8_t Value;
	
} DRV2605_StateT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;
	
	DRV2605_BusT* Bus;
	DRV2605_ResetStateT* Reset;
	
	DRV2605_StateT State;
	DRV2605_StatusT Status;
	
} DRV2605_T;
//==============================================================================
//functions:

DRV2605_Result DRV2605_init(DRV2605_T* driver, void* parent, DRV2605_BusT* bus, DRV2605_ResetStateT* reset_control);

DRV2605_Result DRV2605_read_from_register(DRV2605_T* driver, uint8_t reg,
								uint8_t *bufp, uint16_t len);

DRV2605_Result DRV2605_write_to_register(DRV2605_T* driver, uint8_t reg,
                              uint8_t *bufp, uint16_t len);

DRV2605_Result DRV2605_update_status(DRV2605_T* driver);
DRV2605_Result DRV2605_motor_set_waveform(DRV2605_T* driver, uint8_t slot, uint8_t w);
DRV2605_Result DRV2605_select_library(DRV2605_T* driver, uint8_t lib);
DRV2605_Result DRV2605_motor_go(DRV2605_T* driver);
DRV2605_Result DRV2605_motor_stop(DRV2605_T* driver);
DRV2605_Result DRV2605_motor_set_mode(DRV2605_T* driver, uint8_t mode);
DRV2605_Result DRV2605_motor_set_realtimevalue(DRV2605_T* driver, uint8_t rtp);

// Select ERM (Eccentric Rotating Mass) or LRA (Linear Resonant Actuator)
// vibration motor The default is ERM, which is more common
DRV2605_Result DRV2605_motor_use_erm(DRV2605_T* driver);
DRV2605_Result DRV2605_use_lra(DRV2605_T* driver);

DRV2605_Result DRV2605_is_device_found(DRV2605_T* driver);
DRV2605_Result DRV2605_out_of_standby(DRV2605_T* driver);
DRV2605_Result DRV2605_no_realtime_playback(DRV2605_T* driver);
DRV2605_Result DRV2605_waveform_sequencer_reset(DRV2605_T* driver);
DRV2605_Result DRV2605_set_overdrive_time(DRV2605_T* driver, uint8_t val);
DRV2605_Result DRV2605_set_sustain_time_positive_offset(DRV2605_T* driver, uint8_t val);
DRV2605_Result DRV2605_set_sustain_time_negative_offset(DRV2605_T* driver, uint8_t val);
DRV2605_Result DRV2605_set_brake_time_offset(DRV2605_T* driver, uint8_t val);
DRV2605_Result DRV2605_set_audio_to_vibe_maxinput(DRV2605_T* driver, uint8_t val);
DRV2605_Result DRV2605_set_feedback_control_param(DRV2605_T* driver);
DRV2605_Result DRV2605_set_register_values(DRV2605_T* driver);
DRV2605_Result DRV2605_device_reset(DRV2605_T* driver);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
