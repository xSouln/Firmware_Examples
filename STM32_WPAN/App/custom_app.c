/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
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
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Components.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* TEMPERATURE_SERVICE */
  uint8_t               Temperature_value_Notification_Status;
  /* ACCELEROMETER_SERVICE */
  uint8_t               Accelerometer_points_Notification_Status;
  /* GYROSCOPE_SERVICE */
  uint8_t               Gyroscope_points_Notification_Status;
  /* ECK_SERVICE */
  uint8_t               Ecg_points_Notification_Status;
  uint8_t               Ppg_points_Notification_Status;
  uint8_t               Ekg_cmd_Indication_Status;
  /* SYSTEM_SERVICE */
  uint8_t               Bootloader_port_tx_Notification_Status;
  uint8_t               Bootloader_status_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* TEMPERATURE_SERVICE */
static void Custom_Temperature_value_Update_Char(void);
static void Custom_Temperature_value_Send_Notification(void);
/* ACCELEROMETER_SERVICE */
static void Custom_Accelerometer_points_Update_Char(void);
static void Custom_Accelerometer_points_Send_Notification(void);
/* GYROSCOPE_SERVICE */
static void Custom_Gyroscope_points_Update_Char(void);
static void Custom_Gyroscope_points_Send_Notification(void);
/* ECK_SERVICE */
static void Custom_Ecg_points_Update_Char(void);
static void Custom_Ecg_points_Send_Notification(void);
static void Custom_Ppg_points_Update_Char(void);
static void Custom_Ppg_points_Send_Notification(void);
static void Custom_Ekg_cmd_Update_Char(void);
static void Custom_Ekg_cmd_Send_Indication(void);
/* SYSTEM_SERVICE */
static void Custom_Bootloader_port_tx_Update_Char(void);
static void Custom_Bootloader_port_tx_Send_Notification(void);
static void Custom_Bootloader_status_Update_Char(void);
static void Custom_Bootloader_status_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* TEMPERATURE_SERVICE */
    case CUSTOM_STM_TEMPERATURE_VALUE_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TEMPERATURE_VALUE_READ_EVT */

    	//Habiband.Interface->EventListener(&Habiband, HabibandEventBLE_ConnectionStateChanged, );
      /* USER CODE END CUSTOM_STM_TEMPERATURE_VALUE_READ_EVT */
      break;

    case CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_ENABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_TemperatureNotificationEnnabled, 0, 0);
      /* USER CODE END CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_DISABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_TemperatureNotificationDisabled, 0, 0);
      /* USER CODE END CUSTOM_STM_TEMPERATURE_VALUE_NOTIFY_DISABLED_EVT */
      break;

    /* ACCELEROMETER_SERVICE */
    case CUSTOM_STM_ACCELEROMETER_POINTS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACCELEROMETER_POINTS_READ_EVT */

      /* USER CODE END CUSTOM_STM_ACCELEROMETER_POINTS_READ_EVT */
      break;

    case CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_ENABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_AccelerometerNotificationEnnabled, 0, 0);
      /* USER CODE END CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_DISABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_AccelerometerNotificationDisabled, 0, 0);
      /* USER CODE END CUSTOM_STM_ACCELEROMETER_POINTS_NOTIFY_DISABLED_EVT */
      break;

    /* GYROSCOPE_SERVICE */
    case CUSTOM_STM_GYROSCOPE_POINTS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GYROSCOPE_POINTS_READ_EVT */

      /* USER CODE END CUSTOM_STM_GYROSCOPE_POINTS_READ_EVT */
      break;

    case CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_ENABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_GyroscopeNotificationEnnabled, 0, 0);
      /* USER CODE END CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_DISABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_GyroscopeNotificationDisabled, 0, 0);
      /* USER CODE END CUSTOM_STM_GYROSCOPE_POINTS_NOTIFY_DISABLED_EVT */
      break;

    /* ECK_SERVICE */
    case CUSTOM_STM_ECG_POINTS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ECG_POINTS_READ_EVT */

      /* USER CODE END CUSTOM_STM_ECG_POINTS_READ_EVT */
      break;

    case CUSTOM_STM_ECG_POINTS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ECG_POINTS_NOTIFY_ENABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_ECG_PointsNotificationEnnabled, 0, 0);
      /* USER CODE END CUSTOM_STM_ECG_POINTS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_ECG_POINTS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_ECG_POINTS_NOTIFY_DISABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_ECG_PointsNotificationDisabled, 0, 0);
      /* USER CODE END CUSTOM_STM_ECG_POINTS_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_PPG_POINTS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PPG_POINTS_READ_EVT */

      /* USER CODE END CUSTOM_STM_PPG_POINTS_READ_EVT */
      break;

    case CUSTOM_STM_PPG_POINTS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PPG_POINTS_NOTIFY_ENABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_PPG_PointsNotificationEnnabled, 0, 0);
      /* USER CODE END CUSTOM_STM_PPG_POINTS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PPG_POINTS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PPG_POINTS_NOTIFY_DISABLED_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_PPG_PointsNotificationDisabled, 0, 0);
      /* USER CODE END CUSTOM_STM_PPG_POINTS_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_EKG_CMD_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EKG_CMD_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_EKG_CMD_WRITE_EVT */
      break;

    case CUSTOM_STM_EKG_CMD_INDICATE_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EKG_CMD_INDICATE_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_EKG_CMD_INDICATE_ENABLED_EVT */
      break;

    case CUSTOM_STM_EKG_CMD_INDICATE_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_EKG_CMD_INDICATE_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_EKG_CMD_INDICATE_DISABLED_EVT */
      break;

    /* SYSTEM_SERVICE */
    case CUSTOM_STM_BOOTLOADER_PORT_RX_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_PORT_RX_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_BOOTLOADER_PORT_RX_WRITE_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_PORT_TX_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_PORT_TX_READ_EVT */

      /* USER CODE END CUSTOM_STM_BOOTLOADER_PORT_TX_READ_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_ENABLED_EVT */
    	HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_NotificationTxEnableEvent, 0, 0);
      /* USER CODE END CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_DISABLED_EVT */
    	HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_NotificationTxDisableEvent, 0, 0);
      /* USER CODE END CUSTOM_STM_BOOTLOADER_PORT_TX_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_STATUS_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_STATUS_READ_EVT */

      /* USER CODE END CUSTOM_STM_BOOTLOADER_STATUS_READ_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_ENABLED_EVT */
    	HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_NotificationStatusEnableEvent, 0, 0);
      /* USER CODE END CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_DISABLED_EVT */
    	HabibandBootloaderBLE_EventListener(HabibandBootloaderBLE_NotificationStatusDisableEvent, 0, 0);
      /* USER CODE END CUSTOM_STM_BOOTLOADER_STATUS_NOTIFY_DISABLED_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_Connected, 0, 0);
      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */
    	HabibandEventListener(&Habiband, HabibandEventBLE_Disconnected, 0, 0);
      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* TEMPERATURE_SERVICE */
void Custom_Temperature_value_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Temperature_value_UC_1*/

  /* USER CODE END Temperature_value_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TEMPERATURE_VALUE, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Temperature_value_UC_Last*/

  /* USER CODE END Temperature_value_UC_Last*/
  return;
}

void Custom_Temperature_value_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Temperature_value_NS_1*/

  /* USER CODE END Temperature_value_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_TEMPERATURE_VALUE, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Temperature_value_NS_Last*/

  /* USER CODE END Temperature_value_NS_Last*/

  return;
}

/* ACCELEROMETER_SERVICE */
void Custom_Accelerometer_points_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Accelerometer_points_UC_1*/

  /* USER CODE END Accelerometer_points_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ACCELEROMETER_POINTS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Accelerometer_points_UC_Last*/

  /* USER CODE END Accelerometer_points_UC_Last*/
  return;
}

void Custom_Accelerometer_points_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Accelerometer_points_NS_1*/

  /* USER CODE END Accelerometer_points_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ACCELEROMETER_POINTS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Accelerometer_points_NS_Last*/

  /* USER CODE END Accelerometer_points_NS_Last*/

  return;
}

/* GYROSCOPE_SERVICE */
void Custom_Gyroscope_points_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Gyroscope_points_UC_1*/

  /* USER CODE END Gyroscope_points_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GYROSCOPE_POINTS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Gyroscope_points_UC_Last*/

  /* USER CODE END Gyroscope_points_UC_Last*/
  return;
}

void Custom_Gyroscope_points_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Gyroscope_points_NS_1*/

  /* USER CODE END Gyroscope_points_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_GYROSCOPE_POINTS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Gyroscope_points_NS_Last*/

  /* USER CODE END Gyroscope_points_NS_Last*/

  return;
}

/* ECK_SERVICE */
void Custom_Ecg_points_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ecg_points_UC_1*/

  /* USER CODE END Ecg_points_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ECG_POINTS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ecg_points_UC_Last*/

  /* USER CODE END Ecg_points_UC_Last*/
  return;
}

void Custom_Ecg_points_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ecg_points_NS_1*/

  /* USER CODE END Ecg_points_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_ECG_POINTS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ecg_points_NS_Last*/

  /* USER CODE END Ecg_points_NS_Last*/

  return;
}

void Custom_Ppg_points_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ppg_points_UC_1*/

  /* USER CODE END Ppg_points_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PPG_POINTS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ppg_points_UC_Last*/

  /* USER CODE END Ppg_points_UC_Last*/
  return;
}

void Custom_Ppg_points_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ppg_points_NS_1*/

  /* USER CODE END Ppg_points_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PPG_POINTS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ppg_points_NS_Last*/

  /* USER CODE END Ppg_points_NS_Last*/

  return;
}

void Custom_Ekg_cmd_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ekg_cmd_UC_1*/

  /* USER CODE END Ekg_cmd_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EKG_CMD, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Ekg_cmd_UC_Last*/

  /* USER CODE END Ekg_cmd_UC_Last*/
  return;
}

void Custom_Ekg_cmd_Send_Indication(void) /* Property Indication */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Ekg_cmd_IS_1*/

  /* USER CODE END Ekg_cmd_IS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_EKG_CMD, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Ekg_cmd_IS_Last*/

  /* USER CODE END Ekg_cmd_IS_Last*/

  return;
}

/* SYSTEM_SERVICE */
void Custom_Bootloader_port_tx_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bootloader_port_tx_UC_1*/

  /* USER CODE END Bootloader_port_tx_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BOOTLOADER_PORT_TX, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bootloader_port_tx_UC_Last*/

  /* USER CODE END Bootloader_port_tx_UC_Last*/
  return;
}

void Custom_Bootloader_port_tx_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bootloader_port_tx_NS_1*/

  /* USER CODE END Bootloader_port_tx_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BOOTLOADER_PORT_TX, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bootloader_port_tx_NS_Last*/

  /* USER CODE END Bootloader_port_tx_NS_Last*/

  return;
}

void Custom_Bootloader_status_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bootloader_status_UC_1*/

  /* USER CODE END Bootloader_status_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BOOTLOADER_STATUS, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Bootloader_status_UC_Last*/

  /* USER CODE END Bootloader_status_UC_Last*/
  return;
}

void Custom_Bootloader_status_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Bootloader_status_NS_1*/

  /* USER CODE END Bootloader_status_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_BOOTLOADER_STATUS, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Bootloader_status_NS_Last*/

  /* USER CODE END Bootloader_status_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
