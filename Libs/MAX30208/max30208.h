//==============================================================================
#ifndef MAX30208_H
#define MAX30208_H
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

#define MAX30208_ADDRESS 0xA0

#define MAX30208_SENSOR_RESOLUTION			0.005		//16-bit resolution
/* Interrupt */
#define MAX30208_INTERRUPT_STATUS        (0x00)
#define MAX30208_INTERRUPT_ENABLE        (0x01)
/* Interrupt mask for each bit */
#define MAX30208_INT_A_FULL_MASK 		(0x7F)
#define MAX30208_INT_A_FULL_ENABLE 		(0x80)
#define MAX30208_INT_A_FULL_DISABLE 	 	(0x00)
#define MAX30208_INT_DATA_RDY_MASK 		(0xFE)
#define MAX30208_INT_DATA_RDY_ENABLE 	(0x01)
#define MAX30208_INT_DATA_RDY_DISABLE 	(0x00)
#define MAX30208_INT_TEMP_HIGH_MASK 		(0xFD)
#define MAX30208_INT_TEMP_HIGH_ENABLE 	(0x02)
#define MAX30208_INT_TEMP_HIGH_DISABLE 	(0x00)
#define MAX30208_INT_TEMP_LOW_MASK 		(0xFB)
#define MAX30208_INT_TEMP_LOW_ENABLE 	(0x04)
#define MAX30208_INT_TEMP_LOW_DISABLE 	(0x00)
/* System */
#define MAX30208_SYSTEM_RESET         	(0x0C)
/* Temperature */
#define MAX30208_ALARM_HIGH_MSB          (0x10)
#define MAX30208_ALARM_HIGH_LSB          (0x11)
#define MAX30208_ALARM_LOW_MSB           (0x12)
#define MAX30208_ALARM_LOW_LSB           (0x13)
#define MAX30208_TEMP_SENSOR_SETUP       (0x14)
/* Temperature sensor setup mask*/
#define MAX30208_TEMP_SETUP_CONV_MASK	(0x01)
/* GPIO */
#define MAX30208_GPIO_SETUP_REG          (0x20)
#define MAX30208_GPIO_CONTROL_REG        (0x21)
/* Identifiers */
#define MAX30208_PART_IDENTIFIER         (0xFF)
#define MAX30208_PART_IDENTIFIER_VAL     (0x30)
/* FIFO */
#define MAX30208_FIFO_WRITE_POINTER      (0x04)
#define MAX30208_FIFO_READ_POINTER       (0x05)
#define MAX30208_FIFO_OVERFLOW_COUNTER   (0x06)
#define MAX30208_FIFO_DATA_COUNTER       (0x07)
#define MAX30208_FIFO_DATA               (0x08)
#define MAX30208_FIFO_CONFIGURATION1     (0x09)
#define MAX30208_FIFO_CONFIGURATION2     (0x0A)
/* FIFO configuration 2 masks */
#define MAX30208_ROLLOVER_MASK 			(0xFD)
#define MAX30208_ROLLOVER_ENABLE  		(0x02)
#define MAX30208_ROLLOVER_DISABLE  		(0x00)
#define MAX30208_AFULLTYPE_MASK  		(0xFB)
#define MAX30208_AFULLTYPE_ENABLE  		(0x04)
#define MAX30208_AFULLTYPE_DISABLE  		(0x00)
#define MAX30208_FIFO_STAT_CLR_MASK  	(0xF7)
#define MAX30208_FIFO_STAT_CLR_ENABLE  	(0x08)
#define MAX30208_FIFO_STAT_CLR_DISABLE  	(0x00)
#define MAX30208_FLUSH_FIFO_MASK  		(0xEF)
#define MAX30208_FLUSH_FIFO_ENABLE  		(0x10)
#define MAX30208_FLUSH_FIFO_DISABLE  	(0x00)
/* FIFO configuration 1 masks */
#define MAX30208_FIFO_AFULL_MASK  		(0xE0)
#define MAX30208_FIFO_AFULL_ENABLE  		(0x1F)
#define MAX30208_FIFO_AFULL_DISABLE  	(0x00)
//==============================================================================
//types:

/* result of used functions */
typedef enum
{
	MAX30208_ResultAccept,
	MAX30208_ResultError,
	MAX30208_ResultNullPointer,
	MAX30208_ResultBusy,
	MAX30208_ResultNotSupported

} MAX30208_Result;
//------------------------------------------------------------------------------

typedef enum
{
	MAX30208_GPIO_MODE_00 = 0,
	MAX30208_GPIO_MODE_01 = 1,
	MAX30208_GPIO_MODE_10 = 2,
	MAX30208_GPIO_MODE_11 = 3
	
} MAX30208_GPIO_MODE;
//------------------------------------------------------------------------------

typedef MAX30208_Result (*MAX30208_WriteAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
typedef MAX30208_Result (*MAX30208_ReadAction)(void* driver, uint8_t address, uint8_t reg, uint8_t* data, uint16_t data_size);
//------------------------------------------------------------------------------

typedef struct 
{
	MAX30208_GPIO_MODE (*Get)();
	void (*Set)(MAX30208_GPIO_MODE state);
	
} DRV2605_ModeControlT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Handle;
	
	MAX30208_WriteAction Write;
	MAX30208_ReadAction Read;
	
} MAX30208_BusT;
//------------------------------------------------------------------------------

typedef struct
{
	float temp_high_limit;
	float temp_low_limit;
	uint32_t read_interval;

} MAX30208_config;
//------------------------------------------------------------------------------

typedef union
{
	struct
	{
		uint8_t DriverInit : 1;
		uint8_t BusInit : 1;
	};
	
	uint8_t Value;
	
} MAX30208_StateT;
//------------------------------------------------------------------------------

typedef struct
{
	void* Description;
	void* Parent;
	
	MAX30208_BusT* Bus;
	DRV2605_ModeControlT* Mode;
	
	MAX30208_StateT State;
	
} MAX30208_T;
//==============================================================================
//functions:

MAX30208_Result MAX30208_init(MAX30208_T* driver,
	void* parent,
	MAX30208_BusT* bus,
	DRV2605_ModeControlT* mode_control);

MAX30208_Result MAX30208_write_reg(MAX30208_T* driver, uint8_t reg, uint8_t* data, uint16_t data_size);
MAX30208_Result MAX30208_read_reg(MAX30208_T* driver, uint8_t reg, uint8_t* data, uint16_t data_size);
	
MAX30208_Result MAX30208_dev_id_get(MAX30208_T* driver, uint8_t *buff);
bool MAX30208_is_device_found(MAX30208_T* driver);
MAX30208_Result MAX30208_system_reset(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_AFULL_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_disable_AFULL_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_DATARDY_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_disable_DATARDY_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_TEMPHIGH_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_disable_TEMPHIGH_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_TEMPLOW_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_disable_TEMPLOW_INT(MAX30208_T* driver);
MAX30208_Result MAX30208_set_high_temp(MAX30208_T* driver, float high);
MAX30208_Result MAX30208_set_low_temp(MAX30208_T* driver, float low);
MAX30208_Result MAX30208_start_conversion(MAX30208_T* driver);
MAX30208_Result MAX30208_conversion_is_complite(MAX30208_T* driver);
MAX30208_Result MAX30208_raw_data_get(MAX30208_T* driver, uint8_t *fifo_data);
float MAX30208_get_celcius(uint8_t *fifo_data);
MAX30208_Result MAX30208_read_status_reg(MAX30208_T* driver, uint8_t *status_val);
MAX30208_Result MAX30208_set_watermark(MAX30208_T* driver, uint8_t numberOfSamples);
MAX30208_Result MAX30208_enable_FIFO_Rollover(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_AFULL_TYPE(MAX30208_T* driver);
MAX30208_Result MAX30208_enable_FIFO_STAT_CLR(MAX30208_T* driver);
MAX30208_Result MAX30208_FLUSH_FIFO(MAX30208_T* driver);
MAX30208_Result MAX30208_FIFO_get_write_pointer(MAX30208_T* driver, uint8_t *int_val);
MAX30208_Result MAX30208_FIFO_get_read_pointer(MAX30208_T* driver, uint8_t *int_val);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
