/*
 * led.c
 *
 *  Created on: Jun 12, 2025
 *      Author: Daniel
 */

#include "led.h"

uint16_t LedBuffer[BUFFER_COUNT][BUFFER_SIZE];
bool BufferSelect = 0;

void Led_Init() {
	// Init LED buffer (turn off all LEDs by default))
	uint16_t reg = GPIOA->ODR | 0x1FFF;
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[0][i] = reg;
	}
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[1][i] = reg;
	}

	HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, 1); // Turn on 3.3V
}

void Led_Fill_Buffer(uint16_t color) {
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[0][i] = (GPIOA->ODR & ~0x1FFF) | (color & 0x1FFF); // Write back only PA0–PA12
		LedBuffer[1][i] = (GPIOA->ODR & ~0x1FFF) | (color & 0x1FFF); // Write back only PA0–PA12
	}
}

// Macros for buffer generation
#define ANODE_PIN(row)     (1U << (row))
#define CATHODE_PIN(col)   (1U << ((col) + LED_ROWS))

void Led_Generate_Buffer(uint8_t frame[LED_CNT]) {
	// Delete buffer (keep upper bits)
	// LED anode, cathode HIGH -> Turn OFF
	uint16_t reg = GPIOA->ODR | 0x1FFF;
	for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
		LedBuffer[BufferSelect][i] = reg;
	}

	// Generate buffer
	for (uint16_t row = 0; row < LED_ROWS; row++) { // Select row
		for (uint16_t led = 0; led < LED_NUM; led++) { // Select LED

			// PWM number between 0-255
//			uint8_t pwm = gamma_lut[frame[row * LED_NUM + led]];
			uint8_t pwm = frame[row * LED_NUM + led];
			if (pwm > LED_PWM_MAX) {
				pwm = LED_PWM_MAX;
			}

			// Set LED values
			for (uint16_t n = 0; n < pwm; n++) {
				LedBuffer[BufferSelect][row * LED_PWM_MAX + n] &= ~ANODE_PIN(
						row); // LED anode
				LedBuffer[BufferSelect][row * LED_PWM_MAX + n] &= ~CATHODE_PIN(
						led); // LED cathode
			}
		}
	}

	return;
}

