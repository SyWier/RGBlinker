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

int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (const uint8_t*) ptr, (uint16_t) len,
			HAL_MAX_DELAY);
	return HAL_OK;
}

#define FRAME_COUNT 6

uint8_t LedFrame[FRAME_COUNT][LED_CNT] = {
//		{ 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
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
				31, 15, 0, 0, 0, 15, 0, 0, 0, 15, 7, 0, 0, 0, 7, 0, 0, 0, 7 } };

uint32_t adc_value = 0;
bool adc_flag = 0;
uint16_t pwmCnt = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
		GPIOA->ODR = LedBuffer[!BufferSelect][pwmCnt];
		pwmCnt++;
		if (pwmCnt >= BUFFER_SIZE) {
			pwmCnt = 0;
		}
	}

	if (htim->Instance == TIM14) {
		adc_value = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Start(&hadc1);
		adc_flag = 1;
	}
}

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 1.50 steps = 256 range = 0-255
const uint8_t gamma_lut[256] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,
		4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14,
		14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 21, 22, 23, 23, 24, 25,
		26, 26, 27, 28, 28, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38,
		39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53,
		54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70,
		71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89,
		90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106,
		107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122,
		123, 124, 126, 127, 128, 129, 130, 132, 133, 134, 135, 136, 138, 139,
		140, 141, 142, 144, 145, 146, 147, 149, 150, 151, 152, 154, 155, 156,
		158, 159, 160, 161, 163, 164, 165, 167, 168, 169, 171, 172, 173, 174,
		176, 177, 178, 180, 181, 182, 184, 185, 187, 188, 189, 191, 192, 193,
		195, 196, 197, 199, 200, 202, 203, 204, 206, 207, 209, 210, 211, 213,
		214, 216, 217, 218, 220, 221, 223, 224, 226, 227, 228, 230, 231, 233,
		234, 236, 237, 239, 240, 242, 243, 245, 246, 248, 249, 251, 252, 254,
		255, };

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*
 typedef enum {
 LOAD,
 ADD,
 REPEAT,
 RSHIFT,
 LSHIFT
 } LED_OPCODE_E;

 typedef struct {
 uint8_t red;
 uint8_t green;
 uint8_t blue;
 } LedRgb_t;
 typedef struct {
 uint16_t frameTime;
 LedRgb_t brightness[ROWS][COLS];
 } LedFrame_t;
 typedef struct {
 LED_OPCODE_E opcode;
 LedFrame_t frame;
 } LedAnimation_t;

 LedAnimation_t animation[] = {
 LOAD, { 100, { { {255, 0, 0}, {0, 255, 0}, {0, 0, 255} },
 { {255, 0, 0}, {0, 255, 0}, {0, 0, 255} },
 { {255, 0, 0}, {0, 255, 0}, {0, 0, 255} },
 { {255, 0, 0}, {0, 255, 0}, {0, 0, 255} } } },
 REPEAT,
 };

 int leds[12];

 int instCnt = 0;
 int instMax = 3;
 typedef void (*instructions_t)(void);
 void load() {
 leds = (int){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
 }
 void add() {
 for(int i = 0; i < 12; i++) {

 }
 }
 void repeat() {
 if(instCnt+1 == instMax)
 return;

 for(int i = 0; i < 5; i++)
 instuctions[instCnt+1]();
 }

 instructions_t instuctions[] = {
 load, repeat, add
 };
 */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool pwr_go_sleep = 0;

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
  MX_TIM14_Init();
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
	HAL_TIM_Base_Start_IT(&htim14);

	Battery_Gauge();

	Led_Fill_Buffer(0xEEE);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	/* Infinite loop */
	Log_Info("Start main...");

	int mainCnt = 0;
	while (1) {
//	  printf(ESCAPE_WHITE "Main cnt: %d\r\n", mainCnt);

#if 0
		Led_Generate_Buffer(LedFrame[mainCnt]);
		BufferSelect = !BufferSelect;

		mainCnt++;
		if (mainCnt >= FRAME_COUNT) {
			mainCnt = 0;
		}
#endif

		HAL_Delay(100);

		if (adc_flag) {
			adc_flag = 0;
			Battery_Print();
		}
		if(pwr_go_sleep) {

		}

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
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
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
while (1) {
}
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
