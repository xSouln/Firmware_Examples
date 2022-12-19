/*
 * @file	drv2605.c
 * @brief	Haptic Motor driver file
 * @date	April 19, 2021
 * @author	eInfochips
 *
 */
#include "drv2605.h"

/*
 * @brief  Write generic device register
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 * @retval			 HAL_OK indicates successful write
 *
 */
DRV2605_Result DRV2605_write_to_register(DRV2605_T* driver, uint8_t reg, uint8_t *data, uint16_t len)
{
	DRV2605_Result result = driver->Bus->Read(driver, DRV2605_ADDR, reg, data, len);
	
	return result;
}

/*
 * @brief  Write generic device register
 *
 * @param  ctx   read / write interface definitions(ptr)
 * @param  reg   register to write
 * @param  data  pointer to data to write in register reg(ptr)
 * @param  len   number of consecutive register to write
 * @retval       interface status (MANDATORY: return 0 or HAL_OK -> no Error)
 *
 */
static DRV2605_Result DRV2605_write_reg(DRV2605_T* driver, uint8_t reg, uint8_t *data, uint16_t len)
{
	DRV2605_Result result = driver->Bus->Write(driver, DRV2605_ADDR, reg, data, len);
	
	return result;
}

/*
 * @brief  Read generic device register
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *@retval			 HAL_OK indicates successful write
 *@retval
 */
DRV2605_Result DRV2605_read_from_register(DRV2605_T* driver, uint8_t reg, uint8_t *data, uint16_t len)
{
	DRV2605_Result result = driver->Bus->Read(driver, DRV2605_ADDR, reg, data, len);
	
	return result;
}

/*
 * @brief  Read generic device register
 *
 * @param  ctx   read / write interface definitions(ptr)
 * @param  reg   register to read
 * @param  data  pointer to buffer that store the data read(ptr)
 * @param  len   number of consecutive register to read
 * @retval       interface status (MANDATORY: return 0 or HAL_OK -> no Error)
 *
 */
static DRV2605_Result DRV2605_read_reg(DRV2605_T* driver, uint8_t reg, uint8_t *data, uint16_t len)
{
	DRV2605_Result result = driver->Bus->Read(driver, DRV2605_ADDR, reg, data, len);
	
	return result;
}

/**
 * @brief initializes the driver
 * @param driver 
 * @param parent binding to the parent object
 * @param bus i2c control layer
 * @param reset_control reset control layer
 * @return DRV2605_Result 
 */
DRV2605_Result DRV2605_init(DRV2605_T* driver,
								void* parent,
								DRV2605_BusT* bus,
								DRV2605_ResetStateT* reset_control)
{
	if (!driver)
	{
		return DRV2605_ResultNullPointer;
	}
	
	if (!bus || !bus->Write || !bus->Read)
	{
		return DRV2605_ResultError;
	}
	
	if (!reset_control || !reset_control->Get || !reset_control->Set)
	{
		return DRV2605_ResultError;
	}
	
	driver->Description = "DRV2605_T";
	driver->Parent = parent;
	driver->Bus = bus;
	driver->Reset = reset_control;
	
	driver->State.BusInit = 1;
	driver->State.DriverInit = 1;
	
	return DRV2605_ResultAccept;
}

/*
 * @brief 	Select the haptic waveform to use.
 *
 * @param 	slot 	The waveform slot to set, from 0 to 7
 * @param 	w 		The waveform sequence value, refers to an index in the ROM library.
 *     				Playback starts at slot 0 and continues through to slot 7, stopping if it
 *     				encounters a value of 0. A list of available waveforms can be found in
 *     				section 11.2 of the datasheet
 *
 */
DRV2605_Result DRV2605_set_waveform(DRV2605_T* driver, uint8_t slot, uint8_t w)
{
	return DRV2605_write_reg(driver, DRV2605_REG_WAVESEQ1 + slot, &w, 1);
}


/*
 * @brief 	Select the waveform library to use.
 *
 * @param 	lib 	Library to use, 0 = Empty, 1-5 are ERM, 6 is LRA
 * 					See section 7.6.4 in the datasheet for more details:
 *
 */
DRV2605_Result DRV2605_select_library(DRV2605_T* driver, uint8_t lib)
{
	return DRV2605_write_reg(driver, DRV2605_REG_LIBRARY, &lib, 1);
}


/*
 * @brief 	Start playback of the waveforms (start moving!).
 *
 */
DRV2605_Result DRV2605_go(DRV2605_T* driver)
{
	uint8_t val = 0x01;
	return DRV2605_write_reg(driver, DRV2605_REG_GO, &val, 1);
}


/*
 * @brief 	Stop playback.
 *
 */
DRV2605_Result DRV2605_stop(DRV2605_T* driver)
{
	return DRV2605_write_reg(driver, DRV2605_REG_GO, 0, 1);
}


/*
 * @brief 	Set the device mode.
 *
 * @param 	mode 	Mode value, see datasheet section 7.6.2:
 * 					0: Internal trigger, call go() to start playback
 * 					1: External trigger, rising edge on IN pin starts playback (edge triggered)
 * 					2: External trigger, playback follows the state of IN pin (level triggered)
 * 					3: PWM/analog input
 * 					4: Audio to vibe
 * 					5: Real-time playback
 * 					6: Diagnostics - result stored in diagnostic bit in register 0x00
 * 					7: Auto calibration
 *
 */
DRV2605_Result DRV2605_set_mode(DRV2605_T* driver, uint8_t mode)
{
	return DRV2605_write_reg(driver, DRV2605_REG_MODE, &mode, 1);
}


/*
 * @brief 	Set the realtime value when in RTP mode, used to directly drive the haptic motor.
 *
 * @param 	rtp 	8-bit drive value.
 *
 */
DRV2605_Result DRV2605_set_realtimevalue(DRV2605_T* driver, uint8_t rtp)
{
	return DRV2605_write_reg(driver, DRV2605_REG_RTPIN, &rtp, 1);
}


/*
 * @brief 	Use ERM (Eccentric Rotating Mass) mode.
 *
 */
DRV2605_Result DRV2605_use_erm(DRV2605_T* driver)
{
	uint8_t val;
	DRV2605_Result result = DRV2605_read_reg(driver, DRV2605_REG_FEEDBACK, &val, 1);
	if (result != DRV2605_ResultAccept) { return result; }
	
	val = val & FEEDBACK_CTRL_ERM_MASK;
	return DRV2605_write_reg(driver, DRV2605_REG_FEEDBACK, &val, 1);
}


/*
 * @brief 	Use LRA (Linear Resonance Actuator) mode.
 *
 * @retval  HAL_OK for success
 */
DRV2605_Result DRV2605_use_lra(DRV2605_T* driver)
{
	uint8_t val;
	DRV2605_Result result = DRV2605_read_reg(driver, DRV2605_REG_FEEDBACK, &val, 1);
	if (result != DRV2605_ResultAccept) { return result; }
	
	val = val | FEEDBACK_CTRL_LRA_MASK;
	return DRV2605_write_reg(driver, DRV2605_REG_FEEDBACK, &val, 1);
}


/*
 * @brief 	check the status register value
 *
 * @retval	HAL_OK indicates success
 */
DRV2605_Result DRV2605_is_device_found(DRV2605_T* driver)
{
	uint8_t device_id;
	DRV2605_Result result = DRV2605_read_reg(driver,DRV2605_REG_STATUS, &device_id, 1);
	if (result != DRV2605_ResultAccept) { return result; }
	
	if(device_id != DRV2605_ID) { return DRV2605_ResultError; }
	
	return DRV2605_device_reset(driver);
}

/*
 * @brief	check if target device is ready for communication
 *
 * @retval	HAL_OK for successful communication
 */
DRV2605_Result DRV2605_update_status(DRV2605_T* driver)
{
	return DRV2605_read_reg(driver, DRV2605_REG_STATUS, &driver->Status.Value, 1);
}

/*
 * @brief	remove device from standby mode
 *
 * @retval  HAL_OK for successful communication
 *
 */
DRV2605_Result DRV2605_out_of_standby(DRV2605_T* driver)
{
	uint8_t val = 0x00;
	return DRV2605_write_reg(driver, DRV2605_REG_MODE, &val, 1);
}

/*
 * @brief 	set real-time playback input register value
 *
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_no_realtime_playback(DRV2605_T* driver)
{
	uint8_t val = 0x00;
	return DRV2605_write_reg(driver, DRV2605_REG_RTPIN, &val, 1);
}

/*
 * @brief 	reset waveform sequencer value
 *
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_waveform_sequencer_reset(DRV2605_T* driver)
{
	uint8_t click = 0x01;
	uint8_t wave_seq2 = 0x00;
	DRV2605_Result result = DRV2605_write_reg(driver, DRV2605_REG_WAVESEQ1, &click, 1); // strong click
	if (result != DRV2605_ResultAccept) { return result; }
	
	return DRV2605_write_reg(driver, DRV2605_REG_WAVESEQ2, &wave_seq2, 1); // end sequence
}

/*
 * @brief	set overdrive time value
 *
 * @param	val		value to be set
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_overdrive_time(DRV2605_T* driver, uint8_t val)
{
	return DRV2605_write_reg(driver, DRV2605_REG_OVERDRIVE, &val, 1);
}

/*
 * @brief	set brake time positive offset value
 *
 * @param	val		value to be set
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_sustain_time_positive_offset(DRV2605_T* driver, uint8_t val)
{
	return DRV2605_write_reg(driver, DRV2605_REG_SUSTAINPOS, &val, 1);
}

/*
 * @brief	set sustain time negative offset value
 *
 * @param	val		value to be set
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_sustain_time_negative_offset(DRV2605_T* driver, uint8_t val)
{
	return DRV2605_write_reg(driver, DRV2605_REG_SUSTAINNEG, &val, 1);
}

/*
 * @brief	set brake time offset value
 *
 * @param	val		value to be set
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_brake_time_offset(DRV2605_T* driver, uint8_t val)
{
	return DRV2605_write_reg(driver, DRV2605_REG_BREAK, &val, 1);
}

/*
 * @brief	set Audio-to-vibe register value
 *
 * @param	val		value to be set
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_audio_to_vibe_maxinput(DRV2605_T* driver, uint8_t val)
{
	return 	DRV2605_write_reg(driver, DRV2605_REG_AUDIOMAX, &val, 1);
}

/*
 * @brief	set feedback control register value
 *
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_feedback_control_param(DRV2605_T* driver)
{
	uint8_t val;
	DRV2605_Result result = DRV2605_read_reg(driver,DRV2605_REG_FEEDBACK, &val, 1);
	if (result != DRV2605_ResultAccept) { return result; }
	
	val = val | FEEDBACK_CTRL_MASK;
	return DRV2605_write_reg(driver, DRV2605_REG_FEEDBACK, &val, 1);
}

/*
 * @brief	write default value to registers
 *
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_set_register_values(DRV2605_T* driver)
{
	DRV2605_Result result = DRV2605_no_realtime_playback(driver);	//no real time playback
	if (result != DRV2605_ResultAccept) { return result; }

	result = DRV2605_waveform_sequencer_reset(driver);
	if (result != DRV2605_ResultAccept) { return result; }

	result = DRV2605_set_overdrive_time(driver, 0);	//no overdrive
	if (result != DRV2605_ResultAccept) { return result; }

	result = DRV2605_set_sustain_time_positive_offset(driver, 0);
	if (result != DRV2605_ResultAccept) { return result; }

	result = DRV2605_set_sustain_time_negative_offset(driver, 0);
	if (result != DRV2605_ResultAccept) { return result; }
	
	result = DRV2605_set_brake_time_offset(driver, 0);
	if (result != DRV2605_ResultAccept) { return result; }

	return DRV2605_set_audio_to_vibe_maxinput(driver, 0x64);
}

/*
 * @brief	set Mode register bit for device reset
 *
 * @retval  HAL_OK for successful communication
 */
DRV2605_Result DRV2605_device_reset(DRV2605_T* driver)
{
	uint8_t val;
	DRV2605_Result result = DRV2605_read_reg(driver,DRV2605_REG_MODE, &val, 1);
	if (result != DRV2605_ResultAccept) { return result; }
	
	val |= 1 << 7;		//set 7th bit
	return DRV2605_write_reg(driver, DRV2605_REG_MODE, &val, 1);
}
