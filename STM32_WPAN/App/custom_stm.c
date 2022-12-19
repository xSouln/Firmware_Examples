/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomTemperature_ServiceHdle;                    /**< TEMPERATURE_SERVICE handle */
  uint16_t  CustomTemperature_ValueHdle;                  /**< TEMPERATURE_VALUE handle */
  uint16_t  CustomAccelerometer_ServiceHdle;                    /**< ACCELEROMETER_SERVICE handle */
  uint16_t  CustomAccelerometer_PointsHdle;                  /**< ACCELEROMETER_POINTS handle */
  uint16_t  CustomGyroscope_ServiceHdle;                    /**< GYROSCOPE_SERVICE handle */
  uint16_t  CustomGyroscope_PointsHdle;                  /**< GYROSCOPE_POINTS handle */
  uint16_t  CustomEck_ServiceHdle;                    /**< ECK_SERVICE handle */
  uint16_t  CustomEcg_PointsHdle;                  /**< ECG_POINTS handle */
  uint16_t  CustomPpg_PointsHdle;                  /**< PPG_POINTS handle */
  uint16_t  CustomEcg_ModeHdle;                  /**< ECG_MODE handle */
  uint16_t  CustomEkg_CmdHdle;                  /**< EKG_CMD handle */
  uint16_t  CustomSystem_ServiceHdle;                    /**< SYSTEM_SERVICE handle */
  uint16_t  CustomBootloader_Port_RxHdle;                  /**< BOOTLOADER_PORT_RX handle */
  uint16_t  CustomBootloader_Port_TxHdle;                  /**< BOOTLOADER_PORT_TX handle */
  uint16_t  CustomBootloader_StatusHdle;                  /**< BOOTLOADER_STATUS handle */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t SizeTemperature_Value = 4;
uint8_t SizeAccelerometer_Points = 48;
uint8_t SizeGyroscope_Points = 48;
uint8_t SizeEcg_Points = 64;
uint8_t SizePpg_Points = 64;
uint8_t SizeEcg_Mode = 8;
uint8_t SizeEkg_Cmd = 1;
uint8_t SizeBootloader_Port_Rx = 200;
uint8_t SizeBootloader_Port_Tx = 200;
uint8_t SizeBootloader_Status = 20;

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_TEMPERATURE_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x2c,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_TEMPERATURE_VALUE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x2c,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_ACCELEROMETER_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x3c,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_ACCELEROMETER_POINTS_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x3c,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_GYROSCOPE_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x4c,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_GYROSCOPE_POINTS_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x4c,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_ECK_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x5c,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_ECG_POINTS_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x5c,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_PPG_POINTS_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x5c,0x02,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_ECG_MODE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x5c,0x03,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_EKG_CMD_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x5c,0x04,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_SYSTEM_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0xb0,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_BOOTLOADER_PORT_RX_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xb0,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_BOOTLOADER_PORT_TX_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xb0,0x02,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_BOOTLOADER_STATUS_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xb0,0x03,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

/* USER CODE BEGIN PF */
//==============================================================================
tBleStatus CustomAppWriteData(Custom_STM_Char_Opcode_t characteristic, uint8_t* data, uint16_t size)
{
	tBleStatus ret = BLE_STATUS_INVALID_PARAMS;

	switch ((uint8_t)characteristic)
	{
		case CUSTOM_STM_TEMPERATURE_VALUE:
			aci_gatt_update_char_value(CustomContext.CustomTemperature_ServiceHdle,
									   CustomContext.CustomTemperature_ValueHdle,
									   0, /* charValOffset */
									   size, /* charValueLen */
									   (uint8_t *)data);
			break;

		case CUSTOM_STM_ACCELEROMETER_POINTS:
			ret = aci_gatt_update_char_value(CustomContext.CustomAccelerometer_ServiceHdle,
											   CustomContext.CustomAccelerometer_PointsHdle,
											   0, /* charValOffset */
											   size, /* charValueLen */
											   (uint8_t *)data);
			break;

		case CUSTOM_STM_GYROSCOPE_POINTS:
		      ret = aci_gatt_update_char_value(CustomContext.CustomGyroscope_ServiceHdle,
		                                       CustomContext.CustomGyroscope_PointsHdle,
		                                       0, /* charValOffset */
											   size, /* charValueLen */
											   (uint8_t *)data);
			break;

		case CUSTOM_STM_ECG_POINTS:
		      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
		                                       CustomContext.CustomEcg_PointsHdle,
		                                       0, /* charValOffset */
											   size, /* charValueLen */
											   (uint8_t *)data);
		      break;

		case CUSTOM_STM_PPG_POINTS:
		      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
		                                       CustomContext.CustomPpg_PointsHdle,
		                                       0, /* charValOffset */
											   size, /* charValueLen */
											   (uint8_t *)data);
		      break;

	}

	return ret;
}
//------------------------------------------------------------------------------
int BLE_BootloaderPortTxSend(uint8_t* data, uint32_t data_size)
{
	return aci_gatt_update_char_value(CustomContext.CustomSystem_ServiceHdle,
									   CustomContext.CustomBootloader_Port_TxHdle,
									   0, /* charValOffset */
									   data_size, /* charValueLen */
									   (uint8_t *)data);
}
//------------------------------------------------------------------------------
int BLE_BootloaderStatusSend(uint8_t* data, uint32_t data_size)
{
	return aci_gatt_update_char_value(CustomContext.CustomSystem_ServiceHdle,
									   CustomContext.CustomBootloader_StatusHdle,
									   0, /* charValOffset */
									   data_size, /* charValueLen */
									   (uint8_t *)data);
}
//==============================================================================
/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  aci_gatt_write_permit_req_event_rp0   *write_perm_req;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */
  aci_gatt_tx_pool_available_event_rp0* pool_available_reg;
  aci_gatt_prepare_write_permit_req_event_rp0* prepare_write_permit_req;
  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch (blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if (attribute_modified->Attr_Handle == (CustomContext.CustomTemperature_ValueHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_1_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_1_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_1_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomTemperature_ValueHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomAccelerometer_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_2_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomAccelerometer_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomGyroscope_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomGyroscope_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomEcg_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_ECG_POINTS_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_ECG_POINTS_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomEcg_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomPpg_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_PPG_POINTS_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_PPG_POINTS_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomPpg_PointsHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomEkg_CmdHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_4 */

            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_4_attribute_modified */

              /* Disabled Indication management */
              case (!(COMSVC_Indication)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_attribute_modified */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_EKG_CMD_INDICATE_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_4_Disabled_END */
                break;

                /* Enabled Indication management */
              case COMSVC_Indication:
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_COMSVC_Indication_BEGIN */

              /* USER CODE END CUSTOM_STM_Service_4_Char_4_COMSVC_Indication_BEGIN */
              Notification.Custom_Evt_Opcode = CUSTOM_STM_EKG_CMD_INDICATE_ENABLED_EVT;
              Custom_STM_App_Notification(&Notification);
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_COMSVC_Indication_END */

              /* USER CODE END CUSTOM_STM_Service_4_Char_4_COMSVC_Indication_END */
              break;

              default:
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_default */

              /* USER CODE END CUSTOM_STM_Service_4_Char_4_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomEKG_CMDHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_Port_TxHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_5_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_5_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_5_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_5_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_5_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_5_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_5_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_Port_TxHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_StatusHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3 */

            /* USER CODE END CUSTOM_STM_Service_5_Char_3 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_5_Char_3_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_5_Char_3_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_5_Char_3_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_5_Char_3_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_5_Char_3_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_default */

                /* USER CODE END CUSTOM_STM_Service_5_Char_3_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_StatusHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomEkg_CmdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            /* USER CODE END CUSTOM_STM_Service_4_Char_4_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomEkg_CmdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_Port_RxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_5_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            /* USER CODE END CUSTOM_STM_Service_5_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomBootloader_Port_RxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if (read_req->Attribute_Handle == (CustomContext.CustomTemperature_ValueHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomTemperature_ValueHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomAccelerometer_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomAccelerometer_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomGyroscope_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_3_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_3_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomGyroscope_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomEcg_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomEcg_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomPpg_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_4_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomPpg_PointsHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomBootloader_Port_TxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_5_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_5_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomBootloader_Port_TxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (read_req->Attribute_Handle == (CustomContext.CustomBootloader_StatusHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_5_Char_3_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_3_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_5_Char_3_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if (read_req->Attribute_Handle == (CustomContext.CustomBootloader_StatusHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;
          if (write_perm_req->Attribute_Handle == (CustomContext.CustomEkg_CmdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* Allow or reject a write request from a client using aci_gatt_write_resp(...) function */
            /*USER CODE BEGIN CUSTOM_STM_Service_4_Char_4_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE */

            /*USER CODE END CUSTOM_STM_Service_4_Char_4_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE*/
          } /*if (write_perm_req->Attribute_Handle == (CustomContext.CustomEkg_CmdHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

          else if (write_perm_req->Attribute_Handle == (CustomContext.CustomBootloader_Port_RxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* Allow or reject a write request from a client using aci_gatt_write_resp(...) function */
            /*USER CODE BEGIN CUSTOM_STM_Service_5_Char_1_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE */
            aci_gatt_write_resp(write_perm_req->Connection_Handle,
                        		write_perm_req->Attribute_Handle,
            					0x00,
            					BLE_STATUS_SUCCESS,
            					write_perm_req->Data_Length,
            					write_perm_req->Data);

			HabibandBootloaderRequestsReceiver(write_perm_req->Data, write_perm_req->Data_Length);
            /*USER CODE END CUSTOM_STM_Service_5_Char_1_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE*/
          } /*if (write_perm_req->Attribute_Handle == (CustomContext.CustomBootloader_Port_RxHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */
        case ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE:
			pool_available_reg = (aci_gatt_tx_pool_available_event_rp0*)blecore_evt->data;

			aci_gatt_tx_pool_available_event(pool_available_reg->Connection_Handle, pool_available_reg->Available_Buffers);

			break;

		case ACI_GATT_PREPARE_WRITE_PERMIT_REQ_VSEVT_CODE:
			prepare_write_permit_req = (aci_gatt_prepare_write_permit_req_event_rp0*)blecore_evt->data;

			aci_gatt_write_resp(prepare_write_permit_req->Connection_Handle,
								prepare_write_permit_req->Attribute_Handle,
								0x01,
								BLE_STATUS_SUCCESS,
								prepare_write_permit_req->Data_Length,
								prepare_write_permit_req->Data);

			break;
        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /**
   *          TEMPERATURE_SERVICE
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for TEMPERATURE_SERVICE +
   *                                2 for TEMPERATURE_VALUE +
   *                                1 for TEMPERATURE_VALUE configuration descriptor +
   *                              = 4
   */

  COPY_TEMPERATURE_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             4,
                             &(CustomContext.CustomTemperature_ServiceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: TEMPERATURE_SERVICE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: TEMPERATURE_SERVICE \n\r");
  }

  /**
   *  TEMPERATURE_VALUE
   */
  COPY_TEMPERATURE_VALUE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomTemperature_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeTemperature_Value,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomTemperature_ValueHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : TEMPERATURE_VALUE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : TEMPERATURE_VALUE \n\r");
  }

  /**
   *          ACCELEROMETER_SERVICE
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for ACCELEROMETER_SERVICE +
   *                                2 for ACCELEROMETER_POINTS +
   *                                1 for ACCELEROMETER_POINTS configuration descriptor +
   *                              = 4
   */

  COPY_ACCELEROMETER_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             4,
                             &(CustomContext.CustomAccelerometer_ServiceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: ACCELEROMETER_SERVICE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: ACCELEROMETER_SERVICE \n\r");
  }

  /**
   *  ACCELEROMETER_POINTS
   */
  COPY_ACCELEROMETER_POINTS_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomAccelerometer_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeAccelerometer_Points,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomAccelerometer_PointsHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : ACCELEROMETER_POINTS, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : ACCELEROMETER_POINTS \n\r");
  }

  /**
   *          GYROSCOPE_SERVICE
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for GYROSCOPE_SERVICE +
   *                                2 for GYROSCOPE_POINTS +
   *                                1 for GYROSCOPE_POINTS configuration descriptor +
   *                              = 4
   */

  COPY_GYROSCOPE_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             4,
                             &(CustomContext.CustomGyroscope_ServiceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: GYROSCOPE_SERVICE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: GYROSCOPE_SERVICE \n\r");
  }

  /**
   *  GYROSCOPE_POINTS
   */
  COPY_GYROSCOPE_POINTS_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomGyroscope_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeGyroscope_Points,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomGyroscope_PointsHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : GYROSCOPE_POINTS, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : GYROSCOPE_POINTS \n\r");
  }

  /**
   *          ECK_SERVICE
   *
   * Max_Attribute_Records = 1 + 2*4 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for ECK_SERVICE +
   *                                2 for ECG_POINTS +
   *                                2 for PPG_POINTS +
   *                                2 for ECG_MODE +
   *                                2 for EKG_CMD +
   *                                1 for ECG_POINTS configuration descriptor +
   *                                1 for PPG_POINTS configuration descriptor +
   *                                1 for EKG_CMD configuration descriptor +
   *                              = 12
   */

  COPY_ECK_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             12,
                             &(CustomContext.CustomEck_ServiceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: ECK_SERVICE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: ECK_SERVICE \n\r");
  }

  /**
   *  ECG_POINTS
   */
  COPY_ECG_POINTS_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomEck_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeEcg_Points,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomEcg_PointsHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : ECG_POINTS, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : ECG_POINTS \n\r");
  }
  /**
   *  PPG_POINTS
   */
  COPY_PPG_POINTS_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomEck_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizePpg_Points,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomPpg_PointsHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : PPG_POINTS, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : PPG_POINTS \n\r");
  }
  /**
   *  ECG_MODE
   */
  COPY_ECG_MODE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomEck_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeEcg_Mode,
                          CHAR_PROP_NONE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomEcg_ModeHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : ECG_MODE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : ECG_MODE \n\r");
  }
  /**
   *  EKG_CMD
   */
  COPY_EKG_CMD_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomEck_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeEkg_Cmd,
                          CHAR_PROP_WRITE | CHAR_PROP_INDICATE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomEkg_CmdHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : EKG_CMD, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : EKG_CMD \n\r");
  }

  /**
   *          SYSTEM_SERVICE
   *
   * Max_Attribute_Records = 1 + 2*3 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for SYSTEM_SERVICE +
   *                                2 for BOOTLOADER_PORT_RX +
   *                                2 for BOOTLOADER_PORT_TX +
   *                                2 for BOOTLOADER_STATUS +
   *                                1 for BOOTLOADER_PORT_TX configuration descriptor +
   *                                1 for BOOTLOADER_STATUS configuration descriptor +
   *                              = 9
   */

  COPY_SYSTEM_SERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             9,
                             &(CustomContext.CustomSystem_ServiceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: SYSTEM_SERVICE, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: SYSTEM_SERVICE \n\r");
  }

  /**
   *  BOOTLOADER_PORT_RX
   */
  COPY_BOOTLOADER_PORT_RX_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomSystem_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeBootloader_Port_Rx,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomBootloader_Port_RxHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : BOOTLOADER_PORT_RX, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : BOOTLOADER_PORT_RX \n\r");
  }
  /**
   *  BOOTLOADER_PORT_TX
   */
  COPY_BOOTLOADER_PORT_TX_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomSystem_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeBootloader_Port_Tx,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_VARIABLE,
                          &(CustomContext.CustomBootloader_Port_TxHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : BOOTLOADER_PORT_TX, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : BOOTLOADER_PORT_TX \n\r");
  }
  /**
   *  BOOTLOADER_STATUS
   */
  COPY_BOOTLOADER_STATUS_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomSystem_ServiceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeBootloader_Status,
                          CHAR_PROP_READ | CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomBootloader_StatusHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : BOOTLOADER_STATUS, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : BOOTLOADER_STATUS \n\r");
  }

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch (CharOpcode)
  {

    case CUSTOM_STM_TEMPERATURE_VALUE:
      ret = aci_gatt_update_char_value(CustomContext.CustomTemperature_ServiceHdle,
                                       CustomContext.CustomTemperature_ValueHdle,
                                       0, /* charValOffset */
                                       SizeTemperature_Value, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value TEMPERATURE_VALUE command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value TEMPERATURE_VALUE command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_1*/
      break;

    case CUSTOM_STM_ACCELEROMETER_POINTS:
      ret = aci_gatt_update_char_value(CustomContext.CustomAccelerometer_ServiceHdle,
                                       CustomContext.CustomAccelerometer_PointsHdle,
                                       0, /* charValOffset */
                                       SizeAccelerometer_Points, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value ACCELEROMETER_POINTS command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value ACCELEROMETER_POINTS command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_1*/
      break;

    case CUSTOM_STM_GYROSCOPE_POINTS:
      ret = aci_gatt_update_char_value(CustomContext.CustomGyroscope_ServiceHdle,
                                       CustomContext.CustomGyroscope_PointsHdle,
                                       0, /* charValOffset */
                                       SizeGyroscope_Points, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value GYROSCOPE_POINTS command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value GYROSCOPE_POINTS command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_1*/
      break;

    case CUSTOM_STM_ECG_POINTS:
      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
                                       CustomContext.CustomEcg_PointsHdle,
                                       0, /* charValOffset */
                                       SizeEcg_Points, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value ECG_POINTS command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value ECG_POINTS command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_1*/
      break;

    case CUSTOM_STM_PPG_POINTS:
      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
                                       CustomContext.CustomPpg_PointsHdle,
                                       0, /* charValOffset */
                                       SizePpg_Points, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value PPG_POINTS command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value PPG_POINTS command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_2*/
      break;

    case CUSTOM_STM_ECG_MODE:
      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
                                       CustomContext.CustomEcg_ModeHdle,
                                       0, /* charValOffset */
                                       SizeEcg_Mode, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value ECG_MODE command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value ECG_MODE command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_3*/
      break;

    case CUSTOM_STM_EKG_CMD:
      ret = aci_gatt_update_char_value(CustomContext.CustomEck_ServiceHdle,
                                       CustomContext.CustomEkg_CmdHdle,
                                       0, /* charValOffset */
                                       SizeEkg_Cmd, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value EKG_CMD command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value EKG_CMD command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_4*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_4*/
      break;

    case CUSTOM_STM_BOOTLOADER_PORT_RX:
      ret = aci_gatt_update_char_value(CustomContext.CustomSystem_ServiceHdle,
                                       CustomContext.CustomBootloader_Port_RxHdle,
                                       0, /* charValOffset */
                                       SizeBootloader_Port_Rx, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value BOOTLOADER_PORT_RX command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value BOOTLOADER_PORT_RX command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_5_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_5_Char_1*/
      break;

    case CUSTOM_STM_BOOTLOADER_PORT_TX:
      ret = aci_gatt_update_char_value(CustomContext.CustomSystem_ServiceHdle,
                                       CustomContext.CustomBootloader_Port_TxHdle,
                                       0, /* charValOffset */
                                       SizeBootloader_Port_Tx, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value BOOTLOADER_PORT_TX command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value BOOTLOADER_PORT_TX command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_5_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_5_Char_2*/
      break;

    case CUSTOM_STM_BOOTLOADER_STATUS:
      ret = aci_gatt_update_char_value(CustomContext.CustomSystem_ServiceHdle,
                                       CustomContext.CustomBootloader_StatusHdle,
                                       0, /* charValOffset */
                                       SizeBootloader_Status, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value BOOTLOADER_STATUS command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value BOOTLOADER_STATUS command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_5_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_5_Char_3*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return ret;
}
