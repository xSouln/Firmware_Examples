/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BODY_TEMP_GPIO1_Pin GPIO_PIN_13
#define BODY_TEMP_GPIO1_GPIO_Port GPIOC
#define NTC_ADC_Pin GPIO_PIN_2
#define NTC_ADC_GPIO_Port GPIOC
#define ADP_INT_Pin GPIO_PIN_0
#define ADP_INT_GPIO_Port GPIOA
#define MOTOR_PWM_Pin GPIO_PIN_1
#define MOTOR_PWM_GPIO_Port GPIOA
#define AFE_INT_Pin GPIO_PIN_2
#define AFE_INT_GPIO_Port GPIOA
#define MODULE_UART2_RX_Pin GPIO_PIN_3
#define MODULE_UART2_RX_GPIO_Port GPIOA
#define MR_CONTROL_Pin GPIO_PIN_4
#define MR_CONTROL_GPIO_Port GPIOA
#define ADP_STOP_SWITCHING_Pin GPIO_PIN_4
#define ADP_STOP_SWITCHING_GPIO_Port GPIOC
#define TOUCH_INDICATION_STM_Pin GPIO_PIN_5
#define TOUCH_INDICATION_STM_GPIO_Port GPIOC
#define TOUCH_INDICATION_STM_EXTI_IRQn EXTI9_5_IRQn
#define AFE_RESET_N_Pin GPIO_PIN_2
#define AFE_RESET_N_GPIO_Port GPIOB
#define MOTOR_RESET_N_Pin GPIO_PIN_0
#define MOTOR_RESET_N_GPIO_Port GPIOB
#define MODULE_WAKEUP_Pin GPIO_PIN_1
#define MODULE_WAKEUP_GPIO_Port GPIOB
#define BODY_TEMP_GPIO0_Pin GPIO_PIN_4
#define BODY_TEMP_GPIO0_GPIO_Port GPIOE
#define BODY_TEMP_GPIO0_EXTI_IRQn EXTI4_IRQn
#define MODULE_UART2_RTS_Pin GPIO_PIN_12
#define MODULE_UART2_RTS_GPIO_Port GPIOB
#define MODULE_UART2_CTS_Pin GPIO_PIN_13
#define MODULE_UART2_CTS_GPIO_Port GPIOB
#define ACCEL_GYR_INT2_Pin GPIO_PIN_14
#define ACCEL_GYR_INT2_GPIO_Port GPIOB
#define ACCEL_GYR_INT2_EXTI_IRQn EXTI15_10_IRQn
#define ADP_PGOOD2_OUT_Pin GPIO_PIN_15
#define ADP_PGOOD2_OUT_GPIO_Port GPIOB
#define ADP_PGOOD2_OUT_EXTI_IRQn EXTI15_10_IRQn
#define SE_RESET_N_Pin GPIO_PIN_6
#define SE_RESET_N_GPIO_Port GPIOC
#define LTE_UART_RX_Pin GPIO_PIN_10
#define LTE_UART_RX_GPIO_Port GPIOA
#define LTE_UART_CTS_Pin GPIO_PIN_11
#define LTE_UART_CTS_GPIO_Port GPIOA
#define LTE_UART_RTS_Pin GPIO_PIN_12
#define LTE_UART_RTS_GPIO_Port GPIOA
#define MODULE_RST_STS_Pin GPIO_PIN_10
#define MODULE_RST_STS_GPIO_Port GPIOC
#define AFE_ADC_RDY_Pin GPIO_PIN_11
#define AFE_ADC_RDY_GPIO_Port GPIOC
#define AFE_ADC_RDY_EXTI_IRQn EXTI15_10_IRQn
#define STM_WAKEUP_Pin GPIO_PIN_12
#define STM_WAKEUP_GPIO_Port GPIOC
#define MODULE_RESET_N_Pin GPIO_PIN_0
#define MODULE_RESET_N_GPIO_Port GPIOD
#define ACCEL_GYR_INT1_Pin GPIO_PIN_1
#define ACCEL_GYR_INT1_GPIO_Port GPIOD
#define ACCEL_GYR_INT1_EXTI_IRQn EXTI1_IRQn
#define RGB_DRIVER_ENABLE_Pin GPIO_PIN_4
#define RGB_DRIVER_ENABLE_GPIO_Port GPIOB
#define MODULE_UART2_TX_Pin GPIO_PIN_5
#define MODULE_UART2_TX_GPIO_Port GPIOB
#define LTE_UART_TX_Pin GPIO_PIN_6
#define LTE_UART_TX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
