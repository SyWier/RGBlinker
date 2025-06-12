/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern bool pwr_go_sleep;

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
#define DEBUG_UART_TX_Pin GPIO_PIN_14
#define DEBUG_UART_TX_GPIO_Port GPIOC
#define LEDA0_Pin GPIO_PIN_0
#define LEDA0_GPIO_Port GPIOA
#define LEDA1_Pin GPIO_PIN_1
#define LEDA1_GPIO_Port GPIOA
#define LEDA2_Pin GPIO_PIN_2
#define LEDA2_GPIO_Port GPIOA
#define LEDA3_Pin GPIO_PIN_3
#define LEDA3_GPIO_Port GPIOA
#define LEDK0_Pin GPIO_PIN_4
#define LEDK0_GPIO_Port GPIOA
#define LEDK1_Pin GPIO_PIN_5
#define LEDK1_GPIO_Port GPIOA
#define LEDK2_Pin GPIO_PIN_6
#define LEDK2_GPIO_Port GPIOA
#define LEDK3_Pin GPIO_PIN_7
#define LEDK3_GPIO_Port GPIOA
#define DEBUG_UART_RX_Pin GPIO_PIN_2
#define DEBUG_UART_RX_GPIO_Port GPIOB
#define LEDK4_Pin GPIO_PIN_8
#define LEDK4_GPIO_Port GPIOA
#define LEDK5_Pin GPIO_PIN_9
#define LEDK5_GPIO_Port GPIOA
#define LEDK6_Pin GPIO_PIN_10
#define LEDK6_GPIO_Port GPIOA
#define LEDK7_Pin GPIO_PIN_11
#define LEDK7_GPIO_Port GPIOA
#define LEDK8_Pin GPIO_PIN_12
#define LEDK8_GPIO_Port GPIOA
#define EN_3V3_Pin GPIO_PIN_3
#define EN_3V3_GPIO_Port GPIOB
#define BTN_Pin GPIO_PIN_6
#define BTN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
