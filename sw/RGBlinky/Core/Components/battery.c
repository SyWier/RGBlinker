/*
 * battery.c
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

#include "battery.h"
#include "log.h"
#include "stdio.h"
#include "adc.h"
#include "led.h"
#include "stm32c0xx_hal.h"

void Battery_Init() {
	// Start  and initialize ADC conversion
	if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
		Error_Handler(); /* Calibration Error */
	}

	// Start and wait for one conversion
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint16_t millivolts = Battery_Get();

	// Print battery voltage with no load
	Log_Info("VDDA: %dmV", millivolts);
	Log_Info("Percentage: %d%%", Battery_Percent(millivolts));
}

uint16_t Battery_Get() {
	uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Start(&hadc1); // Start a new single conversion for next read
	uint16_t vdda = (uint16_t) ((*VREFINT_CAL_ADDR) * VREFINT_CAL_VREF / (float) adc_value);

	return vdda;

}

uint8_t Battery_Percent(uint16_t millivolts) {
    if (millivolts >= 3000) return 100;
    else if (millivolts >= 2900) return 85 + (millivolts - 2900) * 15 / 100;
    else if (millivolts >= 2800) return 60 + (millivolts - 2800) * 25 / 100;
    else if (millivolts >= 2700) return 30 + (millivolts - 2700) * 30 / 100;
    else if (millivolts >= 2600) return 10 + (millivolts - 2600) * 20 / 100;
    else if (millivolts >= 2500) return (millivolts - 2500) * 10 / 100;
    else return 0;
}

void Battery_Print() {
	uint16_t millivolts = Battery_Get();
	Log_Debug("VDDA: %dmV", millivolts);
	Log_Debug("Percentage: %d%%", Battery_Percent(millivolts));
}

void Battery_Gauge() {
	uint8_t BatteryFrame[5][LED_CNT] = { // BGR
		// >= 0%
		{ RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		  RGB(0, 0, 0), RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 0, 0),
		  RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) },
		// >= 20%
		{ RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		  RGB(0, 0, 0), RGB(255, 64, 0), RGB(255, 0, 0), RGB(255, 64, 0),
		  RGB(0, 0, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) },
		// >= 40%
		{ RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
		  RGB(255, 255, 0), RGB(255, 64, 0), RGB(255, 0, 0), RGB(255, 64, 0),
		  RGB(255, 255, 0),RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) },
	    // >= 60%
		{ RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(64, 255, 0),
		  RGB(255, 255, 0), RGB(255, 64, 0), RGB(255, 0, 0), RGB(255, 64, 0),
		  RGB(255, 255, 0),RGB(64, 255, 0), RGB(0, 0, 0), RGB(0, 0, 0) },
		// >= 80%
		{ RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 255, 0), RGB(64, 255, 0),
		  RGB(255, 255, 0), RGB(255, 64, 0), RGB(255, 0, 0), RGB(255, 64, 0),
		  RGB(255, 255, 0),RGB(64, 255, 0), RGB(0, 255, 0), RGB(0, 0, 0) },
	};

	uint16_t millivolts = Battery_Get();
	uint8_t batteryPercent = Battery_Percent(millivolts) / 20; // 0...100% -> 0...5
	if (batteryPercent > 4) {
		batteryPercent = 4;
	}

	for(int i = 0; i <= batteryPercent; i++) {
		Led_Generate_Buffer(BatteryFrame[i]);
		HAL_Delay(125);
	}

	HAL_Delay(1500);
}

void PowerOff() {
	Log_Important("Power off...zzz...");

	// Pull down 3.3V ernable pin
	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_B, PWR_GPIO_BIT_3);
	HAL_PWREx_EnablePullUpPullDownConfig();

	// Enable wake up on button rising edge
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN3_HIGH);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);

	//Shutdown
	HAL_PWREx_EnterSHUTDOWNMode();
}



