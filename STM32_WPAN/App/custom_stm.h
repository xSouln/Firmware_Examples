/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.h
  * @author  MCD Application Team
  * @brief   Header for custom_stm.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_STM_H
#define __CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ble_types.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* TEMPERATURE_SERVICE */
  CUSTOM_STM_TEMPERATURE_VALUE,
  /* ACCELEROMETER_SERVICE */
  CUSTOM_STM_ACCELEROMETER_POINTS,
  /* GYROSCOPE_SERVICE */
  CUSTOM_STM_GYROSCOPE_POINTS,
  /* ECK_SERVICE */
  CUSTOM_STM_ECG_POINTS,
  CUSTOM_STM_PPG_POINTS,
  CUSTOM_STM_ECG_MODE,
  CUSTOM_STM_EKG_CMD,
  /* SYSTEM_SERVICE */
  CUSTOM_STM_BOOTLOADER_PORT_RX,
  CUSTOM_STM_BOOTLOADER_PORT_TX,
  CUSTOM_STM_BOOTLOADER_STATUS,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* TEMPERATURE_VALUE */
  CUSTOM_STM_TEMPERATURE_VALUE_READ_EVT,
  CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_DISABLED_EVT,
  /* ACCELEROMETER_POINTS */
  CUSTOM_STM_ACCELEROMETER_POINTS_READ_EVT,
  CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_DISABLED_EVT,
  /* GYROSCOPE_POINTS */
  CUSTOM_STM_GYROSCOPE_POINTS_READ_EVT,
  CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_DISABLED_EVT,
  /* ECG_POINTS */
  CUSTOM_STM_ECG_POINTS_READ_EVT,
  CUSTOM_STM_ECG_POINTS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_ECG_POINTS_NOTIFY_DISABLED_EVT,
  /* PPG_POINTS */
  CUSTOM_STM_PPG_POINTS_READ_EVT,
  CUSTOM_STM_PPG_POINTS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_PPG_POINTS_NOTIFY_DISABLED_EVT,
  /* ECG_MODE */
  /* EKG_CMD */
  CUSTOM_STM_EKG_CMD_WRITE_EVT,
  CUSTOM_STM_EKG_CMD_INDICATE_ENABLED_EVT,
  CUSTOM_STM_EKG_CMD_INDICATE_DISABLED_EVT,
  /* BOOTLOADER_PORT_RX */
  CUSTOM_STM_BOOTLOADER_PORT_RX_WRITE_EVT,
  /* BOOTLOADER_PORT_TX */
  CUSTOM_STM_BOOTLOADER_PORT_TX_READ_EVT,
  CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_DISABLED_EVT,
  /* BOOTLOADER_STATUS */
  CUSTOM_STM_BOOTLOADER_STATUS_READ_EVT,
  CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_DISABLED_EVT,

  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t       Custom_Evt_Opcode;
  Custom_STM_Data_t             DataTransfered;
  uint16_t                      ConnectionHandle;
  uint8_t                       ServiceInstance;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
extern uint8_t SizeTemperature_Value;
extern uint8_t SizeAccelerometer_Points;
extern uint8_t SizeGyroscope_Points;
extern uint8_t SizeEcg_Points;
extern uint8_t SizePpg_Points;
extern uint8_t SizeEcg_Mode;
extern uint8_t SizeEkg_Cmd;
extern uint8_t SizeBootloader_Port_Rx;
extern uint8_t SizeBootloader_Port_Tx;
extern uint8_t SizeBootloader_Status;

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,  uint8_t *pPayload);
/* USER CODE BEGIN EF */
tBleStatus CustomAppWriteData(Custom_STM_Char_Opcode_t characteristic, uint8_t* data, uint16_t size);
/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*__CUSTOM_STM_H */
