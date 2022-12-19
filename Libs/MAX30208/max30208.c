/*
 * @file	MAX30208.c
 * @brief	Body temperature sensor driver file
 * @date	May 19, 2021
 * @author	eInfochips
 *
 */

#include "max30208.h"
//==============================================================================
/*
 * @brief  	set bit
 * @param	data	buffer that contains data.(ptr)
 * @param	pos		which bit to set
 */
static void set_bit(uint8_t *data, uint8_t pos){
	*data = *data | (1 << pos);
}

/*
 * @brief  	clear bit
 * @param	data	buffer that contains data.(ptr)
 * @param	pos		which bit to clear
 */
void clear_bit(uint8_t *data, uint8_t pos){
	*data = *data & (~(1 << pos));
}

//==============================================================================
MAX30208_Result MAX30208_write_reg(MAX30208_T* driver, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	MAX30208_Result result = driver->Bus->Write(driver,
		MAX30208_ADDRESS | (driver->Mode->Get() << 1),
		reg,
		data,
		data_size);
	
	return result;
}
//==============================================================================
MAX30208_Result MAX30208_read_reg(MAX30208_T* driver, uint8_t reg, uint8_t* data, uint16_t data_size)
{
	MAX30208_Result result = driver->Bus->Read(driver,
		MAX30208_ADDRESS | (driver->Mode->Get() << 1),
		reg,
		data,
		data_size);
	
	return result;
}
//==============================================================================
MAX30208_Result MAX30208_init(MAX30208_T* driver,
								void* parent,
								MAX30208_BusT* bus,
								DRV2605_ModeControlT* mode_control)
{
	if (!driver || !bus || !bus->Read || !bus->Write)
	{
		return MAX30208_ResultError;
	}
	
	if (!mode_control || !mode_control->Get || !mode_control->Set)
	{
		return MAX30208_ResultError;
	}
	
	driver->Description = "MAX30208_T";
	driver->Parent = parent;
	driver->Bus = bus;
	driver->Mode = mode_control;
	
	driver->State.BusInit = 1;
	driver->State.DriverInit = 1;
	
	return MAX30208_ResultAccept;
}
//==============================================================================
/*
 * @brief  	mask particular bit and change content of desired bit
 * @param	reg		register to which masking is to be performed
 * @param	mask	mask value
 * @param	thing	which bit to change
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_bit_mask(MAX30208_T* driver, uint8_t reg, uint8_t mask, uint8_t thing){
  // Grab current register context
  uint8_t originalContents;
  MAX30208_Result ret = MAX30208_write_reg(driver, reg, &originalContents, 1);
  uint8_t val;
  if(ret == MAX30208_ResultAccept){
	  // Zero-out the portions of the register we're interested in
	  originalContents = originalContents & mask;

	  // Change contents
	  val = originalContents | thing;
	  ret = MAX30208_write_reg(driver, reg, &val, 1);
  }
  return ret;
}

/*
 * @brief  	get device id
 * @param	buff	buffer that contains data.(ptr)
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_dev_id_get(MAX30208_T* driver, uint8_t *buff){
	MAX30208_Result ret;
	ret = MAX30208_read_reg(driver, MAX30208_PART_IDENTIFIER, buff, 1);
	return ret;
}

/*
 * @brief  	match device id
 * @retval	1 if device is found
 */
bool MAX30208_is_device_found(MAX30208_T* driver){
	
	return false;
}

/*
 * @brief  	perform system reset
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_system_reset(MAX30208_T* driver){
	uint8_t val = 0;
	set_bit(&val, 0);
	MAX30208_Result int_ack;
	int_ack = MAX30208_write_reg(driver,
			MAX30208_SYSTEM_RESET,
			&val,
			1);
	if(int_ack != MAX30208_ResultAccept){
		return MAX30208_ResultError;
	}
	return MAX30208_ResultAccept;
}

/*
 * @brief  	enable AFULL interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_AFULL_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_A_FULL_MASK,
		  MAX30208_INT_A_FULL_ENABLE);
	return ret;
}

/*
 * @brief  	disable AFULL interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_disable_AFULL_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_A_FULL_MASK,
		  MAX30208_INT_A_FULL_DISABLE);
	return ret;
}

/*
 * @brief  	enable data ready interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_DATARDY_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_DATA_RDY_MASK,
		  MAX30208_INT_DATA_RDY_ENABLE);
	return ret;
}

/*
 * @brief  	disable data ready interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_disable_DATARDY_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_DATA_RDY_MASK,
		  MAX30208_INT_DATA_RDY_DISABLE);
	return ret;
}

/*
 * @brief  	enable high temperature interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_TEMPHIGH_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_TEMP_HIGH_MASK,
		  MAX30208_INT_TEMP_HIGH_ENABLE);
	return ret;
}

/*
 * @brief  	disable high temperature interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_disable_TEMPHIGH_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_TEMP_HIGH_MASK,
		  MAX30208_INT_TEMP_HIGH_DISABLE);
	return ret;
}

/*
 * @brief  	enable low temperature interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_TEMPLOW_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_TEMP_LOW_MASK,
		  MAX30208_INT_TEMP_LOW_ENABLE);
	return ret;
}

/*
 * @brief  	disable low temperature interrupt
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_disable_TEMPLOW_INT(MAX30208_T* driver) {
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_INTERRUPT_ENABLE,
			MAX30208_INT_TEMP_LOW_MASK,
		  MAX30208_INT_TEMP_LOW_DISABLE);
	return ret;
}

/*
 * @brief  	set high temperature value
 * @param	high	value to be set
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_set_high_temp(MAX30208_T* driver, float high){
	MAX30208_Result ret;
	high = high/MAX30208_SENSOR_RESOLUTION;
	uint8_t msb = (uint32_t)high >> 8;
	uint8_t lsb = (uint32_t)high & 0xFF;
	ret = MAX30208_write_reg(driver, MAX30208_ALARM_HIGH_MSB, &msb, 1);
	if(ret == MAX30208_ResultAccept){
		ret = MAX30208_write_reg(driver, MAX30208_ALARM_HIGH_LSB, &lsb, 1);
	}
	return ret;
}

/*
 * @brief  	set low temperature value
 * @param	low		value to be set
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_set_low_temp(MAX30208_T* driver, float low){
	MAX30208_Result ret;
	low = low/MAX30208_SENSOR_RESOLUTION;
	uint8_t msb = (uint32_t)low >> 8;
	uint8_t lsb = (uint32_t)low & 0xFF;
	ret = MAX30208_write_reg(driver, MAX30208_ALARM_LOW_MSB, &msb, 1);
	if(ret == MAX30208_ResultAccept){
		ret = MAX30208_write_reg(driver, MAX30208_ALARM_LOW_LSB, &lsb, 1);
	}
	return ret;
}

/*
 * @brief	Starts temperature conversion
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_start_conversion(MAX30208_T* driver)
{
	MAX30208_Result tempsetup_ack;
	uint8_t temp_config;
	tempsetup_ack = MAX30208_read_reg(driver, MAX30208_TEMP_SENSOR_SETUP, &temp_config, 1);
	set_bit(&temp_config, 0);
	if(tempsetup_ack == MAX30208_ResultAccept){
		tempsetup_ack = MAX30208_write_reg(driver, MAX30208_TEMP_SENSOR_SETUP, &temp_config, 1);
	}
	return tempsetup_ack;
}


MAX30208_Result MAX30208_conversion_is_complite(MAX30208_T* driver)
{
	uint8_t temp_config;
	if (MAX30208_read_reg(driver, MAX30208_TEMP_SENSOR_SETUP, &temp_config, 1) == MAX30208_ResultAccept
	&& (temp_config & MAX30208_TEMP_SETUP_CONV_MASK) == 0)
	{
		return MAX30208_ResultAccept;
	}

	return MAX30208_ResultError;
}

/*
 * @brief  	Get raw temperature value
 * @param	fifo_data	Buffer that contains raw data.(ptr)
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_raw_data_get(MAX30208_T* driver, uint8_t *fifo_data){
	MAX30208_Result ret;
	ret = MAX30208_read_reg(driver, MAX30208_FIFO_DATA, fifo_data, 2);
	return ret;
}

/*
 * @brief  	Convert raw temperature value to celcius
 * @param	fifo_data	Buffer that contains raw data.(ptr)
 * @retval	temperature value in celcius
 */
float MAX30208_get_celcius(uint8_t *fifo_data){
	return (((uint16_t)fifo_data[0] << 8) | fifo_data[1]) * MAX30208_SENSOR_RESOLUTION;
}

/*
 * @brief  	Read interrupt status register value
 * @param	status_val	Buffer that stores the data read.(ptr)
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_read_status_reg(MAX30208_T* driver, uint8_t *status_val){
	MAX30208_Result ret;
	ret = MAX30208_read_reg(driver, MAX30208_INTERRUPT_STATUS, status_val, 1);
	return ret;
}

/*
 * @brief  	Set watermark for FIFO
 * 			The A_FULL bit is set when the FIFO contains (32 - FIFO_A_FULL[4:0]) words.
 * 			Example: when FIFO_A_FULL is set to 2, the flag is set when the 30th word is written to the FIFO.
 *
 * @param	numberOfSamples	samples to trigger AFULL condition
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_set_watermark(MAX30208_T* driver, uint8_t numberOfSamples){
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_FIFO_CONFIGURATION1,
			MAX30208_FIFO_AFULL_MASK,
			numberOfSamples);
	return ret;
}

/*
 * @brief  	[FIFO_RO]
 * 			LOW:	The new sample is discarded and the FIFO is not updated.
 * 			HIGH: 	When FIFO is full, old samples are overwritten.
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_FIFO_Rollover(MAX30208_T* driver){
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_FIFO_CONFIGURATION2,
			MAX30208_ROLLOVER_MASK,
			MAX30208_ROLLOVER_ENABLE);
	return ret;
}

/*
 * @brief  	[A_FULL_TYPE]
 * 			LOW:	A_FULL status bit is asserted when the A_FULL condition	is
 * 					detected and cleared by a STATUS register read, then reasserts
 * 					for every sample if the A_FULL condition persists.
 * 			HIGH: 	A_FULL status bit is asserted only when a new A_FULL condition is
 * 					detected. The status bit is cleared by a STATUS register read
 * 					and does not reassert for every sample until a new A_FULL condition is detected.
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_AFULL_TYPE(MAX30208_T* driver){
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_FIFO_CONFIGURATION2,
			MAX30208_AFULLTYPE_MASK,
			MAX30208_AFULLTYPE_ENABLE);
	return ret;
}

/*
 * @brief  	[FIFO_STAT_CLR]
 * 			LOW:	A_FULL and TEMP_RDY status bits are not cleared by a FIFO_DATA
 * 					register read but are cleared by STATUS register read.
 * 			HIGH: 	A_FULL and TEMP_RDY status bits are cleared by a FIFO_DATA
 * 					register read or a STATUS register read
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_enable_FIFO_STAT_CLR(MAX30208_T* driver){
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_FIFO_CONFIGURATION2,
			MAX30208_FIFO_STAT_CLR_MASK,
			MAX30208_FIFO_STAT_CLR_ENABLE);
	return ret;
}

/*
 * @brief  	The FIFO becomes empty and the FIFO_WR_PTR[4:0], FIFO_RD_PTR[4:0],
 * 			FIFO_DATA_COUNT[5:0] and OVF_COUNTER[4:0] are reset to zero.
 * 			FLUSH_FIFO is a self-clearing bit.
 *
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_FLUSH_FIFO(MAX30208_T* driver){
	MAX30208_Result ret = MAX30208_bit_mask(driver,
			MAX30208_FIFO_CONFIGURATION2,
			MAX30208_FLUSH_FIFO_MASK,
			MAX30208_FLUSH_FIFO_ENABLE);
	return ret;
}

/*
 * @brief  	Read FIFO write pointer value
 * @param	val 	Buffer that stores the data read.(ptr)
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_FIFO_get_write_pointer(MAX30208_T* driver, uint8_t *int_val){
	MAX30208_Result ret;
	ret = MAX30208_read_reg(driver, MAX30208_FIFO_WRITE_POINTER, int_val, 1);
	return ret;
}

/*
 * @brief  	Read FIFO read pointer value
 * @param	val 	Buffer that stores the data read.(ptr)
 * @retval	MAX30208_ResultAccept for successful communication
 */
MAX30208_Result MAX30208_FIFO_get_read_pointer(MAX30208_T* driver, uint8_t *val){
	MAX30208_Result ret;
	ret = MAX30208_read_reg(driver, MAX30208_FIFO_READ_POINTER, val, 1);
	return ret;
}
