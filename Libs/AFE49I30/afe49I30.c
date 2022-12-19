/*
 * AFE49I30.c
 *
 *  Created on: Apr 15, 2021
 *      Author: Einfochips
 */

#include "afe49I30.h"
#include <string.h>

/*
 * @brief  AFE49I30 Driver Init
 *
 */
AFE49I30_Result AFE49I30_Init(AFE49I30_T* driver,
									void* parent,
									AFE49I30_BusT* bus,
									AFE49I30_InterfaceT* interface)
{
	if (!driver)
	{
		return AFE49I30_ResultNullPointer;
	}
	
	if (!bus || !bus->Write || !bus->Read)
	{
		return AFE49I30_ResultError;
	}
	
	if (!interface)
	{
		return AFE49I30_ResultError;
	}
	
	driver->Description = "AFE49I30_T";
	driver->Parent = parent;
	driver->Bus = bus;
	driver->Interface = interface;
	
	driver->State.BusInit = 1;
	driver->State.DriverInit = 1;
	
	return AFE49I30_ResultAccept;
}

/*
 * @brief  AFE49I30 Power Down
 *
 */
void AFE49I30_PowerDown(AFE49I30_T* driver)
{
    //HAL_GPIO_WritePin(GPIOC, AFE_RESET_N_Pin, GPIO_PIN_RESET);
	driver->Interface->SetValue(driver, AFE49I30_ValueResetState, AFE49I30_RESET_STATE_ON);
	driver->Interface->RequestListener(driver, AFE49I30_RequestDelay, 5, 0);
}

/*
 * @brief  AFE49I30 HW Reset
 *
 */
void AFE49I30_Reset(AFE49I30_T* driver)
{
	driver->Interface->SetValue(driver, AFE49I30_ValueResetState, AFE49I30_RESET_STATE_ON);
	driver->Interface->RequestListener(driver, AFE49I30_RequestDelay, 10, 0);
    driver->Interface->SetValue(driver, AFE49I30_ValueResetState, AFE49I30_RESET_STATE_OFF);
    driver->Interface->RequestListener(driver, AFE49I30_RequestDelay, 200, 0);

    driver->Config.MeasureMode = AFE49I30_UNKNOWN_MODE;
    driver->Config.SleepMode = AFE49I30_UNKNOWN_SLEEP_MODE;
}

/**
 * register-wise read 3-bytes data from AFE
 */
AFE49I30_Result AFE49I30_GetRegValue(AFE49I30_T* driver, uint8_t address, uint32_t *data)
{
	driver->RxFifo.Value = 0;
	AFE49I30_Result ret = driver->Bus->Read(driver, AFE49I30_ADDR, address, driver->RxFifo.Data, 3);
	
	if (ret != AFE49I30_ResultAccept)
	{
		return ret;
	}

	uint32_t result = driver->RxFifo.Data[0];
	result <<= 8;
	result |= driver->RxFifo.Data[1];
	result <<= 8;
	result |= driver->RxFifo.Data[2];

	*data = result;

	return ret;
}

/********************************************************************************
 * Function        GetFIFOSampleCount
 * Description     Get FIFO Count
 * Parameters      fifoSample
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_SetRegValue(AFE49I30_T* driver, uint8_t reg, uint32_t data)
{
	driver->TxFifo.Value = 0;

	driver->TxFifo.Data[0] = ((data >> 16) & 0x000000FF);
	driver->TxFifo.Data[1] = ((data >> 8) & 0x000000FF);
	driver->TxFifo.Data[2] = (data & 0x000000FF);

	AFE49I30_Result result = driver->Bus->Write(driver, AFE49I30_ADDR, reg, driver->TxFifo.Data, 3);
	
	return result;
}

/********************************************************************************
 * Function        GetFIFOSampleCount
 * Description     Get FIFO Count
 * Parameters      fifoSample
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_GetFIFOSampleCount(AFE49I30_T* driver, uint8_t *fifoSample)
{
	uint32_t value;
	AFE49I30_Result ret = AFE49I30_GetRegValue(driver,  AFE49I30_FIFO_SAMPLE_COUNT_REG, &value);
	fifoSample[0] = (uint8_t)(value & 0xFF) + 1;
	//fifoSample = *fifoSample + 1;
	return ret;
}

/********************************************************************************
 * Function        StopFIFO
 * Description     Stop FIFO
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_StopFIFO(AFE49I30_T* driver)
{
	AFE49I30_Result ret = AFE49I30_ResultAccept;
	//Stop FIFO
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to Page 0
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_TIMER_ENGINE_REG, 0x000000); //Disable Timer Engine
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000002); //Disable FIFO
	return ret;
}

/********************************************************************************
 * Function        StartFIFO
 * Description     Start FIFO
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_StartFIFO(AFE49I30_T* driver)
{
	AFE49I30_Result ret = AFE49I30_ResultAccept;
	//Start FIFO
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to Page 0
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000040);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_TIMER_ENGINE_REG, 0xC000A0);
	return ret;
}

/********************************************************************************
 * Function        UpdateECGSamplingRate
 * Description     Update ECG Sampling Rate
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result UpdateECGSamplingRate(AFE49I30_T* driver)
{
	//Stop FIFO
	AFE49I30_Result ret = AFE49I30_StopFIFO(driver);

	//Set ECG Sampling Rate
	if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_400) {
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x000FF0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000000);
	} else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_200) {
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0007F0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000001);
	} else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_100) {
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0003F0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000002);
	} else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_50) {
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0001F0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000003);
	} else {
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0000F0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000004);
	}

	//Start FIFO
	ret = ret | AFE49I30_StartFIFO(driver);

	return ret;
}

/********************************************************************************
 * Function        GetCompOut
 * Description     Get comparator output
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result GetCompOut(AFE49I30_T* driver, bool *rcomp_inp_high, bool *rcomp_inp_low, bool *rcomp_inm_high, bool *rcomp_inm_low)
{
	uint32_t value;
	AFE49I30_Result ret = AFE49I30_GetRegValue(driver, 0x63, &value);
	
	if (ret == AFE49I30_ResultAccept)
	{
		*rcomp_inp_high = AFE49I30_CHECK_SPEC_BIT(value, 3);
		*rcomp_inp_low = AFE49I30_CHECK_SPEC_BIT(value, 2);
		*rcomp_inm_high = AFE49I30_CHECK_SPEC_BIT(value, 1);
		*rcomp_inm_low = AFE49I30_CHECK_SPEC_BIT(value, 0);
	}
	return ret;
}

/********************************************************************************
 * Function        EnableLeadOffDetection
 * Description     Configure Lead Off Detection
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_EnableLeadOffDetection(AFE49I30_T* driver)
{
	uint32_t value;
	AFE49I30_Result ret = AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to page 0

	ret = AFE49I30_GetRegValue(driver, 0x62, &value);
	if (ret == AFE49I30_ResultAccept)
	{
		AFE49I30_SET_SPEC_BIT(value, 21);
		AFE49I30_SET_SPEC_BIT(value, 22);
		ret = ret | AFE49I30_SetRegValue(driver, 0x62, value);
		if (ret == AFE49I30_ResultAccept) {
			ret = AFE49I30_GetRegValue(driver, 0x29, &value);
			AFE49I30_SET_SPEC_BIT(value, 19);
			ret = ret | AFE49I30_SetRegValue(driver, 0x29, value); //Enable GPIO2
		}
	}
	return ret;
}

/********************************************************************************
 * Function        DisableLeadOffDetection
 * Description     Disable Lead Off Detection
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_DisableLeadOffDetection(AFE49I30_T* driver)
{
	uint32_t value;
	AFE49I30_Result ret = AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to page 0

	ret = AFE49I30_GetRegValue(driver, 0x62, &value);
	if (ret == AFE49I30_ResultAccept)
	{
		AFE49I30_CLEAR_SPEC_BIT(value, 21);
		AFE49I30_CLEAR_SPEC_BIT(value, 22);
		ret = ret | AFE49I30_SetRegValue(driver, 0x62, value);
		if (0 == ret) {
			ret = AFE49I30_GetRegValue(driver, 0x29, &value);
			AFE49I30_CLEAR_SPEC_BIT(value, 19);
			ret = ret | AFE49I30_SetRegValue(driver, 0x29, value); //Disable GPIO2
		}
	}

	return ret;
}

/********************************************************************************
 * Function        DeInitAFE
 * Description     Deinit AFE
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_DeInit(AFE49I30_T* driver)
{
	int32_t ret = AFE49I30_StopFIFO(driver);
	AFE49I30_PowerDown(driver);
	if (ret == AFE49I30_ResultAccept)
		driver->Config.MeasureMode = AFE49I30_UNKNOWN_MODE;
	return ret;
}

/********************************************************************************
 * Function        ConfigureAFEInSleepMode
 * Description     Configure AFE Sleep Mode
 * Parameters      sleep_mode
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_ConfigureAFEInSleepMode(AFE49I30_T* driver, AFE49I30_SLEEP_MODE sleep_mode)
{
	uint32_t value;
	AFE49I30_Result ret = AFE49I30_GetRegValue(driver, 0x29, &value);
	if (ret == AFE49I30_ResultAccept) {
		if (sleep_mode == AFE49I30_DEEP_SLEEP_MODE) {
			AFE49I30_SET_SPEC_BIT(value, 17); //EN_ALWAYS_DEEP_SLEEP=1
			ret = ret | AFE49I30_SetRegValue(driver, 0x29, value);
		}
		else if (sleep_mode == AFE49I30_LIGHT_SLEEP_MODE) {
			AFE49I30_SET_SPEC_BIT(value, 20); //DIS_ACTIVE=1
			AFE49I30_SET_SPEC_BIT(value, 18); //DIS_DEEP_SLEEP=1
			ret = ret | AFE49I30_SetRegValue(driver, 0x29, value);
		}
		else {
			AFE49I30_SET_SPEC_BIT(value, 18); //DIS_DEEP_SLEEP=1
			AFE49I30_SET_SPEC_BIT(value, 16); //EN_ALWAYS_ACTIVE=1
			ret = ret | AFE49I30_SetRegValue(driver, 0x29, value);
		}
	}

	if (ret == AFE49I30_ResultAccept){
		driver->Config.SleepMode = sleep_mode;
	}
	return ret;
}

/********************************************************************************
 * Function        AFE49I30_SetSamplingRate
 * Description     Set ECG sampling rate
 * Parameters      None
 * Return Value    AFE49I30_Result
 * *****************************************************************************/
AFE49I30_Result AFE49I30_SetSamplingRate(AFE49I30_T* driver, AFE49I30_ECG_SAMPLE_RATE value)
{
	if (driver->Config.MeasureMode == AFE49I30_ECG_MODE)
	{
		driver->Options.SamplingRate = value;

		return UpdateECGSamplingRate(driver);
	}

	return AFE49I30_ResultError;
}
/********************************************************************************
 * Function        AFE49I30_ECG_Init
 * Description     Configure AFE for ECG and Start ECG
 * Parameters      None
 * Return Value    AFE49I30_Result
 * *****************************************************************************/
AFE49I30_Result AFE49I30_ECG_Init(AFE49I30_T* driver)
{
	AFE49I30_Result ret = AFE49I30_ResultAccept;

	AFE49I30_Reset(driver);

	/*Page 0 Register Setting */
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to Page 0

	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000008);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_TIMER_ENGINE_REG, 0x000000); //Disable Timer Engine
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000002); //Disable FIFO

	//Set ECG Sampling Rate
	if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_400)
	{
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x000FE0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000000);
		driver->Config.SamplingRate = AFE49I30_ECG_SAMPLE_400;
	}
	else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_200)
	{
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0007E0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000001);
		driver->Config.SamplingRate = AFE49I30_ECG_SAMPLE_200;
	}
	else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_100)
	{
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0003E0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000002);
		driver->Config.SamplingRate = AFE49I30_ECG_SAMPLE_100;
	}
	else if (driver->Options.SamplingRate == AFE49I30_ECG_SAMPLE_50)
	{
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0001E0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000003);
		driver->Config.SamplingRate = AFE49I30_ECG_SAMPLE_50;
	}
	else
	{
		driver->Options.SamplingRate = AFE49I30_ECG_SAMPLE_25;
		ret = ret | AFE49I30_SetRegValue(driver, 0x42, 0x0000E0);
		ret = ret | AFE49I30_SetRegValue(driver, 0x96, 0x000004);
		driver->Config.SamplingRate = AFE49I30_ECG_SAMPLE_25;
	}

	ret = ret | AFE49I30_SetRegValue(driver, 0x02,0x600000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x03,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x04,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x19,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x1A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x1E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x23,0x068000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x24,0x002000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x25,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x28,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x29,0x050000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x30,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x31,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x34,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x35,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x39,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x41,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x49,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4E,0xFEE004); //0xFE8004 //FEE004
	ret = ret | AFE49I30_SetRegValue(driver, 0x4F,0x01A004);//010004

	if (AFE49I30_ECG_GAIN_2_15 == driver->Options.Gain) {
		ret = ret | AFE49I30_SetRegValue(driver, 0x50, 0x040004);
	} else if (AFE49I30_ECG_GAIN_3_23 == driver->Options.Gain) {

		ret = ret | AFE49I30_SetRegValue(driver, 0x50, 0x000004);
	} else {
		//Gain 1.61
		ret = ret | AFE49I30_SetRegValue(driver, 0x50, 0x1C0004);
	}

	ret = ret | AFE49I30_SetRegValue(driver, 0x51,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x57,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x58,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x60,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x61,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x62,0x800000);//0xE00000 - Enable Leadoff, 0x800000 - Disable Leadoff
	ret = ret | AFE49I30_SetRegValue(driver, 0x63,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x72,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x73,0x047C3C);
	ret = ret | AFE49I30_SetRegValue(driver, 0x74,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x78,0x80E005);
	ret = ret | AFE49I30_SetRegValue(driver, 0x79,0x00E005);
	ret = ret | AFE49I30_SetRegValue(driver, 0x80,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x81,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x88,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8B,0x000100);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x90,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x91,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x92,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x93,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x94,0x000084);
	ret = ret | AFE49I30_SetRegValue(driver, 0x95,0x000050);
	ret = ret | AFE49I30_SetRegValue(driver, 0x97,0x000020);
	ret = ret | AFE49I30_SetRegValue(driver, 0xAC,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xAE,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xB4,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xB5,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x98,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x99,0x200355);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9C,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9D,0x000015);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA0,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA1,0x000015);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA2,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA4,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA5,0x000015);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA6,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE6,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE7,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE8,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xF5,0x000000);

	/*Page 1 Register Setting */
    //Setting of each phase (Phase1 to Phase24)
    /*
Reg1: IN_TIA2<20:23>,IN_TIA1<16:19>,LED_DRV2_TX<8:15>,LED_DRV1_TX<0:7>
Reg2: IOFFDAC_PH_TIA1<16:23>,USE_DIG_AACM_TIA1<15>,USE_ANA_AACM_TIA1<14>,UPDATE_BASELINE_TIA1<13>,CF_TIA1<10:12>,REG_LOOP_NUM_TIA1<8:9>,RF_TIA1<4:7>,REG_NUMAV<0:3>
Reg3: TIA_SEL<22:23>,PHASE_INT_GPIO<21>,0,CONFIG_PHASE_AS_ECG<19>,FIFO_DATA_CTRL<17:18>,THR_SEL_DATA_CTRL<15:16>,0,STAGGER_LED<12>,FILTER_SET_SEL<11>,0,REG_TWLED<0:7>
Reg4: IOFFDAC_TIA2<16:23>,USE_DIG_AACM_TIA2<15>,USE_ANA_AACM_TIA2<14>,UPDATE_BASELINE_TIA2<13>,CF_TIA2<10:12>,REG_LOOP_NUM_TIA2<8:9>,RF_TIA2<4:7>,0
Reg5: DIG_AACM_LED_TIA1<23>,DIG_AACM_LED_TIA2<22>,0,REG_MASK_FACTOR<16:19>,ILED_DRV2<8:15>,ILED_DRV1<0:7>
     */
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000001);
	/* Only one phase configured and that is for ECG.
	 * We are using Decimation Filter to reduce sampling rate
	 */
	ret = ret | AFE49I30_SetRegValue(driver, 0x20,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x21,0x000321);
	ret = ret | AFE49I30_SetRegValue(driver, 0x22,0x080002);
	ret = ret | AFE49I30_SetRegValue(driver, 0x23,0x0017B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x24,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x25,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x26,0x00A000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x27,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x28,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x29,0x800000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2A,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2B,0x004000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x30,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x31,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x32,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x33,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x34,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x35,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x36,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x37,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x38,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x39,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3E,0x040000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x40,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x41,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x42,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x43,0x040000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x44,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x45,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x46,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x47,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x48,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x49,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4D,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x50,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x51,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x52,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x53,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x54,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x55,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x56,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x57,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x58,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x59,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5C,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x60,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x61,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x62,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x63,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x64,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x65,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x66,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x67,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x68,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x69,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6B,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x70,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x71,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x72,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x73,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x74,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x75,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x76,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x77,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x78,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x79,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7A,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7F,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x80,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x81,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x82,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x83,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x84,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x85,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x86,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x87,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x88,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x89,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8E,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x90,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x91,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x92,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x93,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x94,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x95,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x96,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x97,0x000000);

	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000); //Switch to page 0
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000002);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000040);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_TIMER_ENGINE_REG, 0xC000A0);

	if (ret == AFE49I30_ResultAccept)
	{
		driver->Config.MeasureMode = AFE49I30_ECG_MODE;
		driver->Config.SleepMode = AFE49I30_ACTIVE_MODE;
	}

	return ret;
}

/********************************************************************************
 * Function        AFE49I30_PPG_Init
 * Description     Configure AFE for PPG and Start PPG
 * Parameters      None
 * Return Value    int32_t
 * *****************************************************************************/
AFE49I30_Result AFE49I30_PPG_Init(AFE49I30_T* driver)
{
	AFE49I30_Result ret = AFE49I30_ResultAccept;
	AFE49I30_Reset(driver);

	//Set Page 0 Registers
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000008);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000002);

	ret = ret | AFE49I30_SetRegValue(driver, 0x42,0x0003E0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x02,0x600000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x03,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x04,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x19,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x1A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x1E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x23,0x068000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x24,0x002000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x25,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x28,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x29,0x050000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x30,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x31,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x34,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x35,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x39,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x41,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x49,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4E,0xFE8000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4F,0x010000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x50,0x1C0000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x51,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x57,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x58,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x60,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x61,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x62,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x63,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x72,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x73,0x047C3C);
	ret = ret | AFE49I30_SetRegValue(driver, 0x74,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x78,0x80E005);
	ret = ret | AFE49I30_SetRegValue(driver, 0x79,0x00E00E);
	ret = ret | AFE49I30_SetRegValue(driver, 0x80,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x81,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x88,0x000007);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8B,0x000100);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x90,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x91,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x92,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x93,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x94,0x000084);
	ret = ret | AFE49I30_SetRegValue(driver, 0x95,0x000050);
	ret = ret | AFE49I30_SetRegValue(driver, 0x96,0x004444);
	ret = ret | AFE49I30_SetRegValue(driver, 0x97,0x8E2860);
	ret = ret | AFE49I30_SetRegValue(driver, 0xAC,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xAE,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xB4,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xB5,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x98,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x99,0x2002AB);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9C,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9D,0x0002AB);
	ret = ret | AFE49I30_SetRegValue(driver, 0x9E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA0,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA1,0x0002AB);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA2,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA4,0x000400);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA5,0x0002AB);
	ret = ret | AFE49I30_SetRegValue(driver, 0xA6,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE6,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE7,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xE8,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0xF5,0x000000);

	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000001);
	ret = ret | AFE49I30_SetRegValue(driver, 0x20,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x21,0x0074A1);
	ret = ret | AFE49I30_SetRegValue(driver, 0x22,0x02000E);
	ret = ret | AFE49I30_SetRegValue(driver, 0x23,0x0017B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x24,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x25,0x020202);
	ret = ret | AFE49I30_SetRegValue(driver, 0x26,0x0094A1);
	ret = ret | AFE49I30_SetRegValue(driver, 0x27,0x04000E);
	ret = ret | AFE49I30_SetRegValue(driver, 0x28,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x29,0x800404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2A,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2B,0x0074A1);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2C,0x02000E);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2D,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x2F,0x020404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x30,0x0095A1);
	ret = ret | AFE49I30_SetRegValue(driver, 0x31,0x04000E);
	ret = ret | AFE49I30_SetRegValue(driver, 0x32,0x000020);
	ret = ret | AFE49I30_SetRegValue(driver, 0x33,0x800404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x34,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x35,0x007C20);
	ret = ret | AFE49I30_SetRegValue(driver, 0x36,0x020803);
	ret = ret | AFE49I30_SetRegValue(driver, 0x37,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x38,0x050000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x39,0x020202);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3A,0x005C20);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3B,0x040803);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3C,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3D,0x050404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3E,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x3F,0x005C20);
	ret = ret | AFE49I30_SetRegValue(driver, 0x40,0x020803);
	ret = ret | AFE49I30_SetRegValue(driver, 0x41,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x42,0x050000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x43,0x020404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x44,0x005C20);
	ret = ret | AFE49I30_SetRegValue(driver, 0x45,0x040803);
	ret = ret | AFE49I30_SetRegValue(driver, 0x46,0x0014B0);
	ret = ret | AFE49I30_SetRegValue(driver, 0x47,0x050404);
	ret = ret | AFE49I30_SetRegValue(driver, 0x48,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x49,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4D,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x4F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x50,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x51,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x52,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x53,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x54,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x55,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x56,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x57,0x080000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x58,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x59,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5C,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x5F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x60,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x61,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x62,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x63,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x64,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x65,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x66,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x67,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x68,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x69,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6B,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x6F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x70,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x71,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x72,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x73,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x74,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x75,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x76,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x77,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x78,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x79,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7A,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7E,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x7F,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x80,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x81,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x82,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x83,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x84,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x85,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x86,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x87,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x88,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x89,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8A,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8B,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8C,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8D,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8E,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x8F,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x90,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x91,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x92,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x93,0x020000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x94,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x95,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x96,0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, 0x97,0x000000);

	/* Enable FIFO */
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_PAGE_SELECT, 0x000000);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000002);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_FIFO_CONFIG_REG, 0x000040);
	ret = ret | AFE49I30_SetRegValue(driver, AFE49I30_TIMER_ENGINE_REG, 0xC000A0);

	if (ret == AFE49I30_ResultAccept)
	{
		driver->Config.MeasureMode = AFE49I30_PPG_MODE;
		driver->Config.SleepMode = AFE49I30_ACTIVE_MODE;
		driver->Config.SamplingRate = AFE49I30_PPG_SAMPLE_12;
	}
	return ret;
}
