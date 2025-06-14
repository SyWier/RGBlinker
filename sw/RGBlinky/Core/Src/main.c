/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

#define HW_VERSION "hw-v1.0"
#define SW_VERSION "sw-v0.3"

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "log.h"
#include "battery.h"
#include "led.h"
#include "button_handler.h"

// Redirect printf to UART
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (const uint8_t*) ptr, (uint16_t) len, HAL_MAX_DELAY);
	return HAL_OK;
}

#define FRAME_TEST 0
#if FRAME_TEST
#define FRAME_COUNT 6
uint8_t LedFrame[FRAME_COUNT][LED_CNT] = {
//  { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
	{ 31, 0, 0, 0, 31, 0, 0, 0, 31, 15, 0, 0, 0, 15, 0, 0, 0, 15, 7, 0, 0,
	0, 7, 0, 0, 0, 7, 1, 0, 0, 0, 1, 0, 0, 0, 1 }, { 15, 0, 0, 0,
	15, 0, 0, 0, 15, 7, 0, 0, 0, 7, 0, 0, 0, 7, 1, 0, 0, 0, 1, 0, 0,
	0, 1, 7, 0, 0, 0, 7, 0, 0, 0, 7 }, { 7, 0, 0, 0, 7, 0, 0, 0, 7,
	1, 0, 0, 0, 1, 0, 0, 0, 1, 7, 0, 0, 0, 7, 0, 0, 0, 7, 15, 0, 0,
	0, 15, 0, 0, 0, 15 }, { 1, 0, 0, 0, 1, 0, 0, 0, 1, 7, 0, 0, 0,
	7, 0, 0, 0, 7, 15, 0, 0, 0, 15, 0, 0, 0, 15, 31, 0, 0, 0, 31, 0,
	0, 0, 31 }, { 7, 0, 0, 0, 7, 0, 0, 0, 7, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 31, 0, 0, 0, 31, 0, 0, 0, 31, 15, 0, 0, 0, 15, 0, 0, 0,
	15 }, { 15, 0, 0, 0, 15, 0, 0, 0, 15, 31, 0, 0, 0, 31, 0, 0, 0,
	31, 15, 0, 0, 0, 15, 0, 0, 0, 15, 7, 0, 0, 0, 7, 0, 0, 0, 7 }
};
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

	/* Build Print Build Time */
	Log_Important("Build time: " __DATE__ " " __TIME__);
	Log_Important("Hardware version: " HW_VERSION);
	Log_Important("Software version: " SW_VERSION);

//	Log_Error("Reset status: %x", RCC->CSR2);

	// Check if we were in standby mode
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_WUF3)) {
		Log_Info("Device waked up.");
	} else {
		Log_Info("Device powered up.");
	}
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);

	/* Init components */
	Led_Init();
	Battery_Init();
	Button_Init(&userBtn);

	/* Start timers */
	HAL_TIM_Base_Start_IT(&htim3);

	Battery_Gauge();

	Led_Fill_Buffer(0xEEE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	/* Infinite loop */
	Log_Info("Start main...");

	while (1) {
		Log_Debug("a");

#if FRAME_TEST
		for(int i = 0; i < FRAME_COUNT; i++)  {
			Led_Generate_Buffer(LedFrame[mainCnt]);
			BufferSelect = !BufferSelect;

			mainCnt++;
			if (mainCnt >= FRAME_COUNT) {
				mainCnt = 0;
			}
			HAL_Delay(100);
		}
#endif

		Battery_Print();
		HAL_Delay(5000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
